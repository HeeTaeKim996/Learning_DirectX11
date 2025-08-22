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

		// �� API ȣ�� ���� �� Ű���� �Է��� ���� ���ϴ� ����? ��Ȳ�Ͻ� ::GetKeyboardState(asciKeys)�� false ��ȯ
	}

	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// Ű�� ���� ������ true
		if (asciKeys[key] & 0x80)
		{
			/*
				- 0x80	: Ű�� ����������
				- 0x01	: Ű�� ��ۻ��� (ĸ��)
				- 0x81	: Ű�� �������ְ�, ��ۻ���
			*/

			KEY_STATE& state = _states[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� PRESS
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

			// ���� �����ӿ� Ű�� �������¶�� UP
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