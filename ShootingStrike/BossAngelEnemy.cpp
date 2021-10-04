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
	Super::Initialize();

	key = eBridgeKey::ENEMY_BOSS;

	animType = AnimationType::DEFAULT;
	bLoopPlayAnim = false;
}

void BossAngelEnemy::Update()
{
	Super::Update();
	// ** Owner�� �����͸� �޾ƿ�
	ReceiveInfoFromOwner();
	
	// �浹ü ����
	collider.Position = Vector3(transInfo.Position.x, transInfo.Position.y + (transInfo.Scale.y * 0.2f));
	collider.Scale = Vector3(transInfo.Scale.x * 0.4f, transInfo.Scale.y * 0.5f);

	// _Debug_ : Update ��ü �׽�Ʈ ��
	if ( CHECK_KEYINPUT_STATE(eInputKey::KEY_LBUTTON, eKeyInputState::DOWN) )
	{
		static int animTypeIndex = 0;
		static int patternIndex = 0;

		if ( animTypeIndex == static_cast<int>(AnimationType::EVOLUTION) )
			animTypeIndex = 0;
		else
			animTypeIndex++;

		if ( patternIndex == static_cast<int>(eBulletSpawnPattern::CIRCLE) )
			patternIndex = 1;
		else
			patternIndex++;
			
		PlayAnimation(static_cast<AnimationType>(animTypeIndex), false);
		bulletScript.ReadyToSpawn(static_cast<eBulletSpawnPattern>(patternIndex), collider.Position, 1);
		
		//SpawnManager::SpawnEffect(transInfo, eBridgeKey::EFFECT_EXPLOSION);
	}

	//transInfo.Position.x += transInfo.Direction.x * Speed;
	//transInfo.Position.y += transInfo.Direction.y * Speed;	

	// ** Owner�� ������ ������ ����
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
					PlayAnimation(AnimationType::DEFAULT, true);
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
					PlayAnimation(AnimationType::DEFAULT, true);
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
					PlayAnimation(AnimationType::DEFAULT, true);
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
				PlayAnimation(AnimationType::DEFAULT, true);
		}
		_time = GetTickCount64();
	}
}

void BossAngelEnemy::PlayAnimAttacked(HDC _hdc, ULONGLONG& _time, int& _offset)
{
	int MaxSegmentCount = 1; // ** �ش� ���ο��� ���ҵ� �̹��� ��
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

	// ** Delay �ð� �ֱ�� Offset ����
	if ( _time + Delay < GetTickCount64() )
	{
		// ** Default�� �׻� Loop �����̹Ƿ� Offset�� 0���� ����
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
