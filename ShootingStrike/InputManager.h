#pragma once
#include "Headers.h"

class InputManager
{
private:
	static InputManager* pInstance;
public:
	static InputManager* GetInstance()
	{
		if ( pInstance == nullptr )
			pInstance = new InputManager;

		return pInstance;
	}
private:
	// ** ���� ����� �ϴ� Key���� ��Ƴ��� List
	vector<vector<DWORD>> overlapKeyList;

	// ** Key�� ���� �Է� ������ �������
	vector<eKeyInputState> keyInfo;

public:
	// ** InputKey�� ���� �Է� ���� üũ
	void CheckKeyInput();

	// ** ���콺 Position ��ȯ
	Vector3 GetMousePosition();
	
	// ** �ش� Key�� ���� �Է� ���� ��ȯ
	inline eKeyInputState GetKeyState(eInputKey _Key) { return keyInfo[static_cast<int>(_Key)]; }

private:
	// ** ���� ����� �ϴ� Key �� �ϳ��� �������� Ȯ���ϴ� �Լ�
	bool IsKeyPressed(vector<DWORD> _overlapKeys);

	// ** Key�� ���� ���� �Է»��� ����
	void SetKeyState(eKeyInputState& _keyState, bool _IsKeyPressed);

	void AddOverlapKey(eInputKey _key, DWORD _dwKey);

private:
	InputManager();
public:
	~InputManager() {}
};

