#pragma once
#include "BackgroundBridge.h"

class ScrollVerticalBkg : public BackgroundBridge
{
public:
	typedef BackgroundBridge Super;

private:
	enum class eScrollDirection
	{
		UP,
		DOWN,
	};

private:
	// ** �Ϲ� Scroll �� ���� Offset
	float imageOffset;

	// ** �̹����� ���� �����Ͽ� Loop�Ǵ� ������ ���� Offset
	float imageOffsetForRestart;

	// ** ��ũ�� ����
	eScrollDirection scrollDirection;	

	// ** ��ũ�� �ݺ� Ƚ��
	int maxLoopCount;
	int curLoopCount;

	// ** ���ۺκа� ���κ��� ���ÿ� �׸��� �۾��� ����������(�̹����� �� �κ�����)
	bool bDrawEachStartEnd;
	
	// ** ���� �̵��Ÿ�
	float curMoveDist;

public:
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Render(HDC _hdc)override;
	virtual void Release()override;

	virtual Bridge* Clone()override { return new ScrollVerticalBkg(*this); }

public:
	void SetLoop(int _loopCount = 0) { maxLoopCount = _loopCount; }

	void StartTop();
	void StartBottom();
	void ScrollUp();
	void ScrollDown();


public:
	ScrollVerticalBkg();
	virtual ~ScrollVerticalBkg();
};

