#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "ptls.h"

bool checkSmaller(File *a, File *b, Args* args)
{
    if(args->sExtension)
    {
        int posA = strlen(a->name)-1;
        int posB = strlen(b->name)-1;

        char* pa = a->name+posA;
        char* pb = b->name+posB;

        while(posA >= 0 && posB >= 0 && !(*pa == '.' && *pb == '.'))
        {
            if(*pa != '.')
            {
                posA--;
                pa--;
            }
            if(*pb != '.')
            {
                posB--;
                pb--; 
            }
        }
        int result = strcmp(pa, pb);
        return (args->reverse) ? result > 0 : result < 0;
    }
    else if (args->sTime)
    {
        int result = a->stats->st_ctime - b->stats->st_ctime;
        return (args->reverse) ? result < 0 : result > 0;
    }
    int result = strcmp(a->name, b->name);
    return (args->reverse) ? result > 0 : result < 0;
}

void swap(File *a, File *b)
{
    File tmp = *a;
    *a = *b;
    *b = tmp;
}
 
int partition (File **file, Args* args, int low, int high)
{
    File *pivot = file[high];
    int i = (low - 1);  
 
    for (int j = low; j <= high- 1; j++)
    {
        if (checkSmaller(file[j], pivot, args))
        {
            i++;
            swap(file[i], file[j]);
        }
    }
    swap(file[i + 1], file[high]);
    return (i + 1);
}
 
void sort(File **file, Args* args, int low, int high)
{
    if (low < high)
    {
        int pi = partition(file, args, low, high);
 
        sort(file, args, low, pi - 1);
        sort(file, args, pi + 1, high);
    }
}
