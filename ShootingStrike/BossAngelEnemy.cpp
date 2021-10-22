#include "BossAngelEnemy.h"
#include "Object.h"
#include "Enemy.h"
#include "BitmapManager.h"
#include "SpawnManager.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "Background.h"
#include "MathManager.h"

BossAngelEnemy::BossAngelEnemy()
	: pBackground(nullptr)
	, curPhase(1)
	, animType(eBossAnimationType::DEFAULT)
	, bLoopPlayAnim(false)
	, prevActionCompleteTime(0)
	, actionState(eActionState::READY)
{
}

BossAngelEnemy::~BossAngelEnemy()
{
	
}


void BossAngelEnemy::Initialize()
{
	Super::Initialize();

	key = eBridgeKey::ENEMY_BOSS;

	curPhase = 1;
	animType = eBossAnimationType::DEFAULT;
	isSpawing = true;
	bLoopPlayAnim = false;

	pBackground = static_cast<Background*>(
		ObjectManager::GetInstance()->FindObjectWithTag(eTagName::STAGE_MAIN_BKG));
	spawnStartPos.x = pBackground->GetPosition().x;
	spawnStartPos.y = (pBackground->GetPosition().y - (pBackground->GetScale().y * 0.5f));
	spawnDestPos.x = spawnStartPos.x;
	spawnDestPos.y = (pBackground->GetPosition().y - (pBackground->GetScale().y * 0.5f)) + 150;

	prevActionCompleteTime = GetTickCount64();
	actionState = eActionState::READY;

	InitActionInfo();
}

void BossAngelEnemy::Update()
{
	// ** Owner�� �����͸� �޾ƿ�
	ReceiveInfoFromOwner();

	Super::Update();	
	
	// Phase üũ
	CheckPhase();

	// ���� ��
	if ( isSpawing )
	{
		static bool bSpawnStart = true;

		// ** Spawn�� ���۵� �� Spawn Start ��ġ�� �̵� ��Ų��
		if ( bSpawnStart )
		{
			// ** �������� �� �������� (�浹 ��Ȱ��ȭ)
			static_cast<Enemy*>(pOwner)->SetGenerateCollisionEvent(false);

			transInfo.Position.x = spawnStartPos.x;
			transInfo.Position.y = spawnStartPos.y;
			transInfo.Direction = MathManager::GetDirection(spawnStartPos, spawnDestPos);

			bSpawnStart = false;
		}

		// ** �̵� �ӵ�
		float moveSpeed = 0.5f;

		// ** ȭ�� ���������� ���� ��ġ���� ������ ����������
		if ( transInfo.Position.y + moveSpeed < spawnDestPos.y )
		{
			transInfo.Position.x += transInfo.Direction.x * moveSpeed;
			transInfo.Position.y += transInfo.Direction.y * moveSpeed;
		}
		// ** ���� ��ġ�� ����
		else
		{
			// ** spawnDestPos�� ��ġ ���� �� Direction 0,0 �� ����
			transInfo.Position = spawnDestPos;
			transInfo.Direction = Vector3(0.0f, 0.0f);
			isSpawing = false;
			bSpawnStart = true;

			prevActionCompleteTime = GetTickCount64();

			// ** ������ ������ �浹�� Ȱ��ȭ �����ش�
			static_cast<Enemy*>(pOwner)->SetGenerateCollisionEvent(true);
		}
	}
	else
	{
		// ** Action Info�� ������ ����
		if ( !actionInfos.empty() )
		{
			eActionType actionType = actionInfos.front().first;
			float delay = actionInfos.front().second;		

			switch ( actionState )
			{
				case eActionState::READY:
					if ( prevActionCompleteTime + delay < GetTickCount64() )
					{
						Action(actionType);
						actionState = eActionState::WORKING;
					}
					break;
				case eActionState::WORKING:
					Action(actionType);
					break;
				case eActionState::END:
					prevActionCompleteTime = GetTickCount64();
					actionInfos.pop();

					actionState = eActionState::READY;
					break;
			}
		}
		else
		{
			// ** Action Info�� ������ �ʱ�ȭ ���� �ݺ�
			InitActionInfo();
		}
	}

	// �浹ü ����
	collider.Position = Vector3(transInfo.Position.x, transInfo.Position.y + (transInfo.Scale.y * 0.2f));
	collider.Scale = Vector3(transInfo.Scale.x * 0.4f, transInfo.Scale.y * 0.5f);

	// ** Owner�� ������ ������ ����
	SendInfoToOwner();

	bulletScript.Run();
}


