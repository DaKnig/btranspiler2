//NTS- add memory freeing with atexit()
#define DEBUG
#define STACK_SIZE 512
#define RAM_SIZE 0x1000

#include "lexer.c"
#include "m_stdlib.c"
#include <stdio.h>
//#include <stdlib.h>
#include <stdint.h>

void parse(int argc, char** argv,FILE** in,FILE** out){
	//in=fopen(argv[1],"r");
	*in=fopen("input","r");
	if (*in==NULL)
		terminate("cant open input file",1);
	//out=fopen(argv[2],"w");
	//out=fopen("output","w");
	*out=stdout;
	if (*out==NULL)
		terminate("cant open output file",2);
}

int main(int argc, char** argv){
	FILE *in, *out;
	{//parse the args

			parse(argc,argv,&in,&out);
	}
	fprintf(out,"\n\n\n");
	statement *root=make_tree(in);
	print_tree(root,0,out);
	free_tree(root);
	fclose(out);
	fclose(in);
	fprintf(out,"\n");
	return 0;
}
