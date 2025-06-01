#pragma once
#include"camera.h"
Camera::Camera(SDL_Renderer* _renderer)
{
	this->renderer = _renderer;

	shake_time.set_one_shot(1);//设置定时器单次触发
	shake_time.set_callback([&]() {
		bool_shake = 0;
		reset();
		});//设置定时器的回调函数
}

Camera::~Camera()
{
}
Vector2D Camera::get_pos() {
	return pos;
}
void Camera::reset() {
	pos.x = 0;
	pos.y = 0;
}
void Camera::on_enter() {

}
void Camera::on_updata(int delta) {
	shake_time.updata(delta);
	if (bool_shake)
	{
		pos.x = (-50 + rand() % 100) / 50.0f * shake_strength;
		pos.y = (-50 + rand() % 100) / 50.0f * shake_strength;
	}
}

void Camera::_shake(float strength, int time) {
	this->shake_strength = strength;
	this->bool_shake = true;

	this->shake_time.set_wait_time(time);
	this->shake_time.restrat();
}
SDL_Renderer* Camera::get_renderer() {
	return renderer;
}

void Camera::renderer_texture(SDL_Texture* _texture, const SDL_Rect* rect_src, const  SDL_FRect* rect_dst,
	double angle, const SDL_FPoint* center)const {
	if (rect_dst == nullptr)
	{
		SDL_RenderCopyExF(renderer, _texture, rect_src, rect_dst, angle, center, SDL_RendererFlip::SDL_FLIP_NONE);
	}
	else {
		SDL_FRect rect_dst_win = *rect_dst;
		rect_dst_win.x -= pos.x;
		rect_dst_win.y -= pos.y;
		SDL_RenderCopyExF(renderer, _texture, rect_src, &rect_dst_win, angle, center, SDL_RendererFlip::SDL_FLIP_NONE);
	}
}