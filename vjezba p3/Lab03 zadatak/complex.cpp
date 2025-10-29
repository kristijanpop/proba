#include "Complex.h"

complex::complex(double re, double im) : re(re), im(im) {}
double complex::getX() { return this->re; }
double complex::getY() { return this->im; }
bool complex::equals(const complex &other) const {
  return (this->re == other.re) && (this->im == other.im);
}