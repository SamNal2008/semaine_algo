#include <stddef.h>
#include <math.h>
#include <stdio.h>

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

void insertionSort(int *tab, unsigned count)
{
    unsigned i;
    int key;
    int j;
    for (i = 1; i < count; i++) 
    {
        key = tab[i];
        j = i - 1;
        while (j >= 0 && tab[j] > key) 
        {
            tab[j + 1] = tab[j];
            j = j - 1;
        }
        tab[j + 1] = key;
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

void insert_sort_cmp(int *tab, unsigned count, int (*cmp)(int a, int b))
{
    unsigned i;
    int key;
    int j;
    for (i = 1; i < count; i++) 
    {
        key = tab[i];
        j = i - 1;
        while (j >= 0 && (cmp(tab[j], key) == 1)) 
        {
            tab[j + 1] = tab[j];
            j = j - 1;
        }
        tab[j + 1] = key;
    }
}
