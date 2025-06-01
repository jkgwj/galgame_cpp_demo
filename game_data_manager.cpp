#include "game_data_manager.h"
//#include "game_data_kyara_futuu.h"
#include<iostream>
#include <io.h>
#include <future>     // �ṩstd::future��std::async
#include<fstream>
#include <filesystem>
#include <mutex>
#include <set>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include<zlib.h>

extern SDL_Renderer* renderer;

using namespace std;
namespace fs = std::filesystem;
//fstream����
std::fstream ioFile;
//extern SceneManager* scene_manager;
struct LoadAudio_idx//�ڲ��ṹ��������Ϸ����ʱ��
{
	std::string name;
	int idx=-1 ;
	bool save=0 ;
	int get_idx() { if (save) { save = 0; return idx; } else { save = 0; return -1; } }
	void check_add(string s) { if (name == s) { save = 1; idx++; } }
};
LoadAudio_idx AMAMIYA_AOI; 

std::mutex sdl_mutex;//������

GameDataManager* GameDataManager::manager = nullptr;
GameDataManager* GameDataManager::instance(SDL_Renderer* _renderer) {
	if (!manager) {
		manager = new GameDataManager(_renderer);
	}
	return manager;
}


		void _read_save_data(fstream& ioFile, SaveData& data) {

			boost::archive::text_oarchive oa(ioFile);
			oa << data.txt_idx;
			oa << data.number;
			oa << data.now_option;
			oa << data.u8_dir;
			oa << data.option_list;

			/*ioFile << data.txt_idx << "\n" << u8": txt_Progress ++" << "\n"
				   << data.now_option << "\n" << u8": now_option ++" << "\n"
				   << data.number << "\n" << u8": number ++" << "\n"
				   << data.u8_dir << "\n" << u8": dir ++" << "\n"
				   << u8": option_record ++" << endl;
			for (int out : data.option_list) {
				   ioFile << out << endl;
			}*/
		}
		namespace ser {
			// ���浥���������ݵ��������ڲ������ʽת����ʹ��Zlibѹ����
			bool SaveTextureToData(SDL_Renderer* renderer, SDL_Texture* texture, TextureData& data, int compressionLevel = Z_DEFAULT_COMPRESSION) {
				std::lock_guard<std::mutex> lock(sdl_mutex);

				int width, height;
				if (SDL_QueryTexture(texture, nullptr, nullptr, &width, &height) != 0) {
					std::cerr << "Query texture size failed: " << SDL_GetError() << std::endl;
					return false;
				}

				// ��ȡ����ԭʼ��ʽ
				Uint32 originalFormat;
				int access;
				if (SDL_QueryTexture(texture, &originalFormat, &access, nullptr, nullptr) != 0) {
					std::cerr << "Query texture format failed: " << SDL_GetError() << std::endl;
					return false;
				}

				// ������ʱ��ȾĿ�꣨ͳһΪRGBA8888��ʽ��
				SDL_Texture* target = SDL_CreateTexture(
					renderer,
					SDL_PIXELFORMAT_RGBA8888,
					SDL_TEXTUREACCESS_TARGET,
					width, height
				);
				if (!target) {
					std::cerr << "Create target texture failed: " << SDL_GetError() << std::endl;
					return false;
				}
				std::cout << "Target texture created successfully." << std::endl;

				// ��������Ŀ�겢��ȡ����
				SDL_SetRenderTarget(renderer, target);
				SDL_RenderClear(renderer);
				if (SDL_RenderCopy(renderer, texture, nullptr, nullptr) != 0) {
					std::cerr << "RenderCopy failed: " << SDL_GetError() << std::endl;
					SDL_DestroyTexture(target);
					return false;
				}
				std::cout << "Texture copied to target successfully." << std::endl;

				// �ٴ���ȷ������ȾĿ��
				if (SDL_SetRenderTarget(renderer, target) != 0) {
					std::cerr << "Failed to set render target before reading pixels: " << SDL_GetError() << std::endl;
					SDL_DestroyTexture(target);
					return false;
				}

				// ������ʱ���棨�̶�32λRGBA��ʽ��
				SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
					0, width, height, 32, SDL_PIXELFORMAT_RGBA8888
				);
				if (!surface) {
					std::cerr << "Create surface failed: " << SDL_GetError() << std::endl;
					SDL_DestroyTexture(target);
					return false;
				}

				// ���Զ�ȡ��������
				if (SDL_RenderReadPixels(
					renderer, nullptr,
					SDL_PIXELFORMAT_RGBA8888,
					surface->pixels, surface->pitch
				) != 0)
				{
					std::cerr << "RenderReadPixels failed: " << SDL_GetError() << std::endl;
					// ����ʹ��ԭʼ��ʽ��ȡ����
					if (SDL_RenderReadPixels(
						renderer, nullptr,
						originalFormat,
						surface->pixels, surface->pitch
					) != 0)
					{
						std::cerr << "RenderReadPixels with original format failed: " << SDL_GetError() << std::endl;
						SDL_FreeSurface(surface);
						SDL_DestroyTexture(target);
						return false;
					}
					else {
						std::cout << "Read pixels with original format successfully." << std::endl;
						// ���ʹ��ԭʼ��ʽ��ȡ�ɹ�������ת��ΪRGBA8888
						SDL_Surface* convertedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
						if (!convertedSurface) {
							std::cerr << "Convert surface format failed: " << SDL_GetError() << std::endl;
							SDL_FreeSurface(surface);
							SDL_DestroyTexture(target);
							return false;
						}
						SDL_FreeSurface(surface);
						surface = convertedSurface;
					}
				}

				// ��ӡ��ȡ���������ݴ�С��������֤
				std::cout << "Read pixel data size: " << surface->pitch * surface->h << std::endl;

				// ��ӡ��������ֵ������֤
				if (surface->pitch * surface->h >= 4) {
					uint8_t* pixels = static_cast<uint8_t*>(surface->pixels);
					std::cout << "First pixel (RGBA) read: "
						<< static_cast<int>(pixels[0]) << ", "
						<< static_cast<int>(pixels[1]) << ", "
						<< static_cast<int>(pixels[2]) << ", "
						<< static_cast<int>(pixels[3]) << std::endl;
				}

				// ׼��Zlibѹ��
				uLong sourceLen = surface->pitch * surface->h;
				uLong destLen = compressBound(sourceLen);
				data.compressedPixels.resize(destLen);

				// ʹ��Zlib����ѹ��
				int result = compress2(data.compressedPixels.data(), &destLen, static_cast<const Bytef*>(surface->pixels), sourceLen, compressionLevel);
				if (result != Z_OK) {
					std::cerr << "Zlib compression failed: " << result << std::endl;
					SDL_FreeSurface(surface);
					SDL_DestroyTexture(target);
					return false;
				}

				// ����ѹ�������ݵĴ�С
				data.compressedPixels.resize(destLen);

				// ������ݣ�ͳһ��ʽ��
				data.width = width;
				data.height = height;
				data.pitch = surface->pitch;

				// ��֤���ƺ���������ݴ�С
				std::cout << "Compressed pixel data size: " << data.compressedPixels.size() << std::endl;

				// ������Դ
				SDL_FreeSurface(surface);
				SDL_DestroyTexture(target);
				return true;
			}

			// �����ݴ�������ͳһʹ��RGBA8888��ʽ��
			SDL_Texture* LoadTextureFromData(SDL_Renderer* renderer, const TextureData& data) {
				std::lock_guard<std::mutex> lock(sdl_mutex);

				std::cout << "Loading texture: width=" << data.width << ", height=" << data.height << ", pitch=" << data.pitch << ", compressed pixel size=" << data.compressedPixels.size() << std::endl;

				// ��ӡ����ѹ��������ֵ������֤
				if (data.compressedPixels.size() >= 4) {
					std::cout << "Loaded first compressed pixel: "
						<< static_cast<int>(data.compressedPixels[0]) << ", "
						<< static_cast<int>(data.compressedPixels[1]) << ", "
						<< static_cast<int>(data.compressedPixels[2]) << ", "
						<< static_cast<int>(data.compressedPixels[3]) << std::endl;
				}

				// ��ѹ����������
				uLong sourceLen = data.compressedPixels.size();
				uLong destLen = data.pitch * data.height;
				std::vector<unsigned char> decompressedPixels(destLen);

				int result = uncompress(decompressedPixels.data(), &destLen, data.compressedPixels.data(), sourceLen);
				if (result != Z_OK) {
					std::cerr << "Zlib decompression failed: " << result << std::endl;
					return nullptr;
				}

				// ��ӡ��ѹ������������ݴ�С
				std::cout << "Decompressed pixel data size: " << destLen << std::endl;

				// ��ӡ���ֽ�ѹ��������ֵ������֤
				if (destLen >= 4) {
					std::cout << "Loaded first decompressed pixel (RGBA): "
						<< static_cast<int>(decompressedPixels[0]) << ", "
						<< static_cast<int>(decompressedPixels[1]) << ", "
						<< static_cast<int>(decompressedPixels[2]) << ", "
						<< static_cast<int>(decompressedPixels[3]) << std::endl;
				}

				// �����ɸ��µ�����
				SDL_Texture* texture = SDL_CreateTexture(
					renderer,
					SDL_PIXELFORMAT_RGBA8888,
					SDL_TEXTUREACCESS_STREAMING,
					data.width, data.height
				);
				if (!texture) {
					std::cerr << "Create texture failed: " << SDL_GetError() << std::endl;
					return nullptr;
				}

				// �����������ݣ��ؼ����裬ȷ��pitch��ȷ��
				if (SDL_UpdateTexture(
					texture, nullptr,
					decompressedPixels.data(), data.pitch
				) != 0)
				{
					std::cerr << "UpdateTexture failed: " << SDL_GetError() << std::endl;
					SDL_DestroyTexture(texture);
					return nullptr;
				}

				// ���������͸����Ϊ��͸��
				if (SDL_SetTextureAlphaMod(texture, 255) != 0) {
					std::cerr << "SDL_SetTextureAlphaMod failed: " << SDL_GetError() << std::endl;
				}

				// ������ɫ����
				if (SDL_SetTextureColorMod(texture, 255, 255, 255) != 0) {
					std::cerr << "SDL_SetTextureColorMod failed: " << SDL_GetError() << std::endl;
				}

				// ���û��ģʽ
				if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) != 0) {
					std::cerr << "SDL_SetTextureBlendMode failed: " << SDL_GetError() << std::endl;
				}

				return texture;
			}
		}//ser

		// ���л��������������������ļ�
		bool serializeTextures(const std::vector<SDL_Texture*>& textures,
			SDL_Renderer* renderer,
			const std::string& filename) {
			std::vector<TextureData> textureDataList;

			// �ռ�������������
			for (auto tex : textures) {
				TextureData data;
				if (!ser::SaveTextureToData(renderer, tex, data)) {
					std::cerr << "Failed to save texture data" << std::endl;
					return false;
				}
				textureDataList.push_back(data);
			}

			// ���л������������ļ�
			try {
				std::ofstream ofs(filename, std::ios::binary);
				boost::archive::binary_oarchive oa(ofs);
				oa << textureDataList;
				return true;
			}
			catch (const std::exception& e) {
				std::cerr << "Serialization error: " << e.what() << std::endl;
				return false;
			}
		}

		// �ӵ����ļ������л���������
		std::vector<SDL_Texture*> deserializeTextures(SDL_Renderer* renderer,
			const std::string& filename) {
			std::vector<TextureData> textureDataList;
			std::vector<SDL_Texture*> loadedTextures;

			// �����л�����
			try {
				std::ifstream ifs(filename, std::ios::binary);
				boost::archive::binary_iarchive ia(ifs);
				ia >> textureDataList;
			}
			catch (const std::exception& e) {
				std::cerr << "Deserialization error: " << e.what() << std::endl;
				return {};
			}

			// �ؽ���������
			for (const auto& data : textureDataList) {
				SDL_Texture* tex = ser::LoadTextureFromData(renderer, data);
				if (tex) {
					loadedTextures.push_back(tex);
				}
				else {
					std::cerr << "Failed to load a texture from data" << std::endl;
					// �����Ѽ��ص�����
					for (auto t : loadedTextures) SDL_DestroyTexture(t);
					loadedTextures.clear();
					break;
				}
			}

			return loadedTextures;
		}



