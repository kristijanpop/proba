#include "complex.h"
class Vector {
private:
 const int length;
  complex *arr;

public:
  Vector(int n);
  ~Vector();
  int size() const;
  complex &at(int i);
  const complex& at(int i) const;
  bool equals(const Vector &other) const;
}; 