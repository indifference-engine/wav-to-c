#include <stdint.h>
#include <stdio.h>
#include "read_u8_or_throw.h"

uint32_t read_u32_or_throw(
    const char *const description,
    FILE *const file)
{
  uint8_t first_byte = read_u8_or_throw(description, file);
  uint8_t second_byte = read_u8_or_throw(description, file);
  uint8_t third_byte = read_u8_or_throw(description, file);
  uint8_t fourth_byte = read_u8_or_throw(description, file);

  return first_byte | (second_byte << 8) | (third_byte << 16) | (fourth_byte << 24);
}
