#include "Monster_Worm.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "CharacterController.h"
#include "Beat_Manager.h"
#include "Player_Manager.h"
#include "Player.h"

CMonster_Worm::CMonster_Worm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CMonster_Base(pDevice, pContext)
{
}

CMonster_Worm::CMonster_Worm(const CMonster_Worm& rhs) :
    CMonster_Base(rhs)
{
}

HRESULT CMonster_Worm::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CMonster_Worm::Initialize(void* pArg)
{
    __super::Initialize(pArg);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Worm", L"Model", (CComponent**)&m_pModelCom);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);

    CCharacterController::CCT_DESC cctDesc;
    if (m_pGameInstance->Get_Current_LevelID() != LEVEL_TOOL)
    {
        cctDesc.pTransform = m_pTransformCom;
    }
    else
    {
        cctDesc.pTransform = nullptr;
    }
    cctDesc.height = 1.0f;
    cctDesc.radius = 0.5f;
    cctDesc.userData = this;
    _float3 vPos;
    XMStoreFloat3(&vPos, m_pTransformCom->Get_Position());
    cctDesc.position = PxExtendedVec3(vPos.x, vPos.y, vPos.z);
    cctDesc.material = m_pGameInstance->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_CharacterController", L"Com_CCT", (CComponent**)&m_pCCTCom, &cctDesc);
   
    m_pCCTCom->m_bGravity = true;
    m_pCCTCom->m_bUseVelocity_Calculation = true;
    m_pCCTCom->m_fFriction = 5.f;

    m_fHp = 125.f;

    m_pModelCom->Set_AnimIndex(1, true);
    return S_OK;
}

void CMonster_Worm::PriorityTick(_float fTimeDelta)
{
    if (m_fInvincible > 0.001f)
    {
        m_fInvincible -= fTimeDelta;
    }
}

void CMonster_Worm::Tick(_float fTimeDelta)
{
    auto pPlayer = m_pPlayerManager->Get_Player();
    if (!m_fHp <= 0.f && pPlayer)
        m_pTransformCom->LookAt_Horizontal(pPlayer->Get_Transform()->Get_Position());
    m_pModelCom->Play_Animation(fTimeDelta);
    if (m_pModelCom->Get_Finished())
    {
        if (m_fHp <= 0.f)
        {
            m_bKilled = true;
            Drop_Coin();
            Dead();
        }
        else
        {
            m_pModelCom->Set_AnimTag(m_strIdleAnimTag, true);
            m_bAttacking = false;
        }
    }

    if (pPlayer)
    {
        if (m_bAttacking)
        {
            _vector vAttackPos = (m_pModelCom->Get_BoneMatrix(m_strAttackBone) * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix())).r[3];
            if (1.2f > XMVectorGetX(XMVector3Length(vAttackPos - pPlayer->Get_Transform()->Get_Position())))
            {
                _float3 vPos;
                XMStoreFloat3(&vPos, vAttackPos);
                pPlayer->Take_Damage(25.f, vPos, DMG_DEFAULT, this);
            }
        }


        if (4.f > XMVectorGetX(XMVector3Length(m_pTransformCom->Get_Position() - pPlayer->Get_Transform()->Get_Position())))
        {
            if(!m_bAttacking && m_fHp > 0.f)
            {
                m_pModelCom->Set_AnimTag(m_strAttackAnimTag, false);
                m_bAttacking = true;
            }
        }
    }


    Hit_ChangeColor(fTimeDelta);
    if (m_fHp <= 0.f)
    {
        m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Dissolve;
    }
    m_pCCTCom->Update(fTimeDelta);
    auto pRawController = m_pCCTCom->Get_Controller();
    if(pRawController)
    {
        auto pxvFP = pRawController->getFootPosition();
        m_pTransformCom->Set_Position(XMVectorSet(pxvFP.x, pxvFP.y, pxvFP.z, 1.f));
    }
}

void CMonster_Worm::LateTick(_float fTimeDelta)
{
    //if(m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_Position(), 2.f))
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMonster_Worm::Render()
{
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
    m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    if (m_iRenderPass == (_uint)SHADER_VTXANIMMESH_PASS::Dissolve)
    {
        m_pShaderCom->Bind_Float("g_Dissolve", m_pModelCom->Get_Current_Animation_TrackRatio());
        DEFAULTNOISE->Bind_ShaderResource(m_pShaderCom, "g_Noise_Texture");
    }
    m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
    for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
    {
        if (m_pModelCom->Get_Material(i).size())
        {
            auto& vecMat = m_pModelCom->Get_Material(i);
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
        m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrix", i);
        m_pShaderCom->Begin(m_iRenderPass);
        m_pModelCom->Render(i);
    }
    return S_OK;
}

void CMonster_Worm::Dying()
{
    __super::Dying();
    if (m_pCCTCom)
        m_pCCTCom->Disable();
}

void CMonster_Worm::Push_From_Pool(void* pArg)
{
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pCCTCom->Set_FootPosition(XMLoadFloat4x4(&pDesc->Transformation));
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
    }
}

void CMonster_Worm::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
{
    if (m_fHp <= 0.f)
    {
        return;
    }
    if (iDmgType == DMG_EXPLODE)
    {
        if (m_fInvincible >= 0.01f)
        {
            return;
        }
        else
        {
            m_fInvincible = m_pBeatManager->Get_Beat();
        }
    }

    m_fHp -= fAmount;
    m_fTakeHit = 0.2f;
    if (m_fHp <= 0.f)
    {
        m_bKilled = true;
        m_pModelCom->Set_AnimTag("Venous_Clawed_worm.ao|Venous_Clawed_Worm_Death_3", false);
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Spider_Death");
    }
    else
    {
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Spider_Attacked");
    }
}

CMonster_Worm* CMonster_Worm::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster_Worm* pInstance = new CMonster_Worm(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMonster_Worm"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster_Worm::Clone(void* pArg)
{
    CMonster_Worm* pInstance = new CMonster_Worm(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMonster_Worm"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_Worm::Free()
{
    Safe_Release(m_pCCTCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    __super::Free();
}