void BossAngelEnemy::Render(HDC _hdc)
{
	Super::Render(_hdc);
	if ( !pImage )
		return;

	static eBossAnimationType oldAnimType = eBossAnimationType::DEFAULT;
	static ULONGLONG time = GetTickCount64();
	static int offset = 0;
	bool bFinishPlay = false;

	// ** ������ �׸��� Animation�� ���� Animation�� �ٸ���,
	// ** �� ���ο� Animation�� �÷��� �Ǵ� ������ �ʱ�ȭ
	if ( oldAnimType != animType )
	{
		oldAnimType = animType;
		time = GetTickCount64();
		offset = 0;		
	}

	switch ( animType )
	{
		case eBossAnimationType::DEFAULT:	PlayAnimDefault(_hdc, time, offset);	break;
		case eBossAnimationType::ATTACK1:	PlayAnimAttack1(_hdc, time, offset);	break;
		case eBossAnimationType::ATTACK2:	PlayAnimAttack2(_hdc, time, offset);	break;
		case eBossAnimationType::ATTACK3:	PlayAnimAttack3(_hdc, time, offset);	break;
		case eBossAnimationType::EVOLUTION:	PlayAnimEvolution(_hdc, time, offset);	break;
		case eBossAnimationType::ATTACKED: break;
		default:
			break;
	}
}

void BossAngelEnemy::Release()
{
	Super::Release();
}

void BossAngelEnemy::PlayAnimation(eBossAnimationType _AnimType, bool _bLoop)
{
	animType = _AnimType;
	bLoopPlayAnim = _bLoop;
}

void BossAngelEnemy::PlayAnimDefault(HDC _hdc, ULONGLONG& _time, int& _offset)
{
	int maxSegmentCount = 6; // ** �ش� ���ο��� ���ҵ� �̹��� ��
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

	// ** Delay �ð� �ֱ�� Offset ����
	if ( _time + delay < GetTickCount64() )
	{
		// ** Default�� �׻� Loop �����̹Ƿ� Offset�� 0���� ����
		if ( ++_offset == maxSegmentCount )
			_offset = 0;

		_time = GetTickCount64();
	}	
}

void BossAngelEnemy::PlayAnimAttack1(HDC _hdc, ULONGLONG& _time, int& _offset)
{
	static bool bRepeatSection = false; // ** ���� ������ �ݺ��ǰ� �ִ� Ÿ�̹�����
	int maxSegmentCount = 6;			// ** �ش� ���ο��� ���ҵ� �̹��� ��	
	int delay = 200;

	// ** Ready Ÿ�̹�
	static ULONGLONG readyTime = 0;
	int readyDurationTime = 1000;
	int readySectionStart = 0;
	int readySectionEnd = 1;

	// ** Fire Ÿ�̹�
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

	// ** Delay �ð� �ֱ�� Offset ����
	if ( _time + delay < GetTickCount64() )
	{
		_time = GetTickCount64();

		// ** Ready ���� ���� ����
		if ( !bRepeatSection && _offset == readySectionStart )
		{
			// ** Ready ���� Ÿ�� ����
			readyTime = GetTickCount64();
		}

		// ** Fire ���� ���� ����
		if ( !bRepeatSection && _offset == fireSectionStart )
		{
			// ** Fire ���� Ÿ�� ����
			fireTime = GetTickCount64();
		}

		// ** Ready ���� ���� ����
		if ( _offset == readySectionEnd )
		{
			// ** ���� �ݺ� ��ŸƮ
			bRepeatSection = true;

			// ** Ready �����ݺ� ���ӽð��� �������� üũ
			if ( readyTime + readyDurationTime < GetTickCount64() )
			{
				// ** ���� �ݺ� ����
				bRepeatSection = false;

				// ** ������ �ѱ� ( ���������� ���� Offset���� ���� )
				_offset = readySectionEnd + 1;
			}
		}
		// ** Fire ���� ���� ����
		else if ( _offset == fireSectionEnd )
		{
			// ** ���� �ݺ� ��ŸƮ
			bRepeatSection = true;

			// ** Fire �����ݺ� ���ӽð��� �������� üũ
			if ( fireTime + fireDurationTime < GetTickCount64() )
			{
				// ** ���� �ݺ� ����
				bRepeatSection = false;

				// ** Loop���̶�� Offset�� �ٽ� 0���� �����ϰ� �ƴ϶�� Default Animation�� Play ��Ų�� 
				if ( bLoopPlayAnim )
					_offset = 0;
				else
					PlayAnimation(eBossAnimationType::DEFAULT, true);
			}
			else
			{
				// ** �ݺ��� ���� Fire ��ŸƮ �������� ����
				_offset = fireSectionStart;
			}
		}
		// ** �ݺ����� ���������� �ƴ϶��
		else
		{
			// Offset ����
			++_offset;
		}
	}
}

