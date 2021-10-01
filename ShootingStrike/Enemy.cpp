#include "Enemy.h"
#include "Bridge.h"
#include "Player.h"
#include "BitmapManager.h"
#include "MathManager.h"
#include "ObjectManager.h"
#include "CollisionManager.h"

Enemy::Enemy()
	: HP(0)
	, bSpawing(false)
	, bAttacking(false)
	, bTakeDamage(false)
	, bDied(false)
	, HitPoint(0)
	, DeathPoint(0)
	, Frame(0)
	, OldPosition(Vector3())
{

}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	Super::Initialize();

	Key = eObjectKey::ENEMY;
	Status = eObjectStatus::ACTIVATED;
	CollisionType = eCollisionType::RECT;
	OldPosition = TransInfo.Position;
	bGenerateCollisionEvent = true;

	HP = 5;

	bSpawing = false;
	bAttacking = false;
	bTakeDamage = false;
	bDied = false;

	Speed = 3.0f;
	Frame = 0;
}

void Enemy::Update()
{
	Super::Update();

	// ** ���� ��
	if ( bSpawing )
	{
		// SpawnEnemy
		return;
	}

	// ** ����
	if ( bDied )
	{
		bDied = false;
		//return;
	}

	// ** ���ظ� �Ծ��� ��
	if ( bTakeDamage )
	{

	}

	// ** �Ϲ� ���
	//RenderEnemy(_hdc);

	// ** Direction ����
	TransInfo.Direction = MathManager::GetDirection(OldPosition, TransInfo.Position);

	// ** Stage ������ ������� üũ
	CheckPositionInBkgBoundary();

	// ** ���� ��ġ ���� ����
	OldPosition = TransInfo.Position;

	// ** �浹ü ����
	SetCollider(TransInfo);

	return;
}

void Enemy::Render(HDC _hdc)
{
	Super::Render(_hdc);

	//TransparentBlt(_hdc, // ** ���� ��� ��ġ
	//	int(TransInfo.Position.x - (TransInfo.Scale.x * 0.5f)),
	//	int(TransInfo.Position.y + (TransInfo.Scale.x * 0.5f) - Offset.y - 50),
	//	int(TransInfo.Scale.x),
	//	int(Offset.y),
	//	BitmapManager::GetInstance()->GetImage(eImageKey::MOLE)->GetMemDC(),
	//	0, 0,
	//	int(TransInfo.Scale.x),
	//	int(Offset.y),
	//	RGB(255, 0, 255));
}

void Enemy::Release()
{
	Super::Release();
}

void Enemy::OnCollision(Object* _pObject)
{
}

void Enemy::Fire()
{
}

void Enemy::ApplyDamage(Object* _pTarget, int _Damage)
{
	// ** ... �������� ���� ���� ����
	switch ( _pTarget->GetKey() )
	{
		case eObjectKey::PLAYER:
			static_cast<Player*>(_pTarget)->TakeDamage(_Damage);
			break;
	}
}

void Enemy::TakeDamage(int _Damage)
{
	HP -= _Damage;
	bTakeDamage = true;

	if ( HP <= 0 )
	{
		HP = 0;
		bDied = true;
	}
}

void Enemy::CheckPositionInBkgBoundary()
{
	// ** Stage ���� ���
	Object* pBackground = ObjectManager::GetInstance()->FindObjectWithTag(eObjectKey::BACKGROUND, eTagName::STAGE_MAIN_BKG);

	// ** Stage�� �ٿ����
	RectF BkgBoundary = pBackground->GetColliderF();

	// ** Stage �ٿ���� �������� Check ������ �߰�/���� ��ų Offset
	float Offset = 0.0f;

	// ** �ٿ���� ũ�� ����
	BkgBoundary.Left -= Offset;
	BkgBoundary.Top -= Offset;
	BkgBoundary.Right += Offset;
	BkgBoundary.Bottom += Offset;

	// ** Stage�� �ٿ���� �� Position�� ��ġ���� ������ Destroy
	if ( !CollisionManager::IsPointInRect(BkgBoundary, TransInfo.Position) )
	{
		SetStatus(eObjectStatus::DESTROYED);
	}
}