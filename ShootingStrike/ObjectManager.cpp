#include "ObjectManager.h"
#include "MathManager.h"
#include "CollisionManager.h"
#include "ObjectFactory.h"
#include "Prototype.h"
#include "Enemy.h"
#include "Bullet.h"

ObjectManager* ObjectManager::pInstance = nullptr;

void ObjectManager::Initialize()
{
	Release();
	
	pPrototypeObject = new Prototype;
	pPrototypeObject->CreatePrototype();
	pPlayer = nullptr;
}

void ObjectManager::Update()
{
	// ** ��� Ȱ��ȭ ������Ʈ �� �浹 �˻�
	CheckCollision();

	// ** ��� ������Ʈ�� Status�� üũ�Ͽ� Update �Ǵ� Recall ó��
	auto enableList = ObjectManager::GetInstance()->GetEnableObjectList();
	for ( auto listIter = enableList->begin(); listIter != enableList->end(); ++listIter )
	{
		for ( auto objIter = listIter->second.begin(); objIter != listIter->second.end(); )
		{
			// ** Status�� üũ�Ͽ� Update �Ǵ� Recall ���ش�
			switch ( (*objIter)->GetStatus() )
			{
				case eObjectStatus::DESTROYED:
					RecallObject(*objIter++);
					break;
				case eObjectStatus::ACTIVATED:
					(*objIter)->Update(); // ** Update
					[[fallthrough]];
				default:
					++objIter;
					break;
			}
		}
	}
}

void ObjectManager::Render(HDC _hdc)
{
	// ** ��� Object Rendering
	// ** Buffer MemDC�� ��� �׸� �� memDC�� hdc�� ���ҽ��� �׸���
	map<eObjectKey, list<Object*>>* enableList = ObjectManager::GetInstance()->GetEnableObjectList();
	for ( map<eObjectKey, list<Object*>>::iterator listIter = enableList->begin();
		listIter != enableList->end(); ++listIter )
	{
		for ( list<Object*>::iterator objIter = listIter->second.begin();
			objIter != listIter->second.end(); ++objIter )
		{
			if ( (*objIter)->GetStatus() == eObjectStatus::ACTIVATED )
			{
				(*objIter)->Render(_hdc);
			}
		}
	}
}

Object* ObjectManager::CreateObject(eObjectKey _key)
{
	// ** ���ο� ��ü�� �������־�� �Ѵ�. ������ ���� ��ü�� ��������ϴ� ������� ������ ���̴�.
	// ** �׷����� ���� ������ü�� �����ϴ��� ã�´�.
	Object* pProtoObject = pPrototypeObject->FindPrototypeObject(_key);

	// ** ������ü�� ���ٸ�....
	if ( pProtoObject == nullptr )
		return nullptr;

	// ** ���� ��ü�� ã�Ҵٸ� ������ü�� ���� �����Ѵ�.
	Object* pObject = pProtoObject->Clone();
	pObject->Initialize();

	return pObject;
}

Object* ObjectManager::CreateObject(eObjectKey _key, Vector3 _position)
{
	// ** ���ο� ��ü�� �������־�� �Ѵ�. ������ ���� ��ü�� ��������ϴ� ������� ������ ���̴�.
	// ** �׷����� ���� ������ü�� �����ϴ��� ã�´�.
	Object* pProtoObject = pPrototypeObject->FindPrototypeObject(_key);

	// ** ������ü�� ���ٸ�....
	if ( pProtoObject == nullptr )
		return nullptr;
		
	// ** ���� ��ü�� ã�Ҵٸ� ������ü�� ���� �����Ѵ�.
	Object* pObject = pProtoObject->Clone();
	pObject->Initialize();
	pObject->SetPosition(_position);

	return pObject;
}

Bridge* ObjectManager::CreateBridge(eBridgeKey _key)
{
	// ** ���ο� ��ü�� �������־�� �Ѵ�. ������ ���� ��ü�� ��������ϴ� ������� ������ ���̴�.
	// ** �׷����� ���� ������ü�� �����ϴ��� ã�´�.
	Bridge* pProtoBridge = pPrototypeObject->FindPrototypeBridge(_key);

	// ** ������ü�� ���ٸ�....
	if ( pProtoBridge == nullptr )
		return nullptr;
	
	// ** ���� ��ü�� ã�Ҵٸ� ������ü�� ���� �����Ѵ�.
	Bridge* pBridge = pProtoBridge->Clone();
	pBridge->Initialize();

	return pBridge;
}

