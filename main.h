#define PARAMS char *pass, int cnt, void **nxt_f
#define LIST_MAX 100  // biggest replace list
#define MAX_PASS_LEN 1024

unsigned long long MAX_OUTPUT;
FILE *OUTPUT;
char *RECOVER;
unsigned long long TOTAL_OUT;

void error(char *msg);
void space_replace(PARAMS);
void punct_ending(PARAMS);
void l33t(PARAMS);
void ucase_flip(PARAMS);
void output(PARAMS);


void init_bounds(char *list[], int *ptr_l, int *ptr_max){
  int max = 0; 
  int l = 0;  

  int relmax=0;
  while ( list[l] != NULL){
    relmax = strlen(list[l]);
    if ( relmax > max ) max = relmax;
    l++;
    if (l >= LIST_MAX) error("init_bounds: someone did not NULL terminate the list...");
  }
  *ptr_l = l;
  *ptr_max = max;
}

void help_menu(){
  printf(
    "\t-h:            help menu\n"
    "\t-i <file>:    input file (required)\n"
    "\t-o <file>:    output file to append to (default: stdout)\n"
    "\t-r <pass>:    resume output after \"pass\"\n"
    "\t-m <num>:     aprox max output size (default: 4294967296 ie: 4G)\n"
    "\t                  Will be off by at most the length of the current password\n"
  );
  exit(1);
}
