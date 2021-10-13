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
		if ( distance > 0.0f )
			return Vector3(fX / distance, fY / distance);
		else
			return Vector3(0.0f, 0.0f);
		// ** ���� 
		// ** ������ �ﰢ���� ������ ������ �׻� �غ��̳� ���̺��� ũ��.
		// ** �ڱ� �ڽź��� ū������ �������ٸ� ���� 1���� ���� ���� ���´�.
		// ** Vector ���� 1�̰ų� 1���� ���� ���� ���⸸ �����ִٰ� ����.
		// ** 1 �̶�� ���� ����� ���ϴ��� ���� ���� ������ �����̴�.
	}

	// ** Convert degree to radian
	static float DegreeToRadian(int _degree) { return _degree * PI / 180; }
	static float DegreeToRadian(float _degree) { return _degree * PI / 180; }

	// ** Convert radian to degree
	static int RadianToDegree(float _radian) { return static_cast<int>(_radian * 180 / PI); }

	// ** Rotate vector by degree
	static Vector3 RotateByDegree(Vector3 _v, float _degree) 
	{
		return Vector3(
			(_v.x * cosf(DegreeToRadian(_degree))) - (_v.y * sinf(DegreeToRadian(_degree))),
			(_v.x * sinf(DegreeToRadian(_degree))) + (_v.y * cosf(DegreeToRadian(_degree))));
	}

	// ** Rotate vector by radian
	static Vector3 RotateByRadian(Vector3 _v, float _radian)
	{
		return Vector3(
			(_v.x * cosf(_radian)) - (_v.y * sinf(_radian)),
			(_v.x * sinf(_radian)) + (_v.y * cosf(_radian)));
	}
};

