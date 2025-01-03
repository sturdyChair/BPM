#include "Monster_Demon.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "CharacterController.h"
#include "Beat_Manager.h"
#include "Player.h"
#include "Player_Manager.h"
#include "Effect_Charge.h"
#include "Effect_Sweep.h"
#include "Effect_Spike.h"
#include "Level_Loading.h"
#include "Particle.h"
#include "UI_BossHP.h"
#include "Effect_AlvisRock.h"
#include "Trail.h"

CMonster_Demon::CMonster_Demon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CMonster_Base(pDevice, pContext)
{
}

CMonster_Demon::CMonster_Demon(const CMonster_Demon& rhs) :
    CMonster_Base(rhs)
{
}

HRESULT CMonster_Demon::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CMonster_Demon::Initialize(void* pArg)
{
    CTransform::TRANSFORM_DESC transDesc;
    transDesc.fRotationPerSec = XM_PI;
    __super::Initialize(pArg);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Demon", L"Model", (CComponent**)&m_pModelCom);
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
    cctDesc.height = 2.5f;
    cctDesc.radius = 2.5f;
    cctDesc.userData = this;
    _float3 vPos;
    XMStoreFloat3(&vPos, m_pTransformCom->Get_Position());
    cctDesc.position = PxExtendedVec3(vPos.x, vPos.y, vPos.z);
    cctDesc.material = m_pGameInstance->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_CharacterController", L"Com_CCT", (CComponent**)&m_pCCTCom, &cctDesc);
    m_pCCTCom->m_bGravity = true;
    m_pCCTCom->m_bUseVelocity_Calculation = true;
    m_pCCTCom->m_fFriction = 50.f;
    m_pCCTCom->m_fGravity = 80.f;


    m_strAnimTag[IDLE] = "SK_Demon_2.ao|Anim_Demon2_4_Idle_160";
    m_fAnimSpeed[IDLE] = 1.5f;
    m_strAnimTag[MOVE] = "SK_Demon_2.ao|Anim_Demon2_1_Running_28";
    m_fAnimSpeed[MOVE] = 1.5f;

    m_strAnimTag[DYING] = "SK_Demon_2.ao|Anim_Demon2_6_IntimidationScream_84";
    m_fAnimSpeed[DYING] = 1.5f;

    m_strAnimTag[PUNCH_1] = "SK_Demon_2.ao|Anim_Demon2_8_AttackSwip_80";
    m_fAnimSpeed[PUNCH_1] = 1.5f;
    m_strAnimTag[JUMP] = "SK_Demon_2.ao|Anim_Demon2_7_Jump2_95";
    m_fAnimSpeed[JUMP] = 2.f;
    m_eState = IDLE;
    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    m_fTimer = m_pBeatManager->Get_Beat();
    m_ProjectileDesc.eCG = CG_MONSTER_PROJECTILE;
    m_fHp = 600.f;
    m_fMaxHp = m_fHp;



    return S_OK;
}

void CMonster_Demon::PriorityTick(_float fTimeDelta)
{
    if (m_fInvincible > 0.001f)
    {
        m_fInvincible -= fTimeDelta;
    }
}

