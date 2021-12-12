/*!
 *  @file moddef.c
 *
 *  Contains mod definition and breaks out all hooks in the MRE used in this mod
 */
#include <stdlib.h>
#include "aer/object.h"
#include "aer/log.h"
#include "aer/room.h"
#include "moddef.h"
#include "export.h"

#include "logic.h"
#include "crate.h"
#include "vanilla_handler.h"

/* ----- PRIVATE FUNCTIONS ----- */

/*!
 *  @brief Registers alarms for all of the objects which will be replaced by this mod
 */
static void registerObjectListeners()
{
    // Replacement Listeners
    registerVanillaObjectListeners();
    
    // Crate
    registerCrateObjectListeners();

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

/*!
 *  @brief Calls this function whenever a save file is loaded.
 *  
 * NOTE: This function is not called upon starting a new game
 */
static void gameLoadListener(int32_t curSlotIdx)
{
    createRandomizedIndexes();
    loadItemTakenFlags();
    return;
}

/*!
 *  @brief This function is called upon every room transition
 */
static void roomChangeListener(int32_t newRoomIdx, int32_t prevRoomIdx)
{
    AERLogInfo("Room Change Event");
    vanillaRoomTracker(newRoomIdx);

    return;
}

/*!
 *  @brief This function is called upon each save event
 */
static void gameSaveListener(int32_t curSlotIdx)
{
    AERLogInfo("Save Event");
    crateSaveEvent();
    return;
}

/* ----- PUBLIC FUNCTIONS ----- */

MOD_EXPORT void DefineMod(AERModDef* def) {
    def->registerSprites = registerSprites;
    def->registerObjects = registerObjects;
    def->registerObjectListeners = registerObjectListeners;
    def->roomStartListener = roomChangeListener;
    def->gameLoadListener = gameLoadListener;
    def->gameSaveListener = gameSaveListener;

    return;
}