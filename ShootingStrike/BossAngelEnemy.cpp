#include "BossAngelEnemy.h"
#include "Object.h"
#include "BitmapManager.h"
#include "SpawnManager.h"
#include "InputManager.h"

BossAngelEnemy::BossAngelEnemy()
	: AnimType(AnimationType::DEFAULT)
	, bLoopPlayAnim(false)
{
}

BossAngelEnemy::~BossAngelEnemy()
{
	
}


void BossAngelEnemy::Initialize()
{
	Key = eBridgeKey::ENEMY_BOSS;

	AnimType = AnimationType::DEFAULT;
	bLoopPlayAnim = false;
}

void BossAngelEnemy::Update()
{
	// ** Owner�� �����͸� �޾ƿ�
	ReceiveInfoFromOwner();

	static int PatternIndex = 0;
	static int ActiveCount = 0;	
	static ULONGLONG ActiveTime = GetTickCount64();

	// _Debug_
	if ( CheckKeyInputState(eInputKey::KEY_LBUTTON, eKeyInputState::DOWN) )
	{
		PatternIndex = 1;
		ActiveCount = 1;
		ActiveTime = GetTickCount64();

		static int TypeIndex = 0;
		if ( TypeIndex == static_cast<int>(AnimationType::EVOLUTION) )
			TypeIndex = 0;
		else
			TypeIndex++;
			
		PlayAnimation(static_cast<AnimationType>(TypeIndex), false);
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

	static AnimationType OldAnimType = AnimationType::DEFAULT;
	static ULONGLONG Time = GetTickCount64();
	static int Offset = 0;
	bool bFinishPlay = false;

	// ** ������ �׸��� Animation�� ���� Animation�� �ٸ���,
	// ** �� ���ο� Animation�� �÷��� �Ǵ� ������ �ʱ�ȭ
	if ( OldAnimType != AnimType )
	{
		OldAnimType = AnimType;
		Time = GetTickCount64();
		Offset = 0;		
	}

	switch ( AnimType )
	{
		case AnimationType::DEFAULT:	PlayAnimDefault(_hdc, Time, Offset);	break;
		case AnimationType::ATTACK1:	PlayAnimAttack1(_hdc, Time, Offset);	break;
		case AnimationType::ATTACK2:	PlayAnimAttack2(_hdc, Time, Offset);	break;
		case AnimationType::ATTACK3:	PlayAnimAttack3(_hdc, Time, Offset);	break;
		case AnimationType::EVOLUTION:	PlayAnimEvolution(_hdc, Time, Offset);	break;
		case AnimationType::ATTACKED: break;
		default:
			break;
	}
}

void BossAngelEnemy::Release()
{

}

void BossAngelEnemy::PlayAnimation(AnimationType _AnimType, bool _bLoop)
{
	AnimType = _AnimType;
	bLoopPlayAnim = _bLoop;
}

void BossAngelEnemy::PlayAnimDefault(HDC _hdc, ULONGLONG& _Time, int& _Offset)
{
	int MaxSegmentCount = 6; // ** �ش� ���ο��� ���ҵ� �̹��� ��
	int Delay = 200;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * _Offset),
		(int)(pImage->GetSegmentationScale().y * static_cast<int>(AnimType)),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(147, 187, 236));

	// ** Delay �ð� �ֱ�� Offset ����
	if ( _Time + Delay < GetTickCount64() )
	{
		// ** Default�� �׻� Loop �����̹Ƿ� Offset�� 0���� ����
		if ( ++_Offset == MaxSegmentCount )
			_Offset = 0;

		_Time = GetTickCount64();
	}	
}

