#include "ScrollHorizontalBkg.h"
#include "BitmapManager.h"

ScrollHorizontalBkg::ScrollHorizontalBkg()
	: bLoopScroll(false)
	, LoopOffset1(0)
	, LoopOffset2(0)
	, bAttachBkg(false)
{
}

ScrollHorizontalBkg::~ScrollHorizontalBkg()
{
	Release();
}

void ScrollHorizontalBkg::Initialize()
{
	bLoopScroll = false;
	LoopOffset1 = 0;
	LoopOffset2 = 0;
	bAttachBkg = false;
}

void ScrollHorizontalBkg::Update()
{
}

void ScrollHorizontalBkg::Render(HDC _hdc)
{
	Bitmap* pImage = pOwner->GetImage();
	if ( !pImage )
		return;

	// ** �̾� ���̴� ���� �ƴ� �� ����� ���� �����ߴٸ�
	if ( !bAttachBkg && pOwner->GetScale().x < WindowsWidth + LoopOffset1 )
	{
		// ** �̾� ���̵��� Offset2�� bAttachBkg ����
		LoopOffset2 = 0;
		bAttachBkg = true;
	}

	// ** �̾� ���̴� ���� �� �̾���� ����� ȭ���� ���� ä��ٸ�
	if ( bAttachBkg && WindowsWidth < LoopOffset2 )
	{
		// ** �׸��� �� ���� ȭ���� �� ä�� �� �ֵ��� Offset1�� bAttachBkg ����
		LoopOffset1 = 0;
		bAttachBkg = false;
	}


	// ** �� ���� �׸�
	if ( !bAttachBkg )
	{
		BitBlt(_hdc,
			0,
			0,
			WindowsWidth,
			WindowsHeight,
			pImage->GetMemDC(),
			LoopOffset1,
			0,
			SRCCOPY);
	}
	// ** �� �κ�, �� �κ� �׸���� ������ �̾����
	else
	{
		BitBlt(_hdc,
			0,
			0,
			WindowsWidth - LoopOffset2,
			WindowsHeight,
			pImage->GetMemDC(),
			LoopOffset1,
			0,
			SRCCOPY);

		BitBlt(_hdc,
			WindowsWidth - LoopOffset2,
			0,
			LoopOffset2,
			WindowsHeight,
			pImage->GetMemDC(),
			0,
			0,
			SRCCOPY);
	}

	LoopOffset1 += static_cast<int>(pOwner->GetSpeed());
	LoopOffset2 += static_cast<int>(pOwner->GetSpeed());
}

void ScrollHorizontalBkg::Release()
{

}
