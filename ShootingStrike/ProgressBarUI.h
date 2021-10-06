#pragma once
#include "UIBridge.h"

class ProgressBarUI : public UIBridge
{
public:
	typedef UIBridge Super;

private:
	Bitmap* pProgressBarImage;

	// ** ǥ���� ��
	int value;

	// ** ǥ���� ���� �ִ�ġ
	int maxValue;

	float valueRatio;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	virtual Bridge* Clone() override { return new ProgressBarUI(*this); }

public:
	void SetValue(int _value, int _maxValue);

public:
	ProgressBarUI();
	virtual ~ProgressBarUI();
};

