#include "Monster_Gullveig.h"
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
#include "Effect_Sharp_Projectile.h"
#include "Effect_RuneField.h"
#include "Effect_Gullveig_Field.h"
#include "Effect_Gullveig_LightWall.h"

CMonster_Gullveig::CMonster_Gullveig(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CMonster_Base(pDevice, pContext)
{
}

CMonster_Gullveig::CMonster_Gullveig(const CMonster_Gullveig& rhs) :
    CMonster_Base(rhs)
{
}

HRESULT CMonster_Gullveig::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CMonster_Gullveig::Initialize(void* pArg)
{
    CTransform::TRANSFORM_DESC transDesc;
    transDesc.fRotationPerSec = XM_PI;
    __super::Initialize(pArg);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Gullveig", L"Model", (CComponent**)&m_pModelCom);
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
    cctDesc.height = 2.f;
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
    m_pCCTCom->m_fGravity = 80.f;


    m_strAnimTag[IDLE] = "Morigesh_DarkHeart.ao|Idle";
    m_fAnimSpeed[IDLE] = 1.5f;
    m_strAnimTag[START] = "Morigesh_DarkHeart.ao|LevelStart";
    m_fAnimSpeed[START] = 2.0f;
    m_strAnimTag[MOVE_FORWARD] = "Morigesh_DarkHeart.ao|Jog_Fwd";
    m_fAnimSpeed[MOVE_FORWARD] = 1.5f;
    m_strAnimTag[MOVE_BACKWARD] = "Morigesh_DarkHeart.ao|Jog_Bwd";
    m_fAnimSpeed[MOVE_BACKWARD] = 1.5f;
    m_strAnimTag[MOVE_LEFT] = "Morigesh_DarkHeart.ao|Jog_Left";
    m_fAnimSpeed[MOVE_LEFT] = 1.5f;
    m_strAnimTag[MOVE_RIGHT] = "Morigesh_DarkHeart.ao|Jog_Right";
    m_fAnimSpeed[MOVE_RIGHT] = 1.5f;

    m_strAnimTag[THROW_PROJ_START] = "Morigesh_DarkHeart.ao|Ability_E_Targeting_Start";
    m_fAnimSpeed[THROW_PROJ_START] = 1.5f;
    m_strAnimTag[THROW_PROJ] = "Morigesh_DarkHeart.ao|Ability_E_MSA";
    m_fAnimSpeed[THROW_PROJ] = 2.0f;

    m_strAnimTag[RISE_START] = "Morigesh_DarkHeart.ao|Ability_Q_Targeting_Start";
    m_fAnimSpeed[RISE_START] = 1.5f;
    m_strAnimTag[RISE] = "Morigesh_DarkHeart.ao|Ability_Q";
    m_fAnimSpeed[RISE] = 1.5f;

    m_strAnimTag[DAGGER] = "Morigesh_DarkHeart.ao|Ability_R";
    m_fAnimSpeed[DAGGER] = 1.5f;

    m_strAnimTag[BOW] = "Morigesh_DarkHeart.ao|Emote_BlowSmoke";
    m_fAnimSpeed[BOW] = 2.5f;

    //m_strAnimTag[JUMP_START] = "Morigesh_DarkHeart.ao|Jump_Start";
    //m_fAnimSpeed[JUMP_START] = 1.5f;
    //m_strAnimTag[JUMP_END] = "Morigesh_DarkHeart.ao|Jump_Preland";
    //m_fAnimSpeed[JUMP_END] = 1.5f;


    m_strAnimTag[DYING] = "Morigesh_DarkHeart.ao|Knock_Back";
    m_fAnimSpeed[DYING] = 1.5f;

    m_eState = START;
    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    m_fTimer = m_pBeatManager->Get_Beat();
    m_ProjectileDesc.eCG = CG_MONSTER_PROJECTILE;
    m_fHp = 3300.f;
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

    //m_pLeftTrail = CTrail::Create(m_pDevice, m_pContext, desc);
    desc.pDownTransform = &m_MatRightDown;
    desc.pUpperTransform = &m_MatRight;
    //m_pRightTrail = CTrail::Create(m_pDevice, m_pContext, desc);

    return S_OK;
}

void CMonster_Gullveig::PriorityTick(_float fTimeDelta)
{
    m_pHpBar->PriorityTick(fTimeDelta);
    if (m_fInvincible > 0.001f)
    {
        m_fInvincible -= fTimeDelta;
    }
    //if (m_fLeftTrailTimer >= 0.f)
    //{
    //    m_fLeftTrailTimer -= fTimeDelta;
    //    m_pLeftTrail->PriorityTick(fTimeDelta);
    //}
    //if (m_fRightTrailTimer >= 0.f)
    //{
    //    m_fRightTrailTimer -= fTimeDelta;
    //    m_pRightTrail->PriorityTick(fTimeDelta);
    //}

    m_iPrevBitCount = m_iCurrBitCount;
    m_iCurrBitCount = m_pBeatManager->Get_BeatCount();
    if (m_fLightWallTimer > 0.f)
    {
        m_fLightWallTimer -= fTimeDelta;
    }
    else
    {
        m_fLightWallTimer = 0.f;
    }
}

void CMonster_Gullveig::Tick(_float fTimeDelta)
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
        case Client::CMonster_Gullveig::START:
            break;
        case Client::CMonster_Gullveig::IDLE:
        {
            m_fTimer -= fTimeDelta;
            if (m_fTimer <= 0.f)
            {
                State_Change();
            }
        }
        break;
        case Client::CMonster_Gullveig::MOVE_FORWARD:
        {
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            _float fDist = XMVectorGetX(XMVector3Length(XMVectorSetY(vPlayerPosition - vPosition, 0.f)));
            if (fDist > 30.f)
            {
                XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f)) * 20.f);
            }
            else if (fDist < 15.f)
            {
                m_eState = MOVE_BACKWARD;
                m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE_BACKWARD], true);
            }
            else
            {
                if (rand() % 2)
                {
                    m_eState = MOVE_RIGHT;
                }
                else
                {
                    m_eState = MOVE_LEFT;
                }
                m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], true);
            }
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                ++m_iMoveCounter;
                if (m_iMoveCounter >= 4)
                {
                    Anim_Finished();
                }
            }
        }
        break;
        case Client::CMonster_Gullveig::MOVE_BACKWARD:
        {
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            _float fDist = XMVectorGetX(XMVector3Length(XMVectorSetY(vPlayerPosition - vPosition, 0.f)));
            if (fDist < 25.f)
            {
                XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f)) * -20.f);
            }
            else if (fDist > 30.f)
            {
                m_eState = MOVE_FORWARD;
                m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE_FORWARD], true);
            }
            else
            {
                if (rand() % 2)
                {
                    m_eState = MOVE_RIGHT;
                }
                else
                {
                    m_eState = MOVE_LEFT;
                }
                m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], true);
            }
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                ++m_iMoveCounter;
                if (m_iMoveCounter >= 4)
                {
                    Anim_Finished();
                }
            }
        }
        break;
        case Client::CMonster_Gullveig::MOVE_RIGHT:
        {
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            _float fDist = XMVectorGetX(XMVector3Length(XMVectorSetY(vPlayerPosition - vPosition, 0.f)));
            if (fDist < 15.f)
            {
                m_eState = MOVE_BACKWARD;
                m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE_BACKWARD], true);
            }
            else if (fDist > 30.f)
            {
                m_eState = MOVE_FORWARD;
                m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], true);
            }
            XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMLoadFloat3(&m_pCCTCom->m_vRight) * 15.f);
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                if (rand() % 2)
                {
                    m_eState = MOVE_LEFT;
                    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], true);
                }
                ++m_iMoveCounter;
                if (m_iMoveCounter >= 4)
                {
                    Anim_Finished();
                }
            }
        }
        break;
        case Client::CMonster_Gullveig::MOVE_LEFT:
        {
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            _float fDist = XMVectorGetX(XMVector3Length(XMVectorSetY(vPlayerPosition - vPosition, 0.f)));
            if (fDist < 15.f)
            {
                m_eState = MOVE_BACKWARD;
                m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE_BACKWARD], true);
            }
            else if (fDist > 30.f)
            {
                m_eState = MOVE_FORWARD;
                m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE_FORWARD], true);
            }
            XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMLoadFloat3(&m_pCCTCom->m_vRight) * -15.f);
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                if (rand() % 2)
                {
                    m_eState = MOVE_RIGHT;
                    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], true);
                }
                ++m_iMoveCounter;
                if (m_iMoveCounter >= 4)
                {
                    Anim_Finished();
                }
            }
        }
        break;
        case THROW_PROJ_START:
        {

        }
        break;
        case THROW_PROJ:
        {
            //Prototype_GameObject_Effect_Sharp_Projectile
            if (!m_bPush)
            {
                m_bPush = true;
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                _vector vPosition = XMVectorSet(m_MatLeft._41, m_MatLeft._42, m_MatLeft._43, 1.f);
                CEffect_Sharp_Projectile::PROJECTILE_DESC RRDESC;
                RRDESC.eCG = CG_MONSTER_PROJECTILE;
                _vector vDir = XMVector3Normalize(vPlayerPosition - vPosition);
               
                for(_int i = -2; i<=2;++i)
                {
                    XMStoreFloat3(&RRDESC.vDirection, XMVector3Rotate(vDir, XMQuaternionRotationRollPitchYaw(0.f, XM_PI * (_float)i * 0.33f, 0.f)) * 40.f);
                    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Sharp_Projectile", m_pGameInstance->Get_Current_LevelID(), L"Effect", &RRDESC, &m_MatLeft);
                }
                
            }
        }
        break;
        case RISE_START:
        {
            if (!m_bPush)
            {
                m_bPush = true;
                CEffect_RuneField::RF_DESC rdesc;
                _float4x4 EffectMat;
                _matrix MatTemp = XMMatrixTranslationFromVector(XMVectorSetW(XMLoadFloat3(&m_pPlayerManager->Get_Player()->Get_FootPosition()),1.f));
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
        case RISE:
        {

        }
        break;
        case DAGGER:
        {
            //Prototype_GameObject_Effect_Gullveig_LightWall
            if (!m_bPush)
            {
                m_bPush = true;
                CEffect_Gullveig_LightWall::RF_DESC rdesc;
                rdesc.fLifeTime = (32.f) * m_pBeatManager->Get_Beat();
                m_fLightWallTimer = (32.f) * m_pBeatManager->Get_Beat();

                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Gullveig_LightWall", m_pGameInstance->Get_Current_LevelID(), L"Effect", &rdesc, m_pTransformCom->Get_WorldMatrix_Ptr());

            }

        }
        break;
        case BOW:
        {
            //
            if (!m_bPush)
            {
                m_bPush = true;
                CEffect_Gullveig_Field::RF_DESC rdesc;
                _float4x4 EffectMat;
                _matrix MatTemp = m_pTransformCom->Get_WorldMatrix_XMMat();

                rdesc.fLifeTime = (4.f) * m_pBeatManager->Get_Beat();
                _matrix MatEffect = MatTemp;
                MatEffect.r[3] += XMVectorSet(0.f, 4.f, 0.f, 0.f);
                XMStoreFloat4x4(&EffectMat, MatEffect);
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Gullveig_Field", m_pGameInstance->Get_Current_LevelID(), L"Effect", &rdesc, &EffectMat);
               

            }
        }
        break;


        case Client::CMonster_Gullveig::DYING:
        {
            pPlayer->NoMiss(true);
        }
        break;
        case Client::CMonster_Gullveig::STATE_END:
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

        //if (m_fLeftTrailTimer >= 0.f)
        //{
        //    m_pLeftTrail->Tick(fTimeDelta);
        //}
        //if (m_fRightTrailTimer >= 0.f)
        //{
        //    m_pRightTrail->Tick(fTimeDelta);
        //}
    }
}