bool GameDataManager::load_game_data(SDL_Renderer* renderer) {
	AMAMIYA_AOI.name = u8"�깬��";

	ioFile.open("resources\\txt\\�籾_��.txt", std::ios::in | std::ios::out);
	if (!ioFile.is_open()) {
		std::cout << u8"·��" << u8"resources\\txt\\�籾_��.txt" << u8"û�ҵ������ļ�" << std::endl;
		return 0;
	}
	int number=0;
	string dump_str,del_str;
	string txt_data;
	//�ı�����
	while (getline(ioFile, dump_str)) {
		if (dump_str.empty() || dump_str.find_first_not_of(' ') == std::string::npos)continue;
		if (dump_str.find(u8"��") != string::npos&& dump_str.find(u8"��") != string::npos) {
			dump_str.erase((int)dump_str.find(u8"��"), (int)(dump_str.find(u8"��") - dump_str.find(u8"��")+3));
		}
		if (dump_str.empty() || dump_str.find_first_not_of(' ') == std::string::npos)continue;
		if(dump_str.find(u8"/////")==string::npos)number++;
		_txt_load(dump_str,txt_data,number);
		std::cout << dump_str << endl;
	}
	ioFile.close();
	bin_save_txt_data(u8"resources\\txt\\txt.bin");

	//img����
	_img_load(u8"resources\\image\\BG", u8".png", data_img_list, renderer);
	_img_load(u8"resources\\image\\CG", u8".png", data_img_list, renderer);
	_img_load(u8"resources\\image\\menu",u8".png", data_img_menu_list, renderer);
	if(serializeTextures(data_img_list,renderer,u8"resources\\image.dat"))
		std::cout<<u8"���л��ɹ�";
	if(serializeTextures(data_img_menu_list, renderer, u8"resources\\menu.dat"))
		std::cout << u8"���л��ɹ�";
	/*vector<std::string> paths = { u8"resources\\audio\\game_bgm",u8"resources\\audio\\menu" };
	_data_save_music(paths);
	paths.clear(); paths.resize(0);
	paths = { u8"resources\\audio\\AOI" };
	_data_save_chunk(paths);*/
	//��Ƶ��Դ���л�
	serializeAudioDirectory(u8"resources\\audio\\game_bgm", u8"resources\\oto\\game.BGM");
	serializeAudioDirectory(u8"resources\\audio\\menu", u8"resources\\oto\\menu.BGM");
	serializeAudioDirectory(u8"resources\\audio\\AOI", u8"resources\\oto\\kyara.AOI");

	//���޸�
//	//������Դ����
//	data_kyara_list.emplace_back(new GameData_kyara_futuu());
//	
//	for (GameData_kyara* kyara : data_kyara_list) {
//		kyara->on_init(DataLoadMode::MOD_FUTUU,KyaracterName::AOI);
//	//����������Դ���л�
//		std::vector<KyaraNode*> kyara_data = kyara->get_kyara_data();
//		bin_save_kyara_data_link(u8"resources\\kyara\\link\\kyara.link", kyara_data);
//		for (KyaraNode* node : kyara_data) {
//			serializeTextures(node->tex_list, renderer, u8"resources\\kyara\\AOI");
//		}
//
//}

	std::cout << endl;
	std::cout << txt_data<<endl;
	cout << data_txt_list.size() << endl;
	cout << data_img_list.size() << endl;
	cout << data_img_menu_list.size() << endl;
	cout << data_kernel.size() << endl;

	_upsavedata_sys(-1);
	_upoptiondata();
	return 1;
}

