/*!
 *  @file logic.c
 *
 *  Contains functions used for randomizing the items in HLD without softlocking items or duplication
 */
#include "aer/log.h"
#include "aer/rand.h"
#include "aer/save.h"
#include "aer/err.h"
#include "aer/event.h"
#include "aer/room.h"
#include "aer/object.h"

#include <stdio.h>
#include <stdlib.h>
#include "export.h"

#include "logic.h"
#include "crate.h"
#include "options.h"

static size_t randomizedIndexes[RAND_MAP_SIZE] = {0};
static const randomItemInfo_t randomItemOrderedMap[RAND_MAP_SIZE] = 
{
    /*      GEARBITS       */
    {.data = {.type = ITEM_GEARBIT, .identifier = 4386, .room_id = 53}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 981, .room_id = 61}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 2287, .room_id = 61}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3525, .room_id = 61}}, // <- 12 keys req
    {.data = {.type = ITEM_GEARBIT, .identifier = 148, .room_id = 62}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7840, .room_id = 62}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6128, .room_id = 63}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6477, .room_id = 63}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 524, .room_id = 64}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6433, .room_id = 64}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 2360, .room_id = 65}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 5622, .room_id = 67}}, // <- chain dash req
    {.data = {.type = ITEM_GEARBIT, .identifier = 6378, .room_id = 71}}, // <- soccer kid
    {.data = {.type = ITEM_GEARBIT, .identifier = 703, .room_id = 84}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6153, .room_id = 84}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 834, .room_id = 85}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 2434, .room_id = 85}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 1626, .room_id = 86}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6649, .room_id = 86}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3256, .room_id = 87}}, // <- 10 keys req
    {.data = {.type = ITEM_GEARBIT, .identifier = 6387, .room_id = 87}}, // <- 10 keys req
    {.data = {.type = ITEM_GEARBIT, .identifier = 7179, .room_id = 89}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7940, .room_id = 89}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3076, .room_id = 90}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 4826, .room_id = 90}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 2698, .room_id = 92}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 4000, .room_id = 92}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3260, .room_id = 93}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3981, .room_id = 93}}, // <- north gun req
    {.data = {.type = ITEM_GEARBIT, .identifier = 5312, .room_id = 93}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7985, .room_id = 94}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3972, .room_id = 96}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3834, .room_id = 98}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 4377, .room_id = 100}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7692, .room_id = 100}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 5927, .room_id = 103}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3050, .room_id = 104}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 5918, .room_id = 104}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3819, .room_id = 106}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 5232, .room_id = 106}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 1544, .room_id = 107}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3022, .room_id = 108}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 1852, .room_id = 109}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7600, .room_id = 111}}, // <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 526, .room_id = 112}}, // <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 3045, .room_id = 112}}, // <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 5164, .room_id = 112}}, // <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 3357, .room_id = 113}}, // <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 7430, .room_id = 113}}, // <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 7037, .room_id = 114}}, // <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 3446, .room_id = 117}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3594, .room_id = 118}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3815, .room_id = 118}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3278, .room_id = 119}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 4059, .room_id = 119}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6788, .room_id = 119}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 12110931, .room_id = 121}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 12110932, .room_id = 121}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 12110933, .room_id = 121}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 5130, .room_id = 129}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6143, .room_id = 130}}, // <- chain dash req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 6877, .room_id = 130}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7015, .room_id = 139}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7569, .room_id = 139}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7717, .room_id = 139}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3664, .room_id = 140}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 4919, .room_id = 142}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 14363901, .room_id = 143}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 14363902, .room_id = 143}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 14363903, .room_id = 143}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 252, .room_id = 147}}, // <- 16 keys req
    {.data = {.type = ITEM_GEARBIT, .identifier = 717, .room_id = 147}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 212, .room_id = 148}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 15051971, .room_id = 150}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 15051972, .room_id = 150}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 15051973, .room_id = 150}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 5908, .room_id = 152}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 1742, .room_id = 154}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 4759, .room_id = 154}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 4991, .room_id = 154}}, // <- north gun req
    {.data = {.type = ITEM_GEARBIT, .identifier = 653, .room_id = 155}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 5778, .room_id = 155}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 15720771, .room_id = 157}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 15720772, .room_id = 157}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 15720773, .room_id = 157}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 5780, .room_id = 158}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3615, .room_id = 160}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3993, .room_id = 160}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 284, .room_id = 161}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 2527, .room_id = 161}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 5807, .room_id = 161}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 5934, .room_id = 162}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 1338, .room_id = 163}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 16436761, .room_id = 164}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 16436762, .room_id = 164}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 16436763, .room_id = 164}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 2391, .room_id = 171}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7675, .room_id = 171}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 1964, .room_id = 172}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7623, .room_id = 172}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 1329, .room_id = 175}}, // <- chain dash req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 7304, .room_id = 175}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 1806, .room_id = 178}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3969, .room_id = 178}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 1136, .room_id = 181}}, // <- chain dash req
    {.data = {.type = ITEM_GEARBIT, .identifier = 1729, .room_id = 181}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3854, .room_id = 181}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6124, .room_id = 181}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 9324, .room_id = 181}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6082, .room_id = 182}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6660, .room_id = 182}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3647, .room_id = 183}}, // <- 1 key required
    {.data = {.type = ITEM_GEARBIT, .identifier = 6266, .room_id = 183}}, // <- 1 key required
    {.data = {.type = ITEM_GEARBIT, .identifier = 646, .room_id = 185}}, // <- chain dash req, north gun req
    {.data = {.type = ITEM_GEARBIT, .identifier = 5821, .room_id = 185}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 717, .room_id = 187}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6000, .room_id = 187}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6774, .room_id = 187}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 910, .room_id = 188}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7889, .room_id = 188}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 2764, .room_id = 189}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 4304, .room_id = 190}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 5065, .room_id = 190}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3042, .room_id = 193}}, // <- 3 key required
    {.data = {.type = ITEM_GEARBIT, .identifier = 3057, .room_id = 193}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3083, .room_id = 193}}, // <- 3 key required
    {.data = {.type = ITEM_GEARBIT, .identifier = 4511, .room_id = 193}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6231, .room_id = 193}}, // <- 3 key required
    {.data = {.type = ITEM_GEARBIT, .identifier = 7433, .room_id = 193}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 2572, .room_id = 194}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 2963, .room_id = 194}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6007, .room_id = 194}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 19543221, .room_id = 195}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 19543222, .room_id = 195}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 19543223, .room_id = 195}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 1224, .room_id = 198}}, // <- chain dash req
    {.data = {.type = ITEM_GEARBIT, .identifier = 1847, .room_id = 199}}, // <- chain dash req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 3596, .room_id = 199}}, // <- chain dash req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 4551, .room_id = 199}}, // <- chain dash req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 2563, .room_id = 200}}, // <- chain dash req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 1879, .room_id = 209}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 1314, .room_id = 211}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7796, .room_id = 211}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 4386, .room_id = 212}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3143, .room_id = 213}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 5276, .room_id = 214}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7330, .room_id = 214}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 2753, .room_id = 215}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 1172, .room_id = 218}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 4111, .room_id = 218}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 4175, .room_id = 218}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 72, .room_id = 222}}, // <- 8 modules
    {.data = {.type = ITEM_GEARBIT, .identifier = 7096, .room_id = 222}}, // <- 8 modules
    {.data = {.type = ITEM_GEARBIT, .identifier = 2703, .room_id = 225}}, // north gun req
    {.data = {.type = ITEM_GEARBIT, .identifier = 3494, .room_id = 225}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 339, .room_id = 226}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 4969, .room_id = 226}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7134, .room_id = 226}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7631, .room_id = 226}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 4060, .room_id = 227}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 1526, .room_id = 229}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 2938, .room_id = 229}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 1069, .room_id = 230}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6585, .room_id = 230}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 2125, .room_id = 231}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6014, .room_id = 231}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 5916, .room_id = 232}}, // <- 12 keys req
    {.data = {.type = ITEM_GEARBIT, .identifier = 3364, .room_id = 233}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7455, .room_id = 233}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 8, .room_id = 234}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 2220, .room_id = 234}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 2034, .room_id = 238}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6516, .room_id = 239}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 808, .room_id = 241}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3195, .room_id = 241}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 1524, .room_id = 242}}, // <- 4 keys req
    {.data = {.type = ITEM_GEARBIT, .identifier = 1969, .room_id = 242}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 3541, .room_id = 242}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6113, .room_id = 242}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 7410, .room_id = 242}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6751, .room_id = 243}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6177, .room_id = 244}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 24527051, .room_id = 245}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 24527052, .room_id = 245}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 24527053, .room_id = 245}},
    {.data = {.type = ITEM_GEARBIT, .identifier = 6843, .room_id = 246}},

    /*      KEYS        */
    {.data = {.type = ITEM_KEY, .identifier = 26, .room_id = 53}},
    {.data = {.type = ITEM_KEY, .identifier = 7232, .room_id = 92}},
    {.data = {.type = ITEM_KEY, .identifier = 164, .room_id = 100}},
    {.data = {.type = ITEM_KEY, .identifier = 4902, .room_id = 113}}, // <- north gun req (8 modules)
    {.data = {.type = ITEM_KEY, .identifier = 1309, .room_id = 129}},
    {.data = {.type = ITEM_KEY, .identifier = 941, .room_id = 142}},
    {.data = {.type = ITEM_KEY, .identifier = 5016, .room_id = 148}},
    {.data = {.type = ITEM_KEY, .identifier = 3615, .room_id = 155}},
    {.data = {.type = ITEM_KEY, .identifier = 78, .room_id = 185}}, // <- chain dash req, north gun req
    {.data = {.type = ITEM_KEY, .identifier = 5867, .room_id = 187}},
    {.data = {.type = ITEM_KEY, .identifier = 4401, .room_id = 189}},
    {.data = {.type = ITEM_KEY, .identifier = 6662, .room_id = 193}},
    {.data = {.type = ITEM_KEY, .identifier = 5886, .room_id = 225}}, // north gun req
    {.data = {.type = ITEM_KEY, .identifier = 4990, .room_id = 229}},
    {.data = {.type = ITEM_KEY, .identifier = 239, .room_id = 238}}, // <- chain dash req
    {.data = {.type = ITEM_KEY, .identifier = 535, .room_id = 242}}, // <- 4 keys req

    /*      WEAPONS        */
    {.data = {.type = ITEM_WEAPON, .identifier = 1, .room_id = 48}},
    {.data = {.type = ITEM_WEAPON, .identifier = 21, .room_id = 121}},
    {.data = {.type = ITEM_WEAPON, .identifier = 23, .room_id = 135}}, // <- chain dash req (8 modules)
    {.data = {.type = ITEM_WEAPON, .identifier = 43, .room_id = 195}},
    {.data = {.type = ITEM_WEAPON, .identifier = 41, .room_id = 200}}, // <- chain dash req (8 modules)
    {.data = {.type = ITEM_WEAPON, .identifier = 2, .room_id = 245}}
};

