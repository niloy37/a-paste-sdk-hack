#pragma once
#include "../misc/vector.h"
#include "../misc/matrix.h"
#include <windows.h>
#include <cstdint>
#include <string>
#include <sstream>
#include <cctype>
#include <random>
#include <array>
#include <utility>
#include <intrin.h>
#include <chrono>
#include <iostream>
namespace ap {
	
	// sse min.
	template< typename t = float >
	t min( const t &a, const t &b ) {
		// check type.
		static_assert( std::is_arithmetic< t >::value, "Math::min only supports integral types." );
		return (t)_mm_cvtss_f32(
			_mm_min_ss( _mm_set_ss( (float)a ),
			            _mm_set_ss( (float)b ) )
		);
	}

	// sse max.
	template< typename t = float >
	t max( const t &a, const t &b ) {
		// check type.
		static_assert( std::is_arithmetic< t >::value, "Math::max only supports integral types." );
		return (t)_mm_cvtss_f32(
			_mm_max_ss( _mm_set_ss( (float)a ),
			            _mm_set_ss( (float)b ) )
		);
	}
	template < typename t >
	t clamp2(t in, t low, t high) {
		return std::min(std::max(in, low), high);
	}
	
	// sse clamp.
	template< typename t = float >
	void clamp( t &val, const t &min, const t &max ) {
		// check type.
		static_assert( std::is_arithmetic< t >::value, "Math::clamp only supports integral types." );
		val = (t)_mm_cvtss_f32(
			_mm_min_ss( _mm_max_ss( _mm_set_ss( (float)val ), _mm_set_ss( (float)min ) ),
			            _mm_set_ss( (float)max ) )
		);
	}

	// sse sqrt.
	template< typename t = float >
	t sqrt( const t &val ) {
		// check type.
		static_assert( std::is_arithmetic< t >::value, "Math::sqrt only supports integral types." );
		return _mm_cvtss_f32( _mm_sqrt_ss( _mm_set_ss( (float)val ) ) );
	}
}
