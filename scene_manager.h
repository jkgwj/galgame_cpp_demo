#pragma once
#include"scene.h"

extern bool quit;
extern GameScene* game_scene;
extern MenuScene* menu_scene;
extern SetScene* set_scene;

//场景管理器
class SceneManager
{

private:
	Scene* now_scene = nullptr;
	SceneState now_state;
	SDL_Event _Event;

	

	void clear_event() {
		SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
	}

public:
	SceneManager(SDL_Event event) { _Event = event; }
	~SceneManager() = default;

	void set_scene_state(Scene* scene);//场景状态设置
	void jmp_scene(SceneState state);//场景跳转

	void Updata(int &delta);//更新数据
	void Render(Camera& camera);//绘制
	void Input(SDL_Event &Event);//消息处理

	void reset_game_scene();

};
void SceneManager::set_scene_state(Scene* scene) {
	now_scene = scene;
	now_state = scene->next_scene_state;
	scene->on_enter();
}
void SceneManager::jmp_scene(SceneState state) {
	SceneState T_state;
	UI_State T_UI;
	T_UI = now_scene->now_UI_state;
	now_scene->next_scene_state = now_state;
	now_scene->now_UI_state = UI_State::nop;
	now_scene->on_exit();
	switch (state)
	{
	case SceneState::menu:
		T_state = now_scene->before_scene_state;
		menu_scene->mouse_pos = now_scene->mouse_pos;
		now_scene = menu_scene;
		now_scene->before_scene_state = T_state;
		now_scene->reset = 1;
		break;
	case SceneState::game:
		T_state = now_scene->before_scene_state;
		game_scene->mouse_pos = now_scene->mouse_pos;
		now_scene = game_scene;
		now_scene->before_scene_state = T_state;
		now_scene->reset = 1;
		break;
	case SceneState::set:
		T_state = now_scene->before_scene_state;
		set_scene->mouse_pos = now_scene->mouse_pos;
		now_scene = set_scene;
		now_scene->before_scene_state = T_state;
		now_scene->reset = 1;
		break;
	default:
		break;
	}
	now_scene->on_enter();
	now_state = now_scene->next_scene_state;
	now_scene->now_UI_state = T_UI;
	clear_event();
}
void SceneManager::Updata(int& delta) {
	
	if (now_scene->next_scene_state != now_state) {
		jmp_scene(now_scene->next_scene_state);
		now_scene->is_next = 0;
		now_scene->scene_reset = 1;
		clear_event();
	}
	if (now_scene->scene_reset) { now_scene->_scene_reset(); now_scene->scene_reset = 0; }
	
	now_scene->on_updata(delta);

	
	if (now_scene->next_scene_state != now_state) {
		jmp_scene(now_scene->next_scene_state);
		now_scene->is_next = 0;
		now_scene->scene_reset = 1;
		clear_event();
	}
	
	if (now_scene->scene_reset) { now_scene->_scene_reset(); now_scene->scene_reset = 0; }
}
void SceneManager::Input(SDL_Event& Event) {

	now_scene->on_input(Event);

}
void SceneManager::Render(Camera& camera) {
	now_scene->on_render(camera);
}
void SceneManager::reset_game_scene() {
	game_scene->_scene_reset();
}