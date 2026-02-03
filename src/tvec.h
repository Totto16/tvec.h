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
#elif __STDC_VERSION__ < 201112L
// empty, as not supported
#define STATIC_ASSERT(check, message)
#else
#define STATIC_ASSERT(check, message) _Static_assert(check, message)
#endif

// maybe some visibility things later, but I just removed the static inline
#define TVEC_FUN_ATTRIBUTES

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
      tvec_from_array_##Name(const T *arr, size_t count);                      \
                                                                               \
  TVEC_FUN_ATTRIBUTES                                                          \
  [[nodiscard]] bool tvec_is_empty_##Name(TVEC_TYPENAME(Name) v);              \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_reserve_##Name(            \
      TVEC_TYPENAME(Name) * v, size_t new_cap);                                \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult                                 \
  tvec_allocate_uninitialized_##Name(TVEC_TYPENAME(Name) * v,                  \
                                     size_t new_length);                       \
                                                                               \
  TVEC_FUN_ATTRIBUTES                                                          \
  [[nodiscard]] T *tvec_push_slot_##Name(TVEC_TYPENAME(Name) * v);             \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_push_##Name(               \
      TVEC_TYPENAME(Name) * v, T value);                                       \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_extend_##Name(             \
      TVEC_TYPENAME(Name) * v, const T *items, size_t count);                  \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_pop_##Name(TVEC_TYPENAME(Name) * v);           \
                                                                               \
  TVEC_FUN_ATTRIBUTES                                                          \
  [[nodiscard]] T tvec_pop_get_##Name(TVEC_TYPENAME(Name) * v);                \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T tvec_at_##Name(TVEC_TYPENAME(Name) v,    \
                                                     size_t index);            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_get_at_mut_##Name(                 \
      TVEC_TYPENAME(Name) * v, size_t index);                                  \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_set_at_##Name(             \
      TVEC_TYPENAME(Name) * v, size_t index, T value);                         \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T const *tvec_get_at_##Name(               \
      const TVEC_TYPENAME(Name) * v, size_t index);                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES                                                          \
  [[nodiscard]] T *tvec_data_##Name(TVEC_TYPENAME(Name) * v);                  \
                                                                               \
  TVEC_FUN_ATTRIBUTES                                                          \
  [[nodiscard]] T *tvec_last_##Name(TVEC_TYPENAME(Name) * v);                  \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_remove_##Name(TVEC_TYPENAME(Name) * v,         \
                                              size_t index);                   \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_swap_remove_##Name(TVEC_TYPENAME(Name) * v,    \
                                                   size_t index);              \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_clear_##Name(TVEC_TYPENAME(Name) * v);         \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_free_##Name(TVEC_TYPENAME(Name) * v);          \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_reverse_##Name(TVEC_TYPENAME(Name) * v);       \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_sort_##Name(                                   \
      TVEC_TYPENAME(Name) * v, int (*compar)(T const *, T const *));           \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_bsearch_##Name(                    \
      const TVEC_TYPENAME(Name) * v, const T *key,                             \
      int (*compar)(T const *, T const *));                                    \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_lower_bound_##Name(                \
      const TVEC_TYPENAME(Name) * v, T const *key,                             \
      int (*compar)(T const *, T const *));

#if defined(__cplusplus)
#define TVEC_EMPTY(TypeName) (TVEC_TYPENAME(TypeName){NULL, 0, 0})
#else
#define TVEC_EMPTY(TypeName)                                                   \
  ((TVEC_TYPENAME(TypeName)){.data = NULL, .length = 0, .capacity = 0})
#endif

#define TVEC_LENGTH(v) (v).length

#define TVEC_ASSERT_SHOULD_USE_PUSH(val)                                       \
  STATIC_ASSERT(sizeof(val) <= 8, "only small values should use push, use "    \
                                  "push slot for larger ones instead!")

#define TVEC_ASSERT_SHOULD_USE_PUSH_SLOT(val)                                  \
  STATIC_ASSERT(sizeof(val) > 8, "only big values should use push slot, use "  \
                                 "push for smaller ones instead!")

#define TVEC_IS_EMPTY(Name, v) tvec_is_empty_##Name(v)
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

#define TVEC_CAT(a, b) a##b
#define TVEC_NAME(a, b) TVEC_CAT(a, b)

