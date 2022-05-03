#pragma once

// Headers
#include <cstdint>

// Platform detection
#if defined(_WIN32)
	// Windows
#define VE_PLATFORM_WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX
#elif defined(__linux__)
	// Linux
#define VE_PLATFORM_LINUX
#endif

#if !defined(VE_DEBUG) && defined(VE_PLATFORM_WINDOWS)
#define MAIN int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
#else
#define MAIN int main(int argc, char** argv)
#endif

// Macro functions
#define CLAMP(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ABS(a) (((a) < 0) ? -(a) : (a))
#define LERP(a, b, t) ((a) + ((b) - (a)) * (t))
#define SIGN(x) ((x) == 0 ? 0 : (((x) > 0) ? 1 : -1))

namespace Voxel
{

	// Fixed-size types
	using Uint8 = uint8_t;
	using Uint16 = uint16_t;
	using Uint32 = uint32_t;
	using Uint64 = uint64_t;

	using Int8 = int8_t;
	using Int16 = int16_t;
	using Int32 = int32_t;
	using Int64 = int64_t;

	namespace math
	{
		// Math constants
		constexpr const double PI = 3.14159265358979323846;
		constexpr const double PI_2 = PI / 2.0;
		constexpr const double TAU = PI * 2.0;

	} // namespace math

} // namespace Voxel