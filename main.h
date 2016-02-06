#define PARAMS char *pass, int cnt, void **nxt_f
#define LIST_MAX 100  // biggest replace list
#define MAX_PASS_LEN 1024

unsigned long long MAX_OUTPUT;
FILE *OUTPUT;
char *RECOVER;
static volatile int DIE = 0;
unsigned long long TOTAL_OUT;

void space_replace(PARAMS);
void punct_ending(PARAMS);
void prepend_word(PARAMS);
void l33t(PARAMS);
void ucase_flip(PARAMS);
void output(PARAMS);
void num_ending(PARAMS);

void error(char *msg){
  if (errno) perror(msg);
  else fprintf(stderr, "%s\n", msg);
  if (errno == 0)
    exit(1);
  exit(errno);
}


// helper to get information about an char *[]
// ARRAY MUST BE NULL TERMINATED
void init_bounds(char *list[], int *ptr_l, int *ptr_max){
  int max = 0; 
  int l = 0;  

  int relmax=0;
  while ( list[l] != NULL){
    relmax = strlen(list[l]);
    if ( relmax > max ) max = relmax;
      l++;
    if (l >= LIST_MAX) 
      error("init_bounds: someone did not NULL terminate the list...");
  }
  *ptr_l = l;
  *ptr_max = max;
}
void die_nicely(int sig){
  if ( DIE ){
    fprintf(stderr, "\n"
    "====================================\n"
    "FORCING QUIT, kill -9 if this fails\n"
    "====================================\n");
    exit(1);
  }else{
    fprintf(stderr, "\n"
    "====================================\n"
    "Ctrl+c, attempting to quit nicely...\n"
    "Ctrl+c again to quit right now!!!\n"
    "====================================\n");
    DIE = 1;
  }

}
void * get_da_func( char *fun_str){
  if ( fun_str == NULL ){
    printf(
      "1 space_replace\n"
      "2 punct_ending\n"
      "3 prepend_word\n"
      "4 l33t\n"
      "5 ucase_flip\n"
      "6 num_ending\n"
    );
    return NULL;
  }   
  int len = strlen(fun_str);
  // len("1,2,3,4,5") / 2 + 2 = 5 mutation functions, 1 output function, 1
  void **fun_list = malloc( sizeof *fun_list * ( len/2 +2 ) );
  if ( fun_list == NULL )
    error("could not get memory for function list\n");
  int i = 0;
  for ( i=0; i<len; i++ ){
    // double check the seperators for sanities
    if ( i % 2 ){
      if ( fun_str[i] == ',' )
        continue;
      else
        error("-f paramater invalid");
    }
    switch( fun_str[i] ){
      case '1':
				fun_list[i/2] = &space_replace;
        break;
      case '2':
				fun_list[i/2] = &punct_ending;
        break;
      case '3':
				fun_list[i/2] = &prepend_word;
        break;
      case '4':
				fun_list[i/2] = &l33t;
        break;
      case '5':
				fun_list[i/2] = &ucase_flip;
        break;
      case '6':
				fun_list[i/2] = &num_ending;
        break;
      default :
        free( fun_list );
        error("invalid function list\n");
        break;
    }
  }
  fun_list[i/2 + 1] = &output;
  return fun_list;
}

void help_menu(){
  printf(
    "\t-h:                 help menu\n"
    "\t-i <file>:          input file (required)\n"
    "\t-o <file>:          output file to append to (default: stdout)\n"
    "\t-r <pass>:          resume output after \"pass\"\n"
    "\t-m <num>:           aprox max output size (default: 4294967296 ie: 4G)\n"
    "\t                    Will be off by at most the length of the current password\n"
    "\t                    -m -1 results in: 18446744073709551615 byte file size limit\n"
  );
  exit(1);
}
