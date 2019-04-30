#pragma once
// includes 
#include "../sdk/c_user_cmd.h"

#define backtrackspace namespace


backtrackspace ap::features::backtrack {

	struct sequence_data {

		sequence_data(int instate, int outstate, int seqnr, float realtime) {

			inreliablestate = instate;
			outreliablestate = outstate;
			sequencenr = seqnr;
			time = realtime;
		}

		int inreliablestate, outreliablestate, sequencenr;
		float time;

	};

	struct vars_bt {

		int latest_tick;
		int last_sequence;
		float interval_per_tick;
	};

	struct backtrack_data {

		float simtime;
		vec3f hitbox_position;
		int tick_count;
		matrix3x4_t bone_matrix[128];
	};
	/* externs */
	extern backtrack_data bone_data[64][12];
	extern backtrack_data entity_data[64][12];
	extern std::list<sequence_data> sequences;

	void on_create_move(ap::sdk::c_user_cmd* mango_cmd);
	void on_paint_traverse();
	bool is_valid_backtrack_tick(float simtime);
	float lerp_time();
	void on_draw_model_execute();
}

