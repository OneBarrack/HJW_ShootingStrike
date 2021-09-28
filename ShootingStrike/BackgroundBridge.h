#pragma once
#include "Bridge.h"

class BackgroundBridge : public Bridge
{
protected:
	Bitmap* pImage;
	Transform TransInfo;
	float Speed;

public:
	virtual void Initialize()PURE;
	virtual void Update()PURE;
	virtual void Render(HDC _hdc)PURE;
	virtual void Release()PURE;

	virtual Bridge* Clone()PURE;

public:
	// ** Owner�� �����͸� �޾ƿ�
	virtual void ReceiveInfoFromOwner() override;

	// ** Owner�� ������ ������ ����
	virtual void SendInfoToOwner() override;

public:
	BackgroundBridge();
	virtual ~BackgroundBridge();
};

