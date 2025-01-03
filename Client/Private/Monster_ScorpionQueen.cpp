#include "Monster_ScorpionQueen.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "Player_Manager.h"
#include "Player.h"
#include "CharacterController.h"
#include "Beat_Manager.h"
#include "Effect_Fragment.h"

CMonster_ScorpionQueen::CMonster_ScorpionQueen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CMonster_Base(pDevice, pContext)
{
}

CMonster_ScorpionQueen::CMonster_ScorpionQueen(const CMonster_ScorpionQueen& rhs) :
    CMonster_Base(rhs)
{
    m_iCollisionGroup = CG_MONSTER_CONTACT;
}

HRESULT CMonster_ScorpionQueen::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CMonster_ScorpionQueen::Initialize(void* pArg)
{
    __super::Initialize(pArg);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Monster_ScorpionQueen", L"Model", (CComponent**)&m_pModelCom);
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
    cctDesc.height = 0.25f;
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
    m_strIdleTag = "JC_ScorpionQueenMonster_Skin.ao|JC_ScorpionQueenMonster_Stand1";
    m_fHp = 500.f;
    m_pModelCom->Set_AnimTag(m_strIdleTag, true);
    return S_OK;
}

void CMonster_ScorpionQueen::PriorityTick(_float fTimeDelta)
{
    if (m_fInvincible > 0.001f)
    {
        m_fInvincible -= fTimeDelta;
    }
}

void CMonster_ScorpionQueen::Tick(_float fTimeDelta)
{
    m_bMoving = false;
    //Moving(fTimeDelta);
    m_pModelCom->Play_Animation(fTimeDelta);
    auto pPlayer = m_pPlayerManager->Get_Player();
    if (m_pModelCom->Get_Current_Animation_Tag() == m_strAttackTag)
    {
        if (m_pModelCom->Get_Current_Animation_TrackRatio() >= 0.4f && !m_bAttacking)
        {
            if (pPlayer)
            {
                m_bAttacking = true;
                m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
                CEffect_Fragment::PROJECTILE_DESC desc;
                desc.eCG = CG_MONSTER_PROJECTILE;
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                _vector vPosition = m_pTransformCom->Get_Position();
                _vector vDir =  vPlayerPosition - vPosition;
                vDir = XMVector3Normalize(vDir);
                _float4x4 Matrix;
                _matrix Projectile;
                _vector vRandir;
                _vector vFinalDir;
                for(_uint i = 0;i<5;++i)
                {
                    Projectile = XMLoadFloat4x4(m_pModelCom->Get_BoneMatrixPtr(m_strLeftFinger + to_string(i))) * m_pTransformCom->Get_WorldMatrix_XMMat();
                    vRandir = XMVectorSet(rand() % 100 * 0.01f, rand() % 100 * 0.01f, rand() % 100 * 0.01f, 0.f);
                    vFinalDir = vDir * 0.8f + XMVector3Normalize(vRandir) * 0.2f;
                    XMStoreFloat3(&desc.vDirection, vFinalDir * 20.f);
                    XMStoreFloat4x4(&Matrix, Projectile);
                    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Fragment",m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc,
                        &Matrix);

                    Projectile = XMLoadFloat4x4(m_pModelCom->Get_BoneMatrixPtr(m_strRightFinger + to_string(i))) * m_pTransformCom->Get_WorldMatrix_XMMat();
                    vRandir = XMVectorSet(rand() % 100 * 0.01f, rand() % 100 * 0.01f, rand() % 100 * 0.01f, 0.f);
                    vFinalDir = vDir * 0.8f + XMVector3Normalize(vRandir) * 0.2f;
                    XMStoreFloat3(&desc.vDirection, vFinalDir * 20.f);
                    XMStoreFloat4x4(&Matrix, Projectile);
                    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Fragment", m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc,
                        &Matrix);
                }

                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ScorpionQueen_Fire");
            }
        }
    }
    if (!m_fHp <= 0.f && !m_bAttacking && pPlayer && !m_bMoving)
        m_pTransformCom->LookAt_Horizontal(pPlayer->Get_Transform()->Get_Position());
    if (!m_bMoving && m_pModelCom->Get_Current_Animation_Tag() == m_strRunTag)
    {
        m_pModelCom->Set_AnimTag(m_strIdleTag, true);
    }

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
            //Choose_Destination();
        }
    }
    //if (m_pBeatManager->Get_BeatCount() - m_iPrevBitCount == m_iAttackDelay - 1)
    //{
    //}
    //else
    //{
    //}
    if (m_pBeatManager->Get_BeatCount() - m_iPrevBitCount >= m_iAttackDelay)
    {
        Attack(fTimeDelta);
    }

    Hit_ChangeColor(fTimeDelta);
    if (m_fHp <= 0.f)
    {
        m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Dissolve;
    }
    m_pCCTCom->Update(fTimeDelta);
    //PxControllerState pCS;
    //m_pCCTCom->Get_Controller()->getState(pCS);
    //if (pCS.collisionFlags & PxControllerCollisionFlag::eCOLLISION_DOWN)
    //{
    //    m_pCCTCom->m_vVelocity = _float3{ 0.f,0.f,0.f };
    //}
    auto pRawController = m_pCCTCom->Get_Controller();
    if(pRawController)
    {
        auto pxvFP = pRawController->getFootPosition();
        m_pTransformCom->Set_Position(XMVectorSet(pxvFP.x, pxvFP.y, pxvFP.z, 1.f));
    }
}

