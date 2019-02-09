#pragma once


namespace ap::sdk
{
	struct c_player_info
	{
		char __pad0[0x8];

		int xuidlow;
		int xuidhigh;

		char name[128];
		int userid;
		char guid[33];

		char __pad1[0x17B];
	};
} // namespace ap::sdk