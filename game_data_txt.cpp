#include "game_data_txt.h"
#include <io.h>
#include<fstream>
#include<iostream>
#include <filesystem>


//GameData_txt::GameData_txt(int num, bool _CG_jmp, bool _MUSIC_jmp, int _category/*类别*/, std::string _str) {
//    number = num; CG_jmp = _CG_jmp; MUSIC_jmp = _MUSIC_jmp; str = _str;
//    if (_category >= 21 && _category <= 26)
//    {
//        category = 2;
//        switch (_category)
//        {
//        case 21:         name = KaracterName::RENN;
//        case 22:         name = KaracterName::AOI;
//        case 23:         name = KaracterName::AYAMIYA_KAISANN;
//        case 24:         name = KaracterName::SENNSEI;
//        case 25:         name = KaracterName::SYATYOU;
//        case 26:         name = KaracterName::HOTO;
//
//        default: printf(u8"没有该人物\n;");
//        }
//
//    }
//    else category = _category;
//}
GameData_txt::GameData_txt(int num, bool _CG_jmp, int _MUSIC_number, std::string _name, std::string _str,int _audio_idx) {
    number = num; CG_jmp = _CG_jmp; MUSIC_number = _MUSIC_number;  str = _str; if (_audio_idx >= 0)audio_idx = _audio_idx; else audio_idx = -1;
    if (_name.find_first_not_of(' ') == std::string::npos)
    {
        name = KaracterName::NPOS;
    }

    //雨宫莲，雨宫葵，元太，老师，年轻女人，混混1，混混2，混混3，工藤新依子，工作人员
    else if (_name == u8"雨宫莲")               name = KaracterName::RENN;
    else if (_name == u8"雨宫葵")               name = KaracterName::AOI;
    else if (_name == u8"妈妈")                 name = KaracterName::AYAMIYA_KAISANN;
    else if (_name == u8"元太")                 name = KaracterName::GENNDAI;
    else if (_name == u8"老师")                 name = KaracterName::SENNSEI;
    else if (_name == u8"混混1")                name = KaracterName::TINNPIRA_1;
    else if (_name == u8"混混2")                name = KaracterName::TINNPIRA_2;
    else if (_name == u8"混混3")                name = KaracterName::TINNPIRA_3;
    else if (_name == u8"混混们")               name = KaracterName::TINNPIRADOMO;
    else if (_name == u8"年轻女人")             name = KaracterName::ONNNA;
    else if (_name == u8"工藤新依子")           name = KaracterName::KUTO_SINNIKO;
    else if (_name == u8"工作人员")             name = KaracterName::SUTAFFU;
    else if (_name == u8"路人")                 name = KaracterName::SHIRANAI_HOTO;
    else if (_name == u8"路人们")               name = KaracterName::SHIRANAI_HOTOTATI;
    else if (_name == u8"龙胆春树")             name = KaracterName::RONNDANHARUKI;
    else if (_name == u8"班主任")               name = KaracterName::TANNNINN;
    else if (_name == u8"男同学1")              name = KaracterName::DANNSI_1;
    else if (_name == u8"男同学2")              name = KaracterName::DANNSI_2;
    else if (_name == u8"女同学1")              name = KaracterName::JYOUSI_1;
    else if (_name == u8"女同学2")              name = KaracterName::JYOUSI_2;
    else if (_name == u8"三人")                 name = KaracterName::SANNNINN;
    else if (_name == u8"优惠美")               name = KaracterName::YUUEMI;
	else if (_name == u8"妹妹")                 name = KaracterName::IMOUTO;
    else if (_name == u8"雨宫莲奈")             name = KaracterName::AMAMIYA_RENNA;
   
    else { std::cout << u8"加载出错，没有该人物" << std::endl; getchar(); }
    
}
void GameData_txt::up_texture(SDL_Renderer* renderer, SDL_Texture*& txt_img/*文本纹理*/, SDL_Texture*& name_img/*名字纹理*/,
    std::string _u8_dir_txt_font, std::string _u8_dir_name_font, int txt_font_size, int name_font_size,
    SDL_FRect& txt_rect, SDL_FRect& name_rect, SDL_Color txt_color = { 255,58,88,0 }) {
    TTF_Font* font;//字体
    font = TTF_OpenFont(_u8_dir_txt_font.c_str(), txt_font_size);
    SDL_Surface* suf_img = TTF_RenderUTF8_Blended_Wrapped(font, str.c_str(), txt_color, 1050);
    txt_rect.w = suf_img->w; txt_rect.h = suf_img->h;
    txt_img = SDL_CreateTextureFromSurface(renderer, suf_img);
    SDL_FreeSurface(suf_img);
    TTF_CloseFont(font);
    if (name == KaracterName::NPOS) { name_img = nullptr; if (str.find(u8"#END_") != std::string::npos) 
        this->callback(number); return; }
    std::string str_name = this->get_name_str();
    font = TTF_OpenFont(_u8_dir_name_font.c_str(), name_font_size);
    SDL_Surface* _suf_img = TTF_RenderUTF8_Blended_Wrapped(font, str_name.c_str(), txt_color, 200);
    name_rect.w = _suf_img->w; name_rect.h = _suf_img->h;
    name_img = SDL_CreateTextureFromSurface(renderer, _suf_img);
    SDL_FreeSurface(_suf_img);
    TTF_CloseFont(font);
   
}
void GameData_txt::up_textureEX_txt(SDL_Renderer* renderer, SDL_Texture*& txt_img, std::string _u8_dir_txt_font,
    int txt_font_size,SDL_FRect &txt_rect, SDL_Color txt_color, int MAX_WIGHT,int MAX_HIGHT) {
    TTF_Font* font;//字体
    font = TTF_OpenFont(_u8_dir_txt_font.c_str(), txt_font_size);
    if (str.find(u8"：：：") != std::string::npos)str = u8"选项";
    SDL_Surface* suf_img = TTF_RenderUTF8_Blended_Wrapped(font, str.c_str(), txt_color, MAX_WIGHT);
    txt_rect.w = suf_img->w; txt_rect.h = suf_img->h;
	if (txt_rect.h >= MAX_HIGHT) { //如果文本高度超过最大高度
        SDL_Surface* suf_img2=nullptr;
        TTF_Font* font2;
        while (txt_rect.h >= MAX_HIGHT) {
            txt_font_size--;
            if(suf_img2!=nullptr)SDL_FreeSurface(suf_img2);
            font2 = TTF_OpenFont(_u8_dir_txt_font.c_str(), txt_font_size);
            suf_img2 = TTF_RenderUTF8_Blended_Wrapped(font2, str.c_str(), txt_color, MAX_WIGHT);
            TTF_CloseFont(font2);
            txt_rect.h=suf_img2->h;
        }
        txt_img = SDL_CreateTextureFromSurface(renderer, suf_img2);
        SDL_FreeSurface(suf_img2);
    }
    else {
        txt_img = SDL_CreateTextureFromSurface(renderer, suf_img);
        
    }
    TTF_CloseFont(font);
    SDL_FreeSurface(suf_img);
   
}

