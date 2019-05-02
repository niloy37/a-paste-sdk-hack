#pragma once


#define IN_ATTACK					(1 << 0)
#define IN_JUMP						(1 << 1)
#define IN_DUCK						(1 << 2)
#define IN_FORWARD					(1 << 3)
#define IN_BACK						(1 << 4)
#define IN_USE						(1 << 5)
#define IN_CANCEL					(1 << 6)
#define IN_LEFT						(1 << 7)
#define IN_RIGHT					(1 << 8)
#define IN_MOVELEFT					(1 << 9)
#define IN_MOVERIGHT				(1 << 10)
#define IN_ATTACK2					(1 << 11)
#define IN_RUN						(1 << 12)
#define IN_RELOAD					(1 << 13)
#define IN_ALT1						(1 << 14)
#define IN_ALT2						(1 << 15)
#define IN_SCORE					(1 << 16)   
#define IN_SPEED					(1 << 17)	
#define IN_WALK						(1 << 18)	
#define IN_ZOOM						(1 << 19)	
#define IN_WEAPON1					(1 << 20)	
#define IN_WEAPON2					(1 << 21)
#define IN_BULLRUSH					(1 << 22)
#define IN_GRENADE1					(1 << 23)
#define IN_GRENADE2					(1 << 24)	
#define	IN_ATTACK3					(1 << 25)

#define	FL_ONGROUND					(1 << 0)	
#define FL_DUCKING					(1 << 1)	
#define	FL_WATERJUMP				(1 << 2)	
#define FL_ONTRAIN					(1 << 3)
#define FL_INRAIN					(1 << 4)	
#define FL_FROZEN					(1 << 5)
#define FL_ATCONTROLS				(1 << 6)
#define	FL_CLIENT					(1 << 7)	
#define FL_FAKECLIENT				(1 << 8)	
#define	FL_INWATER					(1 << 9)										
#define	FL_FLY						(1 << 10)	
#define	FL_SWIM						(1 << 11)
#define	FL_CONVEYOR					(1 << 12)
#define	FL_NPC						(1 << 13)
#define	FL_GODMODE					(1 << 14)
#define	FL_NOTARGET					(1 << 15)
#define	FL_AIMTARGET				(1 << 16)	
#define	FL_PARTIALGROUND			(1 << 17)	
#define FL_STATICPROP				(1 << 18)
#define FL_GRAPHED					(1 << 19) 
#define FL_GRENADE					(1 << 20)
#define FL_STEPMOVEMENT				(1 << 21)	
#define FL_DONTTOUCH				(1 << 22)
#define FL_BASEVELOCITY				(1 << 23)
#define FL_WORLDBRUSH				(1 << 24)	
#define FL_OBJECT					(1 << 25) 
#define FL_KILLME					(1 << 26)
#define FL_ONFIRE					(1 << 27)	
#define FL_DISSOLVING				(1 << 28)
#define FL_TRANSRAGDOLL				(1 << 29)
#define FL_UNBLOCKABLE_BY_PLAYER	(1 << 30) 

#define TEXTURE_GROUP_LIGHTMAP						"Lightmaps"
#define TEXTURE_GROUP_WORLD							"World textures"
#define TEXTURE_GROUP_MODEL							"Model textures"
#define TEXTURE_GROUP_VGUI							"VGUI textures"
#define TEXTURE_GROUP_PARTICLE						"Particle textures"
#define TEXTURE_GROUP_DECAL							"Decal textures"
#define TEXTURE_GROUP_SKYBOX						"SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				"ClientEffect textures"
#define TEXTURE_GROUP_OTHER							"Other textures"
#define TEXTURE_GROUP_PRECACHED						"Precached"				// TODO: assign texture groups to the precached materials
#define TEXTURE_GROUP_CUBE_MAP						"CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					"RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED					"Unaccounted textures"	// Textures that weren't assigned a texture group.
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			"Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			"Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			"Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					"DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					"ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					"Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				"Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			"RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					"Morph Targets"

