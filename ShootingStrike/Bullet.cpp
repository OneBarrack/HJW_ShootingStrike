#include "Bullet.h"
#include "ObjectManager.h"
#include "MathManager.h"
#include "CollisionManager.h"
#include "Bridge.h"
#include "Player.h"
#include "Enemy.h"

Bullet::Bullet()
	: pOwner(nullptr)
	, damage(0)
{

}

Bullet::~Bullet()
{
}


void Bullet::Initialize()
{
	Super::Initialize();

	// ** �ӽ� ������ �Ѿ�
	imageOffsetOrder = { 1, 0 };

	objectKey = eObjectKey::BULLET;
	objectStatus = eObjectStatus::ACTIVATED;
	collisionType = eCollisionType::ELLIPSE;
	bGenerateCollisionEvent = true;

	pOwner = nullptr;	
	damage = 0;
}

void Bullet::Update()
{
	Super::Update();
	
	// ** Stage ������ ������� üũ
	CheckPositionInBkgBoundary();

	// ** �浹ü ����
	SetCollider(transInfo);
}

void Bullet::Render(HDC _hdc)
{
	Super::Render(_hdc);
}

void Bullet::Release()
{
	Super::Release();
}

void Bullet::OnCollision(Object* _pObject)
{
	if ( pBridge )
	{		
		// ** ���� Object�ų� Item�� �浹�� �浹ó�� ���� ����
		if ( _pObject->GetKey() == pOwner->GetKey() || 
			_pObject->GetKey() == eObjectKey::ITEM )
			return;

		// ** Bullet�� ��ü Object�� �������� �浹�� Object�� ����
		switch ( pOwner->GetKey() )
		{
			case eObjectKey::PLAYER: 
				static_cast<Player*>(pOwner)->ApplyDamage(_pObject, damage); 
				SetStatus(eObjectStatus::DESTROYED); 
				break;
			case eObjectKey::ENEMY: 
				static_cast<Enemy*>(pOwner)->ApplyDamage(_pObject, damage);	
				SetStatus(eObjectStatus::DESTROYED); 
				break;
			default: 
				break;
		}
	}	
}

void Bullet::CheckPositionInBkgBoundary()
{
	// ** Stage ���� ���
	Object* pBackground = ObjectManager::GetInstance()->FindObjectWithTag(eObjectKey::BACKGROUND, eTagName::STAGE_MAIN_BKG);

	// ** Stage�� �ٿ����
	RectF bkgBoundary = pBackground->GetColliderF();

	// ** Stage �ٿ���� �������� Check ������ �߰�/���� ��ų Offset
	float offset = 0.0f;

	// ** �ٿ���� ũ�� ����
	bkgBoundary.Left   -= offset;
	bkgBoundary.Top	   -= offset;
	bkgBoundary.Right  += offset;
	bkgBoundary.Bottom += offset;

	// ** Stage�� �ٿ���� �� Position�� ��ġ���� ������ Destroy
	if ( !CollisionManager::IsPointInRect(bkgBoundary, transInfo.Position) )
	{
		SetStatus(eObjectStatus::DESTROYED);
	}
}