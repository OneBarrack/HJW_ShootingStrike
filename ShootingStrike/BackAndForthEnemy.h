#pragma once
#include "EnemyBridge.h"

class BackAndForthEnemy : public EnemyBridge
{
	enum class eBackAndForthDirection
	{
		LEFT,
		RIGHT
	};

public:
	typedef EnemyBridge Super;

private:
	ULONGLONG time;

	// ** ������ �߰� ��ġ�� �����ߴ���
	int angle;

	// ** ���� ������ ���� ����
	Vector3 perpendicularDirection;
	bool bFindPerpendicularDirection;

	// ** ���� ���� ����
	eBackAndForthDirection startDirection;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	virtual Bridge* Clone() override { return new BackAndForthEnemy(*this); }

public:
	void LeftSpin() { startDirection = eBackAndForthDirection::LEFT; }
	void RightSpin() { startDirection = eBackAndForthDirection::RIGHT; }

public:
	BackAndForthEnemy();
	virtual ~BackAndForthEnemy();
};

