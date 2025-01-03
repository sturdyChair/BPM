#pragma once
#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CSpotLight : public CLight
{
private:
	CSpotLight();
	virtual ~CSpotLight() = default;

public:
	HRESULT Initialize(const SpotLight& LightDesc);
	const SpotLight& Get_Light() const { return m_SpotLight; }
	void Set_Position(const _float3& vPos);
	void Set_Position(_fvector vPos);
	void Set_Direction(const _float3& vDir);
	void Set_Direction(_fvector vDir);
	void Set_Color(const _float3& vColor);
	void Set_Color(_fvector vColor);
	void Set_Intensity(_float fIntensity);

private:
	//PointLight			m_PointLight{};
	SpotLight			m_SpotLight{};
	//DirectionalLight	m_DirectionalLight{};


public:
	static CSpotLight* Create(const SpotLight& LightDesc);
	virtual void Free() override;
};

END