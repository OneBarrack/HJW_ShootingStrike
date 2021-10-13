#include "BossAngelEnemy.h"
#include "Object.h"
#include "BitmapManager.h"
#include "SpawnManager.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "Background.h"
#include "MathManager.h"

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
	Super::Initialize();

	key = eBridgeKey::ENEMY_BOSS;

	animType = AnimationType::DEFAULT;
	isSpawing = true;
	bLoopPlayAnim = false;

	Background* pStageBackground = static_cast<Background*>(
		ObjectManager::GetInstance()->FindObjectWithTag(eTagName::STAGE_MAIN_BKG));
	spawnStartPos.x = pStageBackground->GetPosition().x;
	spawnStartPos.y = (pStageBackground->GetPosition().y - (pStageBackground->GetScale().y * 0.5f));
	spawnDestPos.x = spawnStartPos.x;
	spawnDestPos.y = (pStageBackground->GetPosition().y - (pStageBackground->GetScale().y * 0.5f)) + 150;
}

void BossAngelEnemy::Update()
{
	// ** Owner의 데이터를 받아옴
	ReceiveInfoFromOwner();

	Super::Update();	
	
	// 스폰 중
	if ( isSpawing )
	{
		static bool bSpawnStart = true;

		// ** Spawn이 시작될 때 Spawn Start 위치로 이동 시킨다
		if ( bSpawnStart )
		{
			transInfo.Position.x = spawnStartPos.x;
			transInfo.Position.y = spawnStartPos.y;
			transInfo.Direction = MathManager::GetDirection(spawnStartPos, spawnDestPos);

			bSpawnStart = false;
		}

		// ** 이동 속도
		float moveSpeed = 0.5f;

		// ** 화면 위에서부터 일정 위치까지 서서히 내려가도록
		if ( transInfo.Position.y + moveSpeed < spawnDestPos.y )
		{
			transInfo.Position.x += transInfo.Direction.x * moveSpeed;
			transInfo.Position.y += transInfo.Direction.y * moveSpeed;
		}
		// ** 멈출 위치에 도달
		else
		{
			// ** spawnDestPos로 위치 설정 및 Direction 0,0 로 변경
			transInfo.Position = spawnDestPos;
			transInfo.Direction = Vector3(0.0f, 0.0f);
			isSpawing = false;
			bSpawnStart = true;
		}
	}
	else
	{
		// ** 목적지에 도달했을 경우 이동중지
		if ( !bArrivedToDest )
		{
			transInfo.Position.x += transInfo.Direction.x * speed;
			transInfo.Position.y += transInfo.Direction.y * speed;			
		}
	}

	// 충돌체 조정
	collider.Position = Vector3(transInfo.Position.x, transInfo.Position.y + (transInfo.Scale.y * 0.2f));
	collider.Scale = Vector3(transInfo.Scale.x * 0.4f, transInfo.Scale.y * 0.5f);

	// _Debug_ : Update 전체 테스트 중
	if ( CHECK_KEYINPUT_STATE(eInputKey::KEY_LBUTTON, eKeyInputState::DOWN) )
	{
		static int animTypeIndex = 0;
		static int patternIndex = 0;

		if ( animTypeIndex == static_cast<int>(AnimationType::EVOLUTION) )
			animTypeIndex = 0;
		else
			animTypeIndex++;

		//if ( patternIndex == static_cast<int>(eBulletSpawnPattern::CIRCLE_GO_DELAY_SPREAD) )
		//	patternIndex = 1;
		//else
		//	patternIndex++;
		patternIndex = static_cast<int>(eBulletSpawnPattern::N_POLYGON_GO);
		PlayAnimation(static_cast<AnimationType>(animTypeIndex), false);

		Transform spawnTransInfo;
		spawnTransInfo.Position = collider.Position;
		spawnTransInfo.Scale = collider.Scale;
		spawnTransInfo.Direction = Vector3(0.0f, 1.0f);

		bulletScript.ReadyToSpawn(pOwner, static_cast<eBulletSpawnPattern>(patternIndex), spawnTransInfo, 1);
		
		//SpawnManager::SpawnEffect(transInfo, eBridgeKey::EFFECT_EXPLOSION);
		//Bridge* pBridge = ObjectManager::GetInstance()->NewBridge(eBridgeKey::BULLET_SPREAD_AFTER_DELAY);
		//static_cast<SpreadAfterDelayBullet*>(pBridge)->SetDelay(1000);
		//static_cast<SpreadAfterDelayBullet*>(pBridge)->SetSpreadCount(3);
		//static_cast<SpreadAfterDelayBullet*>(pBridge)->SetBulletCount(5);
		//static_cast<SpreadAfterDelayBullet*>(pBridge)->SetIntervalAngle(30);

		//SpawnManager::SpawnBullet(this, bulletTransInfo, bulletSpeed, _damage, pBridge);
	}

	//transInfo.Position.x += transInfo.Direction.x * Speed;
	//transInfo.Position.y += transInfo.Direction.y * Speed;	

	// ** Owner로 가공된 데이터 전달
	SendInfoToOwner();

	bulletScript.Run();
}


void BossAngelEnemy::Render(HDC _hdc)
{
	Super::Render(_hdc);
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
	Super::Release();
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

	// ** Delay 시간 주기로 Offset 적용
	if ( _time + delay < GetTickCount64() )
	{
		_time = GetTickCount64();

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
			// Offset 증가
			++_offset;
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

	// ** Delay 시간 주기로 Offset 적용
	if ( _time + delay < GetTickCount64() )
	{
		_time = GetTickCount64();

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
			// Offset 증가
			++_offset;
		}
	}
}

void BossAngelEnemy::PlayAnimAttack3(HDC _hdc, ULONGLONG& _time, int& _offset)
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

	// ** Delay 시간 주기로 Offset 적용
	if ( _time + delay < GetTickCount64() )
	{
		_time = GetTickCount64();

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
			// Offset 증가
			++_offset;
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
