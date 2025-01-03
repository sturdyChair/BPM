#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CImGui_Manager final : public CBase
{
	DECLARE_SINGLETON(CImGui_Manager);

private:
	CImGui_Manager();
	virtual ~CImGui_Manager() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
	void Tick();
	HRESULT Render();

	CGameObject* Add_Model(const _char* szModelTag);
	CGameObject* Add_Object(const _char* szPrototypeTag);

	CGameObject* Add_Monster(const _char* szPrototypeTag);

	void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition);
	CGameObject* Add_Portal(_int iIdx);
	CGameObject* Add_Portal(const _char* szModelTag, _fmatrix Transform);


	void Save(const string& strFlieName);

	void Load(const string& strFileName);

public:
	vector<const _char*> m_MapModels;
	_int m_iMapListItem = 0;
	vector<const _char*> m_MapOutLiner;
	vector<CGameObject*> m_Objects;
	CGameObject* m_pMap = nullptr;
	CGameObject* m_pSelected = nullptr;
	ImGuizmo::OPERATION mCurrentGizmoOperation;
	_float identityMatrix[16] = { 1.f,0.f,0.f,0.f,
								  0.f,1.f,0.f,0.f,
								  0.f,0.f,1.f,0.f,
								  0.f,0.f,0.f,1.f, };

	vector<const _char*> m_PortalModels;

	_int m_iPortalListItem = 0;
	vector<const _char*>		   m_PortalOutLiner;

	vector<class CPortal*> m_Portals;
	
	vector<const _char*> m_ObjectPrototype;
	_int m_iObjListItem = 0;

	vector<const _char*> m_MonsterPrototype;
	_int m_iMonsterListItem = 0;
	vector<const _char*>		   m_MonsterOutLiner;
	vector<class CMonster_Base*> m_Monsters;
	

	vector<string> m_Load_Portal;
	vector<string> m_Load_Object;
	vector<string> m_Load_Monster;

public:
	virtual void Free() override;
};

END