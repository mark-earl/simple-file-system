/**
 * @type FILE*: Pointer to an object type that identifies a stream and contains the information needed
 * to control it, including a pointer to its buffer, its position indicator and all its state indicators.
 *
 * @doc: https://cplusplus.com/reference/cstdio/FILE/
 */

/**
 * FILE * fopen ( const char * filename, const char * mode );
 * @param filename: C string containing the name of the file to be opened.
 * @param mode: C string containing a file access mode
 * NOTE: "r+": read/update: Open a file for update (both for input and output). The file must exist.
 *
 * @retval: If the file is successfully opened, the function returns a pointer to a FILE object that
 * can be used to identify the stream on future operations. Otherwise, a null pointer (NULL) is returned.
 *
 * @doc: https://cplusplus.com/reference/cstdio/fopen/
 */

/**
 * int fseek( FILE *stream, long offset, int origin );
 * @param stream: file stream to modify
 * @param offset: number of characters to shift the position relative to origin
 * @param origin: position to which offset is added. It can have one of the following values: SEEK_SET, SEEK_CUR, SEEK_END
 *
 **SEEK_SET argument to fseek indicating seeking from beginning of the file
 * SEEK_CUR argument to fseek indicating seeking from the current file position
 * SEEK_END argument to fseek indicating seeking from end of the file
 *
 * @retval ​0​ upon success, nonzero value otherwise.
 *
 * @doc: https://cplusplus.com/reference/cstdio/fseek/
 */

/**
 * int printf ( const char * format, ... );
 * @param format: C string that contains the text to be written to stdout.
 * It can optionally contain embedded format specifiers that are replaced by
 * the values specified in subsequent additional arguments and formatted as requested.
 *
 * @retval On success, the total number of characters written is returned.
 * If a writing error occurs, the error indicator (ferror) is set and a negative number is returned.
 * If a multibyte character encoding error occurs while writing wide characters, errno is set to EILSEQ and a negative number is returned.
 *
 * @doc: https://cplusplus.com/reference/cstdio/printf/
*/

/**
 * int fgetc ( FILE * stream );
 * @param stream: Pointer to a FILE object that identifies an input stream.
 *
 * @retval: On success, the character read is returned (promoted to an int value).
 * The return type is int to accommodate for the special value EOF, which indicates failure:
 * If the position indicator was at the end-of-file, the function returns EOF and sets the eof indicator (feof) of stream.
 * If some other reading error happens, the function also returns EOF, but sets its error indicator (ferror) instead.
*/