void CMonster_Gullveig::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
    m_pHpBar->LateTick(fTimeDelta);
    //if (m_fLeftTrailTimer >= 0.f)
    //{
    //    m_pLeftTrail->LateTick(fTimeDelta);
    //}
    //if (m_fRightTrailTimer >= 0.f)
    //{
    //    m_pRightTrail->LateTick(fTimeDelta);
    //}
}

HRESULT CMonster_Gullveig::Render()
{
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
    m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));

    _float4 vColor = { 1.0f,0.9f,0.2f,1.f };
    m_pShaderCom->Bind_Vector4("g_vColor", &vColor);
    m_pShaderCom->Bind_Vector4("g_vCamPos", &m_pGameInstance->Get_CamPosition_Float4());
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

void CMonster_Gullveig::Dying()
{
    if (m_pCCTCom)
        m_pCCTCom->Disable();
}

void CMonster_Gullveig::Push_From_Pool(void* pArg)
{
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pCCTCom->Set_FootPosition(XMLoadFloat4x4(&pDesc->Transformation));
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
    }
    if(m_pBeatManager)
    {
        m_iCurrBitCount = m_pBeatManager->Get_BeatCount();
        m_iPrevBitCount = m_iCurrBitCount;
    }
    XMStoreFloat4(&m_vStartPos, m_pTransformCom->Get_Position());
}

