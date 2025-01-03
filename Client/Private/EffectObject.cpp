#include "EffectObject.h"

_float4 CEffectObject::s_vAmbientColor = {};

CEffectObject::CEffectObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	CGameObject(pDevice,pContext)
{
}

CEffectObject::CEffectObject(const CEffectObject& rhs):
	CGameObject(rhs)
{
}

void CEffectObject::Free()
{
	__super::Free();
}
