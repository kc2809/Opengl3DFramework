#pragma once
#include "SoundManager.h"
#include "ResourceManager.h"
#include "Object3D.h"
#include "Button.h"
#include "Text.h"
#include "Camera.h"

enum SCENE_STATE{
	NONE = -1,
	HOME = 0,
	PLAYGAME = 1,
	HIGHSCORE = 2,
	HELP = 3,
	QUIT = 4,
	LOGO = 5
};

enum SOUND_ID{
	S_NONE = -1,
	S_SCORE = 0,
	S_GAMETHROW = 1,
	S_GAMEWIN = 2,
	S_GAMELOST =3,
	S_GAMESTART = 4
};

class Scene
{
protected:
	Object3D *object3Ds;
	Object2D *object2Ds;
	Button *buttons;
	GLuint numOfButtons, numOfObject2Ds, numOfObject3Ds;

	Camera *camera2D, *camera3D;
	GLint soundId;
public:
	Scene();
	virtual ~Scene();

	virtual void Init(char *sceneFile);
	virtual void Draw();
	virtual void Update();

	GLint GetSoundId() const;
	void SetSoundId(GLint soundID);
};

