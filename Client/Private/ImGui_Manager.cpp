
#include "ImGui_Manager.h"
#include "Model_Tester.h"
#include "GameInstance.h"
#include "Portal.h"
#include "Monster_Base.h"

IMPLEMENT_SINGLETON(CImGui_Manager)

CImGui_Manager::CImGui_Manager()
{
}

HRESULT CImGui_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(pDevice, pContext);

	return S_OK;
}

void CImGui_Manager::Tick()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    if (m_pSelected)
    {
        ImGuizmo::BeginFrame();
        float windowWidth = (float)ImGui::GetIO().DisplaySize.x;
        float windowHeight = (float)ImGui::GetIO().DisplaySize.y;
        ImGuizmo::SetRect(0, 0, windowWidth, windowHeight);
        ImGuizmo::SetOrthographic(false);

        _float fTransform[16], fView[16], fProjection[16];
        XMFloat4x4ToFloatArray(m_pSelected->Get_Transform()->Get_WorldMatrix(), fTransform);
        XMFloat4x4ToFloatArray(CGameInstance::Get_Instance()->Get_Transform_Float4x4(CPipeLine::PIPELINE_STATE::TS_VIEW), fView);
        XMFloat4x4ToFloatArray(CGameInstance::Get_Instance()->Get_Transform_Float4x4(CPipeLine::PIPELINE_STATE::TS_PROJ), fProjection);

        //ImGuizmo::Manipulate(fView, fProjection, ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, fTransform);
        EditTransform(fView, fProjection, fTransform, true);
        
        XMFLOAT4X4 Transform; FloatArrayToXMFloat4x4(fTransform, Transform);
        m_pSelected->Get_Transform()->Set_WorldMatrix(Transform);

    }

    static _bool bStart = { true };
    static bool bTest = false;
    if (true == bStart)
    {
        ImGui::Begin("Models");
        static bool bMapList = true;
        if (bMapList)
        {
            _int iListSize = m_MapModels.size();
            ImGui::ListBox("Map Model List", &m_iMapListItem, m_MapModels.data(), iListSize);

            if (ImGui::Button("Load_Map", ImVec2{ 60,20 }))
            {
                m_Objects.push_back(Add_Model(m_MapModels[m_iMapListItem]));
                m_MapOutLiner.push_back(m_MapModels[m_iMapListItem]);
                m_pMap = m_Objects.back();
                bMapList = false;
            }
        }

        {
            _int iListSize = m_ObjectPrototype.size();
            ImGui::ListBox("Prototype List", &m_iObjListItem, m_ObjectPrototype.data(), iListSize);

            if (ImGui::Button("Load_Clone", ImVec2{ 60,20 }))
            {
                m_Objects.push_back(Add_Object(m_ObjectPrototype[m_iObjListItem]));
                m_MapOutLiner.push_back(m_ObjectPrototype[m_iObjListItem]);
            }
        }
        
        ImGui::ListBox("Portal Model List", &m_iPortalListItem, m_PortalModels.data(), m_PortalModels.size());
        if (ImGui::Button("Load_Portal", ImVec2{ 60,20 }))
        {
            m_Portals.push_back((CPortal*)Add_Portal(m_iPortalListItem));
            m_PortalOutLiner.push_back(m_PortalModels[m_iPortalListItem]);
        }

        ImGui::ListBox("Monster Prototype List", &m_iMonsterListItem, m_MonsterPrototype.data(), m_MonsterPrototype.size());
        if (ImGui::Button("Load_Monster", ImVec2{ 60,20 }))
        {
            m_Monsters.push_back((CMonster_Base*)Add_Monster(m_MonsterPrototype[m_iMonsterListItem]));
            m_MonsterOutLiner.push_back(m_MonsterPrototype[m_iMonsterListItem]);
        
        }
        ImGui::End();
    }
    {
        ImGui::Begin("OutLiner");

        _int iListSize = m_Objects.size();
        static _int iWorldListItem;

        if (ImGui::ListBox("OutLiner", &iWorldListItem, m_MapOutLiner.data(), iListSize))
        {
            m_pSelected = m_Objects[iWorldListItem];
        }
        static _int iPortalItem;
        if (ImGui::ListBox("PortalOutLiner", &iPortalItem, m_PortalOutLiner.data(), m_Portals.size()))
        {
            m_pSelected = m_Portals[iPortalItem];
        }
        static _int iMonsterItem;
        if (ImGui::ListBox("MonsterOutLiner", &iMonsterItem, m_MonsterOutLiner.data(), m_MonsterOutLiner.size()))
        {
            m_pSelected = m_Monsters[iMonsterItem];
        }

        static char szFileName[256];
        ImGui::InputText("Output FileName", szFileName, 256);

        if (ImGui::Button("Save", ImVec2{ 60,20 }))
        {
            Save(szFileName);
        }       
        static char szFileNameIn[256];
        ImGui::InputText("Input FileName", szFileNameIn, 256);

        if (ImGui::Button("Load", ImVec2{ 60,20 }))
        {
            Load(szFileNameIn);
        }

        ImGui::End();
    }


    ImGui::EndFrame();

}

