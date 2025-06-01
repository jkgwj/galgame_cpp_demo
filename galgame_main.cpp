#define SDL_MAIN_HANDLED

#include"game_data_manager.h"
#include"menu_scene.h"
#include"game_scene.h"
#include"set_scene.h"
#include"scene_manager.h"
#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

#include<chrono>
#include<thread>


using namespace std::chrono;

SDL_FRect* FRect_dst=new SDL_FRect();

SDL_Window* windown = nullptr;
SDL_Renderer* renderer = nullptr;

GameScene* game_scene = nullptr;
MenuScene* menu_scene = nullptr;
SetScene* set_scene   = nullptr;
SceneManager* scene_manager = nullptr;

GameDataManager* data_manager;

Camera* main_camera;

bool quit = 0;

void INIT(SDL_Event& event);

int main() {
	//消息句柄
	SDL_Event Event;
	//初始化
	INIT(Event);
	//动态延迟
	const nanoseconds FPS(1000000000 / 144);
	steady_clock::time_point last_time = steady_clock::now();
	while (!quit) {

		//消息处理
		scene_manager->Input(Event);

		//数据更新

		steady_clock::time_point frame_start = steady_clock::now();
		duration<float> delta = duration<float>(frame_start - last_time);
		int ns_count = delta.count() * 1000000000;

		scene_manager->Updata(ns_count);

		//渲染
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//SDL_SetWindowFullscreen(windown, SDL_WINDOW_FULLSCREEN);//全屏功能

		scene_manager->Render((*main_camera));

		SDL_RenderPresent(renderer);

		//动态延迟
		last_time = frame_start;
		nanoseconds sleep_time = FPS - (steady_clock::now() - frame_start);
		if (sleep_time > nanoseconds(0)) { std::this_thread::sleep_for(sleep_time); }


	}

	
	delete(scene_manager);
	delete(set_scene);
	delete(menu_scene);
	delete(game_scene);
	delete(main_camera);
	//释放游戏资源(还没写,但退出系统会帮你自动回收，后面补上)
	//释放场景资源(还没写,但退出系统会帮你自动回收，后面补上)
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(windown);
	TTF_Quit();
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();

		return 0;
}

void INIT(SDL_Event& event) {
	system("chcp 65001");

	//初始化
	SDL_Init(SDL_INIT_EVERYTHING);
	Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	TTF_Init();

	FRect_dst->w = 1280; FRect_dst->h = 720;
	//打开要使用的音轨
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	//申请使用32条通道
	Mix_AllocateChannels(32);
	//创建Window
	windown = SDL_CreateWindow(u8"GalGame_demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	//创建纹理渲染器
	renderer = SDL_CreateRenderer(windown, 1, SDL_RENDERER_ACCELERATED);
	//实例化资源管理器
	data_manager = data_manager->instance(renderer);
	//加载资源
	bool loaded = data_manager->bin_load_game_data(renderer);
	//bool loaded = data_manager->load_game_data(renderer);
	data_manager->load_savedata(0);

	//实例化摄像机
	main_camera = new Camera(renderer);
	//实例化场景
	game_scene = new GameScene();
	menu_scene = new MenuScene();

	set_scene = new SetScene();
	scene_manager = new SceneManager(event);

	//场景设置
	scene_manager->set_scene_state(menu_scene);
}