#pragma once

#include <string>

#define TYPE_REGULAR 1
#define TYPE_DISABLED 2
#define TYPE_ELECTRIC 3
#define TYPE_MOTORCYCLE 4

struct DB_parkSlot
{
  bool is_changed = false; // ** This is not part of the database! ** indicates whether this slot has been changed by clicking on it and push the finish button.
  bool is_taken;
  int pos_x;
  int pos_y;
  int width;
  int height;
  int type;
};

struct DB_floor
{
  int num_slots;
  DB_parkSlot* slots;
};

struct DB_parkingLot
{
  int num_floors;
  char* adv = nullptr;
  char* movingAdv = nullptr;
  DB_floor* floors;
};

// holds database after calling to getDB() function
DB_parkingLot* db_parkingLot = nullptr;
