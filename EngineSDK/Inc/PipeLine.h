#pragma once
#include "Base.h"

BEGIN(Engine)

class CPipeLine :
    public CBase
{
public:
    enum PIPELINE_STATE
    {
        TS_VIEW,
        TS_PROJ,
        TS_VIEW_INV,
        TS_PROJ_INV,
        TS_END,
    };

private:
    CPipeLine();
    virtual ~CPipeLine() = default;
public:
    void    Update();

    void    Set_Transform(PIPELINE_STATE eState, _fmatrix Matrix);
    void    Set_Transform(PIPELINE_STATE eState, const _float4x4& Matrix);

    _fmatrix  Get_Transform_Matrix(PIPELINE_STATE eState) const;
    _float4x4 Get_Transform_Float4x4(PIPELINE_STATE eState) const;

    _float4 Get_CamPosition_Float4() const;
    _fvector Get_CamPosition_Vector() const;

    void    Add_Camera(const _wstring& strTag, class CCamera* pCamera);
    CCamera* Get_Camera(const _wstring& strTag);
    CCamera* Get_MainCamera();
    void    Set_MainCamera(const _wstring& strTag);
    void    Remove_Camera(const _wstring& strTag);
    void    Clear_Camera();

private:
    _float4x4 m_TransformationMatrices[TS_END] = {};
    _float4   m_vCamPosition = {};

    class CCamera* m_pMainCamera = nullptr;
    _wstring m_strMainCamTag;
    map<_wstring, class CCamera*> m_Cameras;

public:
    static CPipeLine* Create();
    virtual void Free() override;
};

END