void ObjectManager::AddObject(map<eObjectKey, list<Object*>>& _targetList, Object* _pObject)
{
	// ** Ű������ Ž���� Ž���� �Ϸ�� ������� ��ȯ.
	map<eObjectKey, list<Object*>>::iterator listIter = _targetList.find(_pObject->GetKey());

	// ** ���� ������� �������� �ʴ´ٸ�....
	if ( listIter == _targetList.end() )
	{
		// ** ���ο� ����Ʈ�� ����.
		list<Object*> tempList;

		tempList.push_back(_pObject);

		// ** ������Ʈ�� �߰��� ����Ʈ�� �ʿ� ����.
		_targetList.insert(make_pair(_pObject->GetKey(), tempList));
	}
	// ** ������� ���� �Ѵٸ�...
	else
	{
		// ** �ش� ����Ʈ�� ������Ʈ�� �߰�
		listIter->second.push_back(_pObject);
	}
}

void ObjectManager::AddBridge(map<eBridgeKey, list<Bridge*>>& _targetList, Bridge* _pBridge)
{
	// ** Ű������ Ž���� Ž���� �Ϸ�� ������� ��ȯ.
	map<eBridgeKey, list<Bridge*>>::iterator listIter = _targetList.find(_pBridge->GetKey());

	// ** ���� ������� �������� �ʴ´ٸ�....
	if ( listIter == _targetList.end() )
	{
		// ** ���ο� ����Ʈ�� ����.
		list<Bridge*> tempList;

		tempList.push_back(_pBridge);

		// ** ������Ʈ�� �߰��� ����Ʈ�� �ʿ� ����.
		_targetList.insert(make_pair(_pBridge->GetKey(), tempList));
	}
	// ** ������� ���� �Ѵٸ�...
	else
	{
		// ** �ش� ����Ʈ�� ������Ʈ�� �߰�
		listIter->second.push_back(_pBridge);
	}
}

void ObjectManager::CheckCollision()
{
	/** ĳ���Ϳ� �Ѿ�, �����۰� �浹üũ **/
	// Bullet - Character �浹üũ
	list<Object*> enableBulletList = enableObjectList[eObjectKey::BULLET];
	for ( auto bulltetIter = enableBulletList.begin(); bulltetIter != enableBulletList.end(); ++bulltetIter )
	{
		Object* pBulletObject = *bulltetIter;
		if ( pBulletObject->IsGeneratedCollisionEvent() == false )
			continue;

		switch ( static_cast<Bullet*>(pBulletObject)->GetOwner()->GetKey() )
		{
			case eObjectKey::PLAYER:
			{
				list<Object*> enableEnemyList = enableObjectList[eObjectKey::ENEMY];
				for ( auto EnemyIter = enableEnemyList.begin(); EnemyIter != enableEnemyList.end(); ++EnemyIter )
				{
					Object* pEnemyObject = *EnemyIter;
					if ( CollisionManager::IsCollision(pEnemyObject, pBulletObject) )
					{
						// �浹 Ʈ���� �ߵ�
						pEnemyObject->OnCollision(pBulletObject);
						pBulletObject->OnCollision(pEnemyObject);
					}
				}				
				break;
			}
			case eObjectKey::ENEMY:
			{
				if ( CollisionManager::IsCollision(pPlayer, pBulletObject) )
				{
					// �浹 Ʈ���� �ߵ�
					pPlayer->OnCollision(pBulletObject);
					pBulletObject->OnCollision(pPlayer);
				}
				break;
			}
		}	
	}

	// Item - Character �浹üũ
	list<Object*> enableItemList = enableObjectList[eObjectKey::ITEM];
	for ( auto itemIter = enableItemList.begin(); itemIter != enableItemList.end(); ++itemIter )
	{
		Object* pItemObject = *itemIter;
		if ( pItemObject->IsGeneratedCollisionEvent() == false )
			continue;

		list<Object*> enableEnemyList = enableObjectList[eObjectKey::ENEMY];
		for ( auto EnemyIter = enableEnemyList.begin(); EnemyIter != enableEnemyList.end(); ++EnemyIter )
		{
			Object* pEnemyObject = *EnemyIter;
			if ( CollisionManager::IsCollision(pEnemyObject, pItemObject) )
			{
				// �浹 Ʈ���� �ߵ�
				pEnemyObject->OnCollision(pItemObject);
				pItemObject->OnCollision(pEnemyObject);
			}
		}
		if ( CollisionManager::IsCollision(pPlayer, pItemObject) )
		{
			// �浹 Ʈ���� �ߵ�
			pPlayer->OnCollision(pItemObject);
			pItemObject->OnCollision(pPlayer);
		}
	}

	// Character(Player - Enemy) �浹üũ
	list<Object*> enableEnemyList = enableObjectList[eObjectKey::ENEMY];
	for ( auto enemyIter = enableEnemyList.begin(); enemyIter != enableEnemyList.end(); ++enemyIter )
	{
		Object* pEnemyObject = *enemyIter;
		if ( pEnemyObject->IsGeneratedCollisionEvent() == false )
			continue;

		if ( CollisionManager::IsCollision(pPlayer, pEnemyObject) )
		{
			// �浹 Ʈ���� �ߵ�
			pPlayer->OnCollision(pEnemyObject);
			pEnemyObject->OnCollision(pPlayer);
		}
	}
}