void BossAngelEnemy::PlayAnimAttack2(HDC _hdc, ULONGLONG& _time, int& _offset)
{
	static bool bRepeatSection = false; // ** ���� ������ �ݺ��ǰ� �ִ� Ÿ�̹�����
	int maxSegmentCount = 5;			// ** �ش� ���ο��� ���ҵ� �̹��� ��	
	int delay = 200;
	
	// ** Ready Ÿ�̹�
	static ULONGLONG readyTime = 0;
	int readyDurationTime = 1000;
	int readySectionStart = 0;
	int readySectionEnd = 1;

	// ** Fire Ÿ�̹�
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

	// ** Delay �ð� �ֱ�� Offset ����
	if ( _time + delay < GetTickCount64() )
	{
		_time = GetTickCount64();

		// ** Ready ���� ���� ����
		if ( !bRepeatSection && _offset == readySectionStart )
		{
			// ** Ready ���� Ÿ�� ����
			readyTime = GetTickCount64();
		}

		// ** Fire ���� ���� ����
		if ( !bRepeatSection && _offset == fireSectionStart )
		{
			// ** Fire ���� Ÿ�� ����
			fireTime = GetTickCount64();
		}

		// ** Ready ���� ���� ����
		if ( _offset == readySectionEnd )
		{
			// ** ���� �ݺ� ��ŸƮ
			bRepeatSection = true;

			// ** Ready �����ݺ� ���ӽð��� �������� üũ
			if ( readyTime + readyDurationTime < GetTickCount64() )
			{
				// ** ���� �ݺ� ����
				bRepeatSection = false;

				// ** ������ �ѱ� ( ���������� ���� Offset���� ���� )
				_offset = readySectionEnd + 1;
			}
		}
		// ** Fire ���� ���� ����
		else if ( _offset == fireSectionEnd )
		{
			// ** ���� �ݺ� ��ŸƮ
			bRepeatSection = true;

			// ** Fire �����ݺ� ���ӽð��� �������� üũ
			if ( fireTime + fireDurationTime < GetTickCount64() )
			{
				// ** ���� �ݺ� ����
				bRepeatSection = false;

				// ** Loop���̶�� Offset�� �ٽ� 0���� �����ϰ� �ƴ϶�� Default Animation�� Play ��Ų�� 
				if ( bLoopPlayAnim )
					_offset = 0;
				else
					PlayAnimation(eBossAnimationType::DEFAULT, true);
			}
			else
			{
				// ** �ݺ��� ���� Fire ��ŸƮ �������� ����
				_offset = fireSectionStart;
			}
		}
		// ** �ݺ����� ���������� �ƴ϶��
		else
		{
			// Offset ����
			++_offset;
		}
	}
}

