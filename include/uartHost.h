//
// Created by itzRu on 16.06.2021.
//

#ifndef HAT_UARTHOST_H
#define HAT_UARTHOST_H

#define IDLE_STATE 0
#define READ_ID_STATE 1
#define READ_LEN_STATE 2
#define READ_DATA_STATE 3
#define READ_CHECKSUM_STATE 4
#define TIMEOUT_STATE 5

class HostCom {
public:

    static HostCom &getInstance() {
        static HostCom instance;
        return instance;
    }

    HostCom(HostCom const &) = delete;

    void operator=(HostCom const &) = delete;

    void setMessageHandler(void(*callback)(HostCom * instance));

    void receve();

    void sendMessage(char *data, char length, char id);

    char id = 0;
    char len = 0;
    char *data = nullptr;
    char checksum = 0;

private:
    HostCom();


    int state = IDLE_STATE;
    unsigned long timeout = 0;

    char dataCounter = 0;

    char startByte1 = 0xff;
    char startByte2 = 0xfa;
    char buff[2] = {0, 0};

    void (*callback)(HostCom * instance);

};

#endif //HAT_UARTHOST_H
