#include "Set.h"
#include <__config>

Set::Set() : size(0), capacity(4), niz(new Vector *[capacity]) {}
