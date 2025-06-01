#include "Timer.h"
void Timer::set_callback(std::function<void()> call_back) {
	this->callback = call_back;
}
void Timer::restrat() {
	wait_time = 0;
	shot = false;
}
void Timer::set_wait_time(long long time) {
	wait_time = time;
}
void Timer::set_one_shot(bool flag) {
	one_shot = flag;
}
void Timer::set_pause(bool flag) {
	pause = flag;
}
long long  Timer::get_wait_time() {
	return wait_time;
}

void Timer::updata(long long delta) {
	if (pause)return;
	pass_time += delta;
	if (wait_time <= pass_time)
	{
		if ((!one_shot || (one_shot && !shot)) && callback) callback();
		shot = 1;
		pass_time = 0;
	}
}
void Timer::rewait_time() {
	wait_time = 0;
}
void  Timer::updata(long long delta, int i) {
	if (pause)return;

	pass_time += delta;
	if (wait_time <= pass_time)
	{
		if ((!one_shot || (one_shot && !shot)) && callback&&i==0) callback();
		shot = 1;
		pass_time = 0;
	}
}
