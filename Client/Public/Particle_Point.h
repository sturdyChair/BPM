#pragma once

#include "Particle.h"

BEGIN(Client)

class CParticle_Point : public CParticle
{
private:
	CParticle_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Point(const CParticle_Point& rhs);
	virtual ~CParticle_Point() = default;

public:
	virtual HRESULT Initialize(void* Arg) override;
	virtual HRESULT Initialize_Prototype(const PARTICLE_DESC& desc) override;

	virtual void Push_From_Pool(void* pArg) override;
	virtual void Dying() override;
	virtual HRESULT Render() override;

private:
	_wstring m_strBufferTag;

public:
	static  CParticle_Point* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const PARTICLE_DESC& desc);
	CGameObject* Clone(void* pArg) override;

	virtual void Free() override;
};

END