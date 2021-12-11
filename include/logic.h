#ifndef LOGIC_H
#define LOGIC_H

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

/* ----- PUBLIC FUNCTIONS ----- */

void checkRandomizerSpawn(randomItemInfo_t oldItem, float x, float y);
bool updateRandomItem(randomItemInfo_t* item);
void createRandomizedIndexes();
#endif