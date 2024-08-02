#pragma once

struct DB_parkSlot
{
  bool is_taken;
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