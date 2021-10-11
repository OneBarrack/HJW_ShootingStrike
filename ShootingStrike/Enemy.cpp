#include "Enemy.h"
#include "Bridge.h"
#include "Player.h"
#include "Bullet.h"
#include "BitmapManager.h"
#include "MathManager.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "SpawnManager.h"

Enemy::Enemy()
	: maxHP(0)
	, HP(0)
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

	maxHP = 50;
	HP = 50;

	bSpawing = false;
	bAttacking = false;
	bTakeDamage = false;
	bDied = false;

	speed = 3.0f;
}

void Enemy::Update()
{
	Super::Update();

	// ** ���ӵ� ����
	speed += acceleration;
	float aa = acceleration;
	// ** Enemy Update �۾� �ʿ�
	
	// ** ���� ��
	//if ( bSpawing )
	//{
	//	// SpawnEnemy
	//	return;
	//}

	//// ** ����
	//if ( bDied )
	//{
	//	bDied = false;
	//	//return;
	//}

	//// ** ���ظ� �Ծ��� ��
	//if ( bTakeDamage )
	//{

	//}

	// ** �Ϲ� ���
	//RenderEnemy(_hdc);

	// ** Direction ����
	//transInfo.Direction = MathManager::GetDirection(oldPosition, transInfo.Position);

	// ** Stage ������ ������� üũ
	CheckPositionInBkgBoundary();

	// ** ���� ��ġ ���� ����
	//oldPosition = transInfo.Position;	

	// ** �浹ü ������ �긴������
	//SetCollider(transInfo);

	return;
}

void Enemy::Render(HDC _hdc)
{
	Super::Render(_hdc);
}

void Enemy::Release()
{
	Super::Release();
}

void Enemy::OnCollision(Object* _pObject)
{
	switch ( _pObject->GetKey() )
	{
		// ** Player�� �浹�� �÷��̾ ������ ����
		case eObjectKey::PLAYER:
			ApplyDamage(_pObject, 1);
			break;
		// ** Player�� �Ѿ˰� �浹�� Hit����Ʈ ����
		case eObjectKey::BULLET:
			if ( static_cast<Bullet*>(_pObject)->GetOwner()->GetKey() == eObjectKey::PLAYER )
			{
				// ** Hit ����Ʈ ����
				Transform hitEffectTransInfo;
				hitEffectTransInfo.Position = _pObject->GetPosition();
				hitEffectTransInfo.Scale = Vector3(30.0f, 30.0f);
				SpawnManager::SpawnEffect(hitEffectTransInfo, eBridgeKey::EFFECT_HIT);
			}
			break;
		default:
			break;
	}
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
		Die();
	}	
}

void Enemy::Die()
{
	// ** bDied flag true ����
	bDied = true;

	// ** ���� ����Ʈ ����
	Transform explosionTransInfo;
	explosionTransInfo.Position = transInfo.Position;
	explosionTransInfo.Scale = transInfo.Scale;
	SpawnManager::SpawnEffect(explosionTransInfo, eBridgeKey::EFFECT_EXPLOSION);
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