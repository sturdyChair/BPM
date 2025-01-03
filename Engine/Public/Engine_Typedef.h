#pragma once

namespace Engine
{	
	typedef unsigned char			_ubyte;
	typedef signed char				_byte;
	typedef char					_char;
	typedef wchar_t					_tchar;
	typedef wstring					_wstring;

	typedef bool					_bool;

	typedef unsigned short			_ushort;
	typedef signed short			_short;

	typedef unsigned int			_uint;
	typedef signed int				_int;

	typedef unsigned long			_ulong;
	typedef signed long				_long;

	typedef float					_float;
	typedef double					_double;

	typedef XMFLOAT2				_float2;
	typedef XMFLOAT3				_float3;
	typedef XMFLOAT4				_float4;
	typedef XMVECTOR				_vector; // SIMD 연산을 위해 XMFLOAT_N 타입을 적재해준다. heap의 메모리 단편화 등 SIMD를 적용하는 일에 까다로운 점이 있어 스텍에 할당한다.
	typedef FXMVECTOR				_fvector;
	typedef GXMVECTOR				_gvector;
	typedef HXMVECTOR				_hvector;
	typedef CXMVECTOR				_cvector;


	typedef XMFLOAT4X4				_float4x4;
	typedef XMMATRIX				_matrix;
	typedef FXMMATRIX				_fmatrix;
	typedef CXMMATRIX				_cmatrix;

	enum class DELAYED_TYPE
	{
		CREATE_OBJECT,
		DELETE_OBJECT,
		CHANGE_LEVEL_DELAY,
		DELAYED_END,
	};

	enum MOUSEKEYSTATE
	{
		DIMK_LBUTTON,
		DIMK_RBUTTON,
		DIMK_WHEEL,
		DIMK_END,
	};

	enum MOUSEMOVESTATE
	{
		DIMM_X,
		DIMM_Y,
		DIMM_WHEEL,
		DIMM_END,
	};

	enum class COLLIDER_TYPE
	{
		SPHERE,
		AABB,
		OBB
	};

}

