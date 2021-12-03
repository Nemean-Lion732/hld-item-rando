
#include "aer/object.h"
#include "aer/log.h"
#include "aer/sprite.h"

#include "export.h"

#include "crate.h"
#include "stdio.h"
static int32_t crateSpriteIdx;
static int32_t crateMaskIdx;
static int32_t crateObjectIdx;

/* ----- PRIVATE FUNCTIONS ----- */

static void logItemText(randomItemInfo_t item)
{
    FILE *f;
    f = fopen("logic.txt", "a");
    char *typeName = "13_default_l";
    switch (item.data.type)
    {
    case ITEM_GEARBIT:
        typeName = "ITEM_GEARBIT";
        break;
    case ITEM_KEY:
        typeName = "ITEM_KEY";
        break;
    default:
        typeName = "ITEM_WEAPON";
        break;
    }
    fprintf(f, "{.data.type = %s, .data.identifier = %u},\n", typeName, item.data.identifier);
    fclose(f);
}
static void incrementGearBits()
{
    AERInstance* dataInst;
    size_t num = AERInstanceGetByObject(AER_OBJECT_DATA, false, 1, &dataInst);

    if (num == 0)
    {
        // we didnt update our pointer! Log This!
        AERLogErr("Could not get AER_OBJECT_DATA instance!");
        return;
    }
    // assume that we got a valid reference and the first one is good
    // increment the gearBits
    AERInstanceGetHLDLocal(dataInst, "gearbitInventory")->d += 1;
}

static void incrementKeys()
{
    AERLogInfo("Incrementing Keys!");
    AERInstance* dataInst;
    size_t num = AERInstanceGetByObject(AER_OBJECT_DATA, false, 1, &dataInst);

    if (num == 0)
    {
        // we didnt update our pointer! Log This!
        AERLogErr("Could not get AER_OBJECT_DATA instance!");
        return;
    }
    // assume that we got a valid reference and the first one is good
    // increment the keys
    AERInstanceGetHLDLocal(dataInst, "drifterKeyInventory")->d += 1;
}

static void newWeapon(uint32_t weaponNum)
{
    AERLogInfo("Creating Weapon %u", weaponNum);
    
    // Create a new persistent weapon instance. The location does not matter
    AERInstance* weaponInst = AERInstanceCreate(AER_OBJECT_SECONDARY, 0, 0);

    // Set the weapon type
    AERInstanceGetHLDLocal(weaponInst, "type")->d = weaponNum;
}

/* ----- INTERNAL FUNCTIONS ----- */
void createRandomCrate(randomItemInfo_t oldItem, float x, float y)
{
    // Create new instance
    AERInstance* inst = AERInstanceCreate(crateObjectIdx, x, y);
    
    // Copy over our item info for what this object will spawn on its destruction
    AERInstanceCreateModLocal(inst, "randomItemInfo", true, NULL)->u = oldItem.raw;

    // Set the correct render depth
    AERInstanceSyncDepth(inst);
    AERInstanceSetTangible(inst, true);
}

bool destroyCrateListener(AEREvent* event, AERInstance* target, AERInstance* other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;
    
    randomItemInfo_t oldItem = {.raw = AERInstanceGetModLocal(target, "randomItemInfo", true)->u};
    AERLogInfo("Random crate destroyed, original item: %u, identifier: %u", oldItem.data.type, oldItem.data.identifier);

    // Log this item in a file
    logItemText(oldItem);
    // TO DO: Implement randomizer logic
    randomItemInfo_t newItem = {.data.type = ITEM_WEAPON, .data.identifier = 2};
    AERLogInfo("Random crate destroyed, original item: %u, identifier: %u", newItem.data.type, newItem.data.identifier);

    // Create whatever item we got
    switch (newItem.data.type)
    {
    case ITEM_GEARBIT:
        // Increment the players gearbits
        incrementGearBits();
        break;
    case ITEM_KEY:
        // Increment the players keys
        incrementKeys();
        break;
    case ITEM_WEAPON:
        // Give the player a new gun, the identifier field will contain the weapon number
        newWeapon(newItem.data.identifier);
        break;
    default:
        AERLogErr("Got unknown item from Random Crate, Type: 0x%u, IdentifierL 0x%u", newItem.data.type, newItem.data.identifier);
        break;
    }
    // Destroy this local (maybe redundant?)
    AERInstanceDeleteModLocal(target, "randomItemInfo", true);
    return true;
}

void registerCrateObjects()
{
    crateObjectIdx = AERObjectRegister("random_crate", AER_OBJECT_DESTRUCTABLE, crateSpriteIdx, crateMaskIdx,
        0, true, true, false);
}

void registerCrateObjectListeners()
{
    AERObjectAttachDestroyListener(crateObjectIdx, destroyCrateListener);
}

void registerCrateSprites()
{
    crateSpriteIdx = AERSpriteRegister("random_crate", "sprites/balloon/blue_inflated.png", 6, 6, 40);
    crateMaskIdx = AERSpriteRegister("random_crate_mask", "sprites/balloon/inflated_hit_mask.png", 1, 6, 40);
}