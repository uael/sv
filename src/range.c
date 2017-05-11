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

#include <string.h>
#include <stdlib.h>
#include <semver.h>

#ifdef _MSC_VER
# define snprintf(s, maxlen, fmt, ...) _snprintf_s(s, MSG_SIZE, maxlen, fmt, __VA_ARGS__)
#endif

void sv_range_dtor(sv_range_t *self) {
  if (self && self->next) {
    sv_range_dtor(self->next);
    free(self->next);
    self->next = NULL;
  }
}

void sv_range_ctor(sv_range_t *self, const char *str, size_t len) {
//  int offset = 0, i, j = 0, tidle, caret, star_ok;
//  sv_range_t *current = NULL, *tail = NULL;
//  char have[3] = {0, 0, 0}, star[3] = {0, 0, 0};
//  unsigned int bits[3] = {0, 0, 0};
//
//  current = self;
//
//  while (offset < len) {
//    tidle = 0;
//    caret = 0;
//    star_ok = 1;
//
//    if (current == NULL) {
//      current = malloc(sizeof(sv_range_t));
//    }
//    *current = (sv_range_t) {0};
//
//    /* skip whitespace */
//    if (str[offset] == ' ') {
//      while (str[offset] == ' ') {
//        offset++;
//      }
//    }
//
//    if (str[offset] == '>') {
//      offset++;
//
//      star_ok = 0;
//
//      current->has_lower = 1;
//      current->has_upper = 0;
//
//      if (str[offset] == '=') {
//        current->includes_lower = 1;
//
//        offset++;
//      } else {
//        current->includes_lower = 0;
//      }
//    } else if (str[offset] == '<') {
//      offset++;
//
//      star_ok = 0;
//
//      current->has_lower = 0;
//      current->has_upper = 1;
//
//      current->includes_lower = 0;
//
//      if (str[offset] == '=') {
//        current->includes_upper = 1;
//
//        offset++;
//      } else {
//        current->includes_upper = 0;
//      }
//    } else if (str[offset] == '~') {
//      offset++;
//
//      star_ok = 0;
//      tidle = 1;
//
//      current->has_lower = 1;
//      current->has_upper = 1;
//
//      current->includes_lower = 1;
//      current->includes_upper = 0;
//    } else if (str[offset] == '^') {
//      offset++;
//
//      star_ok = 0;
//      caret = 1;
//
//      current->has_lower = 1;
//      current->has_upper = 1;
//
//      current->includes_lower = 1;
//      current->includes_upper = 0;
//    } else {
//      if (str[offset] == '=') {
//        offset++;
//
//        star_ok = 0;
//      }
//
//      current->has_lower = 1;
//      current->has_upper = 1;
//
//      current->includes_lower = 1;
//      current->includes_upper = 1;
//    }
//
//    if (current->has_lower) {
//      current->lower = calloc(1, sizeof(sv_t));
//    }
//
//    if (current->has_upper) {
//      current->upper = calloc(1, sizeof(sv_t));
//    }
//
//    /* skip whitespace */
//    if (str[offset] == ' ') {
//      while (str[offset] == ' ') {
//        offset++;
//      }
//    }
//
//    for (i = offset; i <= len; ++i) {
//      if (str[i] == '.' || str[i] == ' ' || i == len) {
//        have[j] = 1;
//
//        if (str[offset] == 'x' || str[offset] == '*') {
//          if (!star_ok) {
//            return;
//          }
//
//          star[j] = 1;
//          current->includes_upper = 0;
//        } else {
//          bits[j] = (unsigned int) strtol(str + offset, NULL, 10);
//        }
//
//        j++;
//
//        /* skip "." if that's why we're here */
//        if (i < len && str[i] == '.') {
//          i++;
//        }
//
//        offset = i;
//
//        /* we're done */
//        if (j == 3) {
//          break;
//        }
//      }
//    }
//
//    if (current->has_lower && have[2]) {
//      current->lower->patch = bits[2];
//    }
//    if (current->has_upper && have[2]) {
//      current->upper->patch = bits[2];
//    }
//    if (current->has_lower && have[1]) {
//      current->lower->minor = bits[1];
//    }
//    if (current->has_upper && have[1]) {
//      current->upper->minor = bits[1];
//    }
//    if (current->has_lower && have[0]) {
//      current->lower->major = bits[0];
//    }
//    if (current->has_upper && have[0]) {
//      current->upper->major = bits[0];
//    }
//
//    if (tidle) {
//      if (have[2]) {
//        current->upper->patch = 0;
//        current->upper->minor++;
//      } else if (have[1]) {
//        current->upper->patch = 0;
//        current->upper->minor++;
//      } else {
//        current->upper->major++;
//      }
//    } else if (caret) {
//      if (current->upper->major > 0) {
//        current->upper->patch = 0;
//        current->upper->minor = 0;
//        current->upper->major++;
//      } else if (current->upper->minor > 0) {
//        current->upper->patch = 0;
//        current->upper->minor++;
//      }
//    } else if (star[0]) {
//      current->has_upper = 0;
//      current->has_lower = 0;
//    } else if (star[1]) {
//      current->upper->patch = 0;
//      current->upper->minor = 0;
//      current->upper->major++;
//    } else if (star[2]) {
//      current->upper->patch = 0;
//      current->upper->minor++;
//    }
//
//    if (tail != NULL) {
//      tail->next = current;
//    }
//
//    tail = current;
//  }
}

static char sv_range_compare_one(const sv_range_t range, const sv_t sv) {
//  if (range.has_lower && sv_comp(sv, *range.lower) < 1 - range.includes_lower) {
//    return -1;
//  }
//
//  if (range.has_upper && sv_comp(sv, *range.upper) >= range.includes_upper) {
//    return 1;
//  }

  return 0;
}

char sv_range_comp(const sv_range_t self, const sv_t sv) {
  const sv_range_t *current = &self;

  do
    if (sv_range_compare_one(*current, sv) != 0) {
      return 0;
    }
  while ((current = current->next) != NULL);

  return 1;
}
