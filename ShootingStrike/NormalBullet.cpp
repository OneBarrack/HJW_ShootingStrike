#include "NormalBullet.h"
#include "Object.h"
#include "BitmapManager.h"

NormalBullet::NormalBullet() 
{
}

NormalBullet::~NormalBullet()
{
}


void NormalBullet::Initialize()
{
	Super::Initialize();

	key = eBridgeKey::BULLET_NORMAL;
}

void NormalBullet::Update()
{
	Super::Update();

	// ** Owner�� �����͸� �޾ƿ�
	ReceiveInfoFromOwner();

	transInfo.Position.x += transInfo.Direction.x * speed;
	transInfo.Position.y += transInfo.Direction.y * speed;

	// ** Owner�� ������ ������ ����
	SendInfoToOwner();
}


void NormalBullet::Render(HDC _hdc)
{
	Super::Render(_hdc);

	if ( !pImage )
		return;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pOwner->GetImage()->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * pOwner->GetImageOffsetOrder().x),
		(int)(pImage->GetSegmentationScale().y * pOwner->GetImageOffsetOrder().y),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(255, 0, 255));
}

void NormalBullet::Release()
{
	Super::Release();
}
