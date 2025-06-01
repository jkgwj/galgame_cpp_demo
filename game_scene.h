#pragma once
#include"scene.h"
#include"game_data_txt.h"
#include"UI_Skin.h"
#include<iostream>
extern Camera* main_camera;
extern GameDataManager* data_manager;


//游戏场景类
class GameScene : public Scene
{
private:
	
	bool one_game=0;
	bool is_set = 0;
	bool seting=0;
	bool is_mouse_left = 0;
	bool is_mouse_right=0;
	bool is_skip = 0;
	bool is_retrace = 0;
	bool is_retrace_page = 0;
	bool is_retrace_next = 0;
	bool is_option = 0;
	bool is_optioning = 0;
	bool option_enter = 0;

	bool is_debug = 0; 
	bool is_auto = 0;
	bool is_input = 0;
	

	SDL_Texture* txt_tex;
	SDL_Texture* name_tex;
	SDL_FRect name_FRect = {20,550,130,200};
	SDL_FRect txt_FRect  = {230,550,1050,200};
	GameData_txt* data_txt;

	Button_txt* save_button;
	Button_txt* load_button;
	Timer auto_timer;
	Timer delay_timer;
	std::vector<Button_txt*>* option_data;
	std::vector<Button_txt*> dialog_box;

	int re_img_progress = -1;
	int next_option_start = 0;
	int next_option_end = 0;
	int befor_option_end = 0;
	int now_option_end = 0;

	void _optiondata_updata();
	void _option_updata(int reset );
	void _option_render();
	void _dialog_box_updata();
	void _dialog_box_rener();
public:
	GameScene();
	~GameScene() = default;

	int retrace_page_updata(bool reset);

	void on_enter();
	void on_updata(int delta);
	void on_render(Camera& camera);
	void on_input(SDL_Event &Event);
	void on_exit();

	void _scene_reset();

};

int GameScene::retrace_page_updata(bool reset = 0) {
	static int re_TXT = -1;
	static int re_TXT_MAX = -1;
	static int re_TXT_MIN = 0;
	static bool one = 0;
	static int befor_option = -1;
	int _p = -3;
	if (reset) { re_TXT = -1;  re_TXT_MAX = 0; one = 0; befor_option = -1; return 0; }
	if (re_TXT > re_TXT_MAX||(re_TXT>=re_TXT_MAX&&is_option) ) { 
		re_TXT = re_TXT_MAX;return -3;  }
	
	if (re_TXT == -1 ) {
		bool  _temp = 0;
		int p = -1;
		data_manager->get_txt_data(0, _temp, p, 0, 0);
		if (data_manager->get_txt_data(p-1)->check_no_render(0) == 2)p--;
		re_TXT = p;
		re_TXT_MAX = p;
		if (data_manager->get_txt_data(re_TXT)->option == -1) { re_TXT--; re_TXT_MAX--; }
	}
	if (!one && is_option) { one = 1; re_TXT--; }
	if(befor_option==-1)befor_option=data_manager->get_last_option_record(re_TXT_MIN);
	if (is_retrace) {
		re_TXT--;
		while (data_manager->get_txt_data(re_TXT)->check_no_render(befor_option) > 0&&re_TXT>re_TXT_MIN)re_TXT--;
	}
	else if (is_retrace_next) {
		re_TXT++;
		while (data_manager->get_txt_data(re_TXT)->check_no_render(befor_option) > 0 && re_TXT > re_TXT_MIN)re_TXT++;
	}
	if (re_TXT <= re_TXT_MIN)re_TXT = re_TXT_MIN;
	data_txt = data_manager->get_txt_data(is_next, seting, _p, 0, re_TXT);
	game_audio->on_updata((int)SceneState::game, data_txt);
	
		data_txt->up_texture(main_camera->get_renderer(), txt_tex, name_tex,
			u8"resources\\ttf\\simfang.ttf", u8"resources\\ttf\\simfang.ttf", 32, 48, txt_FRect, name_FRect, { 155,158,188,0 });
	
	is_retrace = 0;is_retrace_next = 0;
	return 0;
}

