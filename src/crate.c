/*!
 *  @file crate.c
 *
 *  Randomized crate object, which when broken will distribute a randomized item
 *  
 *  This file also contains flags for which crates have been broken previously
 */
#include "aer/object.h"
#include "aer/log.h"
#include "aer/sprite.h"
#include "aer/err.h"
#include "aer/save.h"
#include <stdio.h>
#include <stdlib.h>

#include "export.h"

#include "logic.h"
#include "crate.h"
#include "indicator.h"

#define RAND_ITEM_TAKEN_FLAGS_SIZE      RAND_MAP_SIZE / 32 + 1

static int32_t crateSpriteIdx;
static int32_t crateMaskIdx;
static int32_t crateObjectIdx;

static uint32_t itemTakenFlags[RAND_ITEM_TAKEN_FLAGS_SIZE];
static size_t saveBuffer[10]; // A buffer of items to be saved during the next save event
static size_t saveBufferCounter = 0;

/* ----- PRIVATE FUNCTIONS ----- */

/*!
 *  @brief Increases (or decreases) the players gearbit count
 */
static void incrementGearBits(int amount)
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
    AERInstanceGetHLDLocal(dataInst, "gearbitInventory")->d += amount;
}

/*!
 *  @brief Increases (or decreases) the players key count
 */
static void incrementKeys(int amount)
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
    // increment the keys
    AERInstanceGetHLDLocal(dataInst, "drifterKeyInventory")->d += amount;
}

/*!
 *  @brief Gives the player another weapon
 *
 *  @param[in] weaponNum        the weapon's type / identifier
 */
static void newWeapon(size_t weaponNum)
{ 
    // Create a new persistent weapon instance. The location does not matter
    AERInstance* weaponInst = AERInstanceCreate(AER_OBJECT_SECONDARY, 0, 0);

    // Set the weapon type
    AERInstanceGetHLDLocal(weaponInst, "type")->d = weaponNum;
}

/*!
 *  @brief Destroy's the players weapon
 *
 *  @param[in] weaponNum        the weapon's type / identifier
 */
static void deleteWeapon(size_t weaponNum)
{ 
    size_t num = AERInstanceGetByObject(AER_OBJECT_SECONDARY, false, 0, NULL);
    AERInstance** weaponArray = malloc(num * sizeof(AERInstance*));
    AERInstanceGetByObject(AER_OBJECT_SECONDARY, false, num, weaponArray);

    for (size_t i = 0; i < num; i++)
    {
        if (AERInstanceGetHLDLocal(weaponArray[i], "type")->d == weaponNum)
            AERInstanceDestroy(weaponArray[i]);
    }
}

/*!
 *  @brief Checks if the player has picked up this item before
 *
 *  @param[in] oldItemIdx        index of the old item in the ordered map
 * 
 *  @return 0 if item has not been taken, 1 if the item has
 */
static bool checkItemTakenFlag(uint32_t oldItemIdx)
{
    // Save File
    // Get the uint32 which this index belongs in
    uint32_t flag4byte = itemTakenFlags[oldItemIdx / 32];

    // Check that its specific bit is 0
    bool saveFile = (flag4byte & (1 << (oldItemIdx % 32))) != 0;
    bool localRoom = false;
    for(size_t i = 0; i < saveBufferCounter; i++)
    {
        if (saveBuffer[i] == oldItemIdx)
            localRoom = true;
    }
    return saveFile || localRoom;
}

/*!
 *  @brief Sets a flag to tell the game that the flag has been taken
 *
 *  @param[in] oldItemIdx        index of the old item in the ordered map
 */
static void setItemTakenFlag(uint32_t oldItemIdx)
{
    itemTakenFlags[oldItemIdx / 32] |= 1 << (oldItemIdx % 32);
}

/*!
 *  @brief Function called upon a randomized crate being destroyed
 *
 *  @param[in] event        Type of event for which this function was called
 *  @param[in] target       AERInstance pointer to the gear crate
 *  @param[in] other        unused
 * 
 *  @return Determines if the destruction event is cancelled (should always returns true)
 */
static bool destroyCrateListener(AEREvent* event, AERInstance* target, AERInstance* other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;
    
    uint32_t oldItemIdx = AERInstanceGetModLocal(target, "randomItemIdx", true)->u;
    randomItemInfo_t newItem = updateRandomItem(oldItemIdx);

    AERLogInfo("Old Item Idx %lu", oldItemIdx);
    AERLogInfo("Random New Item: %u, identifier: %u, room_id: %u", newItem.data.type, newItem.data.identifier, newItem.data.room_id);

    // Create whatever item we got
    switch (newItem.data.type)
    {
    case ITEM_GEARBIT:
        // Increment the players gearbits
        incrementGearBits(1);
        break;
    case ITEM_KEY:
        // Increment the players keys
        incrementKeys(1);
        break;
    case ITEM_WEAPON:
        // Give the player a new gun, the identifier field will contain the weapon number
        newWeapon(newItem.data.identifier);
        break;
    default:
        AERLogErr("Got unknown item from Random Crate, Type: 0x%u, IdentifierL 0x%u", newItem.data.type, newItem.data.identifier);
        break;
    }

    // Set this flag in the save buffer counter
    saveBuffer[saveBufferCounter] = oldItemIdx;
    saveBufferCounter++;

    // Destroy this local (maybe redundant?)
    AERInstanceDeleteModLocal(target, "randomItemIdx", true);

    // Create a indicator
    float x, y;
    AERInstanceGetPosition(target, &x, &y);
    createPickupIndicator(newItem, x, y);
    return true;
}

