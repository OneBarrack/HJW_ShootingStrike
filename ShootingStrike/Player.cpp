#include "Player.h"
#include "Bridge.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "BitmapManager.h"
#include "GameDataManager.h"
#include "SoundManager.h"
#include "NormalBullet.h"
#include "MathManager.h"
#include "SpawnManager.h"
#include "Enemy.h"
#include "Item.h"
#include "Background.h"
#include "GuideBullet.h"
#include "Bullet.h"

Player::Player()
	: damage(0)
	, life(0)
	, maxLife(0)
	, power(0)
	, maxPower(0)
	, bomb(0)
	, maxBomb(0)
	, isSpawing(false)
	, bReSpawn(false)
	, isDied(false)
	, isInvicible(false)
	, bCantAccessInput(false)
	, oldPosition(Vector3())
	, cantAccessInputTimer(0)
	, cantAccessInputDurationTime(0)
	, invincibleTimer(0)
	, invicibleDurationTime(0)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	Super::Initialize();

	pImage = BitmapManager::GetInstance()->GetImage(eImageKey::PLAYER);
	tagName = eTagName::PLAYER_FLIGHT1;	

	transInfo.Position = Vector3(WINDOWS_WIDTH * 0.5f, WINDOWS_HEIGHT * 0.5f);
	transInfo.Scale = Vector3(42.0f, 47.0f);

	collider.Position = Vector3(transInfo.Position.x + 3, transInfo.Position.y);
	collider.Scale = Vector3(12.0f, 12.0f);

	objectKey = eObjectKey::PLAYER;
	objectStatus = eObjectStatus::DEACTIVATED;
	collisionType = eCollisionType::ELLIPSE;
	oldPosition = transInfo.Position;
	bGenerateCollisionEvent = true;

	bulletScript.Initialize();

	damage = 1;
	life = 3;
	maxLife = 10;
	power = 1;
	maxPower = 5;
	bomb = 2;
	maxBomb = 2;

	isSpawing = false;
	isDied = false;

	speed = 3.0f;

	cantAccessInputTimer = 0;
	cantAccessInputDurationTime = 0;
	invincibleTimer = 0;
	invicibleDurationTime = 0;
}

void Player::Update()
{
	if ( life == 0 )
		return;

	Super::Update();

	// ** ���� �÷��̾� ���� üũ
	CheckStatus();
		
	if ( isSpawing )
	{
		// ** �̵� �ӵ�
		float moveSpeed = 0.5f;
		
		// ** ���� ��ġ
		Background* pStageBackground = static_cast<Background*>(
			ObjectManager::GetInstance()->FindObjectWithTag(eTagName::STAGE_MAIN_BKG));
		float stopPositionY = (pStageBackground->GetPosition().y + (pStageBackground->GetScale().y * 0.5f)) - 100;

		// ** ȭ�� �Ʒ��������� ���� ��ġ���� ������ �ö������
		if ( transInfo.Position.y - moveSpeed > stopPositionY )
		{
			transInfo.Position.y -= moveSpeed;
			
		}
		// ** ���� ��ġ�� ����
		else
		{
			// ** Spawn ���� �÷��� ��� ��Ȱ��ȭ �� Ű �Է� �����ϵ��� ����
			transInfo.Position.y = stopPositionY;
			isSpawing = false;
			bReSpawn = false;
			bCantAccessInput = false;
		}
	}

	// ** Ű �Է��� ���� ���°� �ƴ϶��
	if ( !bCantAccessInput )
	{
		if ( CHECK_KEYINPUT_STATE(eInputKey::KEY_LEFT, eKeyInputState::PRESSED) )
		{
			transInfo.Position.x -= 3;
		}
		if ( CHECK_KEYINPUT_STATE(eInputKey::KEY_UP, eKeyInputState::PRESSED) )
		{
			transInfo.Position.y -= 3;
		}
		if ( CHECK_KEYINPUT_STATE(eInputKey::KEY_RIGHT, eKeyInputState::PRESSED) )
		{
			transInfo.Position.x += 3;
		}
		if ( CHECK_KEYINPUT_STATE(eInputKey::KEY_DOWN, eKeyInputState::PRESSED) )
		{
			transInfo.Position.y += 3;
		}

		// ** �̻��� �߻�
		if ( CHECK_KEYINPUT_STATE(eInputKey::KEY_Z, eKeyInputState::PRESSED) )
		{
			static ULONGLONG fireTime = 0; 
			int fireDelay = 100;

			if ( fireTime + fireDelay < GetTickCount64() )
			{
				fireTime = GetTickCount64();

				Fire(power, damage);
			}
		}

		// ** Bomb Skill �ߵ�
		if ( CHECK_KEYINPUT_STATE(eInputKey::KEY_X, eKeyInputState::DOWN) )
		{
			static ULONGLONG bombTime = 0;
			int bombDelay = 1500;

			if ( bombTime + bombDelay < GetTickCount64() )
			{
				bombTime = GetTickCount64();

				ActivateBomb();
			}			
		}
	}

	// ** Direction ����
	transInfo.Direction = MathManager::GetDirection(oldPosition, transInfo.Position);

	// ** Stage ������ ������� üũ
	CheckPositionInBkgBoundary();

	// ** ���� ��ġ ���� ����
	oldPosition = transInfo.Position;

	// ** �浹ü ����
	SetCollider(Vector3(transInfo.Position.x + 1, transInfo.Position.y), Vector3(12.0f, 12.0f));

	// ** Bullet Spawn Pattern Script ����
	bulletScript.Run();

	return;
}

