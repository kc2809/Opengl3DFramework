#include "SoundManager.h"

SoundManager *SoundManager::instance = 0;

SoundManager::SoundManager()
{
}
SoundManager::~SoundManager()
{

}

SoundManager *SoundManager::GetInstance()
{
	if (instance == 0){
		instance = new SoundManager();
	}
	return instance;
}

void SoundManager::DestroyInstance()
{

	if (instance != 0)
	{
		delete instance;
		instance = 0;
	}
}

void SoundManager::RegisterPlaySoundtFunc(void(*playsoundFunc)(int, int)) {
	this->playSound = playsoundFunc;
}

void SoundManager::RegisterPlayMusicFunc(void(*playmusicFunc)(int, int)) {
	this->playMusic = playmusicFunc;
}

void SoundManager::RegisterStopSoundFunc(void(*stopSoundFunc)(void)){
	this->stopSound = stopSoundFunc;
}

void SoundManager::RegisterStopMusicFunc(void(*stopMusicFunc)(void)) {
	this->stopMusic = stopMusicFunc;
}

void SoundManager::PlaySound(int soundid, int loop){
	playSound(soundid, loop);
}
void SoundManager::PlayMusic(int soundid, int loop){
	playMusic(soundid, loop);
}
void SoundManager::StopSound(){

}
void SoundManager::StopMusic(){
	stopMusic();
}