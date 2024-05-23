/**
 * Reads an unsigned int from a file, halting execution with an error message
 * should the operation fail.
 * @param description A description of what is being read
 *                    (e.g. "a sub-heading").
 * @param file The file from which to read an unsigned int.
 * @return The unsigned int read from the file.
 */
uint32_t read_u32_or_throw(
    const char *const description,
    FILE *const file);
