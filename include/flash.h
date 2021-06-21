#include "stdint.h"

#ifndef HAT_FLASH_H
#define HAT_FLASH_H

#define flash Flash::getInstance()

#define BIT0 (1 << 0)
#define BIT1 (1 << 1)
#define BIT2 (1 << 2)
#define BIT4 (1 << 4)
#define BIT6 (1 << 6)

struct SaveData {
    float gyroOffsetX;
    float gyroOffsetY;
    float gyroOffsetZ;
    
    float roll;
    float pitch;
    float yaw;
};

class Flash {
public:
    static Flash &getInstance() {
        static Flash instance;
        return instance;
    }

    Flash(Flash const &) = delete;

    void operator=(Flash const &) = delete;

    void saveToFlash();

    void loadFromFlash();

    SaveData data{};

private:
    Flash();

    int writeSector(uint32_t Address, void *values, uint16_t size);

    void readSector(uint32_t SectorStartAddress, void *values, uint16_t size);

    void eraseSector(uint32_t SectorStartAddress);

    int sector = 0x800fc00;  //Todo: dene kan fucke opp ting, aner ikke hvor det er i minne (fulgte et eksempel)


};

#endif //HAT_FLASH_H
