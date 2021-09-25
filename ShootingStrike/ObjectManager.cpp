#include "ObjectManager.h"
#include "MathManager.h"
#include "CollisionManager.h"
#include "ObjectFactory.h"
#include "Prototype.h"
#include "Enemy.h"
#include "Bullet.h"

ObjectManager* ObjectManager::Instance = nullptr;

void ObjectManager::Initialize()
{
	PrototypeObject = new Prototype;
	PrototypeObject->CreatePrototype();
}

void ObjectManager::Update()
{
	// ** ��� Ȱ��ȭ ������Ʈ �� �浹 �˻�
	CheckCollision();

	// ** ��� ������Ʈ�� Status�� üũ�Ͽ� Update �Ǵ� Recall ó��
	auto enableList = ObjectManager::GetInstance()->GetEnableList();
	for ( auto ListIter1 = enableList->begin(); ListIter1 != enableList->end(); ++ListIter1 )
	{
		for ( auto ObjIter1 = ListIter1->second.begin(); ObjIter1 != ListIter1->second.end(); )
		{
			// ** Status�� üũ�Ͽ� Update �Ǵ� Recall ���ش�
			switch ( (*ObjIter1)->GetStatus() )
			{
				case eObjectStatus::DESTROYED:
					ObjectManager::GetInstance()->RecallObject(*ObjIter1++);
					break;
				case eObjectStatus::ACTIVATED:
					(*ObjIter1)->Update(); // ** Update
					[[fallthrough]];
				default:
					++ObjIter1;
					break;
			}
		}
	}
}

void ObjectManager::Render(HDC _hdc)
{
	// ** ��� Object Rendering
	// ** Buffer MemDC�� ��� �׸� �� memDC�� hdc�� ���ҽ��� �׸���
	map<eObjectKey, list<Object*>>* enableList = ObjectManager::GetInstance()->GetEnableList();
	for ( map<eObjectKey, list<Object*>>::iterator iter = enableList->begin();
		iter != enableList->end(); ++iter )
	{
		for ( list<Object*>::iterator iter2 = iter->second.begin();
			iter2 != iter->second.end(); ++iter2 )
		{
			if ( (*iter2)->GetStatus() == eObjectStatus::ACTIVATED )
			{
				(*iter2)->Render(_hdc);
			}
		}
	}
}

Object* ObjectManager::CreateObject(eObjectKey _Key)
{
	// ** ���ο� ��ü�� �������־�� �Ѵ�. ������ ���� ��ü�� ��������ϴ� ������� ������ ���̴�.
	// ** �׷����� ���� ������ü�� �����ϴ��� ã�´�.
	Object* pProtoObject = PrototypeObject->FindPrototypeObject(_Key);

	// ** ������ü�� ���ٸ�....
	if ( pProtoObject == nullptr )
		return nullptr;
	// ** ���� ��ü�� �ִٸ�...
	else
	{
		// ** ������ü�� ���� �����Ѵ�.
		Object* pObject = pProtoObject->Clone();
		pObject->Initialize();

		return pObject;
	}
}

Object* ObjectManager::CreateObject(eObjectKey _Key, Vector3 _Position)
{
	// ** ���ο� ��ü�� �������־�� �Ѵ�. ������ ���� ��ü�� ��������ϴ� ������� ������ ���̴�.
	// ** �׷����� ���� ������ü�� �����ϴ��� ã�´�.
	Object* pProtoObject = PrototypeObject->FindPrototypeObject(_Key);

	// ** ������ü�� ���ٸ�....
	if ( pProtoObject == nullptr )
		return nullptr;
	// ** ���� ��ü�� �ִٸ�...
	else
	{
		// ** ������ü�� ���� �����Ѵ�.
		Object* pObject = pProtoObject->Clone();
		pObject->Initialize();
		pObject->SetPosition(_Position);

		return pObject;
	}
}

void ObjectManager::AddObject(map<eObjectKey, list<Object*>>& _TargetList, Object* _pObject)
{
	// ** Ű������ Ž���� Ž���� �Ϸ�� ������� ��ȯ.
	map<eObjectKey, list<Object*>>::iterator ListIter = _TargetList.find(_pObject->GetKey());

	// ** ���� ������� �������� �ʴ´ٸ�....
	if ( ListIter == _TargetList.end() )
	{
		// ** ���ο� ����Ʈ�� ����.
		list<Object*> TempList;

		TempList.push_back(_pObject);

		// ** ������Ʈ�� �߰��� ����Ʈ�� �ʿ� ����.
		_TargetList.insert(make_pair(_pObject->GetKey(), TempList));
	}
	// ** ������� ���� �Ѵٸ�...
	else
	{
		// ** �ش� ����Ʈ�� ������Ʈ�� �߰�
		ListIter->second.push_back(_pObject);
	}
}

