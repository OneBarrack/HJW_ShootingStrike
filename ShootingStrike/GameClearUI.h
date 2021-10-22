#pragma once
#include "UIBridge.h"

class GameClearUI : public UIBridge
{
public:
	typedef UIBridge Super;

private:
	Bitmap* pGameClearImage;
	Bitmap* pFadeBackImage;
	Object* pPlayTimeNameTextUI;
	Object* pPlayTimeTextUI;
	Object* pScoreNameTextUI;
	Object* pScoreUI;
	Object* pPressEnterTextUI;

	// ** Object���� �ʱ�ȭ �Ǿ�����
	bool bInitializedObjects;

	// ** AlphaBlend�� ����� ������
	BYTE alpha;
	int fadeSpeed;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	virtual Bridge* Clone() override { return new GameClearUI(*this); }

private:
	void InitTextObjects();

	string ConvertTimeToStrTimeFormat(ULONGLONG _time);	

public:
	GameClearUI();
	virtual ~GameClearUI();
};

