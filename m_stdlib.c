#ifndef M_STDLIB
#define M_STDLIB

void terminate(char* str,int status){
	perror(str);
	exit(status);
}
void* alloc (size_t size){
	void *temp=malloc(size);
	if (temp==NULL)
	terminate ("can't allocate memory",1);
	return temp;
}
#endif
