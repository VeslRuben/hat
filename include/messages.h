//
// Created by itzRu on 16.06.2021.
//

#ifndef HAT_MESSAGES_H
#define HAT_MESSAGES_H

namespace Messages {

    // Message id's
    const char SetTimeId = 0x01;

    const char DataMessageID = 0xdd;

    // Messages
    struct SetTime {

        unsigned short year;
        char month;
        char day;
        char hour;
        char minute;
        char sec;
        unsigned short mSec;

        SetTime() {}

        SetTime(char *data) {
            year = (data[1] << 8) | data[0];
            month = data[2];
            day = data[3];
            hour = data[4];
            minute = data[5];
            sec = data[6];
            mSec = (data[8] << 8) | data[7];
        }
    };

    //============================================================

    // DataMessages id's
    const char TimeId = 0x01;
    const char AccelerationId = 0x02;
    const char GyroId = 0x03;
    const char OrientationId = 0x04;

    // Data Messages
    const char TimeStructSize = 2 * sizeof(short) + 5 * sizeof(char);

    struct Time {
        char id;
        char len;
        unsigned short year;
        char month;
        char day;
        char hour;
        char minute;
        char sec;
        unsigned short mSec;
    };
    const char AccStructSize = 3 * sizeof(float);
    const char GyroStructSize = 3 * sizeof(float);
    const char OrientationStructSize = 3 * sizeof(float);

    typedef struct {
        char id;
        char len;
        float x;
        float y;
        float z;
    } Acceleration, Gyro, Orientation;
}

#endif //HAT_MESSAGES_H
