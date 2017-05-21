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
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int test_id_fwrite(void) {
  static const char	input_str[] = "1.2.3-alpha.1+x86-64";
  semver_id_t		id;
  size_t		offset = 0;
  int			rv;

  rv = semver_id_read(&id, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("test_id_fwrite: ");
    semver_id_fwrite(&id, stdout);
    if (0 == errno) {
      printf("\n");
      rv = 0;
    }
  }
  semver_id_dtor(&id);
  return rv;
}

int test_version_fwrite(void) {
  static const char	input_str[] = "1.2.3-alpha.1+x86-64";
  semver_t		version;
  size_t		offset = 0;
  int			rv;

  rv = semver_read(&version, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("test_version_fwrite: ");
    semver_fwrite(&version, stdout);
    if (0 == errno) {
      printf("\n");
      rv = 0;
    }
  }
  semver_dtor(&version);
  return 0;
}

int test_comparator_fwrite(void) {
  static const char	input_str[] = "<=1.2.3";
  semver_comp_t		comp;
  size_t		offset = 0;
  int			rv;

  rv = semver_comp_read(&comp, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("test_comparator_fwrite: ");
    semver_comp_fwrite(&comp, stdout);
    if (0 == errno) {
      printf("\n");
      rv = 0;
    }
  }
  semver_comp_dtor(&comp);
  return rv;
}

int test_range_fwrite(void) {
  static const char	input_str[] = ">=1.2.3 <4.0.0";
  semver_range_t	range;
  size_t		offset = 0;
  int			rv;

  rv = semver_range_read(&range, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("test_range_fwrite: ");
    semver_range_fwrite(&range, stdout);
    if (0 == errno) {
      printf("\n");
      rv = 0;
    }
  }
  semver_range_dtor(&range);
  return rv;
}

int main(void) {
  if (test_id_fwrite()) {
    return EXIT_FAILURE;
  }

  if (test_version_fwrite()) {
    return EXIT_FAILURE;
  }

  if (test_comparator_fwrite()) {
    return EXIT_FAILURE;
  }

  if (test_range_fwrite()) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
