#ifndef CRATE_H
#define CRATE_H

/* ----- PUBLIC FUNCTIONS ----- */

void registerCrateObjects();
void registerCrateSprites();
bool destroyCrateListener(AEREvent* event, AERInstance* target, AERInstance* other);
void registerCrateObjectListeners();
void createRandomCrate(randomItemInfo_t newItem, float x, float y);
#endif /* CRATE_H */