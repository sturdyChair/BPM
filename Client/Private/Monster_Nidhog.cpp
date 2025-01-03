#include "Monster_Nidhog.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "CharacterController.h"
#include "Beat_Manager.h"
#include "Player.h"
#include "Player_Manager.h"
#include "Effect_Charge.h"
#include "Effect_Sweep.h"
#include "Effect_SummonRune.h"
#include "Effect_RuneSpear.h"
#include "UI_BossHP.h"
#include "Statue_Gimmick.h"
#include "Effect_NidhoggField.h"
#include "Effect_LightningArc.h"
#include "Effect_Teardrop.h"
//9,0,-15 석상 위치
CMonster_Nidhog::CMonster_Nidhog(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CMonster_Base(pDevice, pContext)
{
}

CMonster_Nidhog::CMonster_Nidhog(const CMonster_Nidhog& rhs) :
    CMonster_Base(rhs)
{
    m_iCollisionGroup = CG_MONSTER_CONTACT;
}

HRESULT CMonster_Nidhog::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CMonster_Nidhog::Initialize(void* pArg)
{
    CTransform::TRANSFORM_DESC transDesc;
    transDesc.fRotationPerSec = XM_PI;
    __super::Initialize(pArg);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Nidhog", L"Model", (CComponent**)&m_pModelCom);
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
    cctDesc.height = 2.0f;
    cctDesc.radius = 2.0f;
    cctDesc.userData = this;
    _float3 vPos;
    XMStoreFloat3(&vPos, m_pTransformCom->Get_Position());
    cctDesc.position = PxExtendedVec3(vPos.x, vPos.y, vPos.z);
    cctDesc.material = m_pGameInstance->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_CharacterController", L"Com_CCT", (CComponent**)&m_pCCTCom, &cctDesc);
    m_pCCTCom->m_bGravity = false;
    m_pCCTCom->m_bUseVelocity_Calculation = true;
    m_pCCTCom->m_fFriction = 20.f;
    //WYVERN.ao|WYVERN_flyStationary
    //WYVERN.ao|WYVERN_fly
    //WYVERN.ao|WYVERN_flyStationarySpitFireball
    //WYVERN.ao|WYVERN_flyStationaryRoar
    //WYVERN.ao|WYVERN_flyStationaryToLanding
    //WYVERN.ao|WYVERN_hitTheGroundDeath
    //WYVERN.ao|WYVERN_idle
    //WYVERN.ao|WYVERN_spreadFire
    //WYVERN.ao|WYVERN_stingerAttack
    //WYVERN.ao|WYVERN_walk
    //WYVERN.ao|WYVERN_death
    m_strAnimTag[FLY_STATIONARTY] = "WYVERN.ao|WYVERN_flyStationary";
    m_strAnimTag[FLY_MOVE] = "WYVERN.ao|WYVERN_fly";
    m_strAnimTag[FLY_SPITFIRE] = "WYVERN.ao|WYVERN_flyStationarySpitFireball";
    m_strAnimTag[FLY_ROAR] = "WYVERN.ao|WYVERN_flyStationaryRoar";
    m_strAnimTag[FLY_TO_LAND] = "WYVERN.ao|WYVERN_flyStationaryToLanding";
    m_strAnimTag[FLY_TO_DIE] = "WYVERN.ao|WYVERN_hitTheGroundDeath";
    m_strAnimTag[IDLE] = "WYVERN.ao|WYVERN_idle";
    m_strAnimTag[SPREAD_FIRE] = "WYVERN.ao|WYVERN_spreadFire";
    m_strAnimTag[STING_TAIL] = "WYVERN.ao|WYVERN_stingerAttack";
    m_strAnimTag[WALK] = "WYVERN.ao|WYVERN_walk";
    m_strAnimTag[DEATH] = "WYVERN.ao|WYVERN_death";


    m_eState = FLY_STATIONARTY;
    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    m_fTimer = m_pBeatManager->Get_Beat();
    m_ProjectileDesc.eCG = CG_MONSTER_PROJECTILE;
    m_fHp = 5000.f;
    m_fMaxHp = m_fHp;
    m_pHpBar = (CUI_BossHP*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_BossHP", nullptr);
    m_pHpBar->Set_MaxHp(m_fMaxHp);
    m_pHpBar->Set_Hp(m_fHp);
    m_pGimmick = CStatue_Gimmick::Create(this);
    m_pStatueField = (CEffect_NidhoggField*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_NidhoggField", nullptr);
    
    return S_OK;
}

void CMonster_Nidhog::PriorityTick(_float fTimeDelta)
{
    m_iPrevBitCount = m_iCurrBitCount;
    m_iCurrBitCount = m_pBeatManager->Get_BeatCount();
    if (m_iPrevBitCount != m_iCurrBitCount)
    {
        --m_iMoveCount;
    }
    m_pHpBar->PriorityTick(fTimeDelta);
    if (m_bStatue)
    {
        m_pGimmick->PriorityTick(fTimeDelta);
        m_pStatueField->PriorityTick(fTimeDelta);
    }
    if (m_fInvincible > 0.001f)
    {
        m_fInvincible -= fTimeDelta;
    }
}

void CMonster_Nidhog::Tick(_float fTimeDelta)
{
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
    m_pModelCom->Play_Animation(fTimeDelta * 1.5f, m_fAnimFrom, m_fAnimTo);
    _float fCTR = m_pModelCom->Get_Current_Animation_TrackRatio();
    auto pPlayer = m_pPlayerManager->Get_Player();
    switch (m_eState)
    {
    case Client::CMonster_Nidhog::FLY_STATIONARTY:
    {
        if (m_iMoveCount <= 0)
        {
            Select_Movement();
        }
    }
        break;
    case Client::CMonster_Nidhog::FLY_MOVE:
    {
        if (m_bDestMid)
        {
            _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
            _vector vDir = XMLoadFloat3(&m_vDestMid) - m_pTransformCom->Get_Position();
            _vector vLDDif = XMVectorSetY(vLook - vDir, 0.f);
            _float fAngle = atan2(XMVectorGetZ(vLDDif), XMVectorGetX(vLDDif));
            if (abs(fAngle) > 0.05f)
            {
                m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * XM_PI);
            }
            else
            {
                m_pTransformCom->LookAt_Horizontal(XMLoadFloat3(&m_vDestMid));
            }

            if (XMVectorGetY(vDir) < -0.01f)
            {
                m_pCCTCom->Move_Down(m_fSpeed);
            }
            else if (XMVectorGetY(vDir) > 0.01f)
            {
                m_pCCTCom->Move_Up(m_fSpeed);
            }
            m_pCCTCom->Move_Forward(m_fSpeed);

            if (XMVectorGetX(XMVector3Length(XMVectorSetY(vDir, 0.f))) < 0.1f || m_iMoveCount <= 20)
            {
                m_bDestMid = false;
            }

        }
        else
        {
            _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
            _vector vDir = XMLoadFloat3(&m_vDestEnd) - m_pTransformCom->Get_Position();
            _vector vLDDif = XMVectorSetY(vLook - vDir, 0.f);

            _float fAngle = atan2(XMVectorGetZ(vLDDif), XMVectorGetX(vLDDif));
            if (abs(fAngle) > 0.05f)
            {
                m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * XM_PI);
            }
            else
                m_pTransformCom->LookAt_Horizontal(XMLoadFloat3(&m_vDestEnd) );


            if (XMVectorGetY(vDir) < -0.01f)
            {
                m_pCCTCom->Move_Down(m_fSpeed);
            }
            else if (XMVectorGetY(vDir) > 0.01f)
            {
                m_pCCTCom->Move_Up(m_fSpeed);
            }
            m_pCCTCom->Move_Forward(m_fSpeed);

            if (XMVectorGetX(XMVector3Length(XMVectorSetY(vDir, 0.f))) < 1.0f || m_iMoveCount <= 0)
            {
                if (m_iAttackCount > 0)
                {
                    --m_iAttackCount;
                    m_vDestMid = pPlayer->Get_FootPosition();
                    _vector vPos = m_pTransformCom->Get_Position();
                    XMStoreFloat3(&m_vDestEnd, XMVectorSetY((vPos + (XMLoadFloat3(&m_vDestMid) - vPos) * 2.f), XMVectorGetY(vPos)));
                    m_bDestMid = true;
                }
                else
                {
                    m_pModelCom->Set_AnimTag(m_strAnimTag[FLY_MOVE], false);
                }
            }
        }

    }
        break;
    case Client::CMonster_Nidhog::FLY_SPITFIRE://0.5f
    {
        if (m_iAttackCount > 0 && fCTR >= 0.5f)
        {
            --m_iAttackCount;
            _float4x4 Mat;
            XMStoreFloat4x4(&Mat, m_pModelCom->Get_BoneMatrix(m_strMouth) * m_pTransformCom->Get_WorldMatrix_XMMat());
            m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Space", m_pGameInstance->Get_Current_LevelID(), L"Effect", nullptr, &Mat);
        }
    }
        break;
    case Client::CMonster_Nidhog::FLY_ROAR:
    {
        if (m_iAttackCount > 0 && fCTR >= 0.09f * (11 - m_iAttackCount))
        {
            --m_iAttackCount;
            _float4x4 Mat;
            _matrix Target = pPlayer->Get_Transform()->Get_WorldMatrix_XMMat();
            CEffect_SummonRune::CHARGE_DESC desc;
            desc.fLifeTime = 1.f;
            _vector vRight = XMVectorSet(fRand(-10.f,10.f), fRand(5.f,20.f), fRand(-10.f,10.f), 0.f) + Target.r[2] * 20.f;
            Target.r[3] += vRight;
            XMStoreFloat4x4(&Mat, XMMatrixScaling(3.f,3.f,3.f) * Target);
            m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_SummonRune", m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &Mat);

        }
    }
        break;
    case Client::CMonster_Nidhog::FLY_TO_LAND:
    {
        m_pCCTCom->Move_Forward(10.f);
        m_pCCTCom->m_vVelocity.y = -10.f;
    }
        break;
    case Client::CMonster_Nidhog::FLY_TO_DIE:
        break;
    case Client::CMonster_Nidhog::IDLE:
    {
        if (m_iMoveCount <= 0)
        {
            Select_Movement();
        }
    }
        break;
    case Client::CMonster_Nidhog::SPREAD_FIRE:
    {
        if (m_iAttackCount > 0 && fCTR >= 0.174f + 0.75f * 0.033f * (30 - m_iAttackCount))
        {
            --m_iAttackCount;
            _float4x4 Mat;

            //CEffect_SummonRune::CHARGE_DESC desc;
            //desc.fLifeTime = m_pBeatManager->Get_Beat() * 3.f;
            //_float fRandAngleYaw = rand() % 100 * 0.01f * XM_PI;
            //_float fRandAnglePitch = rand() % 100 * 0.01f * XM_PI;
            //_float fRandAngleRoll = rand() % 100 * 0.01f * XM_PI;
            //_vector vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
            //vRight = XMVector3Rotate(vRight, XMQuaternionRotationRollPitchYaw(fRandAnglePitch, -fRandAngleYaw, fRandAngleRoll));
            //Target.r[3] += vRight * 10.f;
            //XMStoreFloat4x4(&Mat, Target);
            //m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_SummonRune", m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &Mat);
            CEffect_Teardrop::PROJECTILE_DESC prodesc;
            prodesc.eCG = CG_MONSTER_PROJECTILE;
            _matrix ProjectileMat = XMMatrixRotationZ(-XM_PIDIV2)*  m_pModelCom->Get_BoneMatrix(m_strMouth)  * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix());
            _vector vPosition = ProjectileMat.r[3];
            XMStoreFloat3(&prodesc.vDirection, XMVector3Normalize(ProjectileMat.r[2]) * 26.f);
            ProjectileMat.r[3] += XMLoadFloat3(&prodesc.vDirection) * 0.1f;
            XMStoreFloat4x4(&Mat, ProjectileMat);
            m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Teardrop", m_pGameInstance->Get_Current_LevelID(), L"Effect",
                &prodesc, &Mat);


            _matrix Target = pPlayer->Get_Transform()->Get_WorldMatrix_XMMat();

            for(_uint i = 0;i<4;++i)
            {
                _vector vRand = XMVector3Normalize(XMVectorSet(fRand(-1.f, 1.f), 0.f, fRand(-1.f, 1.f), 0.f));
                Target.r[3] += vRand * fRand(3.f, 7.f);
                XMStoreFloat4x4(&Mat, Target);
                CEffect_LightningArc::LIGHRNIG_DESC LTDesc{};
                LTDesc.fLifeTime = fRand(0.2f, 0.4f);
                _float RandScale = (_float)(30 - m_iAttackCount) + fRand(0.f, 3.f);
                LTDesc.vScale = _float3{ RandScale * 0.25f, RandScale , 1.f };
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_LightningArc", m_pGameInstance->Get_Current_LevelID(), L"Effect",
                    &LTDesc, &Mat);
            }
            m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Projectile");
        }
    }
        break;
    case Client::CMonster_Nidhog::STING_TAIL:
    {
        if (m_iAttackCount > 0 && fCTR >= 0.375f)
        {
            --m_iAttackCount;
            _matrix Projectile = m_pModelCom->Get_BoneMatrix(m_strTail) * m_pTransformCom->Get_WorldMatrix_XMMat();
            _float4x4 temp;
            //
            CEffect_RuneSpear::PROJECTILE_DESC desc;
            desc.eCG = CG_MONSTER_PROJECTILE;
            _vector vPos = m_pTransformCom->Get_Position();
            _vector vTarget = pPlayer->Get_Transform()->Get_Position();
            _vector vDir = XMVector3Normalize(vTarget - vPos) * 20.f;
            _vector vRand = XMVector3Normalize(XMVectorSet(fRand(-1.f,1.f), fRand(-1.f,1.f), fRand(-1.f,1.f), 0.f)) * 10.f;

            XMStoreFloat3(&desc.vDirection, vDir + vRand);
            XMStoreFloat4x4(&temp, Projectile);
            m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_RuneSpear", m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &temp);
            for (_uint i = 1; i < 12; ++i)
            {
                vRand = XMVector3Normalize(XMVectorSet(fRand(-1.f, 1.f), fRand(-1.f, 1.f), fRand(-1.f, 1.f), 0.f)) * 10.f;
                Projectile = m_pModelCom->Get_BoneMatrix(m_strTail + to_string(i)) * m_pTransformCom->Get_WorldMatrix_XMMat();
                XMStoreFloat3(&desc.vDirection, vDir + vRand);
                XMStoreFloat4x4(&temp, Projectile);
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_RuneSpear", m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &temp);
            }
        }
    }
        break;
    case Client::CMonster_Nidhog::WALK:
        break;
    case Client::CMonster_Nidhog::DEATH:
    {
        if (m_pModelCom->Get_Finished())
        {
            Dead();
        }
        pPlayer->NoMiss(true);
    }
        break;
    default:
        break;
    }

    if (m_pModelCom->Get_Finished())
    {
        if (m_eState == FLY_TO_LAND)
        {
            m_bFlying = false;
        }
        To_Idle();
    }
    if (!m_bFlying)
    {
        m_pTransformCom->LookAt_Horizontal(pPlayer->Get_Transform()->Get_Position());
    }

    m_pCCTCom->Update(fTimeDelta);
    auto pRawController = m_pCCTCom->Get_Controller();
    if (pRawController && m_eState != FLY_MOVE)
    {
        auto pxvFP = pRawController->getFootPosition();
        m_pTransformCom->Set_Position(XMVectorSet(pxvFP.x, pxvFP.y, pxvFP.z, 1.f));
    }
    m_pHpBar->Set_Hp(m_fHp);
    m_pHpBar->Tick(fTimeDelta);
    if (m_bStatue)
    {
        m_pGimmick->Tick(fTimeDelta);
        m_pStatueField->Tick(fTimeDelta);
        m_pStatueField->Get_Transform()->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix_XMMat());
    }
}