static size_t takenItemIndexes[RAND_MAP_SIZE];
static size_t takenLocationIndexes[RAND_MAP_SIZE];
static size_t takenCounter = 0;

/* ----- PRIVATE FUNCTIONS ----- */

/*!
 *  @brief Function will abort if the given input map is not ordered
 */
static void ensureMapOrdered()
{
    uint64_t lastItem = randomItemOrderedMap[0].raw;
    uint64_t nextItem;

    for (size_t i = 1; i < RAND_MAP_SIZE; i++)
    {
        nextItem = randomItemOrderedMap[i].raw;

        if (nextItem <= lastItem)
            abort();

        lastItem = nextItem;
    }
}

/*!
 *  @brief Updates the item mapping
 *
 *  @param[in] originalIndex    The original item index (or location)
 *  @param[in] newIndex         Index of the item which will appear at this location
 */
static void updateIndexes(size_t originalIndex, size_t newIndex)
{
    randomizedIndexes[originalIndex] = newIndex;
    takenLocationIndexes[takenCounter] = originalIndex;
    takenItemIndexes[takenCounter] = newIndex;
    takenCounter++;
}

/*!
 *  @brief Checks if a location already has an item
 *
 *  @param[in] idx      the index of the location to check
 *  @return false if location already has item, true if it is empty
 */
