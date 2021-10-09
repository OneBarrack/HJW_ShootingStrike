#include "GoTargetAfterDelayBullet.h"
#include "Object.h"
#include "Bullet.h"
#include "ObjectManager.h"
#include "MathManager.h"

GoTargetAfterDelayBullet::GoTargetAfterDelayBullet()
	: time(0)
	, delay(0)
	, bDelayOver(false)
{
}

GoTargetAfterDelayBullet::~GoTargetAfterDelayBullet()
{
}


void GoTargetAfterDelayBullet::Initialize()
{
	Super::Initialize();

	key = eBridgeKey::BULLET_GO_TARGET_AFTER_DELAY;

	time = GetTickCount64();
	delay = 1000;
	bDelayOver = false;
}

void GoTargetAfterDelayBullet::Update()
{
	Super::Update();

	// ** Owner�� �����͸� �޾ƿ�
	ReceiveInfoFromOwner();

	if ( !bDelayOver && time + delay < GetTickCount64() )
	{
		if ( CalcGuideDirection(transInfo.Position, transInfo.Direction) )
		{
			bDelayOver = true;
		}
	}

	transInfo.Position.x += transInfo.Direction.x * speed;
	transInfo.Position.y += transInfo.Direction.y * speed;

	// ** Owner�� ������ ������ ����
	SendInfoToOwner();
}


void GoTargetAfterDelayBullet::Render(HDC _hdc)
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

void GoTargetAfterDelayBullet::Release()
{
	Super::Release();
}

bool GoTargetAfterDelayBullet::CalcGuideDirection(Vector3 _pos, Vector3& _rDirection)
{
	// ** ���� ����� �� Ž��
	Object* target = FindTarget(_pos);

	// ** ã�Ҵٸ� �ش� �� �������� Direction ����
	if ( target )
	{
		_rDirection = MathManager::GetDirection(_pos, target->GetPosition());
		return true;
	}

	return false;
}

Object* GoTargetAfterDelayBullet::FindTarget(Vector3 _pos)
{
	list<Object*> objectList;

	// ** �� Bullet�� Owner�� üũ�Ͽ� ���Ǵ� Object�� ���� ����Ʈ�� �޾ƿ´�
	eObjectKey OwnerObjectKey = static_cast<Bullet*>(pOwner)->GetOwner()->GetKey();
	switch ( OwnerObjectKey )
	{
		case eObjectKey::PLAYER:
			objectList = ObjectManager::GetInstance()->GetObjectList(eObjectKey::ENEMY);
			break;
		case eObjectKey::ENEMY:
			objectList = ObjectManager::GetInstance()->GetObjectList(eObjectKey::PLAYER);
			break;
		default:
			break;
	}

	// ** ��Ƽ���� �����. Key = �Ÿ�, value = Object
	multimap<float, Object*> findTargetList;

	// ** ��� �� ���ָ���Ʈ�� ���鼭 Ȯ���Ѵ�.
	for ( Object* TargetObject : objectList )
	{
		// ** Current �� Target �� �Ÿ��� ���ؼ� ��Ƽ�ʿ� �߰��Ѵ�.
		float distance = MathManager::GetDistance(_pos, TargetObject->GetPosition());
		findTargetList.insert(make_pair(distance, TargetObject));
	}

	// ** ���࿡ ����Ʈ�� �ƹ��͵� ���ٸ�....
	if ( findTargetList.empty() )
		return nullptr;

	// ** ��� ������Ʈ�� �߰��۾��� ������ ���� ù��°�� �ִ� ������Ʈ�� ��ȯ�Ѵ�.
	return findTargetList.begin()->second;
}