HRESULT CImGui_Manager::Render()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}

CGameObject* CImGui_Manager::Add_Model(const _char* szModelTag)
{
    string strTag = szModelTag;
    wstring wstTag{ strTag.begin(),strTag.end() };
    CModel_Tester::MODEL_TESTER_DESC desc;
    desc.strModelTag = wstTag;
    XMStoreFloat4x4(&desc.Transform, XMMatrixIdentity());
    return CGameInstance::Get_Instance()->Add_Clone_Return(LEVEL_TOOL, L"Model_Tester", L"Prototype_GameObject_Model_Tester", &desc);
}

CGameObject* CImGui_Manager::Add_Object(const _char* szPrototypeTag)
{
    string strTag = szPrototypeTag;
    wstring wstTag{ strTag.begin(),strTag.end() };
    return  CGameInstance::Get_Instance()->Add_Clone_Return(LEVEL_TOOL, L"Tool_Object", wstTag);
}

CGameObject* CImGui_Manager::Add_Monster(const _char* szPrototypeTag)
{
    string strTag = szPrototypeTag;
    wstring wstTag{ strTag.begin(),strTag.end() };
    return  CGameInstance::Get_Instance()->Add_Clone_Return(LEVEL_TOOL, L"Tool_Monster", wstTag);
}

void CImGui_Manager::EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
{
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
    static bool useSnap = false;
    static float snap[3] = { 1.f, 1.f, 1.f };
    static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
    static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
    static bool boundSizing = false;
    static bool boundSizingSnap = false;

    if (editTransformDecomposition)
    {
        if (ImGui::IsKeyPressed(ImGuiKey_T))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        //if (ImGui::IsKeyPressed(ImGuiKey_E))
        //    mCurrentGizmoOperation = ImGuizmo::ROTATE;
        if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::RadioButton("Universal", mCurrentGizmoOperation == ImGuizmo::UNIVERSAL))
            mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;
        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
        ImGui::InputFloat3("Tr", matrixTranslation);
        ImGui::InputFloat3("Rt", matrixRotation);
        ImGui::InputFloat3("Sc", matrixScale);
        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

        if (mCurrentGizmoOperation != ImGuizmo::SCALE)
        {
            if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
                mCurrentGizmoMode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
                mCurrentGizmoMode = ImGuizmo::WORLD;
        }
        //if (ImGui::IsKeyPressed(ImGuiKey_S))
        //    useSnap = !useSnap;
        ImGui::Checkbox("##UseSnap", &useSnap);
        ImGui::SameLine();

        switch (mCurrentGizmoOperation)
        {
        case ImGuizmo::TRANSLATE:
            ImGui::InputFloat3("Snap", &snap[0]);
            break;
        case ImGuizmo::ROTATE:
            ImGui::InputFloat("Angle Snap", &snap[0]);
            break;
        case ImGuizmo::SCALE:
            ImGui::InputFloat("Scale Snap", &snap[0]);
            break;
        }
        ImGui::Checkbox("Bound Sizing", &boundSizing);
        if (boundSizing)
        {
            ImGui::PushID(3);
            ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
            ImGui::SameLine();
            ImGui::InputFloat3("Snap", boundsSnap);
            ImGui::PopID();
        }
    }

    ImGuiIO& io = ImGui::GetIO();
    float viewManipulateRight = io.DisplaySize.x;
    float viewManipulateTop = 0;
    static ImGuiWindowFlags gizmoWindowFlags = 0;

    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

    
    ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);

    ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

}