static bool checkLocationNotTaken(size_t idx)
{
    for (size_t i = 0; i < takenCounter; i++)
    {
        if (idx == takenLocationIndexes[i])
        {
            return false;
        }
    }
    return true;
}

/*!
 *  @brief Checks if this item is already at a location
 *
 *  @param[in] idx      the index of the item to check
 *  @return false if item already has location, otherwise true
 */
static bool checkItemNotTaken(size_t idx)
{
    for (size_t i = 0; i < takenCounter; i++)
    {
        if (idx == takenItemIndexes[i])
        {
            return false;
        }
    }
    return true;
}

/*!
 *  @brief Check if this is the first weapon location
 *
 *  @param[in] idx      the index of the location to check
 *  @return true if this is the first weapon location, otherwise false
 */
static bool isFirstWeaponLocation(size_t idx)
{
    return (randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = 1, .room_id = 48}}.raw == randomItemOrderedMap[idx].raw;
}

/*!
 *  @brief Check if the given item is valid for the first weapon location
 *
 *  @param[in] idx      the index of the item to check
 *  @return true if we can place this item here, false otherwise
 */
static bool isValidFirstWeapon(size_t idx)
{
    // Only these 4 weapons are allowed for this location
    return randomItemOrderedMap[idx].raw == (randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = 1, .room_id = 48}}.raw
        || randomItemOrderedMap[idx].raw == (randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = 2, .room_id = 245}}.raw
        || randomItemOrderedMap[idx].raw == (randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = 21, .room_id = 121}}.raw
        || randomItemOrderedMap[idx].raw == (randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = 23, .room_id = 135}}.raw;
}