#define PLAYER_FLAG_BITS			10
#define DISPSURF_FLAG_SURFACE		(1<<0)
#define DISPSURF_FLAG_WALKABLE		(1<<1)
#define DISPSURF_FLAG_BUILDABLE		(1<<2)
#define DISPSURF_FLAG_SURFPROP1		(1<<3)
#define DISPSURF_FLAG_SURFPROP2		(1<<4)

#define HITGROUP_GENERIC	  0
#define HITGROUP_HEAD         1
#define HITGROUP_CHEST        2
#define HITGROUP_STOMACH      3
#define HITGROUP_LEFTARM      4    
#define HITGROUP_RIGHTARM     5
#define HITGROUP_LEFTLEG      6
#define HITGROUP_RIGHTLEG     7
#define HITGROUP_GEAR         10

#define WEAPON_TYPE_PISTOL 2
#define WEAPON_TYPE_RIFLE 4
#define WEAPON_TYPE_GRENADE 0
#define WEAPON_TYPE_KNIFE 1

#define MAX_STUDIOBONES 128
#define MAXSTUDIOSKINS 32

#define BONE_USED_BY_HITBOX	0x00000100
#define BONE_USED_BY_ANYTHING		0x0007FF00

#define	SURF_LIGHT		0x0001		// value will hold the light strength
#define	SURF_SKY2D		0x0002		// don't draw, indicates we should skylight + draw 2d sky but not draw the 3D skybox
#define	SURF_SKY		0x0004		// don't draw, but add to skybox
#define	SURF_WARP		0x0008		// turbulent water warp
#define	SURF_TRANS		0x0010
#define SURF_NOPORTAL	0x0020	// the surface can not have a portal placed on it
#define	SURF_TRIGGER	0x0040	// FIXME: This is an xbox hack to work around elimination of trigger surfaces, which breaks occluders
#define	SURF_NODRAW		0x0080	// don't bother referencing the texture
#define	SURF_HINT		0x0100	// make a primary bsp splitter
#define	SURF_SKIP		0x0200	// completely ignore, allowing non-closed brushes
#define SURF_NOLIGHT	0x0400	// Don't calculate light
#define SURF_BUMPLIGHT	0x0800	// calculate three lightmaps for the surface for bumpmapping
#define SURF_NOSHADOWS	0x1000	// Don't receive shadows
#define SURF_NODECALS	0x2000	// Don't receive decals
#define SURF_NOPAINT	SURF_NODECALS	// the surface can not have paint placed on it
#define SURF_NOCHOP		0x4000	// Don't subdivide patches on this surface 
#define SURF_HITBOX		0x8000	// surface is part of a hitbox

#define CHAR_TEX_ANTLION		'A'
#define CHAR_TEX_BLOODYFLESH	'B'
#define	CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_EGGSHELL		'E' ///< the egg sacs in the tunnels in ep2.
#define CHAR_TEX_FLESH			'F'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_ALIENFLESH		'H'
#define CHAR_TEX_CLIP			'I'
//#define CHAR_TEX_UNUSED		'J'
#define CHAR_TEX_SNOW			'K'
#define CHAR_TEX_PLASTIC		'L'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_COMPUTER		'P'
//#define CHAR_TEX_UNUSED		'Q'
#define CHAR_TEX_REFLECTIVE		'R'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_TILE			'T'
#define CHAR_TEX_CARDBOARD		'U'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
//#define CHAR_TEX_UNUSED		'X' ///< do not use - "fake" materials use this (ladders, wading, clips, etc)
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_WARPSHIELD		'Z' ///< wierd-looking jello effect for advisor shield.

#define CHAR_TEX_STEAM_PIPE		11

// hhhhh macros
#define TIME_TO_TICKS(dt) (int(0.5f + float(dt) / ap::interfaces::globals->interval_per_tick))
#define TICKS_TO_TIME(t) (ap::interfaces::globals->interval_per_tick * (t))
#define TICK_INTERVAL (ap::interfaces::globals->interval_per_tick)
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

