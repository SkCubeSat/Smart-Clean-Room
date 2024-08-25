#include <pigpio.h>
#include <thread>

using namespace std;

int main(int argc, char* argv[]) {
    if (gpioInitialise() < 0)
    {
        return -1;
    } else
    {
        gpioSetMode(2, PI_OUTPUT);
        gpioWrite(2, 1);
        this_thread::sleep_for(chrono::milliseconds(1000))
        gpioWrite(2,0);
        gpioTerminate();
    }

    return 0;
}