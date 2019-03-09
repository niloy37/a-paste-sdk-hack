#pragma once

#include "../misc/vmt.h"
#include "../misc/vector.h"

#include "c_base_entity.h"


#define	CONTENTS_EMPTY			0		/**< No contents. */
#define	CONTENTS_SOLID			0x1		/**< an eye is never valid in a solid . */
#define	CONTENTS_WINDOW			0x2		/**< translucent, but not watery (glass). */
#define	CONTENTS_AUX			0x4
#define	CONTENTS_GRATE			0x8		/**< alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't. */
#define	CONTENTS_SLIME			0x10
#define	CONTENTS_WATER			0x20
#define	CONTENTS_MIST			0x40
#define CONTENTS_OPAQUE			0x80		/**< things that cannot be seen through (may be non-solid though). */
#define	LAST_VISIBLE_CONTENTS	0x80
#define ALL_VISIBLE_CONTENTS 	(LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))
#define CONTENTS_TESTFOGVOLUME	0x100
#define CONTENTS_UNUSED5		0x200
#define CONTENTS_UNUSED6		0x4000
#define CONTENTS_TEAM1			0x800		/**< per team contents used to differentiate collisions. */
#define CONTENTS_TEAM2			0x1000		/**< between players and objects on different teams. */
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000		/**< ignore CONTENTS_OPAQUE on surfaces that have SURF_NODRAW. */
#define CONTENTS_MOVEABLE		0x4000		/**< hits entities which are MOVETYPE_PUSH (doors, plats, etc) */
#define	CONTENTS_AREAPORTAL		0x8000		/**< remaining contents are non-visible, and don't eat brushes. */
#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_MONSTERCLIP	0x20000

/**
* @section currents can be added to any other contents, and may be mixed
*/
#define	CONTENTS_CURRENT_0		0x40000
#define	CONTENTS_CURRENT_90		0x80000
#define	CONTENTS_CURRENT_180	0x100000
#define	CONTENTS_CURRENT_270	0x200000
#define	CONTENTS_CURRENT_UP		0x400000
#define	CONTENTS_CURRENT_DOWN	0x800000

/**
* @endsection
*/

#define	CONTENTS_ORIGIN			0x1000000	/**< removed before bsping an entity. */
#define	CONTENTS_MONSTER		0x2000000	/**< should never be on a brush, only in game. */
#define	CONTENTS_DEBRIS			0x4000000
#define	CONTENTS_DETAIL			0x8000000	/**< brushes to be added after vis leafs. */
#define	CONTENTS_TRANSLUCENT	0x10000000	/**< auto set if any surface has trans. */
#define	CONTENTS_LADDER			0x20000000
#define CONTENTS_HITBOX			0x40000000	/**< use accurate hitboxes on trace. */

/**
* @section Trace masks.
*/
#define	MASK_ALL				(0xFFFFFFFF)
#define	MASK_SOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) 			/**< everything that is normally solid */
#define	MASK_PLAYERSOLID		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) 	/**< everything that blocks player movement */
#define	MASK_NPCSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) /**< blocks npc movement */
#define	MASK_WATER				(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME) 							/**< water physics in these contents */
#define	MASK_OPAQUE				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE) 							/**< everything that blocks line of sight for AI, lighting, etc */
#define MASK_OPAQUE_AND_NPCS	(MASK_OPAQUE|CONTENTS_MONSTER)										/**< everything that blocks line of sight for AI, lighting, etc, but with monsters added. */
#define	MASK_VISIBLE			(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE) 								/**< everything that blocks line of sight for players */
#define MASK_VISIBLE_AND_NPCS	(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE) 							/**< everything that blocks line of sight for players, but with monsters added. */
#define	MASK_SHOT				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX) 	/**< bullets see these as solid */
#define MASK_SHOT_HULL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE) 	/**< non-raycasted weapons see this as solid (includes grates) */
#define MASK_SHOT_PORTAL		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW) 							/**< hits solids (not grates) and passes through everything else */
#define MASK_SOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE) 					/**< everything normally solid, except monsters (world+brush only) */
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE) 			/**< everything normally solid for player movement, except monsters (world+brush only) */
#define MASK_NPCSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE) 			/**< everything normally solid for npc movement, except monsters (world+brush only) */
#define MASK_NPCWORLDSTATIC		(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE) 					/**< just the world, used for route rebuilding */
#define MASK_SPLITAREAPORTAL	(CONTENTS_WATER|CONTENTS_SLIME) 									/**< These are things that can split areaportals */