enum MoveType
{
	MOVETYPE_NONE = 0,            /**< never moves */
	MOVETYPE_ISOMETRIC,            /**< For players */
	MOVETYPE_WALK,                /**< Player only - moving on the ground */
	MOVETYPE_STEP,                /**< gravity, special edge handling -- monsters use this */
	MOVETYPE_FLY,                /**< No gravity, but still collides with stuff */
	MOVETYPE_FLYGRAVITY,        /**< flies through the air + is affected by gravity */
	MOVETYPE_VPHYSICS,            /**< uses VPHYSICS for simulation */
	MOVETYPE_PUSH,                /**< no clip to world, push and crush */
	MOVETYPE_NOCLIP,            /**< No gravity, no collisions, still do velocity/avelocity */
	MOVETYPE_LADDER,            /**< Used by players only when going onto a ladder */
	MOVETYPE_OBSERVER,            /**< Observer movement, depends on player's observer mode */
	MOVETYPE_CUSTOM,            /**< Allows the entity to describe its own physics */
};

enum CSWeaponID_t 
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SCAR20 = 38,
	WEAPON_SG553,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER
};

enum ClassID_t {
	CTestTraceline = 223,
	CTEWorldDecal = 224,
	CTESpriteSpray = 221,
	CTESprite = 220,
	CTESparks = 219,
	CTESmoke = 218,
	CTEShowLine = 216,
	CTEProjectedDecal = 213,
	CFEPlayerDecal = 71,
	CTEPlayerDecal = 212,
	CTEPhysicsProp = 209,
	CTEParticleSystem = 208,
	CTEMuzzleFlash = 207,
	CTELargeFunnel = 205,
	CTEKillPlayerAttachments = 204,
	CTEImpact = 203,
	CTEGlowSprite = 202,
	CTEShatterSurface = 215,
	CTEFootprintDecal = 199,
	CTEFizz = 198,
	CTEExplosion = 196,
	CTEEnergySplash = 195,
	CTEEffectDispatch = 194,
	CTEDynamicLight = 193,
	CTEDecal = 191,
	CTEClientProjectile = 190,
	CTEBubbleTrail = 189,
	CTEBubbles = 188,
	CTEBSPDecal = 187,
	CTEBreakModel = 186,
	CTEBloodStream = 185,
	CTEBloodSprite = 184,
	CTEBeamSpline = 183,
	CTEBeamRingPoint = 182,
	CTEBeamRing = 181,
	CTEBeamPoints = 180,
	CTEBeamLaser = 179,
	CTEBeamFollow = 178,
	CTEBeamEnts = 177,
	CTEBeamEntPoint = 176,
	CTEBaseBeam = 175,
	CTEArmorRicochet = 174,
	CTEMetalSparks = 206,
	CSteamJet = 167,
	CSmokeStack = 157,
	DustTrail = 275,
	CFireTrail = 74,
	SporeTrail = 281,
	SporeExplosion = 280,
	RocketTrail = 278,
	SmokeTrail = 279,
	CPropVehicleDriveable = 144,
	ParticleSmokeGrenade = 277,
	CParticleFire = 116,
	MovieExplosion = 276,
	CTEGaussExplosion = 201,
	CEnvQuadraticBeam = 66,
	CEmbers = 55,
	CEnvWind = 70,
	CPrecipitation = 137,
	CPrecipitationBlocker = 138,
	CBaseTempEntity = 18,
	NextBotCombatCharacter = 0,
	CEconWearable = 54,
	CBaseAttributableItem = 4,
	CEconEntity = 53,
	CWeaponXM1014 = 272,
	CWeaponTaser = 267,
	CTablet = 171,
	CSnowball = 158,
	CSmokeGrenade = 155,
	CWeaponShield = 265,
	CWeaponSG552 = 263,
	CSensorGrenade = 151,
	CWeaponSawedoff = 259,
	CWeaponNOVA = 255,
	CIncendiaryGrenade = 99,
	CMolotovGrenade = 112,
	CMelee = 111,
	CWeaponM3 = 247,
	CKnifeGG = 108,
	CKnife = 107,
	CHEGrenade = 96,
	CFlashbang = 77,
	CFists = 76,
	CWeaponElite = 238,
	CDecoyGrenade = 47,
	CDEagle = 46,
	CWeaponUSP = 271,
	CWeaponM249 = 246,
	CWeaponUMP45 = 270,
	CWeaponTMP = 269,
	CWeaponTec9 = 268,
	CWeaponSSG08 = 266,
	CWeaponSG556 = 264,
	CWeaponSG550 = 262,
	CWeaponScout = 261,
	CWeaponSCAR20 = 260,
	CSCAR17 = 149,
	CWeaponP90 = 258,
	CWeaponP250 = 257,
	CWeaponP228 = 256,
	CWeaponNegev = 254,
	CWeaponMP9 = 253,
	CWeaponMP7 = 252,
	CWeaponMP5Navy = 251,
	CWeaponMag7 = 250,
	CWeaponMAC10 = 249,
	CWeaponM4A1 = 248,
	CWeaponHKP2000 = 245,
	CWeaponGlock = 244,
	CWeaponGalilAR = 243,
	CWeaponGalil = 242,
	CWeaponG3SG1 = 241,
	CWeaponFiveSeven = 240,
	CWeaponFamas = 239,
	CWeaponBizon = 234,
	CWeaponAWP = 232,
	CWeaponAug = 231,
	CAK47 = 1,
	CWeaponCSBaseGun = 236,
	CWeaponCSBase = 235,
	CC4 = 34,
	CBumpMine = 32,
	CBumpMineProjectile = 33,
	CBreachCharge = 28,
	CBreachChargeProjectile = 29,
	CWeaponBaseItem = 233,
	CBaseCSGrenade = 8,
	CSnowballProjectile = 160,
	CSnowballPile = 159,
	CSmokeGrenadeProjectile = 156,
	CSensorGrenadeProjectile = 152,
	CMolotovProjectile = 113,
	CItem_Healthshot = 104,
	CItemDogtags = 106,
	CDecoyProjectile = 48,
	CPhysPropRadarJammer = 126,
	CPhysPropWeaponUpgrade = 127,
	CPhysPropAmmoBox = 124,
	CPhysPropLootCrate = 125,
	CItemCash = 105,
	CEnvGasCanister = 63,
	CDronegun = 50,
	CParadropChopper = 115,
	CSurvivalSpawnChopper = 170,
	CBRC4Target = 27,
	CInfoMapRegion = 102,
	CFireCrackerBlast = 72,
	CInferno = 100,
	CChicken = 36,
	CDrone = 49,
	CFootstepControl = 79,
	CCSGameRulesProxy = 39,
	CWeaponCubemap = 0,
	CWeaponCycler = 237,
	CTEPlantBomb = 210,
	CTEFireBullets = 197,
	CTERadioIcon = 214,
	CPlantedC4 = 128,
	CCSTeam = 43,
	CCSPlayerResource = 41,
	CCSPlayer = 40,
	CPlayerPing = 130,
	CCSRagdoll = 42,
	CTEPlayerAnimEvent = 211,
	CHostage = 97,
	CHostageCarriableProp = 98,
	CBaseCSGrenadeProjectile = 9,
	CHandleTest = 95,
	CTeamplayRoundBasedRulesProxy = 173,
	CSpriteTrail = 165,
	CSpriteOriented = 164,
	CSprite = 163,
	CRagdollPropAttached = 147,
	CRagdollProp = 146,
	CPropCounter = 141,
	CPredictedViewModel = 139,
	CPoseController = 135,
	CGrassBurn = 94,
	CGameRulesProxy = 93,
	CInfoLadderDismount = 101,
	CFuncLadder = 85,
	CTEFoundryHelpers = 200,
	CEnvDetailController = 61,
	CDangerZone = 44,
	CDangerZoneController = 45,
	CWorldVguiText = 274,
	CWorld = 273,
	CWaterLODControl = 230,
	CWaterBullet = 229,
	CVoteController = 228,
	CVGuiScreen = 227,
	CPropJeep = 143,
	CPropVehicleChoreoGeneric = 0,
	CTriggerSoundOperator = 226,
	CBaseVPhysicsTrigger = 22,
	CTriggerPlayerMovement = 225,
	CBaseTrigger = 20,
	CTest_ProxyToggle_Networkable = 222,
	CTesla = 217,
	CBaseTeamObjectiveResource = 17,
	CTeam = 172,
	CSunlightShadowControl = 169,
	CSun = 168,
	CParticlePerformanceMonitor = 117,
	CSpotlightEnd = 162,
	CSpatialEntity = 161,
	CSlideshowDisplay = 154,
	CShadowControl = 153,
	CSceneEntity = 150,
	CRopeKeyframe = 148,
	CRagdollManager = 145,
	CPhysicsPropMultiplayer = 122,
	CPhysBoxMultiplayer = 120,
	CPropDoorRotating = 142,
	CBasePropDoor = 16,
	CDynamicProp = 52,
	CProp_Hallucination = 140,
	CPostProcessController = 136,
	CPointWorldText = 134,
	CPointCommentaryNode = 133,
	CPointCamera = 132,
	CPlayerResource = 131,
	CPlasma = 129,
	CPhysMagnet = 123,
	CPhysicsProp = 121,
	CStatueProp = 166,
	CPhysBox = 119,
	CParticleSystem = 118,
	CMovieDisplay = 114,
	CMaterialModifyControl = 110,
	CLightGlow = 109,
	CItemAssaultSuitUseable = 0,
	CItem = 0,
	CInfoOverlayAccessor = 103,
	CFuncTrackTrain = 92,
	CFuncSmokeVolume = 91,
	CFuncRotating = 90,
	CFuncReflectiveGlass = 89,
	CFuncOccluder = 88,
	CFuncMoveLinear = 87,
	CFuncMonitor = 86,
	CFunc_LOD = 81,
	CTEDust = 192,
	CFunc_Dust = 80,
	CFuncConveyor = 84,
	CFuncBrush = 83,
	CBreakableSurface = 31,
	CFuncAreaPortalWindow = 82,
	CFish = 75,
	CFireSmoke = 73,
	CEnvTonemapController = 69,
	CEnvScreenEffect = 67,
	CEnvScreenOverlay = 68,
	CEnvProjectedTexture = 65,
	CEnvParticleScript = 64,
	CFogController = 78,
	CEnvDOFController = 62,
	CCascadeLight = 35,
	CEnvAmbientLight = 60,
	CEntityParticleTrail = 59,
	CEntityFreezing = 58,
	CEntityFlame = 57,
	CEntityDissolve = 56,
	CDynamicLight = 51,
	CColorCorrectionVolume = 38,
	CColorCorrection = 37,
	CBreakableProp = 30,
	CBeamSpotlight = 25,
	CBaseButton = 5,
	CBaseToggle = 19,
	CBasePlayer = 15,
	CBaseFlex = 12,
	CBaseEntity = 11,
	CBaseDoor = 10,
	CBaseCombatCharacter = 6,
	CBaseAnimatingOverlay = 3,
	CBoneFollower = 26,
	CBaseAnimating = 2,
	CAI_BaseNPC = 0,
	CBeam = 24,
	CBaseViewModel = 21,
	CBaseParticleEntity = 14,
	CBaseGrenade = 13,
	CBaseCombatWeapon = 7,
	CBaseWeaponWorldModel = 23,
};

enum Hitboxes
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_BELLY,
	HITBOX_THORAX,
	HITBOX_LOWER_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_MAX
};