#include "Monster_Golem.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "Player_Manager.h"
#include "Player.h"
#include "CharacterController.h"
#include "Beat_Manager.h"
#include "Effect_RuneField.h"

CMonster_Golem::CMonster_Golem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CMonster_Base(pDevice, pContext)
{
}

CMonster_Golem::CMonster_Golem(const CMonster_Golem& rhs) :
    CMonster_Base(rhs)
{
    m_iCollisionGroup = CG_MONSTER_CONTACT;
}

HRESULT CMonster_Golem::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CMonster_Golem::Initialize(void* pArg)
{
    __super::Initialize(pArg);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Golem", L"Model", (CComponent**)&m_pModelCom);
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
    cctDesc.height = 4.0f;
    cctDesc.radius = 1.0f;
    cctDesc.userData = this;
    _float3 vPos;
    XMStoreFloat3(&vPos, m_pTransformCom->Get_Position());
    cctDesc.position = PxExtendedVec3(vPos.x, vPos.y, vPos.z);
    cctDesc.material = m_pGameInstance->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_CharacterController", L"Com_CCT", (CComponent**)&m_pCCTCom, &cctDesc);
    m_pCCTCom->m_bGravity = true;
    if (m_pGameInstance->Get_Current_LevelID() == LEVEL_TOOL)
    {
        m_pCCTCom->m_bGravity = false;
    }
    m_pCCTCom->m_bUseVelocity_Calculation = true;
    m_pCCTCom->m_fFriction = 30.f;
    m_strIdleTag = "Forest_Golem_3_Realistic.ao|G_RM_Attack3";
    m_fHp = 850.f;
    m_pModelCom->Set_AnimTag(m_strIdleTag, true);
    m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Emission;
    return S_OK;
}

void CMonster_Golem::PriorityTick(_float fTimeDelta)
{
    if (m_fInvincible > 0.001f)
    {
        m_fInvincible -= fTimeDelta;
    }
}

void CMonster_Golem::Tick(_float fTimeDelta)
{

    m_pModelCom->Play_Animation(fTimeDelta * 2.5f);
    auto pPlayer =  m_pPlayerManager->Get_Player();

    if (!m_fHp <= 0.f && !m_bAttacking && pPlayer)
        m_pTransformCom->LookAt_Horizontal(pPlayer->Get_Transform()->Get_Position());

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
            m_pModelCom->Set_AnimTag(m_strIdleTag, true);
        }
    }
    if (m_pBeatManager->Get_BeatCount() - m_iPrevBitCount >= m_iAttackDelay)
    {
        Attack(fTimeDelta);
    }

    m_fTakeHit -= fTimeDelta;
    if (m_fTakeHit <= 0.f)
    {
        m_fTakeHit = 0.f;
    }
    if (m_fTakeHit >= 0.01f)
    {
        m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::TakeHit;
    }
    else
    {
        m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Emission;
    }
    if (m_fHp <= 0.f)
    {
        m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Dissolve;
    }
    m_pCCTCom->Update(fTimeDelta);
    auto pRawController = m_pCCTCom->Get_Controller();
    if (pRawController)
    {
        auto pxvFP = pRawController->getFootPosition();
        m_pTransformCom->Set_Position(XMVectorSet(pxvFP.x, pxvFP.y, pxvFP.z, 1.f));
    }
}

void CMonster_Golem::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMonster_Golem::Render()
{//emmision = 3
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
    m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
    if (m_iRenderPass == (_uint)SHADER_VTXANIMMESH_PASS::Dissolve)
    {
        m_pShaderCom->Bind_Float("g_Dissolve", m_pModelCom->Get_Current_Animation_TrackRatio());
        DEFAULTNOISE->Bind_ShaderResource(m_pShaderCom, "g_Noise_Texture");
    }
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
                pTexture = vecMat[3];
                if (pTexture)
                    pTexture->Bind_ShaderResource(m_pShaderCom, "g_Emission_Texture");
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

void CMonster_Golem::Dying()
{
    __super::Dying();
    if (m_pCCTCom)
        m_pCCTCom->Disable();
}

void CMonster_Golem::Push_From_Pool(void* pArg)
{
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pCCTCom->Set_FootPosition(XMLoadFloat4x4(&pDesc->Transformation));
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
    }
    m_iAttackDelay = 4;
    m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
}

void CMonster_Golem::Attack(_float fTimeDelta)
{
    if (m_fHp <= 0.f)
        return;
    m_bAttacking = false;
    m_iPrevBitCount = m_pBeatManager->Get_BeatCount();

    m_pModelCom->Set_AnimTag(m_strAttackTag, false);
    CEffect_RuneField::RF_DESC desc;
    desc.fLifeTime = 4 * m_pBeatManager->Get_Beat();
    desc.bLight = false;
    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_RuneField", m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, m_pTransformCom->Get_WorldMatrix_Ptr());

    m_iAttackDelay = 4;

    m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Golem_Stomp");
}

void CMonster_Golem::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
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
        m_pModelCom->Set_AnimTag(m_strDeathTag, false);
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Golem_Death");
    }
    else
    {

    }
}

CMonster_Golem* CMonster_Golem::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster_Golem* pInstance = new CMonster_Golem(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMonster_Golem"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster_Golem::Clone(void* pArg)
{
    CMonster_Golem* pInstance = new CMonster_Golem(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMonster_Golem"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_Golem::Free()
{
    Safe_Release(m_pCCTCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    __super::Free();
}
