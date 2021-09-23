#include "Player.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "BitmapManager.h"
#include "Bridge.h"
#include "NormalBullet.h"
#include "MathManager.h"
#include "SpawnManager.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::Initialize()
{
	pPlayerImage = BitmapManager::GetInstance()->GetImage(eImageKey::PLAYER);

	TransInfo.Position = Vector3(WindowsWidth * 0.5f, WindowsHeight * 0.5f);
	TransInfo.Scale = Vector3(42.0f, 47.0f);

	Collider.Position = TransInfo.Position;
	Collider.Scale = TransInfo.Scale;

	Key = eObjectKey::PLAYER;
	Status = eObjectStatus::DEACTIVATED;
	CollisionType = eCollisionType::RECT;
	OldPosition = TransInfo.Position;
	bGenerateCollisionEvent = true;

	HP = 3;
	Damage = 1;
	Power = 1;
	FireType = eBulletFireType::Normal;

	bSpawing = false;
	bAttacking = false;
	bTakeDamage = false;
	bDied = false;

	Speed = 3.0f;
	Frame = 0;
	

	Offset = Vector3(95.0f, -85.0f);
}

void Player::Update()
{
	Frame++;

	// ** ���� ��
	if ( bSpawing )
		return;

	// ** ü���� 0 ������ �� Die
	if ( HP <= 0 )
	{
		bDied = true;
		return;
	}

	// ** ������
	if ( bAttacking )
	{
		// ...
	}

	// _Debug_
	if ( CheckKeyInputStatus(eInputKey::KEY_ENTER, eKeyInputStatus::DOWN) )
	{
		Power++;
	}

	if ( CheckKeyInputStatus(eInputKey::KEY_LEFT, eKeyInputStatus::PRESSED) )
	{
		TransInfo.Position.x -= 3;
	}
	if ( CheckKeyInputStatus(eInputKey::KEY_UP, eKeyInputStatus::PRESSED) )
	{
		TransInfo.Position.y -= 3;
	}
	if ( CheckKeyInputStatus(eInputKey::KEY_RIGHT, eKeyInputStatus::PRESSED) )
	{
		TransInfo.Position.x += 3;
	}
	if ( CheckKeyInputStatus(eInputKey::KEY_DOWN, eKeyInputStatus::PRESSED) )
	{
		TransInfo.Position.y += 3;
	}

	// ** �̻��� �߻�
	if ( CheckKeyInputStatus(eInputKey::KEY_SPACE, eKeyInputStatus::DOWN) )
	{
		Fire(FireType, Power, Damage);
	}
	else
	{
		bAttacking = false;
	}

	// ** Direction ����
	TransInfo.Direction = MathManager::GetDirection(OldPosition, TransInfo.Position);

	// ** ���� ��ġ ���� ����
	OldPosition = TransInfo.Position;

	// ** �浹ü ����
	Collider.Position = TransInfo.Position;
	Collider.Scale = TransInfo.Scale;

	// ** �÷��̾ ȭ�� ������ ������ �ʵ��� ó��	
	CheckPositionInsideScreen();

	return;
}

void Player::Render(HDC _hdc)
{
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
		bDied = false;
		//return;
	}
		
	// ** ���ظ� �Ծ��� ��
	if ( bTakeDamage )
	{

	}

	// ** �Ϲ� ���
	RenderPlayer(_hdc);	
}

void Player::Fire(eBulletFireType _FireType, int _Power, int _Damage)
{
	Bridge* pBridge = nullptr;

	switch ( _FireType )
	{
		case eBulletFireType::Normal:
		{
			// ** Power ��ġ��ŭ �Ѿ� ���ڸ� �ø���, �Ѿ� �� ���ݿ� ���� ������ �����Ͽ�
			// ** ��ä�� ���·� �߻�ǵ��� �Ѵ�.
			// ** 
			int angleGap = 10;
			int startAngle = 90 - ((angleGap * 0.5 * (_Power - 1)));
			
			for ( int i = 0; i < _Power; ++i )
			{
				int angle = startAngle + (angleGap * i);
				Vector3 BulletDirection(cosf(angle * PI / 180), -sinf(angle * PI / 180));

				SpawnManager::SpawnBullet(this, TransInfo.Position, BulletDirection, new NormalBullet, _Damage);
			}
			break;
		} 		
		case eBulletFireType::Guide:
			break;
		default:
			break;
	}	
}