bool GameDataManager::bin_load_game_data(SDL_Renderer* renderer) {
	bin_load_txt_data(u8"resources\\txt\\txt.bin");
	//img����
	/*_img_load(u8"resources\\image\\BG", u8".png", data_img_list, renderer);
	_img_load(u8"resources\\image\\CG", u8".png", data_img_list, renderer);
	_img_load(u8"resources\\image\\menu", u8".png", data_img_menu_list, renderer);*/
	data_img_list = deserializeTextures(renderer, u8"resources\\image.dat");
		
	data_img_menu_list = deserializeTextures(renderer, u8"resources\\menu.dat");
	
	std::cout << endl;
	//std::cout << txt_data << endl;
	cout << data_txt_list.size() << endl;
	cout << data_img_list.size() << endl;
	cout << data_img_menu_list.size() << endl;
	cout << data_kernel.size() << endl;

	_upsavedata_sys(-1);
	_upoptiondata();
	return 1;
}
void GameDataManager::get_option_start_number(int& start, int& end) {
	if (option_number_idx >= option_llist.size() - 1) {
		start = option_llist[option_llist.size() - 1][0]->option.txt_start;
		end = option_llist[option_llist.size() - 1][option_llist[option_llist.size() - 1].size() - 1]->option.txt_end;
		option_number_idx = option_llist.size() - 1;
		return;
	}
	start = option_llist[option_number_idx][0]->option.txt_start;
	end = option_llist[option_number_idx][option_llist[option_number_idx].size()-1]->option.txt_end;
}
int GameDataManager::get_last_option_record(int &bbefor_end) {
	int i = 0;
	if (option_record.size() >= 2) {
		i = option_record.size() - 2;
		bbefor_end = option_llist[i][option_llist[i].size() - 1]->option.txt_end;
	}
	else bbefor_end = 0;
     if(option_record.size()>=1)return option_record[option_record.size()-1];
	else return -1;
}
void GameDataManager::record_option(int option_idx) {
	option_record.push_back(option_idx);
}
int  GameDataManager::jmp_option(int option_idx) {
	_jmp_txt_number(option_llist[option_number_idx-1][option_idx]->option.txt_start);
	if (option_llist[option_number_idx - 1][option_idx]->option.txt_start == option_llist[option_number_idx - 1][option_idx]->option.txt_end)
	{
		_jmp_txt_number(option_llist[option_number_idx - 1][option_llist[option_number_idx - 1].size() - 1]->option.txt_end); return -1;
	}
	return option_llist[option_number_idx - 1][option_idx]->option.txt_end;
}
void GameDataManager::jmp_txt_number(int number) {
	_jmp_txt_number(number);
}
bool GameDataManager::_txt_load(string &str,string &txt_data,int _number) {
	string dump=str,name=u8"";
	static bool option = 0;
	static bool temp_t = 0, temp_i = 0, start = 0;
	static int option_number = 0;
	static int CG_number_idx = 0;
	static int MUSIC_number_idx = 0;
	//bool jmp = 0;
	if (str.find(u8"++����") != string::npos) {
		MUSIC_number_idx++;
	}
	else if (str.find(u8"����") != string::npos)
	{
		name = str.substr(0, (int)str.find(u8"����"));
		dump = str.substr((int)(str.find(u8"����") + 3), (int)(str.find(u8"��") - str.find(u8"����")));
		//std::cout <<endl<< u8"�ҵ��Ի��ı�" << endl;
		AMAMIYA_AOI.check_add(name);
		GameData_txt* dump_txt = new GameData_txt(data_txt_list.size(),0 , MUSIC_number_idx, name, dump, AMAMIYA_AOI.get_idx());
		dump_txt->set_CG_number(CG_number_idx);
		data_txt_list.push_back(dump_txt);
		if (option)data_txt_list[data_txt_list.size() - 1]->set_option(option_number);
		if (temp_t && option && start) {option_list[option_list.size() - 1]->option.txt_start = data_txt_list.size(); temp_t = 0; }
		
		txt_data += dump + u8"\n";
	}
	else if (str.find(u8"/////") != string::npos)
	{
		DataLink data_link(data_txt_list.size(), 0);
		data_kernel.push_back(data_link);
		data_txt_list[data_txt_list.size()-1]->set_CG_jmp(1);
		CG_number_idx++;
	}
	else if (str.find(u8"+++++++") != string::npos) {
		option = !option; if (!option) {
			option_llist.push_back(option_list); option_list.resize(0); option_number = 0; }
	}
	
	else {
		AMAMIYA_AOI.check_add(name);
		GameData_txt* dump_txt = new GameData_txt(data_txt_list.size(),0 , MUSIC_number_idx, name, dump, AMAMIYA_AOI.get_idx());
		dump_txt->set_CG_number(CG_number_idx);
		data_txt_list.push_back(dump_txt);
		if (option)data_txt_list[data_txt_list.size() - 1]->set_option(option_number);
		if (temp_t && option && start) {option_list[option_list.size() - 1]->option.txt_start = data_txt_list.size(); temp_t = 0; }
		
		txt_data += dump+u8"\n";
	}
	if (option && str.find(u8"������") != string::npos) { 
		 if (option) { temp_t = 1; temp_i = 1; start = 1; }
		option_number++; 
		Option* temp_option=new Option();
		string t = u8"��";
		temp_option->str = str.substr((int)str.find(u8"��") + 9, str.find(u8"/��") - ((int)str.find(u8"��") + 9));
		option_list.push_back(temp_option);
		cout << temp_option->str << endl;
	}
	if (option && str.find(u8"#############") != string::npos) {
		
		option_list[option_list.size() - 1]->option.txt_end = data_txt_list.size();
	}
	if (str.find(u8"#END") != string::npos) {
		if (str.find(u8"1") != string::npos|| str.find(u8"2") != string::npos)
			_END_list.push_back(new _END(data_txt_list.size()-1,1));
	}
	return 1;
}
bool GameDataManager::_img_load(string path, string fileType,vector<SDL_Texture*> &_data_img_list,SDL_Renderer* renderer) {
	vector<string> files;
	intptr_t  hFile = 0;// �ļ����
	struct _finddata_t fileInfo;// �ļ���Ϣ
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileInfo)) != -1) {
		do {
			// �����ļ���ȫ·��
			files.push_back(p.assign(path+"\\").append(fileInfo.name));
		} while (_findnext(hFile, &fileInfo) == 0);  //Ѱ����һ�����ɹ�����0������-1
		_findclose(hFile);
	}
	SDL_Texture* tex_img=nullptr;
	for (int i = 0; i < files.size(); i++) {
		tex_img = IMG_LoadTexture(renderer, files[i].c_str());
		
		_data_img_list.push_back(tex_img);
	}
	
	return 1;
}
SDL_Texture* GameDataManager::get_menu_data(int i) {
	return data_img_menu_list[i];
}
void GameDataManager::get_END_idx(std::vector<int> &int_list) {
	for (_END* end : _END_list) {
		int_list.push_back(end->txt_idx);
	}
}

