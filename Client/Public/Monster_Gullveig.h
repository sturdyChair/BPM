#pragma once
#include "Monster_Base.h"
#include "Effect_Projectile.h"


BEGIN(Client)
class CParticle;
class CUI_BossHP;
class CTrail;
class CMonster_Gullveig : public CMonster_Base
{
public:
	enum STATE
	{
		START,//LevelStart
		IDLE,//Idle
		MOVE_FORWARD,//Jog_Fwd
		MOVE_BACKWARD,//Jog_Bwd
		MOVE_LEFT,//Jog_Left
		MOVE_RIGHT,//Jog_Right
		//MOVE_END,
		THROW_PROJ_START,//Ability_E_Targeting_Start
		THROW_PROJ,//Ability_E_MSA
		RISE_START,//Ability_Q_Targeting_Start
		RISE,//Ability_Q
		DAGGER,//Ability_R
		BOW,//Emote_BlowSmoke
		//JUMP_START,//Jump_Start
		//JUMP_END,//Jump_Preland
		DYING,//Knock_Back
		STATE_END
	};


private:
	CMonster_Gullveig(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Gullveig(const CMonster_Gullveig& rhs);
	virtual ~CMonster_Gullveig() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;

	virtual void Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser) override;

private:
	void To_Idle();

	void State_Change();

	void Anim_Finished();

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;
	CCharacterController* m_pCCTCom = nullptr;

	string m_strAnimTag[STATE_END];
	_float m_fAnimSpeed[STATE_END]{};
	_float m_fTimer = 0.f;

	_float m_fAnimFrom = 0.f;
	_float m_fAnimTo = 1.f;

	STATE m_eState = START;
	STATE m_eNextState = START;

	_uint m_iAttackDelay = 6;
	_uint m_iPrevBitCount = 0;
	_uint m_iCurrBitCount = 0;
	_int m_iAttackCount = 0;
	_int m_iMoveCounter = 0;
	_bool m_bPush = false;
	_bool m_bFlag = false;

	_bool m_bChargeFlag = false;
	_int m_iSmashTimer = 0;
	_uint m_iSmashCount = 0;
	_float4x4 m_MatLeft;
	_float4x4 m_MatLeftDown;
	_float4x4 m_MatRight;
	_float4x4 m_MatRightDown;
	_float4   m_vPrevHandLPos{};
	_float4   m_vPrevHandRPos{};
	_int m_iFinish = 0;
	_float4 m_vStartPos;

	_float m_fLightWallTimer = 0.f;

	CEffect_Projectile::PROJECTILE_DESC m_ProjectileDesc;
	_float m_fMaxHp = 0.f;

	CUI_BossHP* m_pHpBar = nullptr;

	_float m_fLeftTrailTimer = 0.f;
	_float m_fRightTrailTimer = 0.f;



	//
	//CTrail* m_pLeftTrail = nullptr;
	//CTrail* m_pRightTrail = nullptr;

public:
	static CMonster_Gullveig* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END