
#pragma once

#include "Client_Defines.h"
#include "Camera.h"
BEGIN(Engine)
class CCharacterController;
class CVIBuffer_Rect;
class CTexture;
class CShader;
END
BEGIN(Client)

class CCamera_FPS final : public CCamera
{
public:
	enum CAM_EFFECT_TYPE
	{
		CE_SHAKE,
		CE_DISSOLVE,
		CE_END,
	};

	enum LENS_EFFECT_TYPE
	{
		LE_REDSCREEN,
		LE_COLORSCREEN,
		LE_END,
	};

	struct CAM_EFFECT
	{
		_float			fTimer;
		_float			fPower;
		_float			fDuration;
	};
	struct LENS_EFFECT
	{
		_float			fTimer;
		_float4			vColor;
		_float			fDuration;
	};
private:
	CCamera_FPS(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_FPS(const CCamera_FPS& rhs);
	virtual ~CCamera_FPS() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void PriorityTick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	void Set_Target(CTransform* pTarget);
	void Set_Offset(_fvector vOffset);
	
	void Shake(_float fDuration, _float fPower);
	void Dissolve(_float fDuration, _float fPower);

	void Clear_LensEffect();
	void RedScreen(_float fDuration, const _float4& vColor);
	void ColorScreen(_float fDuration, const _float4& vColor);

private:
	void _Shake(_float fTimeDelta);
	void _Dissolve(_float fTimeDelta);
	void _LensEffect(_float fTimeDelta);

private:
	CTransform* m_pTargetTransform = nullptr;
	_float3		m_vOffset{};
	_float4x4	m_EffectOffset{};

	_float4x4   m_WorldMat{};
	CTexture* m_pRedScreenTexture = nullptr;
	CTexture* m_pWhiteTexture = nullptr;


	CShader* m_pShaderCom = nullptr;
	CVIBuffer_Rect* m_pLensBuffer = nullptr;

	list<CAM_EFFECT> m_arrEffect[CE_END];
	list<LENS_EFFECT> m_arrLensEffect[LE_END];

	_bool m_bLensEffect[LE_END]{};

public:
	static CCamera_FPS* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
//
END