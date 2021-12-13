#ifndef CRATE_H
#define CRATE_H

/* ----- PUBLIC FUNCTIONS ----- */

void registerCrateObjects();
void registerCrateSprites();
void registerCrateObjectListeners();
void crateLoadListener();
void crateSaveListener();

void checkCrateSpawn(randomItemInfo_t newItem, float x, float y);

#endif /* CRATE_H */