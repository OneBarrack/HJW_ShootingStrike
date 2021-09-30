#include "ScrollVerticalBkg.h"
#include "BitmapManager.h"

ScrollVerticalBkg::ScrollVerticalBkg()
	: ScrollDirection(eScrollDirection::DOWN)
	, ImageOffset(0.0f)
	, LoopOffset(0.0f)
	, bLoop(false)
	, bDrawEachStartEnd(false)
{	
}

ScrollVerticalBkg::~ScrollVerticalBkg()
{
}

void ScrollVerticalBkg::Initialize()
{
	Key = eBridgeKey::BACKGROUND_SCROLL_VERTICAL;
	ScrollDirection = eScrollDirection::DOWN;
	ImageOffset = 0.0f;
	bLoop = true;
	bDrawEachStartEnd = false;
}

void ScrollVerticalBkg::Update()
{
	ReceiveInfoFromOwner();
	
	// ** ������ ��ũ��
	if ( ScrollDirection == eScrollDirection::UP )
	{
		// ** Loop�� ���� �̹��� ���� �۾����̶��
		if ( bDrawEachStartEnd )
		{
			LoopOffset += Speed;

			// ** �̹��� ���� ������ �����ٸ�
			if ( LoopOffset > TransInfo.Scale.y )
			{
				ImageOffset = pImage->GetScale().y - TransInfo.Scale.y;
				bDrawEachStartEnd = false;
			}
		}
		else
		{
			ImageOffset -= Speed;

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
			LoopOffset -= Speed;

			// ** �̹��� ���� ������ �����ٸ�
			if ( LoopOffset < 0.0f )
			{
				ImageOffset = 0.0f;
				bDrawEachStartEnd = false;
			}
		}
		else
		{
			ImageOffset += Speed;

			// ** �̹��� ���ϴܿ� �����ߴٸ�
			if ( ImageOffset > pImage->GetScale().y - TransInfo.Scale.y )
			{
				// ** �̹��� ���ϴ� Offset���� ����
				ImageOffset = pImage->GetScale().y - TransInfo.Scale.y;

				// ** Loop �Ǿ�� �ϴ� ���¶��
				if ( bLoop && !bDrawEachStartEnd )
				{
					LoopOffset = pOwner->GetScale().y;
					bDrawEachStartEnd = true;
				}
			}
		}
	}

	SendInfoToOwner();
}

void ScrollVerticalBkg::Render(HDC _hdc)
{
	if ( !pImage )
		return;

	// ** �̹��� ���۰� �� ���� ����
	// ** Loop Scroll
	if (bLoop && bDrawEachStartEnd )
	{
		TransparentBlt(_hdc,
			(int)(TransInfo.Position.x - (TransInfo.Scale.x * 0.5f)),
			(int)(TransInfo.Position.y - (TransInfo.Scale.y * 0.5f)),
			(int)(TransInfo.Scale.x),
			(int)(LoopOffset),
			pImage->GetMemDC(),
			(int)(pImage->GetSegmentationScale().x * pOwner->GetImageOffsetOrder().x),
			(int)(pImage->GetScale().y - LoopOffset),
			(int)(pImage->GetSegmentationScale().x),
			(int)(LoopOffset),
			RGB(255, 0, 255));

		TransparentBlt(_hdc,
			(int)(TransInfo.Position.x - (TransInfo.Scale.x * 0.5f)),
			(int)((TransInfo.Position.y - (TransInfo.Scale.y * 0.5f)) + LoopOffset),
			(int)(TransInfo.Scale.x),
			(int)(TransInfo.Scale.y - LoopOffset),
			pImage->GetMemDC(),
			(int)(pImage->GetSegmentationScale().x * pOwner->GetImageOffsetOrder().x),
			0,
			(int)(pImage->GetSegmentationScale().x),
			(int)(TransInfo.Scale.y - LoopOffset),
			RGB(255, 0, 255));
	}
	// ** �Ϲ� Scroll
	else
	{
		TransparentBlt(_hdc,
			(int)(TransInfo.Position.x - (TransInfo.Scale.x * 0.5f)),
			(int)(TransInfo.Position.y - (TransInfo.Scale.y * 0.5f)),
			(int)(TransInfo.Scale.x),
			(int)(TransInfo.Scale.y),
			pImage->GetMemDC(),
			(int)(pImage->GetSegmentationScale().x * pOwner->GetImageOffsetOrder().x),
			(int)(ImageOffset),
			(int)(pImage->GetSegmentationScale().x),
			(int)(TransInfo.Scale.y),
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