#pragma once
#include "Headers.h"

class SoundManager
{
private:
	static SoundManager* pInstance;
public:
	static SoundManager* GetInstance()
	{
		if ( pInstance == nullptr )
			pInstance = new SoundManager;

		return pInstance;
	}

private:
	FMOD::System* m_pSystem;
	map<eSoundKey, SoundInfo*> soundList;

	float volume;

public:
	void Initialize();	
	void Update();
	void Release();

	void Play(eSoundKey _key);
	void Pause(eSoundKey _key);
	void Resume(eSoundKey _key);
	void Stop(eSoundKey _key);
	void VolumeUp();
	void VolumeDown();

	bool IsPlaying(eSoundKey _key);

private:
	SoundInfo* GetSoundInfo(eSoundKey _key);
	void LoadSoundData(eSoundKey _key, const char* _fileName, bool _bLoop = false);
	void SetVolume();

private:
	SoundManager();
public:
	~SoundManager();
};

/*
������Ʈ - �Ӽ� - VC++ ���͸�
 - ���� ���͸�(inluce) - ���� - FMOD���� - api���� - core -  inc����
 - ���̺귯�� ���͸�(library) - api���� - core - lib - x86����

include.h -> #pragma comment(lib, "fmod_vc.lib")

lib x86 ���� �� fmod.dll ���� - ������Ʈ ��ο� �ٷ� �ٿ��ֱ�

*/
