
#pragma once
#include "Monster_Base.h"
#include "Effect_Projectile.h"


BEGIN(Client)
class CParticle;
class CUI_BossHP;

class CMonster_Draugr : public CMonster_Base
{
public:
	enum STATE
	{
		START,
		IDLE,
		CLAP,
		MOVE,
		DANCE,
		//SUMMON,
		RAPID,
		SWING,
		DYING,

		STATE_END
	};


private:
	CMonster_Draugr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Draugr(const CMonster_Draugr& rhs);
	virtual ~CMonster_Draugr() = default;

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

	void Dance_Attack(_float fTimeDelta);
	void Clap(_float fTimeDelta);
	void Rapid(_float fTimeDelta);
	void Swing(_float fTimeDelta);

	void Projectile();

	void State_Change();

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;
	CCharacterController* m_pCCTCom = nullptr;

	string m_strAnimTag[STATE_END];
	_float m_fTimer = 0.f;

	_float m_fAnimFrom = 0.f;
	_float m_fAnimTo = 1.f;

	STATE m_eState = START;

	_uint m_iAttackDelay = 6;
	_uint m_iPrevBitCount = 0;
	_int m_iAttackCount = 0;
	_bool m_bPush = false;

	_bool m_bChargeFlag = false;

	_float4x4 m_MatLeft;
	_float4x4 m_MatRight;
	_int m_iFinish = 0;

	CParticle* m_pParticle = nullptr;

	CEffect_Projectile::PROJECTILE_DESC m_ProjectileDesc;
	_float m_fMaxHp = 0.f;

	CUI_BossHP* m_pHpBar = nullptr;

public:
	static CMonster_Draugr* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END