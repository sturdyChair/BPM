

#pragma once
#include "Client_Defines.h"
#include "VIBuffer_Trail.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CTrail : public CGameObject
{
public:
	class TRAIL_DESC : public CVIBuffer_Trail::TRAIL_DESC
	{
	public:
		_wstring strTextureTag;
		_uint iRenderPass = 0;
		_bool bTextureAnim = false;
		_float4x4* pUpperTransform = nullptr;
		_float4x4* pDownTransform = nullptr;
	};
protected:
	CTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrail(const CTrail& rhs);
	virtual ~CTrail() = default;

public:
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Push_From_Pool(void* pArg) override;
	virtual void Dying() override;
	virtual HRESULT Initialize_Prototype(const TRAIL_DESC& desc);
	virtual HRESULT Initialize(void* Arg);
	void Set_RG(CRenderer::RENDERGROUP eRG) { m_eRG = eRG; }
	void Set_RenderPass(_uint iPass) { m_iRenderPass = iPass; }

protected:
	TRAIL_DESC m_tDesc;
	_uint m_iRenderPass = 0;
	CVIBuffer_Trail* m_pVBCom = nullptr;
	CShader* m_pShaderCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CRenderer::RENDERGROUP m_eRG = CRenderer::RENDER_BLEND_NON_LIT;
public:
	static CTrail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const TRAIL_DESC& desc);
	virtual CTrail* Clone(void* pArg) override;
	virtual void Free() override;
};

END