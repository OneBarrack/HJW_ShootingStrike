#include "InputManager.h"

InputManager* InputManager::pInstance = nullptr;

InputManager::InputManager()
{
	// ** Key_MAX ũ�⸸ŭ ������ �Ҵ�
	overlapKeyList.resize(static_cast<int>(eInputKey::KEY_MAX));
	keyInfo.resize(static_cast<int>(eInputKey::KEY_MAX));
	
	AddOverlapKey(eInputKey::KEY_UP, VK_UP);
	AddOverlapKey(eInputKey::KEY_UP, 'W');
	AddOverlapKey(eInputKey::KEY_DOWN, VK_DOWN);
	AddOverlapKey(eInputKey::KEY_DOWN, 'S');
	AddOverlapKey(eInputKey::KEY_LEFT, VK_LEFT);
	AddOverlapKey(eInputKey::KEY_LEFT, 'A');
	AddOverlapKey(eInputKey::KEY_RIGHT, VK_RIGHT);	
	AddOverlapKey(eInputKey::KEY_RIGHT, 'D');
	AddOverlapKey(eInputKey::KEY_ESCAPE, VK_ESCAPE);
	AddOverlapKey(eInputKey::KEY_SPACE, VK_SPACE);
	AddOverlapKey(eInputKey::KEY_ENTER, VK_RETURN);
	AddOverlapKey(eInputKey::KEY_LBUTTON, VK_LBUTTON);
	AddOverlapKey(eInputKey::KEY_F8, VK_F8);
}

void InputManager::AddOverlapKey(eInputKey _key, DWORD _dwKey)
{
	overlapKeyList[static_cast<int>(_key)].push_back(_dwKey);
}

void InputManager::CheckKeyInput()
{
	for ( int key = 0; key < static_cast<int>(eInputKey::KEY_MAX); ++key )
	{
		SetKeyState(keyInfo[key], IsKeyPressed(overlapKeyList[key]));
	}
}

Vector3 InputManager::GetMousePosition()
{
	POINT ptMouse;

	// ** ���콺 ��ǥ�� �޾ƿ�.
	GetCursorPos(&ptMouse);

	// ** ���콺 ��ǥ�� ���� ������â�� ��ǥ�� ����.
	ScreenToClient(g_hWnd, &ptMouse);

	return Vector3((float)ptMouse.x, (float)ptMouse.y);
}

bool InputManager::IsKeyPressed(vector<DWORD> _overlapKeys)
{
	for ( DWORD Key : _overlapKeys )
	{
		if ( GetAsyncKeyState(Key) & 0x8000 )
		{
			return true;
		}
	}

	return false;
}

void InputManager::SetKeyState(eKeyInputState& _keyState, bool _IsKeyPressed)
{
	if ( _IsKeyPressed )
	{
		if ( _keyState == eKeyInputState::DOWN ||
			_keyState == eKeyInputState::PRESSED )
		{
			_keyState = eKeyInputState::PRESSED;
		}
		else
		{
			_keyState = eKeyInputState::DOWN;
		}
	}
	else
	{
		if ( _keyState == eKeyInputState::DOWN ||
			_keyState == eKeyInputState::PRESSED )
		{
			_keyState = eKeyInputState::UP;
		}
		else
		{
			_keyState = eKeyInputState::NONE;
		}
	}
}
