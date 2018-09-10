#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


void parse(int argc, char** argv){

}
#define STACK_SIZE 512
#define RAM_SIZE 0x1000
typedef struct statement statement;
typedef struct loop{
	statement *block;
	statement *end;
	int number;
} loop;

typedef struct statement {
	char type;	/*valid valuse +>,.[]ne
						n=nop e=EOF*/
	union{
		uint16_t size;		//for +>

		int printed_chr;	//for . (print); EOF for unknown
		loop* l;			//for [
		struct statement* start;//for ]
		//null for everything else
	};
	struct statement *next;	//next statement
} statement;

int get_number(){
	static int num=0;
	return (++num);
}

void terminate(char* str,int status){
	perror(str);
	exit(status);
		//NTS- add memory freeing with atexit()
}
void* alloc (size_t size){
	void *temp=malloc(size);
	if (temp==NULL)
		terminate ("can't allocate memory",1);
	return temp;
}

uint8_t count_adds(FILE* in){
	char temp=fgetc(in);
	uint8_t count=0;
	do {
		count+=(temp=='+'?1:-1);
		temp=fgetc(in);
	} while(temp=='+' || temp=='-');
	ungetc(temp,in);
	return count;
}
uint16_t count_movs(FILE* in){
	char temp=fgetc(in);
	uint16_t count=0;
	do {
		count+=(temp=='>'?1:-1);
		temp=fgetc(in);
		count%=RAM_SIZE;
	} while(temp=='<' || temp=='>');
	ungetc(temp,in);
	return count;
}

static statement *make_node(FILE* in){
	static statement *stack[STACK_SIZE]={0}, **sp=stack;
	statement *node= alloc(sizeof (statement));
	int temp;
	do		temp=fgetc(in);
	while (temp!=EOF&&((strchr("+-><[],.",(char)temp)==NULL)||temp=='\0'));
	node->type=temp;
	if (temp==EOF)
		node->type='e';

	node->next = NULL;

	switch(node->type){
		case '+':	case '-':
			ungetc(node->type, in);
			node->type='+';
			node->size=count_adds(in);
			break;
		case '>':	case '<':
			ungetc(node->type, in);
			node->type='>';
			node->size=count_movs(in);	//generalize with modulu!
			break;
		case '.':
			node->printed_chr=EOF;	break;
		case '[':
			node->l=alloc(sizeof(loop));
			node->l->block=node->l->end=NULL;
			node->l->number=get_number();
			*(++sp)=node;
			break;
		case ']':
			node->start=*(sp--);
			node->start->l->end=node;
			node->start->l->block=node->start->next;
			break;

		//case 'e':	case ',':
		default:
			node->l=NULL;
	}

	return node;
}
static void print_node(statement* node,FILE* out){
	fprintf(out,"%c",node->type);
	switch(node->type){
		case '+':	case '>':
			fprintf(out, "%d\t",node->size);	break;
		case '.':
			if(node->printed_chr!=EOF)
				fprintf(out , "%c\t",((char)node->printed_chr));
			break;
		case '[':
			fprintf(out, "%d\t",node->l->number);	break;
		case ']':
			fprintf(out, "%d\t",node->start->l->number);	break;
	}
}

static statement *make_list(FILE* in){
	statement *list_head;
	statement *current;

	list_head=current=make_node(in);

	while(current->type!='e'){
		current->next=make_node(in);
		current=current->next;
	}

	return list_head;
}
static void print_list(statement* node){
	print_node(node,stdout);
	if(node->next != NULL)
		print_list(node->next);
}

statement *make_tree(FILE* in){
	//root points to a list constructed by make_list
	statement* root=make_list(in);
#ifdef DEBUG
	print_list(root);
	printf("\n\n\n");
#endif

	statement *current=root;

	do{
		switch(current->type){
			case '[':	current->next=current->l->end->next;
						current=current->l->block;
						break;
			default:	current=current->next;
		}
	}while (current->type!='e');
	return root;
}

void print_tree(statement* root, const int depth, FILE* out){
	statement* current=root;
	for(current=root;current->type!=']' && current->type!='e';current=current->next){
		for(int i=0;i<depth;i++)	fprintf(out,"\t");
		print_node(current,out);	fprintf(out,"\n");
		if (current->type == '[')
			print_tree(current->l->block,depth+1,out);
	}
	for(int i=0;i<depth;i++)	fprintf(out,"\t");
	print_node(current,out);	fprintf(out,"\n");
}

static void free_node(statement* node){
	if (node->type=='[')
		free(node->l);
	free(node);
}
void free_tree(statement* root){
	if (root==NULL)	return;
	if (root->type=='[')
		root->next=root->l->block;
	free_tree(root->next);
	free_node(root);
}
int main(int argc, char** argv){
	FILE *in, *out;
	{//parse the args
		if (argc!=3){
			parse(argc,argv);
		}
		in=fopen("input","r");
		//in=fopen(argv[1],"r");
		if (in==NULL){
			perror("cant open input file");
			return 1;
		}
//		out=fopen(argv[2],"w");
//		out=fopen("output","w");
		out=stdout;
		if (out==NULL){
			perror("cant open output file");
			return 2;
	}	}
	fprintf(out,"\n\n\n");
	statement *root=make_tree(in);
	print_tree(root,0,out);
	free_tree(root);
	fclose(out);
	fclose(in);
	fprintf(out,"\n");
	return 0;
}