#define tvec_from(Name, ...)                                                   \
  tvec_from_array_##Name((TVEC_ELEMENT_TYPENAME(Name)[])__VA_ARGS__,           \
                         sizeof((TVEC_ELEMENT_TYPENAME(Name)[])__VA_ARGS__) /  \
                             sizeof(TVEC_ELEMENT_TYPENAME(Name)))

#define tvec_foreach(v, iter)                                                  \
  for (size_t TVEC_NAME(_i_, __LINE__) = 0;                                    \
       TVEC_NAME(_i_, __LINE__) < (v)->length &&                               \
       ((iter) = &(v)->data[TVEC_NAME(_i_, __LINE__)]);                        \
       ++TVEC_NAME(_i_, __LINE__))

#define TVEC_IMPLEMENT_VEC_TYPE(T) TVEC_IMPLEMENT_VEC_TYPE_EXTENDED(T, T)

#define TVEC_IMPLEMENT_VEC_TYPE_EXTENDED(T, Name)                              \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TVEC_TYPENAME(Name)                        \
      tvec_init_capacity_##Name(size_t cap) {                                  \
    TVEC_TYPENAME(Name) v;                                                     \
    memset(&v, 0, sizeof(TVEC_TYPENAME(Name)));                                \
    if (cap > 0) {                                                             \
      TvecResult _ = tvec_reserve_##Name(&v, cap);                             \
      (void)_;                                                                 \
    }                                                                          \
    return v;                                                                  \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TVEC_TYPENAME(Name)                        \
      tvec_from_array_##Name(const T *const arr, size_t count) {               \
    TVEC_TYPENAME(Name) v = tvec_init_capacity_##Name(count);                  \
    if (v.data) {                                                              \
      memcpy(v.data, arr, count * sizeof(T));                                  \
      v.length = count;                                                        \
    }                                                                          \
    return v;                                                                  \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES                                                          \
  [[nodiscard]] bool tvec_is_empty_##Name(TVEC_TYPENAME(Name) v) {             \
    return v.length == 0;                                                      \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_reserve_##Name(            \
      TVEC_TYPENAME(Name) * v, size_t new_cap) {                               \
    if (new_cap <= v->capacity)                                                \
      return TvecResultOk;                                                     \
    T *new_data = T_VEC_REALLOC(v->data, new_cap * sizeof(T));                 \
    if (!new_data)                                                             \
      return TvecResultErr;                                                    \
    v->data = new_data;                                                        \
    v->capacity = new_cap;                                                     \
    return TvecResultOk;                                                       \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult                                 \
  tvec_allocate_uninitialized_##Name(TVEC_TYPENAME(Name) * v,                  \
                                     size_t new_length) {                      \
    const TvecResult reserve_result = tvec_reserve_##Name(v, new_length);      \
    if (reserve_result == TvecResultErr) {                                     \
      return TvecResultErr;                                                    \
    }                                                                          \
    if (new_length <= v->length) {                                             \
      return TvecResultOk;                                                     \
    }                                                                          \
    v->length = new_length;                                                    \
    return TvecResultOk;                                                       \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_push_slot_##Name(                  \
      TVEC_TYPENAME(Name) * v) {                                               \
    if (v->length >= v->capacity) {                                            \
      size_t new_cap = T_GROWTH_FACTOR(v->capacity);                           \
      if (tvec_reserve_##Name(v, new_cap) != TvecResultOk)                     \
        return NULL;                                                           \
    }                                                                          \
    return &v->data[v->length++];                                              \
  }                                                                            \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_push_##Name(               \
      TVEC_TYPENAME(Name) * v, T value) {                                      \
    T *slot = tvec_push_slot_##Name(v);                                        \
    if (!slot) {                                                               \
      return TvecResultErr;                                                    \
    }                                                                          \
    *slot = value;                                                             \
    return TvecResultOk;                                                       \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_extend_##Name(             \
      TVEC_TYPENAME(Name) * v, const T *const items, size_t count) {           \
    if (v->length + count > v->capacity) {                                     \
      size_t new_cap = T_GROWTH_FACTOR(v->capacity);                           \
      while (new_cap < v->length + count)                                      \
        new_cap = T_GROWTH_FACTOR(new_cap);                                    \
      if (tvec_reserve_##Name(v, new_cap) != TvecResultOk)                     \
        return TvecResultErr;                                                  \
    }                                                                          \
    memcpy(v->data + v->length, items, count * sizeof(T));                     \
    v->length += count;                                                        \
    return TvecResultOk;                                                       \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_pop_##Name(TVEC_TYPENAME(Name) * v) {          \
    assert(v->length > 0 && "Popping empty vector");                           \
    v->length--;                                                               \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T tvec_pop_get_##Name(                     \
      TVEC_TYPENAME(Name) * v) {                                               \
    assert(v->length > 0 && "Vector is empty, cannot pop!");                   \
    return v->data[--v->length];                                               \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T tvec_at_##Name(TVEC_TYPENAME(Name) v,    \
                                                     size_t index) {           \
    assert(index < v.length && "Vector index out of bounds!");                 \
    return v.data[index];                                                      \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_get_at_mut_##Name(                 \
      TVEC_TYPENAME(Name) * v, size_t index) {                                 \
    return (index < v->length) ? &v->data[index] : NULL;                       \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_set_at_##Name(             \
      TVEC_TYPENAME(Name) * v, size_t index, T value) {                        \
    if (index >= v->length) {                                                  \
      return TvecResultErr;                                                    \
    }                                                                          \
    v->data[index] = value;                                                    \
    return TvecResultOk;                                                       \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T const *tvec_get_at_##Name(               \
      TVEC_TYPENAME(Name) const *const v, size_t index) {                      \
    return (index < v->length) ? &v->data[index] : NULL;                       \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_data_##Name(TVEC_TYPENAME(Name) *  \
                                                        v) {                   \
    return v->data;                                                            \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_last_##Name(TVEC_TYPENAME(Name) *  \
                                                        v) {                   \
    return (v->length > 0) ? &v->data[v->length - 1] : NULL;                   \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_remove_##Name(TVEC_TYPENAME(Name) * v,         \
                                              size_t index) {                  \
    if (index >= v->length)                                                    \
      return;                                                                  \
    memmove(&v->data[index], &v->data[index + 1],                              \
            (v->length - index - 1) * sizeof(T));                              \
    v->length--;                                                               \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_swap_remove_##Name(TVEC_TYPENAME(Name) * v,    \
                                                   size_t index) {             \
    if (index >= v->length)                                                    \
      return;                                                                  \
    v->data[index] = v->data[--v->length];                                     \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_clear_##Name(TVEC_TYPENAME(Name) * v) {        \
    v->length = 0;                                                             \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_free_##Name(TVEC_TYPENAME(Name) * v) {         \
    T_VEC_FREE(v->data);                                                       \
    *v = TVEC_EMPTY(Name);                                                     \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_reverse_##Name(TVEC_TYPENAME(Name) * v) {      \
    if (v->length < 2)                                                         \
      return;                                                                  \
    size_t i = 0, j = v->length - 1;                                           \
    while (i < j) {                                                            \
      T temp = v->data[i];                                                     \
      v->data[i] = v->data[j];                                                 \
      v->data[j] = temp;                                                       \
      i++;                                                                     \
      j--;                                                                     \
    }                                                                          \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_sort_##Name(                                   \
      TVEC_TYPENAME(Name) * v, int (*compar)(T const *, T const *)) {          \
    if (v->length > 1) {                                                       \
      qsort(v->data, v->length, sizeof(T),                                     \
            (int (*)(const void *, const void *))compar);                      \
    }                                                                          \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_bsearch_##Name(                    \
      const TVEC_TYPENAME(Name) *const v, const T *key,                        \
      int (*compar)(T const *, T const *)) {                                   \
    if (v->length == 0) {                                                      \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    return (T *)bsearch(key, v->data, v->length, sizeof(T),                    \
                        (int (*)(const void *, const void *))compar);          \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_lower_bound_##Name(                \
      const TVEC_TYPENAME(Name) *const v, T const *key,                        \
      int (*compar)(T const *, T const *)) {                                   \
    size_t l = 0;                                                              \
    size_t r = v->length;                                                      \
    while (l < r) {                                                            \
      size_t mid = l + (r - l) / 2;                                            \
      if (compar((T const *)&v->data[mid], key) < 0) {                         \
        l = mid + 1;                                                           \
      } else {                                                                 \
        r = mid;                                                               \
      }                                                                        \
    }                                                                          \
    if (l == v->length) {                                                      \
      return NULL;                                                             \
    }                                                                          \
    return &v->data[l];                                                        \
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
