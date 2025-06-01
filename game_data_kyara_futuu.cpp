#include "game_data_kyara_futuu.h"
GameData_kyara_futuu::GameData_kyara_futuu()
{
	animation = new Animation();
}

GameData_kyara_futuu::~GameData_kyara_futuu()
{
}
void GameData_kyara_futuu::on_enter() {}
void GameData_kyara_futuu::on_init(DataLoadMode mode, KyaracterName name) {
	if (mode != DataLoadMode::MOD_FUTUU)return;
	this->name = name;
	kyara_data.push_back(_load_data(u8"resources\\image\\Character\\test", u8".png", Kyara_ACT::NORMAL));//����״̬�������

}
void GameData_kyara_futuu::on_updata(Vector2D& pos, int _delat_, Kyara_ACT ACT) {
	static Timer delay_timer;//�ӳ��õļ�ʱ��
	_delat = _delat_;
	for (KyaraNode* Node : kyara_data) {//�ҵ������ɫ��Ϊ��Դ
		if (Node->ACT == ACT) { now_kyara_node = Node; return; }
	}
}
void GameData_kyara_futuu::on_render() {
	animation->frame_render(now_kyara_node->tex_list, &dst_rect.lift_rect, _delat);
}
void GameData_kyara_futuu::on_delete() {}

KyaraNode* GameData_kyara_futuu::_load_data(std::string path, std::string fileType, Kyara_ACT ACT) {
	KyaraNode* Node = new KyaraNode();
	std::vector<std::string> files;
	intptr_t  hFile = 0;// �ļ����
	struct _finddata_t fileInfo;// �ļ���Ϣ
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileInfo)) != -1) {
		do {
			// �����ļ���ȫ·��
			files.push_back(p.assign(path + "\\").append(fileInfo.name));
		} while (_findnext(hFile, &fileInfo) == 0);  //Ѱ����һ�����ɹ�����0������-1
		_findclose(hFile);
	}
	SDL_Texture* temp_Texture = nullptr;
	for (int i = 0; i < files.size(); i++) {
		temp_Texture = IMG_LoadTexture(renderer, files[i].c_str());
		Node->tex_list.push_back(temp_Texture);
	}
	Node->delta = 1000000000 / Node->tex_list.size();
	return Node;
}
//std::vector<KyaraNode*>& GameData_kyara_futuu::get_kyara_data() {
//	return kyara_data;
//}