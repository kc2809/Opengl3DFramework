#pragma once

#define SOUND SoundManager::GetInstance()

enum SOUNDINDEX {
	S_COLLISION = 1,
	S_THROWEGG = 2,
	S_BACKGROUND =3,
	S_CLICK =4,
	S_EGGCRUSH =5
};

class SoundManager
{
private:
	void(*playSound)(int, int);
	void(*playMusic)(int, int);
	void(*stopSound)(void );
	void (*stopMusic)(void );
	static SoundManager* instance;
	SoundManager();
	~SoundManager();
public:
	static SoundManager *GetInstance();
	static void DestroyInstance();

	void RegisterPlaySoundtFunc(void(*)(int, int));
	void RegisterPlayMusicFunc(void(*)(int, int));
	void RegisterStopSoundFunc(void(*)(void));
	void RegisterStopMusicFunc(void(*)(void));

	void PlaySound(int, int);
	void PlayMusic(int, int);
	void StopSound();
	void StopMusic();



};


