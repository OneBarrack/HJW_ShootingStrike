#include "ScrollHorizontalBkg.h"
#include "BitmapManager.h"

ScrollHorizontalBkg::ScrollHorizontalBkg()
	: ScrollDirection(eScrollDirection::LEFT)
	, ImageOffset(0.0f)
	, LoopOffset(0.0f)
	, bLoop(false)
	, bDrawEachStartEnd(false)
{
	Initialize();
}

ScrollHorizontalBkg::~ScrollHorizontalBkg()
{
	Release();
}

void ScrollHorizontalBkg::Initialize()
{
	Key = eBridgeKey::BACKGROUND_SCROLL_HORIZONTAL;
	ScrollDirection = eScrollDirection::LEFT;
	ImageOffset = 0.0f;
	bLoop = true;
	bDrawEachStartEnd = false;
}

void ScrollHorizontalBkg::Update()
{
	float ScrollSpeed = pOwner->GetSpeed();

	// ** ������ ��ũ��
	if ( ScrollDirection == eScrollDirection::LEFT )
	{
		// ** Loop�� ���� �̹��� ���� �۾����̶��
		if ( bDrawEachStartEnd )
		{
			LoopOffset += ScrollSpeed;

			// ** �̹��� ���� ������ �����ٸ�
			if ( LoopOffset > pOwner->GetScale().x )
			{
				ImageOffset = pOwner->GetImage()->GetScale().x - pOwner->GetScale().x;
				bDrawEachStartEnd = false;
			}
		}
		else
		{
			ImageOffset -= ScrollSpeed;

			// ** �̹��� �ֻ�ܿ� �����ߴٸ�
			if ( ImageOffset < 0.0f )
			{
				// ** �̹��� �ֻ�� Offset���� ����
				ImageOffset = 0.0f;

				// ** Loop �Ǿ�� �ϴ� ���¶��
				if ( bLoop && !bDrawEachStartEnd )
				{
					LoopOffset = 0.0f;
					bDrawEachStartEnd = true;
				}
			}
		}
	}
	// ** �Ʒ����� ��ũ�� Direction == eScrollDirection::DOWN
	else
	{
		// ** Loop�� ���� �̹��� ���� �۾�������
		if ( bDrawEachStartEnd )
		{
			LoopOffset -= ScrollSpeed;

			// ** �̹��� ���� ������ �����ٸ�
			if ( LoopOffset < 0.0f )
			{
				ImageOffset = 0.0f;
				bDrawEachStartEnd = false;
			}
		}
		else
		{
			ImageOffset += ScrollSpeed;

			// ** �̹��� ���ϴܿ� �����ߴٸ�
			if ( ImageOffset > pOwner->GetImage()->GetScale().x - pOwner->GetScale().x )
			{
				// ** �̹��� ���ϴ� Offset���� ����
				ImageOffset = pOwner->GetImage()->GetScale().x - pOwner->GetScale().x;

				// ** Loop �Ǿ�� �ϴ� ���¶��
				if ( bLoop && !bDrawEachStartEnd )
				{
					LoopOffset = pOwner->GetScale().x;
					bDrawEachStartEnd = true;
				}
			}
		}
	}
}

void ScrollHorizontalBkg::Render(HDC _hdc)
{
	// ** �̹��� ���۰� �� ���� ����
	// ** Loop Scroll
	if ( bLoop && bDrawEachStartEnd )
	{
		TransparentBlt(_hdc,
			(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
			(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
			(int)(LoopOffset),
			(int)(pOwner->GetScale().y),
			pOwner->GetImage()->GetMemDC(),
			(int)(pOwner->GetImage()->GetScale().x - LoopOffset),
			(int)(pOwner->GetImage()->GetSegmentationScale().y * pOwner->GetImageOffsetOrder().y),
			(int)(LoopOffset),
			(int)(pOwner->GetImage()->GetSegmentationScale().y),
			RGB(255, 0, 255));

		TransparentBlt(_hdc,
			(int)((pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)) + LoopOffset),
			(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
			(int)(pOwner->GetScale().x - LoopOffset),
			(int)(pOwner->GetScale().y),
			pOwner->GetImage()->GetMemDC(),
			0,
			(int)(pOwner->GetImage()->GetSegmentationScale().y * pOwner->GetImageOffsetOrder().y),
			(int)(pOwner->GetScale().x - LoopOffset),
			(int)(pOwner->GetImage()->GetSegmentationScale().y),
			RGB(255, 0, 255));
	}
	// ** �Ϲ� Scroll
	else
	{
		TransparentBlt(_hdc,
			(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
			(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
			(int)(pOwner->GetScale().x),
			(int)(pOwner->GetScale().y),
			pOwner->GetImage()->GetMemDC(),
			(int)(ImageOffset),
			(int)(pOwner->GetImage()->GetSegmentationScale().y * pOwner->GetImageOffsetOrder().y),
			(int)(pOwner->GetScale().x),
			(int)(pOwner->GetImage()->GetSegmentationScale().y),
			RGB(255, 0, 255));
	}
}

void ScrollHorizontalBkg::Release()
{

}

void ScrollHorizontalBkg::StartLeft()
{
	ImageOffset = 0.0f;
}

void ScrollHorizontalBkg::StartRight()
{
	ImageOffset = pOwner->GetImage()->GetScale().x - pOwner->GetScale().x;
}

void ScrollHorizontalBkg::ScrollLeft()
{
	ScrollDirection = eScrollDirection::LEFT;
}

void ScrollHorizontalBkg::ScrollRight()
{
	ScrollDirection = eScrollDirection::RIGHT;
}