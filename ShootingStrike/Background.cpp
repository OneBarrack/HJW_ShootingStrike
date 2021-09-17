#include "Background.h"
#include "BitmapManager.h"

Background::Background()
{

}

Background::~Background()
{

}


void Background::Initialize()
{
	TransInfo.Position = Vector3(0.0f, 0.0f);
	TransInfo.Direction = Vector3(0.0f, 0.0f);
	TransInfo.Scale = Vector3(600.f, 5527.0f);

	Key = eObjectKey::BACKGROUND;
	Status = eObjectStatus::ACTIVATED;
	CollisionType = eCollisionType::NONE;

	Offset = 0.0f;
	bAttached = false;
}

int Background::Update()
{
	// ** ȭ���� ���� Image�� ���� �������� üũ, ���� ������ �ʾҴٸ� Offset ����
	if ( Offset < TransInfo.Scale.y - WindowsHeight )
	{
		++Offset;
	}

	return 0;
}

void Background::Render(HDC _hdc)
{
	BitBlt(_hdc,
		0, 0,
		WindowsWidth,
		WindowsHeight,
		BitmapManager::GetInstance()->GetMemDC(eImageKey::BACKGROUND),
		0, 0,
		SRCCOPY);

	TransparentBlt(_hdc,
		(WindowsWidth / 2) - (TransInfo.Scale.x / 2),
		0,
		TransInfo.Scale.x,
		WindowsHeight,
		BitmapManager::GetInstance()->GetMemDC(eImageKey::STAGEBACK),
		0,
		TransInfo.Scale.y - WindowsHeight - Offset,
		TransInfo.Scale.x,
		WindowsHeight,
		RGB(255, 0, 255));

	Vector3 SideScale = Vector3(600.0f, 800.0f);
	// Side Background
	TransparentBlt(_hdc,
		0,
		0,
		336,
		WindowsHeight,
		BitmapManager::GetInstance()->GetMemDC(eImageKey::SIDEBACKGROUND),
		0,
		0,
		SideScale.x / 2,
		WindowsHeight,
		RGB(255, 0, 255));

	TransparentBlt(_hdc,
		WindowsWidth - 336,
		0,
		336,
		WindowsHeight,
		BitmapManager::GetInstance()->GetMemDC(eImageKey::SIDEBACKGROUND),
		SideScale.x / 2,
		0,
		SideScale.x / 2,
		WindowsHeight,
		RGB(255, 0, 255));

	//// ** Image�� �� ���� �׸�
	//if ( !bAttached )
	//{
	//	TransparentBlt(_hdc, 
	//		0,
	//		0,
	//		WindowsWidth,
	//		WindowsHeight,
	//		BitmapManager::GetInstance()->GetMemDC(eImageKey::BACKGROUND),
	//		Offset, 0,
	//		WindowsWidth,
	//		WindowsHeight,
	//		RGB(255, 0, 255));
	//}
	//// ** Image�� ������ �׷����� Image�� �̾� ����
	//else
	//{
	//	// ** �ռ� �׸��� �̹����� �޺κ��� ������ �ն������� ���
	//	TransparentBlt(_hdc, 
	//		0,
	//		0,
	//		WindowsWidth - Offset,
	//		WindowsHeight,
	//		BitmapManager::GetInstance()->GetMemDC(eImageKey::BACKGROUND),
	//		(int)TransInfo.Scale.x - WindowsWidth + Offset, 0,
	//		WindowsWidth - Offset,
	//		WindowsHeight,
	//		RGB(255, 0, 255));

	//	// ** �� �̹����� �޺κп� �̹����� ù ��ŸƮ���� �̾�ٿ� �̹����� ���ӵǵ��� ��
	//	TransparentBlt(_hdc, 
	//		WindowsWidth - Offset,
	//		0,
	//		Offset,
	//		WindowsHeight,
	//		BitmapManager::GetInstance()->GetMemDC(eImageKey::BACKGROUND),
	//		0, 0,
	//		Offset,
	//		WindowsHeight,
	//		RGB(255, 0, 255));
	//}
}

void Background::Release()
{

}

void Background::OnCollision(Object* _pObject)
{
}
