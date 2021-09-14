#include "Prototype.h"

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"


Prototype::Prototype()
{

}

Prototype::~Prototype()
{

}


void Prototype::CreatePrototype()
{
	Transform TransInfo;

	PrototypeList[eObjectKey::PLAYER] = new Player(TransInfo);
	PrototypeList[eObjectKey::ENEMY]  = new Enemy(TransInfo);
	PrototypeList[eObjectKey::BULLET] = new Bullet(TransInfo);
}

Object* Prototype::FindPrototypeObject(eObjectKey _Key)
{
	map<eObjectKey, Object*>::iterator iter = PrototypeList.find(_Key);

	if (iter == PrototypeList.end())
	{
		//ERROR_MESSAGE("���� ������ ��ü ������ �����ϴ�.", _Key);
		
		Sleep(500);
		return nullptr;
	}

	return iter->second;
}
