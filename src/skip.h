/**
 * Skips a specified number of bytes in a file, halting execution with an error
 * message should the operation fail.
 * @param description A description of what is being skipped
 *                    (e.g. "a sub-heading").
 * @param file The file in which to skip bytes.
 * @param bytes The number of bytes to skip.
 */
void skip(
    const char *description,
    FILE *file,
    const size_t bytes);
