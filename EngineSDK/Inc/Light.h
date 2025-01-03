#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLight : public CBase
{
protected:
	CLight();
	virtual ~CLight() = default;

public:
	void Activate(_bool bToggle) { m_bActive = bToggle; }
	_bool Is_Active() const { return m_bActive; }


protected:
	//PointLight			m_PointLight{};
	//SpotLight			m_SpotLight{};
	//DirectionalLight	m_DirectionalLight{};
	LIGHT_DESC::TYPE	m_eType;
	bool				m_bActive = true;

public:
	virtual void Free() override;
};

END