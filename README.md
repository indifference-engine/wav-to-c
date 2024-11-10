# WAV to C

Command-line utility to convert WAV files to C files.

## Usage

`wav_to_c MACRO_NAME variable_name path/to/first/include.h path/to/second/include.h < path/to/input.wav > path/to/output.c`

This will output a file similar to the following:

```c
#include "path/to/first/include.h"
#include "path/to/second/include.h"

MACRO_NAME
(
  variable_name,

  // Sample rate in Hertz.
  3,

  // Samples as signed unit intervals, running sooner to later.
  0.3f, 0.7f, 0.8f, 0.7f, 0.2f, 0.4f
);
```

## Limitations

Currently, only mono 32-bit floating WAV files are supported.

## Building

Execute `make` to build the executable, which will be placed at
[dist/wav_to_c](./dist/wav_to_c).

### Dependencies

- Make.
- MinGW-GCC for Windows, Clang for all other platforms.
- Bash.

## Tests

Execute `make test` to run the test suite.
