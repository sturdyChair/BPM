#include "AfterImage.h"
#include "GameInstance.h"
#include "Beat_Manager.h"
CAfterImage::CAfterImage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CAfterImage::CAfterImage(const CAfterImage& rhs) :
	CGameObject(rhs), m_tDesc(rhs.m_tDesc), m_iRenderPass(rhs.m_iRenderPass)
{
}

void CAfterImage::PriorityTick(_float fTimeDelta)
{
	m_bActive = false;
}

void CAfterImage::Tick(_float fTimeDelta)
{
	for (auto& fLifeTime: m_fLifeTime)
	{
		if (fLifeTime > 0.f)
		{
			fLifeTime -= fTimeDelta;
			m_bActive = true;
		}
	}
}

void CAfterImage::LateTick(_float fTimeDelta)
{
	if(m_bActive)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);
}

HRESULT CAfterImage::Render()
{
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));


    m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));

   for (int iMeshIdx = 0; iMeshIdx < m_pModelCom->Get_NumMesh(); ++iMeshIdx)
   {
       for (_uint iImageIdx = 0; iImageIdx < m_tDesc.iMaxImage; ++iImageIdx)
       {
           if (m_fLifeTime[iImageIdx] < 0.001f)
               continue;
		   _float4 vColor = m_tDesc.vColor;
		   vColor.w = m_fLifeTime[iImageIdx] / m_tDesc.fLifeTime;
		   m_pShaderCom->Bind_Vector4("g_vColor", &vColor);
		   m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_Transforms[iImageIdx]);
           if (m_pModelCom->Get_Material(iMeshIdx).size())
           {
               auto& vecMat = m_pModelCom->Get_Material(iMeshIdx);
               if (vecMat.size())
               {
                   auto pTexture = vecMat[0];
                   if (pTexture)
                       pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
                   pTexture = vecMat[5];
                   if (pTexture)
                   {
                       pTexture->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
                   }
                   else
                   {
                       DEFAULTNORMAL->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
                   }
                   pTexture = vecMat[14];
                   if (pTexture)
                   {
                       pTexture->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
                   }
                   else
                   {
                       DEFAULTORM->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
                   }
               }
           }
		   if(m_tDesc.bAnimated)
				m_pShaderCom->Bind_Matrices("g_BoneMatrix", m_BoneMatrices[iImageIdx][iMeshIdx].data(), m_BoneMatrices[iImageIdx][iMeshIdx].size());
           m_pShaderCom->Begin(m_iRenderPass);
           m_pModelCom->Render(iMeshIdx);
       }
   }
    return S_OK;
}

void CAfterImage::SetUp_Matrices(const _float4x4& Transform)
{
	m_Transforms[m_iNumImages] = Transform;
	if (m_tDesc.bAnimated && m_pModelCom)
	{
		
		for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
		{
			m_BoneMatrices[m_iNumImages][i].clear();
			m_pModelCom->Get_BoneMatrices(m_BoneMatrices[m_iNumImages][i], i);
		}
	}
	m_fLifeTime[m_iNumImages] = m_tDesc.fLifeTime;

	++m_iNumImages;
	m_iNumImages %= m_tDesc.iMaxImage;
}

void CAfterImage::Push_From_Pool(void* pArg)
{
}

void CAfterImage::Dying()
{
}

HRESULT CAfterImage::Initialize_Prototype(const AFTERIMAGE_DESC& desc)
{
	m_tDesc = desc;
	m_iRenderPass = m_tDesc.iRenderPass;
	return S_OK;
}

HRESULT CAfterImage::Initialize(void* Arg)
{
	__super::Initialize(nullptr);

	m_pModelCom = (CModel*)Arg;
	Safe_AddRef(m_pModelCom);
	//Add_Component(LEVEL_STATIC, m_tDesc.strModelTag, L"Com_Model", (CComponent**)&m_pModel);
	if(m_tDesc.bAnimated)
	{
		Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
		m_BoneMatrices.resize(m_tDesc.iMaxImage);
		_uint iNumMesh = m_pModelCom->Get_NumMesh();
		for (auto& vecImages : m_BoneMatrices)
		{
			vecImages.resize(iNumMesh);
		}
	}
	else
	{
		Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	}
	m_Transforms.resize(m_tDesc.iMaxImage);
	m_fLifeTime.resize(m_tDesc.iMaxImage);

	return S_OK;
}

CAfterImage* CAfterImage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const AFTERIMAGE_DESC& desc)
{
	CAfterImage* pInstance = new CAfterImage(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype(desc)))
	{
		Safe_Release(pInstance);
	}


	return pInstance;
}

CAfterImage* CAfterImage::Clone(void* pArg)
{
	CAfterImage* pInstance = new CAfterImage(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAfterImage::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