GameScene::GameScene() {
	next_scene_state = SceneState::game;
	before_scene_state = SceneState::game; 
	SDL_FRect size = { 1080,50,150,75 };
	save_button = new Button_txt(size, u8"resources\\ttf\\simfang.ttf", u8"存档",
		{ 255,255,255,0 }, { 195,195,195,150 }, { 235,235,235,75 },150,38);
	size = { 1080,125,150,75 };
	load_button = new Button_txt(size, u8"resources\\ttf\\simfang.ttf", u8"读档",
		{ 255,255,255,0 }, { 195,195,195,150 }, { 235,235,235,75 },150,38);
	size = { 1080,620,50,25 };
	game_audio = new GameData_audio((int)SceneState::game);
	//计时器初始化
	auto_timer.set_wait_time(3000000000);
	auto_timer.set_callback([this]() {is_next = 1; });
	delay_timer.set_wait_time(200000000);
	delay_timer.set_callback([this]() {is_input = 1; });
	_optiondata_updata();//选项数据更新
	//对话框初始化
	Button_txt* button;
	size = { 35,680,55,40 };
	std::vector<std::string> u8_txt = { u8"auto",u8"skip",u8"save",u8"load" };
	for (int i = 0; i < u8_txt.size(); i++)
	{
		button = new Button_txt(size, u8"resources\\ttf\\simfang.ttf", u8_txt[i], {255,255,255,0}, {200,215,215,200}, {200,200,200,100}, 55, 26);
		dialog_box.push_back(button);
		size.x += (size.w + 3);
	}
	dialog_box[0]->set_callback([this]() {is_auto = !is_auto; });
	dialog_box[1]->set_callback([this]() {is_skip = !is_skip; });
	dialog_box[2]->set_callback([this]() {next_scene_state = SceneState::set; before_scene_state = SceneState::game; now_UI_state = UI_State::save; is_mouse_left = 0; seting = 1; is_next = 0; });
	dialog_box[3]->set_callback([this]() {next_scene_state = SceneState::set; before_scene_state = SceneState::game; now_UI_state = UI_State::load; is_mouse_left = 0; seting = 1; is_next = 0; });
	
	std::vector<int> int_list;
	data_manager->get_END_idx(int_list);
	for (int idx : int_list) {
		data_manager->get_txt_data(idx)->set_call_back([this](int _t) {
			next_scene_state = SceneState::menu;
			before_scene_state = SceneState::game; 
			data_manager->option_sync = 1;
			data_manager->read_game_record(_t);
			});
	}
}

