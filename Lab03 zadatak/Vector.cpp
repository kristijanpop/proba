#include "Vector.h"

Vector::Vector(int n) : arr(new complex[n]), length(n) {}
Vector::~Vector() { delete[] arr; }
int Vector::size() const { return this->length; }
complex &Vector::at(int i) { return arr[i]; }
const complex &Vector::at(int i) const { return arr[i]; }
bool Vector::equals(const Vector &other) const {
  if (!(this->length == other.length))
    return false;
  for (int i = 0; i < this->length; i++) {
    if (!(this->arr[i].getX() == this->arr->getY() ||
          this->arr[i].getX() == this->arr->getY()))
      return false;
  }
  return true;
}