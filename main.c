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
#include <libconfig.h>

#define PARAMS char *pass, int cnt, void **nxt_f
#define LIST_MAX 100  // biggest replace list

FILE *OUTPUT;
  
void error(char *msg);
void space_replace(PARAMS);
void punct_ending(PARAMS);
void l33t(PARAMS);
void ucase_flip(PARAMS);

void output(PARAMS){
  fprintf(OUTPUT, "%s\n", pass);
}
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

int main(int argc, char *argv[]){
  if (argc < 2 ) error("need word to mutate");
  else if (argc > 3) error("do you know what you are doing?");
  else if (argc == 3){
    if (( OUTPUT = fopen(argv[2], "a+") ) == NULL)
      error("could not open out file");
  }
  else OUTPUT = stdout;

  // function list
  void *nxt_f[] = { 
    &space_replace,
    &punct_ending, 
    &l33t, 
    &ucase_flip,
    &output 
  };

  int (*func)(PARAMS) = nxt_f[0];
  func(argv[1], 0, &nxt_f[1] );

  if ( OUTPUT != stdout ) fclose(OUTPUT);

  return 0;
}

void error(char *msg){
  if (errno) perror(msg);
  else fprintf(stderr, "%s\n", msg);
  exit(errno);
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
  for (int i=0; i<punct_len; i++){
    strncpy(pass_new + lpass, endings[i], punct_max_add+1);
    func(pass_new, 0, nxt_f+1);
  }
}
void l33t(PARAMS){
  static char *normalc = "AEOT";
  static char *leetc   = "@307";
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
