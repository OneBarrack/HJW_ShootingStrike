#pragma once
#include "Scene.h"


class Object;
class Stage : public Scene
{
private:
	Object* pBackground;	
	Object* pPlayer;
	
	/*
	Object* m_pEffect;
	vector<Object*>* EnemyList;
	vector<Object*>* BulletList;

	// ** Ÿ�� ���� ����
	int TileWidthCnt;
	int TileHeightCnt;
	*/

public:
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Render(HDC _hdc)override;
	virtual void Release()override;

private:
	// ** ��� Ȱ��ȭ ������Ʈ �� �浹 �˻�
	void CheckCollisionForAllObjects();

	// ** ������Ʈ�� Stage ���θ� ������� üũ
	void CheckPositionInsideStage(Object* _pObject);

	// ** ��� ������Ʈ�� Status�� üũ�Ͽ� Update �Ǵ� Recall ó��
	void UpdateForAllObjects();

	void RenderForAllObjects(HDC _hdc);

public:
	Stage();
	virtual ~Stage();
};