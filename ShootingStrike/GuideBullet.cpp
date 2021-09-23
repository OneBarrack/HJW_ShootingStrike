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
	pBulletImage = BitmapManager::GetInstance()->GetImage(eImageKey::PROJECTILE);
	BulletImageScale = Vector3(230.0f, 230.0f);
	BulletRenderScale = Vector3(10.0f, 10.0f);

	Speed = 3.0f;

	DrawKey = "NormalBullet";
}

void GuideBullet::Update(Transform& _rTransInfo)
{
	CalcGuideDirection(_rTransInfo);

	_rTransInfo.Position.x += _rTransInfo.Direction.x * Speed;
	_rTransInfo.Position.y += _rTransInfo.Direction.y * Speed;
}


void GuideBullet::Render(HDC _hdc)
{
	if ( pBulletImage )
		RenderBullet(_hdc);
}

void GuideBullet::Release()
{

}

void GuideBullet::CalcGuideDirection(Transform& _rTransInfo)
{
	Object* Target = FindTarget(_rTransInfo.Position);

	if ( Target )
		_rTransInfo.Direction = MathManager::GetDirection(_rTransInfo.Position, Target->GetPosition());
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
		(int)(pOwner->GetPosition().x - (BulletRenderScale.x * 0.5f)),
		(int)(pOwner->GetPosition().y - (BulletRenderScale.y * 0.5f)),
		(int)BulletRenderScale.x,
		(int)BulletRenderScale.y,
		pBulletImage->GetMemDC(),
		(int)BulletImageScale.x,
		0,
		(int)BulletImageScale.x,
		(int)BulletImageScale.y,
		RGB(255, 0, 255));
}
