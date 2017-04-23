#include "Rat.h"


Rat::Rat()
{
	next = 0;

	keyframe = 0;
}


Rat::~Rat()
{
	if (next)
	{
		delete next;
	}
}

void Rat::SetScore(GLubyte score){
	this->score = score;
}

void Rat::setMove(Vector3 move){
	this->move = move;
}

void Rat::setMaxPosition(Vector3 maxPosition){
	this->maxPosition = maxPosition;
}

void Rat::setMinPosition(Vector3 minPosition){
	this->minPosition = minPosition;
}

void Rat::AddRat(Rat *&list, Rat *rat){
	Rat *p;
	if (list == 0){
		list = rat;
		return;
	}
	else{
		p = list;
		while (p->next != 0){
			p = p->next;
		}
		p->next = rat;
	}

}

Rat *Rat::AddRat(Rat *&list, GLbyte score, Vector3 position, Vector3 scale){
	Rat *p, *rat;
	rat = new Rat;
	rat->SetShader(RESOURCES->GetShaderByID("Basic"));
	rat->SetNumOfModels(1);
	rat->SetModel(RESOURCES->GetModelByID("Rat"), 0);
	rat->SetPosition(position);
	rat->SetScore(score);
	rat->SetScale(scale);
	rat->SetBox();
	if (list == 0){
		list = rat;
		return list;
	}
	else{
		p = list;
		while (p->next != 0){
			p = p->next;
		}
		p->next = rat;
		return p->next;
	}
}

void Rat::RemoveRat(Rat *&list, Rat *rat){
	Rat *p;
	if (list != 0){
		p = list;
		if (list == rat){

			list = p->next;
			p->next = 0;
			delete p;
		}
		else{
			while (p->next != rat){
				p = p->next;
			}
			p->next = rat->next;
			rat->next = 0;
			delete rat;
		}
	}
}

Rat *Rat::IsCollision(Egg *egg){
	Rat *p = this;
	while (p != 0){
		if(egg->IsCollision(p))
		{
			return p;
		}
		p = p->next;
	}
	return 0;
}

void Rat::Update(GLfloat deltaTime){
	position.x += deltaTime*move.x;
	position.y += deltaTime*move.y;
	position.z += deltaTime*move.z;
	
	/*srand(time(NULL));
	int t = rand();
	if (t % 10 == 0) {
		move.x = -move.x;
		printf("\n%d--------------",t);
	}*/

	if (position.x <= minPosition.x) {
		move.x = abs(move.x);
	}
	if (position.x >= maxPosition.x) {
		move.x = -abs(move.x);
	}
	rotate.y = move.x >= 0 ? 0.0f : 3.14f;
	if (position.y <= minPosition.y) move.y = abs(move.y);
	if (position.y >= maxPosition.y) move.y = -abs(move.y);
	if (position.z <= minPosition.z) move.z = abs(move.z);
	if (position.z >= maxPosition.z) move.z = -abs(move.z);
	
	keyframe += Timer::FrameTime();
	if (keyframe > 1.0f){
		usingModel = 0;
	}
	else if (keyframe >0.5f){
		usingModel = 1;
	}
	else{
		usingModel = 2;
	}
	if (keyframe >= 1.0f){
		keyframe -= 1.0f;
	}

	SetBox();
}

void Rat::UpdateAll(float deltatime){
	Rat *p = this;
	while (p != 0){
		p->Update(deltatime);
		p = p->next;
	}
}

void Rat::DrawAll(Matrix world){
	Rat *p = this;
	while (p != 0){
		p->Draw(world);
		p = p->next;
	}
}

void Rat::ClearRat(Rat *&list){
	if (list){
		delete list;
	}
}

GLubyte Rat::GetScore(){
	return score;
}