#pragma once
#include"data_tool.h"
#include"Vector2D.h"
#include"Timer.h"
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
class GameData_kyara
{
public:
	enum KyaraState {//�������ʾ��Ⱦ������״̬
		
		IDLE,//����
		HOVERED,//��ͣ
		PHSHED//����
	};
	KaracterName name;//��ɫ����
	KyaraState render_state;//��Ⱦ״̬
	Kyara_ACT now_ACT;//��ɫ��Ϊ
	SDL_FRect* rect_dst;//Ŀ��λ��

public:
	virtual void on_enter() {}
	virtual void on_init(DataLoadMode mode,KyaracterName name){}
	virtual void on_updata(Vector2D &pos,int _delta, Kyara_ACT ACT) {}
	virtual void on_render() {}
	virtual void on_delete() {}
	//virtual std::vector<KyaraNode*>& get_kyara_data();
};

