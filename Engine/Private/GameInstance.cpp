#include "..\Public\GameInstance.h"

#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Object_Manager.h"
#include "PhysxManager.h"
#include "DelayedProcessManager.h"
#include "Object_Pool.h"
#include "Input_Device.h"
#include "SoundManager.h"
#include "Font_Manager.h"
#include "Light_Manager.h"
#include "Frustum.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{

}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, const ENGINE_DESC& EngineDesc, _Out_ ID3D11Device** ppDevice, _Out_ ID3D11DeviceContext** ppContext)
{
	/* ������ ����ϱ����� �ʿ��� ��Ÿ �ʱ�ȭ �۾����� ��ģ��. */

	/* �׷��� ����̽� �ʱ�ȭ. */
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	m_iWinSizeX = EngineDesc.iWinSizeX;
	m_iWinSizeY = EngineDesc.iWinSizeY;
	m_hWnd = EngineDesc.hWnd;
	/* ��ǲ ����̽� �ʱ�ȭ. */

	/* ���� ����̽� �ʱ�ȭ. */

	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	/* ���� �Ŵ����� �غ��س��´�. */
	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;


	/* ������Ʈ �Ŵ��� ����� �غ�. . */
	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	/* ������Ʈ �Ŵ��� ����� �غ�. . */
	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;
	m_pPhysxManager = CPhysxManager::Create();
	if (nullptr == m_pPhysxManager)
		return E_FAIL;
	m_pDelayedProcessManager = CDelayedProcessManager::Create();
	if (nullptr == m_pDelayedProcessManager)
		return E_FAIL;
	m_pObject_Pool = CObject_Pool::Create();
	if (nullptr == m_pObject_Pool)
		return E_FAIL;
	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;
	m_pInput_Device = CInput_Device::Create(EngineDesc.hInstance, EngineDesc.hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;
	m_pSoundManager = CSoundManager::Create();
	if (nullptr == m_pSoundManager)
		return E_FAIL;
	m_pFontManager = CFont_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pFontManager)
		return E_FAIL;
	m_pLightManager = CLight_Manager::Create();
	if (nullptr == m_pLightManager)
		return E_FAIL;
	m_pFrustum = CFrustum::Create();
	if (nullptr == m_pFrustum)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager || 
		nullptr == m_pObject_Manager)
		return;
	m_pDelayedProcessManager->Tick(fTimeDelta);
	m_pInput_Device->Update_InputDev();
	m_pObject_Manager->PriorityTick(fTimeDelta);

	/* ī�޶� �������� ��, ��������� �̿�����. */
	m_pPipeLine->Update();
	m_pFrustum->Update();

	m_pObject_Manager->Tick(fTimeDelta);
	m_pObject_Manager->LateTick(fTimeDelta);

	m_pLevel_Manager->Tick(fTimeDelta);
	m_pPhysxManager->Simulate(fTimeDelta);
	m_pSoundManager->Update();
}

HRESULT CGameInstance::Draw()
{
	if (nullptr == m_pLevel_Manager || 
		nullptr == m_pRenderer)
		return E_FAIL;

	m_pRenderer->Draw();	

	return m_pLevel_Manager->Render();
}
void CGameInstance::Clear(_uint iLevelIndex)
{
	m_pRenderer->Clear();
	m_pObject_Manager->Clear(iLevelIndex);	
	m_pComponent_Manager->Clear(iLevelIndex);
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);	
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Clear_RTVs(_float4 vClearColor)
{
	return m_pGraphic_Device->Clear_RTVs(vClearColor);
}

HRESULT CGameInstance::Bind_BackBuffer()
{
	return m_pGraphic_Device->Bind_BackBuffer();
}

HRESULT CGameInstance::Bind_RTVs()
{
	return m_pGraphic_Device->Bind_RTVs();
}

HRESULT CGameInstance::Present()
{
	return m_pGraphic_Device->Present();
}

void CGameInstance::Map_Picking_Screen()
{
	m_pGraphic_Device->Map_Picking_Screen();
}

