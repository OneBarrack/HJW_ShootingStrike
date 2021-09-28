#include "ScrollHorizontalBkg.h"
#include "BitmapManager.h"

ScrollHorizontalBkg::ScrollHorizontalBkg()
	: ScrollDirection(eScrollDirection::LEFT)
	, ImageOffset(0.0f)
	, LoopOffset(0.0f)
	, bLoop(false)
	, bDrawEachStartEnd(false)
{
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
	ReceiveInfoFromOwner();

	// ** ������ ��ũ��
	if ( ScrollDirection == eScrollDirection::LEFT )
	{
		// ** Loop�� ���� �̹��� ���� �۾����̶��
		if ( bDrawEachStartEnd )
		{
			LoopOffset += Speed;

			// ** �̹��� ���� ������ �����ٸ�
			if ( LoopOffset > TransInfo.Scale.x )
			{
				ImageOffset = pImage->GetScale().x - TransInfo.Scale.x;
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
			if ( ImageOffset > pImage->GetScale().x - TransInfo.Scale.x )
			{
				// ** �̹��� ���ϴ� Offset���� ����
				ImageOffset = pImage->GetScale().x - TransInfo.Scale.x;

				// ** Loop �Ǿ�� �ϴ� ���¶��
				if ( bLoop && !bDrawEachStartEnd )
				{
					LoopOffset = TransInfo.Scale.x;
					bDrawEachStartEnd = true;
				}
			}
		}
	}

	SendInfoToOwner();
}

void ScrollHorizontalBkg::Render(HDC _hdc)
{
	if ( !pImage ) 
		return;

	// ** �̹��� ���۰� �� ���� ����
	// ** Loop Scroll
	if ( bLoop && bDrawEachStartEnd )
	{
		TransparentBlt(_hdc,
			(int)(TransInfo.Position.x - (TransInfo.Scale.x * 0.5f)),
			(int)(TransInfo.Position.y - (TransInfo.Scale.y * 0.5f)),
			(int)(LoopOffset),
			(int)(TransInfo.Scale.y),
			pImage->GetMemDC(),
			(int)(pImage->GetScale().x - LoopOffset),
			(int)(pImage->GetSegmentationScale().y * pOwner->GetImageOffsetOrder().y),
			(int)(LoopOffset),
			(int)(pImage->GetSegmentationScale().y),
			RGB(255, 0, 255));

		TransparentBlt(_hdc,
			(int)((TransInfo.Position.x - (TransInfo.Scale.x * 0.5f)) + LoopOffset),
			(int)(TransInfo.Position.y - (TransInfo.Scale.y * 0.5f)),
			(int)(TransInfo.Scale.x - LoopOffset),
			(int)(TransInfo.Scale.y),
			pImage->GetMemDC(),
			0,
			(int)(pImage->GetSegmentationScale().y * pOwner->GetImageOffsetOrder().y),
			(int)(TransInfo.Scale.x - LoopOffset),
			(int)(pImage->GetSegmentationScale().y),
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
			(int)(ImageOffset),
			(int)(pImage->GetSegmentationScale().y * pOwner->GetImageOffsetOrder().y),
			(int)(TransInfo.Scale.x),
			(int)(pImage->GetSegmentationScale().y),
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