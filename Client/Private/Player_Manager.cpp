#include "Player_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CPlayer_Manager)

CPlayer_Manager::CPlayer_Manager()
{
}

void CPlayer_Manager::Kill_Instance()
{
	Free();
	Destroy_Instance();
}

void CPlayer_Manager::Register_Player(CPlayer* pPlayer)
{
	Safe_Release(m_pPlayer);
	m_pPlayer = pPlayer;
	Safe_AddRef(m_pPlayer);
}

void CPlayer_Manager::Free()
{
	Safe_Release(m_pPlayer);
	__super::Free();
}
