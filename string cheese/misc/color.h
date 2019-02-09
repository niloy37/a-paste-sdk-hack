#pragma once

#include <limits>
#include <type_traits> // std::conditional

#include "vector.h"


namespace ap
{
	// floats are always from [0, 1] while integral types are from [0, max_value]
	template <typename T, std::size_t MaxValue = (std::is_floating_point<T>::value ? std::size_t(1) : std::size_t(std::numeric_limits<T>::max()))>
	class c_color_rgba : public c_vector<T, 4>
	{
	public:
		constexpr c_color_rgba()
		{
			for (std::size_t i = 0; i < 4; i++)
				this->at(i) = T(MaxValue);
		}
		constexpr c_color_rgba(const T& r, const T& g, const T& b, const T& a = T(MaxValue))
		{
			this->at(0) = r;
			this->at(1) = g;
			this->at(2) = b;
			this->at(3) = a;
		}
		constexpr explicit c_color_rgba(const T& scalar, const T& alpha = T(MaxValue))
		{
			this->at(0) = scalar;
			this->at(1) = scalar;
			this->at(2) = scalar;
			this->at(3) = alpha;
		}

		constexpr const T& r() const { return this->operator[](0); }
		constexpr T& r() { return this->operator[](0); }
		constexpr const T& g() const { return this->operator[](1); }
		constexpr T& g() { return this->operator[](1); }
		constexpr const T& b() const { return this->operator[](2); }
		constexpr T& b() { return this->operator[](2); }
		constexpr const T& a() const { return this->operator[](3); }
		constexpr T& a() { return this->operator[](3); }

		static constexpr c_color_rgba<T, MaxValue> RED(const T& alpha = T(MaxValue))
		{
			return c_color_rgba<T, MaxValue>(T(MaxValue), T(0), T(0), alpha);
		}
		static constexpr c_color_rgba<T, MaxValue> GREEN(const T& alpha = T(MaxValue))
		{
			return c_color_rgba<T, MaxValue>(T(0), T(MaxValue), T(0), alpha);
		}
		static constexpr c_color_rgba<T, MaxValue> BLUE(const T& alpha = T(MaxValue))
		{
			return c_color_rgba<T, MaxValue>(T(0), T(0), T(MaxValue), alpha);
		}
		static constexpr c_color_rgba<T, MaxValue> WHITE(const T& alpha = T(MaxValue))
		{
			return c_color_rgba<T, MaxValue>(T(MaxValue), T(MaxValue), T(MaxValue), alpha);
		}
		static constexpr c_color_rgba<T, MaxValue> BLACK(const T& alpha = T(MaxValue))
		{
			return c_color_rgba<T, MaxValue>(T(0), T(0), T(0), alpha);
		}
		
	private:
	};
	
	using rgba8 = c_color_rgba<uint8_t>;
	using rgba16 = c_color_rgba<uint16_t>;
	using rgba32 = c_color_rgba<uint32_t>;

	using rgbaf = c_color_rgba<float>;
	using rgbad = c_color_rgba<double>;
	using rgbald = c_color_rgba<long double>;
} // namespace ap