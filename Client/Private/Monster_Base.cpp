#include "Monster_Base.h"
#include "GameInstance.h"
#include "Beat_Manager.h"
#include "Player_Manager.h"
#include "RoomManager.h"
#include "Room.h"


CMonster_Base::CMonster_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
    CGameObject(pDevice,pContext)
{
    m_pBeatManager = CBeat_Manager::Get_Instance();
    m_pPlayerManager = CPlayer_Manager::Get_Instance();
}

CMonster_Base::CMonster_Base(const CMonster_Base& rhs):
    CGameObject(rhs), m_pBeatManager(rhs.m_pBeatManager), m_pPlayerManager(rhs.m_pPlayerManager)
{
    m_iCollisionGroup = CG_MONSTER;
}

HRESULT CMonster_Base::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CMonster_Base::Initialize(void* pArg)
{
    __super::Initialize(nullptr);
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
    }

    return S_OK;
}

void CMonster_Base::PriorityTick(_float fTimeDelta)
{

}

void CMonster_Base::Tick(_float fTimeDelta)
{

}

void CMonster_Base::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMonster_Base::Render()
{

    return S_OK;
}

void CMonster_Base::Dying()
{

}

void CMonster_Base::Push_From_Pool(void* pArg)
{
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
    }
}

void CMonster_Base::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
{
    if (m_iCollisionGroup == CG_MONSTER_CONTACT)
    {
        if (pOther)
        {
            if (pOther->Get_CollisionGroup() == CG_PLAYER)
            {
                _float3 vHitPos(ControllerHit.worldPos.x ,ControllerHit.worldPos.y,ControllerHit.worldPos.z );
                pOther->Take_Damage(25.f, vHitPos, DAMAGE_TYPE::DMG_DEFAULT, this);
            }
        }
    }
}

void CMonster_Base::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
{

}

void CMonster_Base::Drop_Coin()
{
    if (rand() % 100 <= m_fCoin * 100.f)
    {
        auto pCoin = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Item_Consume_Coin", nullptr);
        pCoin->Get_Transform()->Set_Position(m_pTransformCom->Get_Position() + XMVectorSet(0.f,1.5f,0.f,0.f));
        auto pRM = CRoomManager::Get_Instance();
        pRM->Get_Room(pRM->Get_CurrentRoomIdx())->Add_Object(pCoin);
        Safe_Release(pCoin);
    }
}

void CMonster_Base::Hit_ChangeColor(_float fTimeDelta)
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
        m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Default;
    }

}

CMonster_Base* CMonster_Base::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster_Base* pInstance = new CMonster_Base(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMonster_Base"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster_Base::Clone(void* pArg)
{
    CMonster_Base* pInstance = new CMonster_Base(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMonster_Base"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_Base::Free()
{
    __super::Free();
}
