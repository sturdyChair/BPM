

#pragma once

#pragma once
#include "Monster_Base.h"
#include "Effect_Projectile.h"


BEGIN(Client)
class CParticle;
class CUI_BossHP;
class CEffect_GideonRune;
class CAfterImage;
class CMonster_Gideon : public CMonster_Base
{
public:
	enum STATE
	{
		START,
		IDLE,
		MOVE,
		MOVE_BWD,
		MOVE_RIGHT,
		DASH_RIGHT,
		MOVE_LEFT,
		DASH_LEFT,

		MOVE_END,

		PHASE_2_START,

		STRAIGHT,
		STRAIGHT_LOOP,
		STRAIGHT_THROW,

		TELEPORT,

		UPPER_START,
		UPPER,

		ZAP_1,
		ZAP_2,
		ZAP_3,

		KIYATHOU,
		KIYATHOU_LOOP,
		KIYATHOU_END,

		RIFT_START,
		RIFT,

		LAST_DITCH_START,
		LAST_DITCH,


		DYING,
		STATE_END
	};

private:
	CMonster_Gideon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Gideon(const CMonster_Gideon& rhs);
	virtual ~CMonster_Gideon() = default;

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
	void Moving(_float fTimeDelta);

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
	_float4x4 m_MatLeftHand;
	_float4x4 m_MatRightHand;
	_float4x4 m_MatLeft;//FX_Trail_L_02
	_float4x4 m_MatRight;//FX_Trail_R_02

	_float4   m_vPrevHandLPos{};
	_float4   m_vPrevHandRPos{};
	_int m_iFinish = 0;

	_int m_iCounter = 0;
	_int m_iMoveCounter = 0;

	CEffect_Projectile::PROJECTILE_DESC m_ProjectileDesc;
	_float m_fMaxHp = 0.f;

	CUI_BossHP* m_pHpBar = nullptr;

	_float m_fRuneTimer = 0.f;
	_bool m_bLeft = false;

	_bool m_bPhase2 = false;
	_bool m_bDitch = false;
	_bool m_bDitching = false;
	_wstring m_strLightTag = L"Gideon_Light_1";
	_wstring m_strLightTag2 = L"Gideon_Light_2";
	
	queue<STATE> m_ComboQue;

	_float3 m_vBlackHolePos{};
	_bool m_bRune = false;
	CEffect_GideonRune* m_pRuneL = nullptr;
	CEffect_GideonRune* m_pRuneR = nullptr;
	CAfterImage*	m_pAfterImage = nullptr;
	_float m_fAfterImageTimer = 0.f;
public:
	static CMonster_Gideon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END