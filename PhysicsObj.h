
#pragma once
#include "Animation.h"
#include "ModelInfo.h"

class CBaseItem;
class CLandBlock;

#define HaltThink() m_pfnThink = NULL;
#define SetThink(x) MakeLive( ); m_pfnThink = static_cast<BOOL (CPhysicsObj::*)(void)>(x);

#define MAX_PLAYER_EQUIPMENT 32
#define MAX_PLAYER_INVENTORY 102
#define MAX_PLAYER_CONTAINERS 7

// Action types
#define ANIM_EMOTE 1
#define ANIM_ATTACK 2
#define ANIM_SPELLCAST 3
#define ANIM_LSRECALL 4
#define ANIM_MPRECALL 5
#define ANIM_DROPITEM 6

// Visual flags
/*
#define VF_JUMPLOCK (VisFlags_t)0x00800000 //no idea, jumping locks you to the north.
#define VF_NOEDGEFALL (VisFlags_t)0x00400000 //falls off edges
#define VF_FROZEN (VisFlags_t)0x00100000
#define VF_DOOR (VisFlags_t)0x00010000
#define VF_BUBBLES (VisFlags_t)0x00004010
#define VF_FREECAMERA (VisFlags_t)0x00001000
#define VF_PLAYER (VisFlags_t)0x00000400
#define VF_ARCS (VisFlags_t)0x00000100 //arcs when airborne (arrows)
#define VF_INVISIBLE (VisFlags_t)0x00000020
#define VF_SIGN (VisFlags_t)0x00000010
#define VF_NORMAL (VisFlags_t)0x00000008
#define VF_HOLLOW (VisFlags_t)0x00000004 //haven't checked
*/

#define USEDISTANCE_ANYWHERE -0.1f

// 0x4000 = bubbly?
// 0x1000 = disconnects camera from player, for making videos, sweet!
// 0x0100 = too cool, gives a sort of tumbling effect
// 0x0020 = invisible/cloaked?
// 0x0004 = can pass through objects?
typedef std::vector<CBaseItem *> ItemVector;

/*
enum eOldWeenieType
{
	TYPE_MELEE_WEAPON = 0x00000001,
	TYPE_ARMOR = 0x00000002,
	eShield = 0x00000002, //Shields too?
	eClothing = 0x00000004,
	eJewelry = 0x00000008,
	eMonster = 0x00000010,
	eMiscellaneous = 0x00000080,
	eCastingTool = 0x00008000
};
*/

enum eHighlightColor
{
	eHighlightNone = 0x00000000,
	eHighlightRed = 0x00000004,
	eHighlightBlue = 0x00000008,
	eHighlightYellow = 0x00000010,
	eHighlightOrange = 0x00000020,
	eHighlightPurple = 0x00000040,
	eHighlightWhite = 0x00000080,
	eHighlightGreen = 0x00000100,
};

enum eObjectStat
{
	eTotalBurden = 0x05,
	eTotalValue = 0x13, //
	eTotalPyreal = 0x14,
	eTotalXP = 0x15,
	eUnassignedXP = 0x16,
	eUnassignedSkillPoints = 0x18,
	eLevel = 0x19,
	eArmorLevel = 0x1C, //
	eRank = 0x1E,
	eMagicResistance = 0x24, //
	eDifficulty = 0x26, //
	eFrozen = 0x28,
	eDeathCount = 0x2B,
	eUnknown2 = 0x2F,
	ePortalLevelMin = 0x56, //
	ePortalLevelMax = 0x57, //
	eBirth = 0x62,
	eAge = 0x7D,
	eExpTillVitaeRegain = 0x81,
	eUnknown4 = 0x8B
};

class CPhysicsObj
{
public:
	CPhysicsObj();
	virtual ~CPhysicsObj();

	//Control handling.
	void Attach(CLandBlock* pBlock);
	void Detach();
	CLandBlock* GetBlock();
	BOOL Think(void);

	//For overriding.
	virtual BOOL DefaultThink(void) { return FALSE; }

	void MakeLive(void);

	//Generic network messages.
	virtual BinaryWriter* CreateMessage(void);
	virtual BinaryWriter* GetModelData(void);
	virtual void UpdateModel(void);

	virtual void Spawn(void);
	virtual void Precache(void) { }
	virtual void Save(void) { }

	// Events
	virtual void MakeAware(CPhysicsObj *) { };
	virtual void Use(CPhysicsObj *) { };
	virtual void UseEx(CPhysicsObj *, CPhysicsObj *) { };
	virtual void Identify(CPhysicsObj *);

	virtual void RemoveMe(void) { m_bRemoveMe = TRUE; }

