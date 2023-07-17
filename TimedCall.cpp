#include "TimedCall.h"

TimedCall::TimedCall(std::function<void(void)> f, uint32_t time) 
{
	f_ = f;
	time_ = time;
}

void TimedCall::Update()
{ 
	if (isFinish)
	{
		return;
	}
	time_--;
	if (time_ <= 0)
	{
		isFinish = true;
		// コールバック関数呼び出し
		f_();
	}
}
