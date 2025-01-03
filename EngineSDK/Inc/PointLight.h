#pragma once
#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CPointLight : public CLight
{
private:
	CPointLight();
	virtual ~CPointLight() = default;

public:
	HRESULT Initialize(const PointLight& LightDesc);
	const PointLight& Get_Light() const { return m_PointLight; }
	void Set_Position(const _float3& vPos);
	void Set_Position(_fvector vPos);
	void Set_Range(_float fRange);
	void Set_Intensity(_float fRange);

private:
	PointLight			m_PointLight{};
	//SpotLight			m_SpotLight{};
	//DirectionalLight	m_DirectionalLight{};


public:
	static CPointLight* Create(const PointLight& LightDesc);
	virtual void Free() override;
};

END