SDL_Texture* GameDataManager::get_CG_data(int CG_num_idx) {
	return data_img_list[CG_info_idx[CG_num_idx]-1];
}
GameData_txt* GameDataManager::get_txt_data(bool is_next,bool seting,int& _dump_txt, bool cover = 0,int _idx=-1) {
	static int dump_txt_progress = 0;
	if (cover)dump_txt_progress = _dump_txt;
	if (_dump_txt == -3 && _idx != -1) {
		return data_txt_list[_idx];
	}
	if (_dump_txt == -1) {
		_dump_txt = txt_progress-1;
		
		return 0;
	}
	if (txt_next) {
		
		txt_progress++;
		txt_next = 0;
	}
	
	if (txt_progress - 1 < 0)txt_progress = 1;
	
	return data_txt_list[txt_progress-1];
}
GameData_txt* GameDataManager::get_txt_data(int idx) {
	return data_txt_list[idx];
}
void GameDataManager::set_txt_next(bool flag) {
	txt_next = flag;
}
void GameDataManager::load_savedata(bool updata=0) {
	fs::path path_str(u8"SAVEDATA\\");
	fs::create_directory(path_str); //��Ŀ¼������ʱ����Ŀ¼
		if (!exists(path_str))		//�����ȼ��Ŀ¼�Ƿ���ڲ���ʹ���ļ����.
			return ;
		fs::directory_entry entry(path_str);		//�ļ����
		if (entry.status().type() == fs::file_type::directory)	//��������C++11��ǿö������
			std::cout << u8"��·����һ��Ŀ¼" << std::endl;
		fs::directory_iterator list(path_str);	        //�ļ��������
		vector<string> name_str;
		for (auto& it : list){
			if (it.path().extension().u8string() != u8".dat")continue;
			name_str.push_back(it.path().filename().u8string());
		}
		if (updata) { save_data_list.clear(); save_data_list.resize(0); }
		
		for (size_t i = 0; i < name_str.size(); i++)
		{
			string open_dir=path_str.u8string() + name_str[i];
			string dump_str,before_str;
			SaveData data;
			ioFile.open(open_dir, std::ios::in);
		    if (!ioFile.is_open()) {
			  std::cout << u8"·��" << open_dir << u8"û�ҵ������ļ�" << std::endl;
			  return;
		    }
			boost::archive::text_iarchive ia(ioFile);
			ia >> data.txt_idx;
			ia >> data.number;
			ia >> data.now_option;
			ia >> data.u8_dir;
			ia >> data.option_list;
			
			save_data_list.emplace_back(data);
			ioFile.close();
		}
		name_str.clear();
}
void GameDataManager::save(int _number) {
	fs::path path_str(u8"SAVEDATA\\");
	fs::create_directory(path_str); //��Ŀ¼������ʱ����Ŀ¼
	if (!exists(path_str))		//�����ȼ��Ŀ¼�Ƿ���ڲ���ʹ���ļ����.
		return;
	for (SaveData savedata : save_data_list) {
		if (savedata.number == _number)
			if (remove(savedata.u8_dir.c_str()) == 0)
			{
				cout << u8"ɾ���ɹ�" << endl;
			}
			else continue;
	}
	string dir = (u8"SAVEDATA\\save_" + std::to_string(_number));
		dir += u8".dat";
		//���������½��ļ�
		ioFile.open(dir, ios::app| ios::out );
		if (!ioFile.is_open()) {

			cout << u8"����" << dir << u8"ʧ��" << std::endl;
			return;
		}
		int p = -1;
		bool _temp = 0;

		SaveData save_data;
		
		
		if(txt_progress-1>=0&&manager->get_txt_data(txt_progress-1)->check_no_render(-2)==3)
		save_data.txt_idx = txt_progress+1;
		else save_data.txt_idx = txt_progress;
		save_data.option_list = option_record;
		save_data.now_option = (option_number_idx - 1>0? option_number_idx - 1:0);
		save_data.number =  _number;
		save_data.u8_dir = dir;
		_read_save_data(ioFile, save_data);
		ioFile.close();
		manager->load_savedata(1);
}
void GameDataManager::load(int _number,bool &_is_next) {
	
	
	for (SaveData savedata : save_data_list) {
		if (savedata.number == _number) {
			txt_progress = savedata.txt_idx;
			option_number_idx = (savedata.now_option  >= 0 ? savedata.now_option  : 0);
			option_record.clear();
			option_record.resize(0);
			option_record = savedata.option_list;
			if (data_txt_list[txt_progress]->check_no_render(-1) == 2)txt_progress--;
			return;
		}
	}

}
GameData_txt* GameDataManager::get_savedata(int _number) {
	for (SaveData savedata : save_data_list) {
		if (savedata.number ==  _number) {
			return data_txt_list[savedata.txt_idx-1];
		}
	}
	return 0;
}
SDL_Texture* GameDataManager::load_texture(std::string dir) {
   SDL_Texture*	tex_img = IMG_LoadTexture(renderer, dir.c_str());
   return tex_img;
}
bool GameDataManager::check_txt_data_cgjmp(int idx) {
	return data_txt_list[idx]->get_CG_jmp();
}
void GameDataManager::data_debaug(bool &debug) {
	std::string dst_str,befor_str;
	std::cout << u8"��ǰ����Ϸ�е����֣�" << data_txt_list[txt_progress-1]->get_string() << std::endl;
	std::cout << u8"�������޸ĺ�����֣��ɸ���ǰ�����΢�ģ���" << std::endl;
	std::cin >> dst_str;
	befor_str = data_txt_list[txt_progress - 1]->get_string();
	int i = 0;
	std::cout << u8"\n������1ȷ���޸�Ϊ��" << dst_str<<std::endl; cin >> i;


	string DIR = u8"resources\\txt\\temp.txt";
	ioFile.open(DIR, ios::app | ios::out);
	if (!ioFile.is_open()) {
		cout << u8"���ļ�·��" << u8"\\resources\\txt\\temp.txt" << u8"ʧ��" << endl;
		return ;
	}

	data_txt_list[txt_progress-1]->set_string(dst_str);
	
	ioFile << u8"ԭ�䣺" << befor_str << u8"\n�ĺ�" << data_txt_list[txt_progress-1]->get_string()<<std::endl;
	cout << u8"\nԭ�䣺" << befor_str << u8"\n�ĺ�" << data_txt_list[txt_progress-1]->get_string()<<std::endl<<u8"����1��ȷ�����޸�\n";
	cin >> i;
	if (i == 1) { cout << u8"�޸��Ѿ�ȷ��\n"; debug = 0; }
	ioFile.close();
}
void GameDataManager::reset_progress() {
	txt_progress = 0;
	this->option_record.clear();
	this->option_record.resize(0);
	option_number_idx=0;
}
void GameDataManager::clear_record(KaracterName name) {
	game_record.zhoumu++;
	switch (name)
	{
	case AOI:
		game_record.clear_aoi = 1;
	default:
		break;
	}
}
void GameDataManager::_upsavedata_sys(int state=0){
	fs::path path_str(u8"SAVEDATA\\");
	fs::create_directory(path_str); //��Ŀ¼������ʱ����Ŀ¼
	if (!exists(path_str))		//�����ȼ��Ŀ¼�Ƿ���ڲ���ʹ���ļ����.
		return;
	string dir = (u8"SAVEDATA\\savedata");
	dir += u8".sys";
	if (std::filesystem::exists(dir)) {
		if (remove(dir.c_str()) == 0)
		{
			cout << u8"savedata.sysɾ���ɹ�" << endl;
		}
	}
	//���������½��ļ�
	ioFile.open(dir, ios::app | ios::out);
	if (!ioFile.is_open()) {

		cout << u8"��" << dir << u8"ʧ��" << std::endl;
		return;
	}
	for (GameData_txt* txt: data_txt_list)
	{
		txt->out_txt_data(ioFile);
	}
	ioFile.close();
}
void GameDataManager::_load_option_data(std::vector<Option*> _option_list) {
	SDL_FRect frect = { 250,200,780,50 };
	for (Button_txt* button : next_option_button_list) { 
		delete(button);
		 }
	next_option_button_list.clear();
   next_option_button_list.resize(0);
	for (Option* op : _option_list)
	{
		
		Button_txt* temp = new Button_txt(frect, "resources\\ttf\\simfang.ttf", op->str, { 215,215,215,200 }, { 115,115,115,100 }, { 185,185,185,210 }, frect.w, 32);
		next_option_button_list.push_back(temp);
		frect.y += 60;
	}
	option_number_idx++;
}
std::vector<Button_txt*>* GameDataManager::get_option_data() {

		_load_option_data(option_llist[option_number_idx]);
		return &next_option_button_list;
	
}
void GameDataManager::_jmp_txt_number(int number) {
	txt_progress = number;
}
void GameDataManager::_upoptiondata() {
	static Option* option = option_llist[option_llist.size()-1][option_llist[option_llist.size() - 1].size()-1];
	if (game_record.zhoumu > 1)
		option_llist[2].push_back(option);
	else if(option_llist[2].size()>1)
		option_llist[2].pop_back();
}
void GameDataManager::read_game_record(int end_idx) {
	for (_END* end : _END_list) {
		if (end->txt_idx == end_idx) { game_record.zhoumu++; game_record.clear_aoi = 1;_upoptiondata(); option_sync = 1; return; }
	}
	
}
int  GameDataManager::get_game_record() {
	return game_record.zhoumu;
}


