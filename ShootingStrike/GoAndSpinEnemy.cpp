#include "GoAndSpinEnemy.h"
#include "Object.h"
#include "BitmapManager.h"
#include "SpawnManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "SpawnManager.h"
#include "MathManager.h"
#include "GameDataManager.h"

GoAndSpinEnemy::GoAndSpinEnemy()
{
}

GoAndSpinEnemy::~GoAndSpinEnemy()
{

}


void GoAndSpinEnemy::Initialize()
{
	Super::Initialize();

	key = eBridgeKey::ENEMY_BACK_AND_FORTH;

	time = GetTickCount64();
	spinDirection = eSpinDirection::LEFT;
}

void GoAndSpinEnemy::Update()
{
	// ** Owner�� �����͸� �޾ƿ�
	ReceiveInfoFromOwner();

	Super::Update();

	// ** ���� ���� ���۾���
	if ( isSpawing )
		isSpawing = false;

	// ** �������� �������� ��� Spin ����
	if ( bArrivedToDest )
	{
		float angle;
		if ( spinDirection == eSpinDirection::LEFT )    
			angle = -1.0f;
		else // spinDirection == eSpinDirection::RIGHT
			angle = 1.0f;
 
		transInfo.Direction = MathManager::RotateByDegree(transInfo.Direction, angle);
	}
	
	// ** ���� �� �̶�� �״�� ����	
	transInfo.Position.x += transInfo.Direction.x * speed;
	transInfo.Position.y += transInfo.Direction.y * speed;
	
	collider = transInfo;

	// ** �ֱ��� Bullet �߻�
	if ( fireBulletIntervalTime > 0 && time + fireBulletIntervalTime < GetTickCount64() )
	{
		time = GetTickCount64();

		// ** ������ ���� Bullet�� TransInfo ����						
		Transform bulletTransInfo;
		bulletTransInfo.Position = transInfo.Position;
		bulletTransInfo.Direction = MathManager::GetDirection(transInfo.Position, GameDataManager::GetInstance()->GetPlayerPos());
		bulletTransInfo.Scale = Vector3(10.0f, 10.0f);

		// ** Bullet Spawn
		SpawnManager::SpawnBullet(pOwner, bulletTransInfo, 3.0f, 1, eBridgeKey::BULLET_NORMAL);
	}

	SendInfoToOwner();

	//bulletScript.Run();
}


void GoAndSpinEnemy::Render(HDC _hdc)
{
	Super::Render(_hdc);

	//if ( !pImage )
	//	return;

	RenderManager::DrawRect(_hdc, transInfo, RGB(120, 13, 150));
}

void GoAndSpinEnemy::Release()
{
	Super::Release();
}