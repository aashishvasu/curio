#pragma once

#include "Array.h"
#include "Memory/Memory.h"
#include "Utils/Hash.h"
#include "Common.h"

namespace Core
{
	static constexpr float MAX_LOAD_FACTOR = 0.75f;
	static constexpr float TOMBSTONE_RATIO = 0.25f;
	static constexpr size_t INITIAL_SLOT_COUNT = 16;
	
	template <typename KeyType, typename ValueType>
	class CURIO_API CuDict
	{
	public:
		CuDict();
		explicit CuDict(IAllocator& inAllocator);
		CuDict(const CuDict& Other);
		CuDict(CuDict&& Other) noexcept;
		~CuDict();

		CuDict& operator=(const CuDict& Other);
		CuDict& operator=(CuDict&& Other) noexcept;

		// Element ops
		void Add(const KeyType& Key, const ValueType& Value);
		void Add(KeyType&& Key, ValueType&& Value);

		void Remove(const KeyType& Key);
		void Clear();

		// Accessors
		ValueType* Find(const KeyType& Key);
		const ValueType* Find(const KeyType& Key) const;
		bool Contains(const KeyType& Key) const;
		CuArray<KeyType> GetKeys() const;
		CuArray<ValueType> GetValues() const;
		ValueType& operator[](const KeyType& Key);
		const ValueType& operator[](const KeyType& Key) const;

		// Trivial getters
		size_t Size() const { return ElementCount; }
		bool IsEmpty() const { return ElementCount == 0; }

	private:
		// Index + i + i^2 modulo length -1
		uint32 GetProbeIndex(const uint32 InitialIndex, const uint32 ProbeCount) { return (InitialIndex + ProbeCount + ProbeCount*ProbeCount)&(Slots.Length() - 1); }

		// Slot ops
		uint32 FindSlotIndex(const KeyType& Key, bool& outSlotFound) const;
		uint32 FindOrAddSlot(const KeyType& Key, bool& outSlotFound);

		// Hash
		bool ShouldRehash(size_t AdditionalElements = 0);
		void Rehash(size_t NewMinCapacity = 0);
		
	private:
		// Slot states enum
	    enum class ESlotState : uint8
	    {
	        Empty=0,      // Slot has never been used
	        Occupied,   // Slot contains a valid key-value pair
	        Tombstone   // Slot had an element that was removed
	    };
	    
	    // A slot structure to store key-value pairs and their state
	    struct SSlot
	    {
	        SKVPair<KeyType, ValueType> Pair;
	        ESlotState State = ESlotState::Empty;
	        
	        // Constructors
	        SSlot() {}
	        SSlot(const KeyType& Key, const ValueType& Value) : Pair(Key, Value), State(ESlotState::Occupied) {}
	        SSlot(KeyType&& Key, ValueType&& Value) : Pair(std::move(Key), std::move(Value)), State(ESlotState::Occupied) {}

	    	// State checkers
	        bool IsAvailable() const { return State != ESlotState::Occupied; }
	        bool IsEmpty() const { return State == ESlotState::Empty; }
	        bool IsOccupied() const { return State == ESlotState::Occupied; }
	        bool IsTombstone() const { return State == ESlotState::Tombstone; }
	        
	        // Slot ops
	        void MarkOccupied() { State = ESlotState::Occupied; }
	    	
	        void MarkTombstone() 
	        { 
	            // Call destructor on the key-value pair
	            Pair.~SKVPair<KeyType, ValueType>();
	            // Mark as tombstone
	            State = ESlotState::Tombstone;
	        }
	    	
	        void Reset()
	        {
	            if (State == ESlotState::Occupied)
	            {
	                // Call destructor if occupied
	                Pair.~SKVPair<KeyType, ValueType>();
	            }
	            State = ESlotState::Empty;
	        }
	    };

		// Dict private functions
		uint64 GetHash(const KeyType& Key) const;

		static size_t GetNextPow2(const size_t Value);
		
		uint32 GetInitialIndex(uint64 Hash) const;
		void InitializeSlots(size_t MinimumCapacity = INITIAL_SLOT_COUNT);
	    
	    // Core member variables for the hash table
	    CuArray<SSlot> Slots;
	    size_t ElementCount = 0;
	    size_t TombstoneCount = 0;
	    IAllocator* Allocator = nullptr;
	};

	template <typename KeyType, typename ValueType>
	CuDict<KeyType, ValueType>::CuDict(): Allocator(&GMemory::Get().GetDefaultAllocator())
	{
		InitializeSlots();
	}

