#include "BossAngelEnemy.h"
#include "Object.h"
#include "BitmapManager.h"
#include "SpawnManager.h"
#include "InputManager.h"

BossAngelEnemy::BossAngelEnemy()
	: animType(AnimationType::DEFAULT)
	, bLoopPlayAnim(false)
{
}

BossAngelEnemy::~BossAngelEnemy()
{
	
}


void BossAngelEnemy::Initialize()
{
	key = eBridgeKey::ENEMY_BOSS;

	animType = AnimationType::DEFAULT;
	bLoopPlayAnim = false;
}

void BossAngelEnemy::Update()
{
	// ** Owner의 데이터를 받아옴
	ReceiveInfoFromOwner();

	static int patternIndex = 0;
	static int activeCount = 0;	
	static ULONGLONG activeTime = GetTickCount64();

	// _Debug_
	if ( CHECK_KEYINPUT_STATE(eInputKey::KEY_LBUTTON, eKeyInputState::DOWN) )
	{
		patternIndex = 1;
		activeCount = 1;
		activeTime = GetTickCount64();

		static int TypeIndex = 0;
		if ( TypeIndex == static_cast<int>(AnimationType::EVOLUTION) )
			TypeIndex = 0;
		else
			TypeIndex++;
			
		PlayAnimation(static_cast<AnimationType>(TypeIndex), false);
	}

	switch ( patternIndex )
	{
		case 1:
		{
			int maxActiveCount = 50;

			// ** Level 만큼 총알 숫자를 늘리고, 상방 기준 총알 간 간격에 대한 각도를 설정하여
			// ** 부채꼴 형태로 발사되도록 한다.
			// ** angleGap : 총알간 간격 각도

			int angleGap = 30;
			if ( activeTime + 50 < GetTickCount64() )
			{				
				int bulletCount = 360 / angleGap;				
				for ( int i = 0; i < bulletCount; ++i )
				{
					// ** 상방 기준 현재 각도
					int Angle = angleGap * i + (activeCount * 7);

					// ** Bullet의 TransInfo 설정
					Transform bulletTransInfo;
					bulletTransInfo.Position = transInfo.Position;
					bulletTransInfo.Scale = Vector3(10.0f, 10.0f);
					bulletTransInfo.Direction = Vector3(cosf(Angle * PI / 180), -sinf(Angle * PI / 180));

					// ** Bullet의 Speed 설정
					float bulletSpeed = 3.0f;

					// ** Bullet Spawn
					SpawnManager::SpawnBullet(pOwner, bulletTransInfo, bulletSpeed, 1, eBulletType::NORMAL);
				}
								
				activeTime = GetTickCount64();
				++activeCount;
			}
			if ( maxActiveCount < activeCount )
			{
				patternIndex = 0;
			}
			break;
		}
		default:
			activeTime = 0;
			break;
	}

	//TransInfo.Position.x += TransInfo.Direction.x * Speed;
	//TransInfo.Position.y += TransInfo.Direction.y * Speed;

	// ** Owner로 가공된 데이터 전달
	SendInfoToOwner();
}