CGameObject* CImGui_Manager::Add_Portal(_int iIdx)
{
    string  strTag = m_PortalModels[iIdx];
    wstring wstTag{ strTag.begin(),strTag.end() };
    CPortal::PORTAL_DESC desc;
    desc.strModelTag = wstTag;

    XMStoreFloat4x4(&desc.Transform, XMMatrixIdentity());
    desc.bFrame = false;
    return CGameInstance::Get_Instance()->Add_Clone_Return(LEVEL_TOOL, L"Model_Tester", L"Prototype_GameObject_Portal", &desc);
}

CGameObject* CImGui_Manager::Add_Portal(const _char* szModelTag, _fmatrix Transform)
{
    string  strTag = szModelTag;
    wstring wstTag{ strTag.begin(),strTag.end() };
    CPortal::PORTAL_DESC desc;
    desc.strModelTag = wstTag;
    XMStoreFloat4x4(&desc.Transform, Transform);
    desc.bFrame = false;
    return CGameInstance::Get_Instance()->Add_Clone_Return(LEVEL_TOOL, L"Model_Tester", L"Prototype_GameObject_Portal", &desc);
}

void CImGui_Manager::Save(const string& strFlieName)
{
    ofstream out(strFlieName);
    if (!out.is_open())
    {
        MSG_BOX(L"No Such Directory");
        return;
    }
    out << "[Map] " << m_MapOutLiner[0] << '\n';
    out << "[Objects] " << m_MapOutLiner.size() << '\n';
    for (int i = 1; i < m_MapOutLiner.size(); ++i)
    {
        out << m_MapOutLiner[i] << '\n';
        _float4x4 Transform = m_Objects[i]->Get_Transform()->Get_WorldMatrix();
        out << Transform._11 << ' ' << Transform._12 << ' ' << Transform._13 << ' ' << Transform._14 << '\n'
            << Transform._21 << ' ' << Transform._22 << ' ' << Transform._23 << ' ' << Transform._24 << '\n'
            << Transform._31 << ' ' << Transform._32 << ' ' << Transform._33 << ' ' << Transform._34 << '\n'
            << Transform._41 << ' ' << Transform._42 << ' ' << Transform._43 << ' ' << Transform._44 << '\n';
    }
    out << "[Portals] " << m_PortalOutLiner.size() << '\n';
    for (int i = 0; i < m_PortalOutLiner.size(); ++i)
    {
        out << m_PortalOutLiner[i] << '\n';
        _float4x4 Transform = m_Portals[i]->Get_Transform()->Get_WorldMatrix();
        out << Transform._11 << ' ' << Transform._12 << ' ' << Transform._13 << ' ' << Transform._14 << '\n'
            << Transform._21 << ' ' << Transform._22 << ' ' << Transform._23 << ' ' << Transform._24 << '\n'
            << Transform._31 << ' ' << Transform._32 << ' ' << Transform._33 << ' ' << Transform._34 << '\n'
            << Transform._41 << ' ' << Transform._42 << ' ' << Transform._43 << ' ' << Transform._44 << '\n';
    }
    out << "[Monsters] " << m_MonsterOutLiner.size() << '\n';
    for (int i = 0; i < m_MonsterOutLiner.size(); ++i)
    {
        out << m_MonsterOutLiner[i] << '\n';
        _float4x4 Transform = m_Monsters[i]->Get_Transform()->Get_WorldMatrix();
        out << Transform._11 << ' ' << Transform._12 << ' ' << Transform._13 << ' ' << Transform._14 << '\n'
            << Transform._21 << ' ' << Transform._22 << ' ' << Transform._23 << ' ' << Transform._24 << '\n'
            << Transform._31 << ' ' << Transform._32 << ' ' << Transform._33 << ' ' << Transform._34 << '\n'
            << Transform._41 << ' ' << Transform._42 << ' ' << Transform._43 << ' ' << Transform._44 << '\n';
    }
}