	template <typename KeyType, typename ValueType>
	CuDict<KeyType, ValueType>::CuDict(IAllocator& inAllocator): Allocator(&inAllocator)
	{
		InitializeSlots();
	}

	template <typename KeyType, typename ValueType>
	CuDict<KeyType, ValueType>::CuDict(const CuDict& Other): Allocator(Other.Allocator)
	{
		// Initialize with empty slots
		InitializeSlots(Other.Slots.Length());
    
		// Copy all occupied elements
		for (size_t i = 0; i < Other.Slots.Length(); i++)
		{
			const SSlot& slot = Other.Slots[i];
			if (slot.IsOccupied())
			{
				Add(slot.Pair.Key, slot.Pair.Value);
			}
		}
	}

	template <typename KeyType, typename ValueType>
	CuDict<KeyType, ValueType>::CuDict(CuDict&& Other) noexcept: Allocator(Other.Allocator), Slots(std::move(Other.Slots)), ElementCount(Other.ElementCount), TombstoneCount(Other.TombstoneCount)
	{
		// Reset the source object
		Other.ElementCount = 0;
		Other.TombstoneCount = 0;
	}

	template <typename KeyType, typename ValueType>
	CuDict<KeyType, ValueType>::~CuDict()
	{
		Clear();
	}

	template <typename KeyType, typename ValueType>
	CuDict<KeyType, ValueType>& CuDict<KeyType, ValueType>::operator=(const CuDict& Other)
	{
		// Self-assignment check with early return
		if (this == &Other)
			return *this;
		
		Clear();

		// Copy allocator
		Allocator = Other.Allocator;

		// Resize slots if needed
		if (Slots.Length() < Other.Slots.Length())
		{
			InitializeSlots(Other.Slots.Length());
		}

		// Copy all occupied elements
		for (size_t i = 0; i < Other.Slots.Length(); i++)
		{
			const SSlot& slot = Other.Slots[i];
			if (slot.IsOccupied())
			{
				Add(slot.Pair.Key, slot.Pair.Value);
			}
		}
    
		return *this;
	}

	template <typename KeyType, typename ValueType>
	CuDict<KeyType, ValueType>& CuDict<KeyType, ValueType>::operator=(CuDict&& Other) noexcept
	{
		// Self-assignment check with early return
		if (this == &Other)
			return *this;

		Clear();

		// Move data
		Slots = std::move(Other.Slots);
		ElementCount = Other.ElementCount;
		TombstoneCount = Other.TombstoneCount;
		Allocator = Other.Allocator;
    
		// Reset the source object
		Other.ElementCount = 0;
		Other.TombstoneCount = 0;
    
		return *this;
	}

	template <typename KeyType, typename ValueType>
	void CuDict<KeyType, ValueType>::Add(const KeyType& Key, const ValueType& Value)
	{
		bool keyExists = false;
		uint32 slotIdx = FindOrAddSlot(Key, keyExists);

		if (slotIdx == INDEX_NONE)
			return;

		if (keyExists)
		{
			// Replace existing value
			Slots[slotIdx].Pair.Value = Value;
		}
		else
		{
			// Add new key-value pair
			new (&Slots[slotIdx].Pair) SKVPair<KeyType, ValueType>(Key, Value);
			Slots[slotIdx].MarkOccupied();
		}
	}

	template <typename KeyType, typename ValueType>
	void CuDict<KeyType, ValueType>::Add(KeyType&& Key, ValueType&& Value)
	{
		bool keyExists = false;
		uint32 slotIdx = FindOrAddSlot(Key, keyExists);

		if (slotIdx == INDEX_NONE)
			return;

		if (keyExists)
		{
			// Replace existing value
			Slots[slotIdx].Pair.Value = Value;
		}
		else
		{
			// Add new key-value pair
			new (&Slots[slotIdx].Pair) SKVPair<KeyType, ValueType>(std::move(Key), std::move(Value));
			Slots[slotIdx].MarkOccupied();
		}
	}

	template <typename KeyType, typename ValueType>
	void CuDict<KeyType, ValueType>::Remove(const KeyType& Key)
	{
		bool keyExists = false;
		uint32 slotIdx = FindSlotIndex(Key, keyExists);

		if (!keyExists || slotIdx == INDEX_NONE)
			return;

		// Mark the slot as a tombstone
		Slots[slotIdx].MarkTombstone();
    
		// Update counts
		ElementCount--;
		TombstoneCount++;
	}