void Player::Render(HDC _hdc)
{
	if ( life == 0 )
		return;

	Super::Render(_hdc);

	// ** ���� ��
	if ( isSpawing )
	{
		RenderSpawn(_hdc);		
		return;
	}

	// ** ����
	if ( isDied )
	{		
		return;
	}		

	// ** �Ϲ� ���
	RenderPlayer(_hdc);	
}

void Player::PowerUp(bool _bMax)
{	
	++power;

	if ( _bMax || maxPower < power )
		power = maxPower;
}

void Player::PowerDown(bool _bMin)
{
	--power;

	if ( _bMin || power < 1 )
		power = 1;
}

void Player::LifeUp(bool _bMax)
{
	life++;

	if ( _bMax || maxLife < life )
		life = maxLife;
}

void Player::LifeDown(bool _bMin)
{
	--life;

	if ( _bMin || life < 0 )
		life = 0;
}

void Player::BombUp(bool _bMax)
{
	bomb++;

	if ( _bMax || maxBomb < bomb )
		bomb = maxBomb;
}

void Player::BombDown(bool _bMin)
{
	--bomb;

	if ( _bMin || bomb < 0 )
		bomb = 0;
}

void Player::Fire(int _power, int _damage)
{
	// ** Play Player Fire Sound
	SoundManager::GetInstance()->Play(eSoundKey::EFFECT_PLAYER_FIRE);

	Object* pObject = nullptr;
	Bridge* pBridge = nullptr;	

	switch ( _power )
	{
		case 1:
		{
			// ** �� ���� �߻�� Normal Bullet ����
			int normalBulletCount = 1;

			// ** Bullet ���� ����
			int bulletGap = 15;

			// ** ���� ���� Bullet�� x Offet
			int startBulletOffsetX = static_cast<int>(-bulletGap * 0.5 * (normalBulletCount - 1));

			// ** Bullet�� TransInfo ����
			Transform bulletTransInfo;
			bulletTransInfo.Position.x = transInfo.Position.x;
			bulletTransInfo.Position.y = transInfo.Position.y - (transInfo.Scale.y * 0.5f);
			bulletTransInfo.Scale = Vector3(15.0f, 15.0f);
			bulletTransInfo.Direction = Vector3(0.0f, -1.0f);

			// ** Bullet�� Speed ����
			float bulletSpeed = 10.0f;

			// ** Bullet Spawn
			pObject = SpawnManager::SpawnBullet(this, bulletTransInfo, bulletSpeed, _damage, eBridgeKey::BULLET_NORMAL);
			pObject->SetImageOffsetOrder(Point(1, 2));
			break;
		}
		case 2:
		{
			// ** �� ���� �߻�� Normal Bullet ����
			int normalBulletCount = 2;

			// ** Bullet ���� ����
			int bulletGap = 15;

			// ** ���� ���� Bullet�� x Offet
			int startBulletOffsetX = static_cast<int>(-bulletGap * 0.5 * (normalBulletCount - 1));

			// ** 2���� Normal Bullet �߻�
			for ( int i = 0; i < normalBulletCount; ++i )
			{
				// ** ��� ���� ���� ����
				int bulletOffset = startBulletOffsetX + (bulletGap * i);

				// ** Bullet�� TransInfo ����
				Transform bulletTransInfo;
				bulletTransInfo.Position.x = transInfo.Position.x - bulletOffset;
				bulletTransInfo.Position.y = transInfo.Position.y - (transInfo.Scale.y * 0.5f);
				bulletTransInfo.Scale = Vector3(15.0f, 15.0f);
				bulletTransInfo.Direction = Vector3(0.0f, -1.0f);

				// ** Bullet�� Speed ����
				float bulletSpeed = 10.0f;

				// ** Bullet Spawn
				pObject = SpawnManager::SpawnBullet(this, bulletTransInfo, bulletSpeed, _damage, eBridgeKey::BULLET_NORMAL);
				pObject->SetImageOffsetOrder(Point(1, 2));
			}
			break;
		}
		case 3:
		{
			// ** �� ���� �߻�� Normal Bullet ����
			int normalBulletCount = 3;

			// ** Bullet ���� ����
			int bulletGap = 15;

			// ** ���� ���� Bullet�� x Offet
			int startBulletOffsetX = static_cast<int>(-bulletGap * 0.5 * (normalBulletCount - 1));

			// ** 3���� Noamal Bullet �߻�
			for ( int i = 0; i < normalBulletCount; ++i )
			{
				// ** ��� ���� ���� ����
				int bulletOffset = startBulletOffsetX + (bulletGap * i);

				// ** Bullet�� TransInfo ����
				Transform bulletTransInfo;
				bulletTransInfo.Position.x = transInfo.Position.x - bulletOffset;												

				// ** ������ ���� Bullet�� Position�� ���ͱ��� ��¦ �ڷ� ��´�
				if ( i == 0 || i == 2 ) // ** ��/��
					bulletTransInfo.Position.y = transInfo.Position.y - (transInfo.Scale.y * 0.3f);
				else // ** ����
					bulletTransInfo.Position.y = transInfo.Position.y - (transInfo.Scale.y * 0.5f);
							
				bulletTransInfo.Scale = Vector3(15.0f, 15.0f);
				bulletTransInfo.Direction = Vector3(0.0f, -1.0f);

				// ** Bullet�� Speed ����
				float bulletSpeed = 10.0f;

				// ** Bullet Spawn
				pObject = SpawnManager::SpawnBullet(this, bulletTransInfo, bulletSpeed, _damage, eBridgeKey::BULLET_NORMAL);
				pObject->SetImageOffsetOrder(Point(1, 2));
			}			
			break;
		}
		case 4:
		{
			// ** �� ���� �߻�� Normal Bullet ����
			int normalBulletCount = 4;

			// ** Bullet ���� ����
			int bulletGap = 15;

			// ** ���� ���� Bullet�� x Offet
			int startBulletOffsetX = static_cast<int>(-bulletGap * 0.5 * (normalBulletCount - 1));

			// ** 4���� Normal Bullet �߻�
			for ( int i = 0; i < normalBulletCount; ++i )
			{
				// ** ��� ���� ���� ����
				int bulletOffset = startBulletOffsetX + (bulletGap * i);

				// ** Bullet�� TransInfo ����
				Transform bulletTransInfo;
				bulletTransInfo.Position.x = transInfo.Position.x - bulletOffset;

				// ** ������ ���� Bullet�� Position�� ���ͱ��� ��¦ �ڷ� ��´�
				if ( i == 0 || i == 3 ) // ** ��/��
					bulletTransInfo.Position.y = transInfo.Position.y - (transInfo.Scale.y * 0.3f);
				else // ** ����
					bulletTransInfo.Position.y = transInfo.Position.y - (transInfo.Scale.y * 0.5f);

				bulletTransInfo.Scale = Vector3(15.0f, 15.0f);
				bulletTransInfo.Direction = Vector3(0.0f, -1.0f);

				// ** Bullet�� Speed ����
				float bulletSpeed = 10.0f;

				// ** Bullet Spawn
				pObject = SpawnManager::SpawnBullet(this, bulletTransInfo, bulletSpeed, _damage, eBridgeKey::BULLET_NORMAL);
				pObject->SetImageOffsetOrder(Point(1, 2));
			}
			break;
		}
		case 5:
		{
			// ** �� ���� �߻�� Normal Bullet ����
			int normalBulletCount = 4;

			// ** Bullet ���� ����
			int bulletGap = 15;

			// ** ���� ���� Bullet�� x Offet
			int startBulletOffsetX = static_cast<int>(-bulletGap * 0.5 * (normalBulletCount - 1));

			// ** 4���� Normal Bullet �߻�
			for ( int i = 0; i < normalBulletCount; ++i )
			{
				// ** ��� ���� ���� ����
				int bulletOffset = startBulletOffsetX + (bulletGap * i);

				// ** Bullet�� TransInfo ����
				Transform bulletTransInfo;
				bulletTransInfo.Position.x = transInfo.Position.x - bulletOffset;

				// ** ������ ���� Bullet�� Position�� ���ͱ��� ��¦ �ڷ� ��´�
				if ( i == 0 || i == 3 ) // ** ��/��
					bulletTransInfo.Position.y = transInfo.Position.y - (transInfo.Scale.y * 0.3f);
				else // ** ����
					bulletTransInfo.Position.y = transInfo.Position.y - (transInfo.Scale.y * 0.5f);

				bulletTransInfo.Scale = Vector3(15.0f, 15.0f);
				bulletTransInfo.Direction = Vector3(0.0f, -1.0f);

				// ** Bullet�� Speed ����
				float bulletSpeed = 10.0f;

				// ** Bullet Spawn
				pObject = SpawnManager::SpawnBullet(this, bulletTransInfo, bulletSpeed, _damage, eBridgeKey::BULLET_NORMAL);
				pObject->SetImageOffsetOrder(Point(1, 2));
			}

			// ** �¿�� Guide Bullet ����
			{
				static ULONGLONG guideBulletSpawnTime = 0;
				int guideBulletSpawnDelay = 500;

				float bulletSpeed = 3.0f;

				if ( guideBulletSpawnTime + guideBulletSpawnDelay < GetTickCount64() )
				{
					guideBulletSpawnTime = GetTickCount64();

					// ** Delay Time�� ���� �ʿ��� ����, �ӵ�, �ִ�ӵ�, ���ӵ� ����
					int guideDelay = 200;
					Vector3 directionAfterDelay = Vector3(0.0, -1.0f);
					float speedAfterDelay = 1.0f;
					float maxSpeed = 10.0f;
					float accelerationAfterDelay = 0.3f;					

					// ** Bullet�� TransInfo ����
					Transform bulletTransInfo;
					bulletTransInfo.Position.x = transInfo.Position.x - (transInfo.Scale.x * 0.5f);
					bulletTransInfo.Position.y = transInfo.Position.y;
					bulletTransInfo.Scale = Vector3(15.0f, 15.0f);
					bulletTransInfo.Direction = Vector3(-1.0f, 0.0f);

					// ** Bullet Spawn
					pBridge = ObjectManager::GetInstance()->NewBridge(eBridgeKey::BULLET_GUIDE);
					static_cast<GuideBullet*>(pBridge)->SetDelay(directionAfterDelay, speedAfterDelay, maxSpeed, accelerationAfterDelay, guideDelay);
					static_cast<GuideBullet*>(pBridge)->SetLoopGuide(true);

					pObject = SpawnManager::SpawnBullet(this, bulletTransInfo, bulletSpeed, _damage, pBridge);
					pObject->SetImageOffsetOrder(Point(0, 1));

					// ** Bullet�� TransInfo ����
					bulletTransInfo.Position.x = transInfo.Position.x + (transInfo.Scale.x * 0.5f);
					bulletTransInfo.Position.y = transInfo.Position.y;
					bulletTransInfo.Scale = Vector3(15.0f, 15.0f);
					bulletTransInfo.Direction = Vector3(1.0f, 0.0f);

					// ** Bullet Spawn
					pBridge = ObjectManager::GetInstance()->NewBridge(eBridgeKey::BULLET_GUIDE);
					static_cast<GuideBullet*>(pBridge)->SetDelay(directionAfterDelay, speedAfterDelay, maxSpeed, accelerationAfterDelay, guideDelay);
					static_cast<GuideBullet*>(pBridge)->SetLoopGuide(true);

					pObject = SpawnManager::SpawnBullet(this, bulletTransInfo, bulletSpeed, _damage, pBridge);
					pObject->SetImageOffsetOrder(Point(0, 1));
				}
			}
			break;
		}
	}
}

