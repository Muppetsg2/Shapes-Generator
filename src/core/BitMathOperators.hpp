#pragma once

#if __cplusplus > 201703L
#define BIT_TEMPLATE template<::std::unsigned_integral T>
#else
#if __cplusplus > 201402L
#define BIT_TEMPLATE template<class T, typename = ::std::enable_if_t<std::is_integral_v<T> && ::std::is_unsigned_v<T>>>
#else
#define BIT_TEMPLATE template<class T>
#endif
#endif

#if defined(_MSC_VER)
#define INLINE __forceinline
#else
#define INLINE inline
#endif

#if __cplusplus < 201402L
#define STATIC_ASSERT_UNSIGNED(T) \
    static_assert((::std::is_integral<T>::value && ::std::is_unsigned<T>::value), \
    "Type must be an unsigned integral type");
#else
#define STATIC_ASSERT_UNSIGNED(T)
#endif

BIT_TEMPLATE
static INLINE constexpr T mul_2(T x) noexcept {
	STATIC_ASSERT_UNSIGNED(T);
	return x << 1;
}

BIT_TEMPLATE
static INLINE constexpr T mul_8(T x) noexcept {
	STATIC_ASSERT_UNSIGNED(T);
	return x << 3;
}

BIT_TEMPLATE
static INLINE constexpr T div_2(T x) noexcept {
	STATIC_ASSERT_UNSIGNED(T);
	return x >> 1;
}

BIT_TEMPLATE
static INLINE constexpr T div_4(T x) noexcept {
	STATIC_ASSERT_UNSIGNED(T);
	return x >> 2;
}

BIT_TEMPLATE
static INLINE constexpr T mod_2(T x) noexcept {
	STATIC_ASSERT_UNSIGNED(T);
	return x & static_cast<T>(1);
}

BIT_TEMPLATE
static INLINE constexpr T mod_4(T x) noexcept {
	STATIC_ASSERT_UNSIGNED(T);
	return x & static_cast<T>(3);
}

static constexpr uint32_t constexpr_sqrt(uint32_t n) {
	if (n <= 1) return n;

	uint32_t low = 1;
	uint32_t high = 1u << div_2(32u);
	uint32_t result = 1;

	while (low <= high) {
		uint32_t mid = low + div_2(high - low);

		if (mid > n / mid) {
			high = mid - 1;
		}
		else {
			result = mid;
			low = mid + 1;
		}
	}

	return result;
}