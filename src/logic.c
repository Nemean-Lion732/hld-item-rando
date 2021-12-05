#include "aer/object.h"
#include "aer/log.h"
#include "aer/sprite.h"

#include "export.h"

#include "crate.h"

static const randomItemInfo_t originalItems[] = 
{
    /*      GEARBITS       */
    {.data = {.type = ITEM_GEARBIT, .identifier = 8}}, // Room ID: 234
    {.data = {.type = ITEM_GEARBIT, .identifier = 72}}, // Room ID: 222 <- 8 modules
    {.data = {.type = ITEM_GEARBIT, .identifier = 148}}, // Room ID: 62
    {.data = {.type = ITEM_GEARBIT, .identifier = 339}}, // Room ID: 226
    {.data = {.type = ITEM_GEARBIT, .identifier = 524}}, // Room ID: 64
    {.data = {.type = ITEM_GEARBIT, .identifier = 526}}, // Room ID: 112 <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 646}}, // Room ID: 185 <- chain dash req, north gun req
    {.data = {.type = ITEM_GEARBIT, .identifier = 703}}, // Room ID: 84
    {.data = {.type = ITEM_GEARBIT, .identifier = 717}}, // Room ID: 187
    {.data = {.type = ITEM_GEARBIT, .identifier = 808}}, // Room ID: 241
    {.data = {.type = ITEM_GEARBIT, .identifier = 834}}, // Room ID: 85
    {.data = {.type = ITEM_GEARBIT, .identifier = 910}}, // Room ID: 188
    {.data = {.type = ITEM_GEARBIT, .identifier = 981}}, // Room ID: 61
    {.data = {.type = ITEM_GEARBIT, .identifier = 1069}}, // Room ID: 230
    {.data = {.type = ITEM_GEARBIT, .identifier = 1136}}, // Room ID: 181 <- chain dash req
    {.data = {.type = ITEM_GEARBIT, .identifier = 1172}}, // Room ID: 218
    {.data = {.type = ITEM_GEARBIT, .identifier = 1224}}, // Room ID: 198 <- chain dash req
    {.data = {.type = ITEM_GEARBIT, .identifier = 1314}}, // Room ID: 211
    {.data = {.type = ITEM_GEARBIT, .identifier = 1329}}, // Room ID: 175 <- chain dash req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 1524}}, // Room ID: 242 <- 4 keys req
    {.data = {.type = ITEM_GEARBIT, .identifier = 1526}}, // Room ID: 229
    {.data = {.type = ITEM_GEARBIT, .identifier = 1544}}, // Room ID: 107
    {.data = {.type = ITEM_GEARBIT, .identifier = 1626}}, // Room ID: 86
    {.data = {.type = ITEM_GEARBIT, .identifier = 1729}}, // Room ID: 181
    {.data = {.type = ITEM_GEARBIT, .identifier = 1806}}, // Room ID: 178
    {.data = {.type = ITEM_GEARBIT, .identifier = 1847}}, // Room ID: 199 <- chain dash req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 1852}}, // Room ID: 109
    {.data = {.type = ITEM_GEARBIT, .identifier = 1879}}, // Room ID: 209
    {.data = {.type = ITEM_GEARBIT, .identifier = 1964}}, // Room ID: 172
    {.data = {.type = ITEM_GEARBIT, .identifier = 1969}}, // Room ID: 242
    {.data = {.type = ITEM_GEARBIT, .identifier = 2034}}, // Room ID: 238
    {.data = {.type = ITEM_GEARBIT, .identifier = 2125}}, // Room ID: 231
    {.data = {.type = ITEM_GEARBIT, .identifier = 2220}}, // Room ID: 234
    {.data = {.type = ITEM_GEARBIT, .identifier = 2287}}, // Room ID: 61
    {.data = {.type = ITEM_GEARBIT, .identifier = 2360}}, // Room ID: 65
    {.data = {.type = ITEM_GEARBIT, .identifier = 2391}}, // Room ID: 171
    {.data = {.type = ITEM_GEARBIT, .identifier = 2434}}, // Room ID: 85
    {.data = {.type = ITEM_GEARBIT, .identifier = 2563}}, // Room ID: 200 <- chain dash req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 2572}}, // Room ID: 194
    {.data = {.type = ITEM_GEARBIT, .identifier = 2698}}, // Room ID: 92
    {.data = {.type = ITEM_GEARBIT, .identifier = 2703}}, // Room ID: 225 // north gun req
    {.data = {.type = ITEM_GEARBIT, .identifier = 2753}}, // Room ID: 215
    {.data = {.type = ITEM_GEARBIT, .identifier = 2764}}, // Room ID: 189
    {.data = {.type = ITEM_GEARBIT, .identifier = 2938}}, // Room ID: 229
    {.data = {.type = ITEM_GEARBIT, .identifier = 2963}}, // Room ID: 194
    {.data = {.type = ITEM_GEARBIT, .identifier = 3022}}, // Room ID: 108
    {.data = {.type = ITEM_GEARBIT, .identifier = 3042}}, // Room ID: 193 <- 3 key required
    {.data = {.type = ITEM_GEARBIT, .identifier = 3045}}, // Room ID: 112 <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 3050}}, // Room ID: 104
    {.data = {.type = ITEM_GEARBIT, .identifier = 3057}}, // Room ID: 193
    {.data = {.type = ITEM_GEARBIT, .identifier = 3076}}, // Room ID: 90
    {.data = {.type = ITEM_GEARBIT, .identifier = 3083}}, // Room ID: 193 <- 3 key required
    {.data = {.type = ITEM_GEARBIT, .identifier = 3143}}, // Room ID: 213
    {.data = {.type = ITEM_GEARBIT, .identifier = 3195}}, // Room ID: 241
    {.data = {.type = ITEM_GEARBIT, .identifier = 3256}}, // Room ID: 87 <- 10 keys req
    {.data = {.type = ITEM_GEARBIT, .identifier = 3260}}, // Room ID: 93
    {.data = {.type = ITEM_GEARBIT, .identifier = 3278}}, // Room ID: 119
    {.data = {.type = ITEM_GEARBIT, .identifier = 3357}}, // Room ID: 113 <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 3364}}, // Room ID: 233
    {.data = {.type = ITEM_GEARBIT, .identifier = 3446}}, // Room ID: 117
    {.data = {.type = ITEM_GEARBIT, .identifier = 3494}}, // Room ID: 225
    {.data = {.type = ITEM_GEARBIT, .identifier = 3541}}, // Room ID: 242
    {.data = {.type = ITEM_GEARBIT, .identifier = 3594}}, // Room ID: 118
    {.data = {.type = ITEM_GEARBIT, .identifier = 3596}}, // Room ID: 199 <- chain dash req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 3647}}, // Room ID: 183 <- 1 key required
    {.data = {.type = ITEM_GEARBIT, .identifier = 3815}}, // Room ID: 118
    {.data = {.type = ITEM_GEARBIT, .identifier = 3819}}, // Room ID: 106
    {.data = {.type = ITEM_GEARBIT, .identifier = 3834}}, // Room ID: 98
    {.data = {.type = ITEM_GEARBIT, .identifier = 3854}}, // Room ID: 181
    {.data = {.type = ITEM_GEARBIT, .identifier = 3969}}, // Room ID: 178
    {.data = {.type = ITEM_GEARBIT, .identifier = 3972}}, // Room ID: 96
    {.data = {.type = ITEM_GEARBIT, .identifier = 3981}}, // Room ID: 93 <- north gun req
    {.data = {.type = ITEM_GEARBIT, .identifier = 4000}}, // Room ID: 92
    {.data = {.type = ITEM_GEARBIT, .identifier = 4059}}, // Room ID: 119
    {.data = {.type = ITEM_GEARBIT, .identifier = 4060}}, // Room ID: 227
    {.data = {.type = ITEM_GEARBIT, .identifier = 4111}}, // Room ID: 218
    {.data = {.type = ITEM_GEARBIT, .identifier = 4175}}, // Room ID: 218
    {.data = {.type = ITEM_GEARBIT, .identifier = 4304}}, // Room ID: 190
    {.data = {.type = ITEM_GEARBIT, .identifier = 4377}}, // Room ID: 100
    {.data = {.type = ITEM_GEARBIT, .identifier = 4386}}, // Room ID: 212
    {.data = {.type = ITEM_GEARBIT, .identifier = 4386}}, // Room ID: 53
    {.data = {.type = ITEM_GEARBIT, .identifier = 4511}}, // Room ID: 193
    {.data = {.type = ITEM_GEARBIT, .identifier = 4551}}, // Room ID: 199 <- chain dash req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 4826}}, // Room ID: 90
    {.data = {.type = ITEM_GEARBIT, .identifier = 4969}}, // Room ID: 226
    {.data = {.type = ITEM_GEARBIT, .identifier = 5065}}, // Room ID: 190
    {.data = {.type = ITEM_GEARBIT, .identifier = 5164}}, // Room ID: 112 <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 5232}}, // Room ID: 106
    {.data = {.type = ITEM_GEARBIT, .identifier = 5276}}, // Room ID: 214
    {.data = {.type = ITEM_GEARBIT, .identifier = 5312}}, // Room ID: 93
    {.data = {.type = ITEM_GEARBIT, .identifier = 5622}}, // Room ID: 67 <- chain dash req
    {.data = {.type = ITEM_GEARBIT, .identifier = 5821}}, // Room ID: 185
    {.data = {.type = ITEM_GEARBIT, .identifier = 5916}}, // Room ID: 232 <- 12 keys req
    {.data = {.type = ITEM_GEARBIT, .identifier = 5918}}, // Room ID: 104
    {.data = {.type = ITEM_GEARBIT, .identifier = 5927}}, // Room ID: 103
    {.data = {.type = ITEM_GEARBIT, .identifier = 6000}}, // Room ID: 187
    {.data = {.type = ITEM_GEARBIT, .identifier = 6007}}, // Room ID: 194
    {.data = {.type = ITEM_GEARBIT, .identifier = 6014}}, // Room ID: 231
    {.data = {.type = ITEM_GEARBIT, .identifier = 6082}}, // Room ID: 182
    {.data = {.type = ITEM_GEARBIT, .identifier = 6113}}, // Room ID: 242
    {.data = {.type = ITEM_GEARBIT, .identifier = 6124}}, // Room ID: 181
    {.data = {.type = ITEM_GEARBIT, .identifier = 6153}}, // Room ID: 84
    {.data = {.type = ITEM_GEARBIT, .identifier = 6177}}, // Room ID: 244
    {.data = {.type = ITEM_GEARBIT, .identifier = 6231}}, // Room ID: 193 <- 3 key required
    {.data = {.type = ITEM_GEARBIT, .identifier = 6266}}, // Room ID: 183 <- 1 key required
    {.data = {.type = ITEM_GEARBIT, .identifier = 6387}}, // Room ID: 87 <- 10 keys req
    {.data = {.type = ITEM_GEARBIT, .identifier = 6433}}, // Room ID: 64
    {.data = {.type = ITEM_GEARBIT, .identifier = 6516}}, // Room ID: 239
    {.data = {.type = ITEM_GEARBIT, .identifier = 6585}}, // Room ID: 230
    {.data = {.type = ITEM_GEARBIT, .identifier = 6649}}, // Room ID: 86
    {.data = {.type = ITEM_GEARBIT, .identifier = 6660}}, // Room ID: 182
    {.data = {.type = ITEM_GEARBIT, .identifier = 6751}}, // Room ID: 243
    {.data = {.type = ITEM_GEARBIT, .identifier = 6774}}, // Room ID: 187
    {.data = {.type = ITEM_GEARBIT, .identifier = 6788}}, // Room ID: 119
    {.data = {.type = ITEM_GEARBIT, .identifier = 6843}}, // Room ID: 246
    {.data = {.type = ITEM_GEARBIT, .identifier = 7037}}, // Room ID: 114 <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 7096}}, // Room ID: 222  <- 8 modules
    {.data = {.type = ITEM_GEARBIT, .identifier = 7134}}, // Room ID: 226
    {.data = {.type = ITEM_GEARBIT, .identifier = 7179}}, // Room ID: 89
    {.data = {.type = ITEM_GEARBIT, .identifier = 7304}}, // Room ID: 175
    {.data = {.type = ITEM_GEARBIT, .identifier = 7330}}, // Room ID: 214
    {.data = {.type = ITEM_GEARBIT, .identifier = 7410}}, // Room ID: 242
    {.data = {.type = ITEM_GEARBIT, .identifier = 7430}}, // Room ID: 113 <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 7433}}, // Room ID: 193
    {.data = {.type = ITEM_GEARBIT, .identifier = 7455}}, // Room ID: 233
    {.data = {.type = ITEM_GEARBIT, .identifier = 7600}}, // Room ID: 111 <- north gun req (8 modules)
    {.data = {.type = ITEM_GEARBIT, .identifier = 7623}}, // Room ID: 172
    {.data = {.type = ITEM_GEARBIT, .identifier = 7631}}, // Room ID: 226
    {.data = {.type = ITEM_GEARBIT, .identifier = 7675}}, // Room ID: 171
    {.data = {.type = ITEM_GEARBIT, .identifier = 7692}}, // Room ID: 100
    {.data = {.type = ITEM_GEARBIT, .identifier = 7796}}, // Room ID: 211
    {.data = {.type = ITEM_GEARBIT, .identifier = 7840}}, // Room ID: 62
    {.data = {.type = ITEM_GEARBIT, .identifier = 7889}}, // Room ID: 188
    {.data = {.type = ITEM_GEARBIT, .identifier = 7940}}, // Room ID: 89
    {.data = {.type = ITEM_GEARBIT, .identifier = 7985}}, // Room ID: 94
    {.data = {.type = ITEM_GEARBIT, .identifier = 9324}}, // Room ID: 181
    
    /*      BOSS GEARBITS       */
    {.data = {.type = ITEM_GEARBIT, .identifier = 4293967297}}, // Room ID: 121
    {.data = {.type = ITEM_GEARBIT, .identifier = 4293967297}}, // Room ID: 121
    {.data = {.type = ITEM_GEARBIT, .identifier = 4293967297}}, // Room ID: 121

    {.data = {.type = ITEM_GEARBIT, .identifier = 4293967297}}, // Room ID: 195
    {.data = {.type = ITEM_GEARBIT, .identifier = 4293967297}}, // Room ID: 195
    {.data = {.type = ITEM_GEARBIT, .identifier = 4293967297}}, // Room ID: 195

    {.data = {.type = ITEM_GEARBIT, .identifier = 4293967297}}, // Room ID: 245
    {.data = {.type = ITEM_GEARBIT, .identifier = 4293967297}}, // Room ID: 245
    {.data = {.type = ITEM_GEARBIT, .identifier = 4293967297}}, // Room ID: 245

    /*      KEYS        */
    {.data = {.type = ITEM_KEY, .identifier = 26}}, // Room ID: 53
    {.data = {.type = ITEM_KEY, .identifier = 78}}, // Room ID: 185 <- chain dash req, north gun req
    {.data = {.type = ITEM_KEY, .identifier = 164}}, // Room ID: 100
    {.data = {.type = ITEM_KEY, .identifier = 239}}, // Room ID: 238 <- chain dash req
    {.data = {.type = ITEM_KEY, .identifier = 535}}, // Room ID: 242 242 <- 4 keys req
    {.data = {.type = ITEM_KEY, .identifier = 4401}}, // Room ID: 189
    {.data = {.type = ITEM_KEY, .identifier = 4902}}, // Room ID: 113 <- north gun req (8 modules)
    {.data = {.type = ITEM_KEY, .identifier = 4990}}, // Room ID: 229
    {.data = {.type = ITEM_KEY, .identifier = 5867}}, // Room ID: 187
    {.data = {.type = ITEM_KEY, .identifier = 5886}}, // Room ID: 225 // north gun req
    {.data = {.type = ITEM_KEY, .identifier = 6662}}, // Room ID: 193
    {.data = {.type = ITEM_KEY, .identifier = 7232}}, // Room ID: 92

    /*      WEAPONS        */
    {.data = {.type = ITEM_WEAPON, .identifier = 1}}, // Room ID: 48
    {.data = {.type = ITEM_WEAPON, .identifier = 2}}, // Room ID: 245
    {.data = {.type = ITEM_WEAPON, .identifier = 21}}, // Room ID: 121
    {.data = {.type = ITEM_WEAPON, .identifier = 41}}, // Room ID: 200 <- chain dash req (8 modules)
    {.data = {.type = ITEM_WEAPON, .identifier = 43}}, // Room ID: 195
};    