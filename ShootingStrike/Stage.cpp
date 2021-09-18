#include "Stage.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "EnemyHole.h"
#include "HammerEffect.h"
#include "ObjectFactory.h"
#include "CollisionManager.h"
#include "StageBackground.h"
#include "BitmapManager.h"

Stage::Stage() : m_pPlayer(nullptr)
{

}

Stage::~Stage()
{
	Release();
}

void Stage::Initialize()
{
	m_pPlayer = ObjectManager::GetInstance()->GetPlayer();

	// ** ������Ʈ �Ŵ������� �Ѿ� ����Ʈ�� �޾ƿ�. (�����ͷ�...)
	BulletList = ObjectManager::GetInstance()->GetBulletList();

	// ** ������Ʈ �Ŵ������� ���� ����Ʈ�� �޾ƿ�. (�����ͷ�...)
	EnemyList = ObjectManager::GetInstance()->GetEnemyList();
	
	Bridge* pBridge = new StageBackground;
	Background = ObjectManager::GetInstance()->TakeObject(eObjectKey::BACKGROUND, pBridge);


	m_pEffect = new HammerEffect;
	m_pEffect->Initialize();

	TileHeightCnt = 4;
	TileWidthCnt = 4;
	/*
	// ** �� ����
	for (int i = 0; i < 8; ++i)
	{
		Object* pObj = new Enemy;
		pObj->Initialize();

		Vector3 RandomPos = Vector3(
			float(rand() % (WindowsWidth - 120) + 60),
			float(rand() % (WindowsHeight - 120) + 60));

		pObj->SetPosition(RandomPos.x, RandomPos.y);
		pObj->SetColliderPosition(RandomPos.x, RandomPos.y);

		EnemyList->push_back(pObj);
	}
	*/

	Vector3 Center = Vector3(WindowsWidth / 2.0f, WindowsHeight / 2.0f);

	//for (int y = 0; y < TileHeightCnt; ++y)
	//{
	//	for (int x = 0; x < TileWidthCnt; ++x)
	//	{
	//		Object* pObj = new EnemyHole;
	//		pObj->Initialize();

	//		pObj->SetPosition(
	//			(Center.x - ((TileWidthCnt / 2) * pObj->GetScale().x )) + pObj->GetScale().x * x,
	//			(Center.y - ((TileHeightCnt / 2) * pObj->GetScale().y)) + pObj->GetScale().y * y);

	//		EnemyList->push_back(pObj);
	//	}
	//}


	/////	
	m_pEffect = ObjectManager::GetInstance()->TakeObject(eObjectKey::HAMMEREFFECT);

	//for ( int y = 0; y < TileHeightCnt; ++y )
	//{
	//	for ( int x = 0; x < TileWidthCnt; ++x )
	//	{
	//		Object* pObj = ObjectManager::GetInstance()->TakeObject(eObjectKey::ENEMYHOLE);
	//		pObj->Initialize();

	//		pObj->SetPosition(
	//			(Center.x - ((TileWidthCnt / 2) * pObj->GetScale().x)) + pObj->GetScale().x * x,
	//			(Center.y - ((TileHeightCnt / 2) * pObj->GetScale().y)) + pObj->GetScale().y * y);

	//		EnemyList->push_back(pObj);
	//	}
	//}
}

void Stage::Update()
{
	if (((Player*)m_pPlayer)->GetSwing())
	{
		m_pEffect->Initialize();
		m_pEffect->SetStatus(eObjectStatus::ACTIVATED);
	}

	// ** ��� Ȱ��ȭ ������Ʈ �� �浹 �˻�
	CheckCollisionForAllObjects();
	UpdateForAllObjects();
}

void Stage::Render(HDC _hdc)
{
	RenderForAllObjects(_hdc);
}

void Stage::Release()
{

}

void Stage::CheckCollisionForAllObjects()
{
	auto EnableList = ObjectManager::GetInstance()->GetEnableList();
	for ( auto ListIter1 = EnableList->begin(); ListIter1 != EnableList->end(); ++ListIter1 )
	{
		for ( auto ObjIter1 = ListIter1->second.begin(); ObjIter1 != ListIter1->second.end(); ++ObjIter1 )
		{
			if ( (*ObjIter1)->GetCollisionType() == eCollisionType::NONE )
				continue;

			// ** ��ü Object�� �浹üũ
			auto ListIter2 = ListIter1;
			for ( ++ListIter2; ListIter2 != EnableList->end(); ++ListIter2 )
			{
				for ( auto ObjIter2 = ListIter2->second.begin(); ObjIter2 != ListIter2->second.end(); ++ObjIter2 )
				{
					if ( (*ObjIter2)->GetCollisionType() == eCollisionType::NONE )
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

void Stage::UpdateForAllObjects()
{
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
					(*ObjIter1)->Update();
					[[fallthrough]];
				default:
					++ObjIter1;
					break;
			}
		}
	}

	static ULONGLONG Time = GetTickCount64();
	if ( Time + 1000 < GetTickCount64() )
	{
		auto temp = ObjectManager::GetInstance()->GetEnableList();
		auto temp2 = temp->find(eObjectKey::BULLET);
		if ( temp2 != temp->end() )
		{
			cout << temp2->second.size() << endl;
		}
		Time = GetTickCount64();
	}
}

void Stage::RenderForAllObjects(HDC _hdc)
{
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
				(*iter2)->Render(BitmapManager::GetInstance()->GetMemDC(eImageKey::BUFFER));
			}
		}
	}

	BitBlt(_hdc,
		0, 0,
		WindowsWidth,
		WindowsHeight,
		BitmapManager::GetInstance()->GetMemDC(eImageKey::BUFFER),
		0, 0,
		SRCCOPY);
}
