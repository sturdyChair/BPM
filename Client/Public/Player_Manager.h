#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CPlayer;

class CPlayer_Manager final : public CBase
{
	DECLARE_SINGLETON(CPlayer_Manager)
private:
	CPlayer_Manager();
	virtual ~CPlayer_Manager() = default;
public:
	void Kill_Instance();

	void Register_Player(CPlayer* pPlayer);

	CPlayer* Get_Player() { return m_pPlayer; }

private:
	CPlayer* m_pPlayer = nullptr;

public:
	virtual void Free() override;
};

END
