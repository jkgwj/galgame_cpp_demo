#include"game_data_kyara.h"
#include"Animation.h"
#include<io.h>

extern SDL_Renderer* renderer;

class GameData_kyara_futuu : public GameData_kyara
{
private:
	
	std::vector<KyaraNode*> kyara_data;
	Animation* animation;
	KyaraNode* now_kyara_node;
	KyaraNode* _load_data( std::string path,  std::string fileType, Kyara_ACT ACT);
	long long _delat;
	RenderRect dst_rect;
public:
	//std::vector<KyaraNode*>& get_kyara_data();
	
	GameData_kyara_futuu();
	~GameData_kyara_futuu();
	void on_enter();
	void on_init(DataLoadMode mode,KyaracterName name);
	void on_updata(Vector2D& pos,int _delta, Kyara_ACT ACT);
    void on_render();
    void on_delete();
	
};