void BossAngelEnemy::PlayAnimAttack1(HDC _hdc, ULONGLONG& _Time, int& _Offset)
{
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
	int FireSectionEnd = MaxSegmentCount - 1;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * _Offset),
		(int)(pImage->GetSegmentationScale().y * static_cast<int>(AnimType)),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(147, 187, 236));

	// ** Ready ���� ���� ����
	if ( !bRepeatSection && _Offset == ReadySectionStart )
	{
		// ** Ready ���� Ÿ�� ����
		ReadyTime = GetTickCount64();
	}

	// ** Fire ���� ���� ����
	if ( !bRepeatSection && _Offset == FireSectionStart )
	{
		// ** Fire ���� Ÿ�� ����
		FireTime = GetTickCount64();
	}

	// ** Ready ���� ���� ����
	if ( _Offset == ReadySectionEnd )
	{
		// ** ���� �ݺ� ��ŸƮ
		bRepeatSection = true;

		// ** Ready �����ݺ� ���ӽð��� �������� üũ
		if ( ReadyTime + ReadyDurationTime < GetTickCount64() )
		{
			// ** ���� �ݺ� ����
			bRepeatSection = false;

			// ** ������ �ѱ� ( ���������� ���� Offset���� ���� )
			_Offset = ReadySectionEnd + 1;
		}
	}
	// ** Fire ���� ���� ����
	else if ( _Offset == FireSectionEnd )
	{
		// ** ���� �ݺ� ��ŸƮ
		bRepeatSection = true;

		// ** Fire �����ݺ� ���ӽð��� �������� üũ
		if ( FireTime + FireDurationTime < GetTickCount64() )
		{
			// ** ���� �ݺ� ����
			bRepeatSection = false;

			// ** Loop���̶�� Offset�� �ٽ� 0���� �����ϰ� �ƴ϶�� Default Animation�� Play ��Ų�� 
			if ( bLoopPlayAnim )
				_Offset = 0;
			else
				PlayAnimation(AnimationType::DEFAULT, true);
		}
		else
		{
			// ** �ݺ��� ���� Fire ��ŸƮ �������� ����
			_Offset = FireSectionStart;
		}
	}
	// ** �ݺ����� ���������� �ƴ϶��
	else
	{
		// ** Delay �ð� �ֱ�� Offset ����
		if ( _Time + Delay < GetTickCount64() )
		{
			++_Offset;

			_Time = GetTickCount64();
		}
	}
}

void BossAngelEnemy::PlayAnimAttack2(HDC _hdc, ULONGLONG& _Time, int& _Offset)
{
	static bool bRepeatSection = false; // ** ���� ������ �ݺ��ǰ� �ִ� Ÿ�̹�����
	int MaxSegmentCount = 5;			// ** �ش� ���ο��� ���ҵ� �̹��� ��	
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
	int FireSectionEnd = MaxSegmentCount - 1;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * _Offset),
		(int)(pImage->GetSegmentationScale().y * static_cast<int>(AnimType)),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(147, 187, 236));

	// ** Ready ���� ���� ����
	if ( !bRepeatSection && _Offset == ReadySectionStart )
	{
		// ** Ready ���� Ÿ�� ����
		ReadyTime = GetTickCount64();
	}

	// ** Fire ���� ���� ����
	if ( !bRepeatSection && _Offset == FireSectionStart )
	{
		// ** Fire ���� Ÿ�� ����
		FireTime = GetTickCount64();
	}

	// ** Ready ���� ���� ����
	if ( _Offset == ReadySectionEnd )
	{
		// ** ���� �ݺ� ��ŸƮ
		bRepeatSection = true;

		// ** Ready �����ݺ� ���ӽð��� �������� üũ
		if ( ReadyTime + ReadyDurationTime < GetTickCount64() )
		{
			// ** ���� �ݺ� ����
			bRepeatSection = false;

			// ** ������ �ѱ� ( ���������� ���� Offset���� ���� )
			_Offset = ReadySectionEnd + 1;
		}
	}
	// ** Fire ���� ���� ����
	else if ( _Offset == FireSectionEnd )
	{
		// ** ���� �ݺ� ��ŸƮ
		bRepeatSection = true;

		// ** Fire �����ݺ� ���ӽð��� �������� üũ
		if ( FireTime + FireDurationTime < GetTickCount64() )
		{
			// ** ���� �ݺ� ����
			bRepeatSection = false;

			// ** Loop���̶�� Offset�� �ٽ� 0���� �����ϰ� �ƴ϶�� Default Animation�� Play ��Ų�� 
			if ( bLoopPlayAnim )
				_Offset = 0;
			else
				PlayAnimation(AnimationType::DEFAULT, true);
		}
		else
		{
			// ** �ݺ��� ���� Fire ��ŸƮ �������� ����
			_Offset = FireSectionStart;
		}
	}
	// ** �ݺ����� ���������� �ƴ϶��
	else
	{
		// ** Delay �ð� �ֱ�� Offset ����
		if ( _Time + Delay < GetTickCount64() )
		{
			++_Offset;

			_Time = GetTickCount64();
		}
	}
}