void GameScene::on_enter() {

}
void GameScene::on_updata(int delta) {
	if (txt_tex == nullptr)delete(txt_tex);
	if (name_tex == nullptr)delete(name_tex);
   if (is_skip&&!is_set) {  is_next = 1; }
   if(reset){
	   is_next = 0; reset = 0; is_mouse_left = 0; is_set = 0; is_option = 0; mouse_pos = { 0,0 }; is_skip = 0;
   }
   if (data_manager->option_sync) { 
	   this->_optiondata_updata();
       option_data = data_manager->get_option_data();
       _option_updata(-1); 
       data_manager->option_sync = 0; is_next = 0; reset = 0; is_mouse_left = 0; is_set = 0; option_enter = 0;
   }

	delay_timer.updata(delta);//更新输入延迟定时器
	//if (is_debug)data_manager->data_debaug(is_debug);
	if (!is_retrace_page && !is_set) { 
		_dialog_box_updata(); if (next_scene_state == SceneState::set)return; }//对话框更新
	if (is_retrace_page) {
		
		if (is_set) { retrace_page_updata(1); is_retrace_page = 0; }
		else re_img_progress = retrace_page_updata(0);
		if(re_img_progress==-3) { retrace_page_updata(1); is_retrace_page = 0; }
	}
	else
   {
	
	
	  if (is_set) { 
	  	save_button->updata(mouse_pos); 
	  	load_button->updata(mouse_pos); 
	  	if (is_mouse_left) {
	  		if (save_button->in_rect) {
	  			next_scene_state = SceneState::set; before_scene_state = SceneState::game; now_UI_state = UI_State::save; is_mouse_left = 0; seting = 1; is_next = 0;
	  			return;
	  		}
	  		if(load_button->in_rect){ 
	  			next_scene_state = SceneState::set; before_scene_state = SceneState::game; now_UI_state = UI_State::load;  is_mouse_left = 0; seting = 1; is_next = 0;
	  			return;
	  		}
	  	}
	  }
	  
	  if (is_option&&!is_set) { 
	  	is_auto = 0; is_next = 0; if (is_mouse_left) { is_mouse_left = 0; option_enter = 1; } }
	  
	  if (is_auto&&!is_set)auto_timer.updata(delta,game_audio->chrck_chunk_playing());//更新计时器
	  
	  
	  
	  //if (!is_set && !is_mouse_right && is_mouse_left && !is_next) { is_next = 1; is_mouse_left = 0; }
	  if (is_next)data_manager->set_txt_next(1);
	  int _temp=0;
	  data_txt = data_manager->get_txt_data(is_next,seting,_temp,0,0);
	  game_audio->on_updata((int)SceneState::game, data_txt);
	  
	  if ((!is_set && is_option) || data_txt->get_number() == next_option_start - 2) {
	  	std::cout << u8"选项中\n";is_option = 1; _option_updata(0); option_enter = 0; 
	  }
	  if (is_optioning && data_txt->get_number() == now_option_end-1) {
	  	data_manager->jmp_txt_number(befor_option_end); is_optioning = 0;}
	  
	  else if (!is_option) {
	  	if (data_txt->check_no_render(-2) == 3)data_txt = data_manager->get_txt_data(data_txt->get_number() + 1);
	  	
	  	data_txt->up_texture(main_camera->get_renderer(), txt_tex, name_tex,
	  		u8"resources\\ttf\\simfang.ttf", u8"resources\\ttf\\simfang.ttf", 32, 48, txt_FRect, name_FRect, { 255,58,88,0 });
	  }
	  std::cout << data_txt->get_string()<<std::endl;
	  if (!one_game)one_game = 1;
   }
}
void GameScene::on_render(Camera& camera) {
	static bool one = 0;
	int _temp = 0;
	if (one == 0) { one = 1; return; }
	if(!is_retrace_page||re_img_progress==-1)
		camera.renderer_texture(data_manager->get_CG_data(data_txt->get_CG_number()), 0, FRect_dst, 0, 0); 
	else { 
	_temp = -3;
	camera.renderer_texture(data_manager->get_CG_data(data_txt->get_CG_number()), 0, FRect_dst, 0, 0);
	}
	if (is_option&&!is_retrace_page)_option_render();
	if(!is_option || is_retrace_page)camera.renderer_texture(txt_tex, 0, &txt_FRect, 0, 0);
	if (!is_option || is_retrace_page)camera.renderer_texture(name_tex, 0, &name_FRect, 0, 0);
	if (is_set) { save_button->on_render(); load_button->on_render(); }
	if (!is_retrace_page && !is_set) { _dialog_box_rener(); }
	if (!is_option|| is_retrace_page)SDL_DestroyTexture(name_tex);
	if (!is_option|| is_retrace_page)SDL_DestroyTexture(txt_tex);

	if (is_next || is_mouse_left) { is_next = 0; is_mouse_left = 0; }

}
void GameScene::on_input(SDL_Event &Event) {
	if (is_input) {
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
			case SDL_KEYDOWN:
				if (Event.key.keysym.sym == SDLK_KP_ENTER || Event.key.keysym.sym == SDLK_RETURN)
					if (!is_set) { is_next = true; is_input = 0; }
				if (!is_set && Event.key.keysym.scancode == SDL_SCANCODE_LCTRL || Event.key.keysym.scancode == SDL_SCANCODE_RCTRL)
					if (!is_set) { is_skip = 1; }
				if (Event.key.keysym.scancode == SDLK_q || Event.key.keysym.scancode == SDL_SCANCODE_Q) { is_debug = !is_debug; is_input = 0; }
				if (Event.key.keysym.sym == SDLK_a) { is_auto = !is_auto; is_input = 0; }

				break;
			case SDL_KEYUP:
				if (Event.key.keysym.sym == SDLK_KP_ENTER || Event.key.keysym.sym == SDLK_RETURN)if (!is_set) { is_next = false; is_input = 0; }
				if (Event.key.keysym.scancode == SDL_SCANCODE_LCTRL || Event.key.keysym.scancode == SDL_SCANCODE_RCTRL)if (!is_set) { is_skip = 0; }

				break;
			case SDL_MOUSEBUTTONDOWN:
				if (SDL_BUTTON_LEFT == Event.button.button) { is_mouse_left = 1; if(!is_option&&!is_set)is_next = 1; is_input = 0; }
				if (SDL_BUTTON_RIGHT == Event.button.button) { is_mouse_right = (!is_mouse_right); is_set = (!is_set); is_input = 0;}
				break;
			case SDL_MOUSEBUTTONUP:
				if (SDL_BUTTON_LEFT == Event.button.button) { is_mouse_left = 0; is_next = 0; is_input = 0; }
				if (SDL_BUTTON_RIGHT == Event.button.button) {  is_input = 0; }
				break;
			case SDL_MOUSEWHEEL:
				if (Event.wheel.y < 0) { if (!is_set)is_next = 1; is_retrace_next = 1; is_input = 0;}
				if (Event.wheel.y > 0) { if (!is_set && !is_retrace_page)is_retrace_page = 1; else if (!is_set && is_retrace_page) is_retrace = 1; is_input = 0;}
				break;

			default:
				break;
			}
		}
		
	}
	else SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
	
}
void GameScene::on_exit() {
}

