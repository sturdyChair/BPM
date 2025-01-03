#pragma once

#include "Renderer.h"
#include "Component_Manager.h"
#include "PipeLine.h"
/* 클라이언트와 엔진의 중계자의 역할을 수행한다. */
/* 클라이언트에서 엔진의 기능을 쓰고하자 한다라면 무조건 게임인스턴스를 떠올리면 되도록. */

BEGIN(Engine)
class CSound;
class C3D_Sound;


class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(_uint iNumLevels, const ENGINE_DESC& EngineDesc, _Out_ ID3D11Device** ppDevice, _Out_ ID3D11DeviceContext** ppContext);
	void Tick_Engine(_float fTimeDelta);
	HRESULT Draw();
	void Clear(_uint iLevelIndex);
	class CPhysxManager* Get_PhysxManager() { return m_pPhysxManager; }
	_float2 Get_Resolution() const { return _float2{(_float)m_iWinSizeX, (_float)m_iWinSizeY}; }
	HWND	Get_hWnd() const { return m_hWnd; }

#pragma region GRAPHIC_DEVICE
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();	
	HRESULT Clear_RTVs(_float4 vClearColor);
	HRESULT Bind_BackBuffer();
	HRESULT Bind_RTVs();
	HRESULT Present();
	void Map_Picking_Screen();
	void Unmap_Picking_Screen();
	_uint Pick_Screen(_uint iX, _uint iY);

	void  Map_Picking_Position();
	void  Unmap_Picking_Position();
	_float4 Pick_Position(_uint iX, _uint iY);

	HRESULT Add_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixel, const _float4& vClearColor);
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strRenderTargetTag);

	HRESULT Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV = nullptr, _bool bClear = true);
	HRESULT End_MRT();
	class CRenderTarget* Find_RenderTarget(const _wstring& strRenderTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _wstring& strMRTTag);
	HRESULT Bind_RT_SRV(const _wstring& strRTTag, class CShader* pShader, const _char* pConstantName);

	HRESULT Bind_Default_ViewPort();
#ifdef _DEBUG
	HRESULT Ready_RTDebug(const _wstring& strRenderTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_RTDebug(const _wstring& strMRTTag, class CShader* pShader, const _char* pConstantName, class CVIBuffer_Rect* pVIBuffer);
#endif

#pragma endregion

public: //Physx
	PxScene* Get_Scene() const;
	PxControllerManager* Get_CCT_Manager() const;
	PxPhysics* Get_Physics() const;
	PxFoundation* Get_Foundation() const;
	HRESULT Add_Shape(const wstring& strTag, const vector<_float3>& vecPosition, const vector<_uint>& vecIndicies);
	PxShape* Get_Shape(const wstring& strTag);
	void Simulate_Physx(_bool bSim = true);
	bool PxSweep(PxTransform& StartTransform, PxVec3& vDirection, _float fDistance, _float fRadius, PxSweepBuffer& HitOut);
#pragma region LEVEL_MANAGER
public: /* For.Level_Manager */
	HRESULT Change_Level(class CLevel* pNewLevel);
	_uint	Get_Current_LevelID();
#pragma endregion

#pragma region TIMER_MANAGER
public:
	_float	Get_TimeDelta(const wstring& strTimerTag);
	HRESULT Add_Timer(const wstring& strTimerTag);
	void Compute_TimeDelta(const wstring& strTimerTag);
#pragma endregion

#pragma region OBJECT_MANAGER
public:
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_Clone(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);
	CGameObject* Clone_Object(const wstring& strPrototypeTag, void* pArg);
	class CComponent* Find_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComponentTag, _uint iIndex = 0);
	CGameObject* Add_Clone_Return(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);
	HRESULT Push_Object(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pObject);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);

#pragma endregion

#pragma region COMPONENT_MANAGER
public:
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion


#pragma region RENDERER
public:
	HRESULT Add_RenderObjects(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();
	_float4x4* Get_ScreenQuadWorld();
	_float4x4* Get_ScreenQuadProjection();
	void Blur_On(const _float2& vCenter, const _float& fStrength);
	void Dissolve_On(class CTexture* pNoiseTexture, const _float& fDissolve, const _float& fRange);
#pragma endregion

public: //DelayedProcessManager
	void   Add_Delayed(DELAYED tEvent);

public: //Object Pool
	HRESULT Pool_Object(const wstring& strPrototypeTag, void* pArg = nullptr);
	class CGameObject* Push_Object_From_Pool(const wstring& strPrototypeTag, _uint iLevelIndex, const wstring& strLayerTag, void* pArg = nullptr, _float4x4* TransformMat = nullptr);
	class CGameObject* Push_Object_Hijack(const wstring& strPrototypeTag, void* pArg = nullptr, _float4x4* TransformMat = nullptr);
	void	Clear_Pool();
	void	Recall_Object_To_Pool();
	void	Recall_Object_To_Pool(const wstring& strPrototypeTag);
	void	Clear_Pool(_wstring strTag);
public: //PipeLine

	void    Set_Transform(CPipeLine::PIPELINE_STATE eState, _fmatrix Matrix);
	void    Set_Transform(CPipeLine::PIPELINE_STATE eState, const _float4x4& Matrix);

	_fmatrix  Get_Transform_Matrix(CPipeLine::PIPELINE_STATE eState) const;
	_float4x4 Get_Transform_Float4x4(CPipeLine::PIPELINE_STATE eState) const;

	_float4 Get_CamPosition_Float4() const;
	_fvector Get_CamPosition_Vector() const;

	void     Add_Camera(const _wstring& strTag, class CCamera* pCamera);
	CCamera* Get_Camera(const _wstring& strTag);
	CCamera* Get_MainCamera();
	void     Set_MainCamera(const _wstring& strTag);
	void     Remove_Camera(const _wstring& strTag);
	void     Clear_Camera();

public: // Input Device
	_byte	Get_DIKeyState(_ubyte byKeyID)	const;

	bool	Get_KeyDown(_ubyte byKeyID) const;
	bool	Get_KeyPressing(_ubyte byKeyID) const;
	bool	Get_KeyUp(_ubyte byKeyID) const;
	bool	Get_KeyNone(_ubyte byKeyID) const;
	bool	Get_MouseDown(MOUSEKEYSTATE byKeyID) const;
	bool	Get_MousePressing(MOUSEKEYSTATE byKeyID) const;
	bool	Get_MouseUp(MOUSEKEYSTATE byKeyID) const;
	bool	Get_MouseNone(MOUSEKEYSTATE byKeyID) const;
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) const;
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);