void GameData_txt::set_number(int& num) { number = num; }
void GameData_txt::set_CG_jmp(bool flag) { CG_jmp = flag; }
void GameData_txt::set_MUSIC_jmp(bool flag) { MUSIC_number = flag; }
void GameData_txt::set_string(std::string s) { str = s; }
void GameData_txt::set_CG_number(int _number) { CG_number = _number; }
void GameData_txt::set_call_back(std::function<void(int _idx)> call_back) {callback = call_back;}

int GameData_txt::get_number() { return number; }
bool GameData_txt::get_CG_jmp() { return CG_jmp; }
int GameData_txt::get_MUSIC_number() { return MUSIC_number; }
std::string GameData_txt::get_string() { return str; }
std::string GameData_txt::get_name_str() {
    /*
雨宫莲
雨宫葵
妈妈
老师
社长
某人
麻美
混混1
混混2
混混3
工作人员
女人
大叔
学生会长
班长
女A
女B
男孩
     */
    switch (name)
    {
    case NPOS:                      return u8"";
    case RENN:                      return u8"雨宫莲";
    case AOI:                       return u8"雨宫葵";
    case AYAMIYA_KAISANN:           return u8"妈妈";
    case GENNDAI:                   return u8"元太";
    case SENNSEI:                   return u8"老师";
    case KUTO_SINNIKO:              return u8"工藤新依子";
    case TINNPIRA_1:                return u8"混混1";
    case TINNPIRA_2:                return u8"混混2";
    case TINNPIRA_3:                return u8"混混3";
    case TINNPIRADOMO:              return u8"混混们";
    case SUTAFFU:                   return u8"工作人员";
    case ONNNA:                     return u8"年轻女人";
    case SHIRANAI_HOTO:             return u8"路人";
    case SHIRANAI_HOTOTATI:         return u8"路人们";
    case RONNDANHARUKI:             return u8"龙胆春树";
    case TANNNINN:                  return u8"班主任";
    case DANNSI_1:                  return u8"男同学1";
    case DANNSI_2:                  return u8"男同学2";
    case JYOUSI_1:                  return u8"女同学1";
    case JYOUSI_2:                  return u8"女同学2";
    case SANNNINN:                  return u8"三人";
    case YUUEMI:                    return u8"优惠美";
    case IMOUTO:                    return u8"妹妹";
    case AMAMIYA_RENNA:             return u8"雨宫莲奈";
    default: printf(u8"没有该人物\n;");
    }
   
}


GameData_txt::~GameData_txt() {

}

void GameData_txt::out_txt_data(std::fstream& ioFile) {
    ioFile << this->get_name_str() << " " << number << ":  " << this->str << "####\n"
           << CG_number << "\n"
           << CG_jmp << "\n"
           << MUSIC_number << "\n"
           << audio_idx << "\n"
           <<readed<<"\n"
           << option << "\n";
} 
void GameData_txt::set_option(int _option) {
    option = _option;
}
int GameData_txt::get_CG_number() {
    return this->CG_number;
}
int GameData_txt::check_no_render(int _option) {
    if (option == -1)return 0;
    else if (str.find(u8"##############") != std::string::npos)
        return 3;
    else if (str.find(u8"：：：") != std::string::npos)
        return 2;
    else if (_option > 0 && option != _option + 1)return 1;
     return 0;
}
bool GameData_txt::check_option_txt(){
    if (option != -1)return 1;
    if (str.find(u8"：：：") != std::string::npos)
        return 1;
}