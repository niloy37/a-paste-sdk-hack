#pragma once

#include "../misc/vector.h"


namespace ap::sdk
{
	struct c_user_cmd
	{
		char			pad_0x0000[0x4];
		int				command_number;		
		int				tick_count;			
		vec3f			viewangles;			
		vec3f			aimdirection;		
		float			forwardmove;		
		float			sidemove;			
		float			upmove;				
		int				buttons;			
		unsigned char	impulse;			
		int				weaponselect;		
		int				weaponsubtype;		
		int				random_seed;		
		short			mousedx;			
		short			mousedy;			
		bool			hasbeenpredicted;	
		char			pad_0x4C[0x18];		
	};
} // namespace ap::sdk