#pragma once
#include"button_inc.h"



//按钮类(非纹理)
class Button_txt
{
private:
	SDL_FRect rect;//按钮的大小
	SDL_FRect ttf_rect;//文字大小
	TTF_Font* font;//字体
	int font_size;
	std::string  u8_str;//内容
	SDL_Color str_color;//颜色RGBA
	SDL_Color background_color;//背景颜色RBGA
	SDL_Color T_color;//颜色变化值
	int MAX_WIGHT;
	std::function<void()> callback;

	SDL_Texture* tex_img;
public:
	
	bool in_rect=0;//在矩形内判断
	bool shot = 0;////是否触发过一次

	Button_txt(){}
	Button_txt(SDL_FRect _size_rect, std::string font_dir_ttf, std::string _u8_str,
		SDL_Color _txt_color, SDL_Color BG_color, SDL_Color _T_color, int max_wight, int txt_size);
	~Button_txt();
	//void on_enter(SDL_FRect _size_rect, std::string font_dir_ttf, std::string _u8_str, SDL_Color _color, SDL_Color BG_color, SDL_Color _T_color, int max_wight);
	void on_render();
	void on_render_nostretch();
	bool check_button(const Vector2D& pos);
	void updata(const Vector2D& pos);

	void set_str_color(SDL_Color _color) { str_color = _color; }
	void set_background_color(SDL_Color _color) { background_color = _color; }
	void set_T_color(SDL_Color _color) { T_color = _color; }
	void set_max_wight(int w) { MAX_WIGHT = w; }
	void set_font_size(int size) { font_size = size; }
	void set_callback(std::function<void()> func) { callback = func; }
	void call_back() { callback(); }
};