void GameScene::_scene_reset() {
	is_set = 0;
	seting = 0;
	is_mouse_left = 0;
	is_mouse_right = 0;
	is_retrace = 0;
	is_retrace_page = 0;
	is_retrace_next = 0;
	reset = 1;
	is_debug = 0;
	is_auto = 0;
	_option_updata(1);
	_optiondata_updata();
}

void GameScene::_optiondata_updata() {
	befor_option_end = next_option_end;
	data_manager->get_option_start_number(next_option_start, next_option_end);
}
void GameScene::_option_updata(int reset = 0) {
	static bool one = 0, next_option = 1;
	int i = -1;
	if (reset == 1) { one = 0; return; }
	if (reset == -1) { one = 1; return; }
	if ((!one && one_game) && next_option) {
		option_data = data_manager->get_option_data();  one = 1; option_enter = 0; next_option = 0;
	}
	if (option_data == nullptr)
		option_data = data_manager->get_option_data();

	for (Button_txt* button : *option_data) {
		i++;
		if (button->check_button(mouse_pos) && option_enter) {
			is_option = 0;
			one = 0;
			now_option_end = data_manager->jmp_option(i);
			//if (now_option_end != -1)is_optioning = 1;
			data_manager->record_option(i);
			_optiondata_updata();
			option_enter = 0;
			next_option = 1;
		}
	}
	option_enter = 0;
}
void GameScene::_option_render() {
	for (Button_txt* button : *option_data) {
		button->on_render_nostretch();
	}
}
void GameScene::_dialog_box_updata() {
	for (Button_txt* button : dialog_box) {
		button->updata(mouse_pos);
		if (button->in_rect && is_mouse_left) { 
		button->call_back(); 
		button->shot = !button->shot;
		is_mouse_left = 0; 
		is_next = 0;
		return;
		}
	}
}
void GameScene::_dialog_box_rener() {
	static SDL_Rect rect = { 0,550,1280,170 };
	bool _x = (mouse_pos.x >= rect.x && mouse_pos.x <= (rect.x + rect.w));
	bool _y = (mouse_pos.y >= rect.y && mouse_pos.y <= (rect.y + rect.h));
	if (_x && _y) {
		for (Button_txt* button : dialog_box) {
			button->on_render();
		}
	}

}