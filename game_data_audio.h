#pragma once
#include"data_tool.h"
#include"game_data_txt.h"
#include <io.h>
#include<fstream>



class GameData_audio
{
private:
	struct Chunk_state
	{
		bool chunk_1 = 0;//角色声音通道
		bool chunk_2 = 0;//效果声音通道
		bool chunk_3 = 0;//效果声音通道
	};
	Chunk_state chunk_playing;
	bool music_playing = 0;
	bool is_next = 0;
	bool is_before = 0;
	int music_idx = 0;
	KaracterName name;

	int music_volume = 100;
	int game_bgm_volume = 30;

	std::vector<int> music_number = { -1,1,-1,2,-1,1,2,3,5,6,
		                               7,8,9,10,11,5,12,-1,13,-1,
		                              -1,14,7,15,6,-1,12,2,16,17,
		                              16,17,16,17,16,2,17,7,-1,5,
		                              18,19 };//记录的是歌曲序号

	std::vector<Mix_Music*> music_list;
	std::vector<Mix_Chunk*> chunk_list_AOI;
	
public:
	GameData_audio(int);
	~GameData_audio();

	
	void data_load_music(std::string path);
	void data_load_chunk(std::string path);

	void HaltMusic();

	void on_enter();
	void on_input(SDL_Event& event);
	void on_updata(int scene_staate, GameData_txt* now_txt);
	void on_delete();

	void play_chunk(int idx);
	void play_music(int idx);
	int chrck_chunk_playing();
	void set_volume_music(int volume);
};

