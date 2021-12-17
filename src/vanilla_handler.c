/*!
 * @file vanilla_handler.c
 *
 * This file contains all functions for handling the vanilla game objects
 * ie. Removing the vanilla items and calling the appropriate functions to
 * spawn randomized items.
 */
#include "aer/object.h"
#include "aer/log.h"
#include "aer/room.h"

#include "primitive.h"
#include "logic.h"
#include "crate.h"
#include <stdlib.h>

int32_t currentRoom; // copy of the current room variable (kept up to date via a listener)

/*!
 *  @brief Function called upon a gear crate being created to break it.
 *
 *  @param[in] event        Type of event for which this function was called
 *  @param[in] target       AERInstance pointer to the gear crate
 *  @param[in] other        unused
 * 
 *  @return Determines if the creation event is cancelled (should always returns true)
 */
static bool gearbitCrateCreatedListener(AEREvent *event, AERInstance *target, AERInstance *other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;

    // Dont do anything if randomizer isnt enabled
    if(!randomizer_enabled)
        return true;

    // We cannot destroy this instance using any destroy method, since
    // this would not cause the gearbit to spawn. This probably isnt the best method

    // Mute the crate so no sound is heard
    AERInstanceGetHLDLocal(target, "muteObject")->d = 1;

    // Spawn a bullet in the crate to destroy it
    float x, y;
    AERInstanceGetPosition(target, &x, &y);

    // Set the bullet to not move (could sometimes pass through crate otherwise)
    AERInstance* bullet = AERInstanceCreate(AER_OBJECT_DIAMONDBULLET, x, y);
    AERInstanceSetMotion(bullet, 0, 0);

    // do not cancel the creation event
    return true;
}

/*!
 *  @brief Function called when a object we are randomizing is created. Sets a timer for the next tick
 *
 *  @param[in] event        Type of event for which this function was called
 *  @param[in] target       AERInstance pointer to instance of interest
 *  @param[in] other        unused
 * 
 *  @return Determines if the creation event is cancelled (should always returns true)
 */
static bool itemCreatedListener(AEREvent *event, AERInstance *target, AERInstance *other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;

    // Dont set our alarm if the randomizer isnt enabled
    if(!randomizer_enabled)
        return true;

    // Set an alarm in the next tick
    AERInstanceSetAlarm(target, 0, 1);
    return true;
}

/*
static void logItemToText(randomItemInfo_t item)
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
 *  @brief Function called after a gearbit is fully created. This will destroy it and update the randomizer info
 *
 *  @param[in] event        Type of event for which this function was called
 *  @param[in] target       AERInstance pointer to instance of interest
 *  @param[in] other        unused
 * 
 *  @return Determines if the creation event is cancelled (should always returns true)
 */
static bool gearbitAlarmListener(AEREvent *event, AERInstance *target, AERInstance *other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;

    // Get information about what gearbit just spawned
    uint32_t identifier;
    Primitive* cid = (Primitive*) AERInstanceGetHLDLocal(target, "cid");
    Primitive* linkID = (Primitive*) AERInstanceGetHLDLocal(target, "linkID");

    // there are multiple ways that gearbits can be spawned, determining the source
    if (linkID->type == PRIMITIVE_ARRAY)
    {
        // This gearbit is being held by an enemy (not boss)
        HLDPrimitiveArray* array = (HLDPrimitiveArray*) linkID->value.p;
        
        // This gets the spawner instance for this gearbit
        AERLocal* spawner = AERInstanceGetById(array->subArrays->elements[0].value.r);

        // We will use the cid for this spawner, since this is constant and unique (between spawners at least)
        identifier = AERInstanceGetHLDLocal(spawner, "cid")->d;
    }
    else if (cid->type == PRIMITIVE_STRING)
    {
        // This gearbit was spawned by a boss
        const char* name = cid->value.s->chars;

        // do a quick check that the string isnt empty
        if (name[0] == '\0')
        {
            // This is the termination char, meaning our string isnt valid?
            AERLogErr("Boss gearbit string is empty!");
            return true;
        }

        // ignore the first letter in the string, and convert directly to int
        identifier = atoi(&name[1]);
    }
    else
    {
        // this gearbit must have been held by a crate
        identifier = AERInstanceGetHLDLocal(target, "crateCID")->d;
    }

    float x, y;
    AERInstanceGetPosition(target, &x, &y);

    // Call a general handler to spawn our randomized objects
    checkCrateSpawn((randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = identifier, .room_id = currentRoom}}, x, y);

    // Now cancel the creation event, we do not want any gearbits spawning this way
    AERInstanceDestroy(target);
    return true;
}

/*!
 *  @brief Function called after a weapon skeleton is fully created. This will destroy it and update the randomizer info
 *
 *  @param[in] event        Type of event for which this function was called
 *  @param[in] target       AERInstance pointer to instance of interest
 *  @param[in] other        unused
 * 
 *  @return Determines if the creation event is cancelled (should always returns true)
 */
static bool weaponAlarmListener(AEREvent *event, AERInstance *target, AERInstance *other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;

    // get the weapon local
    AERLocal* weapon = AERInstanceGetHLDLocal(target, "weapon");
    float x, y;
    AERInstanceGetPosition(target, &x, &y);

    checkCrateSpawn((randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = weapon->d, .room_id = currentRoom}}, x, y);

    AERInstanceDestroy(target);
    
    return true;
}

/*!
 *  @brief Function called after a key skeleton is fully created. This will destroy it and update the randomizer info
 *
 *  @param[in] event        Type of event for which this function was called
 *  @param[in] target       AERInstance pointer to instance of interest
 *  @param[in] other        unused
 * 
 *  @return Determines if the creation event is cancelled (should always returns true)
 */
static bool keyAlarmListener(AEREvent *event, AERInstance *target, AERInstance *other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;
    
    AERLocal* cid = AERInstanceGetHLDLocal(target, "cid");
    
    float x, y;
    AERInstanceGetPosition(target, &x, &y);

    // get the cid local
    AERLogInfo("Got key (uint) %lu", (uint32_t)cid->d);

    checkCrateSpawn((randomItemInfo_t){.data = {.type = ITEM_KEY, .identifier = cid->d, .room_id = currentRoom}}, x, y);

    AERInstanceDestroy(target);
    return true;
}

/*!
 *  @brief Registers alarms for all of the objects which will be replaced by this mod
 */
void registerVanillaObjectListeners()
{
    // Replacement Listeners
    AERObjectAttachCreateListener(AER_OBJECT_GEARBIT, itemCreatedListener);
    AERObjectAttachAlarmListener(AER_OBJECT_GEARBIT, 0, gearbitAlarmListener);

    AERObjectAttachCreateListener(AER_OBJECT_DRIFTERBONES_WEAPON, itemCreatedListener);
    AERObjectAttachAlarmListener(AER_OBJECT_DRIFTERBONES_WEAPON, 0, weaponAlarmListener);

    AERObjectAttachCreateListener(AER_OBJECT_DRIFTERBONES_KEY, itemCreatedListener);
    AERObjectAttachAlarmListener(AER_OBJECT_DRIFTERBONES_KEY, 0, keyAlarmListener);

    AERObjectAttachCreateListener(AER_OBJECT_GEARBITCRATE, gearbitCrateCreatedListener);

    return;
}

/*!
 *  @brief Function to simply keep the static currentRoom variable up to date
 */
void vanillaRoomListener(int32_t newRoomIdx)
{
    // Update our local variable with the current room index
    currentRoom = newRoomIdx;
}