	// Actions
	virtual void ChangeVIS(DWORD);
	virtual void RemovePreviousInstance();
	virtual void EmitEffect(DWORD dwIndex, float flScale);
	virtual void EmitSound(DWORD dwIndex, float fSpeed);
	virtual void SpeakLocal(const char* szText, long lColor = 2);
	virtual void EmoteLocal(const char* szText);
	virtual void ActionLocal(const char* szText);

	virtual CPhysicsObj* FindChild(DWORD dwGUID) { return NULL; }
	virtual DWORD GetLandcell() { return m_Origin.landcell; }
	virtual void SetLocation(DWORD landcell, float x, float y, float z);
	virtual void SetAngles(float w, float x, float y, float z);

	//should be overridden
	virtual BOOL IsItem() { return FALSE; }
	virtual BOOL IsArmor() { return FALSE; }
	virtual BOOL IsMonster() { return FALSE; }
	virtual BOOL IsPlayer() { return FALSE; }

	virtual BOOL IsCorpse() { return FALSE; }
	virtual BOOL IsDoor() { return FALSE; }
	virtual BOOL IsFoci() { return FALSE; }
	virtual BOOL IsInscribable() { return FALSE; }
	virtual BOOL IsLifestone() { return FALSE; }
	virtual BOOL IsMerchant() { return FALSE; }
	virtual BOOL CanPickup() { return FALSE; }
	virtual BOOL IsPortal() { return FALSE; }
	virtual BOOL IsReadable() { return FALSE; }
	virtual BOOL IsAttackable() { return TRUE; }

	virtual BOOL IsContained() { return FALSE; }
	virtual BOOL IsWielded() { return FALSE; }
	virtual BOOL HasOwner();

	virtual DWORD GetDescFlags();
	virtual DWORD GetContainerID() { return 0; }
	virtual DWORD GetWielderID() { return 0; }
	virtual WORD GetTypeID() { return m_wTypeID; }
	virtual WORD GetIcon() { return m_wIcon; }
	virtual DWORD GetSoundSet() { return m_dwSoundSet; }
	virtual DWORD GetEffectSet() { return m_dwEffectSet; }
	virtual const char* GetName() { return m_strName.c_str(); }
	virtual WORD GetBurden() { return 0; }
	virtual DWORD GetValue() { return 0; }
	virtual float GetApproachDist() { return 0; }
	virtual DWORD GetHighlightColor() { return eHighlightNone; }
	virtual DWORD GetEquipSlot() { return 0; }
	virtual DWORD GetEquipType() { return 0; }
	virtual BYTE GetRadarColor() { return 0; }
	const char* GetDescription();
	void SetDescription(const char*);

	virtual BOOL HasEquipSlot() { return GetEquipSlot() ? TRUE : FALSE; }
	virtual BOOL HasEquipType() { return GetEquipType() ? TRUE : FALSE; }
	virtual BOOL HasCoverage() { return FALSE; }
	virtual BOOL HasBurden() { return FALSE; }
	virtual BOOL HasValue() { return FALSE; }
	virtual BOOL HasRadarDot() { return FALSE; }

	virtual DWORD GetObjectStat(eObjectStat index);
	virtual DWORD SetObjectStat(eObjectStat index, DWORD value);

	BOOL m_bRemoveMe;
	BOOL(CPhysicsObj::*m_pfnThink)(void);

	DWORD m_dwGUID;
	std::string m_strName;
	std::string m_strDescription;

	DWORD m_dwModel;
	float m_fScale;

	WORD m_wTypeID;
	WORD m_wIcon;
	ITEM_TYPE m_ItemType;

	ITEM_USEABLE m_Usability;
	float m_UseDistance;
	RadarEnum m_RadarVis;

	WORD m_wNumMovements;
	WORD m_wNumAnimInteracts;
	WORD m_wNumBubbleModes;
	WORD m_wNumJumps;
	WORD m_wNumPortals;
	WORD m_wAnimCount;
	WORD m_wNumOverride;
	WORD m_wNumModelChanges;
	WORD m_wInstance;
	WORD m_wSeagreen10;

	WORD m_wNumCombat;

	DWORD m_dwEffectSet;
	DWORD m_dwSoundSet;

	loc_t m_Origin;
	heading_t m_Angles;
	DWORD m_PhysicsState;
	double m_fNextThink;

	DWORD m_dwStats[0x100];
	ModelInfo m_miBaseModel;

	BYTE *m_AnimOverrideData;
	DWORD m_AnimOverrideDataLen;
	DWORD m_AutonomousMovement;

#include "container.h"
#include "animate.h"
#include "moves.h"

private:
	CLandBlock* m_pBlock;
};



