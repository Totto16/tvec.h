/*
 * tvec.h
 * based on
 * https://github.com/z-libs/zvec.h/blob/3452eea14a046aebfefb0b0b0f7698932491899e/zvec.h
 *
 * modified to suit my needs
 *
 * License: MIT
 * Author: Zuhaitz
 * Repository: https://github.com/z-libs/zvec.h
 * Version: 1.0.3
 *
 * Modifications by: Totto16
 */

#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum : bool {
  TvecResultErr = false,
  TvecResultOk = true,
} TvecResult;

// Memory Macros.
// If the user hasn't defined their own allocator, use the standard one.
#ifndef T_MALLOC
#define T_MALLOC(sz) malloc(sz)
#define T_CALLOC(n, sz) calloc(n, sz)
#define T_REALLOC(p, sz) realloc(p, sz)
#define T_FREE(p) free(p)
#endif

#ifndef T_VEC_MALLOC
#define T_VEC_MALLOC(sz) T_MALLOC(sz)
#endif

#ifndef T_VEC_CALLOC
#define T_VEC_CALLOC(n, sz) T_CALLOC(n, sz)
#endif

#ifndef T_VEC_REALLOC
#define T_VEC_REALLOC(p, sz) T_REALLOC(p, sz)
#endif

#ifndef T_VEC_FREE
#define T_VEC_FREE(p) T_FREE(p)
#endif

#ifndef T_GROWTH_FACTOR
// Default: Double capacity (2.0x).
#define T_GROWTH_FACTOR(cap) ((cap) == 0 ? 32 : (cap) * 2)

// Alternative: 1.5x Growth (Uncomment to use in your project).
// #define T_GROWTH_FACTOR(cap) ((cap) == 0 ? 32 : (cap) + (cap) / 2)
#endif

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L) ||              \
    defined(__cplusplus)
#define STATIC_ASSERT(check, message) static_assert(check, message)
#define MAYBE_UNUSED [[maybe_unused]]
#elif __STDC_VERSION__ < 201112L
#define MAYBE_UNUSED __attribute__((unused))
// empty, as not supported
#define STATIC_ASSERT(check, message)
#else
#define MAYBE_UNUSED [[maybe_unused]]
#define STATIC_ASSERT(check, message) _Static_assert(check, message)
#endif

// maybe some visibility things later, but I just removed the static inline
#define TVEC_FUN_ATTRIBUTES

#define TVEC_STATIC_INLINE MAYBE_UNUSED static inline

#define TVEC_TYPENAME(TypeName) tvec_##TypeName

#define TVEC_ELEMENT_TYPENAME(Name) tvec_T_##Name

#define TVEC_DEFINE_VEC_TYPE(T) TVEC_DEFINE_VEC_TYPE_EXTENDED(T, T)

