#pragma once

#include "Client_Defines.h"
#include "Camera.h"
BEGIN(Engine)
class CCharacterController;
class CShader;
class CModel;
END
BEGIN(Client)
class CWeapon;
class CItem_Equip;
class CEffect_Hex_Shield;
class CEffect_ThreeRune;
class CParticle;
class CAfterImage;
class CPlayer : public CGameObject
{
public:
	struct DAMAGE
	{
		_float fAmount = 0.f;
		_float fDuration = 0.f;
		_float3 vHitPoint;
		DAMAGE_TYPE eType;
		CGameObject* pCauser = nullptr;
	};

protected:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void PriorityTick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	void Set_Position(_fmatrix Transform);
	void Set_FootPosition(_fmatrix Transform);
	_float3 Get_FootPosition();

	void Equip_Weapon(CWeapon* pWeapon);

	void Equip_Item(CItem_Equip* pEquip);

	void Heal(_float fAmount);
	void Increase_Max_Hp(_float fAmount);

	void Add_Shield(_float fAmount);

	void Add_Coin(_int iAmount);
	void Add_Key(_int iAmount);

	_float Get_Shield() const { return m_fShield; }

	virtual void Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit) override;

	virtual void Take_Damage(_float fAmount, const _float3& vHitPoint,_uint iDmgType, CGameObject* pCauser) override;

	_uint Get_PickedItem()const { return m_iPickedItem; }
	_uint Get_PickedMonster() const { return m_iPickedMonster; }

	CCharacterController* Get_CCT() { return m_pCCTCom; }

	_uint Get_Coin() const { return m_iCoin; }
	void  Reduce_Coin(_uint iAmount) { m_iCoin -= iAmount; }

	_uint Get_Key() const { return m_iKey;}
	void  Reduce_Key() { --m_iKey; }

	virtual CCamera* Get_Camera() { return (CCamera*)m_pCamera; }

	virtual void Cam_Shake(_float fDuration, _float fPower);
	virtual void Cam_Dissolve(_float fDuration, _float fRange);
	void Enter_BossRoom(CGameObject* pTarget);

	void Add_PushVector(const _float3& vVector, _float fDuration);
	void Clear_PushVector();

	void WhiteOut();
	void NoMiss(_bool bToggle = true) { m_bNoMissNextFrame = bToggle; }
	void Set_SpotLight_Color(const _float3& vColor);
	void Set_SpotLight_Color(_fvector vColor);

	void Jump_Able();
	void PickUp_SkeletonKey(class CSkeletonKey* pKey);
	_uint Get_SkelKeyNum() const { return m_iSkelKey; }

protected:
	void Dash(_float fTimeDelta);
	void Stomp(_float fTimeDelta);
	void Shoot(_float fTimeDelta);
	void Reload(_float fTimeDelta);
	void Parry(_float fTimeDelta);
	void MegaShoot(_float fTimeDelta);
	void HealOverTime(_float fTimeDelta);
	void Buff(_float fTimeDelta);
	void Clap(_float fTimeDelta);
	void ThunderClap(_float fTimeDelta);
	//6/13
	void SummonAlly(_float fTimeDelta);
	void FireWall();

	void Beat_Move();
	void Bad_Beat();

	void Do_A_Barrel_Roll(_float fTimeDelta);

	void Pick();

	void Take_Control(_bool bFlag = true) { m_bInControl = bFlag; }

protected:
	class CSkeletonKey* m_pSkeletonKey[4]{nullptr, nullptr, nullptr, nullptr};
	_uint m_iSkelKey = 0;


	_float				m_fMouseSensor = {};
	CCharacterController* m_pCCTCom = nullptr;

	class CCamera_FPS* m_pCamera = nullptr;
	class CUI_Controller* m_pUIController = nullptr;
	class CBeat_Manager* m_pBeatManager = nullptr;

	_float m_fGravity = -40.f;
	ABILITY_TEXTURE_ID m_eUlt = ABILITY_U_GRANADE;
	ABILITY_TEXTURE_ID m_eSec = ABILITY_S_BLOCK;

	_float m_fSpeed = 15.f;
	_int m_iJumpCount = 2;
	bool m_bDash = false;
	_int m_iDashCoolDown = 0;
	_float m_fDashTimer = 0.f;
	_int   m_iCurBeatCount = 0;
	_int   m_iPrevBeatCount = 0;
	bool   m_bBeatMoved = false;
	bool   m_bBeatMissed = false;
	_int   m_iMovedBeat = 0;
	_int   m_iMoveLeft = 1;

	_int   m_iPickedMonster = -1;
	_int   m_iPickedItem = -1;

	_float3 m_vDashVelocity{};
	
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;
	CTransform* m_pCamTransformCom = nullptr;
	_float4x4 m_matCamOffset;
	
	_float4x4 m_ModelTransform;
	_float4x4 m_ModelTransformSecond;
	_float m_fAnimFrom = 0.f;
	_float m_fAnimTo = 1.f;
	_float4x4 m_TransformMat;
	_float3 m_vTranslation;
	_float m_fYTrans = 0.f;

	CWeapon* m_pWeapon = nullptr;
	bool m_bReloading = false;

	CItem_Equip* m_ArrEquip[SLOT_END]{};

	_float m_fGatlingTimer = 0.f;
	_int   m_iGatlingStreak = 0;

	_bool m_bFrameEquipped = false;

	_float m_fHp = 100.f;
	_float m_fMaxHp = 100.f;
	_float m_fShield = 0.f;
	_uint  m_iCoin = 0;
	_uint  m_iKey = 0;

	_float m_fFov = 0.f;
	_float m_fFovOri = 0.f;

	_float m_fInvincible = 0.f;
	_bool  m_ArrItem[ITEM_END]{};

	list<DAMAGE> m_Damages;

	_float m_fRollAngle = 0.f;
	_float m_fRollTime = 0.00f;
	_float3 m_vRollAxis;
	_float4x4 m_WorldBeforeRoll;

	_float m_fDamageModifier = 1.f;

	CEffect_Hex_Shield* m_pHex_Shield = nullptr;
	CEffect_ThreeRune* m_pThreeRune = nullptr;

	_float4x4 m_MatHandBone{};

	_float m_fParring = 0.f;
	_bool m_bBossIntro = false;
	_bool m_bInitIntro = false;
	_float m_fBossIntroTimer = 0.f;

	_uint m_iMegaShotCounter = 0;
	_int m_iThunderClapCounter = 0;


	_float m_fStompTimer = 0.f;
	_bool m_bCleared = false;
	_float m_fClearTimer = 0.f;
	static const string strWeapon_r;
	static const string strWeapon_l;
	static const string strMegaShot;
	static const string strThunderClap;

	static const string strHealOverTime;
	static const string strBuff;
	static const string strFireWall;
	static const string strSummonAlly;

	_float m_fHealTimer = 0.f;
	CParticle* m_pParticleHeal = nullptr;//Prototype_GameObject_Effect_Particle_Heal_Rising
	
	_float m_fBuffTimer = 0.f;
	CParticle* m_pParticleBuff = nullptr;//Prototype_GameObject_Effect_Particle_Buff_Rising

	//CAfterImage* m_pAfterImage = nullptr;
	_float m_fAfterImageTimer = 0.f;

	_bool m_bNoMiss = false;
	_bool m_bNoMissNextFrame = false;
	_bool m_bDebugNoMiss = false;
	vector<pair<_float3, _float>> m_vecPushVector;

	_bool m_bInControl = true;
	_wstring m_strSpotLight = L"PlayerSpotLight";
	//0.1 ~ 0.5
public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END