namespace ap::sdk
{
	class IHandleEntity;

	class __declspec(align(16))vec3fAligned : public vec3f
	{
	public:
		vec3fAligned& operator=(const vec3f &vOther)
		{
			at(0) = vOther[0];
			at(1) = vOther[1];
			at(2) = vOther[2];
			return *this;
		}
		float w;
	};

	struct Ray_t
	{
		vec3fAligned  m_Start;    // starting point, centered within the extents
		vec3fAligned  m_Delta;    // direction + length of the ray
		vec3fAligned  m_StartOffset;    // Add this to m_Start to get the actual ray start
		vec3fAligned  m_Extents;    // Describes an axis aligned box extruded along a ray
		const matrix3x4_t *m_pWorldAxisTransform;
		bool    m_IsRay;    // are the extents zero?
		bool    m_IsSwept;    // is delta != 0?

		Ray_t() = default;

		Ray_t(vec3f _start, vec3f _end)
		{
			Init(_start, _end);
		}

		Ray_t(vec3f _start, vec3f _end, vec3f _mins, vec3f _maxs)
		{
			Init(_start, _end, _mins, _maxs);
		}

		void Init(const vec3f& vecStart, const vec3f& vecEnd)
		{
			m_Delta = vecEnd - vecStart;

			m_IsSwept = (vec_length(m_Delta) != 0);

			m_Extents[0] = m_Extents[1] = m_Extents[2] = 0.0f;
			m_IsRay = true;

			m_StartOffset[0] = m_StartOffset[1] = m_StartOffset[2] = 0.0f;

			m_Start = vecStart;
		}
		void Init(vec3f& vecStart, vec3f& vecEnd, vec3f min, vec3f max)
		{
			m_Delta = vecEnd - vecStart;

			m_IsSwept = (vec_length(m_Delta) != 0);

			m_Extents[0] = (max[0] - min[0]);
			m_Extents[1] = (max[1] - min[1]);
			m_Extents[2] = (max[2] - min[2]);
			m_IsRay = false;

			m_StartOffset[0] = m_StartOffset[1] = m_StartOffset[2] = 0.0f;

			m_Start = vecStart + ((max + min) * 0.5f);
		}


	private:
	};

	struct csurface_t
	{
		const char *name;
		short surfaceProps;
		unsigned short flags;
	};

	struct cplane_t
	{
		vec3f normal;
		float dist;
		byte type;
		byte signbits;
		byte pad[2];
	};
	struct trace_t
	{
		bool DidHitWorld() const
		{
			return m_pEnt && m_pEnt->get_index() == 0;
		}
		bool DidHitNonWorldEntity() const
		{
			return m_pEnt != NULL && !DidHitWorld();
		}
		bool DidHit() const
		{
			return flFraction < 1.f;
		}

		vec3f start;
		vec3f end;
		cplane_t plane;
		float flFraction;
		int contents;
		WORD dispFlags;
		bool allsolid;
		bool startSolid;
		float fractionLeftSolid;
		csurface_t surface;
		int hitGroup;
		short physicsBone;
		c_base_entity* m_pEnt;
		int hitbox;
	};

	enum TraceType_t
	{
		TRACE_EVERYTHING = 0,
		TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
		TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
		TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
	};
	class ITraceFilter
	{
	public:
		virtual bool should_hit_entity(void *pEntity, int contentsMask) = 0;
		virtual TraceType_t get_trace_type() const = 0;
	};

	class CTraceFilter : public ITraceFilter
	{
	public:

		explicit CTraceFilter(c_base_entity* entity, TraceType_t tracetype = TRACE_EVERYTHING)
		{
			pSkip1 = entity;
		}

