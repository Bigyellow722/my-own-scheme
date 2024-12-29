#ifndef _DARRAY_H_
#define _DARRAY_H_

#include <stddef.h>
#include <stdio.h>
struct dynamic_array_header {
  size_t capacity;
  size_t count;
  size_t stride;
  int factor;
};

static inline size_t darray_len(void *array)
{
  size_t header_size = sizeof(struct dynamic_array_header);
  struct dynamic_array_header* header = (struct dynamic_array_header*)((char *)array - header_size);
  return header->count;
}

static inline size_t darray_cap(void *array)
{
  size_t header_size = sizeof(struct dynamic_array_header);
  struct dynamic_array_header* header = (struct dynamic_array_header*)((char *)array - header_size);
  return header->capacity;
}

static inline size_t darray_stride(void* array)
{
  size_t header_size = sizeof(struct dynamic_array_header);
  struct dynamic_array_header* header = (struct dynamic_array_header*)((char *)array - header_size);
  return header->stride;
}

static inline void darray_dump(void* darray)
{
	printf("%s\n", __func__);
	printf("\tcaps %lu\n", darray_cap(darray));
	printf("\tstride %lu\n", darray_stride(darray));
	printf("\tlen %lu\n", darray_len(darray));
}

void darray_iterate(void* array, void (*func)(void* param));
void* _darray_create(size_t capacity, size_t stride, int factor);
void darray_destroy(void* array);
void* _darray_push(void* array, const void* value_ptr);
void* _darray_insert_at(void* array, size_t index, const void* value_ptr);
void darray_pop(void* array, void* dest);
void* darray_pop_at(void* array, size_t index, void* dest);


#define darray_create(type) \
  (type*)_darray_create(1, sizeof(type), 2)

#define darray_reserve(type, caps) \
  (type*)_darray_create(caps, sizeof(type), 2)

#define darray_push(array, value) \
  { \
    typeof(value) __tmp = value; \
    array = _darray_push(array, (const void*)&__tmp); \
  }

#define darray_insert_at(array, index, value) \
  { \
    typeof(value) __tmp = value; \
    array = _darray_insert_at(array, index, (const void*)&__tmp); \
  }

#define DEFINE_DARRAY(name, type) \
	type* name = darray_create(type)

#define DEFINE_DARRAY_RESERVE(name, type, caps) \
	type* name = darray_reserve(type, caps)
#endif
