#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "ptls.h"

bool checkSmaller(FileInstance* a, FileInstance* b, Args* args)
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
        return strcmp(pa, pb) < 0;
    }
    else if (args->sTime)
    {
        return a->stats->st_ctime - b->stats->st_ctime > 0;
    }
    return strcmp(a->name, b->name) < 0;
}

void swap(FileInstance* a, FileInstance* b)
{
    FileInstance tmp = *a;
    *a = *b;
    *b = tmp;
}
 
int partition (FileInstance **instances, Args* args, int low, int high)
{
    FileInstance* pivot = instances[high];
    int i = (low - 1);  
 
    for (int j = low; j <= high- 1; j++)
    {
        if (checkSmaller(instances[j], pivot, args))
        {
            i++;
            swap(instances[i], instances[j]);
        }
    }
    swap(instances[i + 1], instances[high]);
    return (i + 1);
}
 
void sort(FileInstance **instances, Args* args, int low, int high)
{
    if (low < high)
    {
        int pi = partition(instances, args, low, high);
 
        sort(instances, args, low, pi - 1);
        sort(instances, args, pi + 1, high);
    }
}
