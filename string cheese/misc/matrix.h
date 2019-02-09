#pragma once

#include "vector.h"


namespace ap
{
	struct matrix3x4_t
	{
	public:
		matrix3x4_t() {}

		inline float* operator[](int i)
		{
			return m[i];
		}
		inline const float* operator[](int i) const
		{
			return m[i];
		}

	public:
		float m[3][4];
	};

	struct matrix4x4_t
	{
	public:
		matrix4x4_t() {}

		inline float* operator[](int i)
		{
			return m[i];
		}
		inline const float* operator[](int i) const
		{
			return m[i];
		}

	public:
		float m[4][4];
	};
} // namespace ap