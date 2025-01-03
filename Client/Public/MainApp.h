#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	HRESULT Render();
	class CUI_Controller* Get_UI_Controller() { return m_pUI_Controller; }

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	CGameInstance*				m_pGameInstance = { nullptr };
	class CBeat_Manager*		m_pBeatManager = nullptr;
	class CUI_Controller*		m_pUI_Controller = nullptr;
	class CImGui_Manager*		m_pImguiManager = nullptr;
	class CPlayer_Manager*		m_pPlayerManager = nullptr;
	class CRoomManager*			m_pRoomManager = nullptr;
private:
	HRESULT Open_Level(LEVEL eLevelID);
	HRESULT Ready_Prototype_Component();


public:
	static CMainApp* Create();
	virtual void Free() override;
};

END