void CGameInstance::Unmap_Picking_Screen()
{
	m_pGraphic_Device->Unmap_Picking_Screen();
}

_uint CGameInstance::Pick_Screen(_uint iX, _uint iY)
{
	return m_pGraphic_Device->Pick_Screen(iX,iY);
}


void CGameInstance::Map_Picking_Position()
{
	m_pGraphic_Device->Map_Picking_Position();
}

void CGameInstance::Unmap_Picking_Position()
{
	m_pGraphic_Device->Unmap_Picking_Position();
}

_float4 CGameInstance::Pick_Position(_uint iX, _uint iY)
{
	return m_pGraphic_Device->Pick_Position(iX, iY);
}

HRESULT CGameInstance::Add_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixel, const _float4& vClearColor)
{
	return m_pGraphic_Device->Add_RenderTarget(strRenderTargetTag, iWidth, iHeight, ePixel, vClearColor);
}

HRESULT CGameInstance::Add_MRT(const _wstring& strMRTTag, const _wstring& strRenderTargetTag)
{
	return m_pGraphic_Device->Add_MRT(strMRTTag, strRenderTargetTag);
}

HRESULT CGameInstance::Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV,  _bool bClear)
{
	return m_pGraphic_Device->Begin_MRT(strMRTTag, pDSV,  bClear);
}

HRESULT CGameInstance::End_MRT()
{
	return m_pGraphic_Device->End_MRT();
}

CRenderTarget* CGameInstance::Find_RenderTarget(const _wstring& strRenderTargetTag)
{
	return m_pGraphic_Device->Find_RenderTarget(strRenderTargetTag);
}

list<class CRenderTarget*>* CGameInstance::Find_MRT(const _wstring& strMRTTag)
{
	return  m_pGraphic_Device->Find_MRT(strMRTTag);
}

HRESULT CGameInstance::Bind_RT_SRV(const _wstring& strRTTag, CShader* pShader, const _char* pConstantName)
{
	return m_pGraphic_Device->Bind_RT_SRV(strRTTag, pShader, pConstantName);
}

HRESULT CGameInstance::Bind_Default_ViewPort()
{
	return m_pGraphic_Device->Bind_Default_ViewPort();
}

#ifdef _DEBUG
HRESULT CGameInstance::Ready_RTDebug(const _wstring& strRenderTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	return m_pGraphic_Device->Ready_Debug(strRenderTargetTag, fX, fY, fSizeX, fSizeY);
}
HRESULT CGameInstance::Render_RTDebug(const _wstring& strMRTTag, class CShader* pShader, const _char* pConstantName, class CVIBuffer_Rect* pVIBuffer)
{
	return m_pGraphic_Device->Render_Debug(strMRTTag, pShader, pConstantName, pVIBuffer);
}
#endif


PxScene* CGameInstance::Get_Scene() const
{
	return m_pPhysxManager->Get_Scene();
}

PxControllerManager* CGameInstance::Get_CCT_Manager() const
{
	return m_pPhysxManager->Get_CCT_Manager();
}

PxPhysics* CGameInstance::Get_Physics() const
{
	return m_pPhysxManager->Get_Physics();
}

PxFoundation* CGameInstance::Get_Foundation() const
{
	return m_pPhysxManager->Get_Foundation();
}

HRESULT CGameInstance::Add_Shape(const wstring& strTag, const vector<_float3>& vecPosition, const vector<_uint>& vecIndicies)
{
	return m_pPhysxManager->Add_Shape(strTag, vecPosition, vecIndicies);
}

PxShape* CGameInstance::Get_Shape(const wstring& strTag)
{
	return m_pPhysxManager->Get_Shape(strTag);
}

void CGameInstance::Simulate_Physx(_bool bSim)
{
	m_pPhysxManager->Simulate_Physx(bSim);
}

bool CGameInstance::PxSweep(PxTransform& StartTransform, PxVec3& vDirection, _float fDistance, _float fRadius, PxSweepBuffer& HitOut)
{
	return m_pPhysxManager->Sweep(StartTransform, vDirection, fDistance, fRadius, HitOut);
}

