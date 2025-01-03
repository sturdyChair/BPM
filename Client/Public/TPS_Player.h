#pragma once
#include "Player.h"

BEGIN(Engine)
class CTexture;
END
BEGIN(Client)
class CEffect_DashIndicator;

class CTPS_Player : public CPlayer
{
public:
	enum STATE
	{
		START,
		IDLE,
		UPPER_IDLE,
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
	CTPS_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTPS_Player(const CTPS_Player& rhs);
	virtual ~CTPS_Player() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void PriorityTick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void Cam_Shake(_float fDuration, _float fPower);
	virtual void Cam_Dissolve(_float fDuration, _float fRange);

	virtual CCamera* Get_Camera() { return (CCamera*)m_pTPSCam; }
	virtual void Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser) override;

private:

	void Upper_End();

private:
	class CCamera_TPS* m_pTPSCam = nullptr;
	string m_strAnimTag[STATE_END];
	_float m_fAnimSpeed[STATE_END]{};
	STATE m_ePrevState = IDLE;
	STATE m_eState = IDLE;

	STATE m_eUpperState = IDLE;
	STATE m_ePrevUpperState = IDLE;

	_float m_fKeyAngle = 0.f;

	_float4x4 m_MatLeftHand;
	_float4x4 m_MatRightHand;

	CAfterImage* m_pAfterImage = nullptr;
	_bool m_bZapDash = false;

	CEffect_DashIndicator* m_pDashIndicator = nullptr;
	_float4x4 m_matDashIndicator;

	_float m_fDisintegration = 0.f;
	_float m_fDisintegrationTarget = 0.f;
	_bool m_bDisintegration = false;
	CTexture* m_pLinearUV = nullptr;

public:
	static CTPS_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END