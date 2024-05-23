#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include "read_f32_or_throw.h"
#include "malloc_or_throw.h"

float *read_f32s_or_throw(
    const char *const description,
    FILE *const file,
    const size_t quantity)
{
  float *const output = malloc_or_throw(description, sizeof(float) * quantity);

  for (size_t read = 0; read < quantity; read++)
  {
    output[read] = read_f32_or_throw(description, file);
  }

  return output;
}