/*!
 *  @brief Check if this location does not needs a sniper to get access
 *
 *  @param[in] idx      the index of the location to check
 *  @return true if this location needs a sniper, false otherwise
 */
static bool doesLocationNotRequireSniper(size_t idx)
{
    // we want to return true for all locations except these 14
    return randomItemOrderedMap[idx].raw != (randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = 526, .room_id = 112}}.raw // <- north gun req (8 modules)
        || randomItemOrderedMap[idx].raw != (randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = 646, .room_id = 185}}.raw // <- chain dash req, north gun req
        || randomItemOrderedMap[idx].raw != (randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = 2703, .room_id = 225}}.raw // north gun req
        || randomItemOrderedMap[idx].raw != (randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = 3045, .room_id = 112}}.raw // <- north gun req (8 modules)
        || randomItemOrderedMap[idx].raw != (randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = 3357, .room_id = 113}}.raw // <- north gun req (8 modules)
        || randomItemOrderedMap[idx].raw != (randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = 3981, .room_id = 93}}.raw // <- north gun req
        || randomItemOrderedMap[idx].raw != (randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = 4991, .room_id = 154}}.raw // <- north gun req
        || randomItemOrderedMap[idx].raw != (randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = 5164, .room_id = 112}}.raw // <- north gun req (8 modules)
        || randomItemOrderedMap[idx].raw != (randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = 7037, .room_id = 114}}.raw // <- north gun req (8 modules)
        || randomItemOrderedMap[idx].raw != (randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = 7430, .room_id = 113}}.raw // <- north gun req (8 modules)
        || randomItemOrderedMap[idx].raw != (randomItemInfo_t){.data = {.type = ITEM_GEARBIT, .identifier = 7600, .room_id = 111}}.raw // <- north gun req (8 modules)
        || randomItemOrderedMap[idx].raw != (randomItemInfo_t){.data = {.type = ITEM_KEY, .identifier = 78, .room_id = 185}}.raw // <- chain dash req, north gun req
        || randomItemOrderedMap[idx].raw != (randomItemInfo_t){.data = {.type = ITEM_KEY, .identifier = 4902, .room_id = 113}}.raw // <- north gun req (8 modules)
        || randomItemOrderedMap[idx].raw != (randomItemInfo_t){.data = {.type = ITEM_KEY, .identifier = 5886, .room_id = 225}}.raw; // north gun req
}

