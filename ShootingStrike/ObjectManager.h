#pragma once
#include "Headers.h"

class Object;
class Prototype;
class Bridge;
class ObjectManager
{
private:
	static ObjectManager* pInstance;
public:
	static ObjectManager* GetInstance()
	{
		if (pInstance == nullptr)
			pInstance = new ObjectManager;

		return pInstance;
	}
private:
	// ** Prototype
	Prototype* pPrototypeObject;

	// ** �÷��̾�
	Object* pPlayer;

	// ** ������Ʈ ����Ʈ
	map<eObjectKey, list<Object*>> enableObjectList;
	map<eObjectKey, list<Object*>> disableObjectList;

	// ** �긴�� ����Ʈ
	map<eBridgeKey, list<Bridge*>> enableBridgeList;
	map<eBridgeKey, list<Bridge*>> disableBridgeList;

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
	Object* NewObject(eObjectKey _key);
	Object* NewObject(eObjectKey _key, Vector3 _position);
	
	// ** �����̳ʿ��� �긴�� ��ü�� ã�Ƽ� ��ȯ. ���ٸ� Prototype ���� �� ��ȯ
	Bridge* NewBridge(eBridgeKey _key);

	// ** ����� ���̻� ������� �ʴ� ������Ʈ ȸ��
	void RecallObject(Object* _pObject);

	// ** ����� ���̻� ������� �ʴ� �긴�� ȸ��
	void RecallBridge(Bridge* _pBridge);

	// ** map �����̳ʸ� ����ϰ��ִ� Object Enable/Disable List�� ��ȯ.
	map<eObjectKey, list<Object*>>* GetEnableObjectList() { return &enableObjectList; }
	map<eObjectKey, list<Object*>>* GetDisableObjectList() { return &disableObjectList; }

	// ** map �����̳ʸ� ����ϰ��ִ� Bridge Enable/Disable List�� ��ȯ.
	map<eBridgeKey, list<Bridge*>>* GetEnableBridgeList() { return &enableBridgeList; }
	map<eBridgeKey, list<Bridge*>>* GetDisableBridgeList() { return &disableBridgeList; }

	// ** ObjectKey�� �ش��ϴ� Object List�� ��ȯ
	list<Object*> GetObjectList(eObjectKey _objectKey);

	// ** BridgeKey�� �ش��ϴ� Bridge List�� ��ȯ
	list<Bridge*> GetBridgeList(eBridgeKey _bridgeKey);	

	// ** TagName�� �ش��ϴ� Object�� ��ȯ
	Object* FindObjectWithTag(eTagName _tagName);
	Object* FindObjectWithTag(eObjectKey _objectKey, eTagName _tagName);

private:
	// ** ������Ʈ ��ü ����
	Object* CreateObject(eObjectKey _key);
	Object* CreateObject(eObjectKey _key, Vector3 _position);

	// ** �긴�� ��ü ����
	Bridge* CreateBridge(eBridgeKey _key);

	// ** ������Ʈ ��ü �߰�
	void AddObject(map<eObjectKey, list<Object*>>& _targetList, Object* _pObject);

	// ** �긴�� ��ü �߰�
	void AddBridge(map<eBridgeKey, list<Bridge*>>& _targetList, Bridge* _pObject);

	// ** ��� Ȱ��ȭ ������Ʈ �� �浹 �˻�
	void CheckCollision();

private:
	ObjectManager() : pPrototypeObject(nullptr), pPlayer(nullptr) { }
public:
	~ObjectManager() { Release(); }
};

