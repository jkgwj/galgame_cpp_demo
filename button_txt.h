#pragma once
#include"button_inc.h"



//��ť��(������)
class Button_txt
{
private:
	SDL_FRect rect;//��ť�Ĵ�С
	SDL_FRect ttf_rect;//���ִ�С
	TTF_Font* font;//����
	int font_size;
	std::string  u8_str;//����
	SDL_Color str_color;//��ɫRGBA
	SDL_Color background_color;//������ɫRBGA
	SDL_Color T_color;//��ɫ�仯ֵ
	int MAX_WIGHT;
	std::function<void()> callback;

	SDL_Texture* tex_img;
public:
	
	bool in_rect=0;//�ھ������ж�
	bool shot = 0;////�Ƿ񴥷���һ��

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

