#pragma once
#include"data_tool.h"
/*
 else if (_name == u8"�깬��")                  name = KaracterName::RENN;
    else if (_name == u8"�깬��")               name = KaracterName::AOI;
    else if (_name == u8"����")                 name = KaracterName::AYAMIYA_KAISANN;
    else if (_name == u8"Ԫ̫")                 name = KaracterName::GENNDAI;
    else if (_name == u8"��ʦ")                 name = KaracterName::SENNSEI;
    else if (_name == u8"���1")                name = KaracterName::TINNPIRA_1;
    else if (_name == u8"���2")                name = KaracterName::TINNPIRA_2;
    else if (_name == u8"���3")                name = KaracterName::TINNPIRA_3;
    else if (_name == u8"�����")               name = KaracterName::TINNPIRADOMO;
    else if (_name == u8"����Ů��")             name = KaracterName::ONNNA;
    else if (_name == u8"����������")           name = KaracterName::KUTO_SINNIKO;
    else if (_name == u8"������Ա")             name = KaracterName::SUTAFFU;
    else if (_name == u8"·��")             name = KaracterName::SHIRANAI_HOTO;
    else if (_name == u8"·����")             name = KaracterName::SHIRANAI_HOTOTATI;
*/

class GameData_txt
{
    friend class boost::serialization::access;
   
private:
	int number;
    int CG_number;//����ʾ˳��
	bool CG_jmp;
	int MUSIC_number;//����ʾ˳��
    KaracterName name;
    std::string str;
    std::function<void(int)> callback;

   
public:
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& number;
        ar& CG_number;
        ar& CG_jmp;
        ar& MUSIC_number;
        ar& name;
        ar& str;
        ar& readed;
        ar& audio_idx;
        ar& option;
    }

    bool readed = 0;
    int audio_idx;
    int option = -1;
    
    //GameData_txt(int number, bool CG_jmp, bool MUSIC_jmp, int category/*���*/,std::string _str);
    GameData_txt(int num, bool _CG_jmp, int _MUSIC_number, std::string _name, std::string _str, int _audio_idx);
    GameData_txt() = default;

    ~GameData_txt();

    //void up_texture(SDL_Renderer* renderer, SDL_Texture*& txt_img/*�ı�����*/, SDL_Texture*& name_img/*��������*/,
    //    std::string _u8_dir_txt_font, std::string _u8_dir_name_font, int txt_font_size, int name_font_size,
    //    SDL_FRect& txt_rect, SDL_FRect& name_rect, SDL_Color txt_color = { 255,58,88,0 });

    void up_texture(SDL_Renderer* renderer, SDL_Texture*& txt_img/*�ı�����*/, SDL_Texture*& name_img/*��������*/,
        std::string _u8_dir_txt_font, std::string _u8_dir_name_font, int txt_font_size, int name_font_size,
        SDL_FRect& txt_rect, SDL_FRect& name_rect, SDL_Color txt_color);
    void up_textureEX_txt(SDL_Renderer* renderer, SDL_Texture*& txt_img,std::string _u8_dir_txt_font, 
        int txt_font_size, SDL_FRect& txt_rect, SDL_Color txt_color, int MAX_WIGHT, int MAX_HIGHT);

    void set_number(int& num);
    void set_CG_jmp(bool flag);
    void set_MUSIC_jmp(bool flag);
    void set_string(std::string s);
    void set_option(int _option);
    void set_CG_number(int number);
    void set_call_back(std::function<void(int)> call_back);
    int  check_no_render(int _option);
    bool check_option_txt();
    KaracterName get_name() { return name; }
    int get_number();
    bool get_CG_jmp();
    int get_MUSIC_number();
    int get_CG_number();
    std::string get_string();
    std::string get_name_str();

    void out_txt_data(std::fstream& ioFile);
};
