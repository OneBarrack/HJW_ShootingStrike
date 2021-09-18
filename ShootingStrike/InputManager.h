#pragma once
#include "Headers.h"

class InputManager
{
private:
	static InputManager* Instance;
public:
	static InputManager* GetInstance()
	{
		if ( Instance == nullptr )
			Instance = new InputManager;

		return Instance;
	}
private:
	// ** ���� ����� �ϴ� Key���� ��Ƴ��� List
	map<eInputKey, vector<DWORD>> SameKeyList;

	// ** Key�� ���� �Է� ������ �������
	map<eInputKey, eKeyInputStatus> KeyInfo;

public:
	// ** InputKey�� ���� �Է� ���� üũ
	void CheckKeyInputStatus();

	// ** �ش� Key�� ���� �Է� ���� ��ȯ
	eKeyInputStatus GetKeyStatus(eInputKey _Key) { return KeyInfo[_Key]; }

	Vector3 GetMousePosition()
	{
		POINT ptMouse;

		// ** ���콺 ��ǥ�� �޾ƿ�.
		GetCursorPos(&ptMouse);

		// ** ���콺 ��ǥ�� ���� ������â�� ��ǥ�� ����.
		ScreenToClient(g_hWnd, &ptMouse);

		return Vector3((float)ptMouse.x, (float)ptMouse.y);
	}

private:
	// ** ���� ����� �ϴ� Key �� �ϳ��� �������� Ȯ���ϴ� �Լ�
	bool IsKeyPressed(vector<DWORD> _SameKeys);

	// ** Key�� ���� ���� �Է»��� ����
	void SetKeyStatus(eKeyInputStatus& _KeyStatus, bool _IsKeyPressed);

	void AddKey(eInputKey _Key, DWORD _dwKey);
private:
	InputManager();
public:
	~InputManager() {}
};

