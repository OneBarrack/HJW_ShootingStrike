#include "GuideBullet.h"
#include "Object.h"
#include "Bullet.h"
#include "ObjectManager.h"
#include "BitmapManager.h"
#include "MathManager.h"

GuideBullet::GuideBullet()
	: distToTarget(0.0f)
	, bLoopGuide(true)
	, bGuideEnd(false)
	, hasDelay(false)
	, time(0)
	, delay(0)
	, speedAfterDelay(0.0f)
	, accelerationAfterDelay(0.0f)
	, maxSpeed(0.0f)
{
}

GuideBullet::~GuideBullet()
{
}


void GuideBullet::Initialize()
{
	Super::Initialize();

	key = eBridgeKey::BULLET_GUIDE;

	speed = 3.0f;

	distToTarget = 0.0f;

	bLoopGuide = true;
	bGuideEnd = false;
	hasDelay = false;
	time = 0;
	delay = 0;
	directionAfterDelay = Vector3();
	speedAfterDelay = 0.0f;
	accelerationAfterDelay = 0.0f;
	maxSpeed = 10.0f;
}

void GuideBullet::Update()
{
	Super::Update();

	// ** Owner�� �����͸� �޾ƿ�
	ReceiveInfoFromOwner();

	// ** Delay�� �ִٸ�
	if ( hasDelay )
	{
		// ** Delay�� ������ Delay ���� ������ ������ ���� �� hasDelay�� false�� ����
		if ( time + delay < GetTickCount64() )
		{
			transInfo.Direction = directionAfterDelay;
			speed = speedAfterDelay;
			pOwner->SetAcceleration(accelerationAfterDelay);
			hasDelay = false;
		}
	}	

	// ** Delay�� ����ǰ� GuideEnd�� false��� ���� ����
	if ( !hasDelay && !bGuideEnd )
	{
		CalcGuideDirection(transInfo.Position, transInfo.Direction);
		
		// ** ���� ������ �ƴ϶�� 1ȸ ���� �� Guide�� �����Ѵ�.
		if ( !bLoopGuide )
			bGuideEnd = true;
	}

	// ** Speed ����
	speed += pOwner->GetAcceleration();
	if ( speed > maxSpeed )
		speed = maxSpeed;

	transInfo.Position.x += transInfo.Direction.x * speed;
	transInfo.Position.y += transInfo.Direction.y * speed;

	// ** Owner�� ������ ������ ����
	SendInfoToOwner();
}


void GuideBullet::Render(HDC _hdc)
{
	Super::Render(_hdc);

	if ( pImage )
		RenderBullet(_hdc);
}

void GuideBullet::Release()
{
	Super::Release();
}

void GuideBullet::SetLoopGuide(bool _bLoopGuide)
{
	bLoopGuide = _bLoopGuide;
	bGuideEnd = false;
}

void GuideBullet::SetDelay(Vector3 _directionAfterDelay, float _speed, float _maxSpeed, float _acceleration, int _delay)
{
	hasDelay = true;
	time = GetTickCount64();
	delay = _delay;

	directionAfterDelay = _directionAfterDelay;
	speedAfterDelay = _speed;
	accelerationAfterDelay = _acceleration;
	maxSpeed = _maxSpeed;
}

void GuideBullet::CalcGuideDirection(Vector3 _pos, Vector3& _rDirection)
{
	// ** ���� ����� �� Ž��
	Object* target = FindTarget(_pos);

	// ** ã�Ҵٸ� �ش� �� �������� Direction ����
	if ( target )
		_rDirection = MathManager::GetDirection(_pos, target->GetPosition());
}

Object* GuideBullet::FindTarget(Vector3 _pos)
{
	// ** Ÿ���� �Ǵ� Object�� Key
	Object* targetObject = nullptr;
	eObjectKey targetObjectKey;

	// ** �� Bullet�� Owner�� üũ�Ͽ� ���Ǵ� Object�� ���� ����Ʈ�� �޾ƿ´�
	eObjectKey ownerObjectKey = static_cast<Bullet*>(pOwner)->GetOwner()->GetKey();	
	switch ( ownerObjectKey )
	{
		case eObjectKey::PLAYER:
			targetObjectKey = eObjectKey::ENEMY;			
			break;
		case eObjectKey::ENEMY:
			targetObjectKey = eObjectKey::PLAYER;
			break;
		default:
			targetObjectKey = eObjectKey::NONE;
			break;
	}	

	if ( targetObjectKey != eObjectKey::NONE )
	{
		list<Object*> objectList = ObjectManager::GetInstance()->GetObjectList(targetObjectKey);

		// ** ��Ƽ���� �����. Key = �Ÿ�, value = Object
		multimap<float, Object*> findTargetList;

		// ** ��� �� ���ָ���Ʈ�� ���鼭 Ȯ���Ѵ�.
		for ( Object* TargetObject : objectList )
		{
			// ** Ȱ��ȭ �Ǿ��� Collision event�� �۵��ϴ� Object�� ���ؼ� ����
			if ( TargetObject->GetStatus() == eObjectStatus::ACTIVATED && 
				TargetObject->IsGeneratedCollisionEvent() )
			{
				// ** Current �� Target �� �Ÿ��� ���ؼ� ��Ƽ�ʿ� �߰��Ѵ�.
				float distance = MathManager::GetDistance(_pos, TargetObject->GetPosition());
				findTargetList.insert(make_pair(distance, TargetObject));
			}
		}

		// ** ���࿡ ����Ʈ�� �ƹ��͵� ���ٸ�....
		if ( findTargetList.empty() )
			return nullptr;

		// ** ���� ù��°�� �� (���� �����) ������Ʈ���� �Ÿ� ����
		distToTarget = findTargetList.begin()->first;

		// ** ��� ������Ʈ�� �߰��۾��� ������ ���� ù��°�� �ִ� ������Ʈ�� ��ȯ�Ѵ�.
		targetObject = findTargetList.begin()->second;
	}

	return targetObject;
}

void GuideBullet::RenderBullet(HDC _hdc)
{
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