/*!
 *  @brief Check if the item is a sniper weapon
 *
 *  @param[in] idx      the index of the item to check
 *  @return true if the item is a sniper, false otherwise
 */
static bool isWeaponSniper(size_t idx)
{
    // These two weapons can open sniper nodes
    return randomItemOrderedMap[idx].raw == (randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = 21, .room_id = 121}}.raw
        || randomItemOrderedMap[idx].raw == (randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = 23, .room_id = 135}}.raw;
}

/*!
 *  @brief A function which returns true no matter the input
 *
 *  @param[in] idx      unused
 *  @return true
 */
static bool alwaysTrue(__attribute__((unused)) size_t idx)
{
    return true;
}

/*!
 *  @brief Assigns randomized items according to the two passed arguments
 *
 *  @param[in] gen                  AERRandGen pointer to be used for randomization
 *  @param[in] location_condition   Function pointer to a function that checks location conditions
 *  @param[in] item_condition       Function pointer to a function that checks item conditions
 */
static void assignFromConditions(AERRandGen* gen, bool (*location_condition)(size_t), bool (*item_condition)(size_t))
{
    // Get all possible locations
    size_t location_indexes[RAND_MAP_SIZE];
    size_t location_count = 0;
    for (size_t i = 0; i < RAND_MAP_SIZE; i++)
    {
        if (location_condition(i) && checkLocationNotTaken(i))
        {
            location_indexes[location_count] = i;
            location_count++;
        }
    }

    // Get all possible items
    size_t item_indexes[RAND_MAP_SIZE];
    size_t item_count = 0;
    for (size_t i = 0; i < RAND_MAP_SIZE; i++)
    {
        if (item_condition(i) && checkItemNotTaken(i))
        {
            item_indexes[item_count] = i;
            item_count++;
        }
    }

    // get the smallest buffer size
    size_t smallest_buffer_size = location_count < item_count ? location_count : item_count;

    AERRandGenShuffle(gen, sizeof(*item_indexes), item_count, item_indexes);

    for (size_t i = 0; i < smallest_buffer_size; i++)
        updateIndexes(location_indexes[i], item_indexes[i]);
}

/*!
 *  @brief Loads item randomization table from save file, or creates a new one
 */
static void refreshRandomMap(uint64_t seed)
{
    // Reset variables
    takenCounter = 0;
    ensureMapOrdered();

    AERRandGen* gen = AERRandGenNew(seed);

    // The basic setup is to choose pick the most restrictive items first
    // Assign a valid weapon to the first slot to get past the trigger
    assignFromConditions(gen, isFirstWeaponLocation, isValidFirstWeapon);

    // Assign where the sniper weapons should go
    assignFromConditions(gen, doesLocationNotRequireSniper, isWeaponSniper);

    // Assign the remaining items
    assignFromConditions(gen, alwaysTrue, alwaysTrue);

    AERRandGenFree(gen);
}

/* ----- INTERNAL FUNCTIONS ----- */

/*!
 *  @brief Loads item randomization table from save file, or creates a new one
 */
