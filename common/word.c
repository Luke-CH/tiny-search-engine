/* 
 * word.c - CS50 'word' module
 *
 * see word.h for more information.
 *
 * Crawford Hovis, CS50 Winter 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <../libcs50/mem.h>

/**************** normalizeWord() ****************/
/* 
 * convert string to lowercase
 * see word.h for more info
 */
void normalizeWord(char* word)
{
  mem_assert(word, "word parameter is NULL in normalizeWord in word.c\n");
  // lowercase each letter in the word
  for (int i = 0; i < strlen(word); i++) {
    word[i] = tolower(word[i]);
  }
}