void Player::ActivateBomb(bool _bPlayAnimation)
{
	// ** Bomb�� �����ִٸ�
	if ( bomb > 0 )
	{
		// ** Play Bomb Sound
		SoundManager::GetInstance()->Play(eSoundKey::EFFECT_PLAYER_BOMB);

		// ** Player�� Bullet�� None/Boss Enemy�� ������ Bullet���� ��� Coin Item���� ��ȯ��Ų��.
		
		// ** ��� Enemy�� Ž���Ѵ�.
		list<Object*> enemyList = ObjectManager::GetInstance()->GetObjectList(eObjectKey::ENEMY);
		for ( Object* pEnemy : enemyList )
		{
			// ** Boss Enemy�� �ƴ� Enemy��� 
			if ( static_cast<Enemy*>(pEnemy)->GetEnemyType() != eEnemyType::NONE &&
				static_cast<Enemy*>(pEnemy)->GetEnemyType() != eEnemyType::ENEMY_BOSS_ANGEL )
			{
				// ** Bullet�� Coin Item���� ��ȯ��Ų��.
				Bridge* pBridge = ObjectManager::GetInstance()->NewBridge(eBridgeKey::ITEM_GO_TO_PLAYER);
				Object* pObject = ObjectManager::GetInstance()->NewObject(eObjectKey::ITEM);
				pObject->SetPosition(pEnemy->GetPosition());
				pObject->SetScale(20.0f, 20.0f);
				pObject->SetDirection(Vector3(0.0f, -1.0f));
				pObject->SetSpeed(3.0f);
				pObject->SetAcceleration(0.1f);
				pObject->SetBridge(pBridge);
				static_cast<Item*>(pObject)->SetItemType(eItemType::COIN);
				static_cast<Enemy*>(pEnemy)->Die();
			}
		}

		// ** ��� Bullet�� Ž���Ѵ�.
		list<Object*> bulletList = ObjectManager::GetInstance()->GetObjectList(eObjectKey::BULLET);
		for ( Object* pBullet : bulletList )
		{
			// ** Player�� Bullet�� �ƴ϶�� 
			if ( static_cast<Bullet*>(pBullet)->GetOwner()->GetKey() != eObjectKey::PLAYER )
			{
				// ** Bullet�� Coin Item���� ��ȯ��Ų��.
				Bridge* pBridge = ObjectManager::GetInstance()->NewBridge(eBridgeKey::ITEM_GO_TO_PLAYER);
				Object* pObject = ObjectManager::GetInstance()->NewObject(eObjectKey::ITEM);
				pObject->SetPosition(pBullet->GetPosition());
				pObject->SetScale(20.0f, 20.0f);
				pObject->SetDirection(Vector3(0.0f, -1.0f));
				pObject->SetSpeed(3.0f);
				pObject->SetAcceleration(0.1f);
				pObject->SetBridge(pBridge);
				static_cast<Item*>(pObject)->SetItemType(eItemType::COIN);

				pBullet->SetStatus(eObjectStatus::DESTROYED);
			}
		}

		// ** Bomb ����
		BombDown();
	}
}

