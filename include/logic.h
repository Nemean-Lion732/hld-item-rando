#ifndef LOGIC_H
#define LOGIC_H

#define RAND_MAP_SIZE 208

/* ----- Data Structures ----- */

typedef enum randomizedItem_e
{
    ITEM_GEARBIT,
    ITEM_KEY,
    ITEM_WEAPON
} randomizedItem_e;

typedef union __attribute__((aligned(4))) randomItemInfo_t
{
    uint64_t raw;
    struct 
    {
        uint32_t identifier;
        uint16_t room_id;
        uint16_t type;
    } data;
} randomItemInfo_t;

/* ----- INTERNAL FUNCTIONS ----- */

void logicGameLoadListener();

int16_t getItemIndex(randomItemInfo_t* item);
randomItemInfo_t updateRandomItem(int32_t oldItemIdx);
void checkForNewGame(int32_t newRoomIdx);

#endif