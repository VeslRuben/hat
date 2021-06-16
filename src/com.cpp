#include "com.h"
#include "WSerial.h"
#include "wiring_time.h"
#include "messages.h"
#include "TimeLib.h"

namespace com {

    void recive() {
        if (Serial.available() > 0) {
            switch (state) {
                case IDLE_STATE:
                    buff[0] = buff[1];
                    buff[1] = Serial.read();
//                    Serial.print((int) buff[0]);
//                    Serial.print(", ");
//                    Serial.println((int) buff[1]);
                    if (buff[0] == startByte1 && buff[1] == startByte2) {
                        state = READ_ID_STATE;
//                        Serial.println("new state: ID");
                        buff[0] = 0;
                        buff[1] = 0;
                        timeout = millis() + 50000; //Todo change to 1000
                    }
                    break;
                case READ_ID_STATE:
                    if (timeout > millis()) {
                        id = Serial.read();
//                        Serial.print((int) id);
//                        Serial.println(" is the id");
                        state = READ_LEN_STATE;
                    } else state = TIMEOUT_STATE;
                    break;
                case READ_LEN_STATE:
                    if (timeout > millis()) {
                        len = Serial.read();
//                        Serial.print((int) len);
//                        Serial.println(" is the len");
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
//                        Serial.print((int) b);
//                        Serial.print(" is the byte at: ");
//                        Serial.println((int) dataCounter);
                        if (dataCounter >= len) state = READ_CHECKSUM_STATE;
                    } else state = TIMEOUT_STATE;
                    break;
                case READ_CHECKSUM_STATE:
                    if (timeout > millis()) {
                        checksum = Serial.read();
//                        Serial.println("checksum");
                        char sum = id + len;
                        for (int i = 0; i < len; ++i) sum += data[i];
                        if (checksum == sum) {
                            newMessage = true;
                            state = IDLE_STATE;
                        } else {
//                            Serial.println("checksum mismatch");
//                            Serial.print((int) sum);
//                            Serial.println(" is the right checksum");
                            state = TIMEOUT_STATE;
                        }
                    } else state = TIMEOUT_STATE;
                    break;
                case TIMEOUT_STATE:
//                    Serial.println("Timeout");
                    state = IDLE_STATE;
                    break;
                default:
                    break;
            }
        }
    }

    void readMessage() {
        if (newMessage) {
            newMessage = false;
            Serial.println("new message!");
            switch (id) {
                case Messages::SetTimeId:
                    Messages::SetTime time{};
                    memcpy(&time, data, sizeof(time));
                    TimeLib::setTime(time.hour, time.minute, time.sec, time.mSec, time.day, time.month, time.year);
                    break;
            }
        }
    }

    void sendMessage(char *data, char length, char id) {
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

    void mergeArray(const char *a1, char size1, const char *a2, char size2, char *&result) {
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
}
