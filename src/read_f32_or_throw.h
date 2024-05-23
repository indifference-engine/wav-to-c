/**
 * Reads a float from a file, halting execution with an error message should the
 * operation fail.
 * @param description A description of what is being read
 *                    (e.g. "a sub-heading").
 * @param file The file from which to read a float.
 * @return The float read from the file.
 */
float read_f32_or_throw(
    const char *const description,
    FILE *const file);