void CImGui_Manager::Load(const string& strFileName)
{
    string strTemp;
    ifstream in(strFileName);
    if (!in.is_open())
    {
        MSG_BOX(L"No Such File");
        return;
    }
    string strMap;
    in >> strTemp >> strMap;
    _int iNumObj;
    in >> strTemp >> iNumObj;
    m_Load_Object.resize(iNumObj);
    m_Load_Object[0] = strMap;
    m_MapOutLiner.resize(iNumObj);
    
    m_MapOutLiner[0] = m_Load_Object[0].c_str();
    m_pMap = Add_Model(m_MapOutLiner[0]);
    m_Objects.push_back(m_pMap);
    _float4x4 Transform;

    for (_int i = 1; i < iNumObj; ++i)
    {
        in >> strTemp;
        m_Load_Object[i] = strTemp;
        m_MapOutLiner[i] = m_Load_Object[i].c_str();
        in  >> Transform._11 >> Transform._12 >> Transform._13 >> Transform._14
            >> Transform._21 >> Transform._22 >> Transform._23 >> Transform._24
            >> Transform._31 >> Transform._32 >> Transform._33 >> Transform._34
            >> Transform._41 >> Transform._42 >> Transform._43 >> Transform._44;
        auto pObj = Add_Object(m_MapOutLiner[i]);
        m_Objects.push_back(pObj);
        pObj->Get_Transform()->Set_WorldMatrix(Transform);
    }
    in >> strTemp >> iNumObj;
    m_PortalOutLiner.resize(iNumObj);
    m_Load_Portal.resize(iNumObj);
    for (_int i = 0; i < iNumObj; ++i)
    {
        in >> strTemp;
        m_Load_Portal[i] = strTemp;
        m_PortalOutLiner[i] = m_Load_Portal[i].c_str();

        in >> Transform._11 >> Transform._12 >> Transform._13 >> Transform._14
            >> Transform._21 >> Transform._22 >> Transform._23 >> Transform._24
            >> Transform._31 >> Transform._32 >> Transform._33 >> Transform._34
            >> Transform._41 >> Transform._42 >> Transform._43 >> Transform._44;
        auto pObj = Add_Portal(m_PortalOutLiner[i], XMLoadFloat4x4(&Transform));
        m_Portals.push_back((CPortal*)pObj);
    }
    in >> strTemp >> iNumObj;
    m_MonsterOutLiner.resize(iNumObj);
    m_Load_Monster.resize(iNumObj);
    for (_int i = 0; i < iNumObj; ++i)
    {
        in >> strTemp;
        m_Load_Monster[i] = strTemp;
        m_MonsterOutLiner[i] = m_Load_Monster[i].c_str();

        in >> Transform._11 >> Transform._12 >> Transform._13 >> Transform._14
            >> Transform._21 >> Transform._22 >> Transform._23 >> Transform._24
            >> Transform._31 >> Transform._32 >> Transform._33 >> Transform._34
            >> Transform._41 >> Transform._42 >> Transform._43 >> Transform._44;
        auto pObj = Add_Monster(m_MonsterOutLiner[i]);
        m_Monsters.push_back((CMonster_Base*)pObj);
        pObj->Get_Transform()->Set_WorldMatrix(Transform);
    }

}

void CImGui_Manager::Free()
{
    __super::Free();
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
