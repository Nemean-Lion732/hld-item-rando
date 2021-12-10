//
#include <stdlib.h>
#include "aer/object.h"
#include "aer/log.h"
#include "aer/room.h"
#include "moddef.h"
#include "export.h"

#include "logic.h"
#include "crate.h"

int32_t currentRoom;

/* ----- PRIVATE FUNCTIONS ----- */

/*
static void logItemText(randomItemInfo_t item)
{
    FILE *f;
    f = fopen("logic.txt", "a");
    char *typeName = malloc(16);
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
    fprintf(f, "{.data = {.type = %s, .identifier = %u, .room_id: %i}}\n", typeName, item.data.identifier, currentRoom);
    free(typeName);
    fclose(f);
}
*/

/*!
 *  @brief Determines if a randomized item should be spawned
 *
 *  @param[in] type     original item category (ex. ITEM_GEARBIT)
 *  @param[in] itemID   original item number (ex. crateCID) 
 *  @param[in] x        x Coordinate to spawn randomized item
 *  @param[in] y        y Coordinate to spawn randomized item
 * 
 *  @return Determines if the creation event is cancelled (should always returns false)
 */
static void checkRandomizerSpawn(randomItemInfo_t oldItem, float x, float y)
{
    // TO DO: Implement checks to see if this crate was opened before
    // logItemText(oldItem);
    // Spawn object
    createRandomCrate(oldItem, x, y);
}

static bool gearbitCrateCreatedListener(AEREvent *event, AERInstance *target, AERInstance *other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;

    // Mute the crate so no sound is heard
    AERInstanceGetHLDLocal(target, "muteObject")->d = 1;

    // Spawn a bullet in the crate to destroy it
    float x, y;
    AERInstanceGetPosition(target, &x, &y);

    AERInstance* bullet = AERInstanceCreate(AER_OBJECT_DIAMONDBULLET, x, y);
    AERInstanceSetMotion(bullet, 0, 0);
    return true;
}

static bool itemCreatedListener(AEREvent *event, AERInstance *target, AERInstance *other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;

    // Set an alarm in the next tick
    AERInstanceSetAlarm(target, 0, 1);
    return true;
}

static bool gearbitAlarmListener(AEREvent *event, AERInstance *target, AERInstance *other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;

    // Get information about what gearbit just spawned
    AERLocal* crateCID = AERInstanceGetHLDLocal(target, "crateCID");

    AERLogInfo("Got Gearbit %e", crateCID->d);
    
    uint16_t cid = crateCID->d; // round
    // The above check will not work if the gearbit is being held by an enemy
    // My "solution" is to try to use the spawner cid instead of the crateCID, I pray these are unique, otherwise we may need to go by room num
    AERLocal* inEnemy = AERInstanceGetHLDLocal(target, "inEnemy");
    if (inEnemy->d > 0)
    {
        // The CID is not correct, use the spawners CID
        AERInstance* spawner = AERInstanceGetById((int32_t)inEnemy->d);
        if (spawner == NULL)
        {
            // This should never happen, we will check anyways
            AERLogInfo("Could not get spawner instance from held gearbit");
            return true;
        }

        // Finally, use the cid
        cid = AERInstanceGetHLDLocal(spawner, "cid")->d;
    }
    float x, y;
    AERInstanceGetPosition(target, &x, &y);

    // Call a general handler to spawn our randomized objects
    checkRandomizerSpawn((randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = cid, .room_id = currentRoom}}, x, y);
    
    AERLogInfo("Got Gearbit (uint) %lu", cid);
    // Now cancel the creation event, we do not want any gearbits spawning this way
    AERInstanceDestroy(target);
    return true;
}

static bool weaponAlarmListener(AEREvent *event, AERInstance *target, AERInstance *other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;
    
    AERLocal* weapon = AERInstanceGetHLDLocal(target, "weapon");
    
    float x, y;
    AERInstanceGetPosition(target, &x, &y);

    AERLogInfo("Got weapon (uint) %lu", (uint16_t)weapon->d);

    checkRandomizerSpawn((randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = weapon->d, .room_id = currentRoom}}, x, y);

    AERInstanceDestroy(target);

    // Do a check here to see if the player needs a map
    if (currentRoom == AER_ROOM_IN_03_TUT_COMBAT)
    {
        createRandomizedIndexes();
        // We are in the tutorial room. We need the drifter to be able to open the map to equip their items
        AERInstance* data_obj;
        if (AERInstanceGetByObject(AER_OBJECT_DATA, false, 1, &data_obj) > 0)
            // the data instance is valid
            AERInstanceGetHLDLocal(data_obj, "playerHasMap")->d = 1;
        else 
            AERLogErr("Randomizer could not equip map to player in tutorial, player is softlocked!");
    }
    return true;
}

static bool keyAlarmListener(AEREvent *event, AERInstance *target, AERInstance *other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;
    
    AERLocal* cid = AERInstanceGetHLDLocal(target, "cid");
    
    float x, y;
    AERInstanceGetPosition(target, &x, &y);

    AERLogInfo("Got key (uint) %lu", (uint16_t)cid->d);

    checkRandomizerSpawn((randomItemInfo_t){.data = {.type = ITEM_KEY, .identifier = cid->d, .room_id = currentRoom}}, x, y);

    AERInstanceDestroy(target);
    return true;
}


/*!
 *  @brief Registers alarms for all of the objects which will be replaced by this mod
 */
static void registerObjectListeners()
{
    // Replacement Listeners
    AERObjectAttachCreateListener(AER_OBJECT_GEARBIT, itemCreatedListener);
    AERObjectAttachAlarmListener(AER_OBJECT_GEARBIT, 0, gearbitAlarmListener);

    AERObjectAttachCreateListener(AER_OBJECT_DRIFTERBONES_WEAPON, itemCreatedListener);
    AERObjectAttachAlarmListener(AER_OBJECT_DRIFTERBONES_WEAPON, 0, weaponAlarmListener);

    AERObjectAttachCreateListener(AER_OBJECT_DRIFTERBONES_KEY, itemCreatedListener);
    AERObjectAttachAlarmListener(AER_OBJECT_DRIFTERBONES_KEY, 0, keyAlarmListener);

    AERObjectAttachCreateListener(AER_OBJECT_GEARBITCRATE, gearbitCrateCreatedListener);
    
    // Crate
    registerCrateObjectListeners();
    AERLogInfo("Object alarms registered");
    return;
}

/*!
 *  @brief Registers all objects created in this mod
 */
static void registerObjects()
{
    // Register each object
    registerCrateObjects();

    return;
}

/*!
 *  @brief Registers all sprites created in this mod
 */
static void registerSprites()
{
    // Register each sprite
    registerCrateSprites();

    return;
}

static void roomChangeListener(int32_t newRoomIdx, int32_t prevRoomIdx)
{
    // Update our local variable with the current room index
    currentRoom = newRoomIdx;
}

static void gameLoadListener(int32_t curSlotIdx)
{
    createRandomizedIndexes(curSlotIdx);
}
/* ----- PUBLIC FUNCTIONS ----- */

MOD_EXPORT void DefineMod(AERModDef* def) {
    def->registerSprites = registerSprites;
    def->registerObjects = registerObjects;
    def->registerObjectListeners = registerObjectListeners;
    def->roomStartListener = roomChangeListener;
    def->gameLoadListener = gameLoadListener;
    
    return;
}