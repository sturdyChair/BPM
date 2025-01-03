#pragma once
#include "GameObject.h"

#ifdef ENGINE_EXPORTS
#define EXPIMP_TEMPLATE 
#else
#define EXPIMP_TEMPLATE extern
#endif

BEGIN(Engine)

class ENGINE_DLL CUIBase :
    public CGameObject
{
public:
    typedef struct tagUIDesc
    {
        _uint           iCurrLevel = { 0 };
        CUIBase*        pParentUI = { nullptr };
        CGameObject*    pValueRef = { nullptr };
        _float3         vPosition = { 0.f, 0.f, 0.f };
        _float3         vSize = { 1.f, 1.f, 1.f };
        wstring         strTextureTag = L"Prototype_Component_Texture_Temp";

    }UI_DESC;

    enum class ALIGN_DIR { LEFT, RIGHT, UP, DOWN, ALIGN_END };

    enum class UI_TYPE { UI_2D, UI_3D, TYPE_END };

protected:
    CUIBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_TYPE eType = UI_TYPE::UI_2D);
    CUIBase(const CUIBase& rhs);
    virtual ~CUIBase() = default;

public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta) override;
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    virtual HRESULT Render(const _float4x4& WorldMatrix, const _float4x4& ViewMatrix, const _float4x4& ProjMatrix);

    HRESULT Add_ChildUI(_uint iLevelIndex, const wstring& strPrototypeTag, CUIBase** ppOut, void* pArg = nullptr);

    void    Apply_Matrix_To_Children();
    void    Align_To_Base(_float fBase, ALIGN_DIR eDir);

    void    Add_To_Renderer();

    void    Set_Render_Pass(_uint iPass)
    {
        m_iRenderPass = iPass;
    }

    class CShader* Get_Shader() {
        return m_pShaderCom;
    }
    

protected:
    _float4x4 Set_BillBoardMatrix(_float4x4 WorldMatrix);

    virtual void Bind_Shader_Constant();

private:
    _float4x4    Multiply_Parent_Except_Scale() const;

public:
    _float4x4 Get_Parent_Matrix() const;
    _float3 Get_Size() const;
    _float3 Get_GlobalPos() const;

    void Set_Size(const _float3& vSize);
    void Set_Scale(_float fScale) { m_fScale = fScale; }
    void Set_Hidden(_bool bIsHidden = true, _bool bChild = true);
    void Set_Color(const _float4& vColor) { m_vColor = vColor; }

#pragma region MouseEvent

public:
    void MouseOnCheck(_float3 vMousePos);
    void SetMouseOn(bool _bOn = true) { m_bMouseOn = _bOn; }
    void SetLButtonDown(bool _bDown = true) { m_bLButtonDown = _bDown; }
    void SetRButtonDown(bool _bDown = true) { m_bRButtonDown = _bDown; }

    bool IsMouseOn() const { return m_bMouseOn; }
    bool IsLButtonDown() const { return m_bLButtonDown; }
    bool IsRButtonDown() const { return m_bRButtonDown; }

    virtual void MouseOn() {};
    virtual void MouseLButtonDown() {};
    virtual void MouseLButtonUp() {};
    virtual void MouseLButtonClicked() {};
    virtual void MouseRButtonDown() {};
    virtual void MouseRButtonUp() {};
    virtual void MouseRButtonClicked() {};

#pragma endregion //MouseEvent

public:
    static const wstring UILayerTag;
    static const wstring UITextBoxTag;

protected:
    UI_TYPE         m_eType = { UI_TYPE::UI_2D };

    class CVIBuffer* m_pVIBuffer = { nullptr };
    class CShader* m_pShaderCom = nullptr;

    _float4x4       m_ParentMatrix = {};

    _float4x4       m_ViewMatrix = {};
    _float4x4       m_ProjMatrix = {};

    _float          m_fWinSizeX = { 0 };
    _float          m_fWinSizeY = { 0 };
    _float4         m_vColor;
    

    CUIBase* m_pParentUI = { nullptr };
    list<CUIBase*>  m_Children;

    _float3     m_vOffset = { 0.f, 0.f, 0.f };
    _float3     m_vSize = { 1.f, 1.f, 1.f };
    _float      m_fScale = 1.f;

    _bool m_bMouseOn = false;
    _bool m_bLButtonDown = false;
    _bool m_bRButtonDown = false;

    _bool m_bIsHidden = false;

    _uint m_iRenderPass = 0;

public:
    virtual void Free() override;

    friend class CUI_Manager;


};

END
EXPIMP_TEMPLATE template class ENGINE_DLL std::list<CUIBase*>;