//���л��뷴���л�

bool serialize_Textures(const std::vector<SDL_Texture*>& textures, const std::string& dir) {
	serializeTextures(textures, renderer, dir);
}
std::vector<SDL_Texture*> deserialize_Textures(SDL_Renderer* renderer, const std::string& dir) {
	deserializeTextures(renderer, dir);
}
//���л��ı�
void GameDataManager::bin_save_txt_data(const std::string& dir) {//�ı���Դ���л�
	try {
		std::ofstream ofs(dir);
		if (!ofs.is_open()) {
			throw std::runtime_error(u8"�޷����ļ�����д��: " + dir);
		}
		boost::archive::text_oarchive oa(ofs);
		oa << data_txt_list;
		oa << option_llist;
		oa << _END_list;
		ofs.close();
	}
	catch (const std::exception& e) {
		std::cerr << u8"�����ļ�ʱ����: " << e.what() << std::endl;
	}
}

//�����л��ı�
void GameDataManager::bin_load_txt_data(const std::string& dir) {
	try {
		std::ifstream ifs(dir);
		if (!ifs.is_open()) {
			throw std::runtime_error(u8"�޷����ļ����ж�ȡ: " + dir);
		}
		boost::archive::text_iarchive ia(ifs);
		ia >> data_txt_list;
		ia >> option_llist;
		ia >> _END_list;
		ifs.close();
	}
	catch (const std::exception& e) {
		std::cerr << u8"�����ļ�ʱ����: " << e.what() << std::endl;
	}
}

