#include "Monster_Alvis.h"
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

CMonster_Alvis::CMonster_Alvis(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CMonster_Base(pDevice, pContext)
{
}

CMonster_Alvis::CMonster_Alvis(const CMonster_Alvis& rhs) :
    CMonster_Base(rhs)
{
}

HRESULT CMonster_Alvis::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CMonster_Alvis::Initialize(void* pArg)
{
    CTransform::TRANSFORM_DESC transDesc;
    transDesc.fRotationPerSec = XM_PI;
    __super::Initialize(pArg);
    Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Monster_Alvis", L"Model", (CComponent**)&m_pModelCom);
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


    m_strAnimTag[IDLE] = "Rampage_Elemental.ao|Idle";
    m_fAnimSpeed[IDLE] = 1.5f;
    m_strAnimTag[START] = "Rampage_Elemental.ao|LevelStart";
    m_fAnimSpeed[START] = 2.0f;
    m_strAnimTag[MOVE_START] = "Rampage_Elemental.ao|Jog_Fwd_Start";
    m_fAnimSpeed[MOVE_START] = 1.5f;
    m_strAnimTag[MOVE_END]   = "Rampage_Elemental.ao|Jog_Fwd_Stop";
    m_fAnimSpeed[MOVE_END]   = 1.5f;
    m_strAnimTag[ENRAGE_START] = "Rampage_Elemental.ao|Ability_Enrage_Start";
    m_fAnimSpeed[ENRAGE_START] = 1.5f;
    m_strAnimTag[ENRAGE_END] = "Rampage_Elemental.ao|Ability_RMB_Smash";
    m_fAnimSpeed[ENRAGE_END] = 1.5f;
    m_strAnimTag[DYING] = "Rampage_Elemental.ao|KnockBack";
    m_fAnimSpeed[DYING] = 1.5f;
    m_strAnimTag[GROUND_SMASH_START] = "Rampage_Elemental.ao|Ability_GroundSmash_Start";
    m_fAnimSpeed[GROUND_SMASH_START] = 1.5f;
    m_strAnimTag[GROUND_SMASH_LOOP] = "Rampage_Elemental.ao|Ability_GroundSmash_Loop";
    m_fAnimSpeed[GROUND_SMASH_LOOP] = 1.5f;
    m_strAnimTag[GROUND_SMASH_END] = "Rampage_Elemental.ao|Ability_GroundSmash_End";
    m_fAnimSpeed[GROUND_SMASH_END] = 1.0f;

    m_strAnimTag[PUNCH_START] = "Rampage_Elemental.ao|Ability_Enrage_Start";
    m_fAnimSpeed[PUNCH_START] = 1.8f;
    m_strAnimTag[PUNCH_1] = "Rampage_Elemental.ao|Attack_Biped_Melee_A";
    m_fAnimSpeed[PUNCH_1] = 1.5f;
    m_strAnimTag[PUNCH_2] = "Rampage_Elemental.ao|Attack_Biped_Melee_B";
    m_fAnimSpeed[PUNCH_2] = 1.5f;
    m_strAnimTag[PUNCH_3] = "Rampage_Elemental.ao|Attack_Biped_Melee_C";
    m_fAnimSpeed[PUNCH_3] = 1.5f;
    m_strAnimTag[ROAR] = "Rampage_Elemental.ao|Emote_Master_Roar_T3";
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

void CMonster_Alvis::PriorityTick(_float fTimeDelta)
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

void CMonster_Alvis::Tick(_float fTimeDelta)
{
    m_pModelCom->Play_Animation(fTimeDelta * m_fAnimSpeed[m_eState], m_fAnimFrom, m_fAnimTo);
    _float fCurRatio = m_pModelCom->Get_Current_Animation_TrackRatio();
    auto pPlayer = m_pPlayerManager->Get_Player();
    XMStoreFloat4(&m_vPrevHandLPos, XMLoadFloat4x4(&m_MatLeft).r[3]);
    XMStoreFloat4(&m_vPrevHandRPos, XMLoadFloat4x4(&m_MatRight).r[3]);

    XMStoreFloat4x4(&m_MatLeft, m_pModelCom->Get_BoneMatrix("hand_l") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
    XMStoreFloat4x4(&m_MatRight, m_pModelCom->Get_BoneMatrix("hand_r") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));

    XMStoreFloat4x4(&m_MatLeftDown, XMMatrixTranslation(0.f, -0.5f, 0.f)* XMLoadFloat4x4(&m_MatLeft));
    XMStoreFloat4x4(&m_MatRightDown, XMMatrixTranslation(0.f, -0.5f, 0.f) * XMLoadFloat4x4(&m_MatRight));
    if(pPlayer)
    {
        if (m_pModelCom->Get_Finished())
        {
            Anim_Finished();
            fCurRatio = 0.f;
        }
        switch (m_eState)
        {
        case Client::CMonster_Alvis::START:
            break;
        case Client::CMonster_Alvis::IDLE:
        {
            m_fTimer -= fTimeDelta;
            if (m_fTimer <= 0.f)
            {
                State_Change();
            }
        }
        break;
        case Client::CMonster_Alvis::MOVE_START:
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
        case Client::CMonster_Alvis::MOVE_END:
        {
            auto pPlayer = m_pPlayerManager->Get_Player();
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(vPlayerPosition - vPosition) * 15.f * (1.f - fCurRatio));
        }
        break;
        case Client::CMonster_Alvis::ENRAGE_START:
            break;
        case Client::CMonster_Alvis::ENRAGE_END:
        {
            if (!m_bPush)
            {
                m_bPush = true;
                auto pPlayer = m_pPlayerManager->Get_Player();
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                _vector vPosition = m_pTransformCom->Get_Position();
                XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(vPlayerPosition - vPosition) * 20.f);
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact0");
                _matrix ImpactWorld = XMMatrixIdentity();
                ImpactWorld.r[3] = m_pTransformCom->Get_Position();
                _float4x4 Impact;
                XMStoreFloat4x4(&Impact, ImpactWorld);
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_AlvisImpact", m_pGameInstance->Get_Current_LevelID(), L"Effect", nullptr, &Impact);
                CEffect_AlvisRock::PROJECTILE_DESC desc;
                desc.eCG = CG_MONSTER_PROJECTILE;

                for (_uint i = 0; i < 5; ++i)
                {
                    _wstring strTag = L"Prototype_GameObject_Effect_AlvisRock_Fragment_";
                    strTag += to_wstring(i);
                    for (_uint j = 0; j < 3; ++j)
                    {
                        XMStoreFloat3(&desc.vDirection, XMVector3Normalize(XMVectorSet(fRand(-1.f, 1.f), fRand(0.5f, 1.f), fRand(-1.f,1.f), 0.f)) * fRand(5.f, 10.f));
                        m_pGameInstance->Push_Object_From_Pool(strTag, m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &Impact);
                    }
                }
            }
        }
        break;
        case Client::CMonster_Alvis::GROUND_SMASH_START:
        {
            if (!m_bPush)
            {
                m_bPush = true;
                m_pCCTCom->m_vVelocity.y = 40.f;
            }
            m_iSmashTimer = 0;
            m_iSmashCount = m_pBeatManager->Get_BeatCount();
        }
        break;
        case Client::CMonster_Alvis::GROUND_SMASH_LOOP:
        {
            auto pPlayer = m_pPlayerManager->Get_Player();
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            XMStoreFloat3(&m_pCCTCom->m_vVelocity, (vPlayerPosition - vPosition) * 14.f);
            m_pCCTCom->m_bGravity = false;
            _uint iCurrCount = m_pBeatManager->Get_BeatCount();
            if (iCurrCount != m_iSmashCount)
            {
                --m_iSmashTimer;
                if (m_iSmashTimer <= 0)
                {
                    m_eState = GROUND_SMASH_END;
                    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
                }
            }
        }
        break;
        case Client::CMonster_Alvis::GROUND_SMASH_END:
        {
            if (m_bPush)
            {
                m_bPush = false;
                m_pCCTCom->m_vVelocity.y = -100.f;
                m_pCCTCom->m_bGravity = true;
                m_fLeftTrailTimer = 2.f;
                m_fRightTrailTimer = 2.f;
                m_pLeftTrail->Push_From_Pool(nullptr);
                m_pRightTrail->Push_From_Pool(nullptr);
            }
            PxControllerState pCS;
            m_pCCTCom->Get_Controller()->getState(pCS);
            if (pCS.collisionFlags & PxControllerCollisionFlag::eCOLLISION_DOWN)
            {
                if (m_pCCTCom->m_vVelocity.y < -50.0f)
                {
                    _matrix ImpactWorld = XMMatrixIdentity();
                    ImpactWorld.r[3] = m_pTransformCom->Get_Position();
                    _float4x4 Impact;
                    XMStoreFloat4x4(&Impact, ImpactWorld);
                    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_AlvisImpact", m_pGameInstance->Get_Current_LevelID(), L"Effect", nullptr, &Impact);
                    //m_fStompTimer = m_pBeatManager->Get_Beat();
                    m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact2");
                    CEffect_AlvisRock::PROJECTILE_DESC desc;
                    desc.eCG = CG_MONSTER_PROJECTILE;

                    for (_uint i = 0; i < 5; ++i)
                    {
                        _wstring strTag = L"Prototype_GameObject_Effect_AlvisRock_Fragment_";
                        strTag += to_wstring(i);
                        for (_uint j = 0; j < 3; ++j)
                        {
                            XMStoreFloat3(&desc.vDirection, XMVector3Normalize(XMVectorSet(fRand(-1.f, 1.f), fRand(0.5f, 1.f), fRand(-1.f, 1.f), 0.f)) * fRand(10.f, 15.f));
                            m_pGameInstance->Push_Object_From_Pool(strTag, m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &Impact);
                        }
                    }
                }
                m_pCCTCom->m_vVelocity.y = 0.f;
                m_pCCTCom->m_vVelocity.x = 0.f;
                m_pCCTCom->m_vVelocity.z = 0.f;
            }
        }
        break;
        case Client::CMonster_Alvis::PUNCH_START:
            break;
        case Client::CMonster_Alvis::PUNCH_1://Right
        {
            if (fCurRatio >= 0.66f)
            {
                m_eState = PUNCH_2;
                m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
                auto pPlayer = m_pPlayerManager->Get_Player();
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                _vector vPosition = m_pTransformCom->Get_Position();
                _vector vDir = XMVectorSetY(vPlayerPosition - vPosition, 0.f);
                if(XMVectorGetX(XMVector3Length(vDir)) >= 8.f)
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
        case Client::CMonster_Alvis::PUNCH_2://Left
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
        case Client::CMonster_Alvis::PUNCH_3://Right
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
        case Client::CMonster_Alvis::ROAR:
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
                    for(_int j = -1; j<2;++j)
                    {
                        if (i == 0 && j == 0)
                            continue;

                        XMStoreFloat3(&desc.vDirection, XMVector3Normalize(XMVectorSet((_float)i , 0.f, (_float)j, 0.f)) * 33.f);
                        desc.vDirection.y += fRand(5.f, 10.f);
                        m_pGameInstance->Push_Object_From_Pool(strTag, m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &Impact);
                    }
                }
            }
        }
        break;
        case Client::CMonster_Alvis::DYING:
        {
            pPlayer->NoMiss(true);
        }
            break;
        case Client::CMonster_Alvis::STATE_END:
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

void CMonster_Alvis::LateTick(_float fTimeDelta)
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

HRESULT CMonster_Alvis::Render()
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

void CMonster_Alvis::Dying()
{
    if (m_pCCTCom)
        m_pCCTCom->Disable();
}

void CMonster_Alvis::Push_From_Pool(void* pArg)
{
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pCCTCom->Set_FootPosition(XMLoadFloat4x4(&pDesc->Transformation));
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
    }
}

void CMonster_Alvis::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
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

        m_pModelCom->Set_AnimTag("Rampage_Elemental.ao|KnockBack", true);
        if (m_iFinish >= 4)
        {
            m_pModelCom->Set_AnimTag("Rampage_Elemental.ao|KnockBack", false);
            m_bKilled = true;
        }
        return;
    }

    m_fHp -= fAmount;
    if (m_fHp <= 0.f)
    {
        m_pModelCom->Set_AnimTag("Rampage_Elemental.ao|KnockBack", true);
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Death");
        m_eState = DYING;
        m_pBeatManager->Queue_Sequence(L"Asgard_CoolDown");
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, L"Alvis_Chord0");
    }

}

