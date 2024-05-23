#include <stdint.h>
#include <stdio.h>
#include "read_u32_or_throw.h"

float read_f32_or_throw(
    const char *const description,
    FILE *const file)
{
  union
  {
    uint32_t u32;
    float f32;
  } convert;

  convert.u32 = read_u32_or_throw(description, file);

  return convert.f32;
}
