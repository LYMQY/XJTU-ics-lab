#include "cachelab.h"
#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<math.h>
#include<stdbool.h>
#include<unistd.h>

typedef unsigned long long int mem_addr_t;
int hit_count = 0, miss_count = 0, eviction_count = 0;

typedef struct cache_line {
    int valid_bit;
    int tag_bit;
    int block;
    int time;
}cache_line;

// 定义全局变量
int s, E, b;
int times = 0;

typedef cache_line* set;

typedef struct cache {
    int lines;
    int sets;
    int s_bits;
    int b_bits;
    set* p;
}cache;

void freeCache(cache* cache_init)
{
    for(int i = 0; i < cache_init->sets; i++)
    {
        free(cache_init->p[i]);
    }
    free(cache_init->p);
}

cache cache_init;

void cacheLoad(mem_addr_t address, bool flag)
{
    int set_index = (address >> b) % (1 << s);
    int tag = address >> (b + s);

    for(int i = 0; i < E; i++)
    {
        if(cache_init.p[set_index][i].tag_bit == tag && cache_init.p[set_index][i].valid_bit == 1)
        {
            hit_count++;
            times++;
            cache_init.p[set_index][i].time = times;
            if(flag)
            {
                printf("hit");
            }
            return;
        }
    }

    if(flag)
    {
        printf("miss ");
    }
    miss_count++;
    for(int i = 0; i < E; i++)
    {
        if(cache_init.p[set_index][i].valid_bit == 0)
        {
            cache_init.p[set_index][i].valid_bit = 1;
            cache_init.p[set_index][i].tag_bit = tag;
            times++;
            cache_init.p[set_index][i].time = times;
            return;
        }
    }

    if(flag)
    {
        printf("eviction ");
    }
    eviction_count++;
    int k = cache_init.p[set_index][0].time;
    int index = 0;
    for(int i = 1; i < E; i++)
    {
        if(k > cache_init.p[set_index][i].time)
        {
            index = i;
            k = cache_init.p[set_index][i].time;
        }
    }

    times++;
    cache_init.p[set_index][index].tag_bit = tag;
    cache_init.p[set_index][index].time = times;
    return;
}

int main(int argc, char** argv)
{
    int opt;
    FILE* t = NULL;
    bool flag = false;
    while(-1 != (opt = getopt(argc, argv, "vs:E:b:t:"))){
        switch(opt) {
            case 'v':
                flag = true;
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                t = fopen(optarg, "r");
                break;
            default:
                printf("wrong argument\n");
                break;
        }
    }

    cache_init.lines = E;
    cache_init.sets = pow(2, s);
    cache_init.s_bits = s;
    cache_init.b_bits = b;
    cache_init.p = (set*)malloc(cache_init.sets * sizeof(set));
    for(int i = 0; i < cache_init.sets; i++)
    {
        cache_init.p[i] = (cache_line*)malloc(E * sizeof(cache_line));
    }

    char identifier;
    mem_addr_t address;
    int size;

    while(fscanf(t, " %c %llx,%d", &identifier, &address, &size) > 0) {
        switch(identifier)
        {
            case 'L':
                if(flag)
                {
                    printf("%c %llx,%d ", identifier, address, size);
                }
                cacheLoad(address, flag);
                break;
            case 'M':
                if(flag)
                {
                    printf("%c %llx,%d ", identifier, address, size);
                }
                cacheLoad(address, flag);
                cacheLoad(address, flag);
                break;
            case 'S':
                if(flag)
                {
                    printf("%c %llx,%d ", identifier, address, size);
                }
                cacheLoad(address, flag);
                break;
            default:
                break;
        }
        if(flag)
        {
            printf("\n");
        }
    }
    
    printSummary(hit_count, miss_count, eviction_count);
    freeCache(&cache_init);
    return 0;
}
