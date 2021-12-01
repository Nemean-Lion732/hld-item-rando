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
    oldItem.data.type++;
    // Check if item was taken before
    // DO LATER

    // Swap the randomized item to the new one
    // DO LATER
    randomItemInfo_t newItem = {
        .data = {.type = ITEM_KEY, .identifier = 10101010}
    };

    // Spawn object
    createRandomCrate(newItem, x, y);
}

/*!
 *  @brief Handler function when a gearbit is created
 *
 *  @param[in] event    event context.
 *  @param[in] target   AERInstance pointer to the instance created
 *  @param[in] other    Unused
 * 
 *  @return Determines if the creation event is cancelled (should always returns false)
 */
static bool gearbitCreatedListener(AEREvent *event, AERInstance *target, __attribute__((unused))AERInstance *other)
{
    // Get information about what gearbit just spawned
    AERLocal *pCrateID = AERInstanceGetHLDLocal(event, "crateCID");

    if (pCrateID == NULL)
    {
        // This should never happen, we will check anyways
        AERLogErr("Could not get crateCID from created gearbit!");
        return true;
    }

    float x, y;
    AERInstanceGetPosition(target, &x, &y);

    // Call a general handler to spawn our randomized objects
    checkRandomizerSpawn((randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = pCrateID->u}}, x, y);
    
    // Now cancel the creation event, we do not want any gearbits spawning this way
    return false;
}

/*!
 *  @brief Registers alarms for all of the objects which will be replaced by this mod
 */
static void registerObjectListeners()
{
    // Replacement Listeners
    AERObjectAttachCreateListener(AER_OBJECT_GEARBIT, gearbitCreatedListener);
    // AERObjectAttachCreateListener(AER_OBJECT_DRIFTERBONES_WEAPON, weaponCreatedListener);
    // AERObjectAttachCreateListener(AER_OBJECT_DRIFTERBONES_KEY, keyCreatedListener);
    AERLogInfo("Object alarms registered");
    return;
}

/*!
 *  @brief Registers all objects created in this mod
 */
static void registerObjects()
{
    // Register each object
    registerCrateObject();

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