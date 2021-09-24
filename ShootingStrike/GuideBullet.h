#pragma once
#include "BulletBridge.h"

class GuideBullet : public BulletBridge
{
private:
	// ** Bullet
	Bitmap* pBulletImage;
	Vector3 BulletImageScale;

public:
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Render(HDC _hdc)override;
	virtual void Release()override;

private:
	// ** ���� �̻����� Direction ���
	void CalcGuideDirection(Vector3 _Pos, Vector3& _rDirection);

	// ** ���� ����� �� Ž��
	Object* FindTarget(Vector3 _Pos);

	void RenderBullet(HDC _hdc);

public:
	GuideBullet();
	virtual ~GuideBullet();
};

