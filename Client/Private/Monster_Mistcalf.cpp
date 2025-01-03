#include "Monster_Mistcalf.h"
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
#include "Effect_Gullveig_Field.h"
#include "Effect_RuneField.h"

CMonster_Mistcalf::CMonster_Mistcalf(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CMonster_Base(pDevice, pContext)
{
}

CMonster_Mistcalf::CMonster_Mistcalf(const CMonster_Mistcalf& rhs) :
    CMonster_Base(rhs)
{
}

HRESULT CMonster_Mistcalf::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CMonster_Mistcalf::Initialize(void* pArg)
{
    CTransform::TRANSFORM_DESC transDesc;
    transDesc.fRotationPerSec = XM_PI;
    __super::Initialize(pArg);
    Add_Component(LEVEL_SVART, L"Prototype_Component_Model_Monster_Mistcalf", L"Model", (CComponent**)&m_pModelCom);
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
    cctDesc.radius = 1.5f;
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


    m_strAnimTag[IDLE] = "Riktor_BadSanta.ao|Idle";
    m_fAnimSpeed[IDLE] = 1.5f;
    m_strAnimTag[START] = "Riktor_BadSanta.ao|Emote_Master_BackToWork_T3";
    m_fAnimSpeed[START] = 2.0f;
    m_strAnimTag[MOVE_START] = "Riktor_BadSanta.ao|Jog_FWD_Start";
    m_fAnimSpeed[MOVE_START] = 1.5f;
    m_strAnimTag[MOVE_END] = "Riktor_BadSanta.ao|Jog_FWD_Stop";
    m_fAnimSpeed[MOVE_END] = 1.5f;
    m_strAnimTag[ENRAGE_START] = "Riktor_BadSanta.ao|Ability_Lockdown";
    m_fAnimSpeed[ENRAGE_START] = 1.5f;

    m_strAnimTag[DYING] = "Riktor_BadSanta.ao|KnockBack";
    m_fAnimSpeed[DYING] = 1.5f;
    m_strAnimTag[PULL_START] = "Riktor_BadSanta.ao|Ability_Hook_Start";
    m_fAnimSpeed[PULL_START] = 1.5f;
    m_strAnimTag[PULL_CAST] = "Riktor_BadSanta.ao|Ability_Hook_Cast";
    m_fAnimSpeed[PULL_CAST] = 1.5f;
    m_strAnimTag[PULL_END] = "Riktor_BadSanta.ao|Ability_Hook_Pull";
    m_fAnimSpeed[PULL_END] = 1.0f;

    m_strAnimTag[PUNCH_START] = "Riktor_BadSanta.ao|Ability_Enrage_Start";
    m_fAnimSpeed[PUNCH_START] = 1.8f;
    m_strAnimTag[PUNCH_1] = "Riktor_BadSanta.ao|PrimaryAttack_A_Slow";
    m_fAnimSpeed[PUNCH_1] = 1.5f;
    m_strAnimTag[PUNCH_2] = "Riktor_BadSanta.ao|PrimaryAttack_B_Slow";
    m_fAnimSpeed[PUNCH_2] = 1.5f;
    m_strAnimTag[PUNCH_3] = "Riktor_BadSanta.ao|PrimaryAttack_C_Slow";
    m_fAnimSpeed[PUNCH_3] = 1.5f;
    m_strAnimTag[ROAR] = "Riktor_BadSanta.ao|Ability_Ultimate";
    m_fAnimSpeed[ROAR] = 2.5f;
    m_eState = START;
    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    m_fTimer = m_pBeatManager->Get_Beat();
    m_ProjectileDesc.eCG = CG_MONSTER_PROJECTILE;
    m_fHp = 3000.f;
    m_fMaxHp = m_fHp;
    m_pHpBar = (CUI_BossHP*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_BossHP", nullptr);
    m_pHpBar->Set_MaxHp(m_fMaxHp);
    m_pHpBar->Set_Hp(m_fHp);

    CTrail::TRAIL_DESC desc{};
    desc.bTextureAnim = false;
    desc.iMaxRect = 12;
    desc.iRenderPass = (_uint)SHADER_VTXPOS_PASS::Trail;
    desc.pDownTransform = &m_MatLeftDown;
    desc.pUpperTransform = &m_MatLeft;
    desc.strTextureTag = L"Prototype_Component_Texture_FireTrail";

    m_pLeftTrail = CTrail::Create(m_pDevice, m_pContext, desc);
    desc.pDownTransform = &m_MatRightDown;
    desc.pUpperTransform = &m_MatRight;
    m_pRightTrail = CTrail::Create(m_pDevice, m_pContext, desc);

    return S_OK;
}

void CMonster_Mistcalf::PriorityTick(_float fTimeDelta)
{
    m_pHpBar->PriorityTick(fTimeDelta);
    if (m_fInvincible > 0.001f)
    {
        m_fInvincible -= fTimeDelta;
    }
    if (m_fLeftTrailTimer >= 0.f)
    {
        m_fLeftTrailTimer -= fTimeDelta;
        m_pLeftTrail->PriorityTick(fTimeDelta);
    }
    if (m_fRightTrailTimer >= 0.f)
    {
        m_fRightTrailTimer -= fTimeDelta;
        m_pRightTrail->PriorityTick(fTimeDelta);
    }
}

void CMonster_Mistcalf::Tick(_float fTimeDelta)
{
    m_pModelCom->Play_Animation(fTimeDelta * m_fAnimSpeed[m_eState], m_fAnimFrom, m_fAnimTo);
    _float fCurRatio = m_pModelCom->Get_Current_Animation_TrackRatio();
    auto pPlayer = m_pPlayerManager->Get_Player();
    XMStoreFloat4(&m_vPrevHandLPos, XMLoadFloat4x4(&m_MatLeft).r[3]);
    XMStoreFloat4(&m_vPrevHandRPos, XMLoadFloat4x4(&m_MatRight).r[3]);

    XMStoreFloat4x4(&m_MatLeft, m_pModelCom->Get_BoneMatrix("hand_l") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
    XMStoreFloat4x4(&m_MatRight, m_pModelCom->Get_BoneMatrix("hand_r") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));

    XMStoreFloat4x4(&m_MatLeftDown, XMMatrixTranslation(0.f, -0.5f, 0.f) * XMLoadFloat4x4(&m_MatLeft));
    XMStoreFloat4x4(&m_MatRightDown, XMMatrixTranslation(0.f, -0.5f, 0.f) * XMLoadFloat4x4(&m_MatRight));
    if (pPlayer)
    {
        if (m_pModelCom->Get_Finished())
        {
            Anim_Finished();
            fCurRatio = 0.f;
        }
        switch (m_eState)
        {
        case Client::CMonster_Mistcalf::START:
            break;
        case Client::CMonster_Mistcalf::IDLE:
        {
            m_fTimer -= fTimeDelta;
            if (m_fTimer <= 0.f)
            {
                State_Change();
            }
        }
        break;
        case Client::CMonster_Mistcalf::MOVE_START:
        {
            auto pPlayer = m_pPlayerManager->Get_Player();
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            if (XMVectorGetX(XMVector3Length(XMVectorSetY(vPlayerPosition - vPosition, 0.f))) > 5.f)
            {
                XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f)) * 25.f);
            }
            else
            {
                Anim_Finished();
            }
        }
        break;
        case Client::CMonster_Mistcalf::MOVE_END:
        {
            auto pPlayer = m_pPlayerManager->Get_Player();
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(vPlayerPosition - vPosition) * 15.f * (1.f - fCurRatio));
        }
        break;
        case Client::CMonster_Mistcalf::ENRAGE_START:
        {
            if (!m_bPush)
            {
                m_bPush = true;
                CEffect_RuneField::RF_DESC rdesc;
                _float4x4 EffectMat;
                _matrix MatTemp = XMMatrixTranslationFromVector(XMVectorSetW(XMLoadFloat3(&m_pPlayerManager->Get_Player()->Get_FootPosition()), 1.f));
                _uint iRand = rand() % 2 + 1;
                rdesc.bLight = true;
                for (_uint i = 0; i < iRand; ++i)
                {
                    rdesc.fLifeTime = (4.f + (_float)i) * m_pBeatManager->Get_Beat();
                    _matrix MatEffect = MatTemp;
                    MatEffect.r[3] += XMVectorSet(fRand(-5.f, 5.f), 0.f, fRand(-5.f, 5.f), 0.f);
                    XMStoreFloat4x4(&EffectMat, MatEffect);
                    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_RuneField", m_pGameInstance->Get_Current_LevelID(), L"Effect", &rdesc, &EffectMat);
                    m_pBeatManager->Set_N_BeatType(3 + i, BM_JUMP);
                }
            }
        }
            break;
        case Client::CMonster_Mistcalf::PULL_START:
        {

        }
        break;
        case Client::CMonster_Mistcalf::PULL_CAST:
        {

        }
        break;
        case Client::CMonster_Mistcalf::PULL_END:
        {
            if (m_bPush)
            {
                auto pPlayer = m_pPlayerManager->Get_Player();
                auto vPos = m_pTransformCom->Get_Position();
                auto vTargetPos = pPlayer->Get_Transform()->Get_Position();
                _float3 vPush;
                XMStoreFloat3(&vPush, (vPos - vTargetPos) * 10.f);
                pPlayer->Add_PushVector(vPush, 0.1f);
                m_bPush = false;
                m_fLeftTrailTimer = 0.5f;
                m_fRightTrailTimer = 0.5f;
                m_pLeftTrail->Push_From_Pool(nullptr);
                m_pRightTrail->Push_From_Pool(nullptr);
            }
        }
        break;
        case Client::CMonster_Mistcalf::PUNCH_START:
            break;
        case Client::CMonster_Mistcalf::PUNCH_1://Right
        {
            if (m_bPush)
            {
                m_bPush = false;
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact1");
                auto pPlayer = m_pPlayerManager->Get_Player();
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                _vector vPosition = m_pTransformCom->Get_Position();
                _vector vDir = XMVectorSetY(vPlayerPosition - vPosition, 0.f);
                if (XMVectorGetX(XMVector3Length(vDir)) >= 8.f)
                    XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(vDir) * 70.f);
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact1");
            }

            if (fCurRatio >= 0.66f)
            {
                m_bPush = true;
                m_eState = PUNCH_2;
                m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
                auto pPlayer = m_pPlayerManager->Get_Player();
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                _vector vPosition = m_pTransformCom->Get_Position();
                _vector vDir = XMVectorSetY(vPlayerPosition - vPosition, 0.f);
                if (XMVectorGetX(XMVector3Length(vDir)) >= 8.f)
                    XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(vDir) * 70.f);
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact1");
            }
            else if (fCurRatio >= 0.2f)
            {
                m_pCCTCom->m_vVelocity.x = 0.f;
                m_pCCTCom->m_vVelocity.z = 0.f;
            }
            _vector vTargetPos = pPlayer->Get_Transform()->Get_Position();
            _matrix HandMat = XMLoadFloat4x4(&m_MatRight);
            if (XMVectorGetX(XMVector4Length(vTargetPos - HandMat.r[3])) < 4.f)
            {
                _float3 vHitPoint;
                XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
                pPlayer->Take_Damage(25.f, vHitPoint, DAMAGE_TYPE::DMG_DEFAULT, this);
                _vector vPrevHandPos = XMLoadFloat4(&m_vPrevHandRPos);
                pPlayer->Clear_PushVector();
                _float3 vPush;
                XMStoreFloat3(&vPush, XMVector3Normalize(HandMat.r[3] - vPrevHandPos) * 30.f);

                pPlayer->Add_PushVector(vPush, 1.0f);
            }
        }
        break;
        case Client::CMonster_Mistcalf::PUNCH_2://Left
        {

            if (fCurRatio >= 0.66f)
            {
                m_eState = PUNCH_3;
                m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
                auto pPlayer = m_pPlayerManager->Get_Player();
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                _vector vPosition = m_pTransformCom->Get_Position();
                _vector vDir = XMVectorSetY(vPlayerPosition - vPosition, 0.f);
                if (XMVectorGetX(XMVector3Length(vDir)) >= 8.f)
                    XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(vDir) * 70.f);
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact1");
            }
            else if (fCurRatio >= 0.2f)
            {
                m_pCCTCom->m_vVelocity.x = 0.f;
                m_pCCTCom->m_vVelocity.z = 0.f;
            }
            _vector vTargetPos = pPlayer->Get_Transform()->Get_Position();
            _matrix HandMat = XMLoadFloat4x4(&m_MatLeft);
            if (XMVectorGetX(XMVector4Length(vTargetPos - HandMat.r[3])) < 4.f)
            {
                _float3 vHitPoint;
                XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
                pPlayer->Take_Damage(25.f, vHitPoint, DAMAGE_TYPE::DMG_DEFAULT, this);
                _vector vPrevHandPos = XMLoadFloat4(&m_vPrevHandLPos);
                pPlayer->Clear_PushVector();
                _float3 vPush;
                XMStoreFloat3(&vPush, XMVector3Normalize(HandMat.r[3] - vPrevHandPos) * 30.f);

                pPlayer->Add_PushVector(vPush, 1.0f);
            }
        }
        break;
        case Client::CMonster_Mistcalf::PUNCH_3://Right
        {
            if (fCurRatio >= 0.2f)
            {
                m_pCCTCom->m_vVelocity.x = 0.f;
                m_pCCTCom->m_vVelocity.z = 0.f;
            }
            _vector vTargetPos = pPlayer->Get_Transform()->Get_Position();
            _matrix HandMat = XMLoadFloat4x4(&m_MatRight);
            if (XMVectorGetX(XMVector4Length(vTargetPos - HandMat.r[3])) < 4.f)
            {
                _float3 vHitPoint;
                XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
                pPlayer->Take_Damage(25.f, vHitPoint, DAMAGE_TYPE::DMG_DEFAULT, this);
                _vector vPrevHandPos = XMLoadFloat4(&m_vPrevHandRPos);
                pPlayer->Clear_PushVector();
                _float3 vPush;
                XMStoreFloat3(&vPush, XMVector3Normalize(HandMat.r[3] - vPrevHandPos) * 30.f);

                pPlayer->Add_PushVector(vPush, 1.0f);
            }
        }
        break;
        case Client::CMonster_Mistcalf::ROAR:
        {
            if (!m_bPush)
            {
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Bump");
                m_bPush = true;

            }
            if (fCurRatio >= 0.5f && !m_bFlag)
            {
                m_bFlag = true;
                CEffect_AlvisRock::PROJECTILE_DESC desc;
                desc.eCG = CG_MONSTER_PROJECTILE;
                _wstring strTag = L"Prototype_GameObject_Effect_AlvisRock";
                _matrix ImpactWorld = XMMatrixIdentity();
                ImpactWorld.r[3] = m_pTransformCom->Get_Position();
                _float4x4 Impact;
                XMStoreFloat4x4(&Impact, ImpactWorld);
                Impact._42 += 1.f;
                for (_int i = -1; i < 2; ++i)
                {
                    for (_int j = -1; j < 2; ++j)
                    {
                        if (i == 0 && j == 0)
                            continue;

                        XMStoreFloat3(&desc.vDirection, XMVector3Normalize(XMVectorSet((_float)i, 0.f, (_float)j, 0.f)) * 33.f);
                        desc.vDirection.y += fRand(5.f, 10.f);
                        m_pGameInstance->Push_Object_From_Pool(strTag, m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &Impact);
                    }
                }
            }
        }
        break;
        case Client::CMonster_Mistcalf::DYING:
        {
            pPlayer->NoMiss(true);
        }
        break;
        case Client::CMonster_Mistcalf::STATE_END:
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
            m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Rim;
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
        m_pHpBar->Set_Hp(m_fHp);
        m_pHpBar->Tick(fTimeDelta);

        if (m_fLeftTrailTimer >= 0.f)
        {
            m_pLeftTrail->Tick(fTimeDelta);
        }
        if (m_fRightTrailTimer >= 0.f)
        {
            m_pRightTrail->Tick(fTimeDelta);
        }
    }
}

