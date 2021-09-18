#include "StageBackground.h"
#include "BitmapManager.h"

StageBackground::StageBackground()
{
}

StageBackground::~StageBackground()
{
}

void StageBackground::Initialize()
{
	StageBkgScale  = Vector3(600.f, 5527.0f);
	SideBkgScale   = Vector3(600.0f, 800.0f);
	StageBkgOffset = 0.0f;
	SideBkgOffset  = 336.0f;
}

void StageBackground::Update(Transform& _rTransInfo)
{
	// ** ȭ���� ���� Image�� ���� �������� üũ, ���� ������ �ʾҴٸ� Offset ����
	if ( StageBkgOffset < StageBkgScale.y - WindowsHeight )
	{
		StageBkgOffset += 0.5f;
	}

}

void StageBackground::Render(HDC _hdc)
{
	BitBlt(_hdc,
		0, 0,
		WindowsWidth,
		WindowsHeight,
		BitmapManager::GetInstance()->GetMemDC(eImageKey::BACKGROUND),
		0, 0,
		SRCCOPY);

	TransparentBlt(_hdc,
		(WindowsWidth / 2) - (StageBkgScale.x / 2),
		0,
		StageBkgScale.x,
		WindowsHeight,
		BitmapManager::GetInstance()->GetMemDC(eImageKey::STAGEBACK),
		0,
		StageBkgScale.y - WindowsHeight - StageBkgOffset,
		StageBkgScale.x,
		WindowsHeight,
		RGB(255, 0, 255));

	Vector3 SideScale = Vector3(600.0f, 800.0f);
	// Side Background
	TransparentBlt(_hdc,
		0,
		0,
		SideBkgOffset,
		WindowsHeight,
		BitmapManager::GetInstance()->GetMemDC(eImageKey::SIDEBACKGROUND),
		0,
		0,
		SideBkgScale.x / 2,
		WindowsHeight,
		RGB(255, 0, 255));

	TransparentBlt(_hdc,
		WindowsWidth - SideBkgOffset,
		0,
		SideBkgOffset,
		WindowsHeight,
		BitmapManager::GetInstance()->GetMemDC(eImageKey::SIDEBACKGROUND),
		SideBkgScale.x / 2,
		0,
		SideBkgScale.x / 2,
		WindowsHeight,
		RGB(255, 0, 255));
}

void StageBackground::Release()
{

}
