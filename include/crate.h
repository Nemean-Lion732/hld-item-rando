#ifndef CRATE_H
#define CRATE_H

/* ----- PUBLIC FUNCTIONS ----- */

void registerCrateObjects();
void registerCrateSprites();
void registerCrateObjectListeners();
void checkCrateSpawn(randomItemInfo_t newItem, float x, float y);
void loadItemTakenFlags();
void crateSaveEvent();
#endif /* CRATE_H */