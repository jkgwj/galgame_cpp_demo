#pragma once
#include"data_tool.h"
#include"game_data_txt.h"
#include"game_data_kyara.h"
//#include"game_data_kyara_futuu.h"
#include"game_data_audio.h"
#include"button_txt.h"


class GameDataManager
{
	friend class boost::serialization::access;
private:

	std::vector<Option*> option_list;//�����Ϸ��ԴҪ��
	std::vector< std::vector<Option*>> option_llist;
	SDL_Renderer* renderer;
	static GameDataManager* manager;
	std::vector<DataLink> data_kernel;//CG/BG��������ת���¼
	std::vector<GameData_txt*> data_txt_list;//��Ϸ�籾��Դ
	std::vector<SDL_Texture*> data_img_list;//��Ϸimg��Դ
	std::vector<SDL_Texture*> data_img_menu_list;//��Ϸ���˵���Դ
	std::vector <GameData_kyara*> data_kyara_list;//��Ϸ������Դ
	std::vector<Button_txt*> next_option_button_list;//��һ��ѡ����Դ
	std::vector<int> option_record;//ѡ���֧��¼
	std::vector<SaveData> save_data_list;

	int txt_progress = 0;//�ı�����
	struct GameRecord //��Ϸ���ݼ�¼
	{
		int zhoumu = 1;//��Ŀ��
        //ͨ�ص�����
		bool clear_aoi = 0;//��
	};
	GameRecord game_record;
	
	std::vector<_END*> _END_list;
	
    int option_number_idx = 0;
	
    bool CG_next = 0;
	bool txt_next = 0;


	std::vector<int> CG_info_idx= { 1,1,2,5,1,2,3,6,2,3,
					                2,3,4,5,2,1,2,3,4,5,
					                4,5,3,4,1,2,2,2,2,4,
		                            2,3,2,4,5,6,5,2,3,4,
	                                5,6,5,4,3,2,1,2,3,4,
		                            5,6,1,5,2,6,5,6,3,5,
	                                6,5,4,5,6,3,2,3,2,1,
		                            6,4};

	bool _txt_load(std::string& str, std::string& txt_data, int number);
	bool _img_load(std::string path,  std::string fileType, std::vector<SDL_Texture*> &game_data_list, SDL_Renderer* renderer);
	void _upsavedata_sys(int state);
	void _load_option_data(std::vector<Option*> _option_list);
	void _jmp_txt_number(int number);
	void _upoptiondata();
	void _data_save_music(std::vector<std::string>& paths);
	void _data_save_chunk(std::vector<std::string>& paths);
public:					  
	bool option_sync = 0;
	bool rectify = 0;
	bool CG_JMP = 0;

	static GameDataManager* instance(SDL_Renderer*);//����ģʽ��������������
	bool load_game_data(SDL_Renderer*);//������Ϸ��Դ
	bool bin_load_game_data(SDL_Renderer*);//������Ϸ��Դ
	SDL_Texture* get_menu_data(int i);//��ȡ���˵���Դ
	//SDL_Texture* get_CG_data(bool is_next, bool& _loading, int& _dump_img, bool cover,int idx);//��ȡCG��BG��Դ
	SDL_Texture* get_CG_data(int CG_num_idx);//��ȡCG��BG��Դ(��)
	GameData_txt* get_txt_data(bool is_next, bool seting, int& _dump_txt, bool cover,int idx);//��ȡ�ı���Դ
	GameData_txt* get_txt_data(int idx);
	void get_END_idx(std::vector<int> &int_list);
	std::vector<Button_txt*>* get_option_data();
	void get_option_start_number(int& start,int& end);
	int get_last_option_record(int& bbefor_number);
	void record_option(int option_idx);
	int jmp_option(int option_idx);
	void jmp_txt_number(int number);
	SDL_Texture* load_texture(std::string dir);
	void load_savedata(bool updata );
	void save(int _number);
	void load(int _number, bool& _is_next);
	void set_txt_next(bool flag);
	GameData_txt* get_savedata(int _number);
	bool check_txt_data_cgjmp(int idx);
	void data_debaug(bool& debug);
	void read_game_record(int end_idx);
	int get_game_record();

	void reset_progress();
	void clear_record(KaracterName name);

	//���л��뷴���л�
	bool serialize_Textures(const std::vector<SDL_Texture*>& textures, const std::string& dir);
	std::vector<SDL_Texture*> deserialize_Textures(SDL_Renderer* renderer, const std::string& dir);
	void bin_save_txt_data(const std::string& dir);
	void bin_load_txt_data(const std::string& dir);
	/**
 * ���л�ָ��Ŀ¼�е���Ƶ�ļ�
 * @param dirPath Ҫ���л���Ŀ¼·��
 * @param extension �ļ���չ��ɸѡ��
 * @param outputPath ����Ķ������ļ�·��
 * @return �ɹ�����true��ʧ�ܷ���false
 */
	bool serializeAudioDirectory(const std::string& dirPath, const std::string& outputPath);

	/**
	 * �Ӷ������ļ������л�ΪMix_Music*�б�
	 * @param inputPath ����Ķ������ļ�·��
	 * @return Mix_Music*�������������߸����ͷ��ڴ�
	 */
	std::vector<Mix_Music*> deserializeAudioToMusic(const std::string& inputPath);

	/**
	 * �Ӷ������ļ������л�ΪMix_Chunk*�б�
	 * @param inputPath ����Ķ������ļ�·��
	 * @return Mix_Chunk*�������������߸����ͷ��ڴ�
	 */
	std::vector<Mix_Chunk*> deserializeAudioToChunk(const std::string& inputPath);

	void bin_save_kyara_data_link(const std::string& dir, std::vector<KyaraNode*> data);
	void bin_load_kyara_data_link(const std::string& dir, std::vector<KyaraNode*> data);

private:
	GameDataManager() = default;
	GameDataManager(SDL_Renderer* _renderer) { renderer = _renderer; }
	~GameDataManager(){}
};


	