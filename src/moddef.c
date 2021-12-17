/*!
 *  @file moddef.c
 *
 *  Contains mod definition and breaks out all hooks in the MRE used in this mod
 */
#include "aer/object.h"
#include "aer/log.h"
#include "aer/room.h"
#include "moddef.h"
#include "export.h"

#include "logic.h"
#include "crate.h"
#include "vanilla_handler.h"
#include "indicator.h"

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

    // Indicator
    registerIndicatorObjectListeners();

    return;
}

/*!
 *  @brief Registers all objects created in this mod
 */
static void registerObjects()
{
    // Register each object
    registerCrateObjects();
    registerIndicatorObjects();
    
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
static void gameLoadListener(int32_t __attribute__((unused))curSlotIdx)
{
    logicGameLoadListener();
    crateLoadListener();
    return;
}

/*!
 *  @brief This function is called upon every room transition
 */
static void roomChangeListener(int32_t newRoomIdx, int32_t __attribute__((unused))prevRoomIdx)
{
    checkForNewGame(newRoomIdx);
    vanillaRoomListener(newRoomIdx);

    return;
}

/*!
 *  @brief This function is called upon each save event
 */
static void gameSaveListener(int32_t __attribute__((unused))curSlotIdx)
{
    crateSaveListener();
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