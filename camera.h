#pragma once
#include"Vector2D.h"
#include"Timer.h"
#include<SDL.h>
#include<SDL_image.h>

class Camera
{
private:
	Vector2D pos{};//摄像机位置
	Timer shake_time;//抖动时间
	bool bool_shake = 0;//抖动信号
	float shake_strength = 0;//抖动强度
	SDL_Renderer* renderer;//渲染对象
	void _shake(float strength, int time);
public:
	Camera(SDL_Renderer* _renderer);
	~Camera();
	Vector2D get_pos();//获取摄像机2维点坐标
	void reset();//重置摄像机位置
	void on_enter();//初始化
	void on_updata(int delta);//数据更新
	SDL_Renderer* get_renderer();
	void renderer_texture(SDL_Texture* _texture,const SDL_Rect* rect_src, const SDL_FRect* rect_dst,
		double angle/*旋转角度*/, const SDL_FPoint* center/*旋转中心*/)const;

};