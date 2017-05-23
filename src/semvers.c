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

#include <stdio.h>
#include <stdlib.h>

#include "semvers.h"

#define ISPOW2(n) (((n) & -(n)) == (n))

static uint32_t roundup32(uint32_t n) {
  uint32_t j;

  return ISPOW2(n) ? n : (
    ((j = n & 0xFFFF0000) || (j = n)),
      ((n = j & 0xFF00FF00) || (n = j)),
      ((j = n & 0xF0F0F0F0) || (j = n)),
      ((n = j & 0xCCCCCCCC) || (n = j)),
      ((j = n & 0xAAAAAAAA) || (j = n)),
      j << 1
  );
}

uint32_t semvers_pgrowth(semvers_t *self, const int32_t nmin) {
  if (nmin > 0) {
    uint32_t unmin = (uint32_t) nmin;

    if (self->capacity) {
      if (self->capacity < unmin) {
        if (ISPOW2(unmin)) {
          self->capacity = unmin;
        } else {
          do self->capacity *= 2; while (self->capacity < unmin);
        }
        self->data = realloc(self->data, sizeof(semver_t) * self->capacity);
      }
    } else {
      if (unmin == SEMVERS_MIN_CAP || (unmin > SEMVERS_MIN_CAP && ISPOW2(unmin))) {
        self->capacity = unmin;
      } else {
        self->capacity = SEMVERS_MIN_CAP;
        while (self->capacity < unmin) self->capacity *= 2;
      }
      self->data = sv_malloc(sizeof(semver_t) * self->capacity);
    }
    return unmin;
  }
  return 0;
}

static int semvers_qsort_fn(const void *a, const void *b) {
  return semver_pcmp(a, b);
}

void semvers_sort(semvers_t *self) {
  qsort((char *) self->data, self->length, sizeof(semver_t), semvers_qsort_fn);
}

static int semvers_rqsort_fn(const void *a, const void *b) {
  return semver_pcmp(b, a);
}

void semvers_rsort(semvers_t *self) {
  qsort((char *) self->data, self->length, sizeof(semver_t), semvers_rqsort_fn);
}