void logicGameLoadListener()
{
    // Try to get Seed from save
    uint64_t seed;

    // The AERDouble saving method will use a standard printf %f to text file that is base64 encoded
    // This means that it is not safe to use all 64 bits for saving data
    // Doubles can store uint32 numbers inside without any loss so we are using that to store larger data
    // Memory is aligned to 4 byte chunks, meaning that seed32[0] points to the first 4 bytes of seed, seed[1] points to the second 4 bytes
    uint32_t* seed32 = (uint32_t*)&seed;
    char key[] = "seedX";
    for (size_t i = 0; i < 2; i++) // interate twice since sizeof(uint64_t)/sizeof(uint32_t) = 2
    {
        sprintf(key, "seed%u", i);
        aererr = AER_TRY;
        double seed_d = AERSaveGetDouble(key);
        switch (aererr) {
            case AER_OK:
                seed32[i] = seed_d;
                break;
            case AER_FAILED_LOOKUP:
                AERLogInfo("Did not detect randomized seed");
                options.randomizer_enabled = false;
                return; // exit if either for loop fails
            default:
                AERLogErr("Getting Randomizer Seed failed unexpectedly");
                abort();
        }
    }
    AERLogInfo("Retrieved seed %llu from save!", seed);
    options.randomizer_enabled = true;
    refreshRandomMap(seed);
    return;
}

/*!
 *  @brief Function to check if a new game was created
 */
void checkForNewGame(int32_t newRoomIdx)
{
    // Check first tutorial room
    if (newRoomIdx != AER_ROOM_IN_01_BROKENSHALLOWS)
        return;

    // Do an additional check that the player does have any weapons
    if (AERInstanceGetByObject(AER_OBJECT_SECONDARY, false, 0, NULL) != 0)
        return;

    // This is only called upon starting a new game
    uint64_t seed = AERRandUInt();

    // Save new seed
    char key[] = "seedX";
    uint32_t* seed32 = (uint32_t*)&seed;
    for (size_t i = 0; i < 2; i++)
    {
        sprintf(key, "seed%u", i);
        AERSaveSetDouble(key, seed32[i]);
    }
    AERLogInfo("Saved New Seed %llu!", seed);

    // Update map
    refreshRandomMap(seed);
    options.randomizer_enabled = true;

    // Give the player access to their inventory
    AERInstance* data_obj;
    if (AERInstanceGetByObject(AER_OBJECT_DATA, false, 1, &data_obj) > 0)
        // the data instance is valid
        AERInstanceGetHLDLocal(data_obj, "playerHasMap")->d = 1;
    else 
    {
        AERLogErr("Randomizer could not equip map to player in tutorial, player is softlocked!");
        abort();
    }
    return;
}

/*!
 *  @brief Gets the item index in the ordered map from the item information
 *
 *  @param[in] item         pointer to a struct containing the item information
 * 
 *  @return the item index for the given item, or -1 if it is not in the map
 */
int16_t getItemIndex(randomItemInfo_t* item)
{
    // Binary search
    size_t lower = 0;
    size_t upper = RAND_MAP_SIZE;
    size_t mid;
    do {
        mid = (lower + upper) / 2;

        if (randomItemOrderedMap[mid].raw == item->raw)
            return mid;
        else if (randomItemOrderedMap[mid].raw > item->raw)
            upper = mid;
        else if (randomItemOrderedMap[mid].raw < item->raw)
            lower = mid + 1;
    } while (lower <= upper);
    // if this exits, that item does not exist in our list

    return -1;
}

/*!
 *  @brief Returns the new randomized item information from an old item index
 */
randomItemInfo_t updateRandomItem(int32_t oldItemIdx)
{
    // Basic check avoid errors
    if (oldItemIdx < 0 || oldItemIdx >= RAND_MAP_SIZE)
    {
        AERLogErr("Got item with an invalid index (%u)! Aborting...", oldItemIdx);
        abort();
    } 
    
    int32_t newIndex = randomizedIndexes[oldItemIdx];
    return randomItemOrderedMap[newIndex];
}