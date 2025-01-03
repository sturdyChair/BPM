

#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CPlayerSelectModel : public CGameObject
{
public:
	class PSM
	{
	public:
		_wstring strModelTag;
		string strIdleTag;
		string strSelectedTag;
	};


private:
	CPlayerSelectModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerSelectModel(const CPlayerSelectModel& rhs);
	virtual ~CPlayerSelectModel() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;

	void Selected(_bool bFlag);

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;
	PSM		m_tDesc;
public:
	static CPlayerSelectModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END