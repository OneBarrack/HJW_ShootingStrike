#pragma once
#include "Bridge.h"

class ItemBridge : public Bridge
{
protected:
	Bitmap* pImage;
	Transform transInfo;
	Transform collider;
	float speed;
	float acceleration;

	eItemType itemType;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	virtual Bridge* Clone() PURE;

public:
	// ** Owner�� �����͸� �޾ƿ�
	virtual void ReceiveInfoFromOwner() override;

	// ** Owner�� ������ ������ ����
	virtual void SendInfoToOwner() override;

public:
	ItemBridge();
	virtual ~ItemBridge();
};
