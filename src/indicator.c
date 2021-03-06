/*!
 *  @file indicator.c
 *
 *  This file contains code for creating an indicator above the destroyed crate
 */

#include "aer/object.h"
#include "aer/log.h"
#include "aer/sprite.h"

#include "logic.h"
#include "indicator.h"
#include "options.h"

#define INDICATOR_TIMEOUT 100 // ticks until indicator object is removed
static int indicatorObjectIdx;

/*!
 *  @brief Destroys the indicator object when its alarm is triggered
 *
 *  @param[in] event        Type of event for which this function was called
 *  @param[in] target       AERInstance pointer to instance of interest
 *  @param[in] other        unused
 * 
 *  @return Determines if the alarm event is cancelled (should always returns true)
 */
static bool indicatorAlarmListener(AEREvent *event, AERInstance *target, AERInstance *other)
{
    // handle other listeners
    if (!event->handle(event, target, other))
        return false;
    
    AERInstanceDestroy(target);
    return true;
}

/*!
 *  @brief Registers the indicator object
 */
void registerIndicatorObjects()
{
    // Use a depth of -999999 so this object will be displayed above all other objects
    // This may cause some glitches with menus, could be improved in the future
    indicatorObjectIdx = AERObjectRegister("random_indicator", AER_OBJECT_MASTERCLASS, AER_SPRITE_NULL, AER_SPRITE_NULL, 
    -999999, true, false, false);
}

/*!
 *  @brief Attaches the function for when a randomized crate is destroyed
 */
void registerIndicatorObjectListeners()
{
    AERObjectAttachAlarmListener(indicatorObjectIdx, options.alarms.rando_indicator, indicatorAlarmListener);
}

/*!
 *  @brief Creates a few indicator objects to display an item that was just obtained
 *
 *  @param[in] itemInfo         Item information for the item being spawned
 *  @param[in] x                Item x location
 *  @param[in] y                Item y location
 */
void createPickupIndicator(randomItemInfo_t itemInfo, float x, float y)
{
    // Create two objects, one as a black background, the other as the item
    // It seems that spawning the item first always makes it appear on top
    AERInstance* item = AERInstanceCreate(indicatorObjectIdx, x - 14, y - 40);
    AERInstance* bg = AERInstanceCreate(indicatorObjectIdx, x - 14, y - 40);
    AERInstanceSetSprite(bg, AER_SPRITE_HUDWEAPONBG);
    AERInstanceSetSpriteAlpha(bg, 0.75);
    
    switch (itemInfo.data.type)
    {
    case ITEM_GEARBIT:
        AERInstanceSetSprite(item, AER_SPRITE_GEARBITDOUBLENEW);
        AERInstanceSetSpriteFrame(item, 16);
        AERInstanceSetPosition(item, x - 1, y - 30);
        break;
    case ITEM_KEY:
        AERInstanceSetSprite(item, AER_SPRITE_INVDRIFTERKEY);
        AERInstanceSetPosition(item, x - 3, y - 37);
        break;
    case ITEM_WEAPON:
        AERInstanceSetSprite(item, AER_SPRITE_ITEMSGUI);
        AERInstanceSetPosition(item, x - 12, y - 39);
        AERInstanceSetSpriteFrame(item, itemInfo.data.identifier);
        break;
    default:
        // We can just ignore the item in that case, it will still be destroyed
        break;
    }
    AERInstanceSetSpriteSpeed(bg, 0);
    AERInstanceSetSpriteSpeed(item, 0);
    AERInstanceSetAlarm(bg, options.alarms.rando_indicator, INDICATOR_TIMEOUT);
    AERInstanceSetAlarm(item, options.alarms.rando_indicator, INDICATOR_TIMEOUT);
}