void ObjectManager::CheckCollision()
{
	auto EnableList = ObjectManager::GetInstance()->GetEnableList();
	for ( auto ListIter1 = EnableList->begin(); ListIter1 != EnableList->end(); ++ListIter1 )
	{
		for ( auto ObjIter1 = ListIter1->second.begin(); ObjIter1 != ListIter1->second.end(); ++ObjIter1 )
		{
			if ( (*ObjIter1)->IsGeneratedCollisionEvent() == false )
				continue;

			// ** ��ü Object�� �浹üũ
			auto ListIter2 = ListIter1;
			for ( ++ListIter2; ListIter2 != EnableList->end(); ++ListIter2 )
			{
				for ( auto ObjIter2 = ListIter2->second.begin(); ObjIter2 != ListIter2->second.end(); ++ObjIter2 )
				{
					if ( (*ObjIter2)->IsGeneratedCollisionEvent() == false )
						continue;

					if ( CollisionManager::IsCollision(*ObjIter1, *ObjIter2) )
					{
						// �浹 Ʈ���� �ߵ�
						(*ObjIter1)->OnCollision(*ObjIter2);
						(*ObjIter2)->OnCollision(*ObjIter1);
					}
				}
			}
		}
	}
}

// ** �����̳ʿ��� ��ü�� ã�Ƽ� ��ȯ. ���ٸ� Prototype ���� �� ��ȯ
Object* ObjectManager::TakeObject(eObjectKey _Key, Bridge* _pBridge)
{
	Object* pObject = nullptr;

	// ** DisableList�� �����Ϸ��� ������Ʈ�� �ִ��� Ȯ��.
	map<eObjectKey, list<Object*>>::iterator iter = DisableList.find(_Key);

	// ** ������..
	if ( iter == DisableList.end() || iter->second.empty() )
	{
		// ** Object�� ���� �����Ͽ� EnableList�� �߰�
		pObject = CreateObject(_Key);
		if ( pObject != nullptr )
		{
			AddObject(EnableList, pObject);
		}
	}
	// ** ������..
	else
	{
		// ** DisableList�� �� Object�� ����.
		pObject = iter->second.front();
		pObject->Initialize();		

		// ** ������ Object�� EnableList�� �߰� �� DisableList���� ����
		AddObject(EnableList, pObject);
		iter->second.pop_front();
	}
	
	// ** Bridge�� �����Ѵٸ� �������ش�
	if ( _pBridge )
	{
		_pBridge->SetOwner(pObject);
		(pObject)->SetBridgeObject(_pBridge);
	}

	return pObject;
}

// ** �����̳ʿ��� ��ü�� ã�Ƽ� ��ȯ. ���ٸ� Prototype ���� �� ��ȯ
Object* ObjectManager::TakeObject(eObjectKey _Key, Vector3 _Position, Bridge* _pBridge)
{
	Object* pObject = nullptr;

	// ** DisableList�� �����Ϸ��� ������Ʈ�� �ִ��� Ȯ��.
	map<eObjectKey, list<Object*>>::iterator iter = DisableList.find(_Key);

	// ** ������.....
	if (iter == DisableList.end() || iter->second.empty())
	{
		// ** Object�� ���� �����Ͽ� EnableList�� �߰�
		pObject = CreateObject(_Key, _Position);
		if ( pObject != nullptr )
		{
			AddObject(EnableList, pObject);
		}
	}
	// ** ������..
	else
	{
		// ** DisableList�� �� Object�� ����.
		pObject = iter->second.front();
		pObject->Initialize();
		pObject->SetPosition(_Position);

		// ** ������ Object�� EnableList�� �߰� �� DisableList���� ����
		AddObject(EnableList, pObject);
		iter->second.pop_front();
	}

	// ** Bridge�� �����Ѵٸ� �������ش�
	if ( _pBridge )
	{
		_pBridge->SetOwner(pObject);
		(pObject)->SetBridgeObject(_pBridge);
	}

	return pObject;
}

void ObjectManager::RecallObject(Object* _pObject)
{
	// ** �ش� Object Release
	_pObject->Release();
	
	// ** Ű������ Enable ObjectList�� Ž���Ͽ� ����Ʈ �� �ش� Object ���� ����.
	list<Object*>& ObjectList = EnableList.find(_pObject->GetKey())->second;
	ObjectList.erase(find(ObjectList.begin(), ObjectList.end(), _pObject));
	
	// ** �ش� Object�� DisableList�� �߰�.
	AddObject(DisableList, _pObject);	
}

list<Object*> ObjectManager::GetObjectList(eObjectKey _ObjectKey)
{
	list<Object*> ResultList;

	auto FindIter = EnableList.find(_ObjectKey);
	if ( FindIter != EnableList.end() )
		ResultList = FindIter->second;

	return ResultList;
}

void ObjectManager::Release()
{
	// ** ������ ����.
	::Safe_Delete(pPlayer);

	for (map<eObjectKey, list<Object*>>::iterator iter = DisableList.begin();
		iter != DisableList.end(); ++iter)
	{
		for (list<Object*>::iterator iter2 = iter->second.begin();
			iter2 != iter->second.end(); ++iter2)
		{
			::Safe_Delete((*iter2));
		}
		iter->second.clear();
	}
	DisableList.clear();

	for ( map<eObjectKey, list<Object*>>::iterator iter = EnableList.begin();
		iter != EnableList.end(); ++iter )
	{
		for ( list<Object*>::iterator iter2 = iter->second.begin();
			iter2 != iter->second.end(); ++iter2 )
		{
			::Safe_Delete((*iter2));
		}
	}
	EnableList.clear();
}
