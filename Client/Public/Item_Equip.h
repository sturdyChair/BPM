

#pragma once
#include "Client_Defines.h"
#include "Item_Base.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CPlayer_Manager;

class CItem_Equip : public CItem_Base
{
public:
	class EQUIP_DESC
	{
	public:
		ITEM_TEXURE_ID eID;
		_wstring strModelTag;
		_wstring strDesc = L"";
		_wstring strName = L"";
		SLOT_ID eSlotID;
		_int iPrice = 0;
	};


protected:
	CItem_Equip(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_Equip(const CItem_Equip& rhs);
	virtual ~CItem_Equip() = default;

public:
	HRESULT Initialize_Prototype(const EQUIP_DESC& desc);


	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;
	SLOT_ID Get_Slot() const { return m_eSlotID; }
	
	//Implement this
	virtual void UnEquip(const _float4x4& Transform);
	virtual void Equip();
	virtual void Activate() {}
	ITEM_TEXURE_ID Get_ID() const { return m_eID; }


protected:
	virtual HRESULT Ready_Components() override;
	virtual void _Initialize() override;

	virtual void Interact() override;


protected:
	_bool m_bEquipped = false;
	SLOT_ID			m_eSlotID = SLOT_END;

	_wstring		m_strModelTag;

public:
	static CItem_Equip* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const EQUIP_DESC& desc);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


END