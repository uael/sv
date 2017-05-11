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
#include <stdio.h>
#include <string.h>

#define STRNSIZE(s) (s), sizeof(s)-1

int test_semver(const char *expected, const char *str, size_t len) {
  size_t offset = 0;
  int slen;
  char buffer[1024];
  sv_t comp = {0};

  if (sv_read(&comp, str, len, &offset)) {
    return 1;
  }
  slen = sv_snprint(comp, buffer, 1024);
  printf("%.*s", slen, buffer);
  if (memcmp(expected, buffer, strlen(expected))) {
    printf(" != %s\n", expected);
    sv_dtor(&comp);
    return 1;
  }
  printf(" == %s\n", expected);
  sv_dtor(&comp);
  return 0;
}

int main(int argc, char *argv[]) {
  if (test_semver("1.2.3", STRNSIZE("v1.2.3"))) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}