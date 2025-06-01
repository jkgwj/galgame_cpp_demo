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
	enum KyaraState {//在这里表示渲染的三种状态
		
		IDLE,//闲置
		HOVERED,//悬停
		PHSHED//按下
	};
	KaracterName name;//角色名字
	KyaraState render_state;//渲染状态
	Kyara_ACT now_ACT;//角色行为
	SDL_FRect* rect_dst;//目标位置

public:
	virtual void on_enter() {}
	virtual void on_init(DataLoadMode mode,KyaracterName name){}
	virtual void on_updata(Vector2D &pos,int _delta, Kyara_ACT ACT) {}
	virtual void on_render() {}
	virtual void on_delete() {}
	//virtual std::vector<KyaraNode*>& get_kyara_data();
};