void CMonster_Mistcalf::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
    m_pHpBar->LateTick(fTimeDelta);
    if (m_fLeftTrailTimer >= 0.f)
    {
        m_pLeftTrail->LateTick(fTimeDelta);
    }
    if (m_fRightTrailTimer >= 0.f)
    {
        m_pRightTrail->LateTick(fTimeDelta);
    }
}

HRESULT CMonster_Mistcalf::Render()
{
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
    m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));

    _float4 vColor = { 1.0f,0.2f,0.2f,1.f };
    m_pShaderCom->Bind_Vector4("g_vColor", &vColor);
    //_float4 vTemp = { 0.f,0.f,0.f,1.f };
    //_float4 vPPos;
    //XMStoreFloat4(&vPPos, m_pPlayerManager->Get_Player()->Get_Transform()->Get_Position());
    m_pShaderCom->Bind_Vector4("g_vCamPos", &m_pGameInstance->Get_CamPosition_Float4());
    //m_pShaderCom->Bind_Vector4("g_vCamPos", &vPPos);
    //m_pShaderCom->Bind_Matrix("g_ViewInvMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TS_VIEW_INV));
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

void CMonster_Mistcalf::Dying()
{
    if (m_pCCTCom)
        m_pCCTCom->Disable();
}

void CMonster_Mistcalf::Push_From_Pool(void* pArg)
{
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pCCTCom->Set_FootPosition(XMLoadFloat4x4(&pDesc->Transformation));
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
        m_vPortalPos.x = pDesc->Transformation._41;
        m_vPortalPos.y = pDesc->Transformation._42;
        m_vPortalPos.z = pDesc->Transformation._43;
        m_vPortalPos.w = 1.f;
    }
}

