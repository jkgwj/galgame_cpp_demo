#pragma once
#include"scene_state.h"
#include"game_data_manager.h"
#include"game_data_audio.h"
#include"Camera.h"
#include"button_txt.h"
#include<SDL.h>


extern SDL_FRect* FRect_dst;

//��������
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


	virtual void on_enter() {}//������ʼ��
	virtual void on_updata(int delta) {}//��������
	virtual void on_render(Camera& camera) {}//����
	virtual void on_input(SDL_Event &Event) {}//��Ϣ����
	virtual void on_exit() {}//�˳��߼�

	virtual void _scene_reset(){}
};