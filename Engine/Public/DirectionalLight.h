#pragma once
#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CDirectionalLight : public CLight
{
private:
	CDirectionalLight();
	virtual ~CDirectionalLight() = default;

public:
	HRESULT Initialize(const DirectionalLight& LightDesc);
	const DirectionalLight& Get_Light() const { return m_DirectionalLight; }


private:
	//PointLight			m_PointLight{};
	//SpotLight			m_SpotLight{};
	DirectionalLight	m_DirectionalLight{};


public:
	static CDirectionalLight* Create(const DirectionalLight& LightDesc);
	virtual void Free() override;
};

END