#include "pch.h"
#include "InputLayout.h"

#if 1// 주석
InputLayout::InputLayout(ComPtr<ID3D11Device> device) : _device(device)
{

}

InputLayout::~InputLayout()
{

}

void InputLayout::Create(const vector<D3D11_INPUT_ELEMENT_DESC>& descs, ComPtr<ID3DBlob> blob)
{
	const int32 count = static_cast<int32>(descs.size());

	_device->CreateInputLayout(descs.data(), count, blob->GetBufferPointer(),
		blob->GetBufferSize(), _inputLayout.GetAddressOf());
}


#endif // 주석