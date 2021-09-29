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
	// ** Prototype
	Prototype* PrototypeObject;

	// ** �÷��̾�
	Object* pPlayer;

	// ** ������Ʈ ����Ʈ
	map<eObjectKey, list<Object*>> EnableObjectList;
	map<eObjectKey, list<Object*>> DisableObjectList;

	// ** �긴�� ����Ʈ
	map<eBridgeKey, list<Bridge*>> EnableBridgeList;
	map<eBridgeKey, list<Bridge*>> DisableBridgeList;

public:
	// ** �ʱ�ȭ
	void Initialize();	
	void Update();
	void Render(HDC _hdc);
	void Release();

	// ** �÷��̾ ��ȯ.
	Object* GetPlayer() { return pPlayer; }
	void SetPlayer(Object* _pPlayer) { pPlayer = _pPlayer; }

	// ** �����̳ʿ��� ������Ʈ ��ü�� ã�Ƽ� ��ȯ. ���ٸ� Prototype ���� �� ��ȯ
	Object* TakeObject(eObjectKey _Key);
	Object* TakeObject(eObjectKey _Key, Vector3 _Position);
	
	// ** �����̳ʿ��� �긴�� ��ü�� ã�Ƽ� ��ȯ. ���ٸ� Prototype ���� �� ��ȯ
	Bridge* TakeBridge(eBridgeKey _Key);

	// ** ����� ���̻� ������� �ʴ� ������Ʈ ȸ��
	void RecallObject(Object* _pObject);

	// ** ����� ���̻� ������� �ʴ� �긴�� ȸ��
	void RecallBridge(Bridge* _pBridge);

	// ** map �����̳ʸ� ����ϰ��ִ� Object Enable/Disable List�� ��ȯ.
	map<eObjectKey, list<Object*>>* GetEnableObjectList() { return &EnableObjectList; }
	map<eObjectKey, list<Object*>>* GetDisableObjectList() { return &DisableObjectList; }

	// ** map �����̳ʸ� ����ϰ��ִ� Bridge Enable/Disable List�� ��ȯ.
	map<eBridgeKey, list<Bridge*>>* GetEnableBridgeList() { return &EnableBridgeList; }
	map<eBridgeKey, list<Bridge*>>* GetDisableBridgeList() { return &DisableBridgeList; }

	// ** ObjectKey�� �ش��ϴ� Object List�� ��ȯ
	list<Object*> GetObjectList(eObjectKey _ObjectKey);	

	// ** BridgeKey�� �ش��ϴ� Bridge List�� ��ȯ
	list<Bridge*> GetBridgeList(eBridgeKey _BridgeKey);

private:
	// ** ������Ʈ ��ü ����
	Object* CreateObject(eObjectKey _Key);
	Object* CreateObject(eObjectKey _Key, Vector3 _Position);

	// ** �긴�� ��ü ����
	Bridge* CreateBridge(eBridgeKey _Key);

	// ** ������Ʈ ��ü �߰�
	void AddObject(map<eObjectKey, list<Object*>>& _TargetList, Object* _pObject);

	// ** �긴�� ��ü �߰�
	void AddBridge(map<eBridgeKey, list<Bridge*>>& _TargetList, Bridge* _pObject);

	// ** ��� Ȱ��ȭ ������Ʈ �� �浹 �˻�
	void CheckCollision();

private:
	ObjectManager() : pPlayer(nullptr) { }
public:
	~ObjectManager() { Release(); }
};