#define TVEC_DEFINE_VEC_TYPE_EXTENDED(T, Name)                                 \
  typedef struct {                                                             \
    T *data;                                                                   \
    size_t length;                                                             \
    size_t capacity;                                                           \
  } TVEC_TYPENAME(Name);                                                       \
                                                                               \
  typedef T TVEC_ELEMENT_TYPENAME(Name);                                       \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TVEC_TYPENAME(Name)                        \
      tvec_init_capacity_##Name(size_t cap);                                   \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TVEC_TYPENAME(Name)                        \
      tvec_from_array_##Name(T const *arr, size_t count);                      \
                                                                               \
  TVEC_FUN_ATTRIBUTES                                                          \
  [[nodiscard]] TVEC_STATIC_INLINE bool tvec_is_empty_##Name(                  \
      const TVEC_TYPENAME(Name) vec) {                                         \
    return vec.length == 0;                                                    \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES                                                          \
  [[nodiscard]] TVEC_STATIC_INLINE size_t tvec_length_##Name(                  \
      const TVEC_TYPENAME(Name) vec) {                                         \
    return vec.length;                                                         \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_reserve_##Name(            \
      TVEC_TYPENAME(Name) * vec, size_t new_cap);                              \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult                                 \
  tvec_allocate_uninitialized_##Name(TVEC_TYPENAME(Name) * vec,                \
                                     size_t new_length);                       \
                                                                               \
  TVEC_FUN_ATTRIBUTES                                                          \
  [[nodiscard]] T *tvec_push_slot_##Name(TVEC_TYPENAME(Name) * vec);           \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_push_##Name(               \
      TVEC_TYPENAME(Name) * vec, T value);                                     \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_extend_##Name(             \
      TVEC_TYPENAME(Name) * vec, T const *items, size_t count);                \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_pop_##Name(TVEC_TYPENAME(Name) * vec);         \
                                                                               \
  TVEC_FUN_ATTRIBUTES                                                          \
  [[nodiscard]] T tvec_pop_get_##Name(TVEC_TYPENAME(Name) * vec);              \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T tvec_at_##Name(TVEC_TYPENAME(Name) vec,  \
                                                     size_t index);            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_get_at_mut_##Name(                 \
      TVEC_TYPENAME(Name) * vec, size_t index);                                \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_set_at_##Name(             \
      TVEC_TYPENAME(Name) * vec, size_t index, T value);                       \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T const *tvec_get_at_##Name(               \
      const TVEC_TYPENAME(Name) * vec, size_t index);                          \
                                                                               \
  TVEC_FUN_ATTRIBUTES                                                          \
  [[nodiscard]] T *tvec_data_##Name(TVEC_TYPENAME(Name) * vec);                \
                                                                               \
  TVEC_FUN_ATTRIBUTES                                                          \
  [[nodiscard]] T *tvec_last_##Name(TVEC_TYPENAME(Name) * vec);                \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_remove_##Name(TVEC_TYPENAME(Name) * vec,       \
                                              size_t index);                   \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_swap_remove_##Name(TVEC_TYPENAME(Name) * vec,  \
                                                   size_t index);              \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_clear_##Name(TVEC_TYPENAME(Name) * vec);       \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_free_##Name(TVEC_TYPENAME(Name) * vec);        \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_reverse_##Name(TVEC_TYPENAME(Name) * vec);     \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_sort_##Name(                                   \
      TVEC_TYPENAME(Name) * vec, int (*compar)(T const *, T const *));         \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_bsearch_##Name(                    \
      TVEC_TYPENAME(Name) const * vec,  T const *key,                           \
      int (*compar)(T const *, T const *));                                    \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_lower_bound_##Name(                \
      const TVEC_TYPENAME(Name) * vec, T const *key,                           \
      int (*compar)(T const *, T const *));

#if defined(__cplusplus)
#define TVEC_EMPTY(TypeName) (TVEC_TYPENAME(TypeName){NULL, 0, 0})
#else
#define TVEC_EMPTY(TypeName)                                                   \
  ((TVEC_TYPENAME(TypeName)){.data = NULL, .length = 0, .capacity = 0})
#endif

#define TVEC_ASSERT_SHOULD_USE_PUSH(val)                                       \
  STATIC_ASSERT(sizeof(val) <= 8, "only small values should use push, use "    \
                                  "push slot for larger ones instead!")

#define TVEC_ASSERT_SHOULD_USE_PUSH_SLOT(val)                                  \
  STATIC_ASSERT(sizeof(val) > 8, "only big values should use push slot, use "  \
                                 "push for smaller ones instead!")

#define TVEC_IS_EMPTY(Name, v) tvec_is_empty_##Name(v)
#define TVEC_LENGTH(Name, v) tvec_length_##Name(v)
#define TVEC_PUSH(Name, v, val) tvec_push_##Name(v, val)
#define TVEC_PUSH_SLOT(Name, v) tvec_push_slot_##Name(v)
#define TVEC_EXTEND(Name, v, arr, count) tvec_extend_##Name(v, arr, count)
#define TVEC_RESERVE(Name, v, cap) tvec_reserve_##Name(v, cap)
#define TVEC_ALLOCATE_UNINITIALIZED(Name, v, length)                           \
  tvec_allocate_uninitialized_##Name(v, length)
