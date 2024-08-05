#pragma once

struct DB_parkSlot
{
  bool is_changed = false; // ** This is not part of the database! ** indicates whether this slot has been changed by clicking on it and push the finish button.
  bool is_taken;
  int pos_x;
  int pos_y;
};

struct DB_floor
{
  int num_slots;
  DB_parkSlot* slots;
};

struct DB_parkingLot
{
  int num_floors;
  DB_floor* floors;
};

// holds database after calling to getDB() function
DB_parkingLot* db_parkingLot = nullptr;