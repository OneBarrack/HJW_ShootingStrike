#include "SceneManager.h"
#include "Logo.h"
#include "Menu.h"
#include "Stage.h"

SceneManager* SceneManager::pInstance = nullptr;


eSCENEID SceneManager::GetCurrentSceneID()
{
	return curSceneId;
}

void SceneManager::SceneStart(eSCENEID _sceneID)
{
	::Safe_Delete(pSceneState);	

	curSceneId = nextSceneId;
	switch ( curSceneId )
	{
		case eSCENEID::LOGO:
			pSceneState = new Logo;
			break;

		case eSCENEID::MENU:
			pSceneState = new Menu;
			break;

		case eSCENEID::STAGE:
			pSceneState = new Stage;
			break;

		case eSCENEID::EXIT:
			exit(NULL);
			break;
	}

	pSceneState->Initialize();
}

void SceneManager::SetScene(eSCENEID _sceneID)
{
	nextSceneId = _sceneID;
}

void SceneManager::Update()
{
	// ** ���� SceneId�� NONE�� �ƴϸ�, Scene ������ �Ͼ ���̹Ƿ� 
	// ** ���� Scene�� nextSceneId�� Scene���� �ʱ�ȭ �� nextSceneId�� NONE���� �ʱ�ȭ
	if ( nextSceneId != eSCENEID::NONE )
	{
		SceneStart(nextSceneId);
		nextSceneId = eSCENEID::NONE;
	}

	pSceneState->Update();	
}

void SceneManager::Render(HDC _hdc)
{
	pSceneState->Render(_hdc);
}

void SceneManager::Release()
{
	::Safe_Delete(pSceneState);
}