#pragma once
#include "Object.h"

class Button : public Object
{
	enum class eButtonState
	{
		NORMAL	,
		HOVER	,
		PRESSED	,
	};

private:
	Bitmap* pButtonImage;
	bool bExistHoverImage;
	bool bExistPressedImage;
	eButtonState ButtonState;
	int ButtonOffset;
	bool bOnClick;

public:
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Render(HDC _hdc)override;
	virtual void Release()override;
	virtual void OnCollision(Object* _pObject)override;

	virtual Object* Clone() { return new Button(*this);	};
public:
	// ** �̹��� ����
	void SetImage(Bitmap* _pImage) { pButtonImage = _pImage; }

	// ** Ŭ�� �Ǿ�����
	bool OnClick() { return bOnClick; }

public:
	Button();
	Button(const Transform& _rTransInfo) : Object(_rTransInfo) {}
	virtual ~Button();
};

