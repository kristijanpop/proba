#pragma once
#include "Vector.h"

class Set {
private:
  int size;
  int capacity;
  Vector **niz; // agregira - niz pokazivaca
public:
  Set();
  ~Set();
  void add(Vector *v);
  bool contains(const Vector *v) const;
  bool equals(const Set &other) const;
  int getSize() const;
};