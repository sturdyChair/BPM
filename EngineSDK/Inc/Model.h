#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Animation.h"

BEGIN(Engine)

class ENGINE_DLL CModel : public CComponent
{
public:
	enum MODEL_TYPE
	{
		NONANIM,
		ANIM,
	};

	struct BONE_NODE
	{
		string strTag;


	};

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT Initialize_Prototype(const _char* pModelPath, _fmatrix TransformMatrix, const string& strTextureDirectory = "");
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint iMeshIndex);
	virtual HRESULT Render_MeshGroup(_uint iGroupIndex);


	_bool Get_Finished() {
		return m_Animations[m_iCurrentAnimIndex]->Get_Finished();
	}
	_bool Get_Departed_Finished()
	{
		return m_Animations[m_iCurrentAnimIndex_Departed]->Get_Finished();
	}

	void Set_AnimIndex(_uint iAnimIndex, _bool isLoop) {
		m_fPrevTrackPosition = m_Animations[m_iCurrentAnimIndex]->Get_CurrentTrackPosition();
		m_Animations[m_iCurrentAnimIndex]->Reset();
		m_iPrevAnimIndex = m_iCurrentAnimIndex;
		m_iCurrentAnimIndex = iAnimIndex;
		m_isLoop = isLoop;

	}
	void Set_AnimTag(const string& strTag, _bool isLoop)
	{
		m_fPrevTrackPosition = m_Animations[m_iCurrentAnimIndex]->Get_CurrentTrackPosition();
		m_Animations[m_iCurrentAnimIndex]->Reset();
		m_iPrevAnimIndex = m_iCurrentAnimIndex;
		m_iCurrentAnimIndex = m_AnimTagIndex[strTag];
		m_isLoop = isLoop;
	}

	void Set_DepartedAnimTag(const string& strTag, _bool isLoop)
	{
		m_fPrevTrackPosition_Departed = m_Animations[m_iCurrentAnimIndex_Departed]->Get_CurrentTrackPosition();
		m_Animations[m_iCurrentAnimIndex_Departed]->Reset();
		m_iPrevAnimIndex_Departed = m_iCurrentAnimIndex_Departed;
		m_iCurrentAnimIndex_Departed = m_AnimTagIndex[strTag];
		m_isLoop_Departed = isLoop;
	}


	void Set_AnimTPS(_uint iIdx, _float fTPS);
	void Set_AnimTPS(const string& strTag, _float fTPS);

	_float Get_AnimTPS(_uint iIdx);
	_float Get_AnimTPS(const string& strTag);