void BossAngelEnemy::Render(HDC _hdc)
{
	if ( !pImage )
		return;

	static AnimationType oldAnimType = AnimationType::DEFAULT;
	static ULONGLONG time = GetTickCount64();
	static int offset = 0;
	bool bFinishPlay = false;

	// ** 이전에 그리던 Animation과 현재 Animation이 다를때,
	// ** 즉 새로운 Animation이 플레이 되는 시점에 초기화
	if ( oldAnimType != animType )
	{
		oldAnimType = animType;
		time = GetTickCount64();
		offset = 0;		
	}

	switch ( animType )
	{
		case AnimationType::DEFAULT:	PlayAnimDefault(_hdc, time, offset);	break;
		case AnimationType::ATTACK1:	PlayAnimAttack1(_hdc, time, offset);	break;
		case AnimationType::ATTACK2:	PlayAnimAttack2(_hdc, time, offset);	break;
		case AnimationType::ATTACK3:	PlayAnimAttack3(_hdc, time, offset);	break;
		case AnimationType::EVOLUTION:	PlayAnimEvolution(_hdc, time, offset);	break;
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
	animType = _AnimType;
	bLoopPlayAnim = _bLoop;
}

void BossAngelEnemy::PlayAnimDefault(HDC _hdc, ULONGLONG& _time, int& _offset)
{
	int maxSegmentCount = 6; // ** 해당 라인에서 분할된 이미지 수
	int delay = 200;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * _offset),
		(int)(pImage->GetSegmentationScale().y * static_cast<int>(animType)),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(147, 187, 236));

	// ** Delay 시간 주기로 Offset 증가
	if ( _time + delay < GetTickCount64() )
	{
		// ** Default는 항상 Loop 상태이므로 Offset을 0으로 세팅
		if ( ++_offset == maxSegmentCount )
			_offset = 0;

		_time = GetTickCount64();
	}	
}

void BossAngelEnemy::PlayAnimAttack1(HDC _hdc, ULONGLONG& _time, int& _offset)
{
	static bool bRepeatSection = false; // ** 일정 구간이 반복되고 있는 타이밍인지
	int maxSegmentCount = 6;			// ** 해당 라인에서 분할된 이미지 수	
	int delay = 200;

	// ** Ready 타이밍
	static ULONGLONG readyTime = 0;
	int readyDurationTime = 1000;
	int readySectionStart = 0;
	int readySectionEnd = 1;

	// ** Fire 타이밍
	static ULONGLONG fireTime = 0;
	int fireDurationTime = 2000;
	int fireSectionStart = 2;
	int fireSectionEnd = maxSegmentCount - 1;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * _offset),
		(int)(pImage->GetSegmentationScale().y * static_cast<int>(animType)),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(147, 187, 236));

	// ** Ready 시작 구간 진입
	if ( !bRepeatSection && _offset == readySectionStart )
	{
		// ** Ready 시작 타임 설정
		readyTime = GetTickCount64();
	}

	// ** Fire 시작 구간 진입
	if ( !bRepeatSection && _offset == fireSectionStart )
	{
		// ** Fire 시작 타임 설정
		fireTime = GetTickCount64();
	}

	// ** Ready 종료 구간 진입
	if ( _offset == readySectionEnd )
	{
		// ** 구간 반복 스타트
		bRepeatSection = true;

		// ** Ready 구간반복 지속시간이 끝났는지 체크
		if ( readyTime + readyDurationTime < GetTickCount64() )
		{
			// ** 구간 반복 종료
			bRepeatSection = false;

			// ** 구간을 넘김 ( 종료지점의 다음 Offset으로 설정 )
			_offset = readySectionEnd + 1;
		}
	}
	// ** Fire 종료 구간 진입
	else if ( _offset == fireSectionEnd )
	{
		// ** 구간 반복 스타트
		bRepeatSection = true;

		// ** Fire 구간반복 지속시간이 끝났는지 체크
		if ( fireTime + fireDurationTime < GetTickCount64() )
		{
			// ** 구간 반복 종료
			bRepeatSection = false;

			// ** Loop중이라면 Offset을 다시 0으로 세팅하고 아니라면 Default Animation을 Play 시킨다 
			if ( bLoopPlayAnim )
				_offset = 0;
			else
				PlayAnimation(AnimationType::DEFAULT, true);
		}
		else
		{
			// ** 반복을 위해 Fire 스타트 지점으로 리셋
			_offset = fireSectionStart;
		}
	}
	// ** 반복구간 종료지점이 아니라면
	else
	{
		// ** Delay 시간 주기로 Offset 증가
		if ( _time + delay < GetTickCount64() )
		{
			++_offset;

			_time = GetTickCount64();
		}
	}
}

