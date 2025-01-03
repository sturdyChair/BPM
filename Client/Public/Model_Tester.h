
#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CModel_Tester : public CGameObject
{
public:
	class MODEL_TESTER_DESC
	{
	public:
		_wstring strModelTag;
		_float4x4 Transform;
	};

private:
	CModel_Tester(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel_Tester(const CModel_Tester& rhs);
	virtual ~CModel_Tester() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;

private:
	HRESULT Ready_Components(MODEL_TESTER_DESC& desc);
	HRESULT Bind_ShaderResources();

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;

public:
	static CModel_Tester* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END