public:
	_uint Get_NumMesh() const { return m_iNumMesh; }
	const _float3& Get_Min(_uint iMeshIdx)const;
	const _float3& Get_Max(_uint iMeshIdx)const;
	
	HRESULT Ready_Meshes(const vector<VTXMESH>& vecVerticies, const vector<_uint>& vecIndicies, const CMesh::MESH_DESC& desc);

	HRESULT Ready_Meshes(const vector<VTXANIMMESH>& vecVerticies, const vector<_uint>& vecIndicies,const vector<_float4x4>& vecOffset, const CMesh::MESH_DESC& desc);

	HRESULT Read_Model(const string& strPath, _fmatrix TransformMatrix, const string& strTextureDirectory = "");

	HRESULT Read(ifstream& in, vector<VTXMESH>& vecVerticies, vector<_uint>& vecIndicies, _fmatrix TransformMatrix, const string& strTexturePath);
	HRESULT ReadBin(ifstream& in, vector<VTXMESH>& vecVerticies, vector<_uint>& vecIndicies, _fmatrix TransformMatrix, const string& strTexturePath);
	HRESULT Read(ifstream& in, vector<VTXANIMMESH>& vecVerticies, vector<_uint>& vecIndicies, _fmatrix TransformMatrix, const string& strTexturePath);
	HRESULT ReadBin(ifstream& in, vector<VTXANIMMESH>& vecVerticies, vector<_uint>& vecIndicies, _fmatrix TransformMatrix, const string& strTexturePath);

	HRESULT Ready_Bones(ifstream& in);
	HRESULT Ready_Bones_Bin(ifstream& in);

	const vector<class CTexture*>& Get_Material(_int iMeshIndex);
	
	HRESULT Make_Shape(const _wstring& strTag, _float fScale = 1.f);

	HRESULT Make_PxRigidStatic(const _wstring& strTag, _fmatrix MatTransform , _float fScale = 1.f);

	void	 Remove_Actor();
	void	 Add_Actor(_fmatrix Transform = XMMatrixIdentity());
	void	Disable_Simulation(bool flag = true);

	PxRigidStatic* Get_RigidActor() { return m_pRigidStatic; }

	void	Play_Animation(_float fTimeDelta, _float fFrom = 0.f, _float fTo = 1.f, _bool bLerp = true);
	
	void	Play_Animation_Departed(_float fTimeDelta, _float fFrom = 0.f, _float fTo = 1.f, _bool bLerp = true);

	void	Blend_Animation(_float fTimeDelta, string strTag, _float fRatio);
	void	Blend_Animation(_float fTimeDelta, _uint iIdx, _float fRatio);
	void	Reset_Animation(string strTag);
	void	Reset_Animation(_uint iIdx);

	void	Bind_BoneMatrices(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex);
	void	Get_BoneMatrices(vector<_float4x4>& vecOut, _uint iMeshIndex);

	void	Inverse_Bones();
	void	Inverse_BoneOffsets();
	void	Inverse_BoneTransform();
	
	_fmatrix Get_BoneMatrix(string strTag);
	_fmatrix Get_BoneMatrix(_uint iIndex);
	_float4x4* Get_BoneMatrixPtr(string strTag);
	_float4x4* Get_BoneMatrixPtr(_uint iIndex);

	const string& Get_Current_Animation_Tag() { return m_Animations[m_iCurrentAnimIndex]->Get_Name(); }
	_float Get_Current_Animation_TrackRatio() { return m_Animations[m_iCurrentAnimIndex]->Get_CurrentTrackPositionRatio(); }

	void Make_Mesh_Material_Group();
	const vector<_uint>& Get_MaterialGroup(_uint iGroupIdx);
	const vector<CMesh*>& Get_MeshGroup(_uint iGroupIdx);
	_uint Get_GroupSize();
	
	void Get_BoneChildren(string strTag, vector<_int>& vecOut);

	void Add_DepartedBoneTag(string strTag);
	void Add_DepartedBoneTag_Child(string strTag);
	void Add_DepartedBoneIdx_Child(_uint iIdx);
private:
	void Hash_Animation();
	void Hash_Bone();

private:
	_uint  m_iNumMesh = 0;
	_uint  m_iNumBones = 0;
	_uint  m_iNumAnimations = 0;
	_uint  m_iCurrentAnimIndex = 0;
	_uint  m_iPrevAnimIndex = 0;
	_float m_fPrevTrackPosition = 0.f;
	_bool  m_isLoop = false;
	
	_uint  m_iCurrentAnimIndex_Departed = 0;
	_uint  m_iPrevAnimIndex_Departed = 0;
	_float m_fPrevTrackPosition_Departed = 0.f;
	_bool  m_isLoop_Departed = false;
	vector<_int> m_vecDepartedBoneIdx;

	vector<CMesh*> m_Meshes;
	map<_wstring, class CTexture*> m_mapMaterial;
	vector<vector<class CTexture*>> m_vecMaterials;

	vector<vector<CMesh*>> m_MeshGroup;
	vector<vector<_uint>> m_vecMaterialGroup;

	vector<class CBone*> m_Bones;
	vector<CAnimation*> m_Animations;
	

	_wstring m_strShapeTag = L"";
	PxRigidStatic* m_pRigidStatic = nullptr;
	_float4x4		m_TransformMatrix{};

	MODEL_TYPE m_eModelType = NONANIM;
	unordered_map<string, _uint> m_AnimTagIndex;
	unordered_map<string, _uint> m_BoneTagIndex;



public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelPath,_fmatrix TransformMatrix = XMMatrixIdentity(), const string& strTextureDirectory = "");
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END