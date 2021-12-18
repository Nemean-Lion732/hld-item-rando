/*!
 * @file options.c
 *
 * This file contains functions for getting options from the user configuration file
 */

#include "aer/log.h"
#include "aer/conf.h"
#include "aer/err.h"
#include <stdlib.h>

#include "options.h"

options_t options;

#define DEFAULT_ALARM_IDX 0

/* ----- PRIVATE FUNCTIONS ----- */

static uint8_t getAlarmConfig(const char* key)
{
    uint8_t alarmIdx;
    aererr = AER_TRY;
    int64_t alarm_i64 = AERConfGetInt(key);
    switch (aererr) {
        case AER_FAILED_PARSE:
            // User edited config, but we cannot parse it
            AERLogErr("Input Configuration for %s is invalid", key);
            abort();
            break;
        case AER_FAILED_LOOKUP:
            // The user has not changed the default
            alarmIdx = DEFAULT_ALARM_IDX;
            break;
        default:
            // check alarm is valid
            if (alarm_i64 < 0 || alarm_i64 >= 12)
            {
                AERLogErr("Input Configuration for %s is outside bounds", key);
                abort();
            }
            
            alarmIdx = alarm_i64;
            break;
    }
    return alarmIdx;
}

/* ----- INTERNAL FUNCTIONS ----- */

void optionsConstructor()
{
    options.alarms.gearbit = getAlarmConfig("alarm.GEARBIT");
    options.alarms.key_skele = getAlarmConfig("alarm.DRIFTERBONES_KEY");
    options.alarms.weapon_skele = getAlarmConfig("alarm.DRIFTERBONES_WEAPON");
    options.alarms.rando_indicator = getAlarmConfig("alarm.random_indicator");

    return;
}