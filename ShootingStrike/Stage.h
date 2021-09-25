#pragma once
#include "Scene.h"


class Object;
class Stage : public Scene
{
private:
	Object* pBackground;	
	Object* pPlayer;

public:
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Render(HDC _hdc)override;
	virtual void Release()override;

private:
	// ** ������Ʈ�� Stage ���θ� ������� üũ
	void CheckPositionInBkgBoundary(eObjectKey _ObjectKey);

public:
	Stage();
	virtual ~Stage();
};