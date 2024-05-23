#include <stdio.h>
#include "throw.h"

void skip(
    const char *description,
    FILE *file,
    const size_t bytes)
{
  if (fseek(file, bytes, SEEK_CUR))
  {
    throw("Failed to skip %s.", description);
  }
}
