#pragma once
#include"scene.h"
#include"UI_Skin.h"
extern GameDataManager* data_manager;

class SetScene:public Scene
{
private:
	struct ExButton
	{
		SDL_FRect rect;
		std::function<void()> callback;//回调函数
		ExButton(SDL_FRect* _rect, std::function<void()> call_back) {
			rect = *_rect; callback = call_back;
		}
	};

	UI_Skin save; UI_Skin load;
	int now_wait_button = -1;
	std::vector<ExButton*> ex_button_list;

	bool ExButton_updata(ExButton* button) {
		bool _x = (mouse_pos.x >= button->rect.x && mouse_pos.x <= (button->rect.x + button->rect.w));
		bool _y = (mouse_pos.y >= button->rect.y && mouse_pos.y <= (button->rect.y + button->rect.h));
		if (_x && _y)button->callback();
		return (_x && _y);
	}
public:

	SetScene();
	~SetScene();

    void on_enter();
	void on_updata(int delta);
	void on_render(Camera& camera);
	void on_input(SDL_Event& Event);
	void on_exit();
	void _scene_reset(){};

};
SetScene::SetScene()
{
	next_scene_state = SceneState::set; 
	before_scene_state = SceneState::set;
	load.set_BG(u8"resources\\image\\UI\\UI_load.png");
	load.auto_create_button({ 0,0,525,110 }, { 112,113,525,555 },525-260,90, 1,190,20);
	load.auto_create_button({ 0,0,525,110 }, { 651,113,525,555 },525-260,90, 1,190,20);
	load.set_button_T(load.get_txt_button_Range_start(), load.get_txt_button_Range_end(), {}, "");

	save.set_BG(u8"resources\\image\\UI\\UI_save.png");
	save.auto_create_button({ 0,0,525,110 }, { 112,113,525,555 },525-260,90, 1,190,20);
	save.auto_create_button({ 0,0,525,110 }, { 651,113,525,555 },525-260,90, 1,190,20);
	save.set_button_T(save.get_txt_button_Range_start(), save.get_txt_button_Range_end(), {}, "");

	SDL_FRect temp_FRect;
	temp_FRect = {1080, 670, 200, 50 };
	ExButton* temp = new ExButton(&temp_FRect, [this]() {next_scene_state = before_scene_state; before_scene_state = SceneState::set; });
	ex_button_list.push_back(temp);//返回上一个场景
	temp_FRect = { 846, 678, 100, 35 };
	temp = new ExButton(&temp_FRect, [this]() {next_scene_state = SceneState::menu; before_scene_state = SceneState::set; });
	ex_button_list.push_back(temp);//返回游戏菜单

	
}

SetScene::~SetScene()
{
}
void SetScene::on_enter() { 

}
void SetScene::on_updata(int delta) {
	switch (now_UI_state)
	{
	case UI_State::save:
		save.up_data(mouse_pos);
		now_wait_button = save.get_now_wait_button();
		
		if (is_next) {
			if (now_wait_button >=0 && now_wait_button <= save.get_txt_button_Range_end() && now_wait_button >= save.get_txt_button_Range_start()) {
				int  number = (save.get_pages_number() * 10) + (now_wait_button + 1);
				data_manager->save(number);
				next_scene_state = SceneState::game; before_scene_state = SceneState::set; 
				is_next = 0;
			}
		}

		for (ExButton* button : ex_button_list) {
			if (!is_next)continue;
			if(ExButton_updata(button))break;
		}
		break;
	case UI_State::load:
		load.up_data(mouse_pos);
		now_wait_button = load.get_now_wait_button();
		if (is_next){
		if(now_wait_button>=0&&now_wait_button<=load.get_txt_button_Range_end()&&now_wait_button>=load.get_txt_button_Range_start()){
			int  number = (load.get_pages_number() * 10) + (now_wait_button + 1);
			data_manager->load(number,is_next);
			next_scene_state = SceneState::game; before_scene_state = SceneState::set; data_manager->option_sync = 1;
			is_next = 0;
		    }
	    }

		for (ExButton* button : ex_button_list) {
			if (!is_next)continue;
			if (ExButton_updata(button))break;
		}

		break;
	case UI_State::set:
		break;
	case UI_State::nop:
		break;
	default:
		break;
	}
	std::cout << now_wait_button << std::endl;
	
}
void SetScene::on_render(Camera& camera) {
	switch (now_UI_state)
	{
	case UI_State::save:
		save.on_render(camera);
		break;
	case UI_State::load:
		load.on_render(camera);
		break;
	case UI_State::set:
		break;
	case UI_State::nop:
		break;
	default:
		break;
	}
}
void SetScene::on_input(SDL_Event& Event) {
	while (SDL_PollEvent(&Event)) {
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
			if (SDL_BUTTON_LEFT == Event.button.button)  is_next = 1;
			break;
		case SDL_MOUSEBUTTONUP:
			if (SDL_BUTTON_LEFT == Event.button.button)  is_next = 0;
			break;
		default:
			break;
		}
	}
}
void SetScene::on_exit() {

}