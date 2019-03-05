#pragma once
#include "../features/ragebot.h"
namespace ap::features::ragebot {
	/*void auto_stop()
	{
		if (!ap::settings::auto_stop)
			return;

		vec3f Velocity = g::mango_local->get_velocity();
		static float Speed = 450.f;

		vec3f Direction;
		vec3f RealView;
		ap::vec3f_angle(Velocity, Direction);
		ap::interfaces::engine->get_viewangles(RealView);
		Direction[1] = RealView[1] - Direction[1];

		vec3f Forward;
		ap::vec3f_angle(Direction, &Forward);
		vec3f NegativeDirection = Forward * -Speed;

		g::mango_cmd->forwardmove = NegativeDirection[0];
		g::mango_cmd->sidemove = NegativeDirection[1];
	}

	bool hit_chance(ap::sdk::c_base_entity* pEnt, ap::sdk::c_base_weapon* pWeapon, vec3f Angle, vec3f Point, int chance)
	{
		if (chance == 0 || ap::settings::hit_chance == 0)
			return true;

		if (Backtrack[pEnt->EntIndex()] || ShotBacktrack[pEnt->EntIndex()]) // doing this bec im lazy
		{
			vec3f ree = g::mango_local->get_velocity();
			float Velocity = (vec_length(ree)); 

			if (Velocity <= (g::mango_local->get_active_weapon()->GetCSWpnData()->max_speed_alt * .34f))
				Velocity = 0.0f;

			float SpreadCone = pWeapon->update_accuracy_penalty() * 256.0f / pi + pWeapon->get_weapon_info()->max_speed * Velocity / 3000.0f; // kmeth https://github.com/DankPaster/kmethdude
			float ribbity = g::mango_local->get_eye_position();
			float a = (vec_length(Point - ribbity));
			float b = float(tan(SpreadCone * pi / 180.0f) * a);
			if (2.2f > b) return true;
			return (chance <= ((2.2f / fmax(b, 2.2f)) * 100.0f));
		}

		float Seeds = (g_Menu.Config.Hitchance == 1) ? 356.f : 256.f;

		Angle -= (g::mango_local->get_aim_punch() * ap::interfaces::cvar->find_var("weapon_recoil_scale")->get_float());

		vec3f forward, right, up;

		ap::angle_vec3f(Angle, &forward, &right, &up);

		int Hits = 0, neededHits = (Seeds * (chance / 100.f));

		float weapSpread = pWeapon->GetSpread(), weapInaccuracy = pWeapon->GetInaccuracy();

		for (int i = 0; i < Seeds; i++)
		{
			float Inaccuracy = g_Math.RandomFloat(0.f, 1.f) * weapInaccuracy;
			float Spread = g_Math.RandomFloat(0.f, 1.f) * weapSpread;

			vec3fspreadView((cos(g_Math.RandomFloat(0.f, 2.f * pi)) * Inaccuracy) + (cos(g_Math.RandomFloat(0.f, 2.f * pi)) * Spread), (sin(g_Math.RandomFloat(0.f, 2.f * pi)) * Inaccuracy) + (sin(g_Math.RandomFloat(0.f, 2.f * pi)) * Spread), 0), direction;
			direction = Vector(forward[0] + (spreadView[0] * right[0]) + (spreadView[1] * up[0]), forward[1] + (spreadView[0] * right[1]) + (spreadView[1] * up[1]), forward[2] + (spreadView[0] * right[2]) + (spreadView[1] * up[2])).Normalize();

			vec3fviewanglesSpread, viewForward;

			g_Math.VectorAngles(direction, up, viewanglesSpread);
			g_Math.NormalizeAngles(viewanglesSpread);

			g_Math.AngleVectors(viewanglesSpread, &viewForward);
			viewForward.NormalizeInPlace();

			viewForward = g::mango_local->GetEyePosition() + (viewForward * pWeapon->GetCSWpnData()->range);

			C_Trace Trace;

			g_pTrace->ClipRayToEntity(C_Ray(g::mango_local->GetEyePosition(), viewForward), mask_shot | contents_grate, pEnt, &Trace);

			if (Trace.m_pEnt == pEnt)
				Hits++;

			if (((Hits / Seeds) * 100.f) >= chance)
				return true;

			if ((Seeds - i + Hits) < neededHits)
				return false;
		}

		return false;
	}

	bool should_baim(ap::sdk::c_base_entity * pEnt) // probably dosnt make sense
	{
		static float oldSimtime[65];
		static float storedSimtime[65];

		static float ShotTime[65];
		static float NextShotTime[65];
		static bool BaimShot[65];

		if (storedSimtime[pEnt->EntIndex()] != pEnt->get_simulation_time())
		{
			oldSimtime[pEnt->EntIndex()] = storedSimtime[pEnt->EntIndex()];
			storedSimtime[pEnt->EntIndex()] = pEnt->get_simulation_time();
		}

		float simDelta = storedSimtime[pEnt->EntIndex()] - oldSimtime[pEnt->EntIndex()];

		bool Shot = false;

		if (pEnt->get_active_weapon() && !pEnt->IsKnifeorNade())
		{
			if (ShotTime[pEnt->EntIndex()] != pEnt->get_active_weapon()->GetLastShotTime())
			{
				Shot = true;
				BaimShot[pEnt->EntIndex()] = false;
				ShotTime[pEnt->EntIndex()] = pEnt->get_active_weapon()->GetLastShotTime();
			}
			else
				Shot = false;
		}
		else
		{
			Shot = false;
			ShotTime[pEnt->EntIndex()] = 0.f;
		}

		if (Shot)
		{
			NextShotTime[pEnt->EntIndex()] = pEnt->get_active_weapon() + pEnt->FireRate();

			if (simDelta >= pEnt->FireRate())
				BaimShot[pEnt->EntIndex()] = true;
		}

		if (BaimShot[pEnt->EntIndex()])
		{
			if (pEnt->get_simulation_time() >= NextShotTime[pEnt->EntIndex()])
				BaimShot[pEnt->EntIndex()] = false;
		}

		if (g_Menu.Config.BaimPitch && BaimShot[pEnt->EntIndex()] && !(pEnt->get_flags() & FL_ONGROUND))
			return true;

		if (g_Menu.Config.BaimInAir && !(pEnt->get_flags() & FL_ONGROUND))
			return true;

		return false;
	}

	vec3f hit_scan(ap::sdk::c_base_entity * pEnt) // supremeemmemememememe
	{
		float DamageArray[28];
		float tempDmg = 0.f;
		vec3f tempHitbox;
		static int HitboxForMuti[] = { 2,2,4,4,6,6 };

		float angToLocal = ap::calc_angle(g::mango_local->get_vec_origin(), pEnt->get_vec_origin())[1];

		vec2i MutipointXY = { (sin(g_Math.GRD_TO_BOG(angToLocal))),(cos(g_Math.GRD_TO_BOG(angToLocal))) };
		vec2i MutipointXY180 = { (sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };
		vec2i Mutipoint[] = { (vec2i(MutipointXY[0], MutipointXY[1])), (vec2i(MutipointXY180[0], MutipointXY180[1])) };

		float Velocity = abs(pEnt->get_velocity().Length2D());

		if (!g_Menu.Config.DelayShot && Velocity > 29.f)
			Velocity = 30.f;

		std::vector<int> Scan;

		int HeadHeight = 0;

		bool Baim = should_baim(pEnt);

		if (!Baim)
			Scan.push_back(HITBOX_HEAD);

		if (Velocity <= 215.f || Baim)
		{
			Scan.push_back(HITBOX_PELVIS);
			Scan.push_back(HITBOX_THORAX);
			Scan.push_back(HITBOX_LOWER_CHEST);
			Scan.push_back(HITBOX_UPPER_CHEST);

			if (g_Menu.Config.MultiPoint)
			{
				Scan.push_back(19);//pelvis
				Scan.push_back(20);

				Scan.push_back(21);//thorax
				Scan.push_back(22);

				Scan.push_back(23);//upperchest
				Scan.push_back(24);

				if (!Baim)
				{
					Scan.push_back(25);//head
					Scan.push_back(26);
					Scan.push_back(27);
				}

				HeadHeight = g_Menu.Config.HeadScale;
			}

			if (!g_Menu.Config.IgnoreLimbs)
				Velocity = 0.f;

			if (Velocity <= 29.f)
			{
				Scan.push_back(HITBOX_LEFT_FOOT);
				Scan.push_back(HITBOX_RIGHT_FOOT);
				Scan.push_back(HITBOX_LEFT_UPPER_ARM);
				Scan.push_back(HITBOX_RIGHT_UPPER_ARM);
				Scan.push_back(HITBOX_LEFT_THIGH);
				Scan.push_back(HITBOX_RIGHT_THIGH);
			}
		}

		vec3f Hitbox;
		int bestHitboxint = 0;

		for (int hitbox : Scan)
		{
			if (hitbox < 19)
				Hitbox = pEnt->get_hitbox_position(hitbox, Matrix[pEnt->EntIndex()]);
			else if (hitbox > 18 && hitbox < 25)
			{
				float Radius = 0;
				Hitbox = pEnt->GetHitboxPosition(HitboxForMuti[hitbox - 19], Matrix[pEnt->EntIndex()], &Radius);
				Radius *= (g_Menu.Config.BodyScale / 100.f);
				Hitbox = vec3f(Hitbox[0] + (Radius * Mutipoint[((hitbox - 19) % 2)][0]), Hitbox[1] - (Radius * Mutipoint[((hitbox - 19) % 2)][1]), Hitbox[2]);
			}
			else if (hitbox > 24 && hitbox < 28)
			{
				float Radius = 0;
				Hitbox = pEnt->GetHitboxPosition(0, Matrix[pEnt->EntIndex()], &Radius);
				Radius *= (HeadHeight / 100.f);
				if (hitbox != 27)
					Hitbox = vec3f(Hitbox[0] + (Radius * Mutipoint[((hitbox - 25) % 2)][0]), Hitbox[1] - (Radius * Mutipoint[((hitbox - 25) % 2)][1]), Hitbox[2]);
				else
					Hitbox += vec3f(0, 0, Radius);
			}

			float Damage = ap::features::autowall::damage(Hitbox);

			if (Damage > 0.f)
				DamageArray[hitbox] = Damage;
			else
				DamageArray[hitbox] = 0;

			if (g_Menu.Config.BaimLethal && hitbox != 0 && hitbox != 25 && hitbox != 26 && hitbox != 27 && Damage >= (pEnt->GetHealth() + 10))
			{
				DamageArray[hitbox] = 400;
				Baim = true;
			}

			if (DamageArray[hitbox] > tempDmg)
			{
				tempHitbox = Hitbox;
				bestHitboxint = hitbox;
				tempDmg = DamageArray[hitbox];
			}

			g::AimbotHitbox[pEnt->EntIndex()][hitbox] = Hitbox;
		}

		PlayerRecords pPlayerEntityRecord = g_LagComp.PlayerRecord[pEnt->EntIndex()].at(0);

		Backtrack[pEnt->EntIndex()] = false;
		ShotBacktrack[pEnt->EntIndex()] = false;

		if (g_Menu.Config.ShotBacktrack && g_LagComp.ShotTick[pEnt->EntIndex()] != -1 && g_Autowall.CanHitFloatingPoint(pEnt->GetHitboxPosition(HITBOX_HEAD, g_LagComp.PlayerRecord[pEnt->EntIndex()].at(g_LagComp.ShotTick[pEnt->EntIndex()]).Matrix) + Vector(0, 0, 1), g::mango_local->GetEyePosition()) && !Baim)
		{
			bestEntDmg = (1000000.f - fabs(g_Math.Distance(vec2i(g::mango_local->get_vec_origin()[0], g::mango_local->get_vec_origin()[1]), vec2i(pEnt->get_vec_origin()[0], pEnt->get_vec_origin()[1])))); // just doing this to get the closest player im backtracking
			ShotBacktrack[pEnt->EntIndex()] = true;
			return pEnt->GetHitboxPosition(HITBOX_HEAD, g_LagComp.PlayerRecord[pEnt->EntIndex()].at(g_LagComp.ShotTick[pEnt->EntIndex()]).Matrix) + Vector(0, 0, 1);
		}
		else if (tempDmg >= g_Menu.Config.Mindmg)
		{
			bestEntDmg = tempDmg;

			if ((bestHitboxint == 25 || bestHitboxint == 26 || bestHitboxint == 27) && abs(DamageArray[HITBOX_HEAD] - DamageArray[bestHitboxint]) <= 10.f)
				return pEnt->GetHitboxPosition(HITBOX_HEAD, Matrix[pEnt->EntIndex()]);
			else if ((bestHitboxint == 19 || bestHitboxint == 20) && DamageArray[HITBOX_PELVIS] > 30)
				return pEnt->GetHitboxPosition(HITBOX_PELVIS, Matrix[pEnt->EntIndex()]);
			else if ((bestHitboxint == 21 || bestHitboxint == 22) && DamageArray[HITBOX_THORAX] > 30)
				return pEnt->GetHitboxPosition(HITBOX_THORAX, Matrix[pEnt->EntIndex()]);
			else if ((bestHitboxint == 23 || bestHitboxint == 24) && DamageArray[HITBOX_UPPER_CHEST] > 30)
				return pEnt->GetHitboxPosition(HITBOX_UPPER_CHEST, Matrix[pEnt->EntIndex()]);

			return tempHitbox;
		}
		else if (g_Menu.Config.PosBacktrack && pPlayerEntityRecord.Velocity >= 29.f && g_Autowall.CanHitFloatingPoint(pEnt->GetHitboxPosition(HITBOX_HEAD, pPlayerEntityRecord.Matrix), g::mango_local->GetEyePosition()))
		{
			bestEntDmg = (100000.f - fabs(g_Math.Distance(Vector2D(g::mango_local->GetOrigin()[0], g::mango_local->GetOrigin()[1]), Vector2D(pEnt->GetOrigin()[0], pEnt->GetOrigin()[1]))));
			Backtrack[pEnt->EntIndex()] = true;
			return pEnt->GetHitboxPosition(HITBOX_HEAD, pPlayerEntityRecord.Matrix);
		}

		return Vector(0, 0, 0);
	}

	void OnCreateMove()
	{
		if (!ap::interfaces::engine->is_in_game())
			return;

		vec3f Aimpoint;
		ap::sdk::c_base_entity* Target = nullptr;

		int targetID = 0;
		int tempDmg = 0;
		static bool shot = false;

		for (int i = 1; i <= interfaces::globals->maxclients; ++i)
		{
			sdk::c_base_entity* pPlayerEntity = ap::interfaces::client_entity_list->get_client_entity(i);

			if (!pPlayerEntity
				|| !pPlayerEntity->is_alive()
				|| pPlayerEntity->IsDormant())
			{
				g_LagComp.ClearRecords(i);
				continue;
			}

			g_LagComp.StoreRecord(pPlayerEntity);

			if (pPlayerEntity == g::mango_local || pPlayerEntity->GetTeam() == g::mango_local->get_team_num())
				continue;

			g::enemy_eye_angles[i] = pPlayerEntity->GetEyeAngles();

			if (g_LagComp.PlayerRecord[i].size() == 0 || !g::mango_local->is_alive() || !g_Menu.Config.Aimbot || g_Menu.Config.LegitBacktrack)
				continue;

			if (!g::mango_local->get_active_weapon() || g::mango_local->IsKnifeorNade())
				continue;

			bestEntDmg = 0;

			vec3f Hitbox = hit_scan(pPlayerEntity);
			vec3f ribbey;
			if (Hitbox != ribbey && tempDmg <= bestEntDmg)
			{
				Aimpoint = Hitbox;
				Target = pPlayerEntity;
				targetID = Target->EntIndex();
				tempDmg = bestEntDmg;
			}
		}

		if (!g::mango_local->is_alive())
		{
			shot = false;
			return;
		}

		if (!g::mango_local->get_active_weapon() || g::mango_local->IsKnifeorNade())
		{
			shot = false;
			return;
		}

		if (shot)
		{
			if (g_Menu.Config.FixShotPitch) // ik it dosnt realy fix much just makes ur pitch go down faster
			{
				g::bSendPacket = true;
				g_AntiAim.OnCreateMove();
			}
			shot = false;
		}

		float flServerTime = g::mango_local->get_tick_base() * ap::interfaces::globals->interval_per_tick;
		bool canShoot = (g::mango_local->get_active_weapon()->GetNextPrimaryAttack() <= flServerTime);

		if (Target)
		{
			g::target_index = targetID;

			float SimulationTime = 0.f;

			if (Backtrack[targetID])
				SimulationTime = g_LagComp.PlayerRecord[targetID].at(0).SimTime;
			else
				SimulationTime = g_LagComp.PlayerRecord[targetID].at(g_LagComp.PlayerRecord[targetID].size() - 1).SimTime;

			if (ShotBacktrack[targetID])
				SimulationTime = g_LagComp.PlayerRecord[targetID].at(g_LagComp.ShotTick[targetID]).SimTime;

			vec3f Angle = ap::calc_angle(g::mango_local->get_eye_position(), Aimpoint);
			auto rommney = g::mango_local->get_velocity();
			if (vec_length(rommney) >= (g::mango_local->get_active_weapon()->GetCSWpnData()->max_speed_alt * .34f) - 5 && !GetAsyncKeyState(VK_SPACE))
				auto_stop();

			if (!(g::mango_cmd->buttons & IN_ATTACK) && canShoot && hit_chance(Target, g::mango_local->get_active_weapon(), Angle, Aimpoint, g_Menu.Config.HitchanceValue))
			{

				if (!Backtrack[targetID] && !ShotBacktrack[targetID])
					g::shot[targetID] = true;

				if (g_Menu.Config.Ak47meme)
					ap::interfaces::engine->exec_client_cmd("play weapons\\ak47\\ak47-1.wav");

				g::bSendPacket = true;
				shot = true;

				g::mango_cmd->viewangles = Angle - (g::mango_local->get_aim_punch() * ap::interfaces::cvar->find_var("weapon_recoil_scale")->get_float());
				g::mango_cmd->buttons |= IN_ATTACK;
				g::mango_cmd->tick_count = TIME_TO_TICKS(SimulationTime + ap::features::backtrack::lerp_time());
			}
		}
	}*/
}