#pragma once

#include "../misc/vector.h"


namespace ap::sdk
{
	class c_base_entity;

	class c_game_movement
	{
	public:
		virtual			~c_game_movement(void) {}

		virtual void	ProcessMovement(c_base_entity *pPlayer, void *pMove) = 0;
		virtual void	Reset(void) = 0;
		virtual void	StartTrackPredictionErrors(c_base_entity *pPlayer) = 0;
		virtual void	FinishTrackPredictionErrors(c_base_entity *pPlayer) = 0;
		virtual void	DiffPrint(char const *fmt, ...) = 0;

		virtual vec3f const&	GetPlayerMins(bool ducked) const = 0;
		virtual vec3f const&	GetPlayerMaxs(bool ducked) const = 0;
		virtual vec3f const&    GetPlayerViewOffset(bool ducked) const = 0;

		virtual bool			IsMovingPlayerStuck(void) const = 0;
		virtual c_base_entity*	GetMovingPlayer(void) const = 0;
		virtual void			UnblockPusher(c_base_entity* pPlayer, c_base_entity *pPusher) = 0;

		virtual void SetupMovementBounds(void *pMove) = 0;
	};
} // namespace ap::sdk