#pragma once

#include "Base.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CUI_Controller final : public CBase
{
	DECLARE_SINGLETON(CUI_Controller)

public:
	struct CONTROLLED_UI
	{
		class CUI_CrossHair*	pUI_CrossHair = nullptr;
		class CUI_Hp*			pUI_Hp = nullptr;
		class CUI_EquipSlot*	pUI_EquipSlot = nullptr;
		class CUI_WeaponSlot*	pUI_WeaponSlot = nullptr;
		class CUI_AbilitySlot*	pUI_AbilitySlot = nullptr;
		class CUI_Score*		pUI_Score = nullptr;
		class CUI_MiniMap*		pUI_MiniMap = nullptr;
		class CUI_SwapItem*		pUI_SwapItem = nullptr;
	};
	enum class UI_ID
	{
		CRISSHAIR,
		HP,
		EQUIP,
		WEAPON,
		ABILITY,
		SCORE,
		MINIMAP,
		SWAPITEM,
		ID_END,
	};

private:
	CUI_Controller();
	virtual ~CUI_Controller() = default;

public:
	void Tick(_float fTimeDelta);

	void Initialize();
	void Kill_Instance();

	HRESULT Set_UI(CONTROLLED_UI* pUI);
	class CUI_CrossHair*		Get_CrossHair() { return m_pUI_CrossHair; }
	class CUI_Hp*				Get_Hp()		 { return m_pUI_Hp; }
	class CUI_EquipSlot*		Get_EquipSlot() { return m_pUI_EquipSlot; }
	class CUI_WeaponSlot*		Get_WeaponSlot() { return m_pUI_WeaponSlot; }
	class CUI_AbilitySlot*		Get_AbilitySlot() { return m_pUI_AbilitySlot; }
	class CUI_Score*			Get_Score() { return m_pUI_Score; }
	class CUI_MiniMap*			Get_MiniMap() { return m_pUI_MiniMap; }
	class CUI_SwapItem*			Get_SwapItem() { return m_pUI_SwapItem; }
	void Set_PosScale(_float fScale) { m_fPosScale = fScale; }

	void    Set_Desc(const ITEM_UI_DESC& desc);

	void Bad_Beat();
	void Dash();
	void Push_UI(LEVEL eLevel);

private:
	CGameInstance*			m_pGameInstance = nullptr;
	CBeat_Manager*			m_pBeatManager = nullptr;
	class CUI_CrossHair*	m_pUI_CrossHair = nullptr;
	class CUI_Hp*			m_pUI_Hp = nullptr;
	class CUI_EquipSlot*	m_pUI_EquipSlot = nullptr;
	class CUI_WeaponSlot*	m_pUI_WeaponSlot = nullptr;
	class CUI_AbilitySlot*	m_pUI_AbilitySlot = nullptr;
	class CUI_Score*		m_pUI_Score = nullptr;
	class CUI_MiniMap*		m_pUI_MiniMap = nullptr;
	class CUI_SwapItem*		m_pUI_SwapItem = nullptr;

	_float  m_fPosScale = 1.f;

	_float3 m_vOrigin[(_uint)UI_ID::ID_END]{};

public:
	virtual void Free() override;
};

END