void Player::ApplyDamage(Object* _pTarget, int _Damage)
{
	// ** ... �������� ���� ���� ����

	TakeDamage(_Damage);
}

void Player::TakeDamage(int _Damage)
{
	HP -= _Damage;
	bTakeDamage = true;

	if ( HP <= 0 )
	{
		HP = 0;
		bDied = true;
	}
}

void Player::CheckPositionInsideScreen()
{
	float Offset = 22.0f;
	float StageScaleX = 600.0f;
	
	RECT ScreenRect;
	ScreenRect.left   = (LONG)((WindowsWidth * 0.5f) - (StageScaleX * 0.5f));
	ScreenRect.top	  = 0;
	ScreenRect.right  = (LONG)((WindowsWidth * 0.5f) + (StageScaleX * 0.5f));
	ScreenRect.bottom = WindowsHeight;
		
	// ** ��
	if ( (LONG)TransInfo.Position.x < ScreenRect.left + Offset )
		TransInfo.Position.x = ScreenRect.left + Offset;

	// ** ��
	if ( (LONG)TransInfo.Position.x > ScreenRect.right - Offset )
		TransInfo.Position.x = ScreenRect.right - Offset;

	// ** ��
	if ( (LONG)TransInfo.Position.y < Offset )
		TransInfo.Position.y = Offset;

	// ** ��
	if ( (LONG)TransInfo.Position.y > WindowsHeight - Offset )
		TransInfo.Position.y = WindowsHeight - Offset;
}

bool Player::RenderSpawn(HDC _hdc)
{
	if ( !pPlayerImage ) return false;

	// ** ȭ�� �Ʒ��������� ���� ��ġ���� ������ �ö������ �׸�	
	static float MovePositionY = WindowsHeight + 30;

	float MoveSpeed = 0.5f;
	float StopPositionY = WindowsHeight - 100;
	Vector3 ImagePosition = Vector3(42.0f, 0.0f);

	TransInfo.Position.x = WindowsWidth * 0.5f;
	TransInfo.Position.y = MovePositionY;

	TransparentBlt(_hdc, 
		(int)(TransInfo.Position.x - TransInfo.Scale.x * 0.5f),
		(int)(TransInfo.Position.y - TransInfo.Scale.y * 0.5f),
		(int)TransInfo.Scale.x,
		(int)TransInfo.Scale.y,
		pPlayerImage->GetMemDC(),
		(int)ImagePosition.x,
		(int)ImagePosition.y,
		(int)TransInfo.Scale.x,
		(int)TransInfo.Scale.y,
		RGB(255, 0, 255));

	if ( MovePositionY < StopPositionY )
	{
		MovePositionY = WindowsHeight + 30;
		return true;
	}

	MovePositionY -= MoveSpeed;
	return false;
}

void Player::RenderPlayer(HDC _hdc)
{
	if ( !pPlayerImage ) return;

	Vector3 ImagePosition = Vector3(0.0f, TransInfo.Scale.y * (Frame % 2 + 1));

	// ** Direction���� ������ üũ�Ͽ� �ش��ϴ� �̹��� ������ ����
	if ( TransInfo.Direction.x < 0.0f )		 ImagePosition.x = 0.0f;  // ** Left
	else if ( TransInfo.Direction.x > 0.0f ) ImagePosition.x = 84.0f; // ** Right
	else									 ImagePosition.x = 42.0f; // ** None(Center)

	TransparentBlt(_hdc,
		(int)TransInfo.Position.x - (int)(TransInfo.Scale.x * 0.5f),
		(int)TransInfo.Position.y - (int)(TransInfo.Scale.y * 0.5f),
		(int)TransInfo.Scale.x,
		(int)TransInfo.Scale.y,
		pPlayerImage->GetMemDC(),
		(int)ImagePosition.x,
		(int)ImagePosition.y,
		(int)TransInfo.Scale.x,
		(int)TransInfo.Scale.y,
		RGB(255, 0, 255));
}

void Player::Release()
{
	
}

void Player::OnCollision(Object* _pObject)
{
}