#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

//立绘左右位置
#define LIFT_RECT {25,220,590,500}
#define RIGHT_RECT {640,220,590,500}



//libboost_serialization-vc143-mt-s-x64-1_88.lib


typedef enum
{
    RENN = 21,
    AOI,
    AYAMIYA_KAISANN,
    GENNDAI,
    SENNSEI,
    TINNPIRA_1,
    TINNPIRA_2,
    TINNPIRA_3,
    TINNPIRADOMO,
    ONNNA,
    KUTO_SINNIKO,
    SUTAFFU,
    SHIRANAI_HOTO,
    SHIRANAI_HOTOTATI,
    RONNDANHARUKI,
    TANNNINN,
    DANNSI_1,
    DANNSI_2,
    JYOUSI_1,
    JYOUSI_2,
    SANNNINN,
    YUUEMI,
    NPOS,
	IMOUTO,
    AMAMIYA_RENNA


} KaracterName, KyaracterName;
struct DataLink
{
	int CG;
	int MUSIC;
	DataLink(int _cg,int _music):CG(_cg),MUSIC(_music){}
};

struct SaveData
{
	int number;
	int txt_idx;
    int now_option;
    std::vector<int> option_list;
	std::string u8_dir;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& number;
        ar& txt_idx;
        ar& now_option;
        ar& option_list;
        ar& u8_dir;
    }
};

struct _Range
{
    int txt_start = 0;
    int txt_end=0;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& txt_start;
        ar& txt_end;
    }
   
};
struct Option
{
    _Range option;
    std::string str;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& option;
        ar& str;
    }
};
enum DataLoadMode
{
    //立绘资源加载模式
    MOD_FUTUU = 0,
    MOD_CUT
};
enum Kyara_ACT {
   NORMAL
};
//角色资源节点结构体
struct KyaraNode
{
    std::vector<SDL_Texture*> tex_list;
    Kyara_ACT ACT;
    long long delta;
    KyaraNode() = default;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
       // ar& tex_list;
        ar& ACT;
		ar& delta;
    }
};
struct _END
{
   
    int txt_idx;
    int END_number;
    _END() = default;
    _END(int idx, int END) :txt_idx(idx), END_number(END) {}
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& txt_idx;
        ar& END_number;
    }
};

// 定义Texture纹理数据结构体（统一使用RGBA8888格式）
struct TextureData {
    int width;               /**< 纹理宽度 */
    int height;              /**< 纹理高度 */
    int pitch;               /**< 纹理行间距 */
    std::vector<unsigned char> compressedPixels; /**< 压缩后的像素数据 */

    // Boost序列化接口
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& width;
        ar& height;
        ar& pitch;
        ar& compressedPixels;
    }
};

// 音频文件数据结构（序列化用）
struct AudioFile {
    std::string filename;
    std::vector<Uint8> data;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& filename;
        ar& data;
    }
};
 // 每次反序列化的独立存储
struct AudioStorage {
    std::vector<std::unique_ptr<Uint8[], void(*)(void*)>> alignedAudioData;
    std::vector<Mix_Music*> musicList;
    std::vector<Mix_Chunk*> chunkList;

    AudioStorage() = default;

    ~AudioStorage() {
        // 自动清理资源
        for (auto* music : musicList) Mix_FreeMusic(music);
        for (auto* chunk : chunkList) Mix_FreeChunk(chunk);
    }
};


// 显式声明类版本号
BOOST_CLASS_VERSION(TextureData, 1)
BOOST_CLASS_VERSION(AudioFile, 2)



//渲染时的位置（会添加动态加载功能）
struct RenderRect
{
    SDL_FRect lift_rect; 
    SDL_FRect right_rect;
    SDL_FRect middle_rect;
};