#pragma once
#include "Headers.h"

class Scene;
class SceneManager
{
private:
	static SceneManager* pInstance;
public:
	static SceneManager* GetInstance()
	{
		if (pInstance == nullptr)
			pInstance = new SceneManager;

		return pInstance;
	}
private:
	// ** ���� Scene ����
	Scene* pSceneState;
	eSCENEID curSceneId;

	// ** ���� Scene ����
	eSCENEID nextSceneId;

public:
	eSCENEID GetCurrentSceneID() { return curSceneId; }
	Scene* GetScene() { return pSceneState; }
	void SetScene(eSCENEID _sceneID);
	
	void Update();
	void Render(HDC _hdc);
	void Release();

private:
	void SceneStart(eSCENEID _sceneID);

private:
	SceneManager();
public:
	~SceneManager();
};

