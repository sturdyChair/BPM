

#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CPlayer_Manager;

class CItem_Base : public CGameObject
{
protected:
	CItem_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_Base(const CItem_Base& rhs);
	virtual ~CItem_Base() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;

protected:
	virtual HRESULT Ready_Components();
	virtual void _Initialize() {};

	virtual void Interact() {}

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;

	CPlayer_Manager* m_pPlayerManager = nullptr;
	_uint m_iRenderPass = 0;
	_uint m_iPrice = 0;
	ITEM_TEXURE_ID m_eID;
	SWAP_ID m_eSwapID = SWAP_ITEM;

	_wstring m_strDesc = L"";
	_wstring m_strName = L"";

public:
	static CItem_Base* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END