static std::mutex serializationMutex; // �����߳�ͬ���ľ�̬������

/**
 * ��ȡ�����ļ���AudioFile�ṹ
 */
static bool readFile(const std::string& path, AudioFile& outFile) {
	std::ifstream file(path, std::ios::binary);
	if (!file) return false;

	outFile.filename = fs::path(path).filename().string();
	outFile.data = std::vector<Uint8>(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>()
	);
	return true;
}
//���߳�
//bool GameDataManager::serializeAudioDirectory(const std::string& dirPath, const std::string& extension, const std::string& outputPath) {
//	std::cout << u8"[���л�] ��ʼ����Ŀ¼: " << dirPath
//		<< u8", ��չ��ɸѡ: " << extension << std::endl;
//
//	std::vector<AudioFile> audioFiles;
//	std::vector<std::future<void>> futures;
//	std::mutex fileMutex;
//	int fileCount = 0;
//
//	try {
//		// �г�Ŀ¼�е������ļ�
//		std::cout << u8"[���л�] Ŀ¼����:" << std::endl;
//		for (const auto& entry : fs::directory_iterator(dirPath)) {
//			std::cout << u8"  - " << entry.path() << std::endl;
//		}
//
//		// ����Ŀ¼�е������ļ�
//		for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
//			if (entry.is_regular_file() && entry.path().extension() == extension) {
//				fileCount++;
//				// ʹ��ֵ����entry���������ò����µ�����
//				futures.push_back(std::async(std::launch::async, [entry = entry, &audioFiles, &fileMutex, fileCount]() {
//					std::cout << u8"[�߳� " << std::this_thread::get_id()
//						<< u8"] ��ʼ��ȡ�ļ� #" << fileCount << ": "
//						<< entry.path() << std::endl;
//
//					AudioFile audioFile;
//					if (readFile(entry.path().string(), audioFile)) {
//						std::lock_guard<std::mutex> lock(fileMutex);
//						std::cout << u8"[�߳� " << std::this_thread::get_id()
//							<< u8"] ������ļ�: " << audioFile.filename << std::endl;
//						audioFiles.push_back(std::move(audioFile));
//					}
//					}));
//			}
//		}
//
//		std::cout << u8"[���л�] �ȴ������ļ���ȡ���..." << std::endl;
//
//		// �ȴ������첽�������
//		for (auto& future : futures) {
//			future.wait();
//		}
//
//		std::cout << u8"[���л�] �Ѷ�ȡ " << audioFiles.size()
//			<< u8" ���ļ�����ʼд��������ļ�: " << outputPath << std::endl;
//
//		std::ofstream ofs(outputPath, std::ios::binary);
//		if (!ofs) {
//			std::cout << u8"[���л�] ����: �޷�������ļ�: " << outputPath << std::endl;
//			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open output file: %s", outputPath.c_str());
//			return false;
//		}
//
//		boost::archive::binary_oarchive oa(ofs);
//		oa << audioFiles;
//
//		std::cout << u8"[���л�] �ɹ�: �ѽ� " << audioFiles.size()
//			<< u8" ���ļ����л��� " << outputPath << std::endl;
//
//		return true;
//
//	}
//	catch (const std::exception& e) {
//		std::cout << u8"[���л�] �쳣: " << e.what() << std::endl;
//		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Serialization error: %s", e.what());
//		return false;
//	}
//}


