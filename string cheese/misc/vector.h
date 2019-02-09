#pragma once

#include <cstddef>
#include <array>
#include <string>


namespace ap
{
	template <typename T, std::size_t N>
	class c_vector
	{
	public:
		// c_vector(scalar)
		constexpr explicit c_vector(const T& scalar = T(0)) : m_data({}) { set_all(scalar); }

		// c_vector(x, y, z, ...)
		template <typename... Pack>
		constexpr c_vector(const Pack&... args) : m_data({ args... }) { static_assert(sizeof...(Pack) == N, "Not enough arguments"); }

		constexpr void set_all(const T& value)
		{
			for (std::size_t i = 0; i < N; i++)
				m_data[i] = value;
		}

		// convert to a string, "(x, y, z, ...)"
		std::string to_str() const
		{
			std::string str = "(";
			for (std::size_t i = 0; i < N - 1; i++)
				str += std::to_string(+m_data[i]) + ", ";
			str += std::to_string(+m_data[N - 1]) + ")";
			return str;
		}

		// get the raw pointer to the data mangos
		constexpr const T* data() const { return m_data.data(); }
		constexpr T* data() { return m_data.data(); }

		// safer than [] operator
		constexpr const T& at(std::size_t index) const { return m_data.at(index); }
		constexpr T& at(std::size_t index) { return m_data.at(index); }

		// does not perform out of bounds check
		constexpr const T& operator[](std::size_t index) const { return m_data[index]; }
		constexpr T& operator[](std::size_t index) { return m_data[index]; }

		constexpr bool operator==(const c_vector<T, N>& vector) const
		{
			for (std::size_t i = 0; i < N; i++)
				if (vector[i] != m_data[i])
					return false;
			return true;
		}
		constexpr bool operator!=(const c_vector<T, N>& vector) const { return !(*this == vector); }

		constexpr c_vector<T, N> operator+(c_vector<T, N> vector) const
		{
			for (std::size_t i = 0; i < N; i++)
				vector[i] += m_data[i];

			return vector;
		}
		constexpr c_vector<T, N> operator+(const T& value) const { return *this + c_vector<T, N>(value); }
		constexpr c_vector<T, N>& operator+=(const c_vector<T, N>& vector) { return *this = *this + vector; }
		constexpr c_vector<T, N>& operator+=(const T& value) { return *this = *this + value; }

		constexpr c_vector<T, N> operator-(const c_vector<T, N>& vector) const
		{
			c_vector<T, N> ret = *this;
			for (std::size_t i = 0; i < N; i++)
				ret[i] -= vector[i];

			return ret;
		}
		constexpr c_vector<T, N> operator-(const T& value) const { return *this - c_vector<T, N>(value); }
		constexpr c_vector<T, N>& operator-=(const c_vector<T, N>& vector) { return *this = *this - vector; }
		constexpr c_vector<T, N>& operator-=(const T& value) { return *this = *this - value; }

		constexpr c_vector<T, N> operator*(c_vector<T, N> vector) const
		{
			for (std::size_t i = 0; i < N; i++)
				vector[i] *= m_data[i];

			return vector;
		}
		constexpr c_vector<T, N> operator*(const T& value) const { return *this * c_vector<T, N>(value); }
		constexpr c_vector<T, N>& operator*=(const c_vector<T, N>& vector) { return *this = *this * vector; }
		constexpr c_vector<T, N>& operator*=(const T& value) { return *this = *this * value; }

		constexpr c_vector<T, N> operator/(const c_vector<T, N>& vector) const
		{
			c_vector<T, N> ret = *this;
			for (std::size_t i = 0; i < N; i++)
				ret[i] /= vector[i];

			return ret;
		}
		constexpr c_vector<T, N> operator/(const T& value) const { return *this / c_vector<T, N>(value); }
		constexpr c_vector<T, N>& operator/=(const c_vector<T, N>& vector) { return *this = *this / vector; }
		constexpr c_vector<T, N>& operator/=(const T& value) { return *this = *this / value; }

	private:
		std::array<T, N> m_data;

	private:
		static_assert(N > 0, "Size must be greater than 0");
		static_assert(std::is_arithmetic<T>::value, "Type must be arithmetic");
	};

	// length
	template <typename T, std::size_t N, std::size_t D = N>
	T vec_length(const c_vector<T, N>& vector)
	{
		T ret(0);
		for (std::size_t i = 0; i < D; i++)
			ret += vector[i] * vector[i];
		return ret != T(0) ? std::sqrt(ret) : ret;
	}

