#include "Enemy.h"
#include "Bridge.h"
#include "BitmapManager.h"
#include "MathManager.h"

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	pEnemyImage = nullptr;// BitmapManager::GetInstance()->GetImage(eImageKey::PLAYER);

	TransInfo.Position = Vector3(WindowsWidth * 0.5f, WindowsHeight * 0.5f);
	TransInfo.Scale = Vector3(42.0f, 47.0f);

	Collider.Position = TransInfo.Position;
	Collider.Scale = TransInfo.Scale;

	Key = eObjectKey::ENEMY;
	Status = eObjectStatus::ACTIVATED;
	CollisionType = eCollisionType::RECT;
	OldPosition = TransInfo.Position;
	bGenerateCollisionEvent = true;

	HP = 3;

	bSpawing = false;
	bAttacking = false;
	bTakeDamage = false;
	bDied = false;

	Speed = 3.0f;
	Frame = 0;
}

void Enemy::Update()
{
	if ( pBridgeObject )
		pBridgeObject->Update();

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

	// ** ���� ��ġ ���� ����
	OldPosition = TransInfo.Position;

	// ** �浹ü ����
	Collider = TransInfo;

	return;
}

void Enemy::Render(HDC _hdc)
{
	if ( pBridgeObject )
		pBridgeObject->Render(_hdc);

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
	if ( pBridgeObject )
	{
		pBridgeObject->Release();
		::Safe_Delete(pBridgeObject);
	}
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

	TakeDamage(_Damage);
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