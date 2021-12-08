#ifndef LOGIC_H
#define LOGIC_H

typedef enum randomizedItem_e
{
    ITEM_GEARBIT,
    ITEM_KEY,
    ITEM_WEAPON
} randomizedItem_e;

typedef union randomItemInfo_t
{
    uint64_t raw;
    struct {
        uint16_t room_id;
        uint16_t identifier;
        uint16_t type;
    } data;
} randomItemInfo_t;

/* ----- PUBLIC FUNCTIONS ----- */

bool updateRandomItem(randomItemInfo_t* item);
void createRandomizedIndexes();
#endif