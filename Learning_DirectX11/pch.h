#pragma once

#include  "Types.h"
#include "Values.h"
#include "Struct.h"

// STL
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
using namespace std;	
#include <string>
#include <memory>
#include <array>

// Win
#include <Windows.h>
#include <assert.h>

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h> // Include
#include <DirectXTex/DirectXTex.inl> // Include 
/* ○ Include 설정 정리
    - 속성 - C/C++ - 일반 - 추가라이브러리디렉터리 를 ' $(SolutionDir)Libraries\Include\ ' 로 설정
	- 강의 3.기본프레임워크 의 강의자료에서 Libraries\Include\ 의 내용을 $(SolutionDir)Libraries\Include\ 로 복사
	- 마무리로 위처럼 Include
*/

using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment (lib, "d3d11.lib"); // 윈도우 SDK Library
#pragma comment (lib, "d3dcompiler.lib"); // 윈도우 SDK Library


#ifdef _DEBUG
#pragma comment (lib, "DirectXTex\\DirectXTex_debug.lib"); // 수동 설치 라이브러리
#else
#pragma comment (lib, "DirectXTex\\DirectXTex.lib"); // 수동 설치 라이브러리
#endif
/*  ○ Library 설정 정리
	 - 속성 - 링커 - 일반 - 추가라이브러리디렉터리 를 ' $(SolutionDir)Libraries\Lib\ ' 로 설정. 
	 - 강의 3.기본프레임워크 의 강의자료에서 Libraries\Lib 의 내용을 $(SolutionDir)Libraries\Lib\ 로 복사
	 - 마무리로 위에 #prgam comment .. 로 추가
*/


#define CHECK(p) assert(SUCCEEDED(p)) 

#define GAME GGame
#define SCENE GGame->GetSceneManager()
#define INPUT GGame->GetInputManager()
#define TIME GGame->GetTimeManager()
#define RESOURCES GGame->GetResourceManager()

// Engine
#include "Graphics.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "VertexData.h"
#include "Geometry.h"
#include "GeometryHelper.h"
#include "ShaderBase.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "BlendState.h"
#include "Pipeline.h"
#include "Transform.h"