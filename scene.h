#pragma once
#include"scene_state.h"
#include"game_data_manager.h"
#include"game_data_audio.h"
#include"Camera.h"
#include"button_txt.h"
#include<SDL.h>


extern SDL_FRect* FRect_dst;

//场景基类
class Scene
{
private:
	
public:
	GameData_audio * game_audio;

	Scene() {};
	~Scene() {};

    bool is_next=0;
	bool reset = 0;
	bool scene_reset = 0;

	Vector2D mouse_pos = {};
	SceneState next_scene_state;
	SceneState before_scene_state;
	UI_State now_UI_state = UI_State::nop;


	virtual void on_enter() {}//场景初始化
	virtual void on_updata(int delta) {}//更新数据
	virtual void on_render(Camera& camera) {}//绘制
	virtual void on_input(SDL_Event &Event) {}//消息处理
	virtual void on_exit() {}//退出逻辑

	virtual void _scene_reset(){}
};