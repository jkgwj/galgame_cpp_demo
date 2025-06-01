#include"game_data_audio.h"
#include"game_data_manager.h"
#include <windows.h>

extern GameDataManager* data_manager;

GameData_audio::~GameData_audio()
{
}
GameData_audio::GameData_audio(int scene_state)
{
	switch (scene_state)
	{
	case 0:
		data_load_music(u8"resources\\oto\\menu.BGM");
		break;
	case 1:
		data_load_chunk(u8"resources\\oto\\kyara.AOI");
		data_load_music(u8"resources\\oto\\game.BGM");
		break;
	case 2:
		break;
	case 3:
		break;

	default:
		break;
	}

}

void GameData_audio::data_load_music(std::string path) {
	music_list= data_manager->deserializeAudioToMusic(path);
}

void GameData_audio::data_load_chunk(std::string path) {
	
	chunk_list_AOI=data_manager->deserializeAudioToChunk(path);
}

void GameData_audio::on_enter() {

}
void GameData_audio::on_input(SDL_Event& Event) {

	switch (Event.type)
	{
	case SDL_KEYDOWN:
		if (Event.key.keysym.sym == SDLK_x)  is_next = 1;
		if (Event.key.keysym.sym == SDLK_z)  is_before = 1;
		break;
	default:
		break;
	}

}
void GameData_audio::on_updata(int scene_state, GameData_txt* now_txt = nullptr) {
	/*enum class SceneState {
		menu = 0,
		game,
		set,
		nup
	};*/

	switch (scene_state)
	{
	case 0://menu场景
		
		if (is_next || is_before) {
			if (is_next) { music_idx++; is_next = 0; }
			else if (is_before) { music_idx--; is_before = 0; }
			if (music_idx > (music_list.size() - 1)) {
				music_idx = 0;
			}
			if (music_idx < 0) {
				music_idx = 0;
			}
			set_volume_music(music_volume);
			Mix_PlayMusic(music_list[music_idx], -1);
		}
		else if (!music_playing) {
			set_volume_music(music_volume);
			music_idx = (rand() % music_list.size()) - 1;
			Mix_PlayMusic(music_list[music_idx], -1);
			this->music_playing = 1;
		}
		break;
	case 1:
		name = now_txt->get_name();
		play_chunk(now_txt->audio_idx);
		set_volume_music(game_bgm_volume);//设置BGM音量大小
		play_music(now_txt->get_MUSIC_number());
		break;
	case 2:
		break;
	case 3:
		break;

	default:
		break;
	}

}

void GameData_audio::HaltMusic() {
	Mix_HaltMusic();
	music_playing = 0;
}

void GameData_audio::on_delete() {

}
void GameData_audio::play_chunk(int idx) {
	if (idx < 0)return;
	static int before_idx = idx;
	switch (name)
	{
	case AOI:
		if (before_idx != idx && chunk_playing.chunk_1) {
			chunk_playing.chunk_1 = 0;
			Mix_HaltChannel(1);
			before_idx = idx;
		}
		if (!chunk_playing.chunk_1) {
			if (idx >= chunk_list_AOI.size())return;
			Mix_PlayChannel(1, chunk_list_AOI[idx], 0);
			chunk_playing.chunk_1 = 1;
		}
		break;
	default:
		break;
	}
}
int GameData_audio::chrck_chunk_playing() {
	return Mix_Playing(1);
}
void GameData_audio::play_music(int idx) {
	static int befor_idx = -1;
	if (idx == -1) { HaltMusic(); befor_idx = -1; }
	if(befor_idx!=idx){
		if (music_number[idx] - 1 >= 0) {
			bool i = 0;
			if (idx >= music_number.size()) { idx = music_number.size() - 1; i = 1; }
			else i = 0;
			Mix_PlayMusic(music_list[music_number[idx] - 1], -1);
			if(!i)befor_idx = idx;
		}
		else return;
	}
}
void GameData_audio::set_volume_music(int volume) { 
	Mix_VolumeMusic(volume);
}