#include "pch.h"
#include "framework.h"
#include "Learning_DirectX11.h"
#include "Game.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
HWND hWnd;

ATOM MyRegisterClass(HINSTANCE hInstance);
/* ATOM ( == WORD == unsgined uint32 )
	- MyRegisterClass 에서는, 생성 완료시 등록한 클래스 핸들(ID) 리턴
*/

BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return false;
	}

	Game game;
	game.Init(hWnd);

	MSG msg = {};

	while (msg.message != WM_QUIT) // ※ WM_QUIT : PostQuitMessage(0); 때 메세지 인큐
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			// PM_REMOVE : PeekMessage 후 해당 메세지 삭제. PM_NOREMOVE 시, PEEK 후에도 메세지 삭제 안함
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			game.Update();
			game.Render();
		}
	}

	return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LEARNINGDIRECTX11));
	wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_LEARNINGDIRECTX11));
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = L"Learning_DirectX11";
	wcex.lpszMenuName = NULL;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	RECT windowRect = { 0, 0, GWinSizeX, GWinSizeY };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	hWnd = CreateWindow(L"Learning_DirectX11", L"Client", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	::ShowWindow(hWnd, nCmdShow);
	::UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wMid = LOWORD(wParam);

		switch (wMid)
		{
		case IDM_EXIT: // ※ IDM_EXIT : 보통 메뉴 명령에서 종료 누를시 호출
			DestroyWindow(hWnd);
			/*
			보통, 메뉴 종료버튼 -> IDM_EXIT -> DestroyWindow -> WM_DESTROY -> PostQUitMessage -> WM_quit
			*/
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}