#pragma once

#include "Memory/Memory.h"

namespace Core
{
	constexpr size_t MIN_CAPACITY = 4;
	
	template <typename Type>
	class CURIO_API CuArray
	{
	public:
		CuArray();
		explicit CuArray(IAllocator& inAllocator);
		CuArray(const CuArray& Other);
		CuArray(CuArray&& Other) noexcept;		
		~CuArray();
		
		CuArray& operator=(const CuArray& Other);
		CuArray& operator=(CuArray&& Other) noexcept;

		// Memory management
		void Reserve(size_t NewCapacity);
		void Resize(size_t NewSize);

		// Element ops
		void Add(const Type& Value);
		void Add(Type&& Value);
		
		template<typename... Args>
		Type& AddInPlace(Args&&... args);
		
		void Remove(uint32 Index);
		void RemoveLast();
		void Swap(CuArray& Other) noexcept;
		void Clear();

		// Accessors
		uint32 Find(const Type& Value) const;
		bool Contains(const Type& Value) const;
		Type& First();
		const Type& First() const;
		Type& Last();
		const Type& Last() const;
		Type Pop();
		Type& operator[](uint32 Index);
		const Type& operator[](uint32 Index) const;

		// Iterator
		Type* begin() { return Data; }
		const Type* begin() const { return Data; }
		Type* end() { return Data + Size; }
		const Type* end() const { return Data + Size; }

		// Trivial getters
		size_t Length() const { return Size; }
		size_t AllocatedCapacity() const { return Capacity; }
		bool IsEmpty() const { return Size == 0; }

	private:
		size_t CalculateGrowth(size_t MinCapacity) const;    
		
		Type* Data = nullptr;
		size_t Size = 0;
		size_t Capacity = 0;
		IAllocator* Allocator = nullptr;
	};

	template <typename Type>
	CuArray<Type>::CuArray(): Allocator(&GMemory::Get().GetDefaultAllocator())
	{}

	template <typename Type>
	CuArray<Type>::CuArray(IAllocator& inAllocator): Allocator(&inAllocator)
	{}

	template <typename Type>
	CuArray<Type>::CuArray(const CuArray& Other) : Allocator(Other.Allocator)
	{
		if (Other.IsEmpty())
			return;

		Reserve(Other.Size);
    
		for (size_t i = 0; i < Other.Size; i++)
		{
			new (&Data[i]) Type(Other.Data[i]);
		}
		
		Size = Other.Size;
	}

	template <typename Type>
	CuArray<Type>::CuArray(CuArray&& Other) noexcept : Data(Other.Data), Size(Other.Size), Capacity(Other.Capacity), Allocator(Other.Allocator)
	{
		// Reset the source object
		Other.Data = nullptr;
		Other.Size = 0;
		Other.Capacity = 0;
	}

	template <typename Type>
	CuArray<Type>::~CuArray()
	{
		Clear();
		if (Data)
		{
			Allocator->Free(Data);
			Data = nullptr;
		}
	}

	template <typename Type>
	CuArray<Type>& CuArray<Type>::operator=(const CuArray& Other)
	{
		// Self-assignment check with early return
		if (this == &Other)
			return *this;
    
		// Copy-and-swap idiom
		CuArray temp(Other);
		Swap(temp);
		return *this;
	}

	template <typename Type>
	CuArray<Type>& CuArray<Type>::operator=(CuArray&& Other) noexcept
	{
		// Self-assignment check with early return
		if (this == &Other)
			return *this;
    
		// Clean up existing resources
		Clear();
		if (Data)
		{
			Allocator->Free(Data);
			Data = nullptr;
			Capacity = 0;
		}
    
		// Use swap for the rest
		Swap(Other);
		return *this;
	}

	template <typename Type>
	void CuArray<Type>::Reserve(const size_t NewCapacity)
	{
		// If required capacity is less than current, we don't need to do anything
		if (NewCapacity <= Capacity)
			return;

		// Allocate new block
		Type* newData = static_cast<Type*>(Allocator->Allocate(NewCapacity * sizeof(Type), alignof(Type)));

		// Move existing elements, if any
		for (size_t i = 0; i < Size; i++)
		{
			// Use placement new with move semantics
			new (&newData[i]) Type(std::move(Data[i]));
			Data[i].~Type();
		}

		// Free old memory and update pointers
		if (Data)
			Allocator->Free(Data);

		Data = newData;
		Capacity = NewCapacity;
	}

	template <typename Type>
	void CuArray<Type>::Resize(const size_t NewSize)
	{
		// Grow the memory if allocated capacity is exceeded
		if (NewSize > Capacity)
		{
			// Calculate growth with 1.5x factor
			const size_t newCapacity = CalculateGrowth(NewSize);
			Reserve(newCapacity);
		}

		// Construct new elements or destroy excess ones
		if (NewSize > Size)
		{
			for (size_t i = Size; i < NewSize; i++)
			{
				new (&Data[i]) Type();
			}
		}
		else if (NewSize < Size)
		{
			for (size_t i = NewSize; i < Size; i++)
			{
				Data[i].~Type();
			}
		}
    
		Size = NewSize;
	}

