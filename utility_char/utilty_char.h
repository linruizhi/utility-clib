

#define MEM_ALIGN(addr,alignment) ((void *)(((addr) + alignment - 1) & ~(alignment-1)))

void swap(char *a, char *b);
void reverseString(char *s, int sSize);
int htoa(const unsigned char* hex, unsigned char* asc, int hex_len );
int atoh( const unsigned char* asc,unsigned char* hex, int asc_len );