#define TVEC_AT(Name, v, idx) tvec_at_##Name(v, idx)
#define TVEC_GET_AT(Name, v, idx) tvec_get_at_##Name(v, idx)
#define TVEC_GET_AT_MUT(Name, v, idx) tvec_get_at_mut_##Name(v, idx)
#define TVEC_SET_AT(Name, v, idx, value) tvec_set_at_##Name(v, idx, value)
#define TVEC_DATA(Name, v) tvec_data_##Name(v)
#define TVEC_LAST(Name, v) tvec_last_##Name(v)
#define TVEC_FREE(Name, v) tvec_free_##Name(v)
#define TVEC_POP(Name, v) tvec_pop_##Name(v)
#define TVEC_POP_GET(Name, v) tvec_pop_get_##Name(v)
#define TVEC_SHRINK_TO_FIT(Name, v) tvec_shrink_to_fit_##Name(v)
#define TVEC_REMOVE(Name, v, i) tvec_remove_##Name(v, i)
#define TVEC_SWAP_REMOVE(Name, v, i) tvec_swap_remove_##Name(v, i)
#define TVEC_CLEAR(Name, v) tvec_clear_##Name(v)
#define TVEC_REVERSE(Name, v) tvec_reverse_##Name(v)
#define TVEC_SORT(Name, v, cmp) tvec_sort_##Name(v, cmp)
#define TVEC_BSEARCH(Name, v, key, cmp) tvec_bsearch_##Name(v, key, cmp)
#define TVEC_LOWER_BOUND(Name, v, key, cmp) tvec_lower_bound_##Name(v, key, cmp)
#define TVEC_FROM(Name, arr, size) tvec_from_array_##Name(arr, size)

#define TVEC_INIT(T) TVEC_EMPTY(T)

#define TVEC_INIT_WITH_CAP(Name, cap) tvec_init_capacity_##Name(cap)

#define tvec_from(Name, ...)                                                   \
  tvec_from_array_##Name((TVEC_ELEMENT_TYPENAME(Name)[])__VA_ARGS__,           \
                         sizeof((TVEC_ELEMENT_TYPENAME(Name)[])__VA_ARGS__) /  \
                             sizeof(TVEC_ELEMENT_TYPENAME(Name)))

#define TVEC_IMPLEMENT_VEC_TYPE(T) TVEC_IMPLEMENT_VEC_TYPE_EXTENDED(T, T)