// ** �����̳ʿ��� ��ü�� ã�Ƽ� ��ȯ. ���ٸ� Prototype ���� �� ��ȯ
Object* ObjectManager::NewObject(eObjectKey _key)
{
	Object* pObject = nullptr;

	// ** DisableList�� �����Ϸ��� ������Ʈ�� �ִ��� Ȯ��.
	map<eObjectKey, list<Object*>>::iterator iter = disableObjectList.find(_key);

	// ** ������..
	if ( iter == disableObjectList.end() || iter->second.empty() )
	{
		// ** Object�� ���� �����Ͽ� EnableList�� �߰�
		pObject = CreateObject(_key);
		if ( pObject != nullptr )
		{
			AddObject(enableObjectList, pObject);
		}
	}
	// ** ������..
	else
	{
		// ** DisableList�� �� Object�� ����.
		pObject = iter->second.front();
		pObject->Initialize();		

		// ** ������ Object�� EnableList�� �߰� �� DisableList���� ����
		AddObject(enableObjectList, pObject);
		iter->second.pop_front();
	}

	return pObject;
}

// ** �����̳ʿ��� ��ü�� ã�Ƽ� ��ȯ. ���ٸ� Prototype ���� �� ��ȯ
Object* ObjectManager::NewObject(eObjectKey _key, Vector3 _position)
{
	Object* pObject = nullptr;

	// ** DisableList�� �����Ϸ��� ������Ʈ�� �ִ��� Ȯ��.
	map<eObjectKey, list<Object*>>::iterator iter = disableObjectList.find(_key);

	// ** ������.....
	if (iter == disableObjectList.end() || iter->second.empty())
	{
		// ** Object�� ���� �����Ͽ� EnableList�� �߰�
		pObject = CreateObject(_key, _position);
		if ( pObject != nullptr )
		{
			AddObject(enableObjectList, pObject);
		}
	}
	// ** ������..
	else
	{
		// ** DisableList�� �� Object�� ����.
		pObject = iter->second.front();
		pObject->Initialize();
		pObject->SetPosition(_position);

		// ** ������ Object�� EnableList�� �߰� �� DisableList���� ����
		AddObject(enableObjectList, pObject);
		iter->second.pop_front();
	}

	return pObject;
}

Bridge* ObjectManager::NewBridge(eBridgeKey _key)
{
	Bridge* pBridge = nullptr;

	// ** DisableList�� �����Ϸ��� ������Ʈ�� �ִ��� Ȯ��.
	map<eBridgeKey, list<Bridge*>>::iterator iter = disableBridgeList.find(_key);

	// ** ������..
	if ( iter == disableBridgeList.end() || iter->second.empty() )
	{
		// ** Bridge�� ���� �����Ͽ� EnableList�� �߰�
		pBridge = CreateBridge(_key);
		if ( pBridge != nullptr )
		{
			AddBridge(enableBridgeList, pBridge);
		}
	}
	// ** ������..
	else
	{
		// ** DisableList�� �� Bridge�� ����.
		pBridge = iter->second.front();
		pBridge->Initialize();

		// ** ������ Bridge�� EnableList�� �߰� �� DisableList���� ����
		AddBridge(enableBridgeList, pBridge);
		iter->second.pop_front();
	}

	return pBridge;
}

void ObjectManager::RecallObject(Object* _pObject)
{
	// ** �ش� Object Release
	_pObject->Release();
	
	// ** Ű������ Enable ObjectList�� Ž���Ͽ� ����Ʈ �� �ش� Object ���� ����.
	list<Object*>& objectList = enableObjectList.find(_pObject->GetKey())->second;
	objectList.erase(find(objectList.begin(), objectList.end(), _pObject));
	
	// ** �ش� Object�� DisableList�� �߰�.
	AddObject(disableObjectList, _pObject);
}

