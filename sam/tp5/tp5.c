#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
unsigned int_width(int i)
{
    float r = i;
    unsigned res = 0;
    if (r == 0)
        return 1;
    if (r < 0)
    {
        res++;
        r *= -1;
    }
    res += log10(r) + 1;
    return res;
}

unsigned ints_width(const int *tab, unsigned count)
{
    if (count > 0)
    {
        int max = tab[0];
        int min = tab[0];
        for (unsigned i = 0; i < count; i++)
        {
            if (tab[i] > max)
                max = tab[i];
            if (tab[i] < min)
                min = tab[i];
        }
        unsigned max_w = int_width(max);
        unsigned min_w = int_width(min);
        return (max_w > min_w) ? max_w : min_w;
    }
    return 0;
}

void print_int_array(FILE *out, const int *tab, unsigned count)
{
    unsigned pad = int_width(count - 1);
    unsigned spaces = ints_width(tab, count);
    unsigned cha = 0;
    for (unsigned i = 0; i < count; i++)
    {
        if (!cha)
        {
            for (unsigned j = 0; j < pad - int_width(i); j++)
            {
                fprintf(out, " ");
            }
            fprintf(out, "[%d] ", i);
            cha += 3 + pad;
        }
        if (cha + 2 * spaces >= 80 || i + 1 == count)
        {
            cha = 0;
            fprintf(out, "%*d\n", spaces, tab[i]);
        }
        else
        {
            fprintf(out, "%*d ", spaces, tab[i]);
            cha += spaces + 1;
        }
    }
}

int increasing(int a, int b)
{
    if (a < b)
        return -1;
    return a > b;
}

int decreasing(int a, int b)
{
    if (a > b)
        return -1;
    return a < b;
}

unsigned partition_cmp(int *tab, unsigned b, unsigned e, unsigned p,
                       int(*cmp)(int a, int b))
{
    int x = tab[p];
    unsigned i = b - 1;
    unsigned j = e;
    while (1)
    {
        do
        {
            i++;
        }
        while (i != e && i != p &&  cmp(tab[i], x) >= 0);
        do
        {
            j--;
        }
        while (j != 0 && j != p && cmp(tab[j], x) >= 0);
        if (j <= i)
            return i + (i == b);
        int tmp = tab[i];
        tab[i] = tab[j];
        tab[j] = tmp;
    }
}

unsigned pivot_first(const int* tab, unsigned l, unsigned r,
                     int (*cmp)(int a, int b))
{
    (void) tab;
    (void) r;
    (void) cmp;
    return l;
}

unsigned pivot_rand(const int* tab, unsigned l, unsigned r,
                    int (*cmp)(int a, int b))
{
    (void) tab;
    return l + (rand() % (r - l));
}


unsigned pivot_median3(const int *tab, unsigned l, unsigned r,
                     int (*cmp)(int a, int b))
{
    int p1 = tab[l];
    int p2 = tab[r - 1];
    int p3 = tab[l + (r - l) / 2];
    if (cmp(p1, p2) >= 0)
    {
        if (cmp(p1, p3) >= 0)
        {
            if (cmp(p2, p3) >= 0)
                return r - 1;
            else
                return l + (r - l) / 2;
        }
        else
        {
            return l;
        }
    }
    if (cmp(p2, p3) >= 0)
    {
        if (cmp(p1, p3) >= 0)
            return l;
        return l + (r - l) / 2;
    }
}

void quick_sort_cmp(int *tab, unsigned count, unsigned (*pivot)(const int *tab, unsigned l, unsigned r, int (*cmp)(int a, int b)), int (*cmp)(int a, int b))
{
    if (count > 1)
    {
        unsigned p = partition_cmp(tab, 0, count, pivot(tab, 0, count,cmp), cmp);
        quick_sort_cmp(tab, p, pivot, cmp);
        quick_sort_cmp(tab + p, count - p, pivot, cmp);
    }
}

void quick_sort_cmp_loop(int *tab, unsigned count, unsigned (*pivot)(const
int *tab, unsigned l, unsigned r, int (*cmp)(int a, int b)),int (*cmp)(int a,
int b))
{
    unsigned l = 0;
    unsigned r = count;
    while (r - l > 0)
    {
        unsigned p = partition_cmp(tab, 0, count, pivot(tab, 0, count,cmp), cmp);
        quick_sort_cmp_loop(tab + l, r - l, pivot, cmp);
        l = p;
    }
}

uintmax_t cmp_count;

int my_increasing(int a, int b)
{
    ++cmp_count;
    if (a > b)
        return -1;
    return a > b;
}

#define mycheck(fun, tab) \
{                         \
    cmp_count = 0;\
    clock_t before = clock();\
    fun(tab, tsize, pivot_median3, my_increasing);\
    clock_t after = clock();\
    printf(#fun " used %ju comparisons and %ju ticks \n",\
    cmp_count, (uintmax_t) (after - before));\
}

int main(void)
{
    srand(0);
    const unsigned tsize = 500000;
    int *tab1 = malloc(tsize * sizeof(int));
    int *tab2 = malloc(tsize * sizeof(int));
    for (unsigned i = 0; i < tsize; ++i)
        tab2[i] = tab1[i] = rand(); // = i;

    mycheck(quick_sort_cmp, tab1);
    mycheck(quick_sort_cmp_loop, tab2);
    return 0;
}
