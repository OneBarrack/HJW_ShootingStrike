#include "LogoBackground.h"
#include "BitmapManager.h"

LogoBackground::LogoBackground() 
	: pLogoBkgImage(nullptr)
	, LogoBkgScale(Vector3())
	, LoopOffset1(0)
	, LoopOffset2(0)
	, ScrollSpeed(0)
	, bAttachBkg(false)
{
}

LogoBackground::~LogoBackground()
{
	Release();
}

void LogoBackground::Initialize()
{
	//** Logo background
	pLogoBkgImage = BitmapManager::GetInstance()->GetImage(eImageKey::LOGOBACK);
	LogoBkgScale = Vector3(1915.0f, 720.0f);
	LoopOffset1  = 0;
	LoopOffset2  = 0;
	bAttachBkg   = false;
	ScrollSpeed  = 1;
}

void LogoBackground::Update()
{

}

void LogoBackground::Render(HDC _hdc)
{
	// ** Logo background
	if ( pLogoBkgImage )
		RenderLogoBkg(_hdc);
}

void LogoBackground::Release()
{

}

void LogoBackground::RenderLogoBkg(HDC _hdc)
{
	// ** �̾� ���̴� ���� �ƴ� �� ����� ���� �����ߴٸ�
	if ( !bAttachBkg && LogoBkgScale.x < WindowsWidth + LoopOffset1 )
	{
		// ** �̾� ���̵��� Offset2�� bAttachBkg ����
		LoopOffset2 = 0;
		bAttachBkg = true;
	}

	// ** �̾� ���̴� ���� �� �̾���� ����� ȭ���� ���� ä��ٸ�
	if ( bAttachBkg && WindowsWidth < LoopOffset2 )
	{
		// ** �׸��� �� ���� ȭ���� �� ä�� �� �ֵ��� Offset1�� bAttachBkg ����
		LoopOffset1 = 0;
		bAttachBkg = false;
	}


	// ** �� ���� �׸�
	if ( !bAttachBkg )
	{
		BitBlt(_hdc,
			0,
			0,
			WindowsWidth,
			WindowsHeight,
			pLogoBkgImage->GetMemDC(),
			LoopOffset1,
			0,
			SRCCOPY);
	}
	// ** �� �κ�, �� �κ� �׸���� ������ �̾����
	else
	{
		BitBlt(_hdc,
			0,
			0,
			WindowsWidth - LoopOffset2,
			WindowsHeight,
			pLogoBkgImage->GetMemDC(),
			LoopOffset1,
			0,
			SRCCOPY);

		BitBlt(_hdc,
			WindowsWidth - LoopOffset2,
			0,
			LoopOffset2,
			WindowsHeight,
			pLogoBkgImage->GetMemDC(),
			0,
			0,
			SRCCOPY);
	}

	LoopOffset1 += ScrollSpeed;
	LoopOffset2 += ScrollSpeed;
}
