#include "Set.h"

Set::Set() : length(0), capacity(4), niz(new Vector *[capacity]) {}
Set::~Set() { delete[] niz; }
bool Set::contains(Vector *v1) {
  for (int i = 0; i < this->length; i++) {
    if (this->niz[i]->equals(*v1))
      return true;
  }
  return false;
}
bool Set::add(Vector *v) {
  if (this->contains(v))
    return false;
  if (this->capacity == this->length) {
    realloc();
  }
  this->niz[length++] = v;
  return true;
}
void Set::realloc() {
  this->realloc(this->capacity > 0 ? this->capacity * 2 : 1);
}
void Set::realloc(int newCapacity) {
  Vector **newArr = new Vector *[newCapacity]();

  for (int i = 0; i < this->length; i++)
    newArr[i] = this->niz[i];

  delete[] this->niz;
  this->capacity = newCapacity;
  this->niz = newArr;
}
bool Set::equals(Set &other) {
  if (this->length != other.length)
    return false;
  for (int i = 0; i < this->length; i++) {
    if (!this->niz[i]->equals(*other.niz[i]))
      return false;
  }
  return true;
}
