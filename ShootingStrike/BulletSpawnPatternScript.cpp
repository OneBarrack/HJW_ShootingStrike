#include "BulletSpawnPatternScript.h"
#include "Object.h"
#include "ObjectManager.h"
#include "SpawnManager.h"
#include "MathManager.h"
#include "GoTargetAfterDelayBullet.h"
#include "SpreadAfterDelayBullet.h"

BulletSpawnPatternScript::BulletSpawnPatternScript()
	: pOwner(nullptr)
	, spawnPattern(eBulletSpawnPattern::NONE)
	, spawnTransInfo(Transform())
	, damage(0)	
	, bReady(false)
{
}

BulletSpawnPatternScript::~BulletSpawnPatternScript()
{
}

void BulletSpawnPatternScript::Initialize()
{
	pOwner = nullptr;
	spawnPattern = eBulletSpawnPattern::NONE;
	damage = 0;
	spawnTransInfo = Transform();
	bReady = false;
}

void BulletSpawnPatternScript::ReadyToSpawn(Object* _pOwner, eBulletSpawnPattern _spawnPattern, Transform _spawnTransInfo, int _damage)
{
	pOwner = _pOwner;
	spawnPattern = _spawnPattern;
	spawnTransInfo = _spawnTransInfo;
	damage = _damage;

	bReady = true;
}

void BulletSpawnPatternScript::Run()
{
	// ** Owner�� ���ų� Pattern ������ �Ǿ����� �ʴٸ� ����
	if ( !pOwner || spawnPattern == eBulletSpawnPattern::NONE )	
		return;

	Spawn();
	Update();
}

