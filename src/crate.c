
#include "aer/object.h"
#include "aer/log.h"
#include "aer/sprite.h"

#include "export.h"

#include "logic.h"
#include "crate.h"

static int32_t crateSpriteIdx;
static int32_t crateMaskIdx;
static int32_t crateObjectIdx;

/* ----- PRIVATE FUNCTIONS ----- */

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

static void newWeapon(uint16_t weaponNum)
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
    AERLogInfo("Random crate created, original item: %u, identifier: %u, room_id: %u", oldItem.data.type, oldItem.data.identifier, oldItem.data.room_id);
    // Set the correct render depth
    AERInstanceSyncDepth(inst);
    AERInstanceSetTangible(inst, true);
}

bool destroyCrateListener(AEREvent* event, AERInstance* target, AERInstance* other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;
    
    randomItemInfo_t item = {.raw = AERInstanceGetModLocal(target, "randomItemInfo", true)->u};
    AERLogInfo("Item raw %llu", item.raw);
    AERLogInfo("Random crate destroyed, original item: %u, identifier: %u, room_id: %u", item.data.type, item.data.identifier, item.data.room_id);

    // TO DO: Implement randomizer logic
    updateRandomItem(&item);

    AERLogInfo("New Item, original item: %u, identifier: %u, room_id: %u", item.data.type, item.data.identifier, item.data.room_id);

    // Create whatever item we got
    switch (item.data.type)
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
        newWeapon(item.data.identifier);
        break;
    default:
        AERLogErr("Got unknown item from Random Crate, Type: 0x%u, IdentifierL 0x%u", item.data.type, item.data.identifier);
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