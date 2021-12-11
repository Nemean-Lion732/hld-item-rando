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

#include "logic.h"
#include "crate.h"
#include <stdlib.h>

typedef enum PrimitiveType {
    PRIMITIVE_REAL = 0x0,
    PRIMITIVE_STRING = 0x1,
    PRIMITIVE_ARRAY = 0x2,
    PRIMITIVE_PTR = 0x3,
    PRIMITIVE_VEC3 = 0x4,
    PRIMITIVE_UNDEFINED = 0x5,
    PRIMITIVE_OBJECT = 0x6,
    PRIMITIVE_INT32 = 0x7,
    PRIMITIVE_VEC4 = 0x8,
    PRIMITIVE_MATRIX = 0x9,
    PRIMITIVE_INT64 = 0xA,
    PRIMITIVE_ACCESSOR = 0xB,
    PRIMITIVE_NULL = 0xC,
    PRIMITIVE_BOOL = 0xD,
    PRIMITIVE_ITERATOR = 0xE,
} PrimitiveType;

typedef struct PrimitiveString {
    char* chars;
    size_t refs;
    size_t length;
} PrimitiveString;

typedef union __attribute__((aligned(4))) PrimitiveValue {
    uint8_t raw[12];
    double r;
    PrimitiveString* s;
    void* p;
    int32_t i32;
    int64_t i64;
    bool b;
} PrimitiveValue;

typedef struct Primitive {
    PrimitiveValue value;
    PrimitiveType type;
} Primitive;

typedef struct __attribute__((aligned(4))) HLDPrimitiveArray {
    size_t refs;
    struct 
    {
        size_t size; 
        Primitive* elements;
    }* subArrays;
    void* field_8;
    uint32_t field_C;
    size_t numSubArrays;
} HLDPrimitiveArray;

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

    uint32_t identifier;
    // Get information about what gearbit just spawned

    Primitive* cid = (Primitive*) AERInstanceGetHLDLocal(target, "cid");
    Primitive* linkID = (Primitive*) AERInstanceGetHLDLocal(target, "linkID");
    if (linkID->type == PRIMITIVE_ARRAY)
    {
        // This gearbit is being held by an enemy (not boss)
        HLDPrimitiveArray* array = (HLDPrimitiveArray*) linkID->value.p;
        AERLogInfo("LinkID Array size %u, elements %u, first element type %u, element value %f", 
            array->numSubArrays, array->subArrays->size, array->subArrays->elements[0].type, array->subArrays->elements[0].value.r);
        identifier = array->subArrays->elements[0].value.r;
    }
    else if (cid->type == PRIMITIVE_STRING)
    {
        // This gearbit was spawned by a boss
        const char* name = cid->value.s->chars;
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
    checkRandomizerSpawn((randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = identifier, .room_id = currentRoom}}, x, y);
    
    AERLogInfo("Got Gearbit (uint) %lu", identifier);
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

    AERLogInfo("Got weapon (uint) %lu", (uint32_t)weapon->d);

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

void roomChangeListener(int32_t newRoomIdx, int32_t prevRoomIdx)
{
    // Update our local variable with the current room index
    currentRoom = newRoomIdx;
}