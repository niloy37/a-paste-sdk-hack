#pragma once
#include "c_handle.h"
#include "c_client_renderable.h"
#include "c_client_networkable.h"
#include "c_client_thinkable.h"
#include "../misc/vector.h"

class c_client_alpha_property;
class c_base_entity;
class c_client_entity;

class c_collideable
{
public:
	virtual void pad0();
	virtual const vec3f& OBBMins() const;
	virtual const vec3f& OBBMaxs() const;
};

class c_client_unknown : public c_handle_entity
{
public:
	virtual c_collideable*              GetCollideable() = 0;
	virtual c_client_networkable*        GetClientNetworkable() = 0;
	virtual ap::sdk::c_client_renderable*         GetClientRenderable() = 0;
	virtual c_client_entity*             GetIClientEntity() = 0;
	virtual c_base_entity*              GetBaseEntity() = 0;
	virtual c_client_thinkable*          GetClientThinkable() = 0;
	//virtual IClientModelRenderable*  GetClientModelRenderable() = 0;
	virtual c_client_alpha_property*      GetClientAlphaProperty() = 0;
};