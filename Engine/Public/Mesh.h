#pragma once

#include "VIBuffer.h"

BEGIN(Engine)
class CBone;

class ENGINE_DLL CMesh final : public CVIBuffer
{
public:
	struct MESH_DESC
	{
		_int iMaterial_Index;
		_uint iNumBones = 0;
		_uint* ArrBones = nullptr;
		string m_strName;
	};

private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	virtual HRESULT Initialize_Prototype(const vector<VTXMESH>& vecVerticies, const vector<_uint>& vecIndicies, const MESH_DESC& desc);
	virtual HRESULT Initialize_Prototype(const vector<VTXANIMMESH>& vecVerticies, const vector<_uint>& vecIndicies, const vector<_float4x4>& vecOffset, const vector<CBone*>& Bones, const MESH_DESC& desc);
	virtual HRESULT Initialize(void* pArg) override;
	_int	Get_Material_Index() const { return m_iMaterial_Index; }
	const _float3& Get_Min() const { return m_vMin; }
	const _float3& Get_Max() const { return m_vMax; }

private:
	string m_strName;
	_int   m_iMaterial_Index = 0;
	_uint  m_iNumBones = 0;
	vector<_uint> m_Bones;
	vector<_float4x4> m_BoneOffset;

	_float3 m_vMin;
	_float3 m_vMax;

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const vector<VTXMESH>& vecVerticies, const vector<_uint>& vecIndicies, const MESH_DESC& desc);
	static CMesh* Create(ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext, const vector<VTXANIMMESH>& vecVerticies, const vector<_uint>& vecIndicies, const vector<_float4x4>& vecOffset, const vector<CBone*>& Bones, const MESH_DESC& desc);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

	friend class CModel;
};

END