/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org>
 */

#include <semver.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#undef SEMVER_FWRITE_STACK_BUFLEN
#define SEMVER_FWRITE_STACK_BUFLEN	64

int
semver_id_fwrite (const semver_id_t * idp, FILE * stream)
/* Serialise the identifier to the  STREAM.  When successful: return the
   number of bytes written and  set "errno" to zero.  When unsuccessful:
   return -1 and set "errno" to an error code. */
{
  size_t	buffer_len = SEMVER_FWRITE_STACK_BUFLEN;
  char		buffer_ptr[buffer_len];
  size_t	needed_count;

  needed_count = (size_t)semver_id_pwrite(idp, buffer_ptr, buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < buffer_len) {
    errno = 0;
    return fwrite(buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    size_t	buffer_len = needed_count+1;
    char *	buffer_ptr;
    errno = 0;
    buffer_ptr = (char *)malloc(buffer_len);
    if (NULL == buffer_ptr) {
      return -1;
    } else {
      size_t	actual_count  = semver_id_pwrite(idp, buffer_ptr, buffer_len);
      size_t	written_count;
      errno = 0;
      written_count = fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      free(buffer_ptr);
      return written_count;
    }
  }
}

int
semver_fwrite (const semver_t * versionp, FILE * stream)
/* Serialise the  version to  the STREAM.   When successful:  return the
   number of bytes written and  set "errno" to zero.  When unsuccessful:
   return -1 and set "errno" to an error code. */
{
  size_t	buffer_len = SEMVER_FWRITE_STACK_BUFLEN;
  char		buffer_ptr[buffer_len];
  size_t	needed_count;

  needed_count = (size_t)semver_pwrite(versionp, buffer_ptr, buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < buffer_len) {
    errno = 0;
    return fwrite(buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    size_t	buffer_len = needed_count+1;
    char *	buffer_ptr;
    errno = 0;
    buffer_ptr = (char *)malloc(buffer_len);
    if (NULL == buffer_ptr) {
      return -1;
    } else {
      size_t	actual_count  = semver_pwrite(versionp, buffer_ptr, buffer_len);
      size_t	written_count;
      errno = 0;
      written_count = fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      free(buffer_ptr);
      return written_count;
    }
  }
}

int
semver_comp_fwrite (const semver_comp_t * compp, FILE * stream)
/* Serialise the comparator to the  STREAM.  When successful: return the
   number of bytes written and  set "errno" to zero.  When unsuccessful:
   return -1 and set "errno" to an error code. */
{
  size_t	buffer_len = SEMVER_FWRITE_STACK_BUFLEN;
  char		buffer_ptr[buffer_len];
  size_t	needed_count;

  needed_count = (size_t)semver_comp_pwrite(compp, buffer_ptr, buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < buffer_len) {
    errno = 0;
    return fwrite(buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    size_t	buffer_len = needed_count+1;
    char *	buffer_ptr;
    errno = 0;
    buffer_ptr = (char *)malloc(buffer_len);
    if (NULL == buffer_ptr) {
      return -1;
    } else {
      size_t	actual_count  = semver_comp_pwrite(compp, buffer_ptr, buffer_len);
      size_t	written_count;
      errno = 0;
      written_count = fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      free(buffer_ptr);
      return written_count;
    }
  }
}

int
semver_range_fwrite (const semver_range_t * rangep, FILE * stream)
/* Serialise  the range  to  the STREAM.   When  successful: return  the
   number of bytes written and  set "errno" to zero.  When unsuccessful:
   return -1 and set "errno" to an error code. */
{
  size_t	buffer_len = SEMVER_FWRITE_STACK_BUFLEN;
  char		buffer_ptr[buffer_len];
  size_t	needed_count;

  needed_count = (size_t)semver_range_pwrite(rangep, buffer_ptr, buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < buffer_len) {
    errno = 0;
    return fwrite(buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    size_t	buffer_len = needed_count+1;
    char *	buffer_ptr;
    errno = 0;
    buffer_ptr = (char *)malloc(buffer_len);
    if (NULL == buffer_ptr) {
      return -1;
    } else {
      size_t	actual_count  = semver_range_pwrite(rangep, buffer_ptr, buffer_len);
      size_t	written_count;
      errno = 0;
      written_count = fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      free(buffer_ptr);
      return written_count;
    }
  }
}

const char *
semver_op_string (enum semver_op op)
{
  switch (op) {
  case SEMVER_OP_EQ:
    return "SEMVER_OP_EQ";
  case SEMVER_OP_LT:
    return "SEMVER_OP_LT";
  case SEMVER_OP_LE:
    return "SEMVER_OP_LE";
  case SEMVER_OP_GT:
    return "SEMVER_OP_GT";
  case SEMVER_OP_GE:
    return "SEMVER_OP_GE";
  default:
    return "invalid value";
  }
}
