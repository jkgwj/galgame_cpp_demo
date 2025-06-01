#pragma once
#include"scene.h"



extern GameDataManager* data_manager;
extern bool quit;
//主菜单场景类
class MenuScene : public Scene
{
private:
	bool is_enter=0;
	Button_txt* start_button;
	Button_txt* quit_button;
	Button_txt* last_button;
public:
	MenuScene();
	~MenuScene() = default;

	void on_enter();
	void on_updata(int delta);
	void on_render(Camera& camera);
	void on_input(SDL_Event& Event);
	void on_exit();
	void _scene_reset() ;

};
MenuScene::MenuScene() {
	next_scene_state = SceneState::menu;
	before_scene_state = SceneState::menu;
	SDL_FRect size = {100,500,200,100};
	start_button=new Button_txt(size, u8"resources\\ttf\\simfang.ttf", u8"开始游戏", { 255,255,255,0 }, {115,115,155,150},{ 100,55,105,250 },200,45);
	size = { (1280 - 300),500,200,100 };
	quit_button =new Button_txt(size, u8"resources\\ttf\\simfang.ttf", u8"退出游戏", { 255,255,255,0 }, { 115,115,155,150 }, { 100,55,105,250 }, 200,45);
	size = { 540,500,200,100 };
	last_button = new Button_txt(size, u8"resources\\ttf\\simfang.ttf", u8"继续游戏", { 255,255,255,0 }, { 115,115,155,150 }, { 100,55,105,250 }, 200, 45);
	game_audio = new GameData_audio((int)SceneState::menu);

}


void MenuScene::on_enter() {

}
void MenuScene::on_updata(int delta) {
	game_audio->on_updata((int)SceneState::menu,nullptr);

	if (start_button->check_button(mouse_pos))
	{
		start_button->set_background_color({ 100,55,105,250 });
		if (is_enter) { next_scene_state = SceneState::game; before_scene_state = SceneState::menu; game_audio->HaltMusic(); data_manager->reset_progress(); scene_reset = 1; }
	}else{ start_button->set_background_color({ 115,115,155,150 }); }
	if (quit_button->check_button(mouse_pos))
	{
		quit_button->set_background_color({ 100,55,105,250 });
		if(is_enter)quit = 1;
	}else { quit_button->set_background_color({ 115,115,155,150 }); }
	if (last_button->check_button(mouse_pos))
	{
		last_button->set_background_color({ 100,55,105,250 });
		if (is_enter) { next_scene_state = SceneState::set; before_scene_state = SceneState::menu; game_audio->HaltMusic(); now_UI_state = UI_State::load; scene_reset = 1;}
	}
	else { last_button->set_background_color({ 115,115,155,150 }); }

}
void MenuScene::on_render(Camera& camera) {
	if(data_manager->get_game_record()>1)camera.renderer_texture(data_manager->get_menu_data(1), 0, FRect_dst, 0, 0);
	else camera.renderer_texture(data_manager->get_menu_data(0), 0, FRect_dst, 0, 0);
	start_button->on_render();
	quit_button->on_render();
	last_button->on_render();
}

void MenuScene::on_input(SDL_Event& Event) {
	

	while (SDL_PollEvent(&Event)) {
		
		game_audio->on_input(Event);
		
		switch (Event.type)
		{
		case SDL_QUIT:
			quit = 1;
			break;
		case SDL_MOUSEMOTION:
			mouse_pos.x = (float)Event.motion.x;
			mouse_pos.y = (float)Event.motion.y;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (SDL_BUTTON_LEFT == Event.button.button)  is_enter = 1;
			break;
		case SDL_MOUSEBUTTONUP:
			if (SDL_BUTTON_LEFT == Event.button.button)  is_enter = 0;
			break;
		default:
			break;
		}
	}
}

void MenuScene::on_exit() {

}
void MenuScene::_scene_reset() {
	is_enter = 0; is_next = 0;
};