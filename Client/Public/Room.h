
#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CRoom : public CGameObject
{
public:
	class ROOM_DESC
	{
	public:
		ROOM_TEXTURE_ID eRoomType;
		string  strRoomFilePath;
		LEVEL	eLevel;
	};

	class CLONEING_DESC
	{
	public:
		_int pRoomConnected[4]{ -1,-1,-1,-1 };
	};


private:
	CRoom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRoom(const CRoom& rhs);
	virtual ~CRoom() = default;

public:
	virtual HRESULT Initialize_Prototype(ROOM_DESC& desc);
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;
	_uint	Get_NumPortal()const { return m_vecPortalTags.size(); }
	_int*   Get_RoomConnected() { return m_pRoomConnected; }
	_float* Get_PortalDistance() { return m_fPortalDistance; }
	void	Enter_Room(_int iPrevRoomIdx, _int iCurrentLevel = -1);

	void	Clear();

	ROOM_TEXTURE_ID Get_RoomType() const { return m_eRoomType; }

	void Add_Object(class CGameObject* pObj);

	vector<class CMonster_Base*>& Get_Monsters() { return m_vecMonsters; }

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	HRESULT Read_Room(const string& strRoomFilePath);

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;
	bool    m_bCleared = false;
	_float4x4 m_PortalTransform[4]{};
	_float    m_fPortalDistance[4]{};
	wstring m_strModelTag;
	wstring m_strRoomTag;
	_int m_pRoomConnected[4]{ -1,-1,-1,-1 };
	ROOM_TEXTURE_ID m_eRoomType = ROOM_DEFAULT;

	vector<class CMonster_Base*>				 m_vecMonsters;
	vector<pair<_tchar[256], _float4x4>>			 m_vecMonsterTags;
	
	vector<CGameObject*>						 m_vecObjects;
	vector<pair<_tchar[256], _float4x4>>		     m_vecObjectTags;

	vector<class CPortal*>						 m_vecPortals;
	vector<pair<_tchar[256], _float4x4>> m_vecPortalTags;
	LEVEL m_eLevel;
public:
	static CRoom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ROOM_DESC& desc);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END