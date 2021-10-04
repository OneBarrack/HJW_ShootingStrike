#include "ScrollVerticalBkg.h"
#include "BitmapManager.h"

ScrollVerticalBkg::ScrollVerticalBkg()
	: scrollDirection(eScrollDirection::DOWN)
	, imageOffset(0.0f)
	, imageOffsetForRestart(0.0f)
	, bLoop(false)
	, bDrawEachStartEnd(false)
{	
}

ScrollVerticalBkg::~ScrollVerticalBkg()
{
}

void ScrollVerticalBkg::Initialize()
{
	Super::Initialize();

	key = eBridgeKey::BACKGROUND_SCROLL_VERTICAL;
	scrollDirection = eScrollDirection::DOWN;
	imageOffset = 0.0f;
	bLoop = true;
	bDrawEachStartEnd = false;
}

void ScrollVerticalBkg::Update()
{
	Super::Update();

	ReceiveInfoFromOwner();
	
	// ** ������ ��ũ��
	if ( scrollDirection == eScrollDirection::UP )
	{
		// ** Loop�� ���� �̹��� ���� �۾����̶��
		if ( bDrawEachStartEnd )
		{
			imageOffsetForRestart += speed;

			// ** �̹��� ���� ������ �����ٸ�
			if ( imageOffsetForRestart > transInfo.Scale.y )
			{
				imageOffset = pImage->GetScale().y - transInfo.Scale.y;
				bDrawEachStartEnd = false;
			}
		}
		else
		{
			imageOffset -= speed;

			// ** �̹��� �ֻ�ܿ� �����ߴٸ�
			if ( imageOffset < 0.0f )
			{
				// ** �̹��� �ֻ�� Offset���� ����
				imageOffset = 0.0f;

				// ** Loop �Ǿ�� �ϴ� ���¶��
				if ( bLoop && !bDrawEachStartEnd )
				{					
					imageOffsetForRestart = 0.0f;
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
			imageOffsetForRestart -= speed;

			// ** �̹��� ���� ������ �����ٸ�
			if ( imageOffsetForRestart < 0.0f )
			{
				imageOffset = 0.0f;
				bDrawEachStartEnd = false;
			}
		}
		else
		{
			imageOffset += speed;

			// ** �̹��� ���ϴܿ� �����ߴٸ�
			if ( imageOffset > pImage->GetScale().y - transInfo.Scale.y )
			{
				// ** �̹��� ���ϴ� Offset���� ����
				imageOffset = pImage->GetScale().y - transInfo.Scale.y;

				// ** Loop �Ǿ�� �ϴ� ���¶��
				if ( bLoop && !bDrawEachStartEnd )
				{
					imageOffsetForRestart = pOwner->GetScale().y;
					bDrawEachStartEnd = true;
				}
			}
		}
	}

	SendInfoToOwner();
}

void ScrollVerticalBkg::Render(HDC _hdc)
{
	Super::Render(_hdc);

	if ( !pImage )
		return;

	// ** �̹��� ���۰� �� ���� ����
	// ** Loop Scroll
	if (bLoop && bDrawEachStartEnd )
	{
		TransparentBlt(_hdc,
			(int)(transInfo.Position.x - (transInfo.Scale.x * 0.5f)),
			(int)(transInfo.Position.y - (transInfo.Scale.y * 0.5f)),
			(int)(transInfo.Scale.x),
			(int)(imageOffsetForRestart),
			pImage->GetMemDC(),
			(int)(pImage->GetSegmentationScale().x * pOwner->GetImageOffsetOrder().x),
			(int)(pImage->GetScale().y - imageOffsetForRestart),
			(int)(pImage->GetSegmentationScale().x),
			(int)(imageOffsetForRestart),
			RGB(255, 0, 255));

		TransparentBlt(_hdc,
			(int)(transInfo.Position.x - (transInfo.Scale.x * 0.5f)),
			(int)((transInfo.Position.y - (transInfo.Scale.y * 0.5f)) + imageOffsetForRestart),
			(int)(transInfo.Scale.x),
			(int)(transInfo.Scale.y - imageOffsetForRestart),
			pImage->GetMemDC(),
			(int)(pImage->GetSegmentationScale().x * pOwner->GetImageOffsetOrder().x),
			0,
			(int)(pImage->GetSegmentationScale().x),
			(int)(transInfo.Scale.y - imageOffsetForRestart),
			RGB(255, 0, 255));
	}
	// ** �Ϲ� Scroll
	else
	{
		TransparentBlt(_hdc,
			(int)(transInfo.Position.x - (transInfo.Scale.x * 0.5f)),
			(int)(transInfo.Position.y - (transInfo.Scale.y * 0.5f)),
			(int)(transInfo.Scale.x),
			(int)(transInfo.Scale.y),
			pImage->GetMemDC(),
			(int)(pImage->GetSegmentationScale().x * pOwner->GetImageOffsetOrder().x),
			(int)(imageOffset),
			(int)(pImage->GetSegmentationScale().x),
			(int)(transInfo.Scale.y),
			RGB(255, 0, 255));
	}	
}

void ScrollVerticalBkg::Release()
{
	Super::Release();
}

void ScrollVerticalBkg::StartTop() 
{ 
	imageOffset = 0.0f;
}

void ScrollVerticalBkg::StartBottom() 
{ 
	imageOffset = pOwner->GetImage()->GetScale().y - pOwner->GetScale().y;		
}

void ScrollVerticalBkg::ScrollUp() 
{
	scrollDirection = eScrollDirection::UP; 
}

void ScrollVerticalBkg::ScrollDown() 
{
	scrollDirection = eScrollDirection::DOWN;
}