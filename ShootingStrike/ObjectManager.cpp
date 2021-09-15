#include "ObjectManager.h"
#include "MathManager.h"
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

Object* ObjectManager::CreateObject(eObjectKey _Key)
{
	// ** ���ο� ��ü�� �������־�� �Ѵ�. ������ ���� ��ü�� ��������ϴ� ������� ������ ���̴�.
	// ** �׷����� ���� ������ü�� �����ϴ��� ã�´�.
	Object* pProtoObject = PrototypeObject->FindPrototypeObject(_Key);

	// ** ������ü�� ���ٸ�....
	if (pProtoObject == nullptr)
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
	if (pProtoObject == nullptr)
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

// ** �����̳ʿ��� ��ü�� ã�Ƽ� ��ȯ. ���ٸ� Prototype ���� �� ��ȯ
Object* ObjectManager::TakeObject(eObjectKey _Key)
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

	return pObject;
}

// ** �����̳ʿ��� ��ü�� ã�Ƽ� ��ȯ. ���ٸ� Prototype ���� �� ��ȯ
Object* ObjectManager::TakeObject(eObjectKey _Key, Vector3 _Position)
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

	return pObject;
}

void ObjectManager::RecallObject(Object* _pObject)
{
	// ** Ű������ Enable ObjectList�� Ž���Ͽ� ����Ʈ �� �ش� Object ���� ����.
	list<Object*>& ObjectList = EnableList.find(_pObject->GetKey())->second;
	ObjectList.erase(find(ObjectList.begin(), ObjectList.end(), _pObject));

	// ** �ش� Object�� DisableList�� �߰�.
	AddObject(DisableList, _pObject);	
}

// _Debug_ : map���� ã�����ְ�
Object* ObjectManager::GetTarget(Vector3 _Pos)
{
	// ** ��Ƽ���� �����. Key = �Ÿ�, value = Object
	multimap<float, Object*> FindTargetList;


	// ** ��� �� ���ָ���Ʈ�� ���鼭 Ȯ���Ѵ�.
	for (vector<Object*>::iterator iter = EnemyList.begin();
		iter != EnemyList.end(); ++iter)
	{
		// ** ��Ƽ�ʿ� Current �� Target �� �Ÿ��� ���ؼ� �߰��Ѵ�.
		FindTargetList.insert(
			make_pair(
			MathManager::GetDistance(_Pos, (*iter)->GetPosition()),	// ** Key
			(*iter)));	// ** Value
	}

	// ** ���࿡ ����Ʈ�� �ƹ��͵� ���ٸ�....
	if (FindTargetList.empty())
		return nullptr;

	// ** ��� ������Ʈ�� �߰��۾��� ������ ���� ù��°�� �ִ� ������Ʈ�� ��ȯ�Ѵ�.
	return FindTargetList.begin()->second;
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

void ObjectManager::AddBullet(Vector3 _vPos)
{
	BulletList.push_back(
		ObjectFactory<Bullet>::CreateObject(_vPos));
}
