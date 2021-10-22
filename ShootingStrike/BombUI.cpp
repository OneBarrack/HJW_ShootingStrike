#include "BombUI.h"
#include "BitmapManager.h"
#include "ObjectManager.h"
#include "GameDataManager.h"
#include "Player.h"

BombUI::BombUI()
	: pItemImage(nullptr)
	, bombCount(0)
{
}

BombUI::~BombUI()
{
}


void BombUI::Initialize()
{
	Super::Initialize();

	if ( !pItemImage )
		pItemImage = BitmapManager::GetInstance()->GetImage(eImageKey::ITEM);

	key = eBridgeKey::UI_BOMB;
	bombCount = 0;
}

void BombUI::Update()
{
	Super::Update();

	Player* pPlayer = static_cast<Player*>(ObjectManager::GetInstance()->GetPlayer());
	if ( pPlayer )
		bombCount = pPlayer->GetBomb();
}

void BombUI::Render(HDC _hdc)
{
	Super::Render(_hdc);

	if ( !pItemImage )
		return;

	Point bombImageOffset = Point(3, 0);

	// ** Life ��ŭ �̹��� ��ο�
	for ( int offset = 0; offset < bombCount; ++offset )
	{
		// ** Owner�� Scale y ���� �̹��� ������� ����. (���� ���η� ǥ���ϴ� ��ĸ� ����)
		TransparentBlt(_hdc, // ** ���� ��� ��ġ
			int(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f) + (pOwner->GetScale().y * offset)),
			int(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
			int(pOwner->GetScale().y),
			int(pOwner->GetScale().y),
			pItemImage->GetMemDC(),
			int(pItemImage->GetSegmentationScale().x * bombImageOffset.x),
			int(pItemImage->GetSegmentationScale().y * bombImageOffset.y),
			int(pItemImage->GetSegmentationScale().x),
			int(pItemImage->GetSegmentationScale().y),
			RGB(255, 0, 255));
	}
}

void BombUI::Release()
{
	Super::Release();

	pItemImage = nullptr;
}