	// dot product
	template <typename T, std::size_t N>
	constexpr T vec_dot(const c_vector<T, N>& vector1, const c_vector<T, N>& vector2)
	{
		T ret(0);
		for (std::size_t i = 0; i < N; i++)
			ret += vector1[i] * vector2[i];
		return ret;
	}

	// normalize
	template <typename T, std::size_t N>
	c_vector<T, N> vec_normalize(c_vector<T, N> vector)
	{
		const auto length = vec_length(vector);
		return length != T(0) ? vector / length : vector;
	}

	// output to stream
	template <typename T, std::size_t N>
	std::ostream& operator<<(std::ostream& stream, const c_vector<T, N>& vector)
	{
		return stream << vector.to_str();
	}

	// aliases
	using vec2i8 = c_vector<std::int8_t, 2>;
	using vec3i8 = c_vector<std::int8_t, 3>;
	using vec4i8 = c_vector<std::int8_t, 4>;

	using vec2ui8 = c_vector<std::uint8_t, 2>;
	using vec3ui8 = c_vector<std::uint8_t, 3>;
	using vec4ui8 = c_vector<std::uint8_t, 4>;

	using vec2i16 = c_vector<std::int16_t, 2>;
	using vec3i16 = c_vector<std::int16_t, 3>;
	using vec4i16 = c_vector<std::int16_t, 4>;

	using vec2ui16 = c_vector<std::uint16_t, 2>;
	using vec3ui16 = c_vector<std::uint16_t, 3>;
	using vec4ui16 = c_vector<std::uint16_t, 4>;

	using vec2i32 = c_vector<std::int32_t, 2>;
	using vec3i32 = c_vector<std::int32_t, 3>;
	using vec4i32 = c_vector<std::int32_t, 4>;

	using vec2ui32 = c_vector<std::uint32_t, 2>;
	using vec3ui32 = c_vector<std::uint32_t, 3>;
	using vec4ui32 = c_vector<std::uint32_t, 4>;

	using vec2i64 = c_vector<std::int64_t, 2>;
	using vec3i64 = c_vector<std::int64_t, 3>;
	using vec4i64 = c_vector<std::int64_t, 4>;

	using vec2ui64 = c_vector<std::uint64_t, 2>;
	using vec3ui64 = c_vector<std::uint64_t, 3>;
	using vec4ui64 = c_vector<std::uint64_t, 4>;

	using vec2c = c_vector<char, 2>;
	using vec3c = c_vector<char, 3>;
	using vec4c = c_vector<char, 4>;

	using vec2uc = c_vector<unsigned char, 2>;
	using vec3uc = c_vector<unsigned char, 3>;
	using vec4uc = c_vector<unsigned char, 4>;

	using vec2s = c_vector<short, 2>;
	using vec3s = c_vector<short, 3>;
	using vec4s = c_vector<short, 4>;

	using vec2us = c_vector<unsigned short, 2>;
	using vec3us = c_vector<unsigned short, 3>;
	using vec4us = c_vector<unsigned short, 4>;

	using vec2i = c_vector<int, 2>;
	using vec3i = c_vector<int, 3>;
	using vec4i = c_vector<int, 4>;

	using vec2ui = c_vector<unsigned int, 2>;
	using vec3ui = c_vector<unsigned int, 3>;
	using vec4ui = c_vector<unsigned int, 4>;

	using vec2l = c_vector<long, 2>;
	using vec3l = c_vector<long, 3>;
	using vec4l = c_vector<long, 4>;

	using vec2ul = c_vector<unsigned long, 2>;
	using vec3ul = c_vector<unsigned long, 3>;
	using vec4ul = c_vector<unsigned long, 4>;

	using vec2ll = c_vector<long long, 2>;
	using vec3ll = c_vector<long long, 3>;
	using vec4ll = c_vector<long long, 4>;

	using vec2ull = c_vector<unsigned long long, 2>;
	using vec3ull = c_vector<unsigned long long, 3>;
	using vec4ull = c_vector<unsigned long long, 4>;

	using vec2f = c_vector<float, 2>;
	using vec3f = c_vector<float, 3>;
	using vec4f = c_vector<float, 4>;

	using vec2d = c_vector<double, 2>;
	using vec3d = c_vector<double, 3>;
	using vec4d = c_vector<double, 4>;
} // namespace ap