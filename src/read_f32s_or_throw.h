/**
 * Reads a number of floats from a file, halting execution with an error message
 * should the operation fail.
 * @param description A description of what is being read
 *                    (e.g. "a sub-heading").
 * @param file The file from which to read floats.
 * @param quantity The number of floats to read from the file.
 * @return A pointer to newly allocated memory containing the read floats.
 */
float *read_f32s_or_throw(
    const char *const description,
    FILE *const file,
    const size_t quantity);
