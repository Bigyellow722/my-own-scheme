#include <assert.h>
#include "log.h"
#include "memory.h"
#include "darray.h"

void darray_iterate(void* array, void (*func)(void* param))
{
  size_t len = darray_len(array);
  size_t stride = darray_stride(array);
  for (size_t i = 0; i < len; i++) {
    func((void*)((char*)array + stride * i));
  }
}

/*
 * output:
 *  @array_ptr: return a pointer to the allocated array. On error, return NULL. 
 * input:
 *  @capacity:
 *  @stride: the size of item
 */
void* _darray_create(size_t capacity, size_t stride, int factor)
{
  assert(capacity != 0 && factor != 0);
  size_t header_size = sizeof(struct dynamic_array_header);
  size_t array_size = capacity * stride;
  void* new_array = 0;
  struct dynamic_array_header* header = NULL;

  new_array = xmalloc(header_size + array_size);
  if (!new_array) {
	  err("%s, malloc size:%ld line:%d\n", __func__, header_size + array_size, __LINE__);
    return NULL;
  }
  xmemset(new_array, 0, header_size + array_size);
  header = new_array;
  header->capacity = capacity;
  header->stride = stride;
  header->count = 0;
  header->factor = factor;
  return (void *)((char *)new_array + header_size);
}

void darray_destroy(void* array)
{
  if (array) {
    size_t header_size = sizeof(struct dynamic_array_header);
    struct dynamic_array_header* header = (struct dynamic_array_header*)((char *)array - header_size);
    xfree(header);
  }
}

/* output:
   @new array_ptr: return a pointer to the pointer to new resized array, On error, exit
 * input:
 * @array_ptr: return a pointer to the pointer to array that needs to be resized. 
 */
static void* darray_resize(void* array)
{
  size_t header_size = sizeof(struct dynamic_array_header);
  struct dynamic_array_header* header = (struct dynamic_array_header*)((char *)array - header_size);

  void* new_array = _darray_create(header->capacity * header->factor, header->stride, header->factor);
  if (!new_array) {
    err("%s, no enough memory\n", __func__);
    exit(1);
  }

  xmemmove(new_array, array, header->count * header->stride);
  
  struct dynamic_array_header* new_header = (struct dynamic_array_header*)((char *)new_array - header_size);
  new_header->count = header->count;
  darray_destroy(array);
  return new_array;
}

/* output:
 * @new array ptr: return a pointer to the new array after append a new item pointed by @value_ptr. On error, exit
 */
void* _darray_push(void* array, const void* value_ptr)
{
  size_t header_size = sizeof(struct dynamic_array_header);
  struct dynamic_array_header* header = (struct dynamic_array_header*)((char *)array - header_size);
  if (header->count >= header->capacity) {
    array = darray_resize(array);
    header = (struct dynamic_array_header*)((char *)array - header_size);
  }

  void* new_value = (void*)((char*)array + header->count * header->stride);
  xmemmove(new_value, value_ptr, header->stride);
  header->count++;
  return array;
}

void* _darray_insert_at(void* array, size_t index, const void* value_ptr)
{
  size_t len = darray_len(array);
  size_t stride = darray_stride(array);
  size_t header_size = sizeof(struct dynamic_array_header);
  struct dynamic_array_header* header = (struct dynamic_array_header*)((char *)array - header_size);
  if (header->count >= header->capacity) {
    array = darray_resize(array);
    header = (struct dynamic_array_header*)((char *)array - header_size);
  }

  void* tmp = (void*)((char*)array + index * stride);
  void* tmp_next = (void*)((char*)array + (index + 1) * stride);
  xmemmove(tmp_next, tmp, len - index);
  xmemmove(tmp, value_ptr, stride);
  header->count++;
  return array;
}

void darray_pop(void* array, void* dest)
{
  size_t header_size = sizeof(struct dynamic_array_header);
  struct dynamic_array_header* header = (struct dynamic_array_header*)((char *)array - header_size);
  if (header->count < 1) {
    err("%s, the darray is empty\n", __func__);
    return;
  }

  if (dest) {
    xmemmove(dest, (void*)((char*)array + (header->count - 1) * header->stride), header->stride);
  }
  header->count--;
}


void* darray_pop_at(void* array, size_t index, void* dest)
{
  size_t len = darray_len(array);
  size_t stride = darray_stride(array);
  if (index >= len) {
    err("%s, index outside the bounds of this array. length: %zu, index: %zu\n", __func__, len, stride);
    return array;
  }
  void* value_ptr = (void*)((char*)array + index * stride);
  if (dest) {
    xmemmove(dest, value_ptr, stride);
  }

  void* next_value_ptr = (void*)((char*)value_ptr + stride);
  if (index != len - 1) {
    xmemmove(value_ptr, next_value_ptr, stride * (len - index - 1));
  }

  size_t header_size = sizeof(struct dynamic_array_header);
  struct dynamic_array_header* header = (struct dynamic_array_header*)((char *)array - header_size);
  header->count--;
  return array;
}
