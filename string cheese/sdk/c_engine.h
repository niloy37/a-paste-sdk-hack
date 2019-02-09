#pragma once

#include "../misc/vmt.h"
#include "../sdk/c_net_channel.h"
#include "../misc/vector.h"
#include "../misc/matrix.h"


namespace ap::sdk
{
	struct c_player_info;

	class c_engine
	{
	public:
		int get_player_for_user_id(int id)
		{
			using fn = int(__thiscall*)(void*, int);
			return (vmt::get_vfunc<fn>(this, 9))(this, id);
		}
		bool get_player_info(int index, c_player_info* player_info)
		{
			using fn = bool(__thiscall*)(void*, int, c_player_info*);
			return (vmt::get_vfunc<fn>(this, 8))(this, index, player_info);
		}
		void get_screen_size(vec2i& screen_size)
		{
			using fn = void(__thiscall*)(void*, int&, int&);
			return (vmt::get_vfunc<fn>(this, 5))(this, screen_size[0], screen_size[1]);
		}
		int get_local_player()
		{
			using fn = int(__thiscall*)(void*);
			return (vmt::get_vfunc<fn>(this, 12))(this);
		}
		void exec_client_cmd(const char* command)
		{
			using fn = void(__thiscall*)(void*, const char*);
			return (vmt::get_vfunc<fn>(this, 108))(this, command);
		}
		void set_viewangle(const vec3f& angle)
		{
			using fn = void(__thiscall*)(void*, const vec3f&);
			return (vmt::get_vfunc<fn>(this, 19))(this, angle);
		}
		void get_viewangles(vec3f& angle)
		{
			using fn = void(__thiscall*)(void*, vec3f&);
			return (vmt::get_vfunc<fn>(this, 18))(this, angle);
		}
		const matrix4x4_t& get_view_matrix()
		{
			using fn = const matrix4x4_t&(__thiscall*)(void*);
			return (vmt::get_vfunc<fn>(this, 37))(this);
		}
		bool is_in_game()
		{
			using fn = bool(__thiscall*)(void*);
			return (vmt::get_vfunc<fn>(this, 26))(this);
		}
		bool is_connected()
		{
			using fn = bool(__thiscall*)(void*);
			return (vmt::get_vfunc<fn>(this, 27))(this);
		}
		bool is_taking_screenshot()
		{
			using fn = bool(__thiscall*)(void*);
			return (vmt::get_vfunc<fn>(this, 92))(this);
		}
		c_net_channel_info* get_net_channel()
		{
			using fn = c_net_channel_info*(__thiscall*)(void*);
			return (vmt::get_vfunc<fn>(this, 78))(this);
		}

	private:
	};
} // namespace ap::sdk