std::vector<std::unique_ptr<AudioStorage>> storages; // �洢���з����л��Ự

// ------------------------- ���л����� -------------------------
bool GameDataManager::serializeAudioDirectory(const std::string& dirPath, const std::string& outputPath) {
	std::vector<AudioFile> audioFiles;
	const std::set<std::string> supportedExtensions = {
		".mp3", ".wav", ".ogg", ".midi", ".mid", ".mod", ".flac"
	};

	for (const auto& entry : fs::directory_iterator(dirPath)) {
		if (!entry.is_regular_file()) continue;

		std::string ext = entry.path().extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		if (supportedExtensions.count(ext)) {
			AudioFile audioFile;
			if (readFile(entry.path().string(), audioFile)) {
				audioFiles.push_back(audioFile);
			}
		}
	}

	std::ofstream ofs(outputPath, std::ios::binary);
	if (!ofs) return false;

	boost::archive::binary_oarchive oa(ofs);
	oa << audioFiles;

	std::cout << u8"[���л�] �ɹ�: �ѽ� " << audioFiles.size()
		<< u8" ���ļ����л��� " << outputPath << std::endl;
	return true;
}

// ------------------------- �����л�Music���� -------------------------
std::vector<Mix_Music*> GameDataManager::deserializeAudioToMusic(const std::string& inputPath) {
	auto storage = std::make_unique<AudioStorage>();
	std::vector<Mix_Music*> currentMusicList;
	std::ifstream ifs(inputPath, std::ios::binary);
	if (!ifs) return currentMusicList;

	try {
		boost::archive::binary_iarchive ia(ifs);
		std::vector<AudioFile> audioFiles;
		ia >> audioFiles;

		for (const auto& audioFile : audioFiles) {
			Uint8* alignedData = static_cast<Uint8*>(SDL_malloc(audioFile.data.size()));
			if (!alignedData) continue;

			memcpy(alignedData, audioFile.data.data(), audioFile.data.size());
			storage->alignedAudioData.emplace_back(alignedData, SDL_free);

			SDL_RWops* rw = SDL_RWFromConstMem(alignedData, audioFile.data.size());
			if (!rw) continue;

			Mix_Music* music = Mix_LoadMUS_RW(rw, 1);
			if (music) {
				currentMusicList.push_back(music);
				std::cout << "Loaded music: " << audioFile.filename << "\n";
			}
			else {
				std::cerr << "Failed to load: " << audioFile.filename
					<< " - " << Mix_GetError() << "\n";
			}
		}

		storage->musicList = currentMusicList;
		storages.push_back(std::move(storage));
	}
	catch (...) {
		std::cerr << "Deserialization failed!\n";
	}

	return currentMusicList;
}

