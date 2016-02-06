/*
*    deglupta, Copyright (c) 2015 Dennis Goodlett <dennis@hurricanelabs.com>
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include "main.h"

int main(int argc, char *argv[]){
  FILE *ifp = NULL;
  int c;
  OUTPUT = stdout;
  RECOVER = NULL;
  TOTAL_OUT = 0;
  MAX_OUTPUT = 0x0100000000; // 4294967296 or about 4G

  // set up signal handling
  struct sigaction new_action, old_action;
  void **nxt_f = NULL;

  /* Set up the structure to specify the new action. */
  new_action.sa_handler = die_nicely;
  sigemptyset (&new_action.sa_mask);
  new_action.sa_flags = 0;

  sigaction (SIGINT, NULL, &old_action);
  if (old_action.sa_handler != SIG_IGN)
    sigaction (SIGINT, &new_action, NULL);
  sigaction (SIGHUP, NULL, &old_action);


  if ( argc == 1 ) help_menu();
  while ((c = getopt (argc, argv, "hf:m:r:i:o:")) != -1){
    switch (c) {
      case 'h':
        help_menu();
        break;
      case 'o':
        if (( OUTPUT = fopen(optarg, "a+") ) == NULL)
          error("could not open out file");
        fseek(OUTPUT, 0, SEEK_END);
        TOTAL_OUT = ftell(OUTPUT);
        rewind(OUTPUT);
        printf("file size: %llu", TOTAL_OUT);
        break;
      case 'i':
        if (( ifp = fopen(optarg, "r") ) == NULL)
          error("could not open in file");
        break;
      case 'r':  // resume option
        RECOVER = optarg;
        break;
      case 'm':  // max output bytes
        if (( MAX_OUTPUT = atoll(optarg) ) == 0)
          error("-m flag invalid\n");
        break;
      case 'f':  // max output bytes
        nxt_f = get_da_func( optarg );
        if ( nxt_f == NULL )
          error("no fun lists...\n");
        break;
      case '?':
        if (optopt == 'o' || optopt == 'i' || optopt == 'r' || optopt == 'm' || optopt == 'f' ){
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
          help_menu();
        } else{
          fprintf (stderr, "You swoopsed a command line parameter\n");
          help_menu();
        }
      default:
        abort();
    }
  }
  if ( ifp == NULL ) error("need input file.");

  // function list
  if ( nxt_f == NULL ){
    nxt_f = get_da_func( "3,1,2,4,6,6,5" );
  }


  // make first function pointer into a 
  // callable function "func"
  int (*func)(PARAMS) = nxt_f[0];

  // run function on each line of input file
  char pass[MAX_PASS_LEN];
  size_t len;
  while (fgets(pass, MAX_PASS_LEN, ifp)) {
      len = strlen(pass);
      if (len && (pass[len - 1] != '\n')) {
        error("pass was too long");
      }
      pass[len-1] = 0x00;
      /* printf("pass: \"%s\"\n", pass); */
      func(pass, 0, &nxt_f[1] );
  }
  if ( OUTPUT != stdout ){
    fclose(OUTPUT);
    printf("\n");
  }
  fclose(ifp);

  return 0;
}

void output(PARAMS){
  if ( RECOVER != NULL ){
    if (strcmp(RECOVER, pass) == 0)
      RECOVER=NULL;
    return ;
  }
  if (OUTPUT == stdout){
    TOTAL_OUT += fprintf(OUTPUT, "%s\n", pass);
  } else{
    if ( TOTAL_OUT == 0 ) printf("\n");
    TOTAL_OUT += fprintf(OUTPUT, "%s\n", pass);
    fflush(stdout);
    printf("\rFile size: %10llu bytes\r\r", TOTAL_OUT);
  }
  if ( DIE ){
    fprintf(stderr, "CTRL-C handled: \n"
        "\tMAX_OUTPUT: %llu bytes\n"
        "\toutput currently: %llu bytes\n"
        "\tresume with: -r %s\n",
        MAX_OUTPUT,
        TOTAL_OUT,
        pass
    );
    exit(1);
  }

  if ( TOTAL_OUT >= MAX_OUTPUT ){
    fprintf(stderr, "MAX_OUTPUT exceded\n"
        "\tMAX_OUTPUT: %llu bytes\n"
        "\toutput currently: %llu bytes\n"
        "\tresume with: -r %s\n"
        "Change the -m paramater to get larger outputs\n",
        MAX_OUTPUT,
        TOTAL_OUT,
        pass
    );
    exit(1);
  } 
}

