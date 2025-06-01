#pragma once
#include<functional>

class Timer
{
private:
	long long pass_time = 0;  //��ȥʱ��
	long long wait_time = 0;  //�ȴ�ʱ��

	bool pause = 0;     //ͣ���ź�
	bool shot = 0;      //�����ź�
	bool one_shot = 0;  //���δ����ź�
	std::function<void()> callback;//�ص�����
public:
	Timer() = default;
	~Timer() = default;

	void set_callback(std::function<void()> call_back);//���ûص�����
	void restrat();//���ü�ʱ��
	void rewait_time();
	void set_wait_time(long long time);//���õȴ�ʱ��
	void set_one_shot(bool flag);//���õ��δ���
	void set_pause(bool flag);//������ͣ�ź�
	long long get_wait_time();//�������õĵȴ�ʱ��

	void updata(long long delta);//��������
	void updata(long long delta,int i);//��������
};



