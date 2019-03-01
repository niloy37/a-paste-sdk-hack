#pragma once

#include "../misc/vmt.h"
#include "../misc/vector.h"
#include "../misc/matrix.h"


namespace ap::sdk
{
	struct model_t;
	using Quaternion = c_vector<float, 4>;

	struct mstudiobbox_t
	{
		int					bone; // 0x0
		int					group; // 0x4
		vec3f				bbmin; // 0x8
		vec3f				bbmax; // 0x14
		int					szhitboxnameindex; // 0x20
		int					m_iPad01[3];
		float				m_flRadius;
		int					m_iPad02[4];
	};

	struct mstudiobone_t
	{
		int					sznameindex;
		inline char * const pszName(void) const { return ((char *)this) + sznameindex; }
		int		 			parent;		// parent bone
		int					bonecontroller[6];	// bone controller index, -1 == none

												// default values
		vec3f				pos;
		Quaternion			quat;
		vec3f			rot;
		// compression scale
		vec3f				posscale;
		vec3f				rotscale;

		matrix3x4_t			poseToBone;
		Quaternion			qAlignment;
		int					flags;
		int					proctype;
		int					procindex;		// procedural rule
		mutable int			physicsbone;	// index into physically simulated bone
		inline void *pProcedure() const { if (procindex == 0) return NULL; else return  (void *)(((byte *)this) + procindex); };
		int					surfacepropidx;	// index into string tablefor property name
		inline char * const pszSurfaceProp(void) const { return ((char *)this) + surfacepropidx; }
		int					contents;		// See BSPFlags.h for the contents flags

		int					unused[8];		// remove as appropriate

		mstudiobone_t() {}
	private:
		// No copy constructors allowed
		mstudiobone_t(const mstudiobone_t& vOther);
	};
	struct mstudiohitboxset_t
	{
		int                                             sznameindex;
		inline char* const              GetName(void) const { return ((char*)this) + sznameindex; }
		int                                             numhitboxes;
		int                                             hitboxindex;
		inline mstudiobbox_t *GetHitbox(int i) const { return (mstudiobbox_t*)(((unsigned char*)this) + hitboxindex) + i; };
	};
	struct studiohdr_t
	{
		int                                     id;
		int                                     version;

		int                                     checksum;

		char                            name[64];
		int                                     length;


		vec3f                          eyeposition;

		vec3f                          illumposition;

		vec3f                          hull_min;
		vec3f                          hull_max;

		vec3f                          view_bbmin;
		vec3f                          view_bbmax;

		int                                     flags;

		int                                     numbones;
		int                                     boneindex;

		inline mstudiobone_t *GetBone(int i) const { return (mstudiobone_t *)(((byte *)this) + boneindex) + i; };
		//	inline mstudiobone_t *pBone(int i) const { Assert(i >= 0 && i < numbones); return (mstudiobone_t *)(((byte *)this) + boneindex) + i; };

		int                                     numbonecontrollers;
		int                                     bonecontrollerindex;

		int                                     numhitboxsets;
		int                                     hitboxsetindex;

		mstudiohitboxset_t* GetHitboxSet(int i) const
		{
			return (mstudiohitboxset_t*)(((byte*)this) + hitboxsetindex) + i;
		}

		inline mstudiobbox_t* GetHitbox(int i, int set) const
		{
			mstudiohitboxset_t const* s = GetHitboxSet(set);

			if (!s)
				return NULL;

			return s->GetHitbox(i);
		}

		inline int GetHitboxCount(int set) const
		{
			mstudiohitboxset_t const* s = GetHitboxSet(set);

			if (!s)
				return 0;

			return s->numhitboxes;
		}

		int                                     numlocalanim;
		int                                     localanimindex;

		int                                     numlocalseq;
		int                                     localseqindex;

		mutable int                     activitylistversion;
		mutable int                     eventsindexed;

		int                                     numtextures;
		int                                     textureindex;

		int                                     numcdtextures;
		int                                     cdtextureindex;

		int                                     numskinref;
		int                                     numskinfamilies;
		int                                     skinindex;

		int                                     numbodyparts;
		int                                     bodypartindex;

		int                                     numlocalattachments;
		int                                     localattachmentindex;

		int                                     numlocalnodes;
		int                                     localnodeindex;
		int                                     localnodenameindex;

		int                                     numflexdesc;
		int                                     flexdescindex;

		int                                     numflexcontrollers;
		int                                     flexcontrollerindex;

		int                                     numflexrules;
		int                                     flexruleindex;

		int                                     numikchains;
		int                                     ikchainindex;

		int                                     nummouths;
		int                                     mouthindex;

		int                                     numlocalposeparameters;
		int                                     localposeparamindex;

		int                                     surfacepropindex;

		int                                     keyvalueindex;
		int                                     keyvaluesize;


		int                                     numlocalikautoplaylocks;
		int                                     localikautoplaylockindex;

		float                           mass;
		int                                     contents;

		int                                     numincludemodels;
		int                                     includemodelindex;

		mutable void            *virtualModel;

		int                                     szanimblocknameindex;
		int                                     numanimblocks;
		int                                     animblockindex;

		mutable void            *animblockModel;

		int                                     bonetablebynameindex;

		void                            *pVertexBase;
		void                            *pIndexBase;

		byte                            constdirectionallightdot;

		byte                            rootLOD;

		byte                            numAllowedRootLODs;

		byte                            unused[1];

		int                                     unused4;

		int                                     numflexcontrollerui;
		int                                     flexcontrolleruiindex;
		float                           flVertAnimFixedPointScale;
		int                                     unused3[1];
		int                                     studiohdr2index;
		int                                     unused2[1];
	};

	class c_model_info
	{
	public:
		studiohdr_t* get_studio_hdr(const model_t* model)
		{
			using fn = studiohdr_t *(__thiscall*)(void*, const model_t*);
			return vmt::get_vfunc<fn>(this, 32)(this, model);
		}
		const char* get_model_name(const model_t *model) 
		{
			typedef const char* (__thiscall* fn)(PVOID, const model_t*);
			return vmt::get_vfunc<fn>(this, 3)(this, model);
		}
	private:
	};
} // namespace ap::sdk