#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "log.h"
#include "darray.h"

#define BUF_SIZE_MAX 1024

enum token_type {
	BOOLEAN,
	NUM,
	CHARACTER,
	STRING,
	SYMBOL,
	INVAILD_TOKEN
};

struct token {
	enum token_type type;
	union {
		bool boolean;
		int num;
		char c;
		char* str;
		char* sym;
	};
};

typedef struct vm {
	char* input_buf;
	char* token_buf;
	size_t* token_idx;
} vm_t;

vm_t* vm_create()
{
	vm_t* new = malloc(sizeof(vm_t));
	new->input_buf = NULL;
	new->token_buf = darray_create(char);
	new->token_idx = darray_create(size_t);
	return new;
}

void vm_destroy(vm_t** v)
{
	if (*v != NULL) {
		free((*v)->input_buf);
		darray_destroy((*v)->token_buf);
		darray_destroy((*v)->token_idx);
		free(*v);
		*v = NULL;
	}
}

char* str_add(char* dest, char* src, int src_len)
{
	printf("len: %d src: %.*s\n", src_len, src_len, src);
	if (src_len) {
		for (int i = 0; i < src_len; i++) {
			darray_push(dest, src[i]);
		}
		darray_push(dest, '\0');
	}
	return dest;
}

void vm_add_token(vm_t* v, char* start, int len)
{
	if (len) {
		darray_push(v->token_idx, darray_len(v->token_buf));
		v->token_buf = str_add(v->token_buf, start, len);
	}
}

void vm_scan_token(vm_t* v)
{
	size_t len = darray_len(v->token_idx);
	printf("token idx buffer len: %lu\n", len);
	for (size_t i = 0; i < len; i++) {
		printf("+++++token %s\n", &v->token_buf[v->token_idx[i]]);
	}
}

int process_input(vm_t* v, char* buffer, ssize_t len)
{
	int ret = 0;
	ssize_t count = 0;
	char* start = buffer;

	printf("input: %s, len: %ld\n", buffer, len);
	while(count < len) {
		switch (buffer[count]) {
		case ' ':
			if (&buffer[count] - start != 0) {
				printf("len: %ld\n", &buffer[count] - start);
				printf("token: %s\n", start);
				vm_add_token(v, start, &buffer[count] - start);
			}
			start = &buffer[count + 1];
			count++;
			break;
		case '(':
			vm_add_token(v, &buffer[count], 1);
			start = &buffer[count + 1];
			count++;
			break;
		case ')':
			vm_add_token(v, start, &buffer[count] - start);
			vm_add_token(v, &buffer[count], 1);
			start = &buffer[count + 1];
			count++;
			break;
		default:
			count++;
		}
	}
	if (*start != '\0') {
		printf("the last one len: %ld\n", len - (start - buffer));
		printf("token: %s\n", start);
		vm_add_token(v, start, &buffer[len] - start + 1);
	}
	return ret;
}

int main(void)
{
	char* buffer = NULL;
	size_t len;
	ssize_t nread;
	vm_t* g_vm;
	printf("Hello, world\n");
	g_vm = vm_create();

	while (1) {
		printf("scheme> ");
		nread = getline(&buffer, &len, stdin);
		if (nread == -1) {
			continue;
		}
		/* the return buf of getline include the enter */
		buffer[nread - 1] = '\0';
		process_input(g_vm, buffer, nread - 1);
		vm_scan_token(g_vm);
	}
	vm_destroy(&g_vm);
	return 0;
}
