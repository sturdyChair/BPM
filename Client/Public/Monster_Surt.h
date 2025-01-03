
#pragma once
//Rampage_Elemental.ao|Ability_Enrage_Start

#pragma once
#include "Monster_Base.h"
#include "Effect_Projectile.h"


BEGIN(Client)
class CParticle;
class CUI_BossHP;
class CTrail;
class CAfterImage;
class CMonster_Surt : public CMonster_Base
{
public:
	enum STATE
	{
		START,
		IDLE,
		MOVE_START,
		MOVE,
		GROUND_SMASH_START,
		GROUND_SMASH,

		SWING_LEFT,
		SWING_RIGHT,

		CHARGE,
		SWING_UPPER,

		CLAP_ROAR,
		ROAR,

		DYING,
		STATE_END
	};

private:
	CMonster_Surt(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Surt(const CMonster_Surt& rhs);
	virtual ~CMonster_Surt() = default;

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
	_bool m_bPush = false;
	_bool m_bFlag = false;

	_bool m_bChargeFlag = false;
	_int m_iSmashTimer = 0;
	_int m_iSmashCount = 0;
	_float4x4 m_MatRune;
	_float4x4 m_MatLeft;//FX_Trail_L_02
	_float4x4 m_MatLeftHand;//FX_Trail_L_02
	_float4x4 m_MatRight;//FX_Trail_R_02
	_float4x4 m_MatRightHand;//FX_Trail_R_02
	_float4   m_vPrevHandLPos{};
	_float4   m_vPrevHandRPos{};
	_int m_iFinish = 0;

	CAfterImage* m_pAfterImage = nullptr;
	_float m_fAfterImageTimer = 0.f;

	queue<STATE> m_ComboQueue;
	CEffect_Projectile::PROJECTILE_DESC m_ProjectileDesc;
	_float m_fMaxHp = 0.f;

	CUI_BossHP* m_pHpBar = nullptr;

	_float m_fRuneTimer = 0.f;


	_float m_fLeftTrailTimer = 0.f;
	_float m_fRightTrailTimer = 0.f;

	CTrail* m_pLeftTrail = nullptr;
	CTrail* m_pRightTrail = nullptr;

public:
	static CMonster_Surt* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END