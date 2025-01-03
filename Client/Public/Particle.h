#pragma once
#include "Client_Defines.h"
#include "VIBuffer_Rect_Instance.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CVIBuffer_Instance;
class CTexture;
END

BEGIN(Client)

class CParticle : public CGameObject
{
public:
	class PARTICLE_DESC : public CVIBuffer_Rect_Instance::INSTANCE_DESC
	{
	public:
		_wstring strTextureTag;
		_uint iRenderPass = 0;
		PARTICLE_MOVEMENT eMovementType;
		_bool bTextureAnim = false;
		_float fMaxLifeTime = 0.f;
		_bool bInfinite = false;
	};
protected:
	CParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle(const CParticle& rhs);
	virtual ~CParticle() = default;
	
public:
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Push_From_Pool(void* pArg) override;
	virtual void Dying() override;
	virtual HRESULT Initialize_Prototype(const PARTICLE_DESC& desc);
	virtual HRESULT Initialize(void* Arg);


protected:
	PARTICLE_DESC m_tDesc{};
	_float m_fLifeTime = 0.f;
	_uint m_iRenderPass = 0;
	CVIBuffer_Instance* m_pVBCom = nullptr;
	CShader* m_pShaderCom = nullptr;
	CTexture* m_pTextureCom = nullptr;

public:
	virtual void Free() override;
};

END