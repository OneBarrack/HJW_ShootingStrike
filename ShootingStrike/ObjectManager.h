#pragma once
#include "Headers.h"

class Object;
class Prototype;
class ObjectManager
{
private:
	static ObjectManager* Instance;
public:
	static ObjectManager* GetInstance()
	{
		if (Instance == nullptr)
			Instance = new ObjectManager;

		return Instance;
	}
private:
	Prototype* PrototypeObject;

	// ** �÷��̾�
	Object* pPlayer;

	// ** ������Ʈ ����Ʈ
	list<Object*> EnableList;
	vector<Object*> EnemyList;
	vector<Object*> BulletList;
	map<eObjectKey, list<Object*>> DisableList;
public:
	// ** �ʱ�ȭ
	void Initialize();

	// ** ��ü ����
	Object* CreateObject(eObjectKey _Key);

	// ** �����̳ʿ��� ��ü�� ã��
	void FindObject(eObjectKey _Key);

	// ** ��ü ����
	Object* CreateObject(eObjectKey _Key, Vector3 _Position);

	// ** �����̳ʿ��� ��ü�� ã��
	void FindObject(eObjectKey _Key, Vector3 _Position);

	// ** ��ü �߰�.
	void AddObject(eObjectKey _strKey);

	// ** ����� ���̻� ������� �ʴ� ������Ʈ ȸ��
	void RecallObject(Object* _Object);

	// ** ���� Ÿ���� ��ġ�� ���´�.
	Object* GetTarget(Vector3 _Pos);


	void Release();
public:
	// ** �÷��̾ ��ȯ.
	Object* GetPlayer() { return pPlayer; }
	void SetPlayer(Object* _pPlayer) { pPlayer = _pPlayer; }


	// ** map �����̳ʸ� ����ϰ��ִ� ObjectList�� ��ȯ.
	list<Object*>* GetEnableList() { return &EnableList; }
	map<eObjectKey, list<Object*>>* GetDisableList() { return &DisableList; }

	
	vector<Object*>* GetBulletList() { return &BulletList; }
	vector<Object*>* GetEnemyList() { return &EnemyList; }

	void AddBullet(Vector3 _vPos);



	// ** map �����̳ʸ� ����ϰ��ִ� ObjectList�� ���Ե� list �� Ž���Ͽ� ��ȯ.
	list<Object*>* FindList(eObjectKey _Key)
	{
		map<eObjectKey, list<Object*>>::iterator iter = DisableList.find(_Key);

		// ** �������� �ʴ� key������ nullptr�� ��ȯ.
		if (iter == DisableList.end())
			return nullptr;

		return &iter->second;
	}
private:
	ObjectManager() : pPlayer(nullptr) { }
public:
	~ObjectManager() { Release(); }
};