void BossAngelEnemy::PlayAnimAttack3(HDC _hdc, ULONGLONG& _Time, int& _Offset)
{
	static bool bRepeatSection = false; // ** ���� ������ �ݺ��ǰ� �ִ� Ÿ�̹�����
	int MaxSegmentCount = 4;			// ** �ش� ���ο��� ���ҵ� �̹��� ��	
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
	int FireSectionEnd = MaxSegmentCount - 1;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * _Offset),
		(int)(pImage->GetSegmentationScale().y * static_cast<int>(AnimType)),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(147, 187, 236));

	// ** Ready ���� ���� ����
	if ( !bRepeatSection && _Offset == ReadySectionStart )
	{
		// ** Ready ���� Ÿ�� ����
		ReadyTime = GetTickCount64();
	}

	// ** Fire ���� ���� ����
	if ( !bRepeatSection && _Offset == FireSectionStart )
	{
		// ** Fire ���� Ÿ�� ����
		FireTime = GetTickCount64();
	}

	// ** Ready ���� ���� ����
	if ( _Offset == ReadySectionEnd )
	{
		// ** ���� �ݺ� ��ŸƮ
		bRepeatSection = true;

		// ** Ready �����ݺ� ���ӽð��� �������� üũ
		if ( ReadyTime + ReadyDurationTime < GetTickCount64() )
		{
			// ** ���� �ݺ� ����
			bRepeatSection = false;

			// ** ������ �ѱ� ( ���������� ���� Offset���� ���� )
			_Offset = ReadySectionEnd + 1;
		}
	}
	// ** Fire ���� ���� ����
	else if ( _Offset == FireSectionEnd )
	{
		// ** ���� �ݺ� ��ŸƮ
		bRepeatSection = true;

		// ** Fire �����ݺ� ���ӽð��� �������� üũ
		if ( FireTime + FireDurationTime < GetTickCount64() )
		{
			// ** ���� �ݺ� ����
			bRepeatSection = false;

			// ** Loop���̶�� Offset�� �ٽ� 0���� �����ϰ� �ƴ϶�� Default Animation�� Play ��Ų�� 
			if ( bLoopPlayAnim )
				_Offset = 0;
			else
				PlayAnimation(AnimationType::DEFAULT, true);
		}
		else
		{
			// ** �ݺ��� ���� Fire ��ŸƮ �������� ����
			_Offset = FireSectionStart;
		}
	}
	// ** �ݺ����� ���������� �ƴ϶��
	else
	{
		// ** Delay �ð� �ֱ�� Offset ����
		if ( _Time + Delay < GetTickCount64() )
		{
			++_Offset;

			_Time = GetTickCount64();
		}
	}
}

void BossAngelEnemy::PlayAnimEvolution(HDC _hdc, ULONGLONG& _Time, int& _Offset)
{
	int MaxSegmentCount = 8; // ** �ش� ���ο��� ���ҵ� �̹��� ��
	int Delay = 200;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * _Offset),
		(int)(pImage->GetSegmentationScale().y * static_cast<int>(AnimType)),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(147, 187, 236));

	// ** Delay �ð� �ֱ�� Offset ����
	if ( _Time + Delay < GetTickCount64() )
	{
		if ( ++_Offset == MaxSegmentCount )
		{
			// ** Loop���̶�� Offset�� �ٽ� 0���� �����ϰ� �ƴ϶�� Default Animation�� Play ��Ų�� 
			if ( bLoopPlayAnim ) 
				_Offset = 0;
			else
				PlayAnimation(AnimationType::DEFAULT, true);
		}
		_Time = GetTickCount64();
	}
}

void BossAngelEnemy::PlayAnimAttacked(HDC _hdc, ULONGLONG& _Time, int& _Offset)
{
	int MaxSegmentCount = 1; // ** �ش� ���ο��� ���ҵ� �̹��� ��
	int Delay = 200;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * _Offset),
		(int)(pImage->GetSegmentationScale().y * static_cast<int>(AnimType)),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(147, 187, 236));

	// ** Delay �ð� �ֱ�� Offset ����
	if ( _Time + Delay < GetTickCount64() )
	{
		// ** Default�� �׻� Loop �����̹Ƿ� Offset�� 0���� ����
		if ( ++_Offset == MaxSegmentCount )
		{
			if ( bLoopPlayAnim )
				_Offset = 0;
			else
				PlayAnimation(AnimationType::DEFAULT, true);
		}

		_Time = GetTickCount64();
	}
}
