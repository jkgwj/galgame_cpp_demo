#include"button_img.h"
#include"camera.h"

extern Camera* main_camera;

Button_img::Button_img(SDL_FRect _rect, SDL_Color txt_color, int max_wight, int max_hight, int txtRect_x_movve_R = 0, int txtRect_y_movve_D = 0,
	std::string u8_font_dir = u8"!", int _font_size = 0, SDL_Texture* _BG_tex = nullptr, SDL_Texture* _T_tex = nullptr)
{
	rect = _rect; MAX_WIGHT = max_wight;
	MAX_HIGHT = max_hight;
	font_size = _font_size;
	if (u8_font_dir == u8"!") {
		font = TTF_OpenFont(u8_font_dir.c_str(), font_size);
		if (!font)
		{
			std::cout << u8"×ÖÌå¼ÓÔØÊ§°Ü\n" << TTF_GetError();
		}
	}
	txt_Rect.x = _rect.x + txtRect_x_movve_R;
	txt_Rect.y = _rect.y + txtRect_y_movve_D;
	str_color = txt_color;
	BG_tex = _BG_tex;
	T_tex = _T_tex;
	txt_tex = nullptr;
}

Button_img::~Button_img()
{
}

void Button_img::up_data(const Vector2D& pos) {
	bool _x = (pos.x >= rect.x && pos.x <= (rect.x + rect.w));
	bool _y = (pos.y >= rect.y && pos.y <= (rect.y + rect.h));
	in_rect = (_x && _y);
}

void Button_img::on_render() {
	SDL_Rect rect2;
	rect2.x = (int)rect.x; rect2.y = (int)rect.y; rect2.w = (int)rect.w; rect2.h = (int)rect.h;
	if (txt_tex != nullptr) {

		main_camera->renderer_texture(txt_tex, 0, &txt_Rect, 0, 0);
	}
	if (!in_rect) {
		main_camera->renderer_texture(BG_tex, 0, &rect, 0, 0);
	}
	else
	{
		if (T_tex != nullptr && T_rect.w <= 0)main_camera->renderer_texture(T_tex, 0, &rect, 0, 0);
		else if (T_tex == nullptr && T_rect.w > 0)main_camera->renderer_texture(txt_tex, 0, &T_rect, 0, 0);
		else  main_camera->renderer_texture(BG_tex, 0, &rect, 0, 0);
	}
}
void Button_img::set_txt(GameData_txt*& data_txt, std::string u8_font_dir = u8"!", int _font_size = 0) {

	SDL_Texture* dump_txt_tex;
	SDL_DestroyTexture(txt_tex);
	data_txt->up_textureEX_txt(main_camera->get_renderer(), dump_txt_tex, u8_font_dir, _font_size, txt_Rect, { 255,255,255,0 }, MAX_WIGHT, MAX_HIGHT);

	txt_tex = dump_txt_tex;


}
void Button_img::set_rect(SDL_FRect _rect) {
	rect = _rect;
}
void Button_img::set_txt_rect(SDL_FRect _rect) {
	txt_Rect = _rect;
}
void Button_img::set_T(SDL_FRect _rect = {}, SDL_Texture* _tex = nullptr) {
	T_rect = _rect;
	T_tex = _tex;
}
void Button_img::Free_Button_img() {
	SDL_DestroyTexture(txt_tex);
	SDL_DestroyTexture(T_tex);
	SDL_DestroyTexture(BG_tex);
	TTF_CloseFont(font);
}