void CMonster_Mistcalf::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
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
    if (m_fHp <= 0.f && !m_bKilled)
    {
        _wstring Chord = L"Alvis_Chord";
        m_pGameInstance->Stop_Sound(SOUND_CHANNEL::MUSIC, Chord + to_wstring(m_iFinish));
        ++m_iFinish;
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, Chord + to_wstring(m_iFinish));

        m_pModelCom->Set_AnimTag(m_strAnimTag[DYING], true);
        if (m_iFinish >= 4)
        {
            m_pModelCom->Set_AnimTag(m_strAnimTag[DYING], false);
            m_bKilled = true;
        }
        return;
    }

    m_fHp -= fAmount;
    if (m_fHp <= 0.f)
    {
        m_pModelCom->Set_AnimTag(m_strAnimTag[DYING], true);
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Death");
        m_eState = DYING;
        m_pBeatManager->Queue_Sequence(L"Asgard_CoolDown");
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, L"Alvis_Chord0");
    }

}

void CMonster_Mistcalf::To_Idle()
{
    m_eState = IDLE;
    m_pModelCom->Set_AnimTag(m_strAnimTag[IDLE], true);
    m_fTimer = m_pBeatManager->Get_Beat() * 2.f;
    m_fAnimFrom = 0.f;
    m_fAnimTo = 1.f;
}

