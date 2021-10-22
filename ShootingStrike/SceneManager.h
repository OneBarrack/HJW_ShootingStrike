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
	eSCENEID GetCurrentSceneID();
	void SetScene(eSCENEID _sceneID);
	
	void Update();
	void Render(HDC _hdc);
	void Release();

private:
	void SceneStart(eSCENEID _sceneID);

private:
	SceneManager() : pSceneState(nullptr) {}
public:
	~SceneManager() { Release(); }
};

