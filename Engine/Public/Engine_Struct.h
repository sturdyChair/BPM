#pragma once


namespace Engine
{
	struct ENGINE_DESC
	{
		HWND			hWnd;
		HINSTANCE		hInstance;
		unsigned int	iWinSizeX;
		unsigned int	iWinSizeY;
		bool			isWindowed;
	};

	struct ENGINE_DLL VTXPOSTEX
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int	iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	};
	struct ENGINE_DLL VTXNORTEX
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int	iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	};
	struct ENGINE_DLL VTXMESH
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		static const unsigned int	iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	};
	struct ENGINE_DLL VTXANIMMESH
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndex0;
		XMUINT4			vBlendIndex1;
		//XMUINT4			vBlendIndex2;
		//XMUINT4			vBlendIndex3;
		//XMUINT4			vBlendIndex4;
		//XMUINT4			vBlendIndex5;
		XMFLOAT4		vBlendWeight0;
		XMFLOAT4		vBlendWeight1;
		//XMFLOAT4		vBlendWeight2;
		//XMFLOAT4		vBlendWeight3;
		//XMFLOAT4		vBlendWeight4;
		//XMFLOAT4		vBlendWeight5;

		static const unsigned int	iNumElements = 8;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	};

	struct ENGINE_DLL VTXPOINT
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vSize;
	};

	struct ENGINE_DLL VTXMATRIX
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vTranslation;
		XMFLOAT2		vLifeTime;

		static const unsigned int	iNumElements = 5;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	};

	struct ENGINE_DLL VTXRECTINSTANCE
	{
		static const unsigned int	iNumElements = 7;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	};

	struct ENGINE_DLL VTXPOINTINSTANCE
	{
		static const unsigned int	iNumElements = 7;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	};


	using DELAYED = struct tDelayed
	{
		DELAYED_TYPE eType;
		DWORD_PTR wParam;
		DWORD_PTR lParam;
	};

	enum class KEY_STATE
	{
		DOWN,
		PRESSING,
		UP,
		NONE,
	};
	struct KEY
	{
		bool bPrevPush = false;
		KEY_STATE eKeyState = KEY_STATE::NONE;
	};

	enum class SOUND_CHANNEL
	{
		MUSIC,
		PLAYER_SOUND_EFFECT,
		SOUND_EFFECT,
		CHANNEL_END
	};

	struct KEYFRAME
	{
		_float3 vScale;
		_float4 vRotation;
		_float3 vTranslation;
		_float  fTrackPosition;
	};
	const XMFLOAT4X4 Identity = { 1.f,0.f,0.f,0.f,
							 0.f,1.f,0.f,0.f,
							 0.f,0.f,1.f,0.f,
							 0.f,0.f,0.f,1.f };

	struct LIGHT_DESC
	{
		enum TYPE
		{
			TYPE_DIRECTIONAL,
			TYPE_POINT,
			TYPE_SPOT,
			TYPE_END,
		};
	};
	struct PointLight : public LIGHT_DESC
	{
		DirectX::XMFLOAT3 position;
		float range;
		DirectX::XMFLOAT3 color;
		float intensity;
		DirectX::XMFLOAT3 specColor;
		float pad;
	};

	struct DirectionalLight : public LIGHT_DESC
	{
		DirectX::XMFLOAT3 direction;
		float intensity;
		DirectX::XMFLOAT3 color;
		float pad1;
		DirectX::XMFLOAT3 specColor;
		float pad2;
	};

	struct SpotLight : public LIGHT_DESC
	{
		DirectX::XMFLOAT3 position;
		float range;
		DirectX::XMFLOAT3 direction;
		float innerConeCos;
		DirectX::XMFLOAT3 color;
		float outerConeCos;
		float intensity;
		DirectX::XMFLOAT3 specColor;
	};
}
