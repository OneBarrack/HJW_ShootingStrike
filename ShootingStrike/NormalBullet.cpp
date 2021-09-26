#include "NormalBullet.h"
#include "Object.h"
#include "BitmapManager.h"

NormalBullet::NormalBullet() 
	: pBulletImage(nullptr)
	, BulletImageScale(Vector3())
{

}

NormalBullet::~NormalBullet()
{
	Release();
}


void NormalBullet::Initialize()
{
	pBulletImage = BitmapManager::GetInstance()->GetImage(eImageKey::PROJECTILE);
	BulletImageScale = Vector3(230.0f, 230.0f);
}

void NormalBullet::Update()
{
	// ** Owner�� �����͸� �޾ƿ�
	ReceiveInfo();

	TransInfo.Position.x += TransInfo.Direction.x * Speed;
	TransInfo.Position.y += TransInfo.Direction.y * Speed;

	// ** Owner�� ������ ������ ����
	SendInfo();
}


void NormalBullet::Render(HDC _hdc)
{
	if ( pBulletImage )
		RenderBullet(_hdc);
}

void NormalBullet::Release()
{

}

void NormalBullet::RenderBullet(HDC _hdc)
{
	TransparentBlt(_hdc, // ** ���� ��� ��ġ
		(int)(TransInfo.Position.x - (TransInfo.Scale.x * 0.5f)),
		(int)(TransInfo.Position.y - (TransInfo.Scale.y * 0.5f)),
		(int)TransInfo.Scale.x,
		(int)TransInfo.Scale.y,
		pBulletImage->GetMemDC(),
		(int)BulletImageScale.x,
		0,
		(int)BulletImageScale.x,
		(int)BulletImageScale.y,
		RGB(255, 0, 255));
}
