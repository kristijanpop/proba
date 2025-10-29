#include "Set.h"
#include <cstdio>
#include <iostream>

// U main funkciji, konstruisati dva skupa od po dva vektora, te demonstrirati
// poređenje ta dva skupa upotrebom equals metode.
int main(void) {
  Vector v1(4);
  Vector v2(4);
  for (int i = 0; i < 4; i++) {
    v1.at(i) = complex(i + 1, i + 2);
    v2.at(i) = complex(i + 1, i + 2);
  }
  for (int i = 0; i < 4; i++) {
    printf("V%d(%lf,%lf)\n", i, v1.at(i).getX(), v1.at(i).getY());
  }
  Vector v3(4), v4(4);
  for (int i = 0; i < 4; i++) {
    v3.at(i) = complex(i + 2, i + 3);
    v4.at(i) = complex(2 * i + 1, 2 * i + 2);
  }
  Set s1, s2;
  s1.add(&v1);
  s1.add(&v2);
  s2.add(&v3);
  s2.add(&v4);

  std::cout << std::endl;
  printf("Skupovi %s jednaki", s1.equals(s2) ? "su" : "nisu");
}