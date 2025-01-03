#include "Monster_ClockWork.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "CharacterController.h"
#include "Beat_Manager.h"
#include "Player.h"
#include "Player_Manager.h"


CMonster_ClockWork::CMonster_ClockWork(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CMonster_Base(pDevice, pContext)
{
}

CMonster_ClockWork::CMonster_ClockWork(const CMonster_ClockWork& rhs) :
    CMonster_Base(rhs)
{
}

HRESULT CMonster_ClockWork::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CMonster_ClockWork::Initialize(void* pArg)
{
    __super::Initialize(pArg);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Monster_ClockWork", L"Model", (CComponent**)&m_pModelCom);
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
    m_pCCTCom->m_fFriction = 50.f;

    m_ProjectileDesc.eCG = CG_MONSTER_PROJECTILE;
    m_iAttackDelay += rand() % 3 - 1;
    m_strIdleTag = "SK_Fire_Golem.ao|ExoGame_Fire_Golem_Idle";
    m_pModelCom->Set_AnimTag(m_strIdleTag, true);
    m_fHp = 350.f;
    return S_OK;
}

void CMonster_ClockWork::PriorityTick(_float fTimeDelta)
{
    if (m_fInvincible > 0.001f)
    {
        m_fInvincible -= fTimeDelta;
    }
}

void CMonster_ClockWork::Tick(_float fTimeDelta)
{
    auto pPlayer = m_pPlayerManager->Get_Player();

    if (!m_fHp <= 0.f && pPlayer)
        m_pTransformCom->LookAt_Horizontal(pPlayer->Get_Transform()->Get_Position());
    //else
    //{
    //    PxControllerState pCS;
    //    auto pRawController = m_pCCTCom->Get_Controller();
    //    if (pRawController)
    //    {
    //        pRawController->getState(pCS);
    //        if (pCS.collisionFlags & PxControllerCollisionFlag::eCOLLISION_DOWN)
    //        {
    //            m_bKilled = true;
    //            Dead();
    //        }
    //    }
    //}
    if (m_pModelCom->Get_Current_Animation_Tag() == m_strAttack && m_fHp > 0.f)
    {
        if (m_pModelCom->Get_Current_Animation_TrackRatio() >= 0.42f && !m_bAttacked)
        {
            m_bAttacked = true;
            if(pPlayer)
                Attack();
        }
    }

    m_pModelCom->Play_Animation(fTimeDelta);

    if (m_pModelCom->Get_Finished())
    {
        if (m_fHp <= 0.f)
        {
            m_bKilled = true;
            Drop_Coin();
            Dead();
            return;
        }
        else if (m_pModelCom->Get_Current_Animation_Tag() == m_strEnterInvincibility)
        {
            m_pModelCom->Set_AnimTag(m_strExitInvincibility, false);
            m_bBall = false;
        }
        else
        {
            m_pModelCom->Set_AnimTag(m_strIdleTag, true);
        }
    }
    if (m_pBeatManager->Get_BeatCount() - m_iPrevBitCount == m_iAttackDelay && m_fHp > 0.f)
    {
        m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
        m_pModelCom->Set_AnimTag(m_strAttack, false);
        m_bAttacked = false;
    }
    if (m_pBeatManager->Get_BeatCount() - m_iPrevBitCount == m_iAttackDelay - 1)
    {
        m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::ColorBoost;
    }
    else
    {
        m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Default;
    }

    Hit_ChangeColor(fTimeDelta);
    if (m_fHp <= 0.f)
    {
        m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Dissolve;
    }
    //if (m_pBeatManager->Get_BeatCount() - m_iPrevBitCount >= m_iAttackDelay)
    //{
    //    Attack(fTimeDelta);
    //}


    m_pCCTCom->Update(fTimeDelta);
}

void CMonster_ClockWork::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMonster_ClockWork::Render()
{
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
    m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
    if (m_iRenderPass == (_uint)SHADER_VTXANIMMESH_PASS::ColorBoost)
    {
        m_pShaderCom->Bind_Float("g_fMagicNumber", 1.f + m_pBeatManager->Get_Timer() / m_pBeatManager->Get_Beat());
    }
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

void CMonster_ClockWork::Dying()
{
    __super::Dying();
    if (m_pCCTCom)
        m_pCCTCom->Disable();
}

void CMonster_ClockWork::Push_From_Pool(void* pArg)
{
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pCCTCom->Set_FootPosition(XMLoadFloat4x4(&pDesc->Transformation));
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
    }
    m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
}

void CMonster_ClockWork::Attack()
{
    if (m_fHp <= 0.f)
        return;
    m_iPrevBitCount = m_pBeatManager->Get_BeatCount();

    auto pPlayer = m_pPlayerManager->Get_Player();
    _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
    _vector vPosition = m_pTransformCom->Get_Position();
    _vector vDir = XMVector3Normalize(vPlayerPosition - vPosition);
    XMStoreFloat3(&m_ProjectileDesc.vDirection, vDir * 26.f);

    _matrix ProjectileMat = XMLoadFloat4x4(m_pModelCom->Get_BoneMatrixPtr(m_strLHand)) * m_pTransformCom->Get_WorldMatrix_XMMat();


    ProjectileMat.r[3] += XMLoadFloat3(&m_ProjectileDesc.vDirection) * 0.1f;
    _float4x4 ProjMat;
    XMStoreFloat4x4(&ProjMat, ProjectileMat);
    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Aura", m_pGameInstance->Get_Current_LevelID(), L"Effect",
        &m_ProjectileDesc, &ProjMat);
    XMStoreFloat3(&m_ProjectileDesc.vDirection, XMVector3Rotate(vDir, XMQuaternionRotationRollPitchYaw(0.f, 0.1666f * XM_PI, 0.f)) * 26.f);
    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Aura", m_pGameInstance->Get_Current_LevelID(), L"Effect",
        &m_ProjectileDesc, &ProjMat);
    XMStoreFloat3(&m_ProjectileDesc.vDirection, XMVector3Rotate(vDir, XMQuaternionRotationRollPitchYaw(0.f, -0.1666f * XM_PI, 0.f)) * 26.f);
    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Aura", m_pGameInstance->Get_Current_LevelID(), L"Effect",
        &m_ProjectileDesc, &ProjMat);

    m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Projectile");
    m_iAttackDelay = 5 + rand() % 5;
}

void CMonster_ClockWork::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
{
    if ( m_fHp <= 0.f )
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
    //if (!m_bInvUsed && m_fHp <= 100.f && m_fHp > 0.f)
    //{
    //    m_bInvUsed = true;
    //    Enter_Invincibility();
    //    m_bBall = true;
    //}

    m_fTakeHit = 0.2f;

    if (m_fHp <= 0.f)
    {
        m_bKilled = true;
        m_pCCTCom->m_bGravity = true;
        m_pModelCom->Set_AnimTag(m_strDeath, false);
    }
}

void CMonster_ClockWork::Enter_Invincibility()
{
    m_bBall = true;
    m_bInvUsed = true;
    m_pModelCom->Set_AnimTag(m_strEnterInvincibility, false);
}

CMonster_ClockWork* CMonster_ClockWork::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster_ClockWork* pInstance = new CMonster_ClockWork(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMonster_ClockWork"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster_ClockWork::Clone(void* pArg)
{
    CMonster_ClockWork* pInstance = new CMonster_ClockWork(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMonster_ClockWork"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_ClockWork::Free()
{
    Safe_Release(m_pCCTCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    __super::Free();
}
