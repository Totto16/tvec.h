/*
 * tvec.h
 * based on
 * https://github.com/z-libs/zvec.h/blob/5ebd8ac1d1ee66e14210037f6938575901f9d17e/zvec.h
 *
 * modified to suit my needs
 *
 * By: Zuhaitz-dev
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

// Compiler Extensions (Optional).
// We check for GCC/Clang features to enable RAII-style cleanup.
// Define T_NO_EXTENSIONS to disable this manually.
#if !defined(T_NO_EXTENSIONS) && (defined(__GNUC__) || defined(__clang__))
#define T_HAS_CLEANUP 1
#define T_CLEANUP(func) __attribute__((cleanup(func)))
#else
#define T_HAS_CLEANUP 0
#define T_CLEANUP(func)
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
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult tvec_reserve_##Name(            \
      TVEC_TYPENAME(Name) * v, size_t new_cap);                                \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TvecResult                                 \
  tvec_allocate_uninitialized_##Name(TVEC_TYPENAME(Name) * v,                  \
                                     size_t new_length);                       \
                                                                               \
  TVEC_FUN_ATTRIBUTES                                                          \
      [[nodiscard]] T *tvec_push_slot_##Name(TVEC_TYPENAME(Name) * v);         \
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
      [[nodiscard]] T tvec_pop_get_##Name(TVEC_TYPENAME(Name) * v);            \
                                                                               \
  TVEC_FUN_ATTRIBUTES void tvec_shrink_to_fit_##Name(TVEC_TYPENAME(Name) * v); \
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
      [[nodiscard]] T *tvec_data_##Name(TVEC_TYPENAME(Name) * v);              \
                                                                               \
  TVEC_FUN_ATTRIBUTES                                                          \
      [[nodiscard]] T *tvec_last_##Name(TVEC_TYPENAME(Name) * v);              \
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
#define TVEC_IS_EMPTY(v) ((v).length == 0)

#define TVEC_ASSERT_SHOULD_USE_PUSH(val)                                       \
  STATIC_ASSERT(sizeof(val) <= 8, "only small values should use push, use "    \
                                  "push slot for larger ones instead!")

#define TVEC_ASSERT_SHOULD_USE_PUSH_SLOT(val)                                  \
  STATIC_ASSERT(sizeof(val) > 8, "only big values should use push slot, use "  \
                                 "push for smaller ones instead!")

#define TVEC_PUSH(T, v, val) TVEC_PUSH_EXTENDED(T, T, v, val)
#define TVEC_PUSH_SLOT(T, v) TVEC_PUSH_SLOT_EXTENDED(T, T, v)
#define TVEC_EXTEND(T, v, arr, count) TVEC_EXTEND_EXTENDED(T, T, v, arr, count)
#define TVEC_RESERVE(T, v, cap) TVEC_RESERVE_EXTENDED(T, T, v, cap)
#define TVEC_ALLOCATE_UNINITIALIZED(T, v, length)                              \
  TVEC_ALLOCATE_UNINITIALIZED_EXTENDED(T, T, v, length)
#define TVEC_AT(T, v, idx) TVEC_AT_EXTENDED(T, T, v, idx)
#define TVEC_GET_AT(T, v, idx) TVEC_GET_AT_EXTENDED(T, T, v, idx)
#define TVEC_GET_AT_MUT(T, v, idx) TVEC_GET_AT_MUT_EXTENDED(T, T, v, idx)
#define TVEC_SET_AT(T, v, idx, value) TVEC_SET_AT_EXTENDED(T, T, v, idx, value)
#define TVEC_DATA(T, v) TVEC_DATA_EXTENDED(T, T, v)
#define TVEC_LAST(T, v) TVEC_LAST_EXTENDED(T, T, v)
#define TVEC_FREE(T, v) TVEC_FREE_EXTENDED(T, T, v)
#define TVEC_POP(T, v) TVEC_POP_EXTENDED(T, T, v)
#define TVEC_POP_GET(T, v) TVEC_POP_GET_EXTENDED(T, T, v)
#define TVEC_SHRINK_TO_FIT(T, v) TVEC_SHRINK_TO_FIT_EXTENDED(T, T, v)
#define TVEC_REMOVE(T, v, i) TVEC_REMOVE_EXTENDED(T, T, v, i)
#define TVEC_SWAP_REMOVE(T, v, i) TVEC_SWAP_REMOVE_EXTENDED(T, T, v, i)
#define TVEC_CLEAR(T, v) TVEC_CLEAR_EXTENDED(T, T, v)
#define TVEC_REVERSE(T, v) TVEC_REVERSE_EXTENDED(T, T, v)
#define TVEC_SORT(T, v, cmp) TVEC_SORT_EXTENDED(T, T, v, cmp)
#define TVEC_BSEARCH(T, v, key, cmp) TVEC_BSEARCH_EXTENDED(T, T, v, key, cmp)
#define TVEC_LOWER_BOUND(T, v, key, cmp)                                       \
  TVEC_LOWER_BOUND_EXTENDED(T, T, v, key, cmp)
#define TVEC_FROM(T, arr, size) TVEC_FROM_EXTENDED(T, T, arr, size)

#define TVEC_PUSH_EXTENDED(T, Name, v, val) tvec_push_##Name(v, val)
#define TVEC_PUSH_SLOT_EXTENDED(T, Name, v) tvec_push_slot_##Name(v)
#define TVEC_EXTEND_EXTENDED(T, Name, v, arr, count)                           \
  tvec_extend_##Name(v, arr, count)
#define TVEC_RESERVE_EXTENDED(T, Name, v, cap) tvec_reserve_##Name(v, cap)
#define TVEC_ALLOCATE_UNINITIALIZED_EXTENDED(T, Name, v, length)               \
  tvec_allocate_uninitialized_##Name(v, length)
#define TVEC_AT_EXTENDED(T, Name, v, idx) tvec_at_##Name(v, idx)
#define TVEC_GET_AT_EXTENDED(T, Name, v, idx) tvec_get_at_##Name(v, idx)
#define TVEC_GET_AT_MUT_EXTENDED(T, Name, v, idx) tvec_get_at_mut_##Name(v, idx)
#define TVEC_SET_AT_EXTENDED(T, Name, v, idx, value)                           \
  tvec_set_at_##Name(v, idx, value)
#define TVEC_DATA_EXTENDED(T, Name, v) tvec_data_##Name(v)
#define TVEC_LAST_EXTENDED(T, Name, v) tvec_last_##Name(v)
#define TVEC_FREE_EXTENDED(T, Name, v) tvec_free_##Name(v)
#define TVEC_POP_EXTENDED(T, Name, v) tvec_pop_##Name(v)
#define TVEC_POP_GET_EXTENDED(T, Name, v) tvec_pop_get_##Name(v)
#define TVEC_SHRINK_TO_FIT_EXTENDED(T, Name, v) tvec_shrink_to_fit_##Name(v)
#define TVEC_REMOVE_EXTENDED(T, Name, v, i) tvec_remove_##Name(v, i)
#define TVEC_SWAP_REMOVE_EXTENDED(T, Name, v, i) tvec_swap_remove_##Name(v, i)
#define TVEC_CLEAR_EXTENDED(T, Name, v) tvec_clear_##Name(v)
#define TVEC_REVERSE_EXTENDED(T, Name, v) tvec_reverse_##Name(v)
#define TVEC_SORT_EXTENDED(T, Name, v, cmp) tvec_sort_##Name(v, cmp)
#define TVEC_BSEARCH_EXTENDED(T, Name, v, key, cmp)                            \
  tvec_bsearch_##Name(v, key, cmp)
#define TVEC_LOWER_BOUND_EXTENDED(T, Name, v, key, cmp)                        \
  tvec_lower_bound_##Name(v, key, cmp)
#define TVEC_FROM_EXTENDED(T, Name, arr, size) tvec_from_array_##Name(arr, size)

#if !defined(T_NO_GENERIC_USAGE)

#define TVEC_PUSH_ENTRY(T, Name) TVEC_TYPENAME(Name) * : tvec_push_##Name,
#define TVEC_PUSH_SLOT_ENTRY(T, Name)                                          \
  TVEC_TYPENAME(Name) * : tvec_push_slot_##Name,
#define TVEC_EXTEND_ENTRY(T, Name) TVEC_TYPENAME(Name) * : tvec_extend_##Name,
#define TVEC_RESERVE_ENTRY(T, Name) TVEC_TYPENAME(Name) * : tvec_reserve_##Name,
#define TVEC_AT_ENTRY(T, Name) TVEC_TYPENAME(Name) * : tvec_at_##Name,
#define TVEC_GET_AT_ENTRY(T, Name) TVEC_TYPENAME(Name) * : tvec_get_at_##Name,
#define TVEC_GET_AT_MUT_ENTRY(T, Name)                                         \
  TVEC_TYPENAME(Name) * : tvec_get_at_mut_##Name,
#define TVEC_DATA_ENTRY(T, Name) TVEC_TYPENAME(Name) * : tvec_data_##Name,
#define TVEC_LAST_ENTRY(T, Name) TVEC_TYPENAME(Name) * : tvec_last_##Name,
#define TVEC_FREE_ENTRY(T, Name) TVEC_TYPENAME(Name) * : tvec_free_##Name,
#define TVEC_POP_ENTRY(T, Name) TVEC_TYPENAME(Name) * : tvec_pop_##Name,
#define TVEC_POP_GET_ENTRY(T, Name) TVEC_TYPENAME(Name) * : tvec_pop_get_##Name,
#define TVEC_SHRINK_ENTRY(T, Name)                                             \
  TVEC_TYPENAME(Name) * : tvec_shrink_to_fit_##Name,
#define TVEC_REMOVE_ENTRY(T, Name) TVEC_TYPENAME(Name) * : tvec_remove_##Name,
#define TVEC_SWAP_REM_ENTRY(T, Name)                                           \
  TVEC_TYPENAME(Name) * : tvec_swap_remove_##Name,
#define TVEC_CLEAR_ENTRY(T, Name) TVEC_TYPENAME(Name) * : tvec_clear_##Name,
#define TVEC_REVERSE_ENTRY(T, Name) TVEC_TYPENAME(Name) * : tvec_reverse_##Name,
#define TVEC_SORT_ENTRY(T, Name) TVEC_TYPENAME(Name) * : tvec_sort_##Name,
#define TVEC_BSEARCH_ENTRY(T, Name) TVEC_TYPENAME(Name) * : tvec_bsearch_##Name,
#define TVEC_LOWER_BOUND_ENTRY(T, Name)                                        \
  TVEC_TYPENAME(Name) * : tvec_lower_bound_##Name,

#define tvec_push(v, val)                                                      \
  _Generic((v), REGISTER_TYPES(TVEC_PUSH_ENTRY) default: 0)(v, val)
#define tvec_push_slot(v)                                                      \
  _Generic((v), REGISTER_TYPES(TVEC_PUSH_SLOT_ENTRY) default: (void *)0)(v)
#define tvec_extend(v, arr, count)                                             \
  _Generic((v), REGISTER_TYPES(TVEC_EXTEND_ENTRY) default: 0)(v, arr, count)
#define tvec_reserve(v, cap)                                                   \
  _Generic((v), REGISTER_TYPES(TVEC_RESERVE_ENTRY) default: 0)(v, cap)
#define tvec_at(v, idx)                                                        \
  _Generic((v), REGISTER_TYPES(TVEC_AT_ENTRY) default: (void *)0)(v, idx)
#define tvec_get_at(v, idx)                                                    \
  _Generic((v), REGISTER_TYPES(TVEC_GET_AT_ENTRY) default: (void)0)(v, idx)
#define tvec_get_at_mut(v, idx)                                                \
  _Generic((v), REGISTER_TYPES(TVEC_GET_AT_MUT_ENTRY) default: (void)0)(v, idx)
#define tvec_data(v)                                                           \
  _Generic((v), REGISTER_TYPES(TVEC_DATA_ENTRY) default: (void *)0)(v)
#define tvec_last(v)                                                           \
  _Generic((v), REGISTER_TYPES(TVEC_LAST_ENTRY) default: (void *)0)(v)
#define tvec_free(v)                                                           \
  _Generic((v), REGISTER_TYPES(TVEC_FREE_ENTRY) default: (void)0)(v)
#define tvec_pop(v)                                                            \
  _Generic((v), REGISTER_TYPES(TVEC_POP_ENTRY) default: (void)0)(v)
#define tvec_pop_get(v)                                                        \
  _Generic((v), REGISTER_TYPES(TVEC_POP_GET_ENTRY) default: (void)0)(v)
#define tvec_shrink_to_fit(v)                                                  \
  _Generic((v), REGISTER_TYPES(TVEC_SHRINK_ENTRY) default: (void)0)(v)
#define tvec_remove(v, i)                                                      \
  _Generic((v), REGISTER_TYPES(TVEC_REMOVE_ENTRY) default: (void)0)(v, i)
#define tvec_swap_remove(v, i)                                                 \
  _Generic((v), REGISTER_TYPES(TVEC_SWAP_REM_ENTRY) default: (void)0)(v, i)
#define tvec_clear(v)                                                          \
  _Generic((v), REGISTER_TYPES(TVEC_CLEAR_ENTRY) default: (void)0)(v)
#define tvec_reverse(v)                                                        \
  _Generic((v), REGISTER_TYPES(TVEC_REVERSE_ENTRY) default: (void)0)(v)
#define tvec_sort(v, cmp)                                                      \
  _Generic((v), REGISTER_TYPES(TVEC_SORT_ENTRY) default: (void)0)(v, cmp)
#define tvec_bsearch(v, key, cmp)                                              \
  _Generic((v), REGISTER_TYPES(TVEC_BSEARCH_ENTRY) default: (void *)0)(v, key, \
                                                                       cmp)
#define tvec_lower_bound(v, k, c)                                              \
  _Generic((v),                                                                \
      REGISTER_TYPES(TVEC_LOWER_BOUND_ENTRY) default: (void *)0)(v, k, c)

#endif

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

#if T_HAS_CLEANUP
#define tvec_autofree(Name) T_CLEANUP(tvec_free_##Name) vec_##Name
#endif

#define TVEC_IMPLEMENT_VEC_TYPE(T) TVEC_IMPLEMENT_VEC_TYPE_EXTENDED(T, T)

#define TVEC_IMPLEMENT_VEC_TYPE_EXTENDED(T, Name)                              \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] TVEC_TYPENAME(Name)                        \
      tvec_init_capacity_##Name(size_t cap) {                                  \
    TVEC_TYPENAME(Name) v = TVEC_EMPTY(Name);                                  \
    if (cap > 0) {                                                             \
      v.data = T_VEC_CALLOC(cap, sizeof(T));                                   \
      v.capacity = v.data ? cap : 0;                                           \
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
      size_t new_cap = v->capacity == 0 ? 8 : v->capacity * 2;                 \
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
      size_t new_cap = v->capacity == 0 ? 8 : v->capacity;                     \
      while (new_cap < v->length + count)                                      \
        new_cap *= 2;                                                          \
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
  TVEC_FUN_ATTRIBUTES void tvec_shrink_to_fit_##Name(TVEC_TYPENAME(Name) *     \
                                                     v) {                      \
    if (v->length == 0) {                                                      \
      T_VEC_FREE(v->data);                                                     \
      *v = TVEC_EMPTY(Name);                                                   \
      return;                                                                  \
    }                                                                          \
    if (v->length == v->capacity)                                              \
      return;                                                                  \
    T *new_data = T_VEC_REALLOC(v->data, v->length * sizeof(T));               \
    if (!new_data)                                                             \
      return;                                                                  \
    v->data = new_data;                                                        \
    v->capacity = v->length;                                                   \
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
      int (*qsort_cmp)(const void *, const void *) =                           \
          (int (*)(const void *, const void *))compar;                         \
      qsort(v->data, v->length, sizeof(T), qsort_cmp);                         \
    }                                                                          \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_bsearch_##Name(                    \
      const TVEC_TYPENAME(Name) *const v, const T *key,                        \
      int (*compar)(T const *, T const *)) {                                   \
    if (v->length == 0)                                                        \
      return NULL;                                                             \
    int (*bs_cmp)(const void *, const void *) =                                \
        (int (*)(const void *, const void *))compar;                           \
    return (T *)bsearch(key, v->data, v->length, sizeof(T), bs_cmp);           \
  }                                                                            \
                                                                               \
  TVEC_FUN_ATTRIBUTES [[nodiscard]] T *tvec_lower_bound_##Name(                \
      const TVEC_TYPENAME(Name) *const v, T const *key,                        \
      int (*compar)(T const *, T const *)) {                                   \
    size_t l = 0;                                                              \
    size_t r = v->length;                                                      \
    while (l < r) {                                                            \
      size_t mid = l + (r - l) / 2;                                            \
      if (compar(&v->data[mid], key) < 0) {                                    \
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