#pragma region GRAPHIC_DEVICE

#pragma endregion

#pragma region LEVEL_MANAGER
HRESULT CGameInstance::Change_Level(CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Change_Level(pNewLevel);
}
_uint CGameInstance::Get_Current_LevelID()
{
	return m_pLevel_Manager->Get_Current_LevelID();
}
#pragma endregion

#pragma region TIMER_MANAGER
_float CGameInstance::Get_TimeDelta(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.f;

	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}
HRESULT CGameInstance::Add_Timer(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(strTimerTag);
}
void CGameInstance::Compute_TimeDelta(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return ;

	return m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}
#pragma endregion

#pragma region OBJECT_MANAGER
HRESULT CGameInstance::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);
	
}
HRESULT CGameInstance::Add_Clone(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Clone(iLevelIndex, strLayerTag, strPrototypeTag, pArg);
}

CGameObject* CGameInstance::Clone_Object(const wstring& strPrototypeTag, void* pArg)
{
	return m_pObject_Manager->Clone_Object(strPrototypeTag, pArg);
}

CComponent * CGameInstance::Find_Component(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Find_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);
}

CGameObject* CGameInstance::Add_Clone_Return(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg)
{
	return m_pObject_Manager->Add_Clone_Return(iLevelIndex, strLayerTag, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Push_Object(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pObject)
{
	return m_pObject_Manager->Push_Object(iLevelIndex, strLayerTag, pObject);
}

CLayer* CGameInstance::Find_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	return m_pObject_Manager->Find_Layer(iLevelIndex, strLayerTag);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring & strPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);	
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
}

#pragma endregion

#pragma region RENDERER
HRESULT CGameInstance::Add_RenderObjects(CRenderer::RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderObjects(eRenderGroup, pRenderObject);	
}

HRESULT CGameInstance::Render_Priority()
{
	return m_pRenderer->Render_Priority();
}

HRESULT CGameInstance::Render_NonBlend()
{
	return m_pRenderer->Render_NonBlend();
}

HRESULT CGameInstance::Render_Blend()
{
	return m_pRenderer->Render_Blend();
}

HRESULT CGameInstance::Render_UI()
{
	return m_pRenderer->Render_UI();
}

_float4x4* CGameInstance::Get_ScreenQuadWorld()
{
	return m_pRenderer->Get_ScreenQuadWorld();
}

_float4x4* CGameInstance::Get_ScreenQuadProjection()
{
	return m_pRenderer->Get_ScreenQuadProjection();
}

void CGameInstance::Blur_On(const _float2& vCenter, const _float& fStrength)
{
	m_pRenderer->Blur_On(vCenter, fStrength);
}

void CGameInstance::Dissolve_On(CTexture* pNoiseTexture, const _float& fDissolve, const _float& fRange)
{
	m_pRenderer->Dissolve_On(pNoiseTexture, fDissolve, fRange);
}



void CGameInstance::Add_Delayed(DELAYED tEvent)
{
	m_pDelayedProcessManager->Add_Delayed(tEvent);
}

HRESULT CGameInstance::Pool_Object(const wstring& strPrototypeTag, void* pArg)
{
	return m_pObject_Pool->Pool_Object(strPrototypeTag, pArg);
}

CGameObject* CGameInstance::Push_Object_From_Pool(const wstring& strPrototypeTag, _uint iLevelIndex, const wstring& strLayerTag, void* pArg, _float4x4* TransformMat)
{
	return m_pObject_Pool->Push_Object_From_Pool(strPrototypeTag, iLevelIndex, strLayerTag, pArg, TransformMat);
}

CGameObject* CGameInstance::Push_Object_Hijack(const wstring& strPrototypeTag, void* pArg, _float4x4* TransformMat)
{
	return m_pObject_Pool->Push_Object_Hijack(strPrototypeTag, pArg, TransformMat);
}

