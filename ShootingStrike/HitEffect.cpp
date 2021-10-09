#include "HitEffect.h"

HitEffect::HitEffect()
	: offset(0)
	, delay(0)
	, time(0)
{

}

HitEffect::~HitEffect()
{
}


void HitEffect::Initialize()
{
	Super::Initialize();

	segmentImageCount = 1;
	key = eBridgeKey::EFFECT_HIT;
	time = GetTickCount64();
	offset = 0;
	delay = 100;
}

void HitEffect::Update()
{
	Super::Update();

	ReceiveInfoFromOwner();
	SendInfoToOwner();
}

void HitEffect::Render(HDC _hdc)
{
	Super::Render(_hdc);

	if ( !pImage )
		return;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * offset),
		0,
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(0, 63, 128));

	// ** Delay �ð� �ֱ�� Offset ����
	if ( time + delay < GetTickCount64() )
	{
		time = GetTickCount64();

		// ** ���� �� ���������.
		if ( ++offset == segmentImageCount )
		{
			offset = 0;
			pOwner->SetStatus(eObjectStatus::DESTROYED);
		}
	}
}

void HitEffect::Release()
{
	Super::Release();
}