void BossAngelEnemy::PlayAnimAttack2(HDC _hdc, ULONGLONG& _time, int& _offset)
{
	static bool bRepeatSection = false; // ** 일정 구간이 반복되고 있는 타이밍인지
	int maxSegmentCount = 5;			// ** 해당 라인에서 분할된 이미지 수	
	int delay = 200;
	
	// ** Ready 타이밍
	static ULONGLONG readyTime = 0;
	int readyDurationTime = 1000;
	int readySectionStart = 0;
	int readySectionEnd = 1;

	// ** Fire 타이밍
	static ULONGLONG fireTime = 0;
	int fireDurationTime = 2000;
	int fireSectionStart = 2;
	int fireSectionEnd = maxSegmentCount - 1;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * _offset),
		(int)(pImage->GetSegmentationScale().y * static_cast<int>(animType)),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(147, 187, 236));

	// ** Ready 시작 구간 진입
	if ( !bRepeatSection && _offset == readySectionStart )
	{
		// ** Ready 시작 타임 설정
		readyTime = GetTickCount64();
	}

	// ** Fire 시작 구간 진입
	if ( !bRepeatSection && _offset == fireSectionStart )
	{
		// ** Fire 시작 타임 설정
		fireTime = GetTickCount64();
	}

	// ** Ready 종료 구간 진입
	if ( _offset == readySectionEnd )
	{
		// ** 구간 반복 스타트
		bRepeatSection = true;

		// ** Ready 구간반복 지속시간이 끝났는지 체크
		if ( readyTime + readyDurationTime < GetTickCount64() )
		{
			// ** 구간 반복 종료
			bRepeatSection = false;

			// ** 구간을 넘김 ( 종료지점의 다음 Offset으로 설정 )
			_offset = readySectionEnd + 1;
		}
	}
	// ** Fire 종료 구간 진입
	else if ( _offset == fireSectionEnd )
	{
		// ** 구간 반복 스타트
		bRepeatSection = true;

		// ** Fire 구간반복 지속시간이 끝났는지 체크
		if ( fireTime + fireDurationTime < GetTickCount64() )
		{
			// ** 구간 반복 종료
			bRepeatSection = false;

			// ** Loop중이라면 Offset을 다시 0으로 세팅하고 아니라면 Default Animation을 Play 시킨다 
			if ( bLoopPlayAnim )
				_offset = 0;
			else
				PlayAnimation(AnimationType::DEFAULT, true);
		}
		else
		{
			// ** 반복을 위해 Fire 스타트 지점으로 리셋
			_offset = fireSectionStart;
		}
	}
	// ** 반복구간 종료지점이 아니라면
	else
	{
		// ** Delay 시간 주기로 Offset 증가
		if ( _time + delay < GetTickCount64() )
		{
			++_offset;

			_time = GetTickCount64();
		}
	}
}

