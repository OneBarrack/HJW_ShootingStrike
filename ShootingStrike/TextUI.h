#pragma once
#include "UIBridge.h"

class TextUI : public UIBridge
{
	enum class eTextType
	{
		ALPHABET,
		NUMBER	,
		SPECIAL	,
	};

public:
	typedef UIBridge Super;

private:
	// ** Text ��¿� ����� �̹���
	Bitmap* pTextImage;

	// ** �޾ƿ� ���ڿ��� ���� ������ ������ ������ ����Ʈ
	vector<pair<eTextType, int>> splitTextList;

	// ** �ؽ�Ʈ�� ũ��
	int textSize;

	// ** ��ȿ�� �ؽ�Ʈ���� ( ��밡���� ���ڷθ� �̷���� �ִ��� )
	bool isValidText;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	virtual Bridge* Clone() override { return new TextUI(*this); }

public:
	bool SetText(string _text, int _textSize);

public:
	TextUI();
	virtual ~TextUI();
};