void CMonster_Nidhog::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
    m_pHpBar->LateTick(fTimeDelta);
    if (m_bStatue)
    {
        m_pGimmick->LateTick(fTimeDelta);
        m_pStatueField->LateTick(fTimeDelta);
    }
}

HRESULT CMonster_Nidhog::Render()
{
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
    m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
    _float4 vColor = { 1.0f,0.8f,0.3f,1.f };
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

void CMonster_Nidhog::Dying()
{
    if (m_pCCTCom)
        m_pCCTCom->Disable();
}

void CMonster_Nidhog::Push_From_Pool(void* pArg)
{
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pCCTCom->Set_FootPosition(XMLoadFloat4x4(&pDesc->Transformation));
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
        m_iCurrBitCount = m_pBeatManager->Get_BeatCount();
        m_iPrevBitCount = m_iCurrBitCount;
    }
}

void CMonster_Nidhog::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
{
    if (m_bKilled)
        return;
    if (m_bStatue)
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
    m_fTakeHit = 0.2f;
    if (m_eState == FLY_TO_LAND)
    {
        return;
    }
    if (m_fHp <= 0.f && !m_bKilled)
    {
        _wstring Chord = L"Nidhog_Chord";
        m_pGameInstance->Stop_Sound(SOUND_CHANNEL::MUSIC, Chord + to_wstring(m_iFinish));
        ++m_iFinish;
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, Chord + to_wstring(m_iFinish));
        m_pModelCom->Set_AnimTag(m_strAnimTag[DEATH], false);
        m_fAnimFrom = 0.111f * m_iFinish;
        m_fAnimTo = 0.111f * (m_iFinish + 1);
        if (m_iFinish >= 8)
        {
            m_pModelCom->Set_AnimTag(m_strAnimTag[DEATH], false);
            m_fAnimFrom = 0.888f;
            m_fAnimTo = 1.f;
            m_bKilled = true;
            m_pPlayerManager->Get_Player()->WhiteOut();
        }
        return;
    }
    
    m_fHp -= fAmount;


    if (m_fHp <= 0.f)
    {
        m_pModelCom->Set_AnimTag(m_strAnimTag[DEATH], false);
        m_fAnimFrom = 0.f;
        m_fAnimTo = 0.111f;
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Nidhog_Death");
        m_eState = DEATH;
        //m_pBeatManager->Queue_Sequence(L"Asgard_CoolDown");
        //m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, L"Draugr_Chord0");
        m_pBeatManager->Queue_Sequence(L"Asgard_CoolDown");
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, L"Nidhog_Chord0");
        m_iFinish = 0;
    }
}

