#pragma once
#include<functional>

class Timer
{
private:
	long long pass_time = 0;  //过去时间
	long long wait_time = 0;  //等待时间

	bool pause = 0;     //停顿信号
	bool shot = 0;      //触发信号
	bool one_shot = 0;  //单次触发信号
	std::function<void()> callback;//回调函数
public:
	Timer() = default;
	~Timer() = default;

	void set_callback(std::function<void()> call_back);//设置回调函数
	void restrat();//重置计时器
	void rewait_time();
	void set_wait_time(long long time);//设置等待时间
	void set_one_shot(bool flag);//设置单次触发
	void set_pause(bool flag);//设置暂停信号
	long long get_wait_time();//返回设置的等待时间

	void updata(long long delta);//更新数据
	void updata(long long delta,int i);//更新数据
};



