#pragma once
#include "Monster_Base.h"
#include "Effect_Projectile.h"


BEGIN(Client)
class CUI_BossHP;
class CStatue_Gimmick;
class CEffect_NidhoggField;

class CMonster_Nidhog : public CMonster_Base
{
public:
	enum STATE
	{
		FLY_STATIONARTY,//WYVERN.ao|WYVERN_flyStationary
		FLY_MOVE,		//WYVERN.ao|WYVERN_fly
		FLY_SPITFIRE,	//WYVERN.ao|WYVERN_flyStationarySpitFireball
		FLY_ROAR,		//WYVERN.ao|WYVERN_flyStationaryRoar
		FLY_TO_LAND,	//WYVERN.ao|WYVERN_flyStationaryToLanding
		FLY_TO_DIE,		//WYVERN.ao|WYVERN_hitTheGroundDeath
		IDLE,			//WYVERN.ao|WYVERN_idle
		SPREAD_FIRE,	//WYVERN.ao|WYVERN_spreadFire
		STING_TAIL,		//WYVERN.ao|WYVERN_stingerAttack
		WALK,			//WYVERN.ao|WYVERN_walk
		DEATH,			//WYVERN.ao|WYVERN_death
		STATE_END
	};


private:
	CMonster_Nidhog(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Nidhog(const CMonster_Nidhog& rhs);
	virtual ~CMonster_Nidhog() = default;

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

	void Kill_Statue();

private:
	void To_Idle();
	void Select_Movement();

	void Spit_Space();
	void Roar();
	void Move();

	void Land();

	void Spread_Fire();
	void Sting_Tail();

	void State_Change();

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;
	CCharacterController* m_pCCTCom = nullptr;

	string m_strAnimTag[STATE_END];
	_float m_fTimer = 0.f;

	_float m_fAnimFrom = 0.f;
	_float m_fAnimTo = 1.f;

	STATE m_eState = FLY_STATIONARTY;

	_bool m_bFlying = true;

	_int m_iMoveCount = 4;

	_uint m_iAttackDelay = 6;
	_uint m_iPrevBitCount = 0;
	_uint m_iCurrBitCount = 0;
	_int m_iAttackCount = 0;
	_bool m_bPush = false;

	_bool m_bChargeFlag = false;

	_float4x4 m_MatLeft;
	_float4x4 m_MatRight;
	_int m_iFinish = 0;
	_float m_fSpeed = 20.f;
	_float m_fAngleSpeed = XM_PI;

	CEffect_Projectile::PROJECTILE_DESC m_ProjectileDesc;

	string m_strMouth = "WYVERN_-Head";

	string m_strTail = "WYVERN_-Tail";

	_bool m_bDestMid = false;
	_float3 m_vDestMid;
	_float3 m_vDestEnd;
	_bool m_bStatue = false;
	_float m_fMaxHp = 0.f;

	CUI_BossHP* m_pHpBar = nullptr;
	CStatue_Gimmick* m_pGimmick = nullptr;

	CEffect_NidhoggField* m_pStatueField = nullptr;
public:
	static CMonster_Nidhog* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END