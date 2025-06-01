#pragma once
#include"button_inc.h"
#include"game_data_txt.h"



//��ť��(����)
class Button_img
{
private:
	SDL_FRect rect;//��ť�Ĵ�Сλ��
	SDL_FRect txt_Rect;
	SDL_Texture* BG_tex;//��������
	SDL_Texture* T_tex;//�仯����
	SDL_Texture* txt_tex;//��������
	SDL_FRect T_rect = {};//�仯��С

	TTF_Font* font;//����
	int font_size;//�����С
	std::string  u8_str;//����
	SDL_Color str_color = {255,255,255,0};//��ɫRGBA

	int MAX_WIGHT;//���������
	int MAX_HIGHT;//�������߶�

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