void CMonster_ScorpionQueen::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMonster_ScorpionQueen::Render()
{
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

void CMonster_ScorpionQueen::Dying()
{
    __super::Dying();
    if (m_pCCTCom)
        m_pCCTCom->Disable();
}

void CMonster_ScorpionQueen::Push_From_Pool(void* pArg)
{
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pCCTCom->Set_FootPosition(XMLoadFloat4x4(&pDesc->Transformation));
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
    }
    m_iAttackDelay = 10 + rand() % 7;
    m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
    XMStoreFloat3(&m_vDestination, m_pTransformCom->Get_Position());
}

void CMonster_ScorpionQueen::Attack(_float fTimeDelta)
{
    if (m_fHp <= 0.f)
        return;
    m_bAttacking = false;
    m_iPrevBitCount = m_pBeatManager->Get_BeatCount();

    m_pModelCom->Set_AnimTag(m_strAttackTag, false);

    m_iAttackDelay = 6 + rand() % 4;
}

void CMonster_ScorpionQueen::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
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
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ScorpionQueen_Death");
        if (iDmgType == DMG_EXPLODE)
        {
            XMStoreFloat3(&m_pCCTCom->m_vImpulse, XMLoadFloat3(&m_pCCTCom->m_vImpulse) + XMVector3Normalize(m_pTransformCom->Get_Position() - XMLoadFloat3(&vHitPoint)) * fAmount * 0.5f);
        }
    }
    else
    {
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ScorpionQueen_Hurt");
    }
}

void CMonster_ScorpionQueen::Choose_Destination()
{
    auto pPlayer = m_pPlayerManager->Get_Player();

    if (!pPlayer)
        return;
    _vector vPlayerPos = pPlayer->Get_Transform()->Get_Position();

    _float fRandX = (rand() % 100 - 100) * 0.5f;
    _float fRandZ = (rand() % 100 - 100) * 0.5f;
    _vector vRand = XMVectorSet(fRandX, 0.f, fRandZ, 0.f);
    m_fMovingTimer = 5.f;
    XMStoreFloat3(&m_vDestination, vPlayerPos + vRand);
}

void CMonster_ScorpionQueen::Moving(_float fTimeDelta)
{
    m_fMovingTimer -= fTimeDelta;
    _vector vPos = m_pTransformCom->Get_Position();
    _vector vDest = XMLoadFloat3(&m_vDestination);
    _vector vDir = vPos - vDest;
    vDir = XMVectorSetY(vDir, 0.f);
    if(XMVectorGetX(XMVector3Length(vDir)) > 1.f || m_fMovingTimer <= 0.f)
    {
        m_bMoving = true;
        m_pTransformCom->LookAt_Horizontal(vDest);
        m_pCCTCom->Move_Forward(m_fSpeed);
        if (m_pModelCom->Get_Current_Animation_Tag() != m_strRunTag)
        {
            m_pModelCom->Set_AnimTag(m_strRunTag, true);
        }
    }
}



CMonster_ScorpionQueen* CMonster_ScorpionQueen::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster_ScorpionQueen* pInstance = new CMonster_ScorpionQueen(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMonster_ScorpionQueen"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster_ScorpionQueen::Clone(void* pArg)
{
    CMonster_ScorpionQueen* pInstance = new CMonster_ScorpionQueen(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMonster_ScorpionQueen"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_ScorpionQueen::Free()
{
    Safe_Release(m_pCCTCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    __super::Free();
}
