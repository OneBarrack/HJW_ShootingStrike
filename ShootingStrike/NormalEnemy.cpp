#include "NormalEnemy.h"
#include "Object.h"
#include "BitmapManager.h"
#include "SpawnManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "SpawnManager.h"
#include "MathManager.h"
#include "GameDataManager.h"

NormalEnemy::NormalEnemy()
{
}

NormalEnemy::~NormalEnemy()
{

}


void NormalEnemy::Initialize()
{
	Super::Initialize();

	key = eBridgeKey::ENEMY_NORMAL;

	time = GetTickCount64();
}

void NormalEnemy::Update()
{
	// ** Owner�� �����͸� �޾ƿ�
	ReceiveInfoFromOwner();

	Super::Update();

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

	collider = transInfo;

	SendInfoToOwner();

	//bulletScript.Run();
}


void NormalEnemy::Render(HDC _hdc)
{
	Super::Render(_hdc);

	//if ( !pImage )
	//	return;

	RenderManager::DrawRect(_hdc, transInfo);
}

void NormalEnemy::Release()
{
	Super::Release();
}