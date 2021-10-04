#include "BulletSpawnPatternScript.h"
#include "SpawnManager.h"
#include "Object.h"

BulletSpawnPatternScript::BulletSpawnPatternScript()
	: pOwner(nullptr)
	, spawnPattern(eBulletSpawnPattern::NONE)
	, damage(0)	
	, bReady(false)
{
}

BulletSpawnPatternScript::~BulletSpawnPatternScript()
{
}

void BulletSpawnPatternScript::Initialize(Object* _pOwner)
{
	if ( _pOwner ) 
		pOwner = _pOwner;

	spawnPattern = eBulletSpawnPattern::NONE;
	damage = 0;

	bReady = false;
}

void BulletSpawnPatternScript::ReadyToSpawn(eBulletSpawnPattern _SpawnPattern, int _damage)
{
	spawnPattern = _SpawnPattern;
	damage = _damage;

	bReady = true;
}

void BulletSpawnPatternScript::Run()
{
	if ( !pOwner || spawnPattern == eBulletSpawnPattern::NONE ) 
		return;

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
		case eBulletSpawnPattern::SPIN:
		{
			//** Spin ���� : 360�� ȸ���ϸ� ���������� Bullet ����

			int maxCycleCount = 500;		// ** �ִ� �ߵ� Ƚ��
			int spawnCycleTime = 10;		// ** �ߵ� �ð� ����
			int angleGap = 30;				// ** AngleGap : �Ѿ˰� ����(����)

			if ( spawnTime + spawnCycleTime < GetTickCount64() )
			{
				spawnTime = GetTickCount64();

				int bulletCount = 360 / angleGap;

				// ** ��� ���� ���� ����
				int angle = angleGap + (cycleCount * 13);

				// ** Bullet�� TransInfo ����
				Transform bulletTransInfo;
				bulletTransInfo.Position = pOwner->GetPosition();
				bulletTransInfo.Scale = Vector3(10.0f, 10.0f);
				bulletTransInfo.Direction = Vector3(cosf(angle * PI / 180), -sinf(angle * PI / 180));

				// ** Bullet�� Speed ����
				float bulletSpeed = 3.0f;

				// ** Bullet Spawn
				SpawnManager::SpawnBullet(pOwner, bulletTransInfo, bulletSpeed, damage, eBulletType::NORMAL);

				++cycleCount;
			}

			// ** ���� ���� �� �ʱ�ȭ
			if ( maxCycleCount < cycleCount )
			{
				Initialize();
			}
			break;
		}			
		case eBulletSpawnPattern::MULTI_SPIN:
		{
			//** Multi Spin ���� : 360�� ���� ���� �������� ������������ ������ ��� ���������� Spin ������ ����

			int maxCycleCount = 50;		// ** �ִ� �ߵ� Ƚ��
			int spawnCycleTime = 50;		// ** �ߵ� �ð� ����
			int angleGap = 30;				// ** AngleGap : �Ѿ˰� ����(����)

			if ( spawnTime + spawnCycleTime < GetTickCount64() )
			{
				spawnTime = GetTickCount64();

				int bulletCount = 360 / angleGap;
				for ( int i = 0; i < bulletCount; ++i )
				{
					// ** ��� ���� ���� ����
					int angle = angleGap * i + (cycleCount * 7);

					// ** Bullet�� TransInfo ����
					Transform bulletTransInfo;
					bulletTransInfo.Position = pOwner->GetPosition();
					bulletTransInfo.Scale = Vector3(10.0f, 10.0f);
					bulletTransInfo.Direction = Vector3(cosf(angle * PI / 180), -sinf(angle * PI / 180));

					// ** Bullet�� Speed ����
					float bulletSpeed = 3.0f;

					// ** Bullet Spawn
					SpawnManager::SpawnBullet(pOwner, bulletTransInfo, bulletSpeed, damage, eBulletType::NORMAL);
				}

				++cycleCount;
			}

			// ** ���� ���� �� �ʱ�ȭ
			if ( maxCycleCount < cycleCount )
			{
				Initialize();
			}
			break;
		}
		case eBulletSpawnPattern::CIRCLE:
		{
			//** �� ���·� �߻�

			int maxCycleCount = 5;		// ** �ִ� �ߵ� Ƚ��
			int spawnCycleTime = 500;		// ** �ߵ� �ð� ����
			int angleGap = 13;				// ** AngleGap : �Ѿ˰� ����(����)

			if ( spawnTime + spawnCycleTime < GetTickCount64() )
			{
				spawnTime = GetTickCount64();

				int bulletCount = 360 / angleGap;
				for ( int i = 0; i < bulletCount; ++i )
				{
					// ** ��� ���� ���� ����
					int angle = angleGap * i;

					// ** Bullet�� TransInfo ����
					Transform bulletTransInfo;
					bulletTransInfo.Position = pOwner->GetPosition();
					bulletTransInfo.Scale = Vector3(10.0f, 10.0f);
					bulletTransInfo.Direction = Vector3(cosf(angle * PI / 180), -sinf(angle * PI / 180));

					// ** Bullet�� Speed ����
					float bulletSpeed = 3.0f;

					// ** Bullet Spawn
					SpawnManager::SpawnBullet(pOwner, bulletTransInfo, bulletSpeed, damage, eBulletType::NORMAL);
				}

				++cycleCount;
			}

			// ** ���� ���� �� �ʱ�ȭ
			if ( maxCycleCount < cycleCount )
			{
				Initialize();
			}
			break;
		}
	}

	//** ���� ������ ���� ������ ��
	prevSpawnPattern = spawnPattern;
}