void CMonster_Mistcalf::State_Change()
{
    _int iRand = rand() % 3;
    if (iRand == 0)
    {
        iRand = ENRAGE_START;
    }
    else if (iRand == 1)
    {
        iRand = PUNCH_1;
    }
    else if (iRand == 2)
    {
        iRand = PULL_START;

    }
    else if (iRand == 3)
    {
        iRand = ROAR;

    }
    m_eState = MOVE_START;
    m_eNextState = (STATE)iRand;
    m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE_START], false);
    m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
    m_iAttackCount = 0;
    m_bChargeFlag = false;
    m_bPush = false;
    m_bFlag = false;
}

void CMonster_Mistcalf::Anim_Finished()
{
    string strCurTag = m_pModelCom->Get_Current_Animation_Tag();

    switch (m_eState)
    {
    case Client::CMonster_Mistcalf::START:
    {
        To_Idle();
    }
    break;
    case Client::CMonster_Mistcalf::IDLE:
        break;
    case Client::CMonster_Mistcalf::MOVE_START:
    {
        m_eState = m_eNextState;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
        m_fLeftTrailTimer = 2.5f;
        m_fRightTrailTimer = 2.5f;
        m_pLeftTrail->Push_From_Pool(nullptr);
        m_pRightTrail->Push_From_Pool(nullptr);
        //m_eState = MOVE_END;
        //m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE_END], false);
    }
    break;
    case Client::CMonster_Mistcalf::MOVE_END:
    {
        //m_eState = m_eNextState;
        //m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
    break;
    case Client::CMonster_Mistcalf::ENRAGE_START:
    {
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
        To_Idle();
    }
    break;
    case Client::CMonster_Mistcalf::PULL_START:
    {
        m_bPush = true;
        m_eState = PULL_CAST;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
    break;
    case Client::CMonster_Mistcalf::PULL_CAST:
    {
        m_bPush = true;
        m_eState = PULL_END;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
    break;
    case Client::CMonster_Mistcalf::PULL_END:
    {
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
        To_Idle();
    }
    break;
    case Client::CMonster_Mistcalf::PUNCH_START:
    {
        m_eState = PUNCH_1;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);

    }
    break;
    case Client::CMonster_Mistcalf::PUNCH_1:
    {
        m_eState = PUNCH_2;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
    break;
    case Client::CMonster_Mistcalf::PUNCH_2:
    {
        m_eState = PUNCH_3;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
    break;
    case Client::CMonster_Mistcalf::PUNCH_3:
    {
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
        To_Idle();
    }
    break;
    case Client::CMonster_Mistcalf::ROAR:
    {
        To_Idle();
    }
    break;
    case Client::CMonster_Mistcalf::DYING:
    {
        _float4x4 PortWorld;
        XMStoreFloat4x4(&PortWorld, XMMatrixTranslation(m_vPortalPos.x, m_vPortalPos.y -10.f, m_vPortalPos.z));
        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Boss_Portal", m_pGameInstance->Get_Current_LevelID(), L"Portal", &m_vPortalPos, &PortWorld);
        Dead();
    }
    break;
    default:
        break;
    }

}

CMonster_Mistcalf* CMonster_Mistcalf::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster_Mistcalf* pInstance = new CMonster_Mistcalf(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMonster_Mistcalf"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster_Mistcalf::Clone(void* pArg)
{
    CMonster_Mistcalf* pInstance = new CMonster_Mistcalf(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMonster_Mistcalf"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_Mistcalf::Free()
{
    Safe_Release(m_pCCTCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pHpBar);
    Safe_Release(m_pLeftTrail);
    Safe_Release(m_pRightTrail);
    __super::Free();
}
