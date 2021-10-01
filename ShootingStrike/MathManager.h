#pragma once
#include "Headers.h"

class MathManager
{
public:
	// ** �Ÿ��� ���ϴ� �Լ�
	static float GetDistance(Vector3 _start, Vector3 _dest)
	{
		// ** _Current �� _Target ������ �غ��� ���̸� ����.
		float fX = _dest.x - _start.x;
		float fY = _dest.y - _start.y;

		// ** ������ ��ȯ
		return sqrt((fX * fX) + (fY * fY));
	}

	// ** ������ ���ϴ� �Լ�
	static Vector3 GetDirection(Vector3 _start, Vector3 _dest)
	{
		// ** _Current �� _Target ������ �غ��� ���̸� ����.
		float fX = _dest.x - _start.x;
		float fY = _dest.y - _start.y;

		// ** ������ ����.
		float distance = sqrt((fX * fX) + (fY * fY));

		// ** ���⸸ ���ܵα� ���� �������� �����ش�.
		return Vector3(fX / distance, fY / distance);
		// ** ���� 
		// ** ������ �ﰢ���� ������ ������ �׻� �غ��̳� ���̺��� ũ��.
		// ** �ڱ� �ڽź��� ū������ �������ٸ� ���� 1���� ���� ���� ���´�.
		// ** Vector ���� 1�̰ų� 1���� ���� ���� ���⸸ �����ִٰ� ����.
		// ** 1 �̶�� ���� ����� ���ϴ��� ���� ���� ������ �����̴�.
	}
};

