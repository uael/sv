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
#include <ctype.h>
#include <semver.h>
#include <stdio.h>

#ifdef _MSC_VER
# define snprintf(s, maxlen, fmt, ...) _snprintf_s(s, _TRUNCATE, maxlen, fmt, __VA_ARGS__)
#endif

void semver_id_ctor(semver_id_t *self) {
#ifndef _MSC_VER
  *self = (semver_id_t) {true};
#else
  self->next = NULL;
  self->len = 0;
  self->raw = NULL;
  self->num = 0;
  self->numeric = true;
#endif
}

void semver_id_dtor(semver_id_t *self) {
  if (self && self->next) {
    semver_id_dtor(self->next);
    sv_free(self->next);
    self->next = NULL;
  }
}

char semver_id_read(semver_id_t *self, const char *str, size_t len, size_t *offset) {
  size_t i = 0;
  char is_zero = 0;

  semver_id_ctor(self);
  while (*offset < len) {
    if (isalnum(str[*offset]) || str[*offset] == '-') {
      if (!isdigit(str[*offset])) {
        is_zero = 0;
        self->numeric = false;
      } else {
        if (i == 0) {
          is_zero = str[*offset] == '0';
        } else if (is_zero) {
          return 1;
        }
      }
      ++i, ++*offset;
      continue;
    }
    break;
  }
  if (!i) {
    return 1;
  }
  self->raw = str + *offset - i;
  self->len = i;
  if (!is_zero && self->numeric) {
    self->num = (int) strtol(self->raw, NULL, 0);
  }
  if (str[*offset] == '.') {
    self->next = (semver_id_t *) sv_malloc(sizeof(semver_id_t));
    if (self->next == NULL) {
      return 1;
    }
    ++*offset;
    return semver_id_read(self->next, str, len, offset);
  }
  return 0;
}

char semver_id_pcomp(const semver_id_t *self, const semver_id_t *other) {
  char s;

  if (self->len && !other->len) {
    return -1;
  }
  if (!self->len && other->len) {
    return 1;
  }
  if (!self->len) {
    return 0;
  }

  if (self->numeric && other->numeric) {
    if (self->num > other->num) {
      return 1;
    }
    if (self->num < other->num) {
      return -1;
    }
  } else if ((s = (char) memcmp(self->raw, other->raw, self->len > other->len ? self->len : other->len)) != 0) {
    return s;
  }

  if (!self->next && other->next) {
    return -1;
  }
  if (self->next && !other->next) {
    return 1;
  }
  if (!self->next) {
    return 0;
  }

  return semver_id_pcomp(self->next, other->next);
}

int semver_id_pwrite(const semver_id_t *self, char *buffer, size_t len) {
  char next[1024];

  if (self->next) {
    return snprintf(buffer, len, "%.*s.%.*s", (int) self->len, self->raw, semver_id_pwrite(self->next, next, 1024), next);
  }
  return snprintf(buffer, len, "%.*s", (int) self->len, self->raw);
}

int
semver_id_fwrite (const semver_id_t * idp, FILE * stream)
{
  size_t	buffer_len = 64;
  char		buffer_ptr[buffer_len];
  size_t	needed_count;

  needed_count = (size_t)semver_id_pwrite(idp, buffer_ptr, buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < buffer_len) {
    return fwrite(buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    size_t	buffer_len = needed_count+1;
    char	buffer_ptr[buffer_len];
    size_t	actual_count = semver_id_pwrite(idp, buffer_ptr, buffer_len);
    return fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
  }
}
