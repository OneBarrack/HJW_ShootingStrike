#include "StageBackground.h"
#include "BitmapManager.h"

StageBackground::StageBackground() 
	: pBkgImageForBorderLine(nullptr)
	, pStageBkgImage(nullptr)
	, StageBkgScale(Vector3())
	, StageBkgOffset(0.0f)
	, pStageSideBkgImage(nullptr)
	, SideBkgScale(Vector3())
	, SideBkgOffset(0.0f)
{
}

StageBackground::~StageBackground()
{
	Release();
}

void StageBackground::Initialize()
{
	// ** �¿� Border line�� �׸��� ���� ���
	pBkgImageForBorderLine = BitmapManager::GetInstance()->GetImage(eImageKey::BACKGROUND);

	// ** Stage background
	pStageBkgImage = BitmapManager::GetInstance()->GetImage(eImageKey::STAGEBACK);
	StageBkgScale  = Vector3(600.f, 5527.0f);
	StageBkgOffset = 0.0f;
	
	// ** Stage side background
	pStageSideBkgImage = BitmapManager::GetInstance()->GetImage(eImageKey::STAGESIDEBACK);
	SideBkgScale   = Vector3(600.0f, 800.0f);
	SideBkgOffset  = 336.0f;

	// ** Owner Position, Scale ����
	Vector3 OwnerScale;
	OwnerScale.x = StageBkgScale.x < WindowsWidth ? StageBkgScale.x : WindowsWidth;
	OwnerScale.y = StageBkgScale.y < WindowsHeight ? StageBkgScale.y : WindowsHeight;
	
	pOwner->SetPosition(Vector3(WindowsWidth * 0.5f, WindowsHeight * 0.5f));
	pOwner->SetScale(OwnerScale);
}

void StageBackground::Update()
{
	// ** ȭ���� ���� Image�� ���� �������� üũ, ���� ������ �ʾҴٸ� Offset ����
	if ( StageBkgOffset < StageBkgScale.y - WindowsHeight )
	{
		StageBkgOffset += 0.5f;
	}

}

void StageBackground::Render(HDC _hdc)
{
	// ** Background for borderLine (���� �¿� ���� ��輱�� ��Ÿ���� ���� ��濡 �׷���)
	if ( pBkgImageForBorderLine )
		RenderBkgForBorderLine(_hdc);

	// ** Stage main background
	if ( pStageBkgImage )
		RenderStageBkg(_hdc);

	// ** Stage side background
	if ( pStageSideBkgImage )
		RenderStageSideBkg(_hdc);
}

void StageBackground::Release()
{

}

void StageBackground::RenderBkgForBorderLine(HDC _hdc)
{
	BitBlt(_hdc,
		0, 0,
		WindowsWidth,
		WindowsHeight,
		pBkgImageForBorderLine->GetMemDC(),
		0, 0,
		SRCCOPY);
}

void StageBackground::RenderStageBkg(HDC _hdc)
{
	TransparentBlt(_hdc,
		(int)((WindowsWidth * 0.5f) - (StageBkgScale.x * 0.5f)),
		(int)0,
		(int)StageBkgScale.x,
		(int)WindowsHeight,
		BitmapManager::GetInstance()->GetImage(eImageKey::STAGEBACK)->GetMemDC(),
		(int)0,
		(int)(StageBkgScale.y - WindowsHeight - StageBkgOffset),
		(int)StageBkgScale.x,
		(int)WindowsHeight,
		RGB(255, 0, 255));
}

void StageBackground::RenderStageSideBkg(HDC _hdc)
{	
	TransparentBlt(_hdc,
		(int)0,
		(int)0,
		(int)SideBkgOffset,
		(int)WindowsHeight,
		BitmapManager::GetInstance()->GetImage(eImageKey::STAGESIDEBACK)->GetMemDC(),
		(int)0,
		(int)0,
		(int)(SideBkgScale.x * 0.5f),
		(int)WindowsHeight,
		RGB(255, 0, 255));

	TransparentBlt(_hdc,
		(int)(WindowsWidth - SideBkgOffset),
		(int)0,
		(int)SideBkgOffset,
		(int)WindowsHeight,
		BitmapManager::GetInstance()->GetImage(eImageKey::STAGESIDEBACK)->GetMemDC(),
		(int)(SideBkgScale.x * 0.5f),
		(int)0,
		(int)(SideBkgScale.x * 0.5f),
		(int)WindowsHeight,
		RGB(255, 0, 255));
}