void CMonster_Demon::Tick(_float fTimeDelta)
{
    m_pModelCom->Play_Animation(fTimeDelta * m_fAnimSpeed[m_eState], m_fAnimFrom, m_fAnimTo);
    _float fCurRatio = m_pModelCom->Get_Current_Animation_TrackRatio();
    auto pPlayer = m_pPlayerManager->Get_Player();
    XMStoreFloat4(&m_vPrevHandLPos, XMLoadFloat4x4(&m_MatLeft).r[3]);
    XMStoreFloat4(&m_vPrevHandRPos, XMLoadFloat4x4(&m_MatRight).r[3]);

    XMStoreFloat4x4(&m_MatLeft, m_pModelCom->Get_BoneMatrix("Character1_LeftHand") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
    XMStoreFloat4x4(&m_MatRight, m_pModelCom->Get_BoneMatrix("Character1_RightHand") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));

    if (pPlayer)
    {
        if (m_pModelCom->Get_Finished())
        {
            Anim_Finished();
            fCurRatio = 0.f;
        }
        switch (m_eState)
        {
        case Client::CMonster_Demon::IDLE:
        {
            m_fTimer -= fTimeDelta;
            if (m_fTimer <= 0.f)
            {
                State_Change();
            }
        }
        break;
        case Client::CMonster_Demon::MOVE:
        {
            auto pPlayer = m_pPlayerManager->Get_Player();
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            if (XMVectorGetX(XMVector3Length(XMVectorSetY(vPlayerPosition - vPosition, 0.f))) > 5.f)
            {
                XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, m_pCCTCom->m_vVelocity.y)) * 25.f);
            }
            else
            {
                Anim_Finished();
            }
        }
        break;
        case Client::CMonster_Demon::PUNCH_1:
        {
            if (fCurRatio >= 0.5f)
            {
                if (!m_bPush)
                {
                    m_bPush = true;
                    auto pPlayer = m_pPlayerManager->Get_Player();

                    _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                    _vector vPosition = m_pTransformCom->Get_Position();
                    CEffect_Projectile::PROJECTILE_DESC PDESC;
                    PDESC.eCG = CG_MONSTER_PROJECTILE;
                    XMStoreFloat3(&PDESC.vDirection, XMVector3Normalize(vPlayerPosition - vPosition) * 13.f);

                    _matrix ProjectileMat = XMLoadFloat4x4(&m_MatRight);
                    ProjectileMat.r[3] += XMLoadFloat3(&PDESC.vDirection) * 0.1f;
                    _float4x4 ProjMat;
                    XMStoreFloat4x4(&ProjMat, XMMatrixScaling(0.5f, 0.5f, 0.5f) * ProjectileMat);
                    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Projectile", m_pGameInstance->Get_Current_LevelID(), L"Effect",
                        &PDESC, &ProjMat);


                    m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Demon_Cast");
                }
            }
        }
        break;
        case Client::CMonster_Demon::JUMP:
        {
            if (fCurRatio >= 0.9f)
            {
                if (!m_bPush)
                {
                    m_bPush = true;
                    auto pPlayer = m_pPlayerManager->Get_Player();

                    _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                    _vector vPosition = m_pTransformCom->Get_Position();
                    CEffect_Projectile::PROJECTILE_DESC PDESC;
                    PDESC.eCG = CG_MONSTER_PROJECTILE;
                    XMStoreFloat3(&PDESC.vDirection, XMVector3Normalize(vPlayerPosition - vPosition) * 13.f);

                    _matrix ProjectileMat = XMLoadFloat4x4(&m_MatLeft);
                    ProjectileMat.r[3] += XMLoadFloat3(&PDESC.vDirection) * 0.1f;
                    _float4x4 ProjMat;
                    XMStoreFloat4x4(&ProjMat, XMMatrixScaling(0.5f, 0.5f, 0.5f) * ProjectileMat);
                    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Projectile", m_pGameInstance->Get_Current_LevelID(), L"Effect",
                        &PDESC, &ProjMat);


                    m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Projectile");
                }
            }
        }
        break;
        case Client::CMonster_Demon::STATE_END:
            break;
        default:
            break;
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
            m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Default;
        }
        if (m_bKilled)
        {
            m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Dissolve;
        }
        _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
        _vector vTargetPos = pPlayer->Get_Transform()->Get_Position();
        m_pTransformCom->LookAt_Horizontal(vTargetPos);


        m_pCCTCom->Update(fTimeDelta);
        auto pRawController = m_pCCTCom->Get_Controller();
        if (pRawController)
        {
            auto pxvFP = pRawController->getFootPosition();
            m_pTransformCom->Set_Position(XMVectorSet(pxvFP.x, pxvFP.y, pxvFP.z, 1.f));
        }
    }
}

void CMonster_Demon::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);


}

HRESULT CMonster_Demon::Render()
{
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
    m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));

    //_float4 vColor = { 1.0f,0.2f,0.2f,1.f };
    //m_pShaderCom->Bind_Vector4("g_vColor", &vColor);
    //m_pShaderCom->Bind_Vector4("g_vCamPos", &m_pGameInstance->Get_CamPosition_Float4());
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

void CMonster_Demon::Dying()
{
    if (m_pCCTCom)
        m_pCCTCom->Disable();
}

void CMonster_Demon::Push_From_Pool(void* pArg)
{
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pCCTCom->Set_FootPosition(XMLoadFloat4x4(&pDesc->Transformation));
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
    }
}

void CMonster_Demon::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
{
    if (m_bKilled)
        return;
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
    m_fTakeHit = 0.2f;

    m_fHp -= fAmount;
    if (m_fHp <= 0.f)
    {
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Demon_Death");
        m_eState = DYING;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
        m_bKilled = true;
    }

}

void CMonster_Demon::To_Idle()
{
    m_eState = IDLE;
    m_pModelCom->Set_AnimTag(m_strAnimTag[IDLE], true);
    m_fTimer = m_pBeatManager->Get_Beat() * 2.f;
    m_fAnimFrom = 0.f;
    m_fAnimTo = 1.f;
}

void CMonster_Demon::State_Change()
{
    _int iRand = PUNCH_1;


    m_eState = MOVE;
    m_eNextState = (STATE)iRand;
    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], true);
    m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
    m_bChargeFlag = false;
    m_bPush = false;
    m_bFlag = false;
}

void CMonster_Demon::Anim_Finished()
{
    string strCurTag = m_pModelCom->Get_Current_Animation_Tag();

    switch (m_eState)
    {
    case Client::CMonster_Demon::IDLE:
        break;
    case Client::CMonster_Demon::MOVE:
    {
        m_eState = m_eNextState;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;

    }
    break;
    case Client::CMonster_Demon::PUNCH_1:
    {
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
        To_Idle();
    }
    break;
    case Client::CMonster_Demon::JUMP:
    {
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
        To_Idle();
    }
    break;
    case Client::CMonster_Demon::DYING:
    {
        Dead();
    }
    break;
    default:
        break;
    }

}

CMonster_Demon* CMonster_Demon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster_Demon* pInstance = new CMonster_Demon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMonster_Demon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster_Demon::Clone(void* pArg)
{
    CMonster_Demon* pInstance = new CMonster_Demon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMonster_Demon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_Demon::Free()
{
    Safe_Release(m_pCCTCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    __super::Free();
}