void space_replace(PARAMS){
  // N means replace with nothing!!!
  char *rplc = "_-.N";  // replace with each character of this string
  static int lrplc = 0;
  if (lrplc == 0) lrplc = strlen(rplc);
  int lpass = strlen(pass);
  void (*func)(PARAMS) = *nxt_f;

  // send it unmollested
  func(pass, 0, nxt_f+1);

  // room to work
  char holder[lpass+1];

  int si, pi, hpos; // space and pass index

  for (si=0; si<lrplc; si++){
    if ( rplc[si] != 0x4e ){
      strcpy(holder, pass);
      for (pi=0; pi<lpass; pi++)
        if ( pass[pi] == 0x20 ) holder[pi] = rplc[si];
    }else{
      hpos = 0;
      for (pi=0; pi<lpass; pi++){
        if ( pass[pi] != 0x20 )
          holder[hpos++] = pass[pi];
        if ( pi == lpass-1 ) holder[hpos++] = 0x00;
      }
    }
    // check there was a space somewhere
    if ( si==0 && strcmp( holder, pass ) == 0 ) return;
    func(holder, 0, nxt_f+1);
  }

}
void num_ending(PARAMS){
  // append 0-9
  // do twice to get 00-99, ect
  int lpass = strlen(pass);
  char pass_new[lpass + 2]; // room for null
  pass_new[lpass] = 0x00;
  pass_new[lpass+1] = 0x00;
  void (*func)(PARAMS) = *nxt_f;

  // send it unmollested
  func(pass, 0, nxt_f+1);

  strcpy(pass_new, pass);
  int i;
  for (i=0; i<10; i++){
    // 0->0x30 9->0x39
    // careful to only move one byte and not overwrite 
    // something with the 4bytes in an int
    // 0x004014e4    880c02       mov [rdx+rax], cl
    pass_new[lpass] = (unsigned char )( i | 0x30 ); 
    func(pass_new, 0, nxt_f+1);
  }
}
void punct_ending(PARAMS){
  // MUST be NULL terminated
  char *endings[] = { "!", "!!", "!!!", "@", "@@", "#", "##", NULL };
  static int punct_len = 0, punct_max_add = 0;
  if ( punct_len == 0 || punct_max_add == 0 )
    init_bounds(endings, &punct_len, &punct_max_add);

  int lpass = strlen(pass);
  char pass_new[lpass + punct_max_add + 1];
  void (*func)(PARAMS) = *nxt_f;

  // send it unmollested
  func(pass, 0, nxt_f+1);

  strcpy(pass_new, pass);
  int i;
  for (i=0; i<punct_len; i++){
    strncpy(pass_new + lpass, endings[i], punct_max_add+1);
    func(pass_new, 0, nxt_f+1);
  }
}

void prepend_word(PARAMS){
  // MUST be NULL terminated
  char *words[] = { "Welcome ", NULL };
  static int num_words = 0, longest_word = 0;
  if (num_words == 0 || longest_word == 0)
    init_bounds(words, &num_words, &longest_word);

  int lpass = strlen(pass);
  char pass_new[lpass + longest_word + 1];
  strcpy(pass_new, pass);

  void (*func)(PARAMS) = *nxt_f;

  // send it unmollested
  func(pass, 0, nxt_f+1);

  int i;
  for (i=0; i<num_words; i++){
    sprintf(pass_new, "%s%s", words[i], pass);
    func(pass_new, 0, nxt_f+1);
  }

}

void l33t(PARAMS){
  static char *normalc = "AEOTI";
  static char *leetc   = "@3071";
  static int len = 0;
  static int lpass = 0;
  if ( len ==0 ){  // same for all possible pass values
    len = strlen(normalc);
    if ( strlen(leetc) != len ) error("l337: Lengths of normalc and leetc differ");
  }
  // changes every new pass
  if ( cnt == 0 ) lpass = strlen(pass); 

  void (*func)(PARAMS) = *nxt_f;


  int cl;
  for ( ; cnt<lpass; cnt++ ){  //loop through pass
    for (cl=0; cl<len; cl++){  // loop through keys
      if ( pass[cnt] == normalc[cl] ){
        char holder[lpass+1];
        strcpy(holder, pass);
        holder[cnt] = leetc[cl];
        if ( cnt != lpass ){
          l33t(pass  , cnt+1,   nxt_f); // send it UNmollested
          l33t(holder, cnt+1,   nxt_f); // send it   mollested
        }else{
          func(pass  ,     0, nxt_f+1); // send it UNmollested
          func(holder,     0, nxt_f+1); // send it   mollested
        }
        return;
      }
    }
  }
  func(pass  ,     0, nxt_f+1);

}
void ucase_flip(PARAMS){
  static int lpass = 0;
  if ( cnt == 0 ) lpass = strlen(pass); 
  void (*func)(PARAMS) = *nxt_f;


  for ( ; cnt<lpass; cnt++ ){  //loop through pass
    if ( pass[cnt] >= 0x41 && pass[cnt] <= 0x5A ){
      char holder[lpass+1];
      strcpy(holder, pass);
      holder[cnt] = pass[cnt] + 0x20;
      if ( cnt != lpass ){
        ucase_flip(pass  , cnt+1,   nxt_f); // send it UNmollested
        ucase_flip(holder, cnt+1,   nxt_f); // send it   mollested
      }else{
        func(pass  ,     0, nxt_f+1); // send it UNmollested
        func(holder,     0, nxt_f+1); // send it   mollested
      }
      return;
    }
  }
  func(pass  ,     0, nxt_f+1);

}
