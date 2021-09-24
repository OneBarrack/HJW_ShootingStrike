#pragma once
#include "BackgroundBridge.h"

class StageBackground : public BackgroundBridge
{
private:
	// ** �¿� Border line�� �׸��� ���� ���
	Bitmap* pBkgImageForBorderLine;

	// ** Stage background
	Bitmap* pStageBkgImage;
	Vector3 StageBkgScale;
	float StageBkgOffset;

	// ** Stage side background
	Bitmap* pStageSideBkgImage;
	Vector3 SideBkgScale;
	float SideBkgOffset;

public:
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Render(HDC _hdc)override;
	virtual void Release()override;

private:
	// ** Draw Background for borderLine (���� �¿� ���� ��輱�� ��Ÿ���� ���� ��濡 �׷���)
	void RenderBkgForBorderLine(HDC _hdc);
	
	// ** Draw Stage main background
	void RenderStageBkg(HDC _hdc);

	// ** Draw Stage side background
	void RenderStageSideBkg(HDC _hdc);

public:
	StageBackground();
	virtual ~StageBackground();
};