void CMonster_Gullveig::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
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
    //
    m_fTakeHit = 0.2f;
    m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"GullveigPain");
    if (m_fHp <= 0.f && !m_bKilled)
    {
        _wstring Chord = L"Gullveig_Chord";
        m_pGameInstance->Stop_Sound(SOUND_CHANNEL::MUSIC, Chord + to_wstring(m_iFinish));
        ++m_iFinish;
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, Chord + to_wstring(m_iFinish));

        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], true);
        if (m_iFinish >= 4)
        {
            m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
            m_bKilled = true;
        }
        return;
    }

    m_fHp -= fAmount;
    if (m_fHp <= 0.f)
    {
        m_eState = DYING;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], true);
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"GullveigDeath");

        m_pBeatManager->Queue_Sequence(L"Asgard_CoolDown");
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, L"Gullveig_Chord0");
    }

}

void CMonster_Gullveig::To_Idle()
{
    m_eState = IDLE;
    m_pModelCom->Set_AnimTag(m_strAnimTag[IDLE], true);
    m_fTimer = m_pBeatManager->Get_Beat() * 2.f;
    m_fAnimFrom = 0.f;
    m_fAnimTo = 1.f;
}

void CMonster_Gullveig::State_Change()
{
    _int iRand = rand() % 3;
    if (m_fLightWallTimer <= 0.f)
    {
        iRand = DAGGER;
    }
    else 
    {

        if (iRand == 0)
        {
            iRand = THROW_PROJ_START;
        }
        else if (iRand == 1)
        {
            iRand = RISE_START;
        }
        else if (iRand == 2)
        {
            iRand = BOW;

        }
    }
    //else if (iRand == 4)
    //{
    //    iRand = JUMP_START;

    //}
    m_eState = MOVE_FORWARD;
    m_eNextState = (STATE)iRand;
    m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE_FORWARD], true);
    //m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
    m_iAttackCount = 0;
    m_iMoveCounter = 0;
    m_bChargeFlag = false;
    m_bPush = false;
    m_bFlag = false;
}

