#include "pch.h"
#include "InputManager.h"

void InputManager::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

void InputManager::Update()
{
	HWND hwnd = ::GetActiveWindow();
	if (_hwnd != hwnd)
	{
		for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
		{
			_states[key] = KEY_STATE::NONE;
		}

		return;
	}

	BYTE asciKeys[KEY_TYPE_COUNT] = {};
	if (::GetKeyboardState(asciKeys) == false)
	{
		return;

		// ※ API 호출 실패 등 키보드 입력을 받지 못하는 오류? 상황일시 ::GetKeyboardState(asciKeys)가 false 반환
	}

	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// 키가 눌려 있으면 true
		if (asciKeys[key] & 0x80)
		{
			/*
				- 0x80	: 키가 눌러져있음
				- 0x01	: 키가 토글상태 (캡스)
				- 0x81	: 키가 눌러져있고, 토글상태
			*/

			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
			{
				state = KEY_STATE::PRESS;
			}
			else
			{
				state = KEY_STATE::DOWN;
			}
		}
		else
		{
			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른상태라면 UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
			{
				state = KEY_STATE::UP;
			}
			else
			{
				state = KEY_STATE::NONE;
			}
		}
	}

	::GetCursorPos(&_mousePos);
	::ScreenToClient(_hwnd, &_mousePos);
}