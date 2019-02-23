

/* -------------------------------------------------------------------------------------------------------------
  Written by: stacker
  With spoonfeed from: navewindre, wzn, Xer0, FlyingLlama, and arcxne
------------------------------------------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------------------------------------------
  https://developer.valvesoftware.com/wiki/Frame_Order

  CHLClient::FrameStageNotify(FRAME_RENDER_START)
	CInput::CAM_Think()
	CViewRender::OnRenderStart()
	  CViewRender::SetUpView()
		CBasePlayer::CalcView()
	  For every player: CBasePlayer::UpdateClientSideAnimation()
		CMultiPlayerAnimState::Update()
	  ProcessOnDataChangedEvents()
		For every entity with data changed: C_BaseEntity::OnDataChanged()
	  Update entities, tempents, particle systems, simulate physics, call ClientThink() functions
  CHLClient::View_Render()
	Draws world and then UI
  CHLClient::FrameStageNotify(FRAME_RENDER_END)
------------------------------------------------------------------------------------------------------------- */

// Hook to prevent setupbones from being called
//bool __fastcall hooks::in_prediction(void* ecx, void* edx) {
//	for (size_t m{ }; m <= 64; m++) {
//		// Check if entity is the world entity
//		if (!m) {
//			continue;
//		}
//
//		// Grab entity based on loop index
//		auto player = csgo->entity_list()->get_client_entity(m);
//
//		// Entity validation checks
//		if (!player || !player->alive()) {
//			continue;
//		}
//
//		// Prevent setupbones from working when we don’t want it to
//		player->get< int >(0x269C) = globals->allow_setup_bones[m] ? 0 : -1; // m_nReadableBones
//		player->get< int >(0x26A0) = globals->allow_setup_bones[m] ? 0 : -1; // m_nWritableBones
//
//		// m_flLastBoneSetupTime invalidation
//		player->get< float >(0x2914) = globals->allow_setup_bones[m] ? csgo->globals()->curtime : sqrt(-1.f);
//	}
//}
//
//vec_3 c_bone_sync::animation_velocity(c_player * player) {
//	// This is what I consider to be good enough, it can be done better by rebuilding GameMovement
//	data.choked_ticks = std::clamp< int >(TIME_TO_TICKS(player->animation_time() - record.animation_time), 1, 15);
//	return data.origin_delta / (choked_ticks > 0 ? choked_ticks : 1) / csgo->globals()->interval_per_tick;
//}
//
//// Detect when an animation update is received by the client
//bool c_bone_sync::update_received(c_player * player) {
//	return player->animation_time() != record.animation_time;
//}
//
//// Sync client-side matrices to the server’s as best we can
//void c_bone_sync::rebuild_server_variables() {
//	for (size_t m{ }; m <= 64; m++) {
//		// Check if entity is the world entity
//		if (!m) {
//			continue;
//		}
//
//		// Grab entity based on loop index
//		auto player = csgo->entity_list()->get_client_entity(m);
//
//		// Entity validation checks
//		if (!player || !player->alive() || player->dormant()) {
//			continue;
//		}
//
//		// Completely revert animations while local player is dead
//		if (!csgo->local_entity()->alive()) {
//			player->client_side_animation() = true;
//			continue;
//		}
//
//		// Correct render bounds
//		*(vec_3*)(player + 0x318 + 0x8) = data.mins;
//		*(vec_3*)(player + 0x318 + 0x14) = data.maxs;
//
//		// Store curtime and frametime for later restoration
//		float previous_curtime = csgo->globals()->curtime;
//		float previous_frametime = csgo->globals()->frametime;
//
//		// Set global variables to prepare for animation fixing
//		csgo->globals()->curtime = player->animation_time();
//		csgo->globals()->frametime = csgo->globals()->interval_per_tick;
//
//		// Calculate origin delta
//		data.origin_delta = data.origin - record.origin;
//
//		// Store flags and speeds for fixing velocity
//		data.velocity = player->velocity();
//		data.unk_byte = player->get< byte >(0x35f8);
//		data.player_flags = player->get< int >(0xe4);
//
//		// Upon animation time update, reanimate (can be shortened by calling in FRAME_NET_UPDATE_END)
//		if (update_received(player)) {
//			// Fix our velocity 
//			player->get< int >(0x110) = animation_velocity(player);
//			player->get< int >(0x94) = animation_velocity(player);
//			player->get< byte >(0x35f8) = 1;
//			player->get< int >(0xe4) &= ~EFL_DIRTY_VELOCITY; // 0x1000
//
//			// Force the feet yaw rate
//			player->animation_state()->feet_yaw_rate = 0.f;
//
//			// Tell client that we are going to update animations this frame, no matter what
//			player->animation_state()->last_update_framecount -= 1;
//
//			// Resolve our entity’s angles from choking inaccuracies
//			resolver->calculate_best_angle(player);
//
//			// Recalculate goal feet yaw
//			float lby_delta = player->lbyt() - player->angles().y;
//			lby_delta = std::remainderf(lby_delta, 360.f);
//			lby_delta = std::clamp(lby_delta, -60.f, 60.f);
//
//			float feet_yaw = std::remainderf(player->angles().y + lby_delta, 360.f);
//
//			if (feet_yaw < 0.f) {
//				feet_yaw += 360.f;
//			}
//
//			// Apply recalculated feet yaw
//			player->animation_state()->goal_feet_yaw = player->animation_state()->current_feet_yaw = feet_yaw;
//
//			// Allow animations to be updated
//			globals->client_side_animation(player) = true;
//			player->animation_state()->last_update_time = record.animation_time;
//
//			// Update animation state and recalculate everything
//			util->update_animations(player);
//
//			// Store animation layers and pose parameters
//			std::memcpy(data.pose_parameters, player->pose_parameters(), sizeof(float) * 24);
//			std::memcpy(data.animation_layers, player->animation_overlay(), sizeof(c_animation_layer) * 13);
//
//			// Block animations from being manipulated otherwise by the client
//			globals->client_side_animation(player) = false;
//
//
//			// Set new absolute origin
//			player->abs_origin(player->origin());
//
//			// Restore animation layers and pose parameters
//			std::memcpy(player->pose_parameters(), data.pose_parameters, sizeof(float) * 24);
//			std::memcpy(player->animation_overlay(), data.animation_layers, sizeof(c_animation_layer) * 13);
//
//			// Restore global variables
//			csgo->globals()->curtime = previous_curtime;
//			csgo->globals()->frametime = previous_frametime;
//
//			// Restore velocity variables
//			player->get< int >(0x110) = data.velocity;
//			player->get< int >(0x94) = data.velocity;
//			player->get< byte >(0x35f8) = data.unk_byte;
//			player->get< int >(0xe4) = data.player_flags;
//
//			// Set model angle to m_flGoalFeetYaw
//			if (player == csgo->local_entity()) {
//				player->set_abs_angles(vec_3(0.f, player->animation_state()->goal_feet_yaw, 0.f));
//			}
//
//			/*
//			if ( *(_BYTE *)(this + 0x39F5)
//			&& *(_DWORD *)(this + 0x3884)
//			&& !(*(_BYTE *)(this + 0xA28) & 0xE)
//			&& *(_WORD *)(this + 0x26E) != -1
//			&& *(_BYTE *)(this + 0x270) & 1
//			&& (!(unsigned __int8)sub_101D7D40(this) || (unsigned __int8)sub_101D7CE0(v7))
//			&& (*(unsigned __int8 (__thiscall **)(int))(*(_DWORD *)(v7 + 4) + 12))(v7 + 4) )
//			{
//
//			  This method bypasses the call to DoExtraBoneProcessing entirely, which may break some IK calculations.
//		  */
//			byte original = player->read< byte >(0x270);
//			player->get< byte >(0x270) = 0;
//
//			// Setup a new bone matrix
//			player->invalidate_bone_cache();
//			globals->allow_setup_bones[m] = true;
//			player->setup_bones(nullptr, -1, 0x7FF00, player->animation_time());
//
//			// Restore original value
//			player->get< byte >(0x270) = original;
//
//			// Prevent game from setting up bones
//			globals->allow_setup_bones[m] = false;
//		}
//	}
//
//	void evaluate_lod(void* entity) {
//		if (!entity) {
//			return;
//		}
//
//		auto& unk = *(uint32_t*)(uint32_t(entity) + 0xA28);
//		*(uint32_t*)(uint32_t(entity) + 0xA24) = -1;
//		*(uint32_t*)(uint32_t(entity) + 0xA30) = csgo->globals()->framecount;
//		*(uint32_t*)(uint32_t(entity) + 0xA2C) = unk;
//		unk = 0;
//	}
//
//	bool __fastcall hooks::is_hltv(void* ecx, void* edi) {
//		static auto original_fn = csgo->engine()->get_function< decltype(&hooks::is_hltv) >(93);
//
//		// Define the return addresses that we need so our fix can be applied at the correct time
//		static auto setupbones = util->byte_array(“client_panorama.dll”, "E8 ? ? ? ? F3 0F 10 45 ? 51") + 5;
//		static auto evalanimlod = util->byte_array(“client_panorama.dll”, "84 C0 0F 85 ? ? ? ? A1 ? ? ? ? 8B B7");
//
//		// Get access to the stack so we can check the function’s return addresses
//		stack_t stack{ };
//
//		// Check if the current return address points to CCSPlayer_EvalAnimLod
//		if (stack.return_address() == evalanimlod) {
//			// Check if the previous return address in the stack points to CCSPlayer_SetupBones
//			if (stack.previous().return_address() == setupbones) {
//				evaluate_lod(edi);
//				return true;
//			}
//		}
//
//		return original_fn(ecx);
//	}
//
//	// (c) wzn
//	__declspec(naked) bool __fastcall hooks::is_hltv_gate(void*, void*) {
//		__asm {
//			mov 	edx, edi
//			jmp		is_hltv
//		}
//	}
//
//	/* -------------------------------------------------------------------------------------------------------------
//	  The animation fix is pretty well polished, but is missing a few key things that I left out. One of which is a
//	  jump_fall fix, but that can be recreated in about 5 lines by simulating PoseParameter 6. Other than that, I
//	  would like to thank everyone who contributed to this project and it’s elements as a whole.
//	------------------------------------------------------------------------------------------------------------- */
//