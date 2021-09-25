#include "LogoBackground.h"
#include "BitmapManager.h"

LogoBackground::LogoBackground() 
	: pLogoBkgImage(nullptr)
	, LogoBkgScale(Vector3())
	, LoopOffset1(0)
	, LoopOffset2(0)
	, ScrollSpeed(0)
	, bAttachBkg(false)
	, pLogoImage(nullptr)
	, LogoPosition(Vector3())
	, LogoScale(Vector3())
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

	// ** Logo
	pLogoImage = BitmapManager::GetInstance()->GetImage(eImageKey::LOGO);
	LogoScale = Vector3(923.0f, 350.0f);
	LogoPosition = Vector3((WindowsWidth * 0.5f) - (LogoScale.x * 0.5f),
							(WindowsHeight * 0.3f) - (LogoScale.y * 0.5f));
}

void LogoBackground::Update()
{

}

void LogoBackground::Render(HDC _hdc)
{
	// ** Logo background
	if ( pLogoBkgImage )
		RenderLogoBkg(_hdc);

	// ** Logo
	if ( pLogoImage )
		RenderLogo(_hdc);
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

void LogoBackground::RenderLogo(HDC _hdc)
{
	TransparentBlt(_hdc,
		(int)LogoPosition.x,
		(int)LogoPosition.y,
		(int)LogoScale.x,
		(int)LogoScale.y,
		pLogoImage->GetMemDC(),
		0,
		0,
		(int)LogoScale.x,
		(int)LogoScale.y,
		RGB(255, 0, 255));
}