void Player::ApplyDamage(Object* _pTarget, int _damage)
{
	// ** �������� ����
	switch ( _pTarget->GetKey() )
	{
		case eObjectKey::ENEMY:
		{
			Enemy* pEnemy = static_cast<Enemy*>(_pTarget);
			pEnemy->TakeDamage(_damage);
			
			int score = 0;

			// ** Enemy�� �׾��ٸ� DeathPoint, Hit�� �ߴٸ� HitPoint Score�� �޾ƿ´�.
			if ( pEnemy->IsDied() )
			{
				score = pEnemy->GetDeathPoint();

				// ** Enemy�� Boss ���ٸ�
				if ( pEnemy->GetEnemyType() == eEnemyType::ENEMY_BOSS_ANGEL )
				{
					// ** Bomb Skill�� �ִϸ��̼� ���� �ߵ����� 
					// ** �����ִ� Bullet���� Coin���� ó���Ͽ� �����Ѵ�.
					BombUp();
					ActivateBomb(false);
				}
			}
			else
			{
				score = pEnemy->GetHitPoint();
			}
			
			GameDataManager::GetInstance()->AddScore(score);
			break;
		}		
		default:
			break;
	}	
}

void Player::TakeDamage(int _damage)
{
	// ** ���� �÷��̾�� HP�� ���� �ѹ濡 ����. Life�� ����
	
	// Die
	if ( !isDied )
	{		
		Die();
	}
}