void CMonster_Nidhog::Kill_Statue()
{
    m_bStatue = false;
}

void CMonster_Nidhog::To_Idle()
{
    if (m_bFlying)
    {
        m_eState = FLY_STATIONARTY;
        m_pModelCom->Set_AnimTag(m_strAnimTag[FLY_STATIONARTY], true);
    }
    else
    {
        m_eState = IDLE;
        m_pModelCom->Set_AnimTag(m_strAnimTag[IDLE], true);
    }
    m_iMoveCount = 4;
    //m_fTimer = m_pBeatManager->Get_Beat() * 4.f;
    m_fAnimFrom = 0.f;
    m_fAnimTo = 1.f;
}

void CMonster_Nidhog::Select_Movement()
{
    if (m_bFlying)
    {
        _uint iRand = rand() % 2;
/*        if (iRand == 0)
        {
            m_eState = FLY_MOVE;
        }
        else */
        if (iRand == 1)
        {
            m_eState = FLY_SPITFIRE;
        }
        else
        {
            m_eState = FLY_ROAR;
        }


        if (m_fHp <= 2500.f)
        {
            m_eState = FLY_TO_LAND;
            m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
            m_pCCTCom->m_bGravity = true;
            m_bStatue = true;
        }
    }
    else
    {
        _uint iRand = rand() % 2;
        if (iRand)
        {
            m_eState = SPREAD_FIRE;
        }
        else
        {
            m_eState = STING_TAIL;
        }
        
    }
    switch (m_eState)
    {
    case Client::CMonster_Nidhog::FLY_MOVE:
        Move();
        break;
    case Client::CMonster_Nidhog::FLY_SPITFIRE:
        Spit_Space();
        break;
    case Client::CMonster_Nidhog::FLY_ROAR:
        Roar();
        break;
    case Client::CMonster_Nidhog::SPREAD_FIRE:
        Spread_Fire();
        break;
    case Client::CMonster_Nidhog::STING_TAIL:
        Sting_Tail();
        break;
    default:
        break;
    }
}

