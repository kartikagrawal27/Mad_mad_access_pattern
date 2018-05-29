/**
 * Mad Mad Access Pattern
 * CS 241 - Spring 2016
 */
#include <time.h>
#include "tree.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/types.h>
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

	int fptr = open(argv[1], O_RDONLY);
	if(fptr == -1)
	{
		openFail(argv[1]);
		exit(2);
	}

	int currword = 2;
	//char buffer[100];

	//fread(buffer, 4, 1, fptr);
	struct stat sz;
	fstat(fptr, &sz);
	size_t mySize = sz.st_size;
	void *curr_addr = mmap(0, mySize, PROT_READ , MAP_PRIVATE, fptr, 0);
	

	if(strncmp("BTRE", curr_addr, 4)!=0)
	{
		formatFail(argv[1]);
		exit(2);
	}
	//fseek(fptr, 4, SEEK_SET);
	void * s = curr_addr;
	curr_addr += 4;
	//int lengthofword=0;
	double startTime = getTime();
	while(1)
	{
		if(currword > argc-1)
			break;
		
		void *temp = (void*)curr_addr + 16;

		char *impword = (char*) temp;

  		BinaryTreeNode * root = (BinaryTreeNode *) curr_addr;
		if(strcmp(impword, argv[currword])==0)
		{
			printFound(impword, root->count, root->price);
			curr_addr = s + 4;
			currword++;
			continue;
		}
		if(strcmp(argv[currword], impword)<0)
		{
			if(root->left_child)
			{
				curr_addr = s + root->left_child;
				continue;
			}
			else
			{
				printNotFound(argv[currword]);
				curr_addr = s + 4;
				currword++;
				continue;
			}
		}
		if(strcmp(argv[currword], impword)>0)
		{
			if(root->right_child)
			{
				curr_addr  = s + root->right_child;
				continue;
			}
			else
			{
				printNotFound(argv[currword]);
				curr_addr = s + 4;
				currword++;
				continue;
			}
		}
	}	
	close(fptr);
	munmap(s, mySize);
	printTime(getTime() - startTime);
	return 0; 
}
