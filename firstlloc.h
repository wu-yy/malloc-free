#ifndef __FIRSTLLOC__
#define __FIRSTLLOC__ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#define SIZE 10000
#define SIZE_ALL 100000 
#define SIZE_TEST 1000 

int _MEM_SIZE = SIZE_ALL;
int MEM_SIZE=SIZE_ALL;

struct mmu{
    void* pointer_begin;
    int size;
};
typedef struct mmu mu;
mu used[SIZE];  
int used_ind;   
mu  pieces[SIZE];  
int pieces_ind;  

union firstlloc{
	
	struct{
		union firstlloc *ptr;
		uint size;
	}s;	
	long x;
};

static fisrtlloc base;
static firstlloc*freep;

void*end;
void* begin;
void* now;
int init()  
{
    begin = sbrk(MEM_SIZE);
    end = begin - MEM_SIZE;
    now = end;
    used_ind = 0;
    pieces_ind = 0;
    if(begin == (void*)-1)
        return 0;
    else
        return 1;
}

void* find(int number)
{
	int i;
    for( i=0;i<pieces_ind;i++)
    {
        if(pieces[i].size > number)
        {
            pieces[i].size -= number;
            return pieces[i].pointer_begin + pieces[i].size;
        }
        if(pieces[i].size == number)
        {
            void* aim = pieces[i].pointer_begin;
            int j;
            for(j=i;j<pieces_ind;j++)
                pieces[j] = pieces[j+1];
            pieces_ind--;
            return aim;
        }
    }
    return (void*)-1;
}

void* malloc(int number)
{
    if(number<0)
        return((void*)-1);
    void* aim;    
    aim = find(number);
    if(aim != (void*)-1)
    {
        used[used_ind].pointer_begin = aim;
        used[used_ind].size = number;
        used_ind++;
        return aim;
    }
    int unused_mem = (int)( (char*)begin - (char*)now );
    printf("%d\n",unused_mem);
    if(unused_mem > number)
    {
        aim = begin;
        begin = begin + number;
        used[used_ind].pointer_begin = aim;
        used[used_ind].size = number;
        used_ind++;
        return aim;
    }
    return (void*)-1;
}

int free(void* pointer)
{
    int i;
    for( i=0;i<used_ind;i++)
        if(used[i].pointer_begin == pointer)
        {
            int j;
            for(j=0;j<pieces_ind;j++)
                if(pointer == pieces[j].pointer_begin + pieces[j].size)
                {
                    pieces[j].size += used[i].size;
                    if(pointer + used[i].size == pieces[j+1].pointer_begin)
                    {
                        pieces[j].size += pieces[j+1].size;
			int k;
                        for( k=j+1;k<pieces_ind;k++)
                        {
                            pieces[k] = pieces[k+1];
                        }
                        pieces_ind--;
                    }
                    break;
                }
                else if(pointer + used[i].size == pieces[j].pointer_begin)
                {
                    pieces[j].pointer_begin = pointer;
                    pieces[j].size += used[i].size;
                    break;
                }
                else if((pointer < pieces[j].pointer_begin) && !(pointer + used[i].size == now))
                {
		    int k;
                    for( k=pieces_ind;k>=j;k--)
                       pieces[k+1] = pieces[k];
                    pieces[j] = used[i];
                    pieces_ind++;
                    break;
                }
            if(pointer + used[i].size == now)
            {
                now = pointer;
            }
            else if(j == pieces_ind)
            {  
                 pieces[j] = used[i];
                 pieces_ind++;
            }
            for(j=i;j<used_ind;j++)
            {
                used[j] = used[j+1];
                printf("index:%d\n", j);
            }
            used_ind--;
            return 1;
        }
    return 0;
}

void test()
{
    printf("top:%d\n", begin);
    printf("bottom:%d\n", end);
    printf("now:%d\n", now); 
    printf("used_index:%d\n", used_ind);
	int i;    
for( i =0;i<used_ind;i++)
    {
        printf("used index:%d pointer:%d size:%d\n", i, used[i].pointer_begin, used[i].size);
    }
    for( i =0;i<pieces_ind;i++)
    {
        printf("pieces index:%d pointer:%d size:%d\n", i, pieces[i].pointer_begin, pieces[i].size);
    }
    printf("\n");
}
#endif