	template <typename KeyType, typename ValueType>
	void CuDict<KeyType, ValueType>::Clear()
	{
		// Reset all slots
		for (size_t i = 0; i < Slots.Length(); i++)
		{
			Slots[i].Reset();
		}
    
		// Reset counts
		ElementCount = 0;
		TombstoneCount = 0;
	}

	template <typename KeyType, typename ValueType>
	ValueType* CuDict<KeyType, ValueType>::Find(const KeyType& Key)
	{
		bool keyExists = false;
		uint32 slotIdx = FindSlotIndex(Key, keyExists);
		if (!keyExists || slotIdx == INDEX_NONE)
			return nullptr;

		return &Slots[slotIdx].Pair.Value;
	}

	template <typename KeyType, typename ValueType>
	const ValueType* CuDict<KeyType, ValueType>::Find(const KeyType& Key) const
	{
		bool keyExists = false;
		uint32 slotIdx = FindSlotIndex(Key, keyExists);
		if (!keyExists || slotIdx == INDEX_NONE)
			return nullptr;

		return &Slots[slotIdx].Pair.Value;
	}

	template <typename KeyType, typename ValueType>
	bool CuDict<KeyType, ValueType>::Contains(const KeyType& Key) const
	{
		bool keyExists = false;
		FindSlotIndex(Key, keyExists);
		return keyExists;
	}

	template <typename KeyType, typename ValueType>
	CuArray<KeyType> CuDict<KeyType, ValueType>::GetKeys() const
	{
		CuArray<KeyType> keys;
		keys.Reserve(ElementCount);

		// Iterate through all slots and collect occupied keys
		for (const SSlot& slot : Slots)
		{
			if (slot.IsOccupied())
			{
				keys.Add(slot.Pair.Key);
			}
		}
    
		return keys;
	}

	template <typename KeyType, typename ValueType>
	CuArray<ValueType> CuDict<KeyType, ValueType>::GetValues() const
	{
		CuArray<ValueType> values;
		values.Reserve(ElementCount);

		// Iterate through all slots and collect occupied values
		for (const SSlot& slot : Slots)
		{
			if (slot.IsOccupied())
			{
				values.Add(slot.Pair.Value);
			}
		}

		return values;
	}

	template <typename KeyType, typename ValueType>
	ValueType& CuDict<KeyType, ValueType>::operator[](const KeyType& Key)
	{
		ValueType* value = Find(Key);
		CU_ASSERT(value != nullptr, "Key does not exist");	// This should ideally never be hit
		return *value;
	}

	template <typename KeyType, typename ValueType>
	const ValueType& CuDict<KeyType, ValueType>::operator[](const KeyType& Key) const
	{
		const ValueType* value = Find(Key);
		CU_ASSERT(value != nullptr, "Key does not exist");	// This should ideally never be hit
		return *value;
	}

	template <typename KeyType, typename ValueType>
	uint32 CuDict<KeyType, ValueType>::FindSlotIndex(const KeyType& Key, bool& outSlotFound) const
	{
		outSlotFound = false;
		if (Slots.IsEmpty())
			return INDEX_NONE;

		// Hash the key and get initial index
		const uint64 hash = GetHash(Key);
		const uint32 index = GetInitialIndex(hash);

		// Keep track of the first available slot (tombstone)
		uint32 firstAvailableIndex = INDEX_NONE;

		// Probing sequence
		for (uint32 i = 0; i < Slots.Length(); i++)
		{
			const uint32 probeIndex = GetProbeIndex(index, i);
			const SSlot& slot = Slots[probeIndex];

			// If we found an empty slot, the key doesn't exist
			if (slot.IsEmpty())
				return firstAvailableIndex != INDEX_NONE ? firstAvailableIndex : probeIndex;

			// If we found a tombstone and haven't recorded one yet, remember it
			if (slot.IsTombstone())
			{
				if (firstAvailableIndex == INDEX_NONE)
					firstAvailableIndex = probeIndex;
				continue;
			}

			// If we found the key, return its index
			if (slot.IsOccupied() && slot.Pair.Key == Key)
			{
				outSlotFound = true;
				return probeIndex;
			}
		}

		// If we get here, the table is full of occupied slots and tombstones
		// Return the first available tombstone if we found one
		return firstAvailableIndex != INDEX_NONE ? firstAvailableIndex : INDEX_NONE;
	}