void Player::Spawn()
{
	isSpawing = true;
	
	// ** �÷��̾� ������Ʈ Ȱ��ȭ �� �浹 Ȱ��ȭ
	SetStatus(eObjectStatus::ACTIVATED);
	bGenerateCollisionEvent = true;

	// ** Power, Bomb �ʱ�ȭ
	PowerDown(true);
	BombUp(true);

	// ** ��ŸƮ ��ġ ����
	Background* pStageBackground = static_cast<Background*>(
		ObjectManager::GetInstance()->FindObjectWithTag(eTagName::STAGE_MAIN_BKG));
			
	transInfo.Position.x = pStageBackground->GetPosition().x;
	transInfo.Position.y = (pStageBackground->GetPosition().y + (pStageBackground->GetScale().y * 0.5f)) + 30;

	// ** Ű �Է��� ����
	bCantAccessInput = true;
	cantAccessInputTimer = GetTickCount64();
	cantAccessInputDurationTime = 5000; // ������ ����Ǹ� Ű �Է� ���������Ƿ� ���⼭�� �˳��� ��Ƶд� 

	// ** ���� ����
	isInvicible = true;
	invincibleTimer = GetTickCount64();
	invicibleDurationTime = 5500;	
}

void Player::ReSpawn()
{
	bReSpawn = true;
	Spawn();
}

