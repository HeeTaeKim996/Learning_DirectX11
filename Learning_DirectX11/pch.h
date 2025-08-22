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
/* �� Include ���� ����
    - �Ӽ� - C/C++ - �Ϲ� - �߰����̺귯�����͸� �� ' $(SolutionDir)Libraries\Include\ ' �� ����
	- ���� 3.�⺻�����ӿ�ũ �� �����ڷῡ�� Libraries\Include\ �� ������ $(SolutionDir)Libraries\Include\ �� ����
	- �������� ��ó�� Include
*/

using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment (lib, "d3d11.lib"); // ������ SDK Library
#pragma comment (lib, "d3dcompiler.lib"); // ������ SDK Library


#ifdef _DEBUG
#pragma comment (lib, "DirectXTex\\DirectXTex_debug.lib"); // ���� ��ġ ���̺귯��
#else
#pragma comment (lib, "DirectXTex\\DirectXTex.lib"); // ���� ��ġ ���̺귯��
#endif
/*  �� Library ���� ����
	 - �Ӽ� - ��Ŀ - �Ϲ� - �߰����̺귯�����͸� �� ' $(SolutionDir)Libraries\Lib\ ' �� ����. 
	 - ���� 3.�⺻�����ӿ�ũ �� �����ڷῡ�� Libraries\Lib �� ������ $(SolutionDir)Libraries\Lib\ �� ����
	 - �������� ���� #prgam comment .. �� �߰�
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