#pragma once
#include"button_inc.h"
#include"game_data_txt.h"



//按钮类(纹理)
class Button_img
{
private:
	SDL_FRect rect;//按钮的大小位置
	SDL_FRect txt_Rect;
	SDL_Texture* BG_tex;//背景纹理
	SDL_Texture* T_tex;//变化纹理
	SDL_Texture* txt_tex;//字体纹理
	SDL_FRect T_rect = {};//变化大小

	TTF_Font* font;//字体
	int font_size;//字体大小
	std::string  u8_str;//内容
	SDL_Color str_color = {255,255,255,0};//颜色RGBA

	int MAX_WIGHT;//文字最大宽度
	int MAX_HIGHT;//文字最大高度

public:
	bool  in_rect;

	Button_img(SDL_FRect _rect,  SDL_Color txt_color,int max_size, int max_hight, int txtRect_x_movve_R, int txtRect_y_movve_D,
		std::string u8_font_dir, int _font_size, SDL_Texture* _BG_tex , SDL_Texture* _T_tex);
	~Button_img();
	void Free_Button_img();
	void up_data(const Vector2D& pos);
	void on_render();
	void set_txt(GameData_txt* &data_txt, std::string u8_font_dir, int _font_size);
	void set_rect(SDL_FRect _rect);
	void set_txt_rect(SDL_FRect _rect);
	void set_T(SDL_FRect _rect, SDL_Texture* _tex);
};

