#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

unsigned int_width(int i)
{
    long int r = (long int) i;
    if (i == 0)
        return 1;
    if (i < 0)
    {
        r *= -1;
        return log10(r) + 2;
    }
    else
        return log10(r) + 1;
}

unsigned ints_width(const int* tab, unsigned count)
{
    unsigned r = int_width(tab[0]);
    unsigned i = 1;
    while (i < count)
    {
        if (int_width(tab[i]) > r)
        {
            r = int_width(tab[i]);
        }
        i++;
    }
    return r;
}

void print_int_array(FILE* out, const int* tab, unsigned count)
{
    unsigned max = ints_width(tab, count);
    int col = 0;
    unsigned i = 0;
    while (i < count)
    {
        if (i == 0)
        {
            unsigned g = 1;
            while (g < int_width(count - 1))
            {
                fputc(' ', out);
                g++;
            }
            fprintf(out,"[0]");
            col += 4;
        }
        int size = int_width(tab[i]);
        if (col + max + 1 > 80)
        {
            col = 0;
            fputc('\n', out);
            int g = 0;
            while (g + int_width(i) < int_width(count - 1))
            {
                col++;
                fputc(' ', out);
                g++;
            }
            fprintf(out,"[%d]", i);
            col += 2 + int_width(i);
        }
        col += max;
        while (max - size > 0)
        {
            fputc(' ', out);
            size++;
        }
        fprintf(out," %d", tab[i]);
        col++;
        i++;
    }
    fputc('\n', out);
}

void heapify(int* tab, unsigned pos, unsigned n)
{
    unsigned l = 2*pos+1;
    unsigned r = 2*pos+2;
    unsigned g;
    if (l < n && tab[l] > tab[pos])
    {
        g = l;
    }
    else
    {
        g = pos;
    }
    if (r<n && tab[r] > tab[g])
        g = r;
    if (g != pos)
    {
        int tmp = tab[pos];
        tab[pos] = tab[g];
        tab[g] = tmp;
        heapify(tab, g, n);
    }
}

void make_heap(int* tab, unsigned n)
{
    for (int i = n/2 -1 ; i >= 0; --i)
    {
        heapify(tab, i, n);
    }
}

bool check_heap(int* heap, unsigned n)
{
    unsigned i = 0;
    while (i < n/2)
    {
        if (heap[2*i+2] > heap[i])
            return false;
        if (heap[2*i+1] > heap[i])
            return false;
        i++;
    }
    return true;
}

void pretty_print_heap(FILE* out, const int* v, unsigned n)
{
    unsigned w = ints_width(v, n)+1;
    int level = round(log2(n));
    int t = w*pow(2,level);
    int pref;
    unsigned i = 0;
    int nb;
    int div = 2;
    for (int j = 0; j < level || i < n; j++)
    {
        nb = pow(2, j);
        pref = t/div;
        div *=2;
        for (int k = 0; k < nb && i < n; k++)
        {
            if (k==0)
            {
                fprintf(out, "%*d", (pref/2) +1, v[i]);
            }
            else
            {
                fprintf(out, "%*d", pref, v[i]);
            }
            i++;
        }
        fprintf(out, "\n");
    }
    
}

int pop_heap(int* heap, unsigned* n)
{
    if (*n == 0)
        return 0;
    int tmp = heap[*n-1];
    (*n)--;
    heap[*n] = heap[0];
    heap[0] = tmp;
    make_heap(heap, *n);
    return heap[0];
}

int main(void)
{
    int a[] = { 123, 0, 33, 42, 544, 165, -73, 228 };
unsigned asize = sizeof(a) / sizeof(*a);
make_heap(a, asize);
puts("init");
pretty_print_heap(stdout, a, asize);
for (unsigned i = 0; i < 5; ++i)
  {
     printf("pop_heap() == %d\n", pop_heap(a, &asize));
     pretty_print_heap(stdout, a, asize);
  }
    return 1;
}