// ------------------------- �����л�Chunk���� -------------------------
std::vector<Mix_Chunk*> GameDataManager::deserializeAudioToChunk(const std::string& inputPath) {
	auto storage = std::make_unique<AudioStorage>();
	std::vector<Mix_Chunk*> currentChunkList;
	std::ifstream ifs(inputPath, std::ios::binary);
	if (!ifs) return currentChunkList;

	try {
		boost::archive::binary_iarchive ia(ifs);
		std::vector<AudioFile> audioFiles;
		ia >> audioFiles;

		for (const auto& audioFile : audioFiles) {
			Uint8* alignedData = static_cast<Uint8*>(SDL_malloc(audioFile.data.size()));
			if (!alignedData) continue;

			memcpy(alignedData, audioFile.data.data(), audioFile.data.size());
			storage->alignedAudioData.emplace_back(alignedData, SDL_free);

			SDL_RWops* rw = SDL_RWFromConstMem(alignedData, audioFile.data.size());
			Mix_Chunk* chunk = Mix_LoadWAV_RW(rw, 1);
			if (chunk) {
				currentChunkList.push_back(chunk);
			}
			else {
				std::cerr << "Failed to load chunk: " << audioFile.filename
					<< " - " << Mix_GetError() << "\n";
			}
		}

		storage->chunkList = currentChunkList;
		storages.push_back(std::move(storage));
	}
	catch (...) {
		std::cerr << "Deserialization failed!\n";
	}

	return currentChunkList;
}

//���л�������Դ
void GameDataManager::bin_save_kyara_data_link(const std::string& dir,std::vector<KyaraNode*> data) {
	try {
		std::ofstream ofs(dir);
		if (!ofs.is_open()) {
			throw std::runtime_error(u8"�޷����ļ�����д��: " + dir);
		}
		boost::archive::text_oarchive oa(ofs);
		oa << data;
		ofs.close();
	}
	catch (const std::exception& e) {
		std::cerr << u8"����kyara_link�ļ�ʱ����: " << e.what() << std::endl;
	}
}

//�����л�������Դ
void GameDataManager::bin_load_kyara_data_link(const std::string& dir, std::vector<KyaraNode*> data) {
	try {
		std::ifstream ifs(dir);
		if (!ifs.is_open()) {
			throw std::runtime_error(u8"�޷����ļ����ж�ȡ: " + dir);
		}
		boost::archive::text_iarchive ia(ifs);
		ia >> data;
		ifs.close();
	}
	catch (const std::exception& e) {
		std::cerr << u8"����kyara_link�ļ�ʱ����: " << e.what() << std::endl;
	}
}