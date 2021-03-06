/******************************************************************************
** Student name: 	Quang Dao
** Student number: 	S3687103
** Course: 			Advanced Programming Techniques - S2 2018
******************************************************************************/

#include "invector.h"

/* Create a new InVector Table */
void invector_init(InVTable *invt)
{
  invt -> tot = invt -> width = 0;
}

/* Insert vector to table */
Boolean insertIntoInvector(InVTable * invt, InVector vector)
{
  int i;

  if(invt->tot == INVT_MAX)
  {
    return FALSE;
  }

  for (i = 0; i < INVT_WIDTH; i++)
  {
    invt->table [invt->tot][i] = vector[i];
  }
  invt->tot++;

  return TRUE;
}

/* Read output file into table */
Boolean readInvector(InVTable * invt, FILE * fp)
{
  char line[256];

  while(fgets(line, sizeof(line), fp) != NULL)
  {
    InVector vector;

    int i;

    if(line[strlen(line) -1] == '\n')
    {
      line[strlen(line) -1] = '\0';

      strtok(line, INV_DELIM2);
      i = 0;
      while(TRUE)
      {
        char * token = strtok(NULL, INV_DELIM3);
        if (token == NULL)
        {
          break;
        }
        vector[i++] = strtol(token, NULL, 10);
      }
      invt->width = i;
      if(!insertIntoInvector(invt, vector))
      {
        return FALSE;
      }
    }
  }
  return TRUE;
}

/* Function to print out table */
void printInvector(InVTable * invt)
{
  int i;
  for (i = 0; i < invt->tot; i++)
  {
    int j;
    for (j=0; j < invt->width; j++)
    {
      printf("%d ", invt->table[i][j]);
    }
    printf("\n");
  }
}
