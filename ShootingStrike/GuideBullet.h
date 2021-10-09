#pragma once
#include "BulletBridge.h"

class GuideBullet : public BulletBridge
{
public:
	typedef BulletBridge Super;

private:
	// ** ���� ����� ������Ʈ���� �Ÿ�
	float distToTarget;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	virtual Bridge* Clone()override { return new GuideBullet(*this); }

public:
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

