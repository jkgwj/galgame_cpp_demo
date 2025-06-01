#include"button_txt.h"
#include"camera.h"

extern Camera* main_camera;
Button_txt::Button_txt(SDL_FRect _size_rect, std::string font_dir_ttf, std::string _u8_str,
	SDL_Color _txt_color, SDL_Color BG_color, SDL_Color _T_color, int max_wight, int txt_size) {
	rect = _size_rect;
	font_size = txt_size;
	font = TTF_OpenFont(font_dir_ttf.c_str(), font_size);
	if (!font)
	{
		std::cout << u8"字体加载失败 \n" << TTF_GetError();
	}
	u8_str = _u8_str;
	str_color = _txt_color;
	background_color = BG_color;
	T_color = _T_color;
	MAX_WIGHT = max_wight;
	SDL_Surface* suf_img = TTF_RenderUTF8_Blended_Wrapped(font, u8_str.c_str(), str_color, MAX_WIGHT);
	ttf_rect.x = rect.x; ttf_rect.y = rect.y; ttf_rect.w = suf_img->w; ttf_rect.h = suf_img->h;
	tex_img = SDL_CreateTextureFromSurface(main_camera->get_renderer(), suf_img);
}

Button_txt::~Button_txt() {
	TTF_CloseFont(font);
}

void Button_txt::on_render() {
	
	SDL_Rect rect2;
	rect2.x = (int)rect.x; rect2.y = (int)rect.y; rect2.w = (int)rect.w; rect2.h = (int)rect.h;
	//  设置颜色，填充目标矩形区域	
	if (!in_rect&&!shot)SDL_SetRenderDrawColor(main_camera->get_renderer(), background_color.r, background_color.g, background_color.b, background_color.a);
	if (in_rect||shot)SDL_SetRenderDrawColor(main_camera->get_renderer(), T_color.r, T_color.g, T_color.b, T_color.a);
	SDL_SetRenderDrawBlendMode(main_camera->get_renderer(), SDL_BLENDMODE_MOD);
	SDL_RenderFillRect(main_camera->get_renderer(), &rect2);
	main_camera->renderer_texture(tex_img, 0, &rect, 0, 0);
}
void Button_txt::on_render_nostretch() {
	SDL_Rect rect2;
	rect2.x = (int)rect.x; rect2.y = (int)rect.y; rect2.w = (int)rect.w; rect2.h = (int)rect.h;
	//  设置颜色，填充目标矩形区域	
	if (!in_rect&&!shot)SDL_SetRenderDrawColor(main_camera->get_renderer(), background_color.r, background_color.g, background_color.b, background_color.a);
	if (in_rect||shot)SDL_SetRenderDrawColor(main_camera->get_renderer(), T_color.r, T_color.g, T_color.b, T_color.a);
	SDL_SetRenderDrawBlendMode(main_camera->get_renderer(), SDL_BLENDMODE_MOD);
	SDL_RenderFillRect(main_camera->get_renderer(), &rect2);
	main_camera->renderer_texture(tex_img, 0, &ttf_rect, 0, 0);
}
bool Button_txt::check_button(const Vector2D& pos) {
	bool _x = (pos.x >= rect.x && pos.x <= (rect.x + rect.w));
	bool _y = (pos.y >= rect.y && pos.y <= (rect.y + rect.h));
	in_rect = (_x && _y);
	return (_x && _y);
}
void Button_txt::updata(const Vector2D& pos) { check_button(pos); }
//void Button::on_enter(SDL_FRect _size_rect, std::string font_dir_ttf, std::string _u8_str, SDL_Color _color, SDL_Color BG_color, SDL_Color _T_color, int max_wight) {
//	rect = _size_rect;
//	font = TTF_OpenFont(_u8_str.c_str(), font_size);
//	u8_str = _u8_str;
//	str_color = _color;
//	background_color = BG_color;
//	SDL_Surface* suf_img = TTF_RenderUTF8_Blended_Wrapped(font, u8_str.c_str(), str_color, MAX_WIGHT);
//	tex_img = SDL_CreateTextureFromSurface(main_camera->get_renderer(), suf_img);
//	SDL_FreeSurface(suf_img);
//}
