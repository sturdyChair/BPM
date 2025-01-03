#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define BEGIN(NAMESPACE) namespace NAMESPACE { 
#define END }


#define MSG_BOX(message)								\
::MessageBox(nullptr, message, L"System Message", MB_OK)

#define NO_COPY(ClassName)								\
ClassName(const ClassName&) = delete;					\
ClassName& operator=(const ClassName&) = delete;

#define DECLARE_SINGLETON(ClassName)					\
		NO_COPY(ClassName)								\
public :												\
	static ClassName* Get_Instance();					\
	static void		  Destroy_Instance();				\
private:												\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)					\
ClassName* ClassName::m_pInstance = nullptr;			\
ClassName* ClassName::Get_Instance()					\
{														\
	if (nullptr == m_pInstance)							\
		m_pInstance = new ClassName;					\
	return m_pInstance;									\
}														\
void ClassName::Destroy_Instance()						\
{														\
	if(nullptr != m_pInstance)							\
	{													\
		delete m_pInstance;								\
		m_pInstance = nullptr;							\
	}													\
}


#define D3DTS_VIEW CPipeLine::PIPELINE_STATE::TS_VIEW
#define D3DTS_PROJ CPipeLine::PIPELINE_STATE::TS_PROJ

#define GET_TRANSFORM_FLOAT4X4(eState) m_pGameInstance->Get_Transform_Float4x4(eState)
#define GET_TRANSFORM_MATRIX(eState) m_pGameInstance->Get_Transform_Matrix(eState)


// ����� ���ÿ� �ʱ�ȭ �Ұ���, ���� ����� ���������� �ڷ���, ��������� ��ġ�ϴ� ���¿����� ��� ����