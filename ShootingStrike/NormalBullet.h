#pragma once
#include "BulletBridge.h"

class NormalBullet : public BulletBridge
{
private:
	// ** Bullet
	Bitmap* pBulletImage;
	Vector3 BulletImageScale;
	Vector3 BulletRenderScale;

public:
	virtual void Initialize()override;
	virtual void Update(Transform& _rTransInfo)override;
	virtual void Render(HDC _hdc)override;
	virtual void Release()override;

private:
	void RenderBullet(HDC _hdc);

public:
	NormalBullet();
	virtual ~NormalBullet();
};