void CGameInstance::Clear_Pool()
{
	m_pObject_Pool->Clear_Pool();
}

void CGameInstance::Recall_Object_To_Pool()
{
	m_pObject_Pool->Recall_Object();
}

void CGameInstance::Recall_Object_To_Pool(const wstring& strPrototypeTag)
{
	m_pObject_Pool->Recall_Object(strPrototypeTag);
}

void CGameInstance::Clear_Pool(_wstring strTag)
{
	m_pObject_Pool->Clear_Pool(strTag);
}

void CGameInstance::Set_Transform(CPipeLine::PIPELINE_STATE eState, _fmatrix Matrix)
{
	m_pPipeLine->Set_Transform(eState, Matrix);
}

void CGameInstance::Set_Transform(CPipeLine::PIPELINE_STATE eState, const _float4x4& Matrix)
{
	m_pPipeLine->Set_Transform(eState, Matrix);
}

_fmatrix CGameInstance::Get_Transform_Matrix(CPipeLine::PIPELINE_STATE eState) const
{
	return m_pPipeLine->Get_Transform_Matrix(eState);
}

_float4x4 CGameInstance::Get_Transform_Float4x4(CPipeLine::PIPELINE_STATE eState) const
{
	return m_pPipeLine->Get_Transform_Float4x4(eState);
}

_float4 CGameInstance::Get_CamPosition_Float4() const
{
	return m_pPipeLine->Get_CamPosition_Float4();
}

_fvector CGameInstance::Get_CamPosition_Vector() const
{
	return m_pPipeLine->Get_CamPosition_Vector();
}

void CGameInstance::Add_Camera(const _wstring& strTag, CCamera* pCamera)
{
	m_pPipeLine->Add_Camera(strTag, pCamera);
}

CCamera* CGameInstance::Get_Camera(const _wstring& strTag)
{
	return m_pPipeLine->Get_Camera(strTag);
}

CCamera* CGameInstance::Get_MainCamera()
{
	return m_pPipeLine->Get_MainCamera();
}

void CGameInstance::Set_MainCamera(const _wstring& strTag)
{
	m_pPipeLine->Set_MainCamera(strTag);
}

void CGameInstance::Remove_Camera(const _wstring& strTag)
{
	m_pPipeLine->Remove_Camera(strTag);
}

void CGameInstance::Clear_Camera()
{
	m_pPipeLine->Clear_Camera();
}

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID) const
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

bool CGameInstance::Get_KeyDown(_ubyte byKeyID) const
{
	return m_pInput_Device->Get_KeyDown(byKeyID);
}

bool CGameInstance::Get_KeyPressing(_ubyte byKeyID) const
{
	return m_pInput_Device->Get_KeyPressing(byKeyID);
}

bool CGameInstance::Get_KeyUp(_ubyte byKeyID) const
{
	return m_pInput_Device->Get_KeyUp(byKeyID);
}


bool CGameInstance::Get_KeyNone(_ubyte byKeyID) const
{
	return m_pInput_Device->Get_KeyNone(byKeyID);
}


bool CGameInstance::Get_MouseDown(MOUSEKEYSTATE byKeyID) const
{
	return m_pInput_Device->Get_MouseDown(byKeyID);
}


bool CGameInstance::Get_MousePressing(MOUSEKEYSTATE byKeyID) const
{
	return m_pInput_Device->Get_MousePressing(byKeyID);
}


bool CGameInstance::Get_MouseUp(MOUSEKEYSTATE byKeyID) const
{
	return m_pInput_Device->Get_MouseUp(byKeyID);
}

bool CGameInstance::Get_MouseNone(MOUSEKEYSTATE byKeyID) const
{
	return m_pInput_Device->Get_MouseNone(byKeyID);
}

_byte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse) const
{
	return m_pInput_Device->Get_DIMouseState(eMouse);
}

_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseState);
}

void CGameInstance::RegisterToBGM(CSound* _pSound)
{
	m_pSoundManager->RegisterToBGM(_pSound);
}

