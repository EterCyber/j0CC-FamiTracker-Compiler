#include "ftmath.h"

std::optional<long> parseLong(CString str) {
  char *endptr;
  const int radix = 10;

  long out;
  out = strtol(CT2A(str), &endptr, radix);
  if (*endptr == '\0') {
    return out;
  }
  return {};
}

