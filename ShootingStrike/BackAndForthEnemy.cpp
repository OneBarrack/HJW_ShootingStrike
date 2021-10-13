#include "BackAndForthEnemy.h"
#include "Object.h"
#include "BitmapManager.h"
#include "SpawnManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "SpawnManager.h"
#include "MathManager.h"
#include "GameDataManager.h"

BackAndForthEnemy::BackAndForthEnemy()
	: angle(0)
	, bFindPerpendicularDirection(false)
{
}

BackAndForthEnemy::~BackAndForthEnemy()
{

}


void BackAndForthEnemy::Initialize()
{
	Super::Initialize();

	key = eBridgeKey::ENEMY_BACK_AND_FORTH;

	time = GetTickCount64();
	angle = 0;
	perpendicularDirection = Vector3();
	bFindPerpendicularDirection = false;
}

void BackAndForthEnemy::Update()
{
	// ** Owner�� �����͸� �޾ƿ�
	ReceiveInfoFromOwner();

	Super::Update();

	// ** ���� ���� ���۾���
	if ( isSpawing )
		isSpawing = false;

	// ** �������� �̵� �ӵ�
	int BackAndForthSpeed = 3;

	// ** ���� ���͸� ���Ѵ�
	if ( !bFindPerpendicularDirection )
	{
		bFindPerpendicularDirection = true;

		if ( startDirection == eBackAndForthDirection::LEFT )
		{
			perpendicularDirection.x = transInfo.Direction.y;
			perpendicularDirection.y = -transInfo.Direction.x;
		}
		else // startDirection == eBackAndForthDirection::RIGHT
		{
			perpendicularDirection.x = -transInfo.Direction.y;
			perpendicularDirection.y = transInfo.Direction.x;
		}			
	}

	// ** 360�� ���� Angle �ʱ�ȭ
	if ( angle > 360 )
	{
		angle = 0;
	}

	// ** ���� ������������ ���ٰ� ���ƿ�����
	transInfo.Position.x += perpendicularDirection.x * BackAndForthSpeed * sinf(MathManager::DegreeToRadian(angle));
	transInfo.Position.y += perpendicularDirection.y * BackAndForthSpeed * sinf(MathManager::DegreeToRadian(angle));
		
	angle += BackAndForthSpeed;
	
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


void BackAndForthEnemy::Render(HDC _hdc)
{
	Super::Render(_hdc);

	//if ( !pImage )
	//	return;

	RenderManager::DrawRect(_hdc, transInfo, RGB(120, 13, 150));
}

void BackAndForthEnemy::Release()
{
	Super::Release();
}