#include "utils.h"

#include <Windows.h>
#include <Psapi.h>

#include "../ap_core.h"

#include "../sdk/c_user_cmd.h"
#include "../sdk/c_base_weapon.h"
#include "../sdk/c_client_class.h"
#include "defines.h"
#include "../sdk/c_base_entity.h"
#include "../sdk/c_model_render.h"

#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))


namespace
{
	void inline sin_cos(float radians, float *sine, float *cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);
	}
} // namespace

namespace ap
{
	
	unsigned int find_signature(const char* module_name, const char* signature)
	{
		MODULEINFO modInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module_name), &modInfo, sizeof(MODULEINFO));

		// get beggining and end of the dll
		unsigned int start_address = reinterpret_cast<unsigned int>(modInfo.lpBaseOfDll),
			end_address = start_address + modInfo.SizeOfImage;

		const char* pattern = signature;
		unsigned int first_match = 0;
		for (unsigned int current_address = start_address; current_address < end_address; current_address++)
		{
			if (!*pattern)
				return first_match;

			if (*(unsigned char*)pattern == '\?' || *(unsigned char*)current_address == getByte(pattern))
			{
				if (!first_match)
					first_match = current_address;

				if (!pattern[2])
					return first_match;

				if (*(unsigned short*)pattern == '\?\?' || *(unsigned char*)pattern != '\?')
					pattern += 3;
				else
					pattern += 2;
			}
			else
			{
				pattern = signature;
				first_match = 0;
			}
		}

		core::log("failed to find signature: \"%s\" - \"%s\"", module_name, signature);
		return 0;
	}

	void angle_vec3f(const vec3f &angles, vec3f& forward, vec3f& right, vec3f& up)
	{
		float sr, sp, sy, cr, cp, cy;
		sin_cos(DEG2RAD(angles[1]), &sy, &cy);
		sin_cos(DEG2RAD(angles[0]), &sp, &cp);
		sin_cos(DEG2RAD(angles[2]), &sr, &cr);

		forward[0] = cp * cy;
		forward[1] = cp * sy;
		forward[2] = -sp;

		right[0] = (-1 * sr * sp * cy + -1 * cr * -sy);
		right[1] = (-1 * sr * sp * sy + -1 * cr * cy);
		right[2] = -1 * sr * cp;

		up[0] = (cr*sp*cy + -sr * -sy);
		up[1] = (cr*sp*sy + -sr * cy);
		up[2] = cr * cp;
	}
	vec3f angle_vec3f(const vec3f& angle)
	{
		float sp, sy, cp, cy;
		sin_cos(DEG2RAD(angle[1]), &sy, &cy);
		sin_cos(DEG2RAD(angle[0]), &sp, &cp);

		return { cp * cy, cp * sy, -sp };
	}
	vec3f calc_angle(const vec3f& vecSource, const vec3f& vecDestination)
	{
		vec3f qAngles;
		vec3f delta = vec3f((vecSource[0] - vecDestination[0]), (vecSource[1] - vecDestination[1]), (vecSource[2] - vecDestination[2]));
		float hyp = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
		qAngles[0] = (float)(atan(delta[2] / hyp) * (180.0f / pi));
		qAngles[1] = (float)(atan(delta[1] / delta[0]) * (180.0f / pi));
		qAngles[2] = 0.f;
		if (delta[0] >= 0.f)
			qAngles[1] += 180.f;

		return qAngles;
	}
	void angle_vector(const vec3f& angles, vec3f& forward)
	{

		float	sp, sy, cp, cy;

		sy = sin(DEG2RAD(angles[1]));
		cy = cos(DEG2RAD(angles[1]));

		sp = sin(DEG2RAD(angles[0]));
		cp = cos(DEG2RAD(angles[0]));

		forward[0] = cp * cy;
		forward[1] = cp * sy;
		forward[2] = -sp;
	}
	void angle_vector(const vec3f& angles, vec3f& forward, vec3f& right, vec3f& up)
	{
		float sr, sp, sy, cr, cp, cy;
		sin_cos(DEG2RAD(angles[1]), &sy, &cy);
		sin_cos(DEG2RAD(angles[0]), &sp, &cp);
		sin_cos(DEG2RAD(angles[2]), &sr, &cr);

		forward[0] = cp * cy;
		forward[1] = cp * sy;
		forward[2] = -sp;

		right[0] = (-1 * sr * sp * cy + -1 * cr * -sy);
		right[1] = (-1 * sr * sp * sy + -1 * cr * cy);
		right[2] = -1 * sr * cp;

		up[0] = (cr * sp * cy + -sr * -sy);
		up[1] = (cr * sp * sy + -sr * cy);
		up[2] = cr * cp;
	}
	vec3f vec3f_angle(const vec3f& forward)
	{
		float tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / pi_f);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / pi_f);
			if (pitch < 0)
				pitch += 360;
		}

		return normalize_angle(vec3f(pitch, yaw, 0.f));
	}

	float dot_product(const float *a, const float *b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}
	float dot_product(const vec3f& a, const vec3f& b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}
	void vec3f_transform(const float *in1, const matrix3x4_t& in2, float *out)
	{
		out[0] = dot_product(in1, in2[0]) + in2[0][3];
		out[1] = dot_product(in1, in2[1]) + in2[1][3];
		out[2] = dot_product(in1, in2[2]) + in2[2][3];
	}
	void vec3f_transform(const vec3f& in1, const matrix3x4_t& in2, vec3f& out)
	{
		vec3f_transform(&in1[0], in2, &out[0]);
	}
	vec3f vec3f_VECTOR_transform(const vec3f& in1, const matrix3x4_t& in2, vec3f& out)
	{
		vec3f_transform(&in1[0], in2, &out[0]);
	}
	void rotate_movement(sdk::c_user_cmd* cmd, const vec3f& viewangles, float yaw)
	{
		float rotation = DEG2RAD(viewangles[1] - yaw);

		float cos_rot = cos(rotation);
		float sin_rot = sin(rotation);

		float new_forwardmove = (cos_rot * cmd->forwardmove) - (sin_rot * cmd->sidemove);
		float new_sidemove = (sin_rot * cmd->forwardmove) + (cos_rot * cmd->sidemove);

		cmd->forwardmove = new_forwardmove;
		cmd->sidemove = new_sidemove;
	}
	bool is_sniper(sdk::c_base_weapon* weapon)
	{
		switch (weapon->get_item_definition_index())
		{
		case sdk::WEAPON_SCAR20:
		case sdk::WEAPON_G3SG1:
		case sdk::WEAPON_SG553:
		case sdk::WEAPON_SSG08:
		case sdk::WEAPON_AUG:
		case sdk::WEAPON_AWP:
			return true;
		default:
			return false;
		}
	}
	bool is_pistol(sdk::c_base_weapon* weapon)
	{
		switch (weapon->get_item_definition_index())
		{
		case sdk::WEAPON_P2000:
		case sdk::WEAPON_USPS:
		case sdk::WEAPON_P250:
		case sdk::WEAPON_FIVESEVEN:
		case sdk::WEAPON_TEC9:
		case sdk::WEAPON_DEAGLE:
		case sdk::WEAPON_REVOLVER:
		case sdk::WEAPON_GLOCK:
		case sdk::WEAPON_CZ75:
			return true;
		default:
			return false;
		}
	}

	vec3f get_hitbox_position(ap::sdk::c_base_entity* mango_entity, int hitbox_id)
	{

		matrix3x4_t bone_matrix[MAX_STUDIOBONES];

		if (mango_entity->SetupBones(bone_matrix, MAX_STUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {

			auto studio_model = ap::interfaces::model_info->get_studio_hdr(mango_entity->get_model());

			if (studio_model) {

				auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);

				if (hitbox) {

					auto min = vec3f{}, max = vec3f{};

					vec3f_transform(hitbox->bbmin, bone_matrix[hitbox->bone], min);
					vec3f_transform(hitbox->bbmax, bone_matrix[hitbox->bone], max);

					return (min + max) / 2.0f;
				}
			}
		}
		return vec3f{};
	}

} // namespace ap