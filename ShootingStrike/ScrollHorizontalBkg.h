#pragma once
#include "BackgroundBridge.h"

class ScrollHorizontalBkg : public BackgroundBridge
{
	enum class eScrollDirection
	{
		LEFT,
		RIGHT,
	};

private:
	float ImageOffset;
	float LoopOffset;

	// ** ��ũ�� ����
	eScrollDirection ScrollDirection;

	// ** ��ũ���� ���� �ݺ��� �� ����
	bool bLoop;

	// ** ���ۺκа� ���κ��� ���ÿ� �׸��� �۾��� ����������(�̹����� �� �κ�����)
	bool bDrawEachStartEnd;

public:
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Render(HDC _hdc)override;
	virtual void Release()override;

	virtual Bridge* Clone()override { return new ScrollHorizontalBkg(*this); }

public:
	void SetLoop(bool _bLoop) { bLoop = _bLoop; }

	void StartLeft();
	void StartRight();
	void ScrollLeft();
	void ScrollRight();


public:
	ScrollHorizontalBkg();
	virtual ~ScrollHorizontalBkg();
};

