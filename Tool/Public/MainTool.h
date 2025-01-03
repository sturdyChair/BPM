#pragma once
class CMainTool
{
};

#pragma once

#include "../../Client/Public/Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Tool)

class CMainTool final : public CBase
{
private:
	CMainTool();
	virtual ~CMainTool() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	HRESULT Render();


private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	CGameInstance* m_pGameInstance = { nullptr };



private:


public:
	static CMainTool* Create();
	virtual void Free() override;
};

END