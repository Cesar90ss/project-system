#ifndef     _MEM_ALLOC_H_
#define     _MEM_ALLOC_H_


/* Allocator functions, to be implemented in mem_alloc.c */
void memory_init(void);
char *memory_alloc(int size);
void memory_free(char *p);

void *malloc(size_t size);
void free(void *p);
void *realloc(void *ptr, size_t size);
void *calloc(size_t nmemb, size_t size);

#endif      /* !_MEM_ALLOC_H_ */