void BulletSpawnPatternScript::Spawn()
{
	static eBulletSpawnPattern prevSpawnPattern = spawnPattern;
	static ULONGLONG spawnTime = 0;
	static int cycleCount = 0;

	// ** ���ο� �������� ù ���� ��
	if ( bReady )
	{
		// ** spawnTime �� cycleCount �ʱ�ȭ
		spawnTime = 0;
		cycleCount = 0;
		bReady = false;
	}

	switch ( spawnPattern )
	{
		case eBulletSpawnPattern::SPIN_GO:
		{
			//** Spin ���� : 360�� ȸ���ϸ� ���������� Bullet ����

			int maxCycleCount = 200; // ** �ִ� �ߵ� Ƚ��
			int spawnCycleTime = 10; // ** �ߵ� �ð� ����
			float angleGap = 13.0f;	 // ** AngleGap : �Ѿ˰� ����(����)

			if ( spawnTime + spawnCycleTime < GetTickCount64() )
			{
				spawnTime = GetTickCount64();				

				// ** ��� ���� ���� ����
				float angle = angleGap * cycleCount;

				// ** Bullet�� TransInfo ����
				Transform bulletTransInfo;								
				bulletTransInfo.Direction = MathManager::RotateByDegree(spawnTransInfo.Direction, angle);
				bulletTransInfo.Position.x = spawnTransInfo.Position.x + bulletTransInfo.Direction.x * spawnTransInfo.Scale.x * 0.5f;
				bulletTransInfo.Position.y = spawnTransInfo.Position.y + bulletTransInfo.Direction.y * spawnTransInfo.Scale.x * 0.5f;
				bulletTransInfo.Scale = Vector3(10.0f, 10.0f);

				// ** Bullet�� Speed ����
				float bulletSpeed = 3.0f;

				// ** Bullet Spawn
				SpawnManager::SpawnBullet(pOwner, bulletTransInfo, bulletSpeed, damage, eBridgeKey::BULLET_NORMAL);

				++cycleCount;
			}

			// ** ���� ���� �� �ʱ�ȭ
			if ( maxCycleCount == cycleCount )
			{
				Initialize();
			}
			break;
		}
		case eBulletSpawnPattern::MULTI_SPIN_GO:
		{
			//** Multi Spin ���� : 360�� ���� ���� �������� ������������ ������ ��� ���������� Spin ������ ����

			int maxCycleCount = 100; // ** �ִ� �ߵ� Ƚ��
			int spawnCycleTime = 50; // ** �ߵ� �ð� ����
			int angleGap = 30;	 // ** AngleGap : �Ѿ˰� ����(����)

			if ( spawnTime + spawnCycleTime < GetTickCount64() )
			{
				spawnTime = GetTickCount64();

				int bulletCount = 360 / angleGap;
				for ( int i = 0; i < bulletCount; ++i )
				{
					// ** ���� ���� ���� ����
					int angle = angleGap * i + (cycleCount * 7);

					// ** Bullet�� TransInfo ����
					Transform bulletTransInfo;
					bulletTransInfo.Direction = MathManager::RotateByDegree(spawnTransInfo.Direction, angle);
					bulletTransInfo.Position.x = spawnTransInfo.Position.x + bulletTransInfo.Direction.x * spawnTransInfo.Scale.x * 0.5f;
					bulletTransInfo.Position.y = spawnTransInfo.Position.y + bulletTransInfo.Direction.y * spawnTransInfo.Scale.x * 0.5f;
					bulletTransInfo.Scale = Vector3(10.0f, 10.0f);

					// ** Bullet�� Speed ����
					float bulletSpeed = 3.0f;

					// ** Bullet Spawn
					SpawnManager::SpawnBullet(pOwner, bulletTransInfo, bulletSpeed, damage, eBridgeKey::BULLET_NORMAL);
				}

				++cycleCount;
			}

			// ** ���� ���� �� �ʱ�ȭ
			if ( maxCycleCount == cycleCount )
			{
				Initialize();
			}
			break;
		}
		case eBulletSpawnPattern::N_POLYGON_GO:
		{
			//** N������ �������� �Ѿ� �߻�

			int maxCycleCount = 1;	  // ** �ִ� �ߵ� Ƚ��
			int spawnCycleTime = 500; // ** �ߵ� �ð� ����			
			float bulletSpeed = 3.0f; // ** Bullet Speed

			// ** N������ ������ ����
			int vertex = 3;

			// ** �߻�� �Ѿ˰���. N�� �����ŭ�� �ߵ���
			int bulletCount = vertex * 10;

			// ** �� ������ ���� �Ѿ˵�� ���� ���� ������ ������ ���� ����
			int equalPartsCount = static_cast<int>(bulletCount / vertex);
			
			// ** Vertex�� ���� ����
			int angleGapForVertex = static_cast<int>(360 / vertex);			
			
			// ** �������� ���� �ӽ� ����
			vector<Transform> vertexList;

			if ( spawnTime + spawnCycleTime < GetTickCount64() )
			{
				spawnTime = GetTickCount64();

				// ** ������
				for ( int i = 0; i < vertex; ++i )
				{
					// ** ������ ����
					int angle = angleGapForVertex * i;

					// ** �������� TransInfo ����
					Transform bulletTransInfo;
					bulletTransInfo.Direction = MathManager::RotateByDegree(spawnTransInfo.Direction, angle);
					bulletTransInfo.Position.x = spawnTransInfo.Position.x + bulletTransInfo.Direction.x * spawnTransInfo.Scale.x * 0.5f;
					bulletTransInfo.Position.y = spawnTransInfo.Position.y + bulletTransInfo.Direction.y * spawnTransInfo.Scale.x * 0.5f;
					bulletTransInfo.Scale = Vector3(10.0f, 10.0f);
					
					// ** Bullet Spawn
					SpawnManager::SpawnBullet(pOwner, bulletTransInfo, bulletSpeed, damage, eBridgeKey::BULLET_NORMAL);

					vertexList.push_back(bulletTransInfo);
				}

				// �߽ɿ��� Vertex������ �Ÿ�
				int distToVertex = static_cast<int>(MathManager::GetDistance(spawnTransInfo.Position, vertexList[0].Position));

				for ( int i = 1; i < equalPartsCount; ++i )
				{
					for ( size_t j = 0; j < vertexList.size(); ++j )
					{		
						// ** ������ ���� �������� ���ݸ�ŭ �̵����� ��ġ�� ��� ���� ���� �������� ���� ������ �ε����� ����
						int curIndex = j;
						int nextIndex = (curIndex < (int)vertexList.size() - 1) ? curIndex + 1 : 0;

						// ** ������ ���� Bullet�� TransInfo ����						
						Transform bulletTransInfo;
						bulletTransInfo.Position = vertexList[curIndex].Position + (((vertexList[nextIndex].Position - vertexList[curIndex].Position) / (float)equalPartsCount) * (float)i);
						bulletTransInfo.Direction = MathManager::GetDirection(spawnTransInfo.Position, bulletTransInfo.Position);
						bulletTransInfo.Scale = Vector3(10.0f, 10.0f);

						// ** Bullet�� Speed�� ������ ratio�� ���� 
						// ** �߽ɿ��� Vertex������ �Ÿ��� ���� �߽ɿ��� ���� bullet��ġ�� ������ �����ش�
						float curDist = MathManager::GetDistance(spawnTransInfo.Position, bulletTransInfo.Position);
						float distRatio = curDist / distToVertex;

						// ** Bullet Spawn
						SpawnManager::SpawnBullet(pOwner, bulletTransInfo, bulletSpeed * distRatio, damage, eBridgeKey::BULLET_NORMAL);
					}
				}

				++cycleCount;
			}

			// ** ���� ���� �� �ʱ�ȭ
			if ( maxCycleCount == cycleCount )
			{
				Initialize();
			}
			break;
		}
		case eBulletSpawnPattern::CIRCLE_GO:
		{
			//** �� ���·� �߻�

			int maxCycleCount = 5;		// ** �ִ� �ߵ� Ƚ��
			int spawnCycleTime = 500;	// ** �ߵ� �ð� ����
			float angleGap = 13.0f;		// ** AngleGap : �Ѿ˰� ����(����)

			if ( spawnTime + spawnCycleTime < GetTickCount64() )
			{
				spawnTime = GetTickCount64();

				int bulletCount = static_cast<int>(360 / angleGap) + 1;
				for ( int i = 0; i < bulletCount; ++i )
				{
					// ** ���� ���� ���� ����
					float angle = angleGap * i;

					// ** Bullet�� TransInfo ����
					Transform bulletTransInfo;
					bulletTransInfo.Direction = MathManager::RotateByDegree(spawnTransInfo.Direction, angle);
					bulletTransInfo.Position.x = spawnTransInfo.Position.x + bulletTransInfo.Direction.x * spawnTransInfo.Scale.x * 0.5f;
					bulletTransInfo.Position.y = spawnTransInfo.Position.y + bulletTransInfo.Direction.y * spawnTransInfo.Scale.x * 0.5f;
					bulletTransInfo.Scale = Vector3(10.0f, 10.0f);

					// ** Bullet�� Speed ����
					float bulletSpeed = 3.0f;

					// ** Bullet Spawn
					SpawnManager::SpawnBullet(pOwner, bulletTransInfo, bulletSpeed, damage, eBridgeKey::BULLET_NORMAL);
				}

				++cycleCount;
			}

			// ** ���� ���� �� �ʱ�ȭ
			if ( maxCycleCount == cycleCount )
			{
				Initialize();
			}
			break;
		}
		case eBulletSpawnPattern::CIRCLE_GO_DELAY_GO_TARGET:
		{
			//** �� ���·� �߻� �� Delay�ð��� ������ Ÿ�ٹ������� ������ ƴ

			int maxCycleCount = 5;		// ** �ִ� �ߵ� Ƚ��
			int spawnCycleTime = 500;	// ** �ߵ� �ð� ����
			float angleGap = 13.0f;		// ** AngleGap : �Ѿ˰� ����(����)

			if ( spawnTime + spawnCycleTime < GetTickCount64() )
			{
				spawnTime = GetTickCount64();

				int bulletCount = static_cast<int>(360 / angleGap) + 1;
				for ( int i = 0; i < bulletCount; ++i )
				{
					// ** ���� ����
					float angle = angleGap * i;

					// ** Bullet�� TransInfo ����
					Transform bulletTransInfo;
					bulletTransInfo.Direction = MathManager::RotateByDegree(spawnTransInfo.Direction, angle);
					bulletTransInfo.Position.x = spawnTransInfo.Position.x + bulletTransInfo.Direction.x * spawnTransInfo.Scale.x * 0.5f;
					bulletTransInfo.Position.y = spawnTransInfo.Position.y + bulletTransInfo.Direction.y * spawnTransInfo.Scale.x * 0.5f;
					bulletTransInfo.Scale = Vector3(10.0f, 10.0f);

					// ** Bullet�� Speed ����
					float bulletSpeed = 3.0f;

					Bridge* pBridge = ObjectManager::GetInstance()->NewBridge(eBridgeKey::BULLET_GO_TARGET_AFTER_DELAY);
					static_cast<GoTargetAfterDelayBullet*>(pBridge)->SetDelay(1000);

					// ** Bullet Spawn
					SpawnManager::SpawnBullet(pOwner, bulletTransInfo, bulletSpeed, damage, pBridge);
				}

				++cycleCount;
			}

			// ** ���� ���� �� �ʱ�ȭ
			if ( maxCycleCount == cycleCount )
			{
				Initialize();
			}
			break;
		}
		case eBulletSpawnPattern::CIRCLE_STOP_DELAY_GO_TARGET:
		{
			//** �� ���·� �߻� �� Delay�ð��� ������ Ÿ�ٹ������� ������ ƴ

			int maxCycleCount = 30;		// ** �ִ� �ߵ� Ƚ��
			int spawnCycleTime = 200;	// ** �ߵ� �ð� ����
			float angleGap = 60;		// ** AngleGap : �Ѿ˰� ����(����)

			if ( spawnTime + spawnCycleTime < GetTickCount64() )
			{
				spawnTime = GetTickCount64();

				// ** ���� ����
				float angle = angleGap * cycleCount;

				// ** Bullet�� TransInfo ����
				Transform bulletTransInfo;
				bulletTransInfo.Direction = MathManager::RotateByDegree(spawnTransInfo.Direction, angle);
				bulletTransInfo.Position.x = spawnTransInfo.Position.x + bulletTransInfo.Direction.x * spawnTransInfo.Scale.x;
				bulletTransInfo.Position.y = spawnTransInfo.Position.y + bulletTransInfo.Direction.y * spawnTransInfo.Scale.x;
				bulletTransInfo.Scale = Vector3(10.0f, 10.0f);
				bulletTransInfo.Direction = Vector3(0.0f, 0.0f);

				// ** Bullet�� Speed ����
				float bulletSpeed = 3.0f;

				Bridge* pBridge = ObjectManager::GetInstance()->NewBridge(eBridgeKey::BULLET_GO_TARGET_AFTER_DELAY);
				static_cast<GoTargetAfterDelayBullet*>(pBridge)->SetDelay(1000);

				// ** Bullet Spawn
				SpawnManager::SpawnBullet(pOwner, bulletTransInfo, bulletSpeed, damage, pBridge);

				++cycleCount;
			}

			// ** ���� ���� �� �ʱ�ȭ
			if ( maxCycleCount == cycleCount )
			{
				Initialize();
			}
			break;
		}
		case eBulletSpawnPattern::CIRCLE_GO_DELAY_SPREAD:
		{
			//** �� ���·� �߻� �� Delay�ð��� ������ Ÿ�ٹ������� ������ ƴ

			int maxCycleCount = 5;		// ** �ִ� �ߵ� Ƚ��
			int spawnCycleTime = 2000;	// ** �ߵ� �ð� ����
			int angleGap = 60;			// ** AngleGap : �Ѿ˰� ����(����)

			if ( spawnTime + spawnCycleTime < GetTickCount64() )
			{
				spawnTime = GetTickCount64();

				// ** Bullet�� TransInfo ����
				Transform bulletTransInfo;
				bulletTransInfo.Position = Vector3(WINDOWS_WIDTH * 0.5f, 0.0f);
				bulletTransInfo.Scale = Vector3(10.0f, 10.0f);
				bulletTransInfo.Direction = Vector3(0.0f, 1.0f);

				// ** Bullet�� Speed ����
				float bulletSpeed = 3.0f;

				int bulletCount = 4 + cycleCount;
				Bridge* pBridge = ObjectManager::GetInstance()->NewBridge(eBridgeKey::BULLET_SPREAD_AFTER_DELAY);
				static_cast<SpreadAfterDelayBullet*>(pBridge)->SetDelay(1000);
				static_cast<SpreadAfterDelayBullet*>(pBridge)->SetSpreadCount(3);
				static_cast<SpreadAfterDelayBullet*>(pBridge)->SetBulletCount(bulletCount);
				static_cast<SpreadAfterDelayBullet*>(pBridge)->SetIntervalAngle(360 / bulletCount);

				// ** Bullet Spawn
				SpawnManager::SpawnBullet(pOwner, bulletTransInfo, bulletSpeed, damage, pBridge);

				++cycleCount;
			}

			// ** ���� ���� �� �ʱ�ȭ
			if ( maxCycleCount == cycleCount )
			{
				Initialize();
			}
			break;
		}
	}

	//** ���� ������ ���� ������ ��
	prevSpawnPattern = spawnPattern;
}

void BulletSpawnPatternScript::Update()
{
}