void CMonster_Nidhog::Spit_Space()
{
    m_eState = FLY_SPITFIRE;
    m_pModelCom->Set_AnimTag(m_strAnimTag[FLY_SPITFIRE], false);
    m_iAttackCount = 1;
    m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Nidhog_Birth");
}

void CMonster_Nidhog::Roar()
{
    m_eState = FLY_ROAR;
    m_pModelCom->Set_AnimTag(m_strAnimTag[FLY_ROAR], false);
    m_iAttackCount = 10;
    m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Nidhog_Spread");
}

void CMonster_Nidhog::Move()
{
    m_eState = FLY_MOVE;
    m_pModelCom->Set_AnimTag(m_strAnimTag[FLY_MOVE], true);
    m_iAttackCount = 4;
    m_iMoveCount = 40;
    m_vDestMid = m_pPlayerManager->Get_Player()->Get_FootPosition();
    _vector vPos = m_pTransformCom->Get_Position();
    XMStoreFloat3(&m_vDestEnd, XMVectorSetY((vPos + (XMLoadFloat3(&m_vDestMid) - vPos) * 2.f), XMVectorGetY(vPos)));
    m_bDestMid = true;

}

void CMonster_Nidhog::Land()
{
    m_pCCTCom->m_bGravity = true;
    m_bFlying = false;
    m_eState = FLY_TO_LAND;
    m_pModelCom->Set_AnimTag(m_strAnimTag[FLY_TO_LAND], false);
}

