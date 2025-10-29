#pragma once
#include "Vector.h"

class Set {
private: // agregacija znaci niz pokazivaca...
  int capacity;
  int length;
  Vector **niz;

public:
  Set();
  ~Set();
  bool add(Vector *v);
  bool equals(Set &other);
  bool contains(Vector *v1);
  void realloc();
  void realloc(int newCapacity);
};