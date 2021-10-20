#pragma once
#include "EffectBridge.h"

class WarningEffect : public EffectBridge
{
public:
	typedef EffectBridge Super;

private:
	Bitmap* pFadeBackImage;

	// ** AlphaBlend�� ����� ������
	BYTE alpha;
	int fadeSpeed;

	// ** Warning image�� Fade out / in �Ǵ� cycle Ƚ��
	int fadeCycleCount;

	// ** Fade out / in cycle�� �� ȸ ���� �Ұ�����
	int maxFadeCycleCount;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	virtual Bridge* Clone() override { return new WarningEffect(*this); }

public:
	WarningEffect();
	virtual ~WarningEffect();
};

