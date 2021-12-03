//
#include <stdlib.h>
#include "aer/object.h"
#include "aer/log.h"
#include "moddef.h"
#include "export.h"

#include "crate.h"

/* ----- PRIVATE FUNCTIONS ----- */

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

    // Spawn object
    createRandomCrate(oldItem, x, y);
}

/*!
 *  @brief Handler function when a gearbit is created
 *
 *  @param[in] event    event context.
 *  @param[in] target   AERInstance pointer to the instance created
 *  @param[in] other    Unused
 * 
 *  @return true / false whether the creation continues or not
 */
static bool gearbitCreatedListener(AEREvent *event, AERInstance *target, AERInstance *other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;

    // We need to wait 1 tick for get the gearbit data

    // Set an alarm in the next tick
    AERInstanceSetAlarm(target, 0, 1);
    return true;
}

static bool gearbitCrateCreatedListener(AEREvent *event, AERInstance *target, AERInstance *other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;

    // TO DO: Find a way to break this box automatically to spawn a gearbit
    return true;
}

static bool gearbitAlarmListener(AEREvent *event, AERInstance *target, AERInstance *other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;

    // Get information about what gearbit just spawned
    AERLocal* pCrateID = AERInstanceGetHLDLocal(target, "crateCID");
    
    if (pCrateID == NULL)
    {
        // This should never happen, we will check anyways
        AERLogInfo("Could not get crateCID from created gearbit!");
        return true;
    }
    AERLogInfo("Got Gearbit %e", pCrateID->d);

    float x, y;
    AERInstanceGetPosition(target, &x, &y);

    // Call a general handler to spawn our randomized objects
    checkRandomizerSpawn((randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = (uint32_t) pCrateID->d}}, x, y);
    
    AERLogInfo("Got Gearbit (uint) %lu", (uint32_t) pCrateID->d);
    // Now cancel the creation event, we do not want any gearbits spawning this way
    AERInstanceDestroy(target);
    return true;
}

/*!
 *  @brief Registers alarms for all of the objects which will be replaced by this mod
 */
static void registerObjectListeners()
{
    // Replacement Listeners
    AERObjectAttachCreateListener(AER_OBJECT_GEARBIT, gearbitCreatedListener);
    AERObjectAttachCreateListener(AER_OBJECT_GEARBITCRATE, gearbitCrateCreatedListener);
    AERObjectAttachAlarmListener(AER_OBJECT_GEARBIT, 0, gearbitAlarmListener);
    // AERObjectAttachCreateListener(AER_OBJECT_DRIFTERBONES_WEAPON, weaponCreatedListener);
    // AERObjectAttachCreateListener(AER_OBJECT_DRIFTERBONES_KEY, keyCreatedListener);

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

/* ----- PUBLIC FUNCTIONS ----- */

MOD_EXPORT void DefineMod(AERModDef* def) {
    def->registerSprites = registerSprites;
    def->registerObjects = registerObjects;
    def->registerObjectListeners = registerObjectListeners;

    return;
}