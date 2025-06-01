#pragma once
#include"Timer.h"
#include"Camera.h"
#include<vector>
#include<SDL.h>

extern Camera* main_camera;

class Animation
{
private:
	int now_idx=0;
	int max_idx;
	bool is_next=0;
	Timer timer;
public:
	Animation();
	~Animation();
	void frame_render(std::vector<SDL_Texture*> frame_list, SDL_FRect* dst_rect, long long ns_delat);
};

Animation::Animation()
{
	timer.set_callback([this]() {is_next = 1; });
}

Animation::~Animation()
{
}
void Animation::frame_render(std::vector<SDL_Texture*> frame_list,SDL_FRect* dst_rect, long long ns_delat) {
	max_idx = frame_list.size();
	if (ns_delat != timer.get_wait_time())timer.set_wait_time(ns_delat);
	timer.updata(ns_delat);
	if (is_next) {
		now_idx++;
		if (now_idx >= max_idx||now_idx<0)now_idx = 0;
	}
	main_camera->renderer_texture(frame_list[now_idx],nullptr,dst_rect,NULL,NULL);
}