		bool should_hit_entity(void* pEntityHandle, int contentsMask)
		{
			return (pEntityHandle != pSkip1);
		}

		TraceType_t get_trace_type() const
		{
			return TRACE_EVERYTHING;
		}

		void* pSkip1;
	};
	class CTraceFilterOneEntity : public ITraceFilter
	{
	public:
		bool should_hit_entity(void* pEntityHandle, int contentsMask)
		{
			return (pEntityHandle == pEntity);
		}

		TraceType_t get_trace_type() const
		{
			return TRACE_EVERYTHING;
		}

		void* pEntity;
	};
	class CTraceFilterOneEntity2 : public ITraceFilter
	{
	public:
		bool should_hit_entity(void* pEntityHandle, int contentsMask)
		{
			return (pEntityHandle == pEntity);
		}

		TraceType_t get_trace_type() const
		{
			return TRACE_ENTITIES_ONLY;
		}

		void* pEntity;
	};

	class CTraceFilterSkipTwoEntities : public ITraceFilter
	{
	public:
		CTraceFilterSkipTwoEntities(void *pPassEnt1, void *pPassEnt2)
		{
			passentity1 = pPassEnt1;
			passentity2 = pPassEnt2;
		}

		virtual bool should_hit_entity(void *pEntityHandle, int contentsMask)
		{
			return !(pEntityHandle == passentity1 || pEntityHandle == passentity2);
		}

		virtual TraceType_t    get_trace_type() const
		{
			return TRACE_EVERYTHING;
		}

		void *passentity1;
		void *passentity2;
	};

	class CTraceEntity : public ITraceFilter
	{
	public:
		bool should_hit_entity(void* pEntityHandle, int contentsMask)
		{
			return !(pEntityHandle == pSkip1);
		}

		TraceType_t get_trace_type() const
		{
			return TRACE_ENTITIES_ONLY;
		}

		void* pSkip1;
	};
	class CTraceWorldOnly : public ITraceFilter
	{
	public:
		bool should_hit_entity(void* pEntityHandle, int contentsMask)
		{
			return false;
		}

		TraceType_t get_trace_type() const
		{
			return TRACE_EVERYTHING;
		}

		void* pSkip1;
	};

	class c_trace 
	{
	public:
		int get_point_contents(const vec3f &vecAbsPosition, int contentsMask = MASK_ALL, IHandleEntity** ppEntity = NULL)
		{
			using fn = int(__thiscall*)(void*, const vec3f&, int, IHandleEntity**);
			return vmt::get_vfunc<fn>(this, 0)(this, vecAbsPosition, contentsMask, ppEntity);
		}
		void trace_ray(const Ray_t &ray, unsigned int fMask, ITraceFilter* filter, trace_t *trace)
		{
			using fn = void(__thiscall*)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
			return vmt::get_vfunc<fn>(this, 5)(this, ray, fMask, filter, trace);
		}
		void clip_ray_to_exit(const Ray_t &ray, unsigned int fMask, c_base_entity *pEnt, trace_t *pTrace)
		{
			using fn = void(__thiscall*)(void*, const Ray_t&, unsigned int, c_base_entity*, trace_t*);
			return vmt::get_vfunc<fn>(this, 3)(this, ray, fMask, pEnt, pTrace);
		}
		bool is_visible(ap::sdk::c_base_entity* pLocalClientBaseCBaseEntity, vec3f vecOrigin, vec3f vecFinal, ap::sdk::c_base_entity* pClientBaseCBaseEntity, int& hitgroup) {
			Ray_t ray;
			ray.Init(vecOrigin, vecFinal);

			CTraceFilter* TraceFilter;
			TraceFilter->pSkip1 = pLocalClientBaseCBaseEntity;

			trace_t trace;
			trace_ray(ray, MASK_SHOT, TraceFilter, &trace);
			hitgroup = trace.hitbox;
			delete TraceFilter;
			return (trace.m_pEnt == pClientBaseCBaseEntity || trace.flFraction >= 1.0f);
		}
	private:
	};
} // namespace ap::sdk