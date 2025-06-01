#pragma once
#include"Vector2D.h"
#include"Timer.h"
#include<SDL.h>
#include<SDL_image.h>

class Camera
{
private:
	Vector2D pos{};//�����λ��
	Timer shake_time;//����ʱ��
	bool bool_shake = 0;//�����ź�
	float shake_strength = 0;//����ǿ��
	SDL_Renderer* renderer;//��Ⱦ����
	void _shake(float strength, int time);
public:
	Camera(SDL_Renderer* _renderer);
	~Camera();
	Vector2D get_pos();//��ȡ�����2ά������
	void reset();//���������λ��
	void on_enter();//��ʼ��
	void on_updata(int delta);//���ݸ���
	SDL_Renderer* get_renderer();
	void renderer_texture(SDL_Texture* _texture,const SDL_Rect* rect_src, const SDL_FRect* rect_dst,
		double angle/*��ת�Ƕ�*/, const SDL_FPoint* center/*��ת����*/)const;

};