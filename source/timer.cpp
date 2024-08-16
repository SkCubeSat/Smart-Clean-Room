#include<thread>

using namespace std::this_thread;

void setIn(bool val, bool& target, int millis)
{
    sleep_for(std::chrono::milliseconds(millis));
    target = val;
}

void setTrueIn(bool& flag, int millis)
{
    std::thread t(setIn, true, flag, millis);
    t.detach();
}

void setFalseIn(bool& flag, int millis)
{
    std::thread t(setIn, false, flag, millis);
    t.detach();
}