#pragma once
#include "BulletBridge.h"

class GuideBullet : public BulletBridge
{
public:
	typedef BulletBridge Super;

private:
	// ** ���� ����� ������Ʈ���� �Ÿ�
	float distToTarget;

	// ** ���� ���� �� ������. false �� �� ������ Ÿ�������� 1ȸ ���� �� �����ȴ�.
	bool bLoopGuide;
	bool bGuideEnd;

	// ** Delay�� �ִ���
	bool hasDelay;
	ULONGLONG time;
	int delay;;
	
	// ** Delay ���� �ʿ��� ����, �ӵ�, ���ӵ�
	Vector3 directionAfterDelay;
	float speedAfterDelay;
	float accelerationAfterDelay;
	float maxSpeed;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	virtual Bridge* Clone()override { return new GuideBullet(*this); }

public:
	void SetLoopGuide(bool _bLoopGuide);
	void SetDelay(Vector3 _directionAfterDelay, float _speed = 0.0f, float _maxSpeed = 10.0f, float _acceleration = 0.0f, int _delay = 0);

	// ** ���� ����� ������Ʈ���� �Ÿ� ��ȯ
	float GetDistToTarget() { return distToTarget; }

private:
	// ** ���� �̻����� Direction ���
	void CalcGuideDirection(Vector3 _pos, Vector3& _rDirection);

	// ** ���� ����� �� Ž��
	Object* FindTarget(Vector3 _pos);

	void RenderBullet(HDC _hdc);

public:
	GuideBullet();
	virtual ~GuideBullet();
};

