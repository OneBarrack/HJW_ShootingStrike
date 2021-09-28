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
	vector<eKeyInputState> KeyInfo;

public:
	// ** InputKey�� ���� �Է� ���� üũ
	void CheckKeyInput();

	// ** �ش� Key�� ���� �Է� ���� ��ȯ
	eKeyInputState GetKeyState(eInputKey _Key) { return KeyInfo[static_cast<int>(_Key)]; }

	// ** ���콺 Position ��ȯ
	Vector3 GetMousePosition();	

private:
	// ** ���� ����� �ϴ� Key �� �ϳ��� �������� Ȯ���ϴ� �Լ�
	bool IsKeyPressed(vector<DWORD> _OverlapKeys);

	// ** Key�� ���� ���� �Է»��� ����
	void SetKeyState(eKeyInputState& _KeyState, bool _IsKeyPressed);

	void AddOverlapKey(eInputKey _Key, DWORD _dwKey);
private:
	InputManager();
public:
	~InputManager() {}
};