void CGameInstance::Set_BGM_Tag(const wstring& strTag)
{
	m_pSoundManager->Set_BGM_Tag(strTag);
}

wstring CGameInstance::Get_BGM_Tag() const
{
	return m_pSoundManager->Get_BGM_Tag();
}

void CGameInstance::Stop_BGM()
{
	m_pSoundManager->Stop_BGM();
}

void CGameInstance::Start_BGM()
{
	m_pSoundManager->Start_BGM();
}

bool CGameInstance::IsBGMExist()
{
	return m_pSoundManager->IsBGMExist();
}

bool CGameInstance::IsBGMPlaying()
{
	return m_pSoundManager->IsBGMPlaying();
}

HRESULT CGameInstance::Add_Sound(SOUND_CHANNEL eChannel, const wstring& strSoundTag, const wstring& strSoundPath)
{
	return m_pSoundManager->Add_Sound(eChannel, strSoundTag,strSoundPath);
}

HRESULT CGameInstance::Add_Sound_3D(SOUND_CHANNEL eChannel, const wstring& strSoundTag, const wstring& strSoundPath)
{
	return m_pSoundManager->Add_Sound_3D(eChannel, strSoundTag, strSoundPath);
}

void CGameInstance::Set_Master_Volume(float fVolume)
{
	m_pSoundManager->Set_Master_Volume(fVolume);
}

void CGameInstance::Set_Channel_Volume(SOUND_CHANNEL eChannel, float fVolume)
{
	m_pSoundManager->Set_Channel_Volume(eChannel, fVolume);
}

void CGameInstance::Play_Sound(SOUND_CHANNEL eChannel, const wstring& strSoundTag, bool bLoop)
{
	m_pSoundManager->Play_Sound(eChannel, strSoundTag, bLoop);
}

void CGameInstance::Play_Sound_3D(SOUND_CHANNEL eChannel, const wstring& strSoundTag, _fvector vPos, _fvector vVel, bool bLoop)
{
	m_pSoundManager->Play_Sound_3D(eChannel, strSoundTag, vPos, vVel, bLoop);
}

void CGameInstance::Pause_Sound(SOUND_CHANNEL eChannel, const wstring& strSoundTag, bool bPausing)
{
	m_pSoundManager->Pause_Sound(eChannel, strSoundTag, bPausing);
}

void CGameInstance::Pause_Sound_3D(SOUND_CHANNEL eChannel, const wstring& strSoundTag, bool bPausing)
{
	m_pSoundManager->Pause_Sound_3D(eChannel, strSoundTag, bPausing);
}

void CGameInstance::Stop_Sound(SOUND_CHANNEL eChannel, const wstring& strSoundTag)
{
	m_pSoundManager->Stop_Sound(eChannel, strSoundTag);
}

void CGameInstance::Stop_Sound_3D(SOUND_CHANNEL eChannel, const wstring& strSoundTag)
{
	m_pSoundManager->Stop_Sound_3D(eChannel, strSoundTag);
}

void CGameInstance::Set_Sound_Volume(SOUND_CHANNEL eChannel, const wstring& strSoundTag, float fVolume)
{
	m_pSoundManager->Set_Sound_Volume(eChannel, strSoundTag, fVolume);
}

void CGameInstance::Set_Master_Speed(float fSpeed)
{
	m_pSoundManager->Set_Master_Speed(fSpeed);
}

void CGameInstance::Set_Channel_Speed(SOUND_CHANNEL eChannel, float fSpeed)
{
	m_pSoundManager->Set_Channel_Speed(eChannel, fSpeed);
}

void CGameInstance::Set_Sound_Speed(SOUND_CHANNEL eChannel, const wstring& strSoundTag, float fSpeed)
{
	m_pSoundManager->Set_Sound_Speed(eChannel, strSoundTag, fSpeed);
}

void CGameInstance::Set_BGM_Position(_float fPosition)
{
	m_pSoundManager->Set_BGM_Position(fPosition);
}

