#include "ScrollVerticalBkg.h"
#include "BitmapManager.h"

ScrollVerticalBkg::ScrollVerticalBkg()
	: ScrollDirection(eScrollDirection::DOWN)
	, ImageOffset(0.0f)
	, LoopOffset(0.0f)
	, bLoop(false)
	, bDrawEachStartEnd(false)
{
	Initialize();
}

ScrollVerticalBkg::~ScrollVerticalBkg()
{
	Release();
}

void ScrollVerticalBkg::Initialize()
{
	ScrollDirection = eScrollDirection::DOWN;
	ImageOffset = 0.0f;
	bLoop = true;
	bDrawEachStartEnd = false;
}

void ScrollVerticalBkg::Update()
{
	float ScrollSpeed = pOwner->GetSpeed();
	
	// ** ������ ��ũ��
	if ( ScrollDirection == eScrollDirection::UP )
	{
		// ** Loop�� ���� �̹��� ���� �۾����̶��
		if ( bDrawEachStartEnd )
		{
			LoopOffset += ScrollSpeed;

			// ** �̹��� ���� ������ �����ٸ�
			if ( LoopOffset > pOwner->GetScale().y )
			{
				ImageOffset = pOwner->GetImage()->GetScale().y - pOwner->GetScale().y;
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
			if ( ImageOffset > pOwner->GetImage()->GetScale().y - pOwner->GetScale().y )
			{
				// ** �̹��� ���ϴ� Offset���� ����
				ImageOffset = pOwner->GetImage()->GetScale().y - pOwner->GetScale().y;

				// ** Loop �Ǿ�� �ϴ� ���¶��
				if ( bLoop && !bDrawEachStartEnd )
				{
					LoopOffset = pOwner->GetScale().y;
					bDrawEachStartEnd = true;
				}
			}
		}
	}
}

void ScrollVerticalBkg::Render(HDC _hdc)
{
	// ** �̹��� ���۰� �� ���� ����
	// ** Loop Scroll
	if (bLoop && bDrawEachStartEnd )
	{
		TransparentBlt(_hdc,
			(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
			(int)(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
			(int)(pOwner->GetScale().x),
			(int)(LoopOffset),
			pOwner->GetImage()->GetMemDC(),
			(int)(pOwner->GetImage()->GetSegmentationScale().x * pOwner->GetImageOffsetOrder().x),
			(int)(pOwner->GetImage()->GetScale().y - LoopOffset),
			(int)(pOwner->GetImage()->GetSegmentationScale().x),
			(int)(LoopOffset),
			RGB(255, 0, 255));

		TransparentBlt(_hdc,
			(int)(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
			(int)((pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)) + LoopOffset),
			(int)(pOwner->GetScale().x),
			(int)(pOwner->GetScale().y - LoopOffset),
			pOwner->GetImage()->GetMemDC(),
			(int)(pOwner->GetImage()->GetSegmentationScale().x * pOwner->GetImageOffsetOrder().x),
			0,
			(int)(pOwner->GetImage()->GetSegmentationScale().x),
			(int)(pOwner->GetScale().y - LoopOffset),
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
			(int)(pOwner->GetImage()->GetSegmentationScale().x * pOwner->GetImageOffsetOrder().x),
			(int)(ImageOffset),
			(int)(pOwner->GetImage()->GetSegmentationScale().x),
			(int)(pOwner->GetScale().y),
			RGB(255, 0, 255));
	}	
}

void ScrollVerticalBkg::Release()
{

}

void ScrollVerticalBkg::StartTop() 
{ 
	ImageOffset = 0.0f;
}

void ScrollVerticalBkg::StartBottom() 
{ 
	ImageOffset = pOwner->GetImage()->GetScale().y - pOwner->GetScale().y;		
}

void ScrollVerticalBkg::ScrollUp() 
{
	ScrollDirection = eScrollDirection::UP; 
}

void ScrollVerticalBkg::ScrollDown() 
{
	ScrollDirection = eScrollDirection::DOWN;
}