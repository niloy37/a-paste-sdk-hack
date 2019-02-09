#pragma once
#include "../misc/vector.h"

typedef unsigned short ClientShadowHandle_t;
typedef unsigned short ClientRenderHandle_t;
typedef unsigned short ModelInstanceHandle_t;
typedef unsigned char uint8_t;

class matrix3x4_t;
class c_client_unknown;
struct model_t;
class vec3f;
namespace ap::sdk {
	class c_client_renderable
	{
	public:
		virtual c_client_unknown*            GetIClientUnknown() = 0;
		virtual vec3f const&               GetRenderOrigin(void) = 0;
		virtual vec3f const&               GetRenderAngles(void) = 0;
		virtual bool                       ShouldDraw(void) = 0;
		virtual int                        GetRenderFlags(void) = 0; // ERENDERFLAGS_xxx
		virtual void                       Unused(void) const {}
		virtual ClientShadowHandle_t       GetShadowHandle() const = 0;
		virtual ClientRenderHandle_t&      RenderHandle() = 0;
		virtual const model_t*             GetModel() const = 0;
		virtual int                        DrawModel(int flags, const int /*RenderableInstance_t*/ &instance) = 0;
		virtual int                        GetBody() = 0;
		virtual void                       GetColorModulation(float* color) = 0;
		virtual bool                       LODTest() = 0;
		virtual bool                       SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;
		virtual void                       SetupWeights(const matrix3x4_t *pBoneToWorld, int nFlexWeightCount, float *pFlexWeights, float *pFlexDelayedWeights) = 0;
		virtual void                       DoAnimationEvents(void) = 0;
		virtual void* /*IPVSNotify*/       GetPVSNotifyInterface() = 0;
		virtual void                       GetRenderBounds(vec3f& mins, vec3f& maxs) = 0;
		virtual void                       GetRenderBoundsWorldspace(vec3f& mins, vec3f& maxs) = 0;
		virtual void                       GetShadowRenderBounds(vec3f &mins, vec3f &maxs, int /*ShadowType_t*/ shadowType) = 0;
		virtual bool                       ShouldReceiveProjectedTextures(int flags) = 0;
		virtual bool                       GetShadowCastDistance(float *pDist, int /*ShadowType_t*/ shadowType) const = 0;
		virtual bool                       GetShadowCastDirection(vec3f *pDirection, int /*ShadowType_t*/ shadowType) const = 0;
		virtual bool                       IsShadowDirty() = 0;
		virtual void                       MarkShadowDirty(bool bDirty) = 0;
		virtual c_client_renderable*         GetShadowParent() = 0;
		virtual c_client_renderable*         FirstShadowChild() = 0;
		virtual c_client_renderable*         NextShadowPeer() = 0;
		virtual int /*ShadowType_t*/       ShadowCastType() = 0;
		virtual void                       CreateModelInstance() = 0;
		virtual ModelInstanceHandle_t      GetModelInstance() = 0;
		virtual const matrix3x4_t&         RenderableToWorldTransform() = 0;
		virtual int                        LookupAttachment(const char *pAttachmentName) = 0;
		virtual   bool                     GetAttachment(int number, vec3f &origin, vec3f &angles) = 0;
		virtual bool                       GetAttachment(int number, matrix3x4_t &matrix) = 0;
		virtual float*                     GetRenderClipPlane(void) = 0;
		virtual int                        GetSkin() = 0;
		virtual void                       OnThreadedDrawSetup() = 0;
		virtual bool                       UsesFlexDelayedWeights() = 0;
		virtual void                       RecordToolMessage() = 0;
		virtual bool                       ShouldDrawForSplitScreenUser(int nSlot) = 0;
		virtual uint8_t                      OverrideAlphaModulation(uint8_t nAlpha) = 0;
		virtual uint8_t                      OverrideShadowAlphaModulation(uint8_t nAlpha) = 0;
	};
}
