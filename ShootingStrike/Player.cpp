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

Player::Player()
	: HP(0)
	, damage(0)
	, level(0)
	, bulletType(eBulletType::NORMAL)
	, bSpawing(false)
	, bAttacking(false)
	, bTakeDamage(false)
	, bDied(false)
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

	collider.Position = transInfo.Position;
	collider.Scale = transInfo.Scale;

	key = eObjectKey::PLAYER;
	status = eObjectStatus::DEACTIVATED;
	collisionType = eCollisionType::RECT;
	oldPosition = transInfo.Position;
	bGenerateCollisionEvent = true;

	HP = 3;
	damage = 1;
	level = 1;
	bulletType = eBulletType::NORMAL;

	bSpawing = false;
	bAttacking = false;
	bTakeDamage = false;
	bDied = false;

	speed = 3.0f;
}

void Player::Update()
{
	Super::Update();

	// ** ���� �۾� �ʿ�

	if ( bDied )
	{
		HP = 3;
		level = 1;
		bDied = false;
		bSpawing = true;
	}

	// ** ���� ��
	if ( bSpawing )
	{
		bGenerateCollisionEvent = false;
	}
	else
	{
		bGenerateCollisionEvent = true;
		// ** ������
		if ( bAttacking )
		{
			// ...
		}

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
			Fire(bulletType, level, damage);
		}
		else
		{
			bAttacking = false;
		}
	}

	// ** Direction ����
	transInfo.Direction = MathManager::GetDirection(oldPosition, transInfo.Position);

	// ** Stage ������ ������� üũ
	CheckPositionInBkgBoundary();

	// ** ���� ��ġ ���� ����
	oldPosition = transInfo.Position;

	// ** �浹ü ����
	SetCollider(transInfo);

	return;
}

void Player::Render(HDC _hdc)
{
	Super::Render(_hdc);

	// ** ���� ��
	if ( bSpawing )
	{
		if ( RenderSpawn(_hdc) )			
			bSpawing = false;

		return;
	}

	// ** ����
	if ( bDied )
	{		
		return;
	}
		
	// ** ���ظ� �Ծ��� ��
	if ( bTakeDamage )
	{

	}

	// ** �Ϲ� ���
	RenderPlayer(_hdc);	
}

void Player::Fire(eBulletType _firingType, int _level, int _damage)
{
	Bridge* pBridge = nullptr;

	switch ( _firingType )
	{
		case eBulletType::NORMAL:
		{
			// ** Level ��ŭ �Ѿ� ���ڸ� �ø���, ��� ���� �Ѿ� �� ���ݿ� ���� ������ �����Ͽ�
			// ** ��ä�� ���·� �߻�ǵ��� �Ѵ�.
			// ** angleGap : �Ѿ˰� ���� ����
			int angleGap = 10;
			int startAngle = 90 - static_cast<int>(angleGap * 0.5 * (_level - 1));
			
			for ( int i = 0; i < _level; ++i )
			{
				// ** ��� ���� ���� ����
				int angle = startAngle + (angleGap * i);

				// ** Bullet�� TransInfo ����
				Transform bulletTransInfo;
				bulletTransInfo.Position  = transInfo.Position;
				bulletTransInfo.Scale	  = Vector3(10.0f, 10.0f);
				bulletTransInfo.Direction = Vector3(cosf(angle * PI / 180), -sinf(angle * PI / 180));
				
				// ** Bullet�� Speed ����
				float bulletSpeed = 3.0f;

				// ** Bullet Spawn
				SpawnManager::SpawnBullet(this, bulletTransInfo, bulletSpeed, _damage, _firingType);
			}
			break;
		} 		
		case eBulletType::GUIDE:
			break;
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
	HP -= _damage;
	bTakeDamage = true;

	if ( HP <= 0 )
	{
		HP = 0;
		bDied = true;
	}
}

void Player::CheckPositionInBkgBoundary()
{
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

bool Player::RenderSpawn(HDC _hdc)
{
	if ( !pImage ) return false;

	float moveSpeed = 0.5f;

	// ** ȭ�� �Ʒ��������� ���� ��ġ���� ������ �ö������ �׸�	
	static float movePositionY = WINDOWS_HEIGHT + 30;	
	float stopPositionY = WINDOWS_HEIGHT - 100;

	transInfo.Position.x = WINDOWS_WIDTH * 0.5f;
	transInfo.Position.y = movePositionY;

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

	if ( movePositionY < stopPositionY )
	{
		movePositionY = WINDOWS_HEIGHT + 30;
		return true;
	}

	// ** Speed��ŭ ��ġ�� ����
	movePositionY -= moveSpeed;
	return false;
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