#pragma once
#if 1 // �ּ�
class Graphics
{
public:
	Graphics(HWND hwnd);
	~Graphics();

	void RenderBegin();
	void RenderEnd();


	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }



private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();

private:
	HWND _hwnd;
	uint32 _width = 0;
	uint32 _height = 0;


private:
	// Device & SwapChain

	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;
	// �� ComPtr : ����Ʈ ������ Dx11 ����. #include <wrl.h> / using namespace Micosoft::WRL; �� ���
	// �� Device, DeviceContext : Ŀ�ǵ弾��
	// 
	// �� Device
	//  - Ŀ�ǵ� ����. ���� ����
	// 
	// �� DeviceContext
	//  - Device ���� ������� �͵��, Device�� �����ϴ� ����
	// 
	// 
	// 
	// �� IDXGI : GPU�� �ų� �����ϱ� ������, directx �� ������ ������Ʈ������, ȭ�� ��� �κ��� DXGI �� ������ ����� ������,
	//           ���Ұ� ������ ���� DXGI �� directX �� �ƴ� DXGI ���� ����
	// �� SwapChain	: WinAPI�� BitBlt, StetchBlt ó��, �ĸ���ۿ� �̸� �׸���, ������ۿ� ������ �����ϴ� ���
	//   �� �ĸ� ������ ������ 2�� �̻��� ���� �ְ�. ������ۿ� �ĸ���۰� ���� ������ �۵��ϴ� ��. SwapChain�� ����� �پ���
	// 


	// RTV (RenderTargetView)
	ComPtr<ID3D11RenderTargetView> _renderTargetView; // �� RenderTargetView : ������, �ĸ� ���۸� �����ϴ� ����


	// Misc
	D3D11_VIEWPORT _viewPort = { 0 }; // �� ȭ���� ũ�⸦ �����ϴ� ����
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f }; // R G B A

};
#endif // �ּ�
