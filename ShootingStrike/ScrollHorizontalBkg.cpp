#include "ScrollHorizontalBkg.h"
#include "BitmapManager.h"

ScrollHorizontalBkg::ScrollHorizontalBkg()
	: scrollDirection(eScrollDirection::LEFT)
	, imageOffset(0.0f)
	, imageOffsetForRestart(0.0f)
	, bLoop(false)
	, bDrawEachStartEnd(false)
{
}

ScrollHorizontalBkg::~ScrollHorizontalBkg()
{
}

void ScrollHorizontalBkg::Initialize()
{
	Super::Initialize();

	key = eBridgeKey::BACKGROUND_SCROLL_HORIZONTAL;
	scrollDirection = eScrollDirection::LEFT;
	imageOffset = 0.0f;
	bLoop = true;
	bDrawEachStartEnd = false;
}

void ScrollHorizontalBkg::Update()
{
	Super::Update();

	ReceiveInfoFromOwner();

	// ** ������ ��ũ��
	if ( scrollDirection == eScrollDirection::LEFT )
	{
		// ** Loop�� ���� �̹��� ���� �۾����̶��
		if ( bDrawEachStartEnd )
		{
			imageOffsetForRestart += speed;

			// ** �̹��� ���� ������ �����ٸ�
			if ( imageOffsetForRestart > transInfo.Scale.x )
			{
				imageOffset = pImage->GetScale().x - transInfo.Scale.x;
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
			if ( imageOffset > pImage->GetScale().x - transInfo.Scale.x )
			{
				// ** �̹��� ���ϴ� Offset���� ����
				imageOffset = pImage->GetScale().x - transInfo.Scale.x;

				// ** Loop �Ǿ�� �ϴ� ���¶��
				if ( bLoop && !bDrawEachStartEnd )
				{
					imageOffsetForRestart = transInfo.Scale.x;
					bDrawEachStartEnd = true;
				}
			}
		}
	}

	SendInfoToOwner();
}

void ScrollHorizontalBkg::Render(HDC _hdc)
{
	Super::Render(_hdc);

	if ( !pImage ) 
		return;

	// ** �̹��� ���۰� �� ���� ����
	// ** Loop Scroll
	if ( bLoop && bDrawEachStartEnd )
	{
		TransparentBlt(_hdc,
			(int)(transInfo.Position.x - (transInfo.Scale.x * 0.5f)),
			(int)(transInfo.Position.y - (transInfo.Scale.y * 0.5f)),
			(int)(imageOffsetForRestart),
			(int)(transInfo.Scale.y),
			pImage->GetMemDC(),
			(int)(pImage->GetScale().x - imageOffsetForRestart),
			(int)(pImage->GetSegmentationScale().y * pOwner->GetImageOffsetOrder().y),
			(int)(imageOffsetForRestart),
			(int)(pImage->GetSegmentationScale().y),
			RGB(255, 0, 255));

		TransparentBlt(_hdc,
			(int)((transInfo.Position.x - (transInfo.Scale.x * 0.5f)) + imageOffsetForRestart),
			(int)(transInfo.Position.y - (transInfo.Scale.y * 0.5f)),
			(int)(transInfo.Scale.x - imageOffsetForRestart),
			(int)(transInfo.Scale.y),
			pImage->GetMemDC(),
			0,
			(int)(pImage->GetSegmentationScale().y * pOwner->GetImageOffsetOrder().y),
			(int)(transInfo.Scale.x - imageOffsetForRestart),
			(int)(pImage->GetSegmentationScale().y),
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
			(int)(imageOffset),
			(int)(pImage->GetSegmentationScale().y * pOwner->GetImageOffsetOrder().y),
			(int)(transInfo.Scale.x),
			(int)(pImage->GetSegmentationScale().y),
			RGB(255, 0, 255));
	}
}

void ScrollHorizontalBkg::Release()
{
	Super::Release();
}

void ScrollHorizontalBkg::StartLeft()
{
	imageOffset = 0.0f;
}

void ScrollHorizontalBkg::StartRight()
{
	imageOffset = pOwner->GetImage()->GetScale().x - pOwner->GetScale().x;
}

void ScrollHorizontalBkg::ScrollLeft()
{
	scrollDirection = eScrollDirection::LEFT;
}

void ScrollHorizontalBkg::ScrollRight()
{
	scrollDirection = eScrollDirection::RIGHT;
}