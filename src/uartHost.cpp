//
// Created by itzRu on 16.06.2021.
//

#include "uartHost.h"
#include "WSerial.h"
#include "wiring_time.h"


HostCom::HostCom() {

}

void HostCom::receve() {
    if (Serial.available() > 0) {
        switch (state) {
            case IDLE_STATE:
                buff[0] = buff[1];
                buff[1] = Serial.read();
                if (buff[0] == startByte1 && buff[1] == startByte2) {
                    state = READ_ID_STATE;
                    buff[0] = 0;
                    buff[1] = 0;
                    timeout = millis() + 10000; //Todo change to 1000
                }
                break;
            case READ_ID_STATE:
                if (timeout > millis()) {
                    id = Serial.read();
                    state = READ_LEN_STATE;
                } else state = TIMEOUT_STATE;
                break;
            case READ_LEN_STATE:
                if (timeout > millis()) {
                    len = Serial.read();
                    delete[] data;
                    data = new char[len];
                    dataCounter = 0;
                    state = READ_DATA_STATE;
                } else state = TIMEOUT_STATE;
                break;
            case READ_DATA_STATE:
                if (timeout > millis()) {
                    char b = Serial.read();
                    data[dataCounter] = b;
                    dataCounter++;
                    if (dataCounter >= len) state = READ_CHECKSUM_STATE;
                } else state = TIMEOUT_STATE;
                break;
            case READ_CHECKSUM_STATE:
                if (timeout > millis()) {
                    checksum = Serial.read();
                    char sum = id + len;
                    for (int i = 0; i < len; ++i) sum += data[i];
                    if (checksum == sum) {
                        callback(this);
                        state = IDLE_STATE;
                    } else {
                        state = TIMEOUT_STATE;
                    }
                } else state = TIMEOUT_STATE;
                break;
            case TIMEOUT_STATE:
                state = IDLE_STATE;
                break;
            default:
                break;
        }
    }
}

void HostCom::sendMessage(char *data, char length, char id) {
    char sum = 0;
    Serial.write(startByte1);
    Serial.write(startByte2);
    Serial.write(id);
    Serial.write(length);
    for (int i = 0; i < length; i++) {
        sum += data[i];
        Serial.write(data[i]);
    }
    sum += length;
    sum += id;
    Serial.write(sum);
}

void HostCom::mergeArray(const char *a1, char size1, const char *a2, char size2, char *&result) {
    result = new char[size1 + size2];
    int ri = 0;
    for (int i = 0; i < size1; ++i) {
        result[ri] = a1[i];
        ri++;
    }
    for (int i = 0; i < size2; ++i) {
        result[ri] = a2[i];
        ri++;
    }
}

void HostCom::setMessageHandler(void(*callback)(HostCom * instance)){
    this->callback = callback;
}