void CMonster_Alvis::To_Idle()
{
    m_eState = IDLE;
    m_pModelCom->Set_AnimTag(m_strAnimTag[IDLE], true);
    m_fTimer = m_pBeatManager->Get_Beat() * 2.f;
    m_fAnimFrom = 0.f;
    m_fAnimTo = 1.f;
}

void CMonster_Alvis::State_Change()
{
    _int iRand = rand() % 4;
    if(iRand == 0)
    { 
        iRand = ENRAGE_START;
    }
    else if(iRand == 1)
    {
        iRand = PUNCH_START;
    }
    else if (iRand == 2)
    {
        iRand = GROUND_SMASH_START;

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

void CMonster_Alvis::Anim_Finished()
{
    string strCurTag = m_pModelCom->Get_Current_Animation_Tag();

    switch (m_eState)
    {
    case Client::CMonster_Alvis::START:
    {
        To_Idle();
    }
        break;
    case Client::CMonster_Alvis::IDLE:
        break;
    case Client::CMonster_Alvis::MOVE_START:
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
    case Client::CMonster_Alvis::MOVE_END:
    {
        //m_eState = m_eNextState;
        //m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
        break;
    case Client::CMonster_Alvis::ENRAGE_START:
    {
        m_eState = ENRAGE_END;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
        break;
    case Client::CMonster_Alvis::ENRAGE_END:
    {
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
        To_Idle();
    }
        break;
    case Client::CMonster_Alvis::GROUND_SMASH_START:
    {
        m_eState = GROUND_SMASH_LOOP;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], true);
    }
        break;
    case Client::CMonster_Alvis::GROUND_SMASH_LOOP:
    {
        m_eState = GROUND_SMASH_END;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
        break;
    case Client::CMonster_Alvis::GROUND_SMASH_END:
    {
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
        To_Idle();
    }
        break;
    case Client::CMonster_Alvis::PUNCH_START:
    {
        m_eState = PUNCH_1;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact1");
        auto pPlayer = m_pPlayerManager->Get_Player();
        _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
        _vector vPosition = m_pTransformCom->Get_Position();
        _vector vDir = XMVectorSetY(vPlayerPosition - vPosition, 0.f);
        if (XMVectorGetX(XMVector3Length(vDir)) >= 8.f)
            XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(vDir) * 70.f);
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact1");
    }
    break;
    case Client::CMonster_Alvis::PUNCH_1:
    {
        m_eState = PUNCH_2;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
        break;
    case Client::CMonster_Alvis::PUNCH_2:
    {
        m_eState = PUNCH_3;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
        break;
    case Client::CMonster_Alvis::PUNCH_3:
    {
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
        To_Idle();
    }
        break;
    case Client::CMonster_Alvis::ROAR:
    {
        To_Idle();
    }
        break;
    case Client::CMonster_Alvis::DYING:
    {
        Dead();
    }
        break;
    default:
        break;
    }

}

CMonster_Alvis* CMonster_Alvis::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster_Alvis* pInstance = new CMonster_Alvis(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMonster_Alvis"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster_Alvis::Clone(void* pArg)
{
    CMonster_Alvis* pInstance = new CMonster_Alvis(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMonster_Alvis"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_Alvis::Free()
{
    Safe_Release(m_pCCTCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pHpBar);
    Safe_Release(m_pLeftTrail);
    Safe_Release(m_pRightTrail);
    __super::Free();
}
