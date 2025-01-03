#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;
public:
	virtual HRESULT Initialize_Prototype(const _wstring& strTextureFilePath, _uint iNumTexture);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render() override;

	_bool  MoveFrame(_float fTimeDelta, _bool bLoop = true);
	const _uint& Get_Frame()const { return m_iCurrentTex; }
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName, _uint iTextureIdx = 0);
	HRESULT Bind_ShaderResources(class CShader* pShader, const _char* pConstantName, _uint iFromIdx = 0);
	void Set_Speed(_float fSpeed) { m_fSpeed = fSpeed; }

private:
	vector<ID3D11ShaderResourceView*> m_SRVs;
	_uint							  m_iNumTexture = 0;
	_uint							  m_iCurrentTex = 0;
	_float							  m_fSpeed = 1.f;
	_float							  m_fTimeAcc = 0.f;
public:
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strTextureFilePath, _uint iNumTexture);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END
