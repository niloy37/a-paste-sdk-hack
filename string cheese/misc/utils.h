#pragma once

#include "vector.h"
#include "matrix.h"

#define DEG2RAD(x)  (float(x) * (ap::pi_f / 180.f))
#define RAD2DEG( x  )  (float(x) * (180.f / ap::pi_f))


namespace ap
{
	namespace sdk
	{
		struct c_user_cmd;
		class c_base_weapon;
		class c_base_entity;
	} // namespace sdk
	
	static constexpr double pi = 3.14159265358979323846;
	static constexpr float pi_f = 3.14159265358979323846f;

	unsigned int find_signature(const char* module_name, const char* signature);

	inline float normalize_pitch(float pitch)
	{
		while (pitch > 89.f)
			pitch -= 180.f;
		while (pitch < -89.f)
			pitch += 180.f;

		return pitch;
	}
	inline float normalize_yaw(float yaw)
	{
		if (yaw > 180)
			yaw -= (round(yaw / 360) * 360.f);
		else if (yaw < -180)
			yaw += (round(yaw / 360) * -360.f);

		return yaw;
	}
	inline vec3f normalize_angle(const vec3f& angle)
	{
		return vec3f(normalize_pitch(angle[0]), normalize_yaw(angle[1]), 0.f);
	}

	void angle_vec3f(const vec3f& angles, vec3f& forward, vec3f& right, vec3f& up);
	vec3f angle_vec3f(const vec3f& angle);
	void angle_vector(const vec3f& angles, vec3f& forward);
	vec3f calc_angle(const vec3f& vecSource, const vec3f& vecDestination);
	vec3f vec3f_angle(const vec3f& forward);

	float dot_product(const float *a, const float *b);
	void vec3f_transform(const float *in1, const matrix3x4_t& in2, float *out);
	void vec3f_transform(const vec3f& in1, const matrix3x4_t& in2, vec3f& out);

	vec3f vec3f_VECTOR_transform(const vec3f & in1, const matrix3x4_t & in2, vec3f & out);
	void rotate_movement(sdk::c_user_cmd* cmd, const vec3f& viewangles, float yaw);
	bool is_sniper(sdk::c_base_weapon* weapon);
	vec3f get_hitbox_position(sdk::c_base_entity * mango_entity, int hitbox_id);
} // namespace ap