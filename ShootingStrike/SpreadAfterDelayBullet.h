#pragma once
#include "BulletBridge.h"

class SpreadAfterDelayBullet : public BulletBridge
{
public:
	typedef BulletBridge Super;

private:
	ULONGLONG time;
	int delay;
	bool bDelayOver;

	// ** �۶߸��� ���� Ƚ��
	int spreadCount;
	
	// ** �۶߸� �� �߻���ų Bullet ��
	int bulletCount;

	// ** �۶߷����� Bullet �� ����
	int intervalAngle;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	virtual Bridge* Clone() override { return new SpreadAfterDelayBullet(*this); }

public:
	void SetDelay(int _milliSeconds) { delay = _milliSeconds; }
	void SetSpreadCount(int _spreadCount) { spreadCount = _spreadCount; }
	void SetBulletCount(int _bulletCount) { bulletCount = _bulletCount; }
	void SetIntervalAngle(int _intervalAngle) { intervalAngle = _intervalAngle; }

public:
	SpreadAfterDelayBullet();
	virtual ~SpreadAfterDelayBullet();
};

