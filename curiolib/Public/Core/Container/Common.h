#pragma once

namespace Core
{
	template <typename KeyType, typename ValueType>
	struct SKVPair
	{
		KeyType Key;
		ValueType Value;

		SKVPair() = default;
		SKVPair(const KeyType& inKey, const ValueType& inValue) : Key(inKey), Value(inValue) {}
		SKVPair(KeyType&& inKey, ValueType&& inValue) : Key(static_cast<KeyType&&>(inKey)), Value(static_cast<ValueType&&>(inValue)) {}

		SKVPair& operator=(const SKVPair& Other) = default;
		SKVPair& operator=(SKVPair&& Other) = default;
		bool operator==(const SKVPair& Other) const
		{
			return Key == Other.Key;
		}
		bool operator !=(const SKVPair& Other) const
		{
			return !(*this == Other);
		}
	};
}
