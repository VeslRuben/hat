//
// Created by itzRu on 16.06.2021.
//

#ifndef HAT_COM_H
#define HAT_COM_H

#define IDLE_STATE 0
#define READ_ID_STATE 1
#define READ_LEN_STATE 2
#define READ_DATA_STATE 3
#define READ_CHECKSUM_STATE 4
#define TIMEOUT_STATE 5

namespace com {

    static int state = IDLE_STATE;
    static unsigned long timeout = 0;
    static bool newMessage = false;

    static char id = 0;
    static char len = 0;
    static char *data = nullptr;
    static char checksum = 0;

    static char dataCounter = 0;

    static char startByte1 = 0xff;
    static char startByte2 = 0xfa;
    static char buff[2] = {0, 0};

    void recive();

    void readMessage();

    void sendMessage(char *data, char length, char id);

    void mergeArray(const char *a1, char size1, const char *a2, char size2, char *&result);
}
#endif //HAT_COM_H
