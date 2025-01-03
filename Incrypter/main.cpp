#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>

using namespace std;
namespace fs = filesystem;


struct XMFLOAT3
{
	float x;
	float y;
	float z;
};

struct XMFLOAT2
{
	float x;
	float y;
};

struct VTXMESH
{
	XMFLOAT3		vPosition;
	XMFLOAT3		vNormal;
	XMFLOAT2		vTexcoord;
	XMFLOAT3		vTangent;
};

string normalize_path(const std::string& path) {
	std::string normalized;
	int iBs = 0;
	for (char ch : path) {
		if (ch == '\"')
		{

		}
		else if (ch == '\\') {
			if (iBs == 0)
			{
				normalized += '/';
				++iBs;
			}
			else
			{
				iBs = 0;
			}

		}
		else {
			normalized += ch;
		}
	}

	return normalized;
}
int main()
{


	return 0;
}