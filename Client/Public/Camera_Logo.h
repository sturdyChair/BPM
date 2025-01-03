

#pragma once

#include "Client_Defines.h"
#include "Camera.h"
BEGIN(Engine)
class CCharacterController;
END
BEGIN(Client)

class CCamera_Logo final : public CCamera
{


private:
	CCamera_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Logo(const CCamera_Logo& rhs);
	virtual ~CCamera_Logo() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void PriorityTick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	void Set_Speed(_float fSpeed) { m_fSpeed = fSpeed; }

	_float Get_TargetAngle()const { return m_fTargetAngle; }
	void Set_TargetAngle(_float fTA) { m_fTargetAngle = fTA; }

private:
	_float				m_fMouseSensor = {};
	_float				m_fSpeed = 0.f;
	_float				m_fAngle = 0.01f;
	_float				m_fTargetAngle = 0.01f;
	_float4x4			m_matTrans;

public:
	static CCamera_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
//
END