/*
	Key press:
	W S: Move forward, Move backward.
	A D: Move left, Move right.
	Q E: Move up, Move down.
	
	J L: rotate left, rotate right.
	K I: rotate up, rotate down.
*/
#if WIN32

#include "SceneManager.h"
#include <stdio.h>


int width = 700;
int height = 400;

void Init()
{
	srand(time(0));
	RESOURCES->Load("../Resources/Managers/RM.txt");
	SceneManager::GetInstance()->Init("../Resources/Managers/SM.txt");
}

void Update()
{
	SceneManager::GetInstance()->Update();
}

void Draw()
{
	SceneManager::GetInstance()->Draw();
}

void Destroy()
{
	InputManager::DestroyInstance();
	ResourceManager::DestroyInstance();
	SceneManager::DestroyInstance();
	Text::FreeInstance();
	SoundManager::DestroyInstance();
}

void Key(int keyCode, bool isKeyDown)
{
	printf("\nKey code: %d - is key down: %d", keyCode, isKeyDown);
	if (isKeyDown)
	{
		INPUT->KeyPressed(keyCode);
	}
	else
	{
		INPUT->KeyReleased(keyCode);
	}
}


void Mouse(int type, int x, int y)
{
	printf("\nMouse type: %d - (x = %d, y = %d)", type, x, y);
	INPUT->SetMouseClick(type, x, y);
}


void PlaySound(int, int){}
void PlayMusic(int, int){}
void StopSound(){}
void StopMusic(){}

int main()
{
	SOUND->RegisterPlaySoundtFunc(PlaySound);
	SOUND->RegisterPlayMusicFunc(PlayMusic);
	SOUND->RegisterStopSoundFunc(StopSound);
	SOUND->RegisterStopMusicFunc(StopMusic);
	InitWindow(width, height);
	INPUT->SetSize(width, height);
	RegisterInitFunc(Init);
	RegisterDrawFunc(Draw);
	RegisterUpdateFunc(Update);
	RegisterDestroyFunc(Destroy);
	RegisterKeyFunc(Key);
	RegisterMouseFunc(Mouse);

	WinLoop();
	return 0;
}

#endif