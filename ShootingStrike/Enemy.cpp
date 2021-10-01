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
	, hitScore(0)
	, deathScore(0)
	, oldPosition(Vector3())
{

}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	Super::Initialize();

	key = eObjectKey::ENEMY;
	status = eObjectStatus::ACTIVATED;
	collisionType = eCollisionType::RECT;
	oldPosition = transInfo.Position;
	bGenerateCollisionEvent = true;

	HP = 5;

	bSpawing = false;
	bAttacking = false;
	bTakeDamage = false;
	bDied = false;

	speed = 3.0f;
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
	transInfo.Direction = MathManager::GetDirection(oldPosition, transInfo.Position);

	// ** Stage ������ ������� üũ
	CheckPositionInBkgBoundary();

	// ** ���� ��ġ ���� ����
	oldPosition = transInfo.Position;

	// ** �浹ü ����
	SetCollider(transInfo);

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

void Enemy::ApplyDamage(Object* _pTarget, int _damage)
{
	// ** ... �������� ���� ���� ����
	switch ( _pTarget->GetKey() )
	{
		case eObjectKey::PLAYER:
			static_cast<Player*>(_pTarget)->TakeDamage(_damage);
			break;
		default:
			break;
	}
}

void Enemy::TakeDamage(int _damage)
{
	HP -= _damage;
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
	RectF bkgBoundary = pBackground->GetColliderF();

	// ** Stage �ٿ���� �������� Check ������ �߰�/���� ��ų Offset
	float Offset = 0.0f;

	// ** �ٿ���� ũ�� ����
	bkgBoundary.Left -= Offset;
	bkgBoundary.Top -= Offset;
	bkgBoundary.Right += Offset;
	bkgBoundary.Bottom += Offset;

	// ** Stage�� �ٿ���� �� Position�� ��ġ���� ������ Destroy
	if ( !CollisionManager::IsPointInRect(bkgBoundary, transInfo.Position) )
	{
		SetStatus(eObjectStatus::DESTROYED);
	}
}