	template <typename KeyType, typename ValueType>
	uint32 CuDict<KeyType, ValueType>::FindOrAddSlot(const KeyType& Key, bool& outSlotFound)
	{
		// Check if we need to resize the dict
		if (ShouldRehash())
			Rehash();

		const uint32 index = FindSlotIndex(Key, outSlotFound);
		
		// If the key wasn't found and we're adding a new element
		if (!outSlotFound && index != INDEX_NONE)
		{
			// If we're reusing a tombstone, decrement the tombstone count
			if (Slots[index].IsTombstone())
				TombstoneCount--;
            
			// Increment the element count
			ElementCount++;
		}

		return index;
	}

	template <typename KeyType, typename ValueType>
	bool CuDict<KeyType, ValueType>::ShouldRehash(size_t AdditionalElements)
	{
		// If the table is empty, we need to initialize it
		if (Slots.IsEmpty())
			return true;

		// Calculate the load factor if we were to add the additional elements
		const float projectedLoadFactor = static_cast<float>(ElementCount + TombstoneCount + AdditionalElements) / Slots.Length();
		const float tombstoneRatio = static_cast<float>(TombstoneCount) / Slots.Length();

		// Rehash if the load factor exceeds our threshold
		return projectedLoadFactor >= MAX_LOAD_FACTOR || tombstoneRatio >= TOMBSTONE_RATIO;
	}

	template <typename KeyType, typename ValueType>
	void CuDict<KeyType, ValueType>::Rehash(size_t NewMinCapacity)
	{
		// Calculate new capacity (at least double current size or accommodate new elements)
		size_t newCapacity = Slots.IsEmpty() ? INITIAL_SLOT_COUNT : Slots.Length() * 2;

		// If a minimum capacity was specified, use the larger of the two
		if (NewMinCapacity > 0)
			newCapacity = GetNextPow2(NewMinCapacity);

		// Ensure the new capacity is large enough for current elements
		const size_t minRequiredCapacity = static_cast<size_t>(ElementCount / MAX_LOAD_FACTOR) + 1;
		if (newCapacity < minRequiredCapacity)
			newCapacity = GetNextPow2(minRequiredCapacity);

		// Save the old slots and create a new empty slot array
		CuArray<SSlot> oldSlots;
		oldSlots.Swap(Slots);

		// Initialize the new slot array
		Slots.Resize(newCapacity);

		// Reset element and tombstone counts
		const size_t oldElementCount = ElementCount;
		ElementCount = 0;
		TombstoneCount = 0;

		// Reinsert all elements from the old slots
		for (const SSlot& slot : oldSlots)
		{
			if (slot.IsOccupied())
			{
				// Find a slot for this key
				bool keyExists = false;
				const uint32 newIndex = FindSlotIndex(slot.Pair.Key, keyExists);

				// This should never happen since we're inserting into a fresh table
				CU_ASSERT(!keyExists, "Key already exists during rehash");

				// Move the key-value pair to the new slot
				Slots[newIndex].Pair.Key = std::move(const_cast<KeyType&>(slot.Pair.Key));
				Slots[newIndex].Pair.Value = std::move(const_cast<ValueType&>(slot.Pair.Value));
            
				// Mark the slot as occupied and increment the element count
				Slots[newIndex].MarkOccupied();
				ElementCount++;
			}
		}

		// Verify that we reinserted all elements
		CU_ASSERT(ElementCount == oldElementCount, "Element count mismatch after rehash");
	}

	template <typename KeyType, typename ValueType>
	uint64 CuDict<KeyType, ValueType>::GetHash(const KeyType& Key) const
	{
		return Utils::Hash64(Key);
	}

	template <typename KeyType, typename ValueType>
	size_t CuDict<KeyType, ValueType>::GetNextPow2(const size_t Value)
	{
		// Early return if power of two
		if (IsPowerOfTwo(Value))
			return Value;

		// Find the next power of two
		size_t power = 1;
		while (power < Value)
			power <<= 1;
    
		return power;
	}

	template <typename KeyType, typename ValueType>
	uint32 CuDict<KeyType, ValueType>::GetInitialIndex(uint64 Hash) const
	{
		return Hash & (Slots.Length() - 1);
	}

	template <typename KeyType, typename ValueType>
	void CuDict<KeyType, ValueType>::InitializeSlots(size_t MinimumCapacity)
	{
		size_t capacity = GetNextPow2(MinimumCapacity);

		Slots.Resize(capacity);;
	}
}
