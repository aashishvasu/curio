#pragma once

namespace Math
{
	// Constants
	constexpr float PI = 3.14159265358979323846f;
	constexpr float TAU = PI * 2.f;
	constexpr float DEG_TO_RAD = PI / 180.0f;
	constexpr float RAD_TO_DEG = 180.0f / PI;
	constexpr float EPSILON = 1e-6f;

	// Basic operations
	template<typename T>
	constexpr T Min(const T& A, const T& B) { return (A < B) ? A : B; }

	template<typename T>
	constexpr T Max(const T& A, const T& B) { return (A > B) ? A : B; }

	template<typename T>
	constexpr T Clamp(const T& Value, const T& Lower, const T& Upper) {	return Min(Max(Value, Lower), Upper); }

	template<typename T>
	constexpr T Abs(const T& value) { return (value < 0) ? -value : value; }

	// Floating point utilities
	inline bool IsNearlyEqual(const float A, const float B, const float Threshold = EPSILON) { return Abs(A - B) <= Threshold; }

	inline bool IsNearlyZero(const float Value, const float Threshold = EPSILON) { return Abs(Value) <= Threshold; }

}