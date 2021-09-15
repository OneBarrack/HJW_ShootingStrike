#include "Stage.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "EnemyHole.h"
#include "HammerEffect.h"
#include "ObjectFactory.h"
#include "CollisionManager.h"
#include "Background.h"
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
	
	State_Back = new Background;
	State_Back->Initialize();

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

	for (int y = 0; y < TileHeightCnt; ++y)
	{
		for (int x = 0; x < TileWidthCnt; ++x)
		{
			Object* pObj = new EnemyHole;
			pObj->Initialize();

			pObj->SetPosition(
				(Center.x - ((TileWidthCnt / 2) * pObj->GetScale().x )) + pObj->GetScale().x * x,
				(Center.y - ((TileHeightCnt / 2) * pObj->GetScale().y)) + pObj->GetScale().y * y);

			EnemyList->push_back(pObj);
		}
	}


	/////
	ObjectManager::GetInstance()->TakeObject(eObjectKey::BACKGROUND);
	m_pEffect = ObjectManager::GetInstance()->TakeObject(eObjectKey::HAMMEREFFECT);

	for ( int y = 0; y < TileHeightCnt; ++y )
	{
		for ( int x = 0; x < TileWidthCnt; ++x )
		{
			Object* pObj = ObjectManager::GetInstance()->TakeObject(eObjectKey::ENEMYHOLE);
			pObj->Initialize();

			pObj->SetPosition(
				(Center.x - ((TileWidthCnt / 2) * pObj->GetScale().x)) + pObj->GetScale().x * x,
				(Center.y - ((TileHeightCnt / 2) * pObj->GetScale().y)) + pObj->GetScale().y * y);

			EnemyList->push_back(pObj);
		}
	}
}

void Stage::Update()
{
	if (((Player*)m_pPlayer)->GetSwing())
	{
		m_pEffect->SetStatus(eObjectStatus::ACTIVATED);
		m_pEffect->Initialize();
	}

	// ** EnableList �� Object ��ȸ
	auto enableList = ObjectManager::GetInstance()->GetEnableList();
	for ( auto ListIter1 = enableList->begin(); ListIter1 != enableList->end(); ++ListIter1 )
	{
		for ( auto ObjIter1 = ListIter1->second.begin(); ObjIter1 != ListIter1->second.end(); )
		{
			// ** ��ü Object�� �浹üũ
			auto ListIter2 = ListIter1;
			for ( ++ListIter2; ListIter2 != enableList->end(); ++ListIter2 )
			{
				for ( auto ObjIter2 = ListIter2->second.begin(); ObjIter2 != ListIter2->second.end(); ++ObjIter2 )
				{
					// ��ü �浹�� ���� ó�� �ʿ�
					if ( CollisionManager::IsCollision(*ObjIter1, *ObjIter2) )
					{
						// �浹 Ʈ���� �ߵ�
						(*ObjIter1)->OnCollision(*ObjIter2);
						(*ObjIter2)->OnCollision(*ObjIter1);
					}
				}
			}
		
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
}

void Stage::Render(HDC _hdc)
{
	//State_Back->Render(BitmapManager::GetInstance()->GetMemDC(eImageKey::BUFFER));

	//
	//for (vector<Object*>::iterator iter = EnemyList->begin();
	//	iter != EnemyList->end(); ++iter)
	//	(*iter)->Render(BitmapManager::GetInstance()->GetMemDC(eImageKey::BUFFER));
	//

	//for (vector<Object*>::iterator iter = BulletList->begin();
	//	iter != BulletList->end(); ++iter)
	//	(*iter)->Render(BitmapManager::GetInstance()->GetMemDC(eImageKey::BUFFER));


	//if (m_pEffect->GetActive())
	//	m_pEffect->Render(BitmapManager::GetInstance()->GetMemDC(eImageKey::BUFFER));


	//m_pPlayer->Render(BitmapManager::GetInstance()->GetMemDC(eImageKey::BUFFER));

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

void Stage::Release()
{

}
