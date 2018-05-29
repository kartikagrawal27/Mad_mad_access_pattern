/**
 * Mad Mad Access Pattern
 * CS 241 - Spring 2016
 */
#include <time.h>
#include "tree.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/*
  Look up a few nodes in the tree and print the info they contain.
  This version uses fseek() and fread() to access the data.

  ./lookup1 <data_file> <word> [<word> ...]
*/

double getTime() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return t.tv_sec + 1e-9 * t.tv_nsec;
}

int main(int argc, char **argv) 
{

	if(argc<=2)
	{
		printf("\n  lookup1 <data_file> <word> [<word> ...]\n\n");
		exit(1);
	}

	FILE * fptr = fopen(argv[1], "r");
	if(fptr==NULL)
	{
		openFail(argv[1]);
		exit(2);
	}

	int currword = 2;
	char buffer[100];

	fread(buffer, 4, 1, fptr);

	if(strncmp("BTRE", buffer, 4)!=0)
	{
		formatFail(argv[1]);
		exit(2);
	}
	fseek(fptr, 4, SEEK_SET);
	long int position;
	int lengthofword=0;
	double startTime = getTime();
	while(1)
	{
		if(currword > argc-1)
			break;
		lengthofword = 0;
		position = ftell(fptr);

		fseek(fptr, sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(float), SEEK_CUR);

		while(fgetc(fptr)!='\0')
		{
			lengthofword++;
		}

		fseek(fptr, position, SEEK_SET);
		char* myline = malloc(sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(float) + lengthofword + 1);

		fread(myline, sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(float) + lengthofword + 1 , 1, fptr);
		void *temp = (void*)myline + 16;

		char *impword = (char*) temp;

  		BinaryTreeNode * root = (BinaryTreeNode *) myline;
		if(strcmp(impword, argv[currword])==0)
		{
			printFound(impword, root->count, root->price);
			fseek(fptr, 4, SEEK_SET);
			currword++;
			free(myline);
			continue;
		}
		if(strcmp(argv[currword], impword)<0)
		{
			if(root->left_child)
			{
				fseek(fptr,root->left_child ,SEEK_SET);
				free(myline);
				continue;
			}
			else
			{
				printNotFound(argv[currword]);
				fseek(fptr, 4, SEEK_SET);
				currword++;
				free(myline);
				continue;
			}
		}
		if(strcmp(argv[currword], impword)>0)
		{
			if(root->right_child)
			{
				fseek(fptr, root->right_child, SEEK_SET);
				free(myline);
				continue;
			}
			else
			{
				printNotFound(argv[currword]);
				fseek(fptr, 4, SEEK_SET);
				currword++;
				free(myline);
				continue;
			}
		}
	}	
	fclose(fptr);
	printTime(getTime() - startTime);
	return 0; 
}