void BossAngelEnemy::PlayAnimAttack3(HDC _hdc, ULONGLONG& _time, int& _offset)
{
	static bool bRepeatSection = false; // ** 일정 구간이 반복되고 있는 타이밍인지
	int maxSegmentCount = 4;			// ** 해당 라인에서 분할된 이미지 수	
	int delay = 200;

	// ** Ready 타이밍
	static ULONGLONG readyTime = 0;
	int readyDurationTime = 1000;
	int readySectionStart = 0;
	int readySectionEnd = 1;

	// ** Fire 타이밍
	static ULONGLONG fireTime = 0;
	int fireDurationTime = 2000;
	int fireSectionStart = 2;
	int fireSectionEnd = maxSegmentCount - 1;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * _offset),
		(int)(pImage->GetSegmentationScale().y * static_cast<int>(animType)),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(147, 187, 236));

	// ** Ready 시작 구간 진입
	if ( !bRepeatSection && _offset == readySectionStart )
	{
		// ** Ready 시작 타임 설정
		readyTime = GetTickCount64();
	}

	// ** Fire 시작 구간 진입
	if ( !bRepeatSection && _offset == fireSectionStart )
	{
		// ** Fire 시작 타임 설정
		fireTime = GetTickCount64();
	}

	// ** Ready 종료 구간 진입
	if ( _offset == readySectionEnd )
	{
		// ** 구간 반복 스타트
		bRepeatSection = true;

		// ** Ready 구간반복 지속시간이 끝났는지 체크
		if ( readyTime + readyDurationTime < GetTickCount64() )
		{
			// ** 구간 반복 종료
			bRepeatSection = false;

			// ** 구간을 넘김 ( 종료지점의 다음 Offset으로 설정 )
			_offset = readySectionEnd + 1;
		}
	}
	// ** Fire 종료 구간 진입
	else if ( _offset == fireSectionEnd )
	{
		// ** 구간 반복 스타트
		bRepeatSection = true;

		// ** Fire 구간반복 지속시간이 끝났는지 체크
		if ( fireTime + fireDurationTime < GetTickCount64() )
		{
			// ** 구간 반복 종료
			bRepeatSection = false;

			// ** Loop중이라면 Offset을 다시 0으로 세팅하고 아니라면 Default Animation을 Play 시킨다 
			if ( bLoopPlayAnim )
				_offset = 0;
			else
				PlayAnimation(AnimationType::DEFAULT, true);
		}
		else
		{
			// ** 반복을 위해 Fire 스타트 지점으로 리셋
			_offset = fireSectionStart;
		}
	}
	// ** 반복구간 종료지점이 아니라면
	else
	{
		// ** Delay 시간 주기로 Offset 증가
		if ( _time + delay < GetTickCount64() )
		{
			++_offset;

			_time = GetTickCount64();
		}
	}
}

void BossAngelEnemy::PlayAnimEvolution(HDC _hdc, ULONGLONG& _time, int& _offset)
{
	int maxSegmentCount = 8; // ** 해당 라인에서 분할된 이미지 수
	int delay = 200;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * _offset),
		(int)(pImage->GetSegmentationScale().y * static_cast<int>(animType)),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(147, 187, 236));

	// ** Delay 시간 주기로 Offset 증가
	if ( _time + delay < GetTickCount64() )
	{
		if ( ++_offset == maxSegmentCount )
		{
			// ** Loop중이라면 Offset을 다시 0으로 세팅하고 아니라면 Default Animation을 Play 시킨다 
			if ( bLoopPlayAnim ) 
				_offset = 0;
			else
				PlayAnimation(AnimationType::DEFAULT, true);
		}
		_time = GetTickCount64();
	}
}

void BossAngelEnemy::PlayAnimAttacked(HDC _hdc, ULONGLONG& _time, int& _offset)
{
	int MaxSegmentCount = 1; // ** 해당 라인에서 분할된 이미지 수
	int Delay = 200;

	TransparentBlt(_hdc,
		(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		(int)(pOwner->GetScale().x),
		(int)(pOwner->GetScale().y),
		pImage->GetMemDC(),
		(int)(pImage->GetSegmentationScale().x * _offset),
		(int)(pImage->GetSegmentationScale().y * static_cast<int>(animType)),
		(int)(pImage->GetSegmentationScale().x),
		(int)(pImage->GetSegmentationScale().y),
		RGB(147, 187, 236));

	// ** Delay 시간 주기로 Offset 증가
	if ( _time + Delay < GetTickCount64() )
	{
		// ** Default는 항상 Loop 상태이므로 Offset을 0으로 세팅
		if ( ++_offset == MaxSegmentCount )
		{
			if ( bLoopPlayAnim )
				_offset = 0;
			else
				PlayAnimation(AnimationType::DEFAULT, true);
		}

		_time = GetTickCount64();
	}
}
