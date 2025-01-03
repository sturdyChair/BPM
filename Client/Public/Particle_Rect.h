#pragma once

#include "Particle.h"

BEGIN(Client)

class CParticle_Rect : public CParticle
{
private:
	CParticle_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Rect(const CParticle_Rect& rhs);
	virtual ~CParticle_Rect() = default;

public:
	virtual HRESULT Initialize(void* Arg) override;
	virtual HRESULT Initialize_Prototype(const PARTICLE_DESC& desc) override;

	virtual void Push_From_Pool(void* pArg) override;
	virtual void Dying() override;

private:
	_wstring m_strBufferTag;

public:
	static  CParticle_Rect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const PARTICLE_DESC& desc);
	CGameObject* Clone(void* pArg) override;

	virtual void Free() override;
};

END