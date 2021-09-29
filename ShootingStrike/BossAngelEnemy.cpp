#include "BossAngelEnemy.h"
#include "Object.h"
#include "BitmapManager.h"
#include "SpawnManager.h"
#include "InputManager.h"

BossAngelEnemy::BossAngelEnemy()
	: AnimType(AnimationType::DEFAULT)
{
}

BossAngelEnemy::~BossAngelEnemy()
{
	Release();
}


void BossAngelEnemy::Initialize()
{
	Key = eBridgeKey::ENEMY_BOSS;

	AnimType = AnimationType::DEFAULT;
}

void BossAngelEnemy::Update()
{
	// ** Owner�� �����͸� �޾ƿ�
	ReceiveInfoFromOwner();

	static int PatternIndex = 0;
	static int ActiveCount = 0;	
	static ULONGLONG ActiveTime = GetTickCount64();

	if ( CheckKeyInputState(eInputKey::KEY_LBUTTON, eKeyInputState::DOWN) )
	{
		PatternIndex = 1;
		ActiveCount = 1;
		ActiveTime = GetTickCount64();
	}

	switch ( PatternIndex )
	{
		case 1:
		{
			int MaxActiveCount = 50;

			// ** Level ��ŭ �Ѿ� ���ڸ� �ø���, ��� ���� �Ѿ� �� ���ݿ� ���� ������ �����Ͽ�
			// ** ��ä�� ���·� �߻�ǵ��� �Ѵ�.
			// ** angleGap : �Ѿ˰� ���� ����

			int AngleGap = 30;
			if ( ActiveTime + 50 < GetTickCount64() )
			{				
				int BulletCount = 360 / AngleGap;				
				for ( int i = 0; i < BulletCount; ++i )
				{
					// ** ��� ���� ���� ����
					int Angle = AngleGap * i + (ActiveCount * 7);

					// ** Bullet�� TransInfo ����
					Transform BulletTransInfo;
					BulletTransInfo.Position = TransInfo.Position;
					BulletTransInfo.Scale = Vector3(10.0f, 10.0f);
					BulletTransInfo.Direction = Vector3(cosf(Angle * PI / 180), -sinf(Angle * PI / 180));

					// ** Bullet�� Speed ����
					float BulletSpeed = 3.0f;

					// ** Bullet Spawn
					SpawnManager::SpawnBullet(pOwner, BulletTransInfo, BulletSpeed, 1, eBulletFiringType::NORMAL);
				}
								
				ActiveTime = GetTickCount64();
				++ActiveCount;
			}
			if ( MaxActiveCount < ActiveCount )
			{
				PatternIndex = 0;
			}
			break;
		}
		default:
			ActiveTime = 0;
			break;
	}

	//TransInfo.Position.x += TransInfo.Direction.x * Speed;
	//TransInfo.Position.y += TransInfo.Direction.y * Speed;

	// ** Owner�� ������ ������ ����
	SendInfoToOwner();
}


void BossAngelEnemy::Render(HDC _hdc)
{
	if ( !pImage )
		return;

	switch ( AnimType )
	{
		case AnimationType::DEFAULT:	PlayAnimDefault(_hdc);	 break;
		case AnimationType::ATTACK1:	PlayAnimAttack1(_hdc);	 break;
		case AnimationType::ATTACK2:	PlayAnimAttack2(_hdc);	 break;
		case AnimationType::ATTACK3:	PlayAnimAttack3(_hdc);	 break;
		case AnimationType::EVOLUTION:	PlayAnimEvolution(_hdc); break;
		case AnimationType::ATTACKED: break;
		default:
			break;
	}	
}

void BossAngelEnemy::Release()
{

}

void BossAngelEnemy::PlayAnimDefault(HDC _hdc)
{
	static ULONGLONG Time = GetTickCount64();
	static int Offset = 0;
	int MaxSegmentCount = 6; // ** �ش� ���ο��� ���ҵ� �̹��� ��
	int Delay = 200;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * Offset),
		(int)(pImage->GetSegmentationScale().y * static_cast<int>(AnimType)),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(147, 187, 236));

	// ** Delay �ð� �ֱ�� Offset ����
	if ( Time + Delay < GetTickCount64() )
	{
		if ( ++Offset == MaxSegmentCount )
			Offset = 0;

		Time = GetTickCount64();
	}
	
}

void BossAngelEnemy::PlayAnimAttack1(HDC _hdc)
{
	static ULONGLONG Time = GetTickCount64();
	static int Offset = 0;
	static bool bRepeatSection = false; // ** ���� ������ �ݺ��ǰ� �ִ� Ÿ�̹�����
	int MaxSegmentCount = 6;			// ** �ش� ���ο��� ���ҵ� �̹��� ��	
	int Delay = 200;

	// ** Ready Ÿ�̹�
	static ULONGLONG ReadyTime = 0;
	int ReadyDurationTime = 1000;
	int ReadySectionStart = 0;
	int ReadySectionEnd = 1;

	// ** Fire Ÿ�̹�
	static ULONGLONG FireTime = 0;
	int FireDurationTime = 2000;
	int FireSectionStart = 2;
	int FireSectionEnd = 5;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * Offset),
		(int)(pImage->GetSegmentationScale().y * static_cast<int>(AnimType)),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(147, 187, 236));

	// ** Ready ���� ���� ����
	if ( !bRepeatSection && Offset == ReadySectionStart )
	{
		// ** Ready ���� Ÿ�� ����
		ReadyTime = GetTickCount64();
	}

	// ** Fire ���� ���� ����
	if ( !bRepeatSection && Offset == FireSectionStart )
	{
		// ** Fire ���� Ÿ�� ����
		FireTime = GetTickCount64();
	}

	// ** Ready ���� ���� ����
	if ( Offset == ReadySectionEnd )
	{
		// ** ���� �ݺ� ��ŸƮ
		bRepeatSection = true;

		// ** Ready �����ݺ� ���ӽð��� �������� üũ
		if ( ReadyTime + ReadyDurationTime < GetTickCount64() )
		{
			// ** ���� �ݺ� ����
			bRepeatSection = false;

			// ** ������ �ѱ� ( ���������� ���� Offset���� ���� )
			Offset = ReadySectionEnd + 1;
		}
	}
	// ** Fire ���� ���� ����
	else if ( Offset == FireSectionEnd )
	{
		// ** ���� �ݺ� ��ŸƮ
		bRepeatSection = true;

		// ** Fire �����ݺ� ���ӽð��� �������� üũ
		if ( FireTime + FireDurationTime < GetTickCount64() )
		{
			// ** ���� �ݺ� ����
			bRepeatSection = false;

			// ** ������ �����̹Ƿ� Offset�� ���� ó������ ����
			Offset = 0;
		}
		else
		{
			// ** �ݺ��� ���� Fire ��ŸƮ �������� ����
			Offset = FireSectionStart;
		}
	}
	// ** �ݺ����� ���������� �ƴ϶��
	else
	{
		// ** Delay �ð� �ֱ�� Offset ����
		if ( Time + Delay < GetTickCount64() )
		{
			++Offset;

			Time = GetTickCount64();
		}
	}
}

void BossAngelEnemy::PlayAnimAttack2(HDC _hdc)
{
}

void BossAngelEnemy::PlayAnimAttack3(HDC _hdc)
{
}

void BossAngelEnemy::PlayAnimEvolution(HDC _hdc)
{
}