CSound* CGameInstance::Find_Sound(SOUND_CHANNEL eChannel, const wstring& strSoundTag)
{
	return m_pSoundManager->Find_Sound(eChannel, strSoundTag);
}

C3D_Sound* CGameInstance::Find_Sound_3D(SOUND_CHANNEL eChannel, const wstring& strSoundTag)
{
	return m_pSoundManager->Find_Sound_3D(eChannel,strSoundTag);
}

void CGameInstance::Set_Listener(CGameObject* pListener)
{
	m_pSoundManager->Set_Listener(pListener);
}

HRESULT CGameInstance::Add_Font(const _wstring& strTag, const _wstring& strFile)
{
	return m_pFontManager->Add_Font(strTag, strFile);
}

HRESULT CGameInstance::Render_Font(const _wstring& strTag, const _wstring& strText, const _float2& vPos, _fvector vColor, _float fRotation, const _float2& vOrigin, _float fScale)
{
	return m_pFontManager->Render(strTag, strText, vPos, vColor, fRotation, vOrigin, fScale);
}

CPointLight* CGameInstance::Find_PointLight(const _wstring& strTag)
{
	return m_pLightManager->Find_PointLight(strTag);
}

CSpotLight* CGameInstance::Find_SpotLight(const _wstring& strTag)
{
	return m_pLightManager->Find_SpotLight(strTag);
}

CDirectionalLight* CGameInstance::Find_DirectionalLight(const _wstring& strTag)
{
	return m_pLightManager->Find_DirectionalLight(strTag);
}

HRESULT CGameInstance::Remove_PointLight(const _wstring& strTag)
{
	return m_pLightManager->Remove_PointLight(strTag);
}

HRESULT CGameInstance::Remove_SpotLight(const _wstring& strTag)
{
	return m_pLightManager->Remove_SpotLight(strTag);
}

HRESULT CGameInstance::Remove_DirectionalLight(const _wstring& strTag)
{
	return m_pLightManager->Remove_DirectionalLight(strTag);
}

HRESULT CGameInstance::Add_Light(const _wstring& strTag, const PointLight& LightDesc)
{
	return m_pLightManager->Add_Light(strTag, LightDesc);
}

HRESULT CGameInstance::Add_Light(const _wstring& strTag, const DirectionalLight& LightDesc)
{
	return m_pLightManager->Add_Light(strTag, LightDesc);
}

HRESULT CGameInstance::Add_Light(const _wstring& strTag, const SpotLight& LightDesc)
{
	return m_pLightManager->Add_Light(strTag, LightDesc);
}

HRESULT CGameInstance::Bind_Lights(CShader* pShader)
{
	return m_pLightManager->Bind_Lights(pShader);
}

void CGameInstance::Set_AmbientColor(const _float4& vColor)
{
	m_pLightManager->Set_AmbientColor(vColor);
}

void CGameInstance::Transform_Frustum_ToLocalSpace(_fmatrix WorldMatrix)
{
	m_pFrustum->Transform_ToLocalSpace(WorldMatrix);
}

_bool CGameInstance::isIn_Frustum_WorldSpace(_fvector vWorldPos, _float fRange)
{
	return m_pFrustum->isIn_WorldSpace(vWorldPos, fRange);
}

_bool CGameInstance::isIn_Frustum_LocalSpace(_fvector vLocalPos, _float fRange)
{
	return m_pFrustum->isIn_LocalSpace(vLocalPos, fRange);
}



void CGameInstance::Release_Engine()
{
	CGameInstance::Get_Instance()->Free();

	CGameInstance::Get_Instance()->Destroy_Instance();	
}

void CGameInstance::Free()
{	
	Safe_Release(m_pRenderer);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pObject_Pool);
	Safe_Release(m_pPhysxManager);
	Safe_Release(m_pDelayedProcessManager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pSoundManager);
	Safe_Release(m_pFontManager);
	Safe_Release(m_pFrustum);

	Safe_Release(m_pLightManager);


	//�� �ؿ� �α� ����
	Safe_Release(m_pGraphic_Device);
}