void Player::Die()
{
	// ** Play player die sound
	SoundManager::GetInstance()->Play(eSoundKey::EFFECT_PLAYER_DIE);

	// ** bDied flag true ����
	isDied = true;

	// ** �浹 ��Ȱ��ȭ
	bGenerateCollisionEvent = false;

	// ** Life ����
	LifeDown();

	// ** ���� ����Ʈ ����
	Transform explosionTransInfo;
	explosionTransInfo.Position = transInfo.Position;
	explosionTransInfo.Scale = transInfo.Scale * 2.0f;
	SpawnManager::SpawnEffect(explosionTransInfo, eBridgeKey::EFFECT_EXPLOSION);

	// ** ���� �ܰ迡�� ���� ���̶�� Power Max Item ����
	if ( ObjectManager::GetInstance()->FindObjectWithTag(eTagName::ENEMY_BOSS) )
	{
		Bridge* pBridge = ObjectManager::GetInstance()->NewBridge(eBridgeKey::ITEM_BOUNCE_ON_WALL);
		Object* pObject = ObjectManager::GetInstance()->NewObject(eObjectKey::ITEM);
		pObject->SetPosition(transInfo.Position);
		pObject->SetScale(Vector3(40.0f, 40.0f));
		pObject->SetSpeed(2.0f);
		pObject->SetBridge(pBridge);
		static_cast<Item*>(pObject)->SetItemType(eItemType::POWER_MAX);
	}
}

void Player::CheckStatus()
{
	// ** �׾��� ��
	if ( isDied )
	{
		// ** ü�°� ���� �ʱ�ȭ		
		power = 1;

		// ** �� ����
		isDied = false;
		ReSpawn();
	}	

	// ** Ű �Է��� ������ ��
	if ( bCantAccessInput )
	{
		// ** ���� ������ �� Ű �Է� ���
		if ( cantAccessInputTimer + cantAccessInputDurationTime < GetTickCount64() )
		{
			bCantAccessInput = false;
		}
	}

	// ** ������ �� 
	if ( isInvicible )
	{
		// ** ���� ������ �� ���� ����
		if ( invincibleTimer + invicibleDurationTime < GetTickCount64() )
		{
			isInvicible = false;
		}		
	}
}

