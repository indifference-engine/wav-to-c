#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "reopen_as_read_only_binary_or_throw.h"
#include "reopen_as_write_only_binary_or_throw.h"
#include "read_u8s_or_throw.h"
#include "read_u16_or_throw.h"
#include "read_u32_or_throw.h"
#include "read_f32s_or_throw.h"
#include "skip.h"
#include "write_or_throw.h"
#include "throw.h"

static uint32_t remaining_chunk_bytes;

static void allocate_chunk_bytes(uint32_t bytes)
{
  if (bytes > remaining_chunk_bytes)
  {
    throw("RIFF chunk ended unexpectedly.");
  }
  else
  {
    remaining_chunk_bytes -= bytes;
  }
}

int main(int argc, char **argv)
{
  (void)(argc);
  (void)(argv);

  if (argc < 3)
  {
    throw("At least 2 arguments are required (macro name and variable name).");
  }

  reopen_as_read_only_binary_or_throw("the input WAV file", stdin);
  reopen_as_write_only_binary_or_throw("the output C file", stdout);

  const uint8_t *const magic_number = read_u8s_or_throw("the magic number", stdin, 4);

  if (magic_number[0] != 'R' || magic_number[1] != 'I' || magic_number[2] != 'F' || magic_number[3] != 'F')
  {
    throw("Incorrect magic number.");
  }

  remaining_chunk_bytes = read_u32_or_throw("the size of the chunk", stdin);

  allocate_chunk_bytes(4);
  const uint8_t *const format = read_u8s_or_throw("the format", stdin, 4);

  if (format[0] != 'W' || format[1] != 'A' || format[2] != 'V' || format[3] != 'E')
  {
    throw("Incorrect format.");
    exit(1);
  }

  uint32_t sample_rate = 0;
  const float *samples = NULL;
  size_t samples_length = 0;

  while (remaining_chunk_bytes > 0)
  {
    allocate_chunk_bytes(8);
    const uint8_t *const sub_chunk_id = read_u8s_or_throw("sub-chunk ID", stdin, 4);
    const uint32_t sub_chunk_bytes = read_u32_or_throw("the sub-chunk size", stdin);
    allocate_chunk_bytes(sub_chunk_bytes);

    if (sub_chunk_id[0] == 'L' && sub_chunk_id[1] == 'I' && sub_chunk_id[2] == 'S' && sub_chunk_id[3] == 'T')
    {
      skip("a LIST chunk", stdin, sub_chunk_bytes);
    }
    else if (sub_chunk_id[0] == 'i' && sub_chunk_id[1] == 'd' && sub_chunk_id[2] == '3' && sub_chunk_id[3] == ' ')
    {
      skip("an id3 chunk", stdin, sub_chunk_bytes);
    }
    else if (sub_chunk_id[0] == 'f' && sub_chunk_id[1] == 'm' && sub_chunk_id[2] == 't' && sub_chunk_id[3] == ' ')
    {
      if (sample_rate != 0)
      {
        throw("Multiple fmt sub-chunks in the same file.");
      }

      if (sub_chunk_bytes != 16)
      {
        throw("Expected 16 byte fmt sub-chunk, actual %d.", sub_chunk_bytes);
      }

      const uint16_t format = read_u16_or_throw("the format", stdin);

      if (format != 3)
      {
        throw("Expected format 3, actual %d.", format);
      }

      const uint16_t channels = read_u16_or_throw("the number of channels", stdin);

      if (channels != 1)
      {
        throw("Expected 1 channel, actual %d.", channels);
      }

      sample_rate = read_u32_or_throw("the sample rate", stdin);

      if (sample_rate == 0)
      {
        throw("Sample rate cannot be 0.");
      }

      const uint32_t expected_average_bytes_per_second = sample_rate * 4;
      const uint32_t actual_average_bytes_per_second = read_u32_or_throw("the number of bytes per second on average", stdin);

      if (actual_average_bytes_per_second != expected_average_bytes_per_second)
      {
        throw("Expected %d bytes per second on average, actual %d.", expected_average_bytes_per_second, actual_average_bytes_per_second);
      }

      const uint16_t block_align = read_u16_or_throw("the block alignment", stdin);

      if (block_align != 4)
      {
        throw("Expected block alignment 4, actual %d.", block_align);
      }

      const uint16_t bits_per_sample = read_u16_or_throw("the number of bits per sample", stdin);

      if (bits_per_sample != 32)
      {
        throw("Expected 32 bits per sample, actual %d.", bits_per_sample);
      }
    }
    else if (sub_chunk_id[0] == 'f' && sub_chunk_id[1] == 'a' && sub_chunk_id[2] == 'c' && sub_chunk_id[3] == 't')
    {
      skip("a fact chunk", stdin, sub_chunk_bytes);
    }
    else if (sub_chunk_id[0] == 'P' && sub_chunk_id[1] == 'E' && sub_chunk_id[2] == 'A' && sub_chunk_id[3] == 'K')
    {
      skip("a PEAK chunk", stdin, sub_chunk_bytes);
    }
    else if (sub_chunk_id[0] == 'd' && sub_chunk_id[1] == 'a' && sub_chunk_id[2] == 't' && sub_chunk_id[3] == 'a')
    {
      if (samples != NULL)
      {
        throw("Multiple data sub-chunks in the same file.");
      }

      if (sub_chunk_bytes == 0)
      {
        throw("The file contains no samples.");
      }

      if (sub_chunk_bytes % 4 != 0)
      {
        throw("The file contains incomplete samples.");
      }

      samples_length = sub_chunk_bytes / 4;
      samples = read_f32s_or_throw("samples", stdin, samples_length);
    }
    else
    {
      throw("Unsupported sub-chunk ID %c%c%c%c.", sub_chunk_id[0], sub_chunk_id[1], sub_chunk_id[2], sub_chunk_id[3]);
    }
  }

  if (sample_rate == 0)
  {
    throw("The file does not contain formatting data.");
  }

  if (samples == NULL)
  {
    throw("The file does not contain samples.");
  }

  for (int index = 3; index < argc; index++)
  {
    write_or_throw("an include", stdout, "#include \"%s\"\n", argv[index]);
  }

  write_or_throw("the header", stdout, "%s%s(\n  %s,\n  %u,\n  ", argc > 3 ? "\n" : "", argv[1], argv[2], sample_rate);

  for (size_t sample_index = 0; sample_index < samples_length; sample_index++)
  {
    write_or_throw("a sample", stdout, "%s%ff", sample_index == 0 ? "" : ", ", samples[sample_index]);
  }

  write_or_throw("the footer", stdout, "\n);\n");

  return 0;
}
