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
	: bulletSpawnTime(0)
{
}

NormalEnemy::~NormalEnemy()
{

}


void NormalEnemy::Initialize()
{
	Super::Initialize();

	pImage = BitmapManager::GetInstance()->GetImage(eImageKey::ENEMY_NORMAL);

	key = eBridgeKey::ENEMY_NORMAL;

	bulletSpawnTime = GetTickCount64();
	animationTime = GetTickCount64();
	segmentationOffset = 0;
}

void NormalEnemy::Update()
{
	// ** Owner�� �����͸� �޾ƿ�
	ReceiveInfoFromOwner();
	
	Super::Update();

	// ** �ֱ��� Bullet �߻�
	if ( fireBulletIntervalTime > 0 && bulletSpawnTime + fireBulletIntervalTime < GetTickCount64() )
	{
		bulletSpawnTime = GetTickCount64();

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

	if ( !pImage )
		return;

	switch ( enemyType )
	{
		case eEnemyType::ENEMY_BLUE_ELF:
			imageOffset = Vector3(0.0f, 0.0f);
			imageScale = Vector3(32.0f, 32.0f);
			break;
		case eEnemyType::ENEMY_RED_ELF:
			imageOffset = Vector3(0.0f, 96.0f);
			imageScale = Vector3(48.0f, 32.0f);
			break;
		case eEnemyType::ENEMY_GREEN_ELF:
			imageOffset = Vector3(0.0f, 192.0f);
			imageScale = Vector3(48.0f, 48.0f);
			break;
		case eEnemyType::ENEMY_WHITE_ELF:
			imageOffset = Vector3(192.0f, 0.0f);
			imageScale = Vector3(64.0f, 64.0f);
			break;
		case eEnemyType::ENEMY_MIDDLE_BOSS:
			imageOffset = Vector3(192.0f, 320.0f);
			imageScale = Vector3(64.0f, 64.0f);
			break;
		default:
			imageOffset = Vector3();
			imageScale = Vector3();
			break;
	}

	int maxSegmentCount = 4; // ** �ش� ���ο��� ���ҵ� �̹��� ��
	int delay = 200;

	TransparentBlt(_hdc,
		int(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		int(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		int(pOwner->GetScale().x),
		int(pOwner->GetScale().y),
		pImage->GetMemDC(),
		int(imageOffset.x + (imageScale.x * segmentationOffset)),
		int(imageOffset.y),
		int(imageScale.x),
		int(imageScale.y),
		RGB(255, 0, 255));

	// ** Delay �ð� �ֱ�� Offset ����
	if ( animationTime + delay < GetTickCount64() )
	{
		// ** Default�� �׻� Loop �����̹Ƿ� Offset�� 0���� ����
		if ( ++segmentationOffset == maxSegmentCount )
			segmentationOffset = 0;

		animationTime = GetTickCount64();
	}

	//RenderManager::DrawRect(_hdc, transInfo);
}

void NormalEnemy::Release()
{
	Super::Release();
}