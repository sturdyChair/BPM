#pragma once

namespace Engine
{
	template<typename T>
	void Safe_Delete(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete pPointer;
			pPointer = nullptr;
		}
	}

	template<typename T>
	unsigned int Safe_AddRef(T& pInstance)
	{
		unsigned int iRefCnt = 0;

		if (nullptr != pInstance)
			iRefCnt = pInstance->AddRef();

		return iRefCnt;
	}


	template<typename T>
	unsigned int Safe_Release(T& pInstance)
	{
		unsigned int iRefCnt = 0;

		if (nullptr != pInstance)
		{
			iRefCnt = pInstance->Release();
			pInstance = nullptr;
		}

		return iRefCnt;
	}


	template<typename T>
	void Safe_Delete_Array(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete[] pPointer;
			pPointer = nullptr;
		}
	}

	Engine::_float4x4 Matrix_Inverse(const Engine::_float4x4 & Matrix)
	{
		auto LoadedMatrix = XMLoadFloat4x4(&Matrix);
		LoadedMatrix = XMMatrixInverse(nullptr, LoadedMatrix);
		Engine::_float4x4 MatRet;
		XMStoreFloat4x4(&MatRet, LoadedMatrix);
		return MatRet;
	}
	_float3 Get_Cursor_Pos(HWND hWnd)
	{
		POINT pPt = {};
		GetCursorPos(&pPt);
		ScreenToClient(hWnd, &pPt);
		return _float3{ (float)pPt.x,(float)pPt.y, 0.f };

	}
	void XMFloat4x4ToFloatArray(const XMFLOAT4X4& mat, float out[16]) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				out[i * 4 + j] = mat.m[i][j];
			}
		}
	}

	void FloatArrayToXMFloat4x4(const float in[16], XMFLOAT4X4& mat) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				mat.m[i][j] = in[i * 4 + j];
			}
		}
	}

	template<typename T>
	std::ostream& write_data(std::ostream& stream, const T& value) {
		return stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
	}

	template<typename T>
	std::istream& read_data(std::istream& stream, T& value) {
		return stream.read(reinterpret_cast<char*>(&value), sizeof(T));
	}


	_float fRand()
	{
		return rand() % 1000 * 0.001f;
	}
	_float fRand(_float min, _float max)
	{
		return min + fRand() * (max - min);
	}
}