/*!
 *  @brief Checks if the character has died. Resets random item progress if true
 *
 *  @param[in] event        Type of event for which this function was called
 *  @param[in] target       AERInstance pointer to instance of interest
 *  @param[in] other        unused
 * 
 *  @return Determines if the creation event is cancelled (should always returns true)
 */
static bool charDestroyListener(AEREvent *event, AERInstance *target, AERInstance *other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;
    
    if (AERInstanceGetHLDLocal(target, "hp")->d == 0){
        // The character has died, reset the random item tracker
        AERLogInfo("Drifter has died! Reseting flags...");
        for (size_t i = 0; i < saveBufferCounter; i++)
        {
            randomItemInfo_t newItem = updateRandomItem(saveBuffer[i]);

            // Remove the item we got
            switch (newItem.data.type)
            {
            case ITEM_GEARBIT:
                // Remove the players gearbits
                incrementGearBits(-1);
                break;
            case ITEM_KEY:
                // Remove the players keys
                incrementKeys(-1);
                break;
            case ITEM_WEAPON:
                // Take away the weapon the player got
                deleteWeapon(newItem.data.identifier);
                break;
            default:
                AERLogErr("Got unknown item from Random Crate, Type: 0x%u, IdentifierL 0x%u", newItem.data.type, newItem.data.identifier);
                break;
            }

        }
        saveBufferCounter = 0;
    }

    return true;
}

/* ----- INTERNAL FUNCTIONS ----- */

/*!
 *  @brief Loads the flags for whether each item was taken from the save file
 */
void crateLoadListener()
{
    // just allocate enough memory for this
    char key[] = "takenFlagXX";
    for (size_t i = 0; i < RAND_ITEM_TAKEN_FLAGS_SIZE; i++)
    {
        sprintf(key, "takenFlag%u", i);
        aererr = AER_TRY;
        double flags = AERSaveGetDouble(key);
        switch (aererr) {
            case AER_OK:
                // Assume we got the correct seed
                itemTakenFlags[i] = flags; // cast into uint32_t
                AERLogInfo("Loaded flag %s: %f!", key, flags);
                break;
            case AER_FAILED_LOOKUP:
                break;
            case AER_FAILED_PARSE:
                break;
            default:
                AERLogErr("Getting Randomizer Seed failed unexpectedly");
                abort();
        }
    }
}

/*!
 *  @brief Handles all checks for spawning a randomized item
 *
 *  @param[in] oldItem  original item data (randomItemInfo_t)
 *  @param[in] x        x Coordinate to spawn randomized item
 *  @param[in] y        y Coordinate to spawn randomized item
 */
void checkCrateSpawn(randomItemInfo_t oldItem, float x, float y)
{
    // Get the item index in our array
    int32_t oldItemIdx = getItemIndex(&oldItem);
    if (oldItemIdx < 0)
    {
        AERLogErr("Index for item not found! Type: %u, ID: %u, Room %u", oldItem.data.type, oldItem.data.identifier, oldItem.data.room_id);
        return;
    }
    // Check if the item was taken before
    if (checkItemTakenFlag(oldItemIdx))
        // This item has already been taken
        return;

    // Create new instance
    AERInstance* inst = AERInstanceCreate(crateObjectIdx, x, y+1); // offset y by 1 to fix rendering issue (sprite is already offset to compensate)
    
    // Copy over our item info for what this object will spawn on its destruction
    AERInstanceCreateModLocal(inst, "randomItemIdx", true, NULL)->u = oldItemIdx;
    AERLogInfo("Random crate created, original item: %u, identifier: %u, room_id: %u", oldItem.data.type, oldItem.data.identifier, oldItem.data.room_id);

    // Set the correct render depth
    AERInstanceSyncDepth(inst);
    AERInstanceSetSpriteSpeed(inst, 0.05);
}

/*!
 *  @brief Registers the crate object in HLD
 */
void registerCrateObjects()
{
    crateObjectIdx = AERObjectRegister("random_crate", AER_OBJECT_DESTRUCTABLE, crateSpriteIdx, crateMaskIdx,
        0, true, true, false);
}

/*!
 *  Attaches the function for when a randomized crate is destroyed
 */
void registerCrateObjectListeners()
{
    AERObjectAttachDestroyListener(crateObjectIdx, destroyCrateListener);
    AERObjectAttachDestroyListener(AER_OBJECT_CHAR, charDestroyListener);
}

/*!
 *  @brief Registers the randomized crate sprite and hit mask
 */
void registerCrateSprites()
{
    crateSpriteIdx = AERSpriteRegister("random_crate", "sprites/rando_box.png", 8, 8, 13);
    crateMaskIdx = AERSpriteRegister("random_crate_mask", "sprites/rando_box_hit_mask.png", 1, 8, 13);
}

/*!
 *  @brief Saves the taken crate flags into the savefile
 */
void crateSaveListener()
{
    // Set the keys flags
    for (size_t i = 0; i < saveBufferCounter; i++)
        setItemTakenFlag(saveBuffer[i]);

    // reset the counter
    saveBufferCounter = 0;

    // Put into save file
    char key[] = "takenFlagXX";
    for (size_t i = 0; i < RAND_ITEM_TAKEN_FLAGS_SIZE; i++)
    {
        sprintf(key, "takenFlag%u", i);
        AERSaveSetDouble(key, itemTakenFlags[i]);
        AERLogInfo("Saved flag %s: %u!", key, itemTakenFlags[i]);
    }
}