void Player::CheckPositionInBkgBoundary()
{
	// ** ���� ���� ���� üũ x
	if ( isSpawing )
		return;

	// ** Stage ���� ���
	Object* pBackground = ObjectManager::GetInstance()->FindObjectWithTag(eObjectKey::BACKGROUND, eTagName::STAGE_MAIN_BKG);

	// ** Stage�� �ٿ����
	RectF bkgBoundary = pBackground->GetColliderF();

	// ** �׵θ� ��輱 ���ؿ� ��ü�� �߸��� �ʰ� �ϱ� ���� Offset ��
	float offset = 22.0f;

	// ** ��
	if ( transInfo.Position.x < bkgBoundary.Left + offset )
		transInfo.Position.x = bkgBoundary.Left + offset;
	// ** ��
	if ( transInfo.Position.x > bkgBoundary.Right - offset )
		transInfo.Position.x = bkgBoundary.Right - offset;
	// ** ��
	if ( transInfo.Position.y < bkgBoundary.Top + offset )
		transInfo.Position.y = bkgBoundary.Top + offset;
	// ** ��
	if ( transInfo.Position.y > bkgBoundary.Bottom - offset )
		transInfo.Position.y = bkgBoundary.Bottom - offset;

	cout << "pos: " << transInfo.Position.x << ", " << transInfo.Position.y << endl;
	cout << "boundary: " << bkgBoundary.Left << ", " << bkgBoundary.Top 
		<< ", " << bkgBoundary.Right << ", " << bkgBoundary.Bottom << endl;
}

void Player::RenderSpawn(HDC _hdc)
{
	if ( !pImage ) 
		return;
	
	// ** �׸��� ���� �Ǵ�
	static bool bDraw = true;	
	
	// ** ������ �� ĳ������ �������� �ֱ� ���� flicker �ð�
	static ULONGLONG flickerTime = GetTickCount64();
	int flickerCycleTime = 200;

	// ** �������� ��� 
	if ( bReSpawn ) 		
	{
		// ** ĳ���Ͱ� �����̸鼭 �����ǵ���
		if ( flickerTime + flickerCycleTime < GetTickCount64() )
		{
			flickerTime = GetTickCount64();
			bDraw ^= true;
		}
	}
	else
	{
		bDraw = true;
	}

	// ** Render
	if ( bDraw )
	{
		TransparentBlt(_hdc,
			(int)(transInfo.Position.x - transInfo.Scale.x * 0.5f),
			(int)(transInfo.Position.y - transInfo.Scale.y * 0.5f),
			(int)transInfo.Scale.x,
			(int)transInfo.Scale.y,
			pImage->GetMemDC(),
			(int)pImage->GetSegmentationScale().x,
			(int)0,
			(int)pImage->GetSegmentationScale().x,
			(int)pImage->GetSegmentationScale().y,
			RGB(255, 0, 255));
	}
}

void Player::RenderPlayer(HDC _hdc)
{
	if ( !pImage ) return;

	static POINT segmentOffset = { 0, 0 };

	// ** Direction���� ������ üũ�Ͽ� �ش��ϴ� Offset x�� ����
	if ( transInfo.Direction.x < 0.0f )		 segmentOffset.x = 0; // ** Left
	else if ( transInfo.Direction.x > 0.0f ) segmentOffset.x = 2; // ** Right
	else									 segmentOffset.x = 1; // ** None(Center)

	// ** ����� ���� �Ҳ� �ִϸ��̼��� ���� Offset.y���� 1, 2 ���ѹݺ� ��Ŵ
	segmentOffset.y = (segmentOffset.y < 2) ? 2 : 1;

	TransparentBlt(_hdc,
		(int)transInfo.Position.x - (int)(transInfo.Scale.x * 0.5f),
		(int)transInfo.Position.y - (int)(transInfo.Scale.y * 0.5f),
		(int)transInfo.Scale.x,
		(int)transInfo.Scale.y,
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * segmentOffset.x),
		(int)(pImage->GetSegmentationScale().y * segmentOffset.y),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(255, 0, 255));
}

void Player::Release()
{
	Super::Release();
}

void Player::OnCollision(Object* _pObject)
{
	// ** Item �� �浹 ��
	if ( _pObject->GetKey() == eObjectKey::ITEM )
	{
		eItemType itemType = static_cast<Item*>(_pObject)->GetItemType();
		if ( itemType != eItemType::NONE )
		{
			SoundManager::GetInstance()->Play(eSoundKey::EFFECT_GET_ITEM);
			switch ( itemType )
			{
				case eItemType::POWER_MAX: PowerUp(true); break;
				case eItemType::POWER_UP:  PowerUp();	  break;
				case eItemType::LIFE_UP:   LifeUp();	  break;
				case eItemType::COIN: GameDataManager::GetInstance()->AddScore(258); break;
			}
		}
	}
}