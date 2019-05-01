#include <list>
#include <algorithm>
#include "../misc/defines.h"
#include "../misc/vector.h"
#include "../misc/interfaces.h"
#include "../sdk/c_client_entity_list.h"
#include "../sdk/c_engine.h"
#include "../sdk/c_base_entity.h"
#include "../misc/utils.h"
#include "../sdk/c_client_state.h"
#include "../sdk/c_globals.h"
#include "../sdk/c_player_info.h"
#include "../features/backtrack.h"
#include "../misc/renderer.h"
#include "../sdk/c_cvar.h"
#include "../menu.h"
#include "../../string cheese/variables.h"

backtrackspace ap::features::backtrack
{
	/* pointers */
	vars_bt variables;
	backtrack_data bone_data[64][12];
	backtrack_data entity_data[64][12];
	std::list<sequence_data> sequences;

	/* fuctions */
	void clear()
	{
		auto net_channel = *reinterpret_cast<sdk::c_net_channel**>(reinterpret_cast<std::uintptr_t>(interfaces::
			client_state) + 0x9C);

		if (net_channel)
		{
			net_channel->in_sequence_nr = 0;
			sequences.clear();
		}
	}

	inline float point_to_line(vec3f point, vec3f line_origin, vec3f dir)
	{
		auto point_dir = point - line_origin;

		auto temp_offset = vec_dot(point_dir, dir) / (dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);

		if (temp_offset < 0.000001f)
			return FLT_MAX;

		auto perpendicular_point = line_origin + (dir * temp_offset);

		return vec_length(point - perpendicular_point);
	}

	void legit(sdk::c_user_cmd* mango_cmd)
	{
		/* if our backtrack is disabled, don't do anything */
		//if (!c_menu.aimbot.backtrack.get_bool())
		//	return;

		/* if we enable the backtrack */
		//if (c_menu.aimbot.backtrack.get_bool()) {

		int best_target = -1;
		float best_fov = FLT_MAX;

		sdk::c_player_info pInfo;

		/* pointer to the local player */
		sdk::c_base_entity* mango_local = interfaces::client_entity_list->get_client_entity(
			interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;

		/* don't do anything if the local player is dead */
		if (!mango_local->is_alive())
			return;

		/* get the local player view direction */
		vec3f view_direction = angle_vec3f(mango_cmd->viewangles + (mango_local->get_aim_punch() * 2.f));

		/* loop through all connected clients */
		for (int i = 1; i < 65; i++)
		{
			/* pointer to the entity (clients) */
			sdk::c_base_entity* mango_entity = interfaces::client_entity_list->get_client_entity(i);

			/* don't do anything if the local player is somehow invalid */
			if (!mango_local)
				continue;

			/* if we don't have a entity to backtrack, don't do anything */
			if (!mango_entity)
				continue;

			/* if the entity is the local player, don't do anything */
			if (mango_entity == mango_local)
				continue;

			/* if we can't get the player info, don't do anything */
			if (!interfaces::engine->get_player_info(i, &pInfo))
				continue;

			/* if the entity is dormant, don't do anything */
			if (mango_entity->is_dormant())
				continue;

			/* don't backtrack players of the local player team */
			if (mango_entity->get_team_num() == mango_local->get_team_num())
				continue;

			if (!mango_entity->SetupBones(bone_data[i][mango_cmd->command_number % 12].bone_matrix, MAX_STUDIOBONES,
			                              BONE_USED_BY_HITBOX, interfaces::globals->curtime))
				continue;

			/* if the entity is alive, we can start searching for a candidate to backtrack */
			if (mango_entity->is_alive())
			{
				/* get the simulation time */
				float simtime = mango_entity->get_simulation_time();

				/* get the hitbox position */
				vec3f hitbox_position = get_hitbox_position(mango_entity, 0);

				/* store data about the entity (his position, etc) */
				entity_data[i][mango_cmd->command_number % 12] = backtrack_data{simtime, hitbox_position};

				float fov_distance = point_to_line(hitbox_position, mango_local->get_eye_position(), view_direction);

				if (best_fov > fov_distance)
				{
					best_fov = fov_distance;
					best_target = i;
				}
			}
		}

		/* best simulation time (we will store in here) */
		float best_simtime;

		/* if we have a good target */
		if (best_target != -1)
		{
			/* temporary float */
			float temporary_float = FLT_MAX;

			for (int t = 0; t < 12; ++t)
			{
				/* temporary fov */
				float temp_fov_distance = point_to_line(entity_data[best_target][t].hitbox_position,
				                                        mango_local->get_eye_position(), view_direction);

				if (temporary_float > temp_fov_distance && entity_data[best_target][t].simtime > mango_local->
					get_simulation_time() - 1)
				{
					temporary_float = temp_fov_distance;
					best_simtime = entity_data[best_target][t].simtime;
				}
			}

			/* do the magic :) */
			if (mango_cmd->buttons & IN_ATTACK)
				mango_cmd->tick_count = TIME_TO_TICKS(best_simtime);
		}
		//}
	}

	void update_backtrack()
	{
		//if (!c_menu.aimbot.backtrack.get_bool())
		//	return;

		variables.latest_tick = interfaces::globals->tickcount;
	}

	void update_sequences()
	{
		auto net_channel = *reinterpret_cast<sdk::c_net_channel**>(reinterpret_cast<std::uintptr_t>(interfaces::
			client_state) + 0x9C);

		if (net_channel)
		{
			if (net_channel->in_sequence_nr > variables.last_sequence)
			{
				variables.last_sequence = net_channel->in_sequence_nr;
				sequences.push_front(sequence_data(net_channel->in_reliable_state_count,
				                                   net_channel->out_reliable_state_count, net_channel->in_sequence_nr,
				                                   interfaces::globals->realtime));
			}

			if (sequences.size() > 2048)
				sequences.pop_back();
		}
	}

	void update()
	{
		/* update our backtrack */
		update_backtrack();
		update_sequences();
	}

	void run(sdk::c_user_cmd* mango_cmd)
	{
		/* if our backtrack is disabled, don't do anything */
		//if (!c_menu.aimbot.backtrack.get_bool())
		//	return;
		if (!variables::legit_backtrack)
			return;
		/* if we enable the backtrack */
		//if (c_menu.aimbot.backtrack.get_bool()) {

		/* run the legit backtrack */
		legit(mango_cmd);
		//}
	}

	template <class T, class U>
	T clamp(T in, U low, U high)
	{
		if (in <= low)
			return low;
		if (in >= high)
			return high;
		return in;
	}

	float latency()
	{
		const auto netch = interfaces::engine->get_net_channel();

		if (netch)
			return netch->get_average_latency(0);

		return 0.0f;
	}

	float lerp_time()
	{
		//printf("Lerp da time!\n");
		int ud_rate = interfaces::cvar->find_var("cl_updaterate")->get_int();
		float ratio = interfaces::cvar->find_var("cl_interp_ratio")->get_float();
		return ratio / (float)ud_rate;
	}

	bool is_valid_backtrack_tick(float simtime)
	{
		float correct = clamp(latency() + lerp_time(), 0.f, 0.2f);
		float delta_time = correct - (interfaces::globals->curtime - simtime + lerp_time());
		return fabsf(delta_time) < 0.2f;
	}

	void backtrack_visual_stuff()
	{
		sdk::c_base_entity* mango_local = interfaces::client_entity_list->get_client_entity(
			interfaces::engine->get_local_player());
		if (mango_local == nullptr)
			return;

		/* anti-screenshot */
		if (interfaces::engine->is_taking_screenshot())
			return;

		//if (c_config::get().backtrackdots) {
		for (int eeboy = 1; eeboy < interfaces::globals->maxclients; eeboy++)
		{
			/* pointer to the entity (clients) */
			sdk::c_base_entity* mango_entity = interfaces::client_entity_list->get_client_entity(eeboy);
			if (mango_entity && mango_entity->is_valid(mango_entity, true, true, false))
			{
				if (mango_entity != mango_local)
				{
					for (int ree = 0; ree < 12; ree++)
					{
						backtrack_data currentrecord = entity_data[eeboy][ree];

						if (!(is_valid_backtrack_tick(currentrecord.simtime)))
							continue;

						vec3f thisTick = currentrecord.hitbox_position;

						vec2i screenThisTick;
						/* bone positions */

						if (renderer::world_to_screen(thisTick, screenThisTick))
						{
							if (variables::visualize_backtrack_dots)
							{
								renderer::render_filled_rect(screenThisTick, screenThisTick + 8, rgba8::WHITE());
							}


							if (variables::visualize_backtrack_octagon)
							{
								renderer::render_empty_circle(screenThisTick[0], screenThisTick[1], 10, 8,
								                              rgba8::RED());
								renderer::render_filled_circle(screenThisTick[0], screenThisTick[1], 9, 8,
								                               ree < 6 ? rgba8::BLUE() : rgba8::WHITE());
							}
						}
						if (variables::backtrack_stick_figure)
						{
							/* draw the skeleton in the backtracked position */
							if (entity_data[eeboy][ree].simtime && entity_data[eeboy][ree].simtime + 1 > mango_local->
								get_simulation_time())
							{
								auto model = interfaces::model_info->get_studio_hdr(mango_entity->get_model());

								/* if we don't have a valid model to do our skeleton, don't do anything */
								if (!model)
									continue;

								for (int b = 0; b < model->numbones; b++)
								{
									/* get the entity bone */
									sdk::mstudiobone_t* pBone = model->GetBone(b);

									/* if we don't have any valid bone, don't do anything */
									if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
										continue;

									/* bone positions */
									static vec2i bone_position, bone_previous_position;

									/* check if we have a valid place to draw the skeleton */
									if (!renderer::world_to_screen(
										vec3f(bone_data[eeboy][ree].bone_matrix[b][0][3],
										      bone_data[eeboy][ree].bone_matrix[b][1][3],
										      bone_data[eeboy][ree].bone_matrix[b][2][3]), bone_position))
										continue;

									if (!renderer::world_to_screen(
										vec3f(bone_data[eeboy][ree].bone_matrix[pBone->parent][0][3],
										      bone_data[eeboy][ree].bone_matrix[pBone->parent][1][3],
										      bone_data[eeboy][ree].bone_matrix[pBone->parent][2][3]),
										bone_previous_position))
										continue;

									/* draw the skeleton */
									//renderer::render_empty_circle(bone_position[0], bone_previous_position[1], 9, 8, rgba8::BLUE());
									renderer::render_line(vec2i(bone_position[0], bone_position[1]),
									                      vec2i(bone_previous_position[0], bone_previous_position[1]),
									                      ree <= 6 ? rgba8::RED() : rgba8::BLUE());
								}
							}
						}
					}
				}
			}
		}
	}

	void backtrack_visual_dme()
	{
		//	ap::sdk::c_base_entity* mango_local = ap::interfaces::client_entity_list->get_client_entity(ap::interfaces::engine->get_local_player());
		//	if (mango_local == nullptr)
		//		return;
		//
		//	/* anti-screenshot */
		//	if (ap::interfaces::engine->is_taking_screenshot())
		//		return;
		//
		//	//if (c_config::get().backtrackdots) {
		//	for (int eeboy = 1; eeboy < ap::interfaces::globals->maxclients; eeboy++) {
		//
		//		/* pointer to the entity (clients) */
		//		ap::sdk::c_base_entity* mango_entity = ap::interfaces::client_entity_list->get_client_entity(eeboy);
		//		if (mango_entity && mango_entity->is_valid(mango_entity, true, true, false)) {
		//			if (mango_entity != mango_local)
		//			{
		//				for (int ree = 0; ree < 12; ree++)
		//				{
		//					if (ap::text_menu::menu::get()._get(L"esp_backtrack_stick_figure")) {
		//						/* draw the skeleton in the backtracked position */
		//						if (entity_data[eeboy][ree].simtime && entity_data[eeboy][ree].simtime + 1 > mango_local->get_simulation_time()) {
		//
		//							auto model = ap::interfaces::model_info->get_studio_hdr(mango_entity->get_model());
		//
		//							/* if we don't have a valid model to do our skeleton, don't do anything */
		//							if (!model)
		//								continue;
		//
		//							for (int b = 0; b < model->numbones; b++) {
		//
		//								/* get the entity bone */
		//								ap::sdk::mstudiobone_t* pBone = model->GetBone(b);
		//
		//								/* if we don't have any valid bone, don't do anything */
		//								if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
		//									continue;
		//
		//								/* bone positions */
		//								static vec2i bone_position, bone_previous_position;
		//
		//								/* check if we have a valid place to draw the skeleton */
		//								if (!renderer::world_to_screen(vec3f(bone_data[eeboy][ree].bone_matrix[b][0][3], bone_data[eeboy][ree].bone_matrix[b][1][3], bone_data[eeboy][ree].bone_matrix[b][2][3]), bone_position))
		//									continue;
		//
		//								if (!renderer::world_to_screen(vec3f(bone_data[eeboy][ree].bone_matrix[pBone->parent][0][3], bone_data[eeboy][ree].bone_matrix[pBone->parent][1][3], bone_data[eeboy][ree].bone_matrix[pBone->parent][2][3]), bone_previous_position))
		//									continue;
		//								mango_local->draw_model(0x1, 255);
		//							}
		//						}
		//					}
		//	
		//				}
		//			}
		//		}
		//	}
	}


	void on_create_move(sdk::c_user_cmd* mango_cmd)
	{
		run(mango_cmd);
		update();
	}

	void on_paint_traverse()
	{
		backtrack_visual_stuff();
	}

	void on_draw_model_execute()
	{
		backtrack_visual_dme();
	}
} // ap::features::backtrack
