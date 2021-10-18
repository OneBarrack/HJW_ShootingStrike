#include "Enemy.h"
#include "Bridge.h"
#include "Player.h"
#include "Bullet.h"
#include "BitmapManager.h"
#include "MathManager.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "SpawnManager.h"
#include "InputManager.h"

Enemy::Enemy()
	: enemyType(eEnemyType::ENEMY_BLUE_ELF)
	, maxHP(0)
	, HP(0)
	, bSpawing(false)
	, bAttacking(false)
	, bTakeDamage(false)
	, bDied(false)
	, hitScore(0)
	, deathScore(0)
	, oldPosition(Vector3())
	, fireBulletIntervalTime(0)
	, totalDegreeForSpin(0.0f)
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

	maxHP = 1;
	HP = 1;

	bSpawing = false;
	bAttacking = false;
	bTakeDamage = false;
	bDied = false;

	hitScore = 10;
	deathScore = 100;

	speed = 3.0f;
	fireBulletIntervalTime = 0;
	destPosition = Vector3();
	totalDegreeForSpin = 0.0f;

	SetEnemyType(eEnemyType::ENEMY_BLUE_ELF);

	InitMoveInfo();
}

void Enemy::Update()
{
	Super::Update();

	speed += acceleration;

	if ( !moveInfos.empty() )
	{
		eMoveType moveType = moveInfos.front().first;
		destPosition = moveInfos.front().second;

		bool bLoop = false;

		// **Ready �ܰ�. �ʱ�ȭ �۾�
		if ( moveState == eMoveState::READY )
		{
			moveState = eMoveState::MOVING;
			
			switch ( moveType )
			{
				case eMoveType::MOVE_TO:
					transInfo.Direction = MathManager::GetDirection(transInfo.Position, destPosition);
					break;
				case eMoveType::SPIN_LEFT:
					totalDegreeForSpin = 0.0f;
					break;
				case eMoveType::SPIN_RIGHT:
					totalDegreeForSpin = 0.0f;
					break;
				case eMoveType::BACK_AND_FORTH_LEFT:
					break;
				case eMoveType::BACK_AND_FORTH_RIGHT:
					break;
			}
		}

		// ** Moving �ܰ�. Move ����
		switch ( moveType )
		{
			case eMoveType::MOVE_TO:
				break;
			case eMoveType::SPIN_LEFT_FOR_LOOP:
				bLoop = true;
				[[fallthrough]];
			case eMoveType::SPIN_LEFT:
			{
				float rotationDegree = -1.0f;
				totalDegreeForSpin += rotationDegree;

				if ( !bLoop && abs(totalDegreeForSpin) >= 360 )
				{
					moveState = eMoveState::END;
				}

				transInfo.Direction = MathManager::RotateByDegree(transInfo.Direction, rotationDegree);				
				break;
			}			
			case eMoveType::SPIN_RIGHT_FOR_LOOP:
				bLoop = true;
				[[fallthrough]];
			case eMoveType::SPIN_RIGHT:
			{
				float rotationDegree = 1.0f;
				totalDegreeForSpin += rotationDegree;

				if ( !bLoop && abs(totalDegreeForSpin) >= 360 )
				{
					moveState = eMoveState::END;
				}

				transInfo.Direction = MathManager::RotateByDegree(transInfo.Direction, rotationDegree);				
				break;
			}
			case eMoveType::BACK_AND_FORTH_LEFT:
			{
				float backAndForthDegree = 1.0f;

				// ** ���� ���͸� ���Ѵ�.
				Vector3 perpendicularDirection;
				perpendicularDirection.x = transInfo.Direction.y;
				perpendicularDirection.y = -transInfo.Direction.x;

				transInfo.Position.x += perpendicularDirection.x * (speed * 2) * sinf(MathManager::DegreeToRadian(totalDegreeForSpin));
				transInfo.Position.y += perpendicularDirection.y * (speed * 2) * sinf(MathManager::DegreeToRadian(totalDegreeForSpin));

				totalDegreeForSpin += backAndForthDegree;
				break;
			}
			case eMoveType::BACK_AND_FORTH_RIGHT:
				float backAndForthDegree = 1.0f;

				// ** ���� ���͸� ���Ѵ�.
				Vector3 perpendicularDirection;
				perpendicularDirection.x = -transInfo.Direction.y;
				perpendicularDirection.y = transInfo.Direction.x;

				transInfo.Position.x += perpendicularDirection.x * (speed * 2) * sinf(MathManager::DegreeToRadian(totalDegreeForSpin));
				transInfo.Position.y += perpendicularDirection.y * (speed * 2) * sinf(MathManager::DegreeToRadian(totalDegreeForSpin));

				totalDegreeForSpin += backAndForthDegree;
				break;
		}

		transInfo.Position.x += transInfo.Direction.x * speed;
		transInfo.Position.y += transInfo.Direction.y * speed;

		// ** ���� �����ӿ� �������� �Ѿ�� ���¶��
		if ( abs(destPosition.x - transInfo.Position.x) <= abs(transInfo.Direction.x * speed) &&
			abs(destPosition.y - transInfo.Position.y) <= abs(transInfo.Direction.y * speed) )
		{
			// ** ������ ����
			transInfo.Position = destPosition;
			moveState = eMoveState::END;
		}

		if ( moveState == eMoveState::END )
		{
			moveInfos.pop();
			moveState = eMoveState::READY;
		}
	}	

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

void Enemy::Spawn()
{
	isSpawing = true;
	status = eObjectStatus::ACTIVATED;
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

	status = eObjectStatus::DESTROYED;
}

void Enemy::InitMoveInfo()
{
	moveInfos = queue<pair<eMoveType, Vector3>>();
	moveState = eMoveState::READY;
}

bool Enemy::isMoving()
{
	if ( moveState == eMoveState::MOVING )
		return true;

	return false;
}

void Enemy::SetEnemyType(eEnemyType _enemyType)
{
	enemyType = _enemyType;

	switch ( enemyType )
	{
		case eEnemyType::ENEMY_BLUE_ELF:
			SetMaxHP(10);
			SetHP(10);
			SetHitPoint(10);
			SetDeathPoint(100);
			break;
		case eEnemyType::ENEMY_RED_ELF:
			SetMaxHP(20);
			SetHP(20);
			SetHitPoint(20);
			SetDeathPoint(200);
			break;
		case eEnemyType::ENEMY_GREEN_ELF:
			SetMaxHP(30);
			SetHP(30);
			SetHitPoint(30);
			SetDeathPoint(300);
			break;
		case eEnemyType::ENEMY_WHITE_ELF:
			SetMaxHP(50);
			SetHP(50);
			SetHitPoint(50);
			SetDeathPoint(500);
			break;
		case eEnemyType::ENEMY_MIDDLE_BOSS:
			SetMaxHP(500);
			SetHP(500);
			SetHitPoint(100);
			SetDeathPoint(100000);
			break;
		case eEnemyType::ENEMY_BOSS_ANGEL:
			SetMaxHP(2000);
			SetHP(2000);
			SetHitPoint(256);
			SetDeathPoint(777777);
			break;
		default:
			SetMaxHP(0);
			SetHP(0);
			SetHitPoint(0);
			SetDeathPoint(0);
			break;
	}
}

void Enemy::MoveTo(Vector3 _destPosition)
{
	moveInfos.push(make_pair(eMoveType::MOVE_TO, _destPosition));
}

void Enemy::SpinLeft(bool _bLoop)
{
	// ** Loop üũ
	if ( _bLoop )
		moveInfos.push(make_pair(eMoveType::SPIN_LEFT_FOR_LOOP, Vector3()));
	else
		moveInfos.push(make_pair(eMoveType::SPIN_LEFT, Vector3()));
}

void Enemy::SpinRight(bool _bLoop)
{
	// ** Loop üũ
	if ( _bLoop )
		moveInfos.push(make_pair(eMoveType::SPIN_RIGHT_FOR_LOOP, Vector3()));
	else
		moveInfos.push(make_pair(eMoveType::SPIN_RIGHT, Vector3()));
}

void Enemy::BackAndForthLeft(Vector3 _destPosition)
{
	moveInfos.push(make_pair(eMoveType::BACK_AND_FORTH_LEFT, _destPosition));
}
void Enemy::BackAndForthRight(Vector3 _destPosition)
{
	moveInfos.push(make_pair(eMoveType::BACK_AND_FORTH_RIGHT, _destPosition));
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