#ifndef CRATE_H
#define CRATE_H

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
        uint32_t type;
        uint32_t identifier;
    } data;
} randomItemInfo_t;

/* ----- PUBLIC FUNCTIONS ----- */

void registerCrateObjects();
void registerCrateSprites();
bool destroyCrateListener(AEREvent* event, AERInstance* target, AERInstance* other);
void registerCrateObjectListeners();
void createRandomCrate(randomItemInfo_t newItem, float x, float y);
#endif /* CRATE_H */