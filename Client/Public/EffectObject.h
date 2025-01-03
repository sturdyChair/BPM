
#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Beat_Manager.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CEffectObject : public CGameObject
{
public:
	class EFFECT_DESC
	{
	public:
	};

	static void Set_AmbientColor(_float4 vColor) { s_vAmbientColor = vColor; }

protected:
	CEffectObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffectObject(const CEffectObject& rhs);
	virtual ~CEffectObject() = default;
public:
	static _float4 s_vAmbientColor;

public:
	virtual void Free() override;
};

END