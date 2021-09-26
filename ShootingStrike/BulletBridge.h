#pragma once
#include "Bridge.h"

class BulletBridge : public Bridge
{
protected:
	Transform TransInfo;
	float Speed;

public:
	virtual void Initialize()PURE;
	virtual void Update()PURE;
	virtual void Render(HDC _hdc)PURE;
	virtual void Release()PURE;

	// ** Owner�� �����͸� �޾ƿ�
	virtual void ReceiveInfo();

	// ** Owner�� ������ ������ ����
	virtual void SendInfo();

public:
	BulletBridge();
	virtual ~BulletBridge();
};

