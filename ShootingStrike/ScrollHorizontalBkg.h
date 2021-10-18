#pragma once
#include "BackgroundBridge.h"

class ScrollHorizontalBkg : public BackgroundBridge
{
public:
	typedef BackgroundBridge Super;

private:
	enum class eScrollDirection
	{
		LEFT,
		RIGHT,
	};

private:
	//** �Ϲ� Scroll �� ���� Offset
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

	virtual Bridge* Clone()override { return new ScrollHorizontalBkg(*this); }

public:
	void SetLoop(int _loopCount = 0) { maxLoopCount = _loopCount; }

	void StartLeft();
	void StartRight();
	void ScrollLeft();
	void ScrollRight();


public:
	ScrollHorizontalBkg();
	virtual ~ScrollHorizontalBkg();
};

