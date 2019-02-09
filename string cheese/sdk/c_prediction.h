#pragma once

#include "../misc/vector.h"


namespace ap::sdk
{
	class c_base_entity;
	class c_move_helper;
	class c_move_helper;
	struct c_user_cmd;

	class c_prediction
	{
	public:
		char pad00[8]; 				// 0x0000
		bool m_bInPrediction;				// 0x0008
		char pad01[1];				// 0x0009
		bool m_bEnginePaused;				// 0x000A
		char pad02[13];				// 0x000B
		bool m_bIsFirstTimePredicted;			// 0x0018

	private:
		virtual void UnknownVirtual0() = 0;
		virtual void UnknownVirtual1() = 0;
		virtual void UnknownVirtual2() = 0;
		virtual void UnknownVirtual3() = 0;
		virtual void UnknownVirtual4() = 0;
		virtual void UnknownVirtual5() = 0;
		virtual void UnknownVirtual6() = 0;
		virtual void UnknownVirtual7() = 0;
		virtual void UnknownVirtual8() = 0;
		virtual void UnknownVirtual9() = 0;
		virtual void UnknownVirtual10() = 0;
		virtual void UnknownVirtual11() = 0;
		virtual void UnknownVirtual12() = 0;
		virtual void UnknownVirtual13() = 0;
		virtual void UnknownVirtual14() = 0;
		virtual void UnknownVirtual15() = 0;

	public:
		virtual void GetLocalViewAngles(vec3f& ang);
		virtual void SetLocalViewAngles(vec3f& ang);

		virtual void CheckMovingGround(c_base_entity *player, double frametime);
		virtual void RunCommand(c_base_entity* player, c_user_cmd* ucmd, c_move_helper* moveHelper);

		virtual void SetupMove(c_base_entity* player, c_user_cmd* ucmd, c_move_helper* pHelper, void* move) = 0; //20
		virtual void FinishMove(c_base_entity* player, c_user_cmd* ucmd, void* move) = 0;
	};
} // namespace ap::sdk