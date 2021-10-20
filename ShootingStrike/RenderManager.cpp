#include "RenderManager.h"

void RenderManager::DrawRect(HDC _hdc, Transform _transInfo, COLORREF _color, bool _bFillInside)
{
	HPEN hPen = CreatePen(PS_SOLID, 2, _color);
	HGDIOBJ hOldPen = SelectObject(_hdc, hPen);

	HBRUSH hBrush;	
	if ( _bFillInside )
		hBrush = CreateSolidBrush(_color); // ** ���� �ֱ�
	else
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH); // ** ����

	HGDIOBJ hOldBrush = SelectObject(_hdc, hBrush);
	
	Rectangle(_hdc,
		int(_transInfo.Position.x - (_transInfo.Scale.x / 2)),
		int(_transInfo.Position.y - (_transInfo.Scale.y / 2)),
		int(_transInfo.Position.x + (_transInfo.Scale.x / 2)),
		int(_transInfo.Position.y + (_transInfo.Scale.y / 2)));

	SelectObject(_hdc, hOldPen);
	SelectObject(_hdc, hOldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void RenderManager::DrawEllipse(HDC _hdc, Transform _transInfo, COLORREF _color, bool _bFillInside)
{
	HPEN hPen = CreatePen(PS_SOLID, 2, _color);
	HGDIOBJ hOldPen = SelectObject(_hdc, hPen);

	HBRUSH hBrush;
	if ( _bFillInside )
		hBrush = CreateSolidBrush(_color); // ** ���� �ֱ�
	else
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH); // ** ����

	HGDIOBJ hOldBrush = SelectObject(_hdc, hBrush);

	Ellipse(_hdc,
		int(_transInfo.Position.x - (_transInfo.Scale.x / 2)),
		int(_transInfo.Position.y - (_transInfo.Scale.y / 2)),
		int(_transInfo.Position.x + (_transInfo.Scale.x / 2)),
		int(_transInfo.Position.y + (_transInfo.Scale.y / 2)));

	SelectObject(_hdc, hOldPen);
	SelectObject(_hdc, hOldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

bool RenderManager::FadeAnimation(HDC _hdc, eFadeStatus fadeStatus)
{
	static BYTE alpha = 0;
	int fadeSpeed = 3;

	if ( fadeStatus == eFadeStatus::FADE_IN  && alpha > 0   ) alpha -= fadeSpeed;
	if ( fadeStatus == eFadeStatus::FADE_OUT && alpha < 255 ) alpha += fadeSpeed;

	BLENDFUNCTION bf;
	bf.AlphaFormat			= 0;			// ** �Ϲ� ��Ʈ�� 0, 32��Ʈ ��Ʈ�� AC_SRC_ALPHA
	bf.BlendFlags			= 0;			// ** ������ �ʴ� �ɼ�. ������ 0
	bf.BlendOp				= AC_SRC_OVER;  // ** AC_SRC_OVER
	bf.SourceConstantAlpha	= alpha;		// ** ����(���� 0 - ������ 255)

	// ** Fade Background �̹����� �������� Fade ȿ�� �ο�
	AlphaBlend(_hdc, 0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT,
		BitmapManager::GetInstance()->GetImage(eImageKey::FADEBACK_BLACK)->GetMemDC(),
		0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT, bf);

	// ** alpha�� 0 �Ǵ� 255��� Fade Animation�� ������.
	return (alpha == 0 || alpha == 255);
}

void RenderManager::RenderToScreen(HDC _hdc)
{
	BitBlt(_hdc,
		0, 0,
		WINDOWS_WIDTH,
		WINDOWS_HEIGHT,
		GetBufferDC(),
		0, 0,
		SRCCOPY);
}