#define TVEC_IMPLEMENT_VEC_TYPE_EXTENDED(T, Name)                              \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TVEC_TYPENAME(Name)                        \
      tvec_init_capacity_##Name(const size_t cap) {                                  \
    TVEC_TYPENAME(Name) vec;                                                   \
    memset(&vec, 0, sizeof(TVEC_TYPENAME(Name)));                              \
    if (cap > 0) {                                                             \
      TvecResult _ = tvec_reserve_##Name(&vec, cap);                           \
      (void)_;                                                                 \
    }                                                                          \
    return vec;                                                                \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TVEC_TYPENAME(Name)                        \
      tvec_from_array_##Name(T const *const arr, const size_t count) {               \
    TVEC_TYPENAME(Name) vec = tvec_init_capacity_##Name(count);                \
    if (vec.data) {                                                            \
      memcpy((void *)vec.data, (void *)arr, count * sizeof(T));                \
      vec.length = count;                                                      \
    }                                                                          \
    return vec;                                                                \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_reserve_##Name(            \
      TVEC_TYPENAME(Name) * const vec, const size_t new_cap) {                             \
    if (new_cap <= vec->capacity) {                                            \
      return TvecResultOk;                                                     \
    }                                                                          \
    T *new_data = (T *)T_VEC_REALLOC((void *)vec->data, new_cap * sizeof(T));  \
    if (!new_data) {                                                           \
      return TvecResultErr;                                                    \
    }                                                                          \
    vec->data = new_data;                                                      \
    vec->capacity = new_cap;                                                   \
    return TvecResultOk;                                                       \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult                                 \
  tvec_allocate_uninitialized_##Name(TVEC_TYPENAME(Name) * const vec,                \
                                     const size_t new_length) {                      \
    const TvecResult reserve_result = tvec_reserve_##Name(vec, new_length);    \
    if (reserve_result == TvecResultErr) {                                     \
      return TvecResultErr;                                                    \
    }                                                                          \
    if (new_length <= vec->length) {                                           \
      return TvecResultOk;                                                     \
    }                                                                          \
    vec->length = new_length;                                                  \
    return TvecResultOk;                                                       \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_push_slot_##Name(                  \
      TVEC_TYPENAME(Name) * const vec) {                                             \
    if (vec->length >= vec->capacity) {                                        \
      size_t new_cap = T_GROWTH_FACTOR(vec->capacity);                         \
      if (tvec_reserve_##Name(vec, new_cap) != TvecResultOk)                   \
        return NULL;                                                           \
    }                                                                          \
    return &vec->data[vec->length++];                                          \
  }                                                                            \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_push_##Name(               \
      TVEC_TYPENAME(Name) * const vec, T const value) {                                    \
    T *slot = tvec_push_slot_##Name(vec);                                      \
    if (!slot) {                                                               \
      return TvecResultErr;                                                    \
    }                                                                          \
    *slot = value;                                                             \
    return TvecResultOk;                                                       \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_extend_##Name(             \
      TVEC_TYPENAME(Name) * const vec,  T const *const items, const size_t count) {         \
    if (vec->length + count > vec->capacity) {                                 \
      size_t new_cap = T_GROWTH_FACTOR(vec->capacity);                         \
      while (new_cap < vec->length + count) {                                  \
        new_cap = T_GROWTH_FACTOR(new_cap);                                    \
      }                                                                        \
      if (tvec_reserve_##Name(vec, new_cap) != TvecResultOk) {                 \
        return TvecResultErr;                                                  \
      }                                                                        \
    }                                                                          \
    memcpy((void *)(vec->data + vec->length), (void *)items,                   \
           count * sizeof(T));                                                 \
    vec->length += count;                                                      \
    return TvecResultOk;                                                       \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_pop_##Name(TVEC_TYPENAME(Name) * const vec) {        \
    assert(vec->length > 0 && "Popping empty vector");                         \
    vec->length--;                                                             \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T tvec_pop_get_##Name(                     \
      TVEC_TYPENAME(Name) * const vec) {                                             \
    assert(vec->length > 0 && "Vector is empty, cannot pop!");                 \
    return vec->data[--vec->length];                                           \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T tvec_at_##Name(TVEC_TYPENAME(Name) const vec,  \
                                                     const size_t index) {           \
    assert(index < vec.length && "Vector index out of bounds!");               \
    return vec.data[index];                                                    \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_get_at_mut_##Name(                 \
      TVEC_TYPENAME(Name) * const vec, const size_t index) {                               \
    return (index < vec->length) ? &vec->data[index] : NULL;                   \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_set_at_##Name(             \
      TVEC_TYPENAME(Name) * const vec, const size_t index, T const value) {                      \
    if (index >= vec->length) {                                                \
      return TvecResultErr;                                                    \
    }                                                                          \
    vec->data[index] = value;                                                  \
    return TvecResultOk;                                                       \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T const *tvec_get_at_##Name(               \
      TVEC_TYPENAME(Name) const *const vec, const size_t index) {                    \
    return (index < vec->length) ? &vec->data[index] : NULL;                   \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_data_##Name(TVEC_TYPENAME(Name) *const  \
                                                        vec) {                 \
    return vec->data;                                                          \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_last_##Name(TVEC_TYPENAME(Name) *const  \
                                                        vec) {                 \
    return (vec->length > 0) ? &vec->data[vec->length - 1] : NULL;             \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_remove_##Name(TVEC_TYPENAME(Name) *const  vec,       \
                                              const size_t index) {                  \
    if (index >= vec->length)                                                  \
      return;                                                                  \
    memmove((void *)(&vec->data[index]), (void *)(&vec->data[index + 1]),      \
            (vec->length - index - 1) * sizeof(T));                            \
    vec->length--;                                                             \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_swap_remove_##Name(TVEC_TYPENAME(Name) *const vec,  \
                                                   const size_t index) {             \
    if (index >= vec->length) {                                                \
      return;                                                                  \
    }                                                                          \
    vec->data[index] = vec->data[--vec->length];                               \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_clear_##Name(TVEC_TYPENAME(Name) * const vec) {      \
    vec->length = 0;                                                           \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_free_##Name(TVEC_TYPENAME(Name) *const vec) {  \
    T_VEC_FREE((void *)vec->data);                                             \
    *vec = TVEC_EMPTY(Name);                                                   \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_reverse_##Name(TVEC_TYPENAME(Name) * const vec) {    \
    if (vec->length < 2) {                                                     \
      return;                                                                  \
    }                                                                          \
    size_t i = 0;                                                              \
    size_t j = vec->length - 1;                                                \
    while (i < j) {                                                            \
      T temp = vec->data[i];                                                   \
      vec->data[i] = vec->data[j];                                             \
      vec->data[j] = temp;                                                     \
      i++;                                                                     \
      j--;                                                                     \
    }                                                                          \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_sort_##Name(                                   \
      TVEC_TYPENAME(Name) * const vec, int (* const compar )(T const * const, T const * const)) {        \
    if (vec->length > 1) {                                                     \
      qsort((void *)vec->data, vec->length, sizeof(T),                         \
            (int (* const)(const void * const, const void * const))compar);                      \
    }                                                                          \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_bsearch_##Name(                    \
      const TVEC_TYPENAME(Name) *const vec, T const * const key,                      \
      int (*const compar)(T const *, T const *)) {                                   \
    if (vec->length == 0) {                                                    \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    return (T *)bsearch((void *)key, (void *)vec->data, vec->length,           \
                        sizeof(T),                                             \
                        (int (* const )(const void *const , const void * const))compar);          \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_lower_bound_##Name(                \
      const TVEC_TYPENAME(Name) *const vec, T const *key,                      \
      int (* const compar)(T const *, T const *)) {                                   \
    size_t left = 0;                                                           \
    size_t right = vec->length;                                                \
    while (left < right) {                                                     \
      size_t mid = left + ((right - left) / 2);                                \
      if (compar((T const *)&vec->data[mid], key) < 0) {                       \
        left = mid + 1;                                                        \
      } else {                                                                 \
        right = mid;                                                           \
      }                                                                        \
    }                                                                          \
    if (left == vec->length) {                                                 \
      return NULL;                                                             \
    }                                                                          \
    return &vec->data[left];                                                   \
  }

#define TVEC_DEFINE_AND_IMPLEMENT_VEC_TYPE(T)                                  \
  TVEC_DEFINE_VEC_TYPE(T)                                                      \
  TVEC_IMPLEMENT_VEC_TYPE(T)

#define TVEC_DEFINE_AND_IMPLEMENT_VEC_TYPE_EXTENDED(T, Name)                   \
  TVEC_DEFINE_VEC_TYPE_EXTENDED(T, Name)                                       \
  TVEC_IMPLEMENT_VEC_TYPE_EXTENDED(T, Name)

#ifdef __cplusplus
// extern "C" {
}
#endif
