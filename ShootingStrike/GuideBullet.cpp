#include "GuideBullet.h"
#include "Object.h"
#include "ObjectManager.h"
#include "BitmapManager.h"
#include "MathManager.h"

GuideBullet::GuideBullet() 
	: pBulletImage(nullptr)
	, BulletImageScale(Vector3())
{
	Initialize();
}

GuideBullet::~GuideBullet()
{
	Release();
}


void GuideBullet::Initialize()
{
	pBulletImage = BitmapManager::GetInstance()->GetImage(eImageKey::PROJECTILE);
	BulletImageScale = Vector3(230.0f, 230.0f);

	Speed = 3.0f;
}

void GuideBullet::Update()
{
	// ** Owner�� �����͸� �޾ƿ�
	ReceiveInfo();

	CalcGuideDirection(TransInfo.Position, TransInfo.Direction);

	TransInfo.Position.x += TransInfo.Direction.x * Speed;
	TransInfo.Position.y += TransInfo.Direction.y * Speed;

	// ** Owner�� ������ ������ ����
	SendInfo();
}


void GuideBullet::Render(HDC _hdc)
{
	if ( pBulletImage )
		RenderBullet(_hdc);
}

void GuideBullet::Release()
{

}

void GuideBullet::CalcGuideDirection(Vector3 _Pos, Vector3& _rDirection)
{
	// ** ���� ����� �� Ž��
	Object* Target = FindTarget(_Pos);

	// ** ã�Ҵٸ� �ش� �� �������� Direction ����
	if ( Target )
		_rDirection = MathManager::GetDirection(_Pos, Target->GetPosition());
}

Object* GuideBullet::FindTarget(Vector3 _Pos)
{
	list<Object*> ObjectList; 

	// ** ���Ǵ� Object�� List�� �޾ƿ´�
	eObjectKey OwnerObjectKey = pOwner->GetKey();
	switch ( OwnerObjectKey )
	{
		case eObjectKey::PLAYER:
			ObjectList = ObjectManager::GetInstance()->GetObjectList(eObjectKey::ENEMY);
			break;
		case eObjectKey::ENEMY:
			ObjectList = ObjectManager::GetInstance()->GetObjectList(eObjectKey::PLAYER);
			break;
		default:
			break;
	}	

	// ** ��Ƽ���� �����. Key = �Ÿ�, value = Object
	multimap<float, Object*> FindTargetList;

	// ** ��� �� ���ָ���Ʈ�� ���鼭 Ȯ���Ѵ�.
	for ( Object* TargetObject : ObjectList )
	{
		// ** Current �� Target �� �Ÿ��� ���ؼ� ��Ƽ�ʿ� �߰��Ѵ�.
		float Distance = MathManager::GetDistance(_Pos, TargetObject->GetPosition());		
		FindTargetList.insert(make_pair(Distance,TargetObject));
	}

	// ** ���࿡ ����Ʈ�� �ƹ��͵� ���ٸ�....
	if ( FindTargetList.empty() )
		return nullptr;

	// ** ��� ������Ʈ�� �߰��۾��� ������ ���� ù��°�� �ִ� ������Ʈ�� ��ȯ�Ѵ�.
	return FindTargetList.begin()->second;
}

void GuideBullet::RenderBullet(HDC _hdc)
{
	TransparentBlt(_hdc, // ** ���� ��� ��ġ
		(int)(TransInfo.Position.x - (TransInfo.Scale.x * 0.5f)),
		(int)(TransInfo.Position.y - (TransInfo.Scale.y * 0.5f)),
		(int)TransInfo.Scale.x,
		(int)TransInfo.Scale.y,
		pBulletImage->GetMemDC(),
		(int)TransInfo.Scale.x,
		0,
		(int)TransInfo.Scale.x,
		(int)TransInfo.Scale.y,
		RGB(255, 0, 255));
}