void BossAngelEnemy::PlayAnimAttack3(HDC _hdc, ULONGLONG& _time, int& _offset)
{
	static bool bRepeatSection = false; // ** ���� ������ �ݺ��ǰ� �ִ� Ÿ�̹�����
	int maxSegmentCount = 6;			// ** �ش� ���ο��� ���ҵ� �̹��� ��	
	int delay = 200;

	// ** Ready Ÿ�̹�
	static ULONGLONG readyTime = 0;
	int readyDurationTime = 1000;
	int readySectionStart = 0;
	int readySectionEnd = 1;

	// ** Fire Ÿ�̹�
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

	// ** Delay �ð� �ֱ�� Offset ����
	if ( _time + delay < GetTickCount64() )
	{
		_time = GetTickCount64();

		// ** Ready ���� ���� ����
		if ( !bRepeatSection && _offset == readySectionStart )
		{
			// ** Ready ���� Ÿ�� ����
			readyTime = GetTickCount64();
		}

		// ** Fire ���� ���� ����
		if ( !bRepeatSection && _offset == fireSectionStart )
		{
			// ** Fire ���� Ÿ�� ����
			fireTime = GetTickCount64();
		}

		// ** Ready ���� ���� ����
		if ( _offset == readySectionEnd )
		{
			// ** ���� �ݺ� ��ŸƮ
			bRepeatSection = true;

			// ** Ready �����ݺ� ���ӽð��� �������� üũ
			if ( readyTime + readyDurationTime < GetTickCount64() )
			{
				// ** ���� �ݺ� ����
				bRepeatSection = false;

				// ** ������ �ѱ� ( ���������� ���� Offset���� ���� )
				_offset = readySectionEnd + 1;
			}
		}
		// ** Fire ���� ���� ����
		else if ( _offset == fireSectionEnd )
		{
			// ** ���� �ݺ� ��ŸƮ
			bRepeatSection = true;

			// ** Fire �����ݺ� ���ӽð��� �������� üũ
			if ( fireTime + fireDurationTime < GetTickCount64() )
			{
				// ** ���� �ݺ� ����
				bRepeatSection = false;

				// ** Loop���̶�� Offset�� �ٽ� 0���� �����ϰ� �ƴ϶�� Default Animation�� Play ��Ų�� 
				if ( bLoopPlayAnim )
					_offset = 0;
				else
					PlayAnimation(eBossAnimationType::DEFAULT, true);
			}
			else
			{
				// ** �ݺ��� ���� Fire ��ŸƮ �������� ����
				_offset = fireSectionStart;
			}
		}
		// ** �ݺ����� ���������� �ƴ϶��
		else
		{
			// Offset ����
			++_offset;
		}
	}
}

void BossAngelEnemy::PlayAnimEvolution(HDC _hdc, ULONGLONG& _time, int& _offset)
{
	int maxSegmentCount = 8; // ** �ش� ���ο��� ���ҵ� �̹��� ��
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

	// ** Delay �ð� �ֱ�� Offset ����
	if ( _time + delay < GetTickCount64() )
	{
		if ( ++_offset == maxSegmentCount )
		{
			// ** Loop���̶�� Offset�� �ٽ� 0���� �����ϰ� �ƴ϶�� Default Animation�� Play ��Ų�� 
			if ( bLoopPlayAnim ) 
				_offset = 0;
			else
				PlayAnimation(eBossAnimationType::DEFAULT, true);
		}
		_time = GetTickCount64();
	}
}

void BossAngelEnemy::PlayAnimAttacked(HDC _hdc, ULONGLONG& _time, int& _offset)
{
	int maxSegmentCount = 1; // ** �ش� ���ο��� ���ҵ� �̹��� ��
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

	// ** Delay �ð� �ֱ�� Offset ����
	if ( _time + delay < GetTickCount64() )
	{
		// ** Default�� �׻� Loop �����̹Ƿ� Offset�� 0���� ����
		if ( ++_offset == maxSegmentCount )
		{
			if ( bLoopPlayAnim )
				_offset = 0;
			else
				PlayAnimation(eBossAnimationType::DEFAULT, true);
		}

		_time = GetTickCount64();
	}
}

void BossAngelEnemy::InitActionInfo()
{
	// ** Move info �ʱ�ȭ
	if ( pOwner )
	{
		static_cast<Enemy*>(pOwner)->InitMoveInfo();
	}

	// ** Action Info �ʱ�ȭ
	actionInfos = queue<pair<eActionType, float>>();	

	vector<float> delayTimes = { 3000.0f, 2000.0f, 1000.0f };

	actionInfos.push(make_pair(eActionType::MOVE_SPAWN_POS, 0.0f));
	actionInfos.push(make_pair(eActionType::FIRE_FALLDOWN_BACK_AND_FORTH, delayTimes[curPhase - 1]));
	actionInfos.push(make_pair(eActionType::FIRE_TRIANGLE, delayTimes[curPhase - 1]));
	actionInfos.push(make_pair(eActionType::FIRE_DIAMOND, delayTimes[curPhase - 1]));
	actionInfos.push(make_pair(eActionType::FIRE_MULTI_SPIN, delayTimes[curPhase - 1]));
}

