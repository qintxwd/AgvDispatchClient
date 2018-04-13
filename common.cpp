#include "common.h"
#include <ctime>

unsigned char checkSum(unsigned char *data, int len)
{
    int sum = 0;
    for (int i = 0;i < len;++i) {
        //int v = data[i] &0xFF;
        sum += data[i] & 0xFF;
        sum &= 0xFF;
    }
    return sum & 0xFF;
}


int getInt32FromByte(char *data)
{
    int j = data[0] & 0x000000ff;
    j |= ((data[1] << 8) & 0x0000ff00);
    j |= ((data[2] << 16) & 0x00ff0000);
    j |= ((data[3] << 24) & 0xff000000);
    return j;
}

uint16_t getInt16FromByte(char *data)
{
    uint16_t j = data[0] & 0x000000ff;
    j |= ((data[1] << 8) & 0x0000ff00);
    return j;
}

uint8_t getInt8FromByte(char *data)
{
    return (uint8_t)(data[0]);
}

std::string getTimeStrNow()
{
    using std::chrono::system_clock;

    //获取当前时间
    auto time_now = std::chrono::system_clock::now();
    //计算当前时间的 毫秒数 %1000;
    int ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch()).count()%1000;

    std::time_t tt = std::chrono::system_clock::to_time_t(time_now);

    std::stringstream ss;

    ss << std::put_time(std::localtime(&tt), "%Y-%m-%d %H:%M:%S") << "." << ms;

    return ss.str();
}
