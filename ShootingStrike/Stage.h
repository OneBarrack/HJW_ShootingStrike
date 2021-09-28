#pragma once
#include "Scene.h"


class Object;
class Stage : public Scene
{
private:
	Object* pPlayer;

	Object* pBackground;
	Object* pLeftSideBackground;
	Object* pRightSideBackground;

	Object* pEnemyBoss;

public:
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Render(HDC _hdc)override;
	virtual void Release()override;

private:
	// ** ������Ʈ���� ���� Background�� ������� üũ
	void CheckPositionInBkgBoundary(eObjectKey _ObjectKey);

public:
	Stage();
	virtual ~Stage();
};