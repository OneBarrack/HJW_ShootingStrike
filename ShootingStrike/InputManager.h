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
	vector<vector<DWORD>> OverlapKeyList;

	// ** Key�� ���� �Է� ������ �������
	vector<eKeyInputStatus> KeyInfo;

public:
	// ** InputKey�� ���� �Է� ���� üũ
	void CheckKeyInputStatus();

	// ** �ش� Key�� ���� �Է� ���� ��ȯ
	eKeyInputStatus GetKeyStatus(eInputKey _Key) { return KeyInfo[static_cast<int>(_Key)]; }

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
	bool IsKeyPressed(vector<DWORD> _OverlapKeys);

	// ** Key�� ���� ���� �Է»��� ����
	void SetKeyStatus(eKeyInputStatus& _KeyStatus, bool _IsKeyPressed);

	void AddOverlapKey(eInputKey _Key, DWORD _dwKey);
private:
	InputManager();
public:
	~InputManager() {}
};