public: // SoundManager
	void RegisterToBGM(CSound* _pSound);
	void Set_BGM_Tag(const wstring& strTag);
	wstring Get_BGM_Tag() const;
	void Stop_BGM();
	void Start_BGM();
	bool IsBGMExist();
	bool IsBGMPlaying();

	HRESULT Add_Sound(SOUND_CHANNEL eChannel, const wstring& strSoundTag, const wstring& strSoundPath);
	HRESULT Add_Sound_3D(SOUND_CHANNEL eChannel, const wstring& strSoundTag, const wstring& strSoundPath);

	void Set_Master_Volume(float fVolume);
	void Set_Channel_Volume(SOUND_CHANNEL eChannel, float fVolume);

	void Play_Sound(SOUND_CHANNEL eChannel, const wstring& strSoundTag, bool bLoop = false);
	void Play_Sound_3D(SOUND_CHANNEL eChannel, const wstring& strSoundTag, _fvector vPos, _fvector vVel = XMVectorSet(0.f, 0.f, 0.f, 0.f), bool bLoop = false);

	void Pause_Sound(SOUND_CHANNEL eChannel, const wstring& strSoundTag, bool bPausing = true);
	void Pause_Sound_3D(SOUND_CHANNEL eChannel, const wstring& strSoundTag, bool bPausing = true);

	void Stop_Sound(SOUND_CHANNEL eChannel, const wstring& strSoundTag);
	void Stop_Sound_3D(SOUND_CHANNEL eChannel, const wstring& strSoundTag);

	void Set_Sound_Volume(SOUND_CHANNEL eChannel, const wstring& strSoundTag, float fVolume);

	void Set_Master_Speed(float fSpeed);
	void Set_Channel_Speed(SOUND_CHANNEL eChannel, float fSpeed);
	void Set_Sound_Speed(SOUND_CHANNEL eChannel, const wstring& strSoundTag, float fSpeed);

	void Set_BGM_Position(_float fPosition);

	CSound* Find_Sound(SOUND_CHANNEL eChannel, const wstring& strSoundTag);
	C3D_Sound* Find_Sound_3D(SOUND_CHANNEL eChannel, const wstring& strSoundTag);

	void Set_Listener(class CGameObject* pListener);


public://Font_Manager
	HRESULT Add_Font(const _wstring& strTag, const _wstring& strFile);
	HRESULT Render_Font(const _wstring& strTag, const _wstring& strText,
		const _float2& vPos, _fvector vColor = Colors::White,
		_float fRotation = 0.f, const _float2& vOrigin = _float2{ 0.f,0.f }, _float fScale = 1.f);

public://Light_Manager
	class CPointLight* Find_PointLight(const _wstring& strTag);
	class CSpotLight* Find_SpotLight(const _wstring& strTag);
	class CDirectionalLight* Find_DirectionalLight(const _wstring& strTag);

	HRESULT Remove_PointLight(const _wstring& strTag);
	HRESULT Remove_SpotLight(const _wstring& strTag);
	HRESULT Remove_DirectionalLight(const _wstring& strTag);
	HRESULT Add_Light(const _wstring& strTag, const PointLight& LightDesc);
	HRESULT Add_Light(const _wstring& strTag, const DirectionalLight& LightDesc);
	HRESULT Add_Light(const _wstring& strTag, const SpotLight& LightDesc);
	HRESULT Bind_Lights(class CShader* pShader);
	void	Set_AmbientColor(const _float4& vColor);

public://Frustum
	void Transform_Frustum_ToLocalSpace(_fmatrix WorldMatrix);
	_bool isIn_Frustum_WorldSpace(_fvector vWorldPos, _float fRange);
	_bool isIn_Frustum_LocalSpace(_fvector vLocalPos, _float fRange);

private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CRenderer*				m_pRenderer = { nullptr };
	class CPhysxManager*			m_pPhysxManager = { nullptr };
	class CDelayedProcessManager*	m_pDelayedProcessManager = nullptr;
	class CObject_Pool*				m_pObject_Pool = nullptr;
	class CPipeLine*				m_pPipeLine = nullptr;
	class CInput_Device*			m_pInput_Device = nullptr;
	class CSoundManager*			m_pSoundManager = nullptr;
	class CFont_Manager*			m_pFontManager = nullptr;
	class CLight_Manager*			m_pLightManager = nullptr;
	class CFrustum*					m_pFrustum = nullptr;

	_uint	m_iWinSizeX = 0;
	_uint	m_iWinSizeY = 0;
	HWND	m_hWnd = NULL;
	HINSTANCE m_hInstance = NULL;

public:
	static void Release_Engine();
	virtual void Free() override;
	
};

END