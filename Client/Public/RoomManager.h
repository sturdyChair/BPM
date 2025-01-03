
#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)
class CPlayer;
class CRoom;

class CRoomManager final : public CBase
{
	DECLARE_SINGLETON(CRoomManager)
public:
	enum ROOM_SHAPE_TYPE
	{
		RST_L,
		RST_I,
		RST_T,
		RST_CROSS,
		RST_SPECIAL,
		RST_END,
	};
	enum SPECIAL_ROOM_TAG
	{
		ARMORY,
		BOSS,
		SHOP,
		START,
		TREASURE,
		SRT_END,
	};
	enum SVART_ROOM_TAG
	{
		START_SV,
		CHOICE_SV,
		LIB_SV,
		PORTAL_SV,
		STRAIGHT_SV,
		TREASURE_SV,
		SVART_END,
	};

private:
	CRoomManager();
	virtual ~CRoomManager() = default;
public:
	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	void Kill_Instance();

	void Purge_Rooms();
	void Resize_Room();//!!calling this function will purge all cloned Rooms

	void Generate_Room();
	void Generate_Vana();
	void Generate_Svart();
	void Generate_Hel();
	void Generate_BossRush();

	_uint Get_CurrentRoomIdx() const { return m_iCurrentRoomIdx; }
	CRoom* Get_Room(_int idx) { return m_Rooms[idx / 5][idx % 5]; }

	CRoom* Get_CurrentRoom() { return m_pCurrentRoom; }
	void Set_CurrentRoom(CRoom* pRoom) { m_pCurrentRoom = pRoom; }

	void Ready_Rooms_Asgard();
	void Ready_Rooms_Vana();
	void Ready_Rooms_Svart();
	void Ready_Rooms_Hel();
	void Ready_Rooms_BossRush();

	void Start(LEVEL eLevel);

	void Set_CurrentRoomIdx(_int iCurrentRoomIdx) { m_iCurrentRoomIdx = iCurrentRoomIdx; }

	void Purge_Prototype();
	void Register_PooledObjectTag(_wstring strTag);
	
private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	vector<vector<CRoom*>> m_Rooms;
	_uint m_iSizeX = 5;
	_uint m_iSizeY = 5;
	_uint m_iStart = 12;

	vector<CRoom*> m_RoomPrototype[RST_END];


	_int m_iCurrentRoomIdx = 0;
	CRoom* m_pCurrentRoom = nullptr;

	CGameInstance* m_pGameInstance = nullptr;

	set<_wstring> m_PooledObjectToClear;

public:
	virtual void Free() override;
};

END
