#include "Complex.h"
class Vector {
private:
  const int length;
  complex *arr;

public:
  Vector(int n = 0);
  ~Vector();
  int size() const;
  complex &at(int i);
  const complex &at(int i) const;
  bool equals(const Vector &other) const;

  Vector(const Vector &) = delete;
  Vector(Vector &&) = delete;
  Vector &operator=(const Vector &) = delete;
  Vector &operator=(Vector &&) = delete;
};