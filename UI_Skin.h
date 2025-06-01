#pragma once
#include"button_img.h"
#include"game_data_manager.h"
#include<vector>
#include<SDL.h>

extern GameDataManager* data_manager;
class UI_Skin
{
private:

	struct Range
	{
		int start;
		int end;
	};
	std::vector<Button_img*> button_list;
	SDL_Texture* BG_Tex;
	int pages_number = 1;
	Range txt_button_range = {-1,-1};
	int now_wait_button = -1;
	
public:
	UI_Skin();
	~UI_Skin();
	void auto_create_button(SDL_Rect one_rect_size, SDL_Rect dst_rect,int txt_MAX_WIGHT, int txt_MAX_HIGHT,
		bool txt_button, int txtRect_x_movve_R , int txtRect_y_movve_D );
	void up_data(const Vector2D &pos);
	void on_render(Camera& camera);
	void set_button_rect(int idx, SDL_FRect rect);
	void set_BG(std::string dir);
	void set_button_T(int idx, int end, SDL_FRect rect, SDL_Texture* tex_img);
	void set_button_T(int idx, int end, SDL_FRect rect,std::string tex_img_dir);
	void set_button_T(int idx, int end, SDL_FRect rect, SDL_Color color);
	int get_now_wait_button() { return now_wait_button; }
	int get_txt_button_Range_start() { return txt_button_range.start; }
	int get_txt_button_Range_end() { return txt_button_range.end; }
	int get_pages_number() { return pages_number; }
};

UI_Skin::UI_Skin()
{
	
}

UI_Skin::~UI_Skin()
{
	
}
void UI_Skin::auto_create_button( SDL_Rect one_rect_size, SDL_Rect dst_rect, int txt_MAX_WIGHT, int txt_MAX_HIGHT,
	bool txt_button=0, int txtRect_x_movve_R = 0, int txtRect_y_movve_D = 0){
	int start_idx = button_list.size();
	int h_number = dst_rect.h / one_rect_size.h;
	int w_number = dst_rect.w / one_rect_size.w;
	float x= dst_rect.x, y = dst_rect.y;
	SDL_FRect FRect = { dst_rect.x,dst_rect.y,one_rect_size.w,one_rect_size.h };
	for (int i =1; i <= w_number; i++)
	{
		for (int l = 1; l <= h_number; l++) {
			Button_img* button=new Button_img(FRect, { 255,255,255,0 }, txt_MAX_WIGHT,txt_MAX_HIGHT, txtRect_x_movve_R, txtRect_y_movve_D,u8"!",0,nullptr,nullptr);
			button_list.push_back(button);
			FRect.y += one_rect_size.h;
		}
		FRect.x += one_rect_size.w;
	}
	int end_idx = button_list.size();
	if (txt_button) {
		if (txt_button_range.start == -1)txt_button_range = { start_idx,end_idx };
		else txt_button_range = { txt_button_range.start,end_idx };
	}
}
void UI_Skin::up_data(const Vector2D& pos) {
	if (!(txt_button_range.end - txt_button_range.start <= 0))
	{
		int p= 1;
		for (int i = txt_button_range.start; i <= txt_button_range.end; i++)
		{
			GameData_txt* data_txt= data_manager->get_savedata((pages_number * 10 + p));
			if(data_txt!=nullptr)button_list[i]->set_txt(data_txt, u8"resources\\ttf\\simfang.ttf", 16);
			p++;
		}
	}
	int i = 0;
	for (Button_img* button : button_list) {
		button->up_data(pos);
		if (button->in_rect)now_wait_button = i;
		i++;
	}
	if (now_wait_button!=-1&&!button_list[now_wait_button]->in_rect)now_wait_button = -1;
}
void UI_Skin::on_render(Camera& camera) {
	camera.renderer_texture(BG_Tex, nullptr, nullptr, 0, 0);
	for (Button_img* button : button_list) {
		button->on_render();
	}
}
void UI_Skin::set_button_rect(int idx, SDL_FRect rect) {
	button_list[idx - 1]->set_rect(rect);
}
void UI_Skin::set_BG(std::string dir) {
	BG_Tex = data_manager->load_texture(dir);
}
void UI_Skin::set_button_T(int start_idx, int end_idx, SDL_FRect rect, SDL_Texture* tex_img) {
	for (int  i = start_idx; i <= end_idx; i++)
	{
		if (i<=0)button_list[i]->set_T(rect, tex_img);
		
		else button_list[i-1]->set_T(rect, tex_img);
	}
}
void UI_Skin::set_button_T(int start_idx, int end_idx, SDL_FRect rect, std::string tex_img_dir) {
	for (int i = start_idx; i <= end_idx; i++)
	{
		if (i <= 0)button_list[i]->set_T(rect, data_manager->load_texture(tex_img_dir));

		else button_list[i - 1]->set_T(rect, data_manager->load_texture(tex_img_dir));
	}
}
void UI_Skin::set_button_T(int idx, int end, SDL_FRect rect, SDL_Color color) {
	//ÔÝÎÞ
}