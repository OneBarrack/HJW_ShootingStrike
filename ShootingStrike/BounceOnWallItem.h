#pragma once
#include "ItemBridge.h"

class BounceOnWallItem : public ItemBridge
{
public:
	typedef ItemBridge Super;

private:
	// ** item�� ���� ����
	Vector3 itemDirection;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	virtual Bridge* Clone() override { return new BounceOnWallItem(*this); };

private:
	// ** Stage Map �ܰ��� ƨ���� ���� ���� ����
	void CalcDirectionForBounceOnWall(Transform& _transInfo);

public:
	BounceOnWallItem();
	virtual ~BounceOnWallItem();
};



