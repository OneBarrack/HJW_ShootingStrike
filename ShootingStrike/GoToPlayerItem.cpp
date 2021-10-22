#include "GoToPlayerItem.h"
#include "ObjectManager.h"
#include "RenderManager.h"
#include "MathManager.h"
#include "GameDataManager.h"

GoToPlayerItem::GoToPlayerItem()
	: guideTime(0)
	, guideDelayTime(0)
	, bGuidePlayer(false)
{

}

GoToPlayerItem::~GoToPlayerItem()
{
}


void GoToPlayerItem::Initialize()
{
	Super::Initialize();

	key = eBridgeKey::ITEM_GO_TO_PLAYER;

	guideTime = GetTickCount64();
	guideDelayTime = 1000;

	bGuidePlayer = false;
}

void GoToPlayerItem::Update()
{
	Super::Update();

	ReceiveInfoFromOwner();

	// ���������� �������� �ӵ��� ���̰� �������϶� �ӵ��� ���δ�.
	if ( !bGuidePlayer )
	{
		speed -= acceleration;
	}
	else
	{
		speed += (acceleration * 2.0f);
	}


	// ** speed�� 0�� �Ǵ� �������� �÷��̾� ���� ����
	if ( speed <= 0.0f )
	{
		bGuidePlayer = true;
		speed = 0.0f;
	}

	// ** Player ��ġ�� ����
	if ( bGuidePlayer )
		CalcGuideDirectionForPlayer(transInfo.Position, transInfo.Direction);

	// ** ���� �ð�(guideDelayTime)�� ������
	//if ( guideTime + guideDelayTime < GetTickCount64() )
	//{
	//	// ** Player ��ġ�� ����
	//	CalcGuideDirectionForPlayer(transInfo.Position, transInfo.Direction);
	//}

	transInfo.Position.x += transInfo.Direction.x * speed;
	transInfo.Position.y += transInfo.Direction.y * speed;

	collider = transInfo;

	SendInfoToOwner();
}

void GoToPlayerItem::Render(HDC _hdc)
{
	Super::Render(_hdc);
	
	Point segmentationOffset;
	switch ( itemType )
	{
		case eItemType::POWER_MAX:
			segmentationOffset = Point(0, 0);
			break;
		case eItemType::POWER_UP:
			segmentationOffset = Point(1, 0);
			break;
		case eItemType::LIFE_UP:
			segmentationOffset = Point(2, 0);
			break;
		case eItemType::COIN:
			segmentationOffset = Point(GameDataManager::GetInstance()->GetFrame() % 7, 1);
			break;
		default:
			// ** �� ItemType�� �ƴϸ� �׸��� �ʴ´�
			return;
	}

	TransparentBlt(_hdc, // ** ���� ��� ��ġ
		int(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		int(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		int(pOwner->GetScale().x),
		int(pOwner->GetScale().y),
		pImage->GetMemDC(),
		int(pImage->GetSegmentationScale().x * segmentationOffset.x),
		int(pImage->GetSegmentationScale().y * segmentationOffset.y),
		int(pImage->GetSegmentationScale().x),
		int(pImage->GetSegmentationScale().y),
		RGB(255, 0, 255));

	//RenderManager::DrawRect(_hdc, pOwner->GetTransInfo(), RGB(255, 228, 0));
}

void GoToPlayerItem::Release()
{
	Super::Release();
}

void GoToPlayerItem::CalcGuideDirectionForPlayer(Vector3 _pos, Vector3& _rDirection)
{
	// ** �÷��̾� Ž��
	Object* pPlayer = ObjectManager::GetInstance()->GetPlayer();

	// ** ã�Ҵٸ� �ش� �� �������� Direction ����
	if ( pPlayer )
		_rDirection = MathManager::GetDirection(_pos, pPlayer->GetPosition());
}