#include "aer/log.h"
#include "aer/rand.h"
#include "aer/save.h"
#include "aer/err.h"
#include "aer/event.h"

#include <time.h>
#include <stdlib.h>
#include "export.h"

#include "logic.h"
#include "crate.h"

#define RAND_MAP_SIZE 208
uint16_t randomizedIndexes[RAND_MAP_SIZE] = {0};
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

uint16_t takenItemIndexes[RAND_MAP_SIZE];
uint16_t takenItemCounter = 0;
uint16_t takenLocationIndexes[RAND_MAP_SIZE];
uint16_t takenLocationCounter = 0;

/*!
 *  Function will abort if the given input map is not ordered
 */
static void ensureMapOrdered()
{
    uint64_t lastItem = randomItemOrderedMap[0].raw;
    uint64_t nextItem;

    for (int i = 1; i < RAND_MAP_SIZE; i++)
    {
        nextItem = randomItemOrderedMap[i].raw;

        if (nextItem <= lastItem)
            abort();

        lastItem = nextItem;
    }
}
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
void checkRandomizerSpawn(randomItemInfo_t oldItem, float x, float y)
{
    // TO DO: Implement checks to see if this crate was opened before
    // logItemText(oldItem);
    // Spawn object
    createRandomCrate(oldItem, x, y);
}

int16_t getItemIndex(randomItemInfo_t* item)
{
    // Binary search
    uint16_t lower = 0;
    uint16_t upper = RAND_MAP_SIZE;
    uint16_t mid;
    do {
        mid = (lower + upper) / 2;
        
        if (randomItemOrderedMap[mid].raw > item->raw)
            upper = mid;
        else if (randomItemOrderedMap[mid].raw < item->raw)
            lower = mid;
        
        if (randomItemOrderedMap[mid].raw == item->raw)
            return mid;
        
    } while (lower < upper);
    // if this exits, that item does not exist in our list

    return -1;
}

bool updateRandomItem(randomItemInfo_t* item)
{
    // get the item index
    int16_t index = getItemIndex(item);
    if (index < 0)
        return false;
    
    // put the code to save the item data here

    // update the returned item
    uint16_t newIndex = randomizedIndexes[index];
    *item = randomItemOrderedMap[newIndex];
    return true;
}

static void updateIndexes(uint16_t originalIndex, uint16_t newIndex)
{
    randomizedIndexes[originalIndex] = newIndex;
    takenLocationIndexes[takenLocationCounter] = originalIndex;
    takenLocationCounter++;
    takenItemIndexes[takenItemCounter] = newIndex;
    takenItemCounter++;
}

static bool checkLocationNotTaken(uint16_t idx)
{
    for (int i = 0; i < takenLocationCounter; i++)
    {
        if (idx == takenLocationIndexes[i])
        {
            return false;
        }
    }
    return true;
}

static bool checkItemNotTaken(uint16_t idx)
{
    for (int i = 0; i < takenItemCounter; i++)
    {
        if (idx == takenItemIndexes[i])
        {
            return false;
        }
    }
    return true;
}

static bool isFirstWeaponLocation(uint16_t idx)
{
    return (randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = 1, .room_id = 48}}.raw == randomItemOrderedMap[idx].raw;
}

static bool isValidFirstWeapon(uint16_t idx)
{
    // Only these 4 weapons are allowed for this location
    return randomItemOrderedMap[idx].raw == (randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = 1, .room_id = 48}}.raw
        || randomItemOrderedMap[idx].raw == (randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = 2, .room_id = 245}}.raw
        || randomItemOrderedMap[idx].raw == (randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = 21, .room_id = 121}}.raw
        || randomItemOrderedMap[idx].raw == (randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = 23, .room_id = 135}}.raw;
}

static bool doesLocationNotRequireSniper(uint16_t idx)
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

static bool isWeaponSniper(uint16_t idx)
{
    // These two weapons can open sniper nodes
    return randomItemOrderedMap[idx].raw == (randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = 21, .room_id = 121}}.raw
        || randomItemOrderedMap[idx].raw == (randomItemInfo_t){.data = {.type = ITEM_WEAPON, .identifier = 23, .room_id = 135}}.raw;
}

static bool alwaysTrue(uint16_t idx)
{
    return true;
}
static void assignFromConditions(AERRandGen* gen, bool location_condition(uint16_t), bool item_condition(uint16_t))
{
    // Get all possible locations
    uint16_t location_indexes[RAND_MAP_SIZE];
    uint16_t location_count = 0;
    for (int i = 0; i < RAND_MAP_SIZE; i++)
    {
        if (location_condition(i) && checkLocationNotTaken(i))
        {
            location_indexes[location_count] = i;
            location_count++;
        }
    }

    // Get all possible items
    uint16_t item_indexes[RAND_MAP_SIZE];
    uint16_t item_count = 0;
    for (int i = 0; i < RAND_MAP_SIZE; i++)
    {
        if (item_condition(i) && checkItemNotTaken(i))
        {
            item_indexes[item_count] = i;
            item_count++;
        }
    }

    // get the smallest buffer size
    uint16_t smallest_buffer_size = location_count < item_count ? location_count : item_count;

    AERRandGenShuffle(gen, sizeof(*item_indexes), item_count, item_indexes);

    for (int i = 0; i < smallest_buffer_size; i++)
        updateIndexes(location_indexes[i], item_indexes[i]);
}

void createRandomizedIndexes()
{
    // Reset variables
    takenItemCounter = 0;
    takenLocationCounter = 0;
    ensureMapOrdered();

    // Try to get Seed from save
    uint64_t seed;
    bool createNewSeed = false;
    aererr = AER_TRY;
    double seed_d = AERSaveGetDouble("seed");
    switch (aererr) {
        case AER_OK:
            // Assume we got the correct seed
            seed = *(uint64_t*)&seed_d; // cast into uint64_t
            AERLogInfo("Retrieved Seed %lu from save!", seed);
            break;
        case AER_FAILED_LOOKUP:
            createNewSeed = true;
            break;
        case AER_FAILED_PARSE:
            createNewSeed = true;
            break;
        default:
            AERLogErr("Getting Randomizer Seed failed unexpectedly");
            abort();
    }

    if (createNewSeed || seed == 0)
    {
        seed = AERRandUInt();
        AERSaveSetDouble("seed", *(double*)&seed);
        AERLogInfo("Saved New Seed %lu!", seed);
    }

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