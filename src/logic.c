#include "aer/object.h"
#include "aer/log.h"
#include "aer/sprite.h"

#include "export.h"

#include "crate.h"

static const randomItemInfo_t originalItems[] = 
{
    /*      GEARBITS       */
    {.data = {.type = ITEM_GEARBIT, .identifier = 148}}, // Room ID: 62
    {.data = {.type = ITEM_GEARBIT, .identifier = 526}}, // Room ID: 112 <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 703}}, // Room ID: 84
    {.data = {.type = ITEM_GEARBIT, .identifier = 834}}, // Room ID: 85
    {.data = {.type = ITEM_GEARBIT, .identifier = 981}}, // Room ID: 61
    {.data = {.type = ITEM_GEARBIT, .identifier = 1544}}, // Room ID: 107
    {.data = {.type = ITEM_GEARBIT, .identifier = 1626}}, // Room ID: 86
    {.data = {.type = ITEM_GEARBIT, .identifier = 1852}}, // Room ID: 109
    {.data = {.type = ITEM_GEARBIT, .identifier = 2434}}, // Room ID: 85
    {.data = {.type = ITEM_GEARBIT, .identifier = 2698}}, // Room ID: 92
    {.data = {.type = ITEM_GEARBIT, .identifier = 3022}}, // Room ID: 108
    {.data = {.type = ITEM_GEARBIT, .identifier = 3045}}, // Room ID: 112 <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 3050}}, // Room ID: 104
    {.data = {.type = ITEM_GEARBIT, .identifier = 3076}}, // Room ID: 90
    {.data = {.type = ITEM_GEARBIT, .identifier = 3260}}, // Room ID: 93
    {.data = {.type = ITEM_GEARBIT, .identifier = 3278}}, // Room ID: 119
    {.data = {.type = ITEM_GEARBIT, .identifier = 3357}}, // Room ID: 113 <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 3446}}, // Room ID: 117
    {.data = {.type = ITEM_GEARBIT, .identifier = 3594}}, // Room ID: 118
    {.data = {.type = ITEM_GEARBIT, .identifier = 3815}}, // Room ID: 118
    {.data = {.type = ITEM_GEARBIT, .identifier = 3819}}, // Room ID: 106
    {.data = {.type = ITEM_GEARBIT, .identifier = 3834}}, // Room ID: 98
    {.data = {.type = ITEM_GEARBIT, .identifier = 3972}}, // Room ID: 96
    {.data = {.type = ITEM_GEARBIT, .identifier = 3981}}, // Room ID: 93 <- north gun req
    {.data = {.type = ITEM_GEARBIT, .identifier = 4000}}, // Room ID: 92
    {.data = {.type = ITEM_GEARBIT, .identifier = 4059}}, // Room ID: 119
    {.data = {.type = ITEM_GEARBIT, .identifier = 4377}}, // Room ID: 100
    {.data = {.type = ITEM_GEARBIT, .identifier = 4386}}, // Room ID: 53
    {.data = {.type = ITEM_GEARBIT, .identifier = 4826}}, // Room ID: 90
    {.data = {.type = ITEM_GEARBIT, .identifier = 5164}}, // Room ID: 112 <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 5232}}, // Room ID: 106
    {.data = {.type = ITEM_GEARBIT, .identifier = 5312}}, // Room ID: 93
    {.data = {.type = ITEM_GEARBIT, .identifier = 5918}}, // Room ID: 104
    {.data = {.type = ITEM_GEARBIT, .identifier = 5927}}, // Room ID: 103
    {.data = {.type = ITEM_GEARBIT, .identifier = 6153}}, // Room ID: 84
    {.data = {.type = ITEM_GEARBIT, .identifier = 6649}}, // Room ID: 86
    {.data = {.type = ITEM_GEARBIT, .identifier = 6788}}, // Room ID: 119
    {.data = {.type = ITEM_GEARBIT, .identifier = 7037}}, // Room ID: 114 <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 7179}}, // Room ID: 89
    {.data = {.type = ITEM_GEARBIT, .identifier = 7430}}, // Room ID: 113 <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 7600}}, // Room ID: 111 <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 7692}}, // Room ID: 100
    {.data = {.type = ITEM_GEARBIT, .identifier = 7840}}, // Room ID: 62
    {.data = {.type = ITEM_GEARBIT, .identifier = 7940}}, // Room ID: 89
    {.data = {.type = ITEM_GEARBIT, .identifier = 7985}}, // Room ID: 94
    
    /*      BOSS GEARBITS       */
    {.data = {.type = ITEM_GEARBIT, .identifier = 4293967297}}, // Room ID: 121
    {.data = {.type = ITEM_GEARBIT, .identifier = 4293967297}}, // Room ID: 121
    {.data = {.type = ITEM_GEARBIT, .identifier = 4293967297}}, // Room ID: 121

    /*      KEYS        */
    {.data = {.type = ITEM_KEY, .identifier = 26}}, // Room ID: 53
    {.data = {.type = ITEM_KEY, .identifier = 164}}, // Room ID: 100
    {.data = {.type = ITEM_KEY, .identifier = 4902}}, // Room ID: 113 <- north gun req (8 modules)
    {.data = {.type = ITEM_KEY, .identifier = 7232}}, // Room ID: 92

    /*      WEAPONS        */
    {.data = {.type = ITEM_WEAPON, .identifier = 1}}, // Room ID: 48
    {.data = {.type = ITEM_WEAPON, .identifier = 21}}, // Room ID: 121
};    