	template <typename Type>
	void CuArray<Type>::Add(const Type& Value)
	{
		if (Size == Capacity)
		{
			const size_t newCapacity = CalculateGrowth(Size + 1);
			Reserve(newCapacity);
		}
		new (&Data[Size]) Type(Value);
		Size++;
	}

	template <typename Type>
	void CuArray<Type>::Add(Type&& Value)
	{
		if (Size == Capacity)
		{
			const size_t newCapacity = CalculateGrowth(Size + 1);
			Reserve(newCapacity);
		}
		new (&Data[Size]) Type(std::move(Value));
		Size++;
	}

	template <typename Type>
	template <typename ... Args>
	Type& CuArray<Type>::AddInPlace(Args&&... args)
	{
		if (Size == Capacity)
		{
			const size_t newCapacity = CalculateGrowth(Size + 1);
			Reserve(newCapacity);
		}
    
		// Construct element in-place with perfect forwarding
		Type* newElement = new (&Data[Size]) Type(std::forward<Args>(args)...);
		Size++;
    
		return *newElement;
	}

	template <typename Type>
	void CuArray<Type>::Remove(uint32 Index)
	{
		CU_ASSERT(Index < Size, "Index is out of bounds");
        
		// Call destructor on the removed element
		Data[Index].~Type();
    
		// Move all elements after it one step back
		for (size_t i = Index; i < Size - 1; i++)
		{
			new (&Data[i]) Type(std::move(Data[i + 1]));
			Data[i + 1].~Type();
		}
    
		Size--;
	}

	template <typename Type>
	void CuArray<Type>::RemoveLast()
	{
		if (IsEmpty())
			return;
		
		Data[Size - 1].~Type();
		Size--;
	}

	template <typename Type>
	void CuArray<Type>::Swap(CuArray& Other) noexcept
	{
		// Swap pointers and values
		Type* tempData = Data;
		size_t tempSize = Size;
		size_t tempCapacity = Capacity;
		IAllocator* tempAllocator = Allocator;
    
		Data = Other.Data;
		Size = Other.Size;
		Capacity = Other.Capacity;
		Allocator = Other.Allocator;
    
		Other.Data = tempData;
		Other.Size = tempSize;
		Other.Capacity = tempCapacity;
		Other.Allocator = tempAllocator;
	}

	template <typename Type>
	void CuArray<Type>::Clear()
	{
		for (size_t i = 0; i < Size; i++)
		{
			Data[i].~Type();
		}
		Size = 0;
	}

	template <typename Type>
	Type& CuArray<Type>::operator[](uint32 Index)
	{
		CU_ASSERT(Index < Size, "Array index out of bounds");
		return Data[Index];
	}

	template <typename Type>
	const Type& CuArray<Type>::operator[](uint32 Index) const
	{
		CU_ASSERT(Index < Size, "Array index out of bounds");
		return Data[Index];
	}

	template <typename Type>
	uint32 CuArray<Type>::Find(const Type& Value) const
	{
		for (uint32 i = 0; i < Size; i++)
		{
			if (Data[i] == Value)
				return i;
		}

		return INDEX_NONE;
	}

	template <typename Type>
	bool CuArray<Type>::Contains(const Type& Value) const
	{
		return Find(Value) != INDEX_NONE;
	}

	template <typename Type>
	Type& CuArray<Type>::First()
	{
		CU_ASSERT(!IsEmpty(), "Array is empty");
		return Data[0];
	}

	template <typename Type>
	const Type& CuArray<Type>::First() const
	{
		CU_ASSERT(!IsEmpty(), "Array is empty");
		return Data[0];
	}

	template <typename Type>
	Type& CuArray<Type>::Last()
	{
		CU_ASSERT(!IsEmpty(), "Array is empty");
		return Data[Size - 1];
	}

	template <typename Type>
	const Type& CuArray<Type>::Last() const
	{
		CU_ASSERT(!IsEmpty(), "Array is empty");
		return Data[Size - 1];
	}

	template <typename Type>
	Type CuArray<Type>::Pop()
	{
		CU_ASSERT(!IsEmpty(), "Cannot pop from an empty array");
    
		// Get a copy of the last element
		Type result = std::move(Data[Size - 1]);
    
		// Destroy the last element and decrement size
		Data[Size - 1].~Type();
		Size--;
    
		return result;
	}

	template <typename Type>
	size_t CuArray<Type>::CalculateGrowth(const size_t MinCapacity) const
	{
		// This will give us a min capacity of at least 4
		if (Capacity == 0)
			return MinCapacity > MIN_CAPACITY ? MinCapacity : MIN_CAPACITY;

		// We are using 1.5x growth factor. Fast bit shift op for /2 or *0.5
		const size_t newCapacity = Capacity + (Capacity >> 1);

		// If new capacity is still less than minimum, we return that
		return newCapacity > MinCapacity ? newCapacity : MinCapacity;
	}
}
