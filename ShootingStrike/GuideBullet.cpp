#include "GuideBullet.h"
#include "Object.h"
#include "ObjectManager.h"
#include "BitmapManager.h"
#include "MathManager.h"

GuideBullet::GuideBullet()
{
}

GuideBullet::~GuideBullet()
{
}


void GuideBullet::Initialize()
{
	if ( pOwner )
		ReceiveInfoFromOwner();

	key = eBridgeKey::BULLET_GUIDE;

	speed = 3.0f;
}

void GuideBullet::Update()
{
	// ** Owner�� �����͸� �޾ƿ�
	ReceiveInfoFromOwner();

	CalcGuideDirection(transInfo.Position, transInfo.Direction);

	transInfo.Position.x += transInfo.Direction.x * speed;
	transInfo.Position.y += transInfo.Direction.y * speed;

	// ** Owner�� ������ ������ ����
	SendInfoToOwner();
}


void GuideBullet::Render(HDC _hdc)
{
	if ( pImage )
		RenderBullet(_hdc);
}

void GuideBullet::Release()
{

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
	list<Object*> objectList; 

	// ** ���Ǵ� Object�� List�� �޾ƿ´�
	eObjectKey OwnerObjectKey = pOwner->GetKey();
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
		findTargetList.insert(make_pair(distance,TargetObject));
	}

	// ** ���࿡ ����Ʈ�� �ƹ��͵� ���ٸ�....
	if ( findTargetList.empty() )
		return nullptr;

	// ** ��� ������Ʈ�� �߰��۾��� ������ ���� ù��°�� �ִ� ������Ʈ�� ��ȯ�Ѵ�.
	return findTargetList.begin()->second;
}

void GuideBullet::RenderBullet(HDC _hdc)
{
	TransparentBlt(_hdc, // ** ���� ��� ��ġ
		(int)(transInfo.Position.x - (transInfo.Scale.x * 0.5f)),
		(int)(transInfo.Position.y - (transInfo.Scale.y * 0.5f)),
		(int)transInfo.Scale.x,
		(int)transInfo.Scale.y,
		pImage->GetMemDC(),
		(int)transInfo.Scale.x,
		0,
		(int)transInfo.Scale.x,
		(int)transInfo.Scale.y,
		RGB(255, 0, 255));
}