void CMonster_Gullveig::Anim_Finished()
{
    string strCurTag = m_pModelCom->Get_Current_Animation_Tag();
    //_uint iRand = rand() % 3 + 1;
    //_wstring Talk = L"GullveigTalk";
    //m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, Talk + to_wstring(iRand));
    switch (m_eState)
    {
    case Client::CMonster_Gullveig::START:
    {
        To_Idle();
    }
    break;
    case Client::CMonster_Gullveig::IDLE:
        break;
    case Client::CMonster_Gullveig::MOVE_FORWARD:
    case Client::CMonster_Gullveig::MOVE_BACKWARD:
    case Client::CMonster_Gullveig::MOVE_LEFT:
    case Client::CMonster_Gullveig::MOVE_RIGHT:
    {
        m_eState = m_eNextState;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
        m_fLeftTrailTimer = 2.5f;
        m_fRightTrailTimer = 2.5f;
        //m_pLeftTrail->Push_From_Pool(nullptr);
        //m_pRightTrail->Push_From_Pool(nullptr);
        //m_eState = MOVE_END;
        //m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE_END], false);
    }
    break;
    case THROW_PROJ_START:
    {
        m_eState = THROW_PROJ;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
    break;
    case THROW_PROJ:
    {
        To_Idle();
    }
    break;
    case RISE_START:
    {
        m_eState = RISE;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
    break;
    case RISE:
    {
        To_Idle();
    }
    break;
    case DAGGER:
    {
        To_Idle();
    }
    break;
    case BOW:
    {
        To_Idle();
    }
    break;   
    //case JUMP_START:
    //{
    //    m_eState = JUMP_END;
    //    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    //}
    //break;
    //case JUMP_END:
    //{
    //    To_Idle();
    //}
    //break;
    case Client::CMonster_Gullveig::DYING:
    {
        _float4 vDest = m_vStartPos;
        _float4x4 PortWorld;
        XMStoreFloat4x4(&PortWorld, XMMatrixTranslationFromVector(XMLoadFloat4(&m_vStartPos) + XMVectorSet(0.f, -10.f, 0.f, 0.f)));
        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Boss_Portal", m_pGameInstance->Get_Current_LevelID(), L"Portal", &vDest, &PortWorld);
        Dead();
    }
    break;
    default:
        break;
    }

}

CMonster_Gullveig* CMonster_Gullveig::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster_Gullveig* pInstance = new CMonster_Gullveig(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMonster_Gullveig"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster_Gullveig::Clone(void* pArg)
{
    CMonster_Gullveig* pInstance = new CMonster_Gullveig(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMonster_Gullveig"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_Gullveig::Free()
{
    Safe_Release(m_pCCTCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pHpBar);
    //Safe_Release(m_pLeftTrail);
    //Safe_Release(m_pRightTrail);
    __super::Free();
}
