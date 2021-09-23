#pragma once
#include "Headers.h"

class Object;
class Prototype;
class Bridge;
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
	vector<Object*> EnemyList;
	vector<Object*> BulletList;
	map<eObjectKey, list<Object*>> EnableList;
	map<eObjectKey, list<Object*>> DisableList;

public:
	// ** �ʱ�ȭ
	void Initialize();	

	// ** �����̳ʿ��� ��ü�� ã�Ƽ� ��ȯ. ���ٸ� Prototype ���� �� ��ȯ
	Object* TakeObject(eObjectKey _Key, Bridge* _pBridge = nullptr);
	Object* TakeObject(eObjectKey _Key, Vector3 _Position, Bridge* _pBridge = nullptr);
	
	// ** ����� ���̻� ������� �ʴ� ������Ʈ ȸ��
	void RecallObject(Object* _pObject);

	// ** ObjectKey�� �ش��ϴ� Object List�� ��ȯ
	list<Object*> GetObjectList(eObjectKey _ObjectKey);

	void Release();

private:
	// ** ��ü ����
	Object* CreateObject(eObjectKey _Key);
	Object* CreateObject(eObjectKey _Key, Vector3 _Position);

	// ** ��ü �߰�.
	void AddObject(map<eObjectKey, list<Object*>>& _TargetList, Object* _pObject);
public:
	// ** �÷��̾ ��ȯ.
	Object* GetPlayer() { return pPlayer; }
	void SetPlayer(Object* _pPlayer) { pPlayer = _pPlayer; }

	// ** map �����̳ʸ� ����ϰ��ִ� ObjectList�� ��ȯ.
	map<eObjectKey, list<Object*>>* GetEnableList() { return &EnableList; }
	map<eObjectKey, list<Object*>>* GetDisableList() { return &DisableList; }
		
	vector<Object*>* GetBulletList() { return &BulletList; }
	vector<Object*>* GetEnemyList() { return &EnemyList; }
private:
	ObjectManager() : pPlayer(nullptr) { }
public:
	~ObjectManager() { Release(); }
};

