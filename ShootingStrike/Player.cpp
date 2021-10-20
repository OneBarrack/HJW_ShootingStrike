#include "Player.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "BitmapManager.h"
#include "GameDataManager.h"
#include "Bridge.h"
#include "NormalBullet.h"
#include "MathManager.h"
#include "SpawnManager.h"
#include "Enemy.h"
#include "Background.h"
#include "GuideBullet.h"

Player::Player()
	: life(0)
	, damage(0)
	, level(0)
	, isSpawing(false)
	, bReSpawn(false)
	, isDied(false)
	, oldPosition(Vector3())
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

	key = eObjectKey::PLAYER;
	status = eObjectStatus::DEACTIVATED;
	collisionType = eCollisionType::ELLIPSE;
	oldPosition = transInfo.Position;
	bGenerateCollisionEvent = true;

	bulletScript.Initialize();

	life = 3;
	damage = 1;
	level = 1;

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
		#ifdef GAME_DEBUG_MODE
		// _Debug_
		if ( CHECK_KEYINPUT_STATE(eInputKey::KEY_ENTER, eKeyInputState::DOWN) )
		{
			level++;
		}
		#endif // GAME_DEBUG_MODE

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
		if ( CHECK_KEYINPUT_STATE(eInputKey::KEY_SPACE, eKeyInputState::PRESSED) )
		{
			static ULONGLONG fireTime = 0; 
			int fireDelay = 100;

			if ( fireTime + fireDelay < GetTickCount64() )
			{
				fireTime = GetTickCount64();

				Fire(level, damage);
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

void Player::Fire(int _power, int _damage)
{
	Object* pObject = nullptr;
	Bridge* pBridge = nullptr;

	// ** Bullet ���� ����
	int bulletGap = 15;

	// ** ���� ���� Bullet�� x Offet
	int startBulletOffsetX = static_cast<int>(-bulletGap * 0.5 * (_power - 1));

	switch ( _power )
	{
		case 1:
		{
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
			int bulletGap = 15;
			int startBulletOffset = static_cast<int>(-bulletGap * 0.5 * (_power - 1));

			for ( int i = 0; i < 2; ++i )
			{
				// ** ��� ���� ���� ����
				int bulletOffset = startBulletOffset + (bulletGap * i);

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
			// ** 3���� Bullet �߻�
			for ( int i = 0; i < 3; ++i )
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
			// ** 4���� Bullet �߻�
			for ( int i = 0; i < 4; ++i )
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
			// ** 4���� Bullet �߻�
			for ( int i = 0; i < 4; ++i )
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

void Player::ApplyDamage(Object* _pTarget, int _damage)
{
	// ** �������� ����
	switch ( _pTarget->GetKey() )
	{
		case eObjectKey::ENEMY:
		{
			Enemy* pEnemy = static_cast<Enemy*>(_pTarget);
			pEnemy->TakeDamage(_damage);
			
			// ** Enemy�� �׾��ٸ� DeathPoint, Hit�� �ߴٸ� HitPoint Score�� �޾ƿ´�.			
			int score = pEnemy->IsDead() ? pEnemy->GetDeathPoint() : pEnemy->GetHitPoint();
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
		life--;
		Die();
	}
}

void Player::Spawn()
{
	isSpawing = true;
	status = eObjectStatus::ACTIVATED;

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
	// ** bDied flag true ����
	isDied = true;

	// ** ���� ����Ʈ ����
	Transform explosionTransInfo;
	explosionTransInfo.Position = transInfo.Position;
	explosionTransInfo.Scale = transInfo.Scale * 2.0f;
	SpawnManager::SpawnEffect(explosionTransInfo, eBridgeKey::EFFECT_EXPLOSION);
}

void Player::CheckStatus()
{
	// ** �׾��� ��
	if ( isDied )
	{
		// ** ü�°� ���� �ʱ�ȭ		
		level = 1;

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
		// ** �浹 ��Ȱ��ȭ
		bGenerateCollisionEvent = false;
		
		// ** ���� ������ �� ���� ���� �� �浹 Ȱ��ȭ
		if ( invincibleTimer + invicibleDurationTime < GetTickCount64() )
		{
			isInvicible = false;
			bGenerateCollisionEvent = true;
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
}