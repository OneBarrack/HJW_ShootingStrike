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

#include "SpreadAfterDelayBullet.h"

Player::Player()
	: life(0)
	, damage(0)
	, level(0)
	, firingType(eFiringType::NORMAL)
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
	collider.Scale = Vector3(15.0f, 15.0f);

	key = eObjectKey::PLAYER;
	status = eObjectStatus::DEACTIVATED;
	collisionType = eCollisionType::ELLIPSE;
	oldPosition = transInfo.Position;
	bGenerateCollisionEvent = true;

	bulletScript.Initialize();

	life = 3;
	damage = 1;
	level = 1;
	firingType = eFiringType::NORMAL;

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
		if ( CHECK_KEYINPUT_STATE(eInputKey::KEY_SPACE, eKeyInputState::DOWN) )
		{
			//bulletScript.ReadyToSpawn(eBulletSpawnPattern::SPIN, damage);
			//bulletScript.ReadyToSpawn(eBulletSpawnPattern::MULTI_SPIN, damage);			
			firingType = eFiringType::NORMAL;
			Fire(firingType, level, damage);
		}
	}

	// ** Direction ����
	transInfo.Direction = MathManager::GetDirection(oldPosition, transInfo.Position);

	// ** Stage ������ ������� üũ
	CheckPositionInBkgBoundary();

	// ** ���� ��ġ ���� ����
	oldPosition = transInfo.Position;

	// ** �浹ü ����
	SetCollider(Vector3(transInfo.Position.x + 1, transInfo.Position.y), Vector3(15.0f, 15.0f));

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

void Player::Fire(eFiringType _firingType, int _level, int _damage)
{
	Bridge* pBridge = nullptr;

	switch ( _firingType )
	{
		case eFiringType::NORMAL:
		{
			int bulletGap = 10;

			int startBulletOffset = static_cast<int>(-bulletGap * 0.5 * (_level - 1));

			for ( int i = 0; i < _level; ++i )
			{
				// ** ��� ���� ���� ����
				int bulletOffset = startBulletOffset + (bulletGap * i);

				// ** Bullet�� TransInfo ����
				Transform bulletTransInfo;
				bulletTransInfo.Position.x = transInfo.Position.x - bulletOffset;
				bulletTransInfo.Position.y = transInfo.Position.y - (transInfo.Scale.y * 0.5f);
				bulletTransInfo.Scale = Vector3(10.0f, 10.0f);
				bulletTransInfo.Direction = Vector3(0.0f, -1.0f);

				// ** Bullet�� Speed ����
				float bulletSpeed = 3.0f;

				// ** Bullet Spawn
				SpawnManager::SpawnBullet(this, bulletTransInfo, bulletSpeed, _damage, eBridgeKey::BULLET_NORMAL);
			}

			// ** Level ��ŭ �Ѿ� ���ڸ� �ø���, ��� ���� �Ѿ� �� ���ݿ� ���� ������ �����Ͽ�
			// ** ��ä�� ���·� �߻�ǵ��� �Ѵ�.
			// ** angleGap : �Ѿ˰� ���� ����
			//int angleGap = 10;
			//int startAngle = 90 - static_cast<int>(angleGap * 0.5 * (_level - 1));
			//
			//for ( int i = 0; i < _level; ++i )
			//{
			//	// ** ��� ���� ���� ����
			//	int angle = startAngle + (angleGap * i);

			//	// ** Bullet�� TransInfo ����
			//	Transform bulletTransInfo;
			//	bulletTransInfo.Position  = transInfo.Position;
			//	bulletTransInfo.Scale	  = Vector3(10.0f, 10.0f);
			//	bulletTransInfo.Direction = Vector3(cosf(angle * PI / 180), -sinf(angle * PI / 180));
			//	
			//	// ** Bullet�� Speed ����
			//	float bulletSpeed = 3.0f;
			//	
			//	// ** Bullet Spawn
			//	SpawnManager::SpawnBullet(this, bulletTransInfo, bulletSpeed, _damage, eBridgeKey::BULLET_NORMAL);
			//}
			break;
		} 		
		case eFiringType::GUIDE:
		{
			float bulletSpeed = 3.0f;

			// ** Bullet�� TransInfo ����
			Transform bulletTransInfo;
			bulletTransInfo.Position = transInfo.Position;
			bulletTransInfo.Scale = Vector3(10.0f, 10.0f);
			bulletTransInfo.Direction = Vector3(0.0f, -1.0f);			

			// ** Bullet Spawn
			SpawnManager::SpawnBullet(this, bulletTransInfo, bulletSpeed, _damage, eBridgeKey::BULLET_GUIDE);
			break;
		}
		default:
			break;
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