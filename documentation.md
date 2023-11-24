# @file: documentation.md

This file contains references for all of the C functions I used If the abbreviated documentation contained within this file is insufficient, the full docs are linked in the header of each section.

## [cstdio](https://cplusplus.com/reference/cstdio/)

### [`FILE*`](https://cplusplus.com/reference/cstdio/FILE/)
Pointer to an object type that identifies a stream and contains the information needed
to control it, including a pointer to its buffer, its position indicator and all its state indicators.

### [`FILE * fopen ( const char * filename, const char * mode );`](https://cplusplus.com/reference/cstdio/fopen/)
* **@param** filename: C string containing the name of the file to be opened.
* **@param** mode: C string containing a file access mode
    * *NOTE* "r+": read/update: Open a file for update (both for input and output). The file must exist.
* **@retval**: If the file is successfully opened, the function returns a pointer to a FILE object that can be used to identify the stream on future operations.
    * Otherwise, a null pointer (NULL) is returned.

### [`int fclose ( FILE * stream );`](https://cplusplus.com/reference/cstdio/fclose/)
* **@param** stream: Pointer to a FILE object that specifies the stream to be closed.
* **@retval**: If the stream is successfully closed, a zero value is returned.
    * On failure, EOF is returned.

### [`size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );`](https://cplusplus.com/reference/cstdio/fwrite/)
* **@param** ptr: Pointer to the array of elements to be written, converted to a const void*.
* **@param** size: Size in bytes of each element to be written.
* **@param** count: Number of elements, each one with a size of size bytes.
* **@param** stream: Pointer to a FILE object that specifies an output stream.
* **@retval**: The total number of elements successfully written is returned.
    * If this number differs from the count parameter, a writing error prevented the function from completing.
    * In this case, the error indicator (ferror) will be set for the stream.
    * If either size or count is zero, the function returns zero and the error indicator remains unchanged.

### [`char * fgets ( char * str, int num, FILE * stream );`](https://cplusplus.com/reference/cstdio/fgets/)
* **@param** str: Pointer to an array of chars where the string read is copied.
* **@param** num: Maximum number of characters to be copied into str (including the terminating null-character).
* **@param** stream: Pointer to a FILE object that identifies an input stream.
    * stdin can be used as argument to read from the standard input.
* **@retval**: On success, the function returns str.
    * If the end-of-file is encountered while attempting to read a character, the eof indicator is set (feof).
    * If this happens before any characters could be read, the pointer returned is a null pointer (and the contents of str remain unchanged).
    * If a read error occurs, the error indicator (ferror) is set and a null pointer is also returned (but the contents pointed by str may have changed).

### [`int fgetc ( FILE * stream );`](https://cplusplus.com/reference/cstdio/FILE/)
* **@param** stream: Pointer to a FILE object that identifies an input stream.
* **@retval**: On success, the character read is returned (promoted to an int value).
    * The return type is int to accommodate for the special value EOF, which indicates failure.
    * If the position indicator was at the end-of-file, the function returns EOF and sets the eof indicator (feof) of stream.
    * If some other reading error happens, the function also returns EOF, but sets its error indicator (ferror) instead.

### [`int fputc ( int character, FILE * stream );`](https://cplusplus.com/reference/cstdio/fputc/)
* **@param** character: The int promotion of the character to be written.
    * The value is internally converted to an unsigned char when written.
* **@param** stream: Pointer to a FILE object that identifies an output stream.
* **@retval**: On success, the character written is returned.
    * If a writing error occurs, EOF is returned and the error indicator (ferror) is set.

### [`int fseek( FILE *stream, long offset, int origin );`](https://cplusplus.com/reference/cstdio/fseek/)
* **@param** stream: file stream to modify
* **@param** offset: number of characters to shift the position relative to origin
* **@param** origin: position to which offset is added. It can have one of the following values: SEEK_SET, SEEK_CUR, SEEK_END
    * **SEEK_SET** argument to fseek indicating seeking from beginning of the file
    * SEEK_CUR argument to fseek indicating seeking from the current file position
    * SEEK_END argument to fseek indicating seeking from end of the file
* **@retval**: ​0​ upon success, nonzero value otherwise.

### [`int printf ( const char * format, ... );`](https://cplusplus.com/reference/cstdio/printf/)
* **@param** format: C string that contains the text to be written to stdout.
    * It can optionally contain embedded format specifiers that are replaced by the values specified in subsequent additional arguments and formatted as requested.
* **@retval**: On success, the total number of characters written is returned.
    * If a writing error occurs, the error indicator (ferror) is set and a negative number is returned.
    * If a multibyte character encoding error occurs while writing wide characters, errno is set to EILSEQ and a negative number is returned.

## [cstring](https://cplusplus.com/reference/cstring/)

### [`int strcmp ( const char * str1, const char * str2 );`](https://cplusplus.com/reference/cstring/strcmp/)
* **@param** str1: C string to be compared.
* **@param** str2: C string to be compared.
* **@retval**: Returns an integral value indicating the relationship between the strings:
    * <0	the first character that does not match has a lower value in ptr1 than in ptr2
    * 0	the contents of both strings are equal
    * \>0	the first character that does not match has a greater value in ptr1 than in ptr2

### [`char * strncpy ( char * destination, const char * source, size_t num );`](https://cplusplus.com/reference/cstring/strncpy/)
* **@param** destination: Pointer to the destination array where the content is to be copied.
* **@param** source: C string to be copied.
* **@param** num: Maximum number of characters to be copied from source.
* **@retval**: destination is returned.

### [`char * strtok ( char * str, const char * delimiters );`](https://cplusplus.com/reference/cstring/strtok/)
* **@param** str: C string to truncate.
    * Notice that this string is modified by being broken into smaller strings (tokens).
    * Alternatively, a null pointer may be specified, in which case the function continues scanning where a previous successful call to the function ended.
* **@param** delimiters: C string containing the delimiter characters.
    * These can be different from one call to another.
* **@retval**: If a token is found, a pointer to the beginning of the token. Otherwise, a null pointer.
    * A null pointer is always returned when the end of the string (i.e., a null character) is reached in the string being scanned.

### [`size_t strlen ( const char * str );`](https://cplusplus.com/reference/cstring/strlen/)
* **@param** str: C string.
* **@retval**: The length of string.

### [`void * memset ( void * ptr, int value, size_t num );`](https://cplusplus.com/reference/cstring/memset/)
* **@param** ptr: Pointer to the block of memory to fill.
* **@param** value: Value to be set. The value is passed as an int, but the function fills the block of memory using the unsigned char conversion of this value.
* **@param** num: Number of bytes to be set to the value.
* **@retval**: ptr is returned.