void BossAngelEnemy::Action(eActionType _ActionType)
{
	Vector3 bkgPos = pBackground->GetPosition();
	Vector3 bkgScale = pBackground->GetScale();
	
	static ULONGLONG actionTime = GetTickCount64();
	static int moveCount = 0;
	
	switch ( _ActionType )
	{
		case eActionType::MOVE_SPAWN_POS:
			if ( actionState == eActionState::READY )
			{
				speed = 0.75f;
				static_cast<Enemy*>(pOwner)->MoveTo(spawnDestPos);

				moveCount = 1;
			
			}
			else // actionState == eActionState::WORKING
			{
				if ( !static_cast<Enemy*>(pOwner)->isMoving() )
					actionState = eActionState::END;
			}
			break;
		case eActionType::FIRE_FALLDOWN_BACK_AND_FORTH:
			if ( actionState == eActionState::READY )
			{
				actionTime = GetTickCount64();
				speed = 3.0f;
				moveCount = 5;

				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x - bkgScale.x * 0.4f, spawnDestPos.y));
				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x + bkgScale.x * 0.4f, spawnDestPos.y));
				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x - bkgScale.x * 0.4f, spawnDestPos.y));
				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x + bkgScale.x * 0.4f, spawnDestPos.y));
				static_cast<Enemy*>(pOwner)->MoveTo(Vector3(spawnDestPos));				

				PlayAnimation(eBossAnimationType::ATTACK1, true);
			}			
			else
			{
				int bulletSpawnDelay = 200;
				if ( actionTime + bulletSpawnDelay < GetTickCount64() )
				{
					actionTime = GetTickCount64();

					float bulletSpeed = 5.0f;

					// ��ü ���� �¿�� Scale ������ �ΰ� 5���� Bullet �߻�
					for ( int offset = 0; offset < 5; ++offset )
					{
						Transform bulletTransInfo;
						bulletTransInfo.Direction = Vector3(0.0f, 1.0f);
						bulletTransInfo.Position.x = (collider.Position.x - collider.Scale.x) + ((collider.Scale.x * 0.5f) * offset);
						bulletTransInfo.Position.y = collider.Position.y;
						bulletTransInfo.Scale = Vector3(10.0f, 10.0f);

						// ** Bullet Spawn
						SpawnManager::SpawnBullet(pOwner, bulletTransInfo, bulletSpeed, 1, eBridgeKey::BULLET_NORMAL);
					}
				}

				if ( !static_cast<Enemy*>(pOwner)->isMoving() )
				{
					--moveCount;
					if ( moveCount == 0 )
					{
						actionState = eActionState::END;
						PlayAnimation(eBossAnimationType::DEFAULT, true);
					}
				}
			}
			break;
		case eActionType::FIRE_TRIANGLE:
			if ( actionState == eActionState::READY )
			{
				actionTime = GetTickCount64();
				speed = 3.0f;
				moveCount = 6;

				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x - bkgScale.x * 0.24f, bkgPos.y - bkgScale.y * 0.48f));
				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x + bkgScale.x * 0.24f, bkgPos.y - bkgScale.y * 0.48f));
				static_cast<Enemy*>(pOwner)->MoveTo(Vector3(spawnDestPos));
				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x - bkgScale.x * 0.24f, bkgPos.y - bkgScale.y * 0.48f));
				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x + bkgScale.x * 0.24f, bkgPos.y - bkgScale.y * 0.48f));
				static_cast<Enemy*>(pOwner)->MoveTo(Vector3(spawnDestPos));
				
				

				PlayAnimation(eBossAnimationType::ATTACK1, true);
			}
			else
			{
				int bulletSpawnDelay = 500;
				if ( actionTime + bulletSpawnDelay < GetTickCount64() )
				{
					actionTime = GetTickCount64();

					Transform spawnTransInfo;
					spawnTransInfo.Position = collider.Position;
					spawnTransInfo.Scale = collider.Scale;
					spawnTransInfo.Direction = Vector3(0.0f, 1.0f);

					bulletScript.ReadyToSpawn(pOwner, static_cast<eBulletSpawnPattern>(eBulletSpawnPattern::N_POLYGON_GO), spawnTransInfo, 1, 3);
				}

				if ( !static_cast<Enemy*>(pOwner)->isMoving() )
				{
					--moveCount;
					if ( moveCount == 0 )
					{
						actionState = eActionState::END;
						PlayAnimation(eBossAnimationType::DEFAULT, true);
					}
				}
			}
			break;
		case eActionType::FIRE_DIAMOND:
			if ( actionState == eActionState::READY )
			{
				actionTime = GetTickCount64();
				speed = 3.0f;
				moveCount = 9;

				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x, bkgPos.y));
				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x - bkgScale.x * 0.24f, bkgPos.y - bkgScale.y * 0.24f));
				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x, bkgPos.y - bkgScale.y * 0.48f));
				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x + bkgScale.x * 0.24f, bkgPos.y - bkgScale.y * 0.24f));
				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x, bkgPos.y));
				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x - bkgScale.x * 0.24f, bkgPos.y - bkgScale.y * 0.24f));
				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x, bkgPos.y - bkgScale.y * 0.48f));
				static_cast<Enemy*>(pOwner)->MoveTo(
					Vector3(bkgPos.x + bkgScale.x * 0.24f, bkgPos.y - bkgScale.y * 0.24f));
				static_cast<Enemy*>(pOwner)->MoveTo(Vector3(spawnDestPos));

				PlayAnimation(eBossAnimationType::ATTACK2, true);
			}
			else
			{
				int bulletSpawnDelay = 500;
				if ( actionTime + bulletSpawnDelay < GetTickCount64() )
				{
					actionTime = GetTickCount64();

					Transform spawnTransInfo;
					spawnTransInfo.Position = collider.Position;
					spawnTransInfo.Scale = collider.Scale;
					spawnTransInfo.Direction = Vector3(0.0f, 1.0f);

					bulletScript.ReadyToSpawn(pOwner, static_cast<eBulletSpawnPattern>(eBulletSpawnPattern::N_POLYGON_GO), spawnTransInfo, 1, 4);
				}

				if ( !static_cast<Enemy*>(pOwner)->isMoving() )
				{
					--moveCount;
					if ( moveCount == 0 )
					{
						actionState = eActionState::END;
						PlayAnimation(eBossAnimationType::DEFAULT, true);
					}
				}
			}
			break;
		case eActionType::FIRE_MULTI_SPIN:
			if ( actionState == eActionState::READY )
			{
				actionTime = GetTickCount64();
				speed = 3.0f;
				moveCount = 1;

				static_cast<Enemy*>(pOwner)->MoveTo(Vector3(spawnDestPos));

				Transform spawnTransInfo;
				spawnTransInfo.Position = collider.Position;
				spawnTransInfo.Scale = collider.Scale;
				spawnTransInfo.Direction = Vector3(0.0f, 1.0f);

				bulletScript.ReadyToSpawn(pOwner, static_cast<eBulletSpawnPattern>(eBulletSpawnPattern::MULTI_SPIN_GO), spawnTransInfo, 1);

				PlayAnimation(eBossAnimationType::ATTACK2, true);
			}
			else
			{
				int actionDelayTime = 10000;

				if ( actionTime + actionDelayTime < GetTickCount64() )
				{
					if ( !static_cast<Enemy*>(pOwner)->isMoving() )
					{
						--moveCount;
						if ( moveCount == 0 )
						{
							actionState = eActionState::END;
							PlayAnimation(eBossAnimationType::DEFAULT, true);
						}
					}
				}
			}
			break;
	}
}

void BossAngelEnemy::CheckPhase()
{
	int maxHP = static_cast<Enemy*>(pOwner)->GetMaxHP();
	int HP = static_cast<Enemy*>(pOwner)->GetHP();
	float HpRatio = HP / static_cast<float>(maxHP);

	// ü���� 33% �̸��� �� ������ 3
	if ( HpRatio < 0.33f )
	{
		curPhase = 3;
	}
	// ü���� 66% �̸��� �� ������ 2
	else if ( HpRatio < 0.66f )
	{
		curPhase = 2;
	}
	// �⺻ ������ 1
	else
	{
		curPhase = 1;
	}
}