void ObjectManager::RecallBridge(Bridge* _pBridge)
{
	// ** �ش� Bridge Release
	_pBridge->Release();

	// ** Ű������ Enable BridgeList�� Ž���Ͽ� ����Ʈ �� �ش� Bridge ���� ����.
	list<Bridge*>& bridgeList = enableBridgeList.find(_pBridge->GetKey())->second;
	bridgeList.erase(find(bridgeList.begin(), bridgeList.end(), _pBridge));

	// ** �ش� Bridge�� DisableList�� �߰�.
	AddBridge(disableBridgeList, _pBridge);
}

list<Object*> ObjectManager::GetObjectList(eObjectKey _objectKey)
{
	auto FindIter = enableObjectList.find(_objectKey);
	if ( FindIter != enableObjectList.end() )
		return FindIter->second;

	return list<Object*>();
}

list<Bridge*> ObjectManager::GetBridgeList(eBridgeKey _bridgeKey)
{
	auto FindIter = enableBridgeList.find(_bridgeKey);
	if ( FindIter != enableBridgeList.end() )
		return FindIter->second;

	return list<Bridge*>();
}

Object* ObjectManager::FindObjectWithTag(eTagName _tagName)
{
	for ( map<eObjectKey, list<Object*>>::iterator listIter = enableObjectList.begin();
		listIter != enableObjectList.end(); ++listIter )
	{
		for ( list<Object*>::iterator objIter = listIter->second.begin();
			objIter != listIter->second.end(); ++objIter )
		{
			if ( (*objIter)->GetTagName() == _tagName )
				return (*objIter);
		}
	}
	return nullptr;
}

Object* ObjectManager::FindObjectWithTag(eObjectKey _objectKey, eTagName _tagName)
{
	map<eObjectKey, list<Object*>>::iterator findIter = enableObjectList.find(_objectKey);
	if ( findIter != enableObjectList.end() )
	{
		list<Object*> objectList = findIter->second;
		for ( Object* pObject : objectList )
		{
			if ( pObject->GetTagName() == _tagName )
				return pObject;
		}
	}

	return nullptr;
}

void ObjectManager::Release()
{
	// ** Delete PrototypeObject
	Safe_Delete(pPrototypeObject);

	// ** Recall Player
	if ( pPlayer )
		RecallObject(pPlayer);	
	
	/** 
	* Object / Bridge EnableList�� ���� Recall ó��.
	* 
	* ������ ���� ���� �Ǿ��ٸ� Player Recall�� ���������� ��� ������Ʈ/�긴���� ȸ���Ǿ� �־�� �Ѵ�.
	* ��, EnableList�� �����Ͱ� �����ִٸ� ��𼱰� Recall ������ �ʾҰų� ������ ������ ��.
	* Ȥ���� �Ϻ� Object �Ǵ� Bridge�� ȸ������ ������ �ø� ����� ����ó���̴�.
	*/

	// ** Recall Enable Object
	for ( map<eObjectKey, list<Object*>>::iterator listIter = enableObjectList.begin();
		listIter != enableObjectList.end(); ++listIter )
	{
		for ( list<Object*>::iterator objIter = listIter->second.begin();
			objIter != listIter->second.end(); )
		{
			RecallObject((*objIter++));
		}
	}
	enableObjectList.clear();
	
	// ** Recall Enable Bridge
	for ( map<eBridgeKey, list<Bridge*>>::iterator listIter = enableBridgeList.begin();
		listIter != enableBridgeList.end(); ++listIter )
	{
		for ( list<Bridge*>::iterator bridgeIter = listIter->second.begin();
			bridgeIter != listIter->second.end(); )
		{
			RecallBridge((*bridgeIter++));
		}
	}
	enableBridgeList.clear();

	// ** Delete Object
	for (map<eObjectKey, list<Object*>>::iterator listIter = disableObjectList.begin();
		listIter != disableObjectList.end(); ++listIter)
	{
		for (list<Object*>::iterator objIter = listIter->second.begin();
			objIter != listIter->second.end(); ++objIter)
		{
			::Safe_Delete((*objIter));
		}
		listIter->second.clear();
	}
	disableObjectList.clear();	

	// ** Delete Bridge		
	for ( map<eBridgeKey, list<Bridge*>>::iterator listIter = disableBridgeList.begin();
		listIter != disableBridgeList.end(); ++listIter )
	{
		for ( list<Bridge*>::iterator bridgeIter = listIter->second.begin();
			bridgeIter != listIter->second.end(); ++bridgeIter )
		{
			::Safe_Delete((*bridgeIter));
		}
		listIter->second.clear();
	}
	disableBridgeList.clear();	
}
