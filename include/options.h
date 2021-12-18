#ifndef OPTIONS_H
#define OPTIONS_H

/* ----- INTERNAL GLOBALS ----- */

typedef struct options_t
{
    bool randomizer_enabled;
    struct
    {
        uint8_t gearbit;
        uint8_t key_skele;
        uint8_t weapon_skele;
        uint8_t rando_indicator;
    } alarms;
} options_t;

extern options_t options;

/* ----- INTERNAL FUNCTIONS ----- */

void optionsConstructor();

#endif /*   OPTIONS_H   */