void CMonster_Nidhog::Spread_Fire()
{
    //0.174
    m_eState = SPREAD_FIRE;
    m_pModelCom->Set_AnimTag(m_strAnimTag[SPREAD_FIRE], false);
    m_iAttackCount = 30;
    m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Nidhog_Spread");
}

void CMonster_Nidhog::Sting_Tail()
{
    //0.375
    m_eState = STING_TAIL;
    m_pModelCom->Set_AnimTag(m_strAnimTag[STING_TAIL], false);
    m_iAttackCount = 1;
    m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Nidhog_Birth");
}



void CMonster_Nidhog::State_Change()
{
    //_int iRand = rand() % (_int)STATE_END;
    //while (iRand == STATE::MOVE || iRand == STATE::IDLE || iRand == STATE::DYING || iRand == START)
    //{
    //    iRand = rand() % (_int)STATE_END;
    //}
    //m_eState = (STATE)iRand;
    //m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE], false);
    //auto pPlayer = m_pPlayerManager->Get_Player();
    //_vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
    //_vector vPosition = m_pTransformCom->Get_Position();
    //XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(vPlayerPosition - vPosition) * 20.f);
    //m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
    //m_iAttackCount = 0;
    //m_bChargeFlag = false;
    //m_bPush = false;
}

CMonster_Nidhog* CMonster_Nidhog::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster_Nidhog* pInstance = new CMonster_Nidhog(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMonster_Nidhog"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster_Nidhog::Clone(void* pArg)
{
    CMonster_Nidhog* pInstance = new CMonster_Nidhog(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMonster_Nidhog"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_Nidhog::Free()
{
    Safe_Release(m_pStatueField);
    Safe_Release(m_pCCTCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pHpBar);
    Safe_Release(m_pGimmick);
    __super::Free();
}
