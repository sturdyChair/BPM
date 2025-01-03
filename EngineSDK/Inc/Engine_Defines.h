#pragma once

#pragma warning (disable : 4005)
#pragma warning (disable : 4251)

#include <d3d11.h>

#define  DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include <Fx11\d3dx11effect.h>
#include <DirectXTK\DDSTextureLoader.h>
#include <DirectXTK\WICTextureLoader.h>
#include <DirectXTK\SpriteBatch.h>
#include <DirectXTK\SpriteFont.h>

#include <DirectXTK\Effects.h>

#include <DirectXTK\ScreenGrab.h>
#include <DirectXCollision.h>
//#include <assimp\scene.h>
//#include <assimp\Importer.hpp>
//#include <assimp\postprocess.h>
//
//using namespace Assimp;
using namespace DirectX;
using namespace std;
#include <map>
#include <unordered_map>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <sstream>
#include <fstream>

#include "PxPhysicsApi.h"
using namespace physx;


#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#ifndef DBG_NEW 
//
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
//#define new DBG_NEW 
//
//#endif

#endif // _DEBUG

using namespace Engine;

