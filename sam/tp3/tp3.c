#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


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

unsigned cmp_count;

unsigned linear_search(const int *tab, unsigned count, int val,
                       int (*cmp)(int a, int b))
{
    unsigned i = 0;
    for (; i < count && cmp(tab[i], val) == -1; i++);
    return i;
}

unsigned binary_search(int *tab, unsigned count, int val, int (*cmp)(int a,
                                                                     int b))
{
    if (!tab)
        return 0;
    unsigned b = 0;
    unsigned e = count;
    unsigned m;
    while (b < e)
    {
        m = (b + e) / 2;
        int res = cmp(tab[m], val);
        if (!res)
            return m;
        else if (res == 1)
            e = m;
        else if (res == -1)
            b = m + 1;
    }
    return b;
}

void bs_insert_sort_cmp(int *tab, unsigned count, int (*cmp)(int a, int b))
{
    int loc, j, selected;
    for (unsigned i = 1; i < count; ++i)
    {
        j = i - 1;
        selected = tab[i];
        loc = binary_search(tab, j + 1, selected, cmp);
        while (j >= loc)
        {
            memmove(tab + j + 1, tab + j, sizeof(int));
            j--;
        }
        tab[j+1] = selected;
    }
}

unsigned interpolate(int *tab, unsigned b, unsigned e, int val,
                     int (*cmp)(int a, int b))
{
    int cp = cmp(tab[b], tab[e]);
    if (!cp)
    {
        int comp = cmp(tab[b],val);
        if (comp != -1)
            return b;
        if (comp == -1)
            return e;
    }
    if (cmp(val,tab[b]) != 1)
        return b;
    if (cmp(val, tab[e]) == 1)
        return e;
    float xa = b;
    float ya = tab[b];
    float xb = e;
    float yb = tab[e];
    float slope = (yb - ya) / (xb - xa);
    float snd = (xa * yb - xb * ya)/(xa - xb);
    double res = (val - snd) / slope;
    res = floor(res + 0.5);
    return res;
}

unsigned interpolation_search(int *tab, unsigned count, int val,
                              int (*cmp)(int a, int b))
{
    if (!tab)
        return 0;
    unsigned b = 0;
    unsigned e = count;
    unsigned m;
    while (b < e)
    {
        m = interpolate(tab, b, e-1, val, cmp);
        int res = cmp(tab[m], val);
        if (!res)
            return m;
        else if (res == 1)
            e = m;
        else if (res == -1)
            b = m + 1;
    }
    return b;
}


void heapify(int *tab, int pos, unsigned m)
{
    unsigned l = 2 * pos + 1;
    unsigned r = 2 * pos + 2;
    int g;
    if (l < m && tab[l] > tab[pos])
        g = l;
    else
        g = pos;
    if (r < m && tab[r] > tab[g])
        g = r;
    if (g != pos)
    {
        int tmp = tab[pos];
        tab[pos] = tab[g];
        tab[g] = tmp;
        heapify(tab, g, m);
    }
}

void make_heap(int *tab, unsigned n)
{
    unsigned start = (n / 2) - 1;
    for (int i = start; i >= 0; i--)
    {
        heapify(tab, i, n);
    }
}

bool check_heap(int *heap, unsigned n)
{
    int end = (n / 2);
    for (int i = 0; i < end; i++)
    {
        if (heap[2 * i + 1] > heap[i] || heap[2 * i + 2] > heap[i])
            return false;
    }
    return true;
}

void pretty_print_heap(FILE *out, const int *v, unsigned n)
{
    int hight = log2(n) + 1;
    int space = ints_width(v, n) + 1;
    int weight = pow(2, hight) * space; 
    int nb_word = pow(2, 0);
    int div = 2;
    int ind = 0;
    int place = weight / div;
    int end = n;
    for (int i = 0; i < hight || ind < end; i++)
    {
        //if (i < hight)
        //{
            nb_word = pow(2, i);
            place = weight / div;
            div *= 2;
        //}
        for (int j = 0; j < nb_word && ind < end; j++)
        {
            if (!j)
            {

                fprintf(out,"%*d", (place / 2) + 1, v[ind]);
            }
            else
            {
                fprintf(out,"%*d", place, v[ind]);
            }
            ind++;
        }
        fprintf(out,"\n");
    }
}

int pop_heap(int *heap, unsigned *n)
{
    if (*n <= 0)
    {
        return 0;
    }
    int res = heap[0];
    (*n)--;
    heap[0] = heap[*n];
    heap[*n] = res;
    make_heap(heap, *n); 
    return res;
}

void heap_sort(int *tab, unsigned n)
{
    make_heap(tab, n);
    for (unsigned i = n - 1; i >= 1; i--)
    {
        int tmp = tab[0];
        tab[0] = tab[i];
        tab[i] = tmp;
        heapify(tab, 0, i);
    }
}

void heapify_cmp(int *v, unsigned i, unsigned n, int (*cmp)(int a, int b))
{
    unsigned l = 2 * i + 1;
    unsigned r = 2 * i + 2;
    unsigned g;
    if (l < n && cmp(v[l], v[i]) == 1)
        g = l;
    else
        g = i;
    if (r < n && cmp(v[r], v[g]) == 1)
        g = r;
    if (g != i)
    {
        int tmp = v[i];
        v[i] = v[g];
        v[g] = tmp;
        heapify_cmp(v, g, n, cmp);
    }
}

void make_heap_cmp(int *v, unsigned n, int (*cmp)(int a, int b))
{
    unsigned start = (n / 2) - 1;
    for (int i = start; i >= 0; i--)
    {
        heapify_cmp(v, i, n, cmp);
    }
}

bool check_heap_cmp(int *v, unsigned n, int (*cmp)(int a, int b))
{
    int end = (n / 2);
    for (int i = 0; i < end; i++)
    {
        if (cmp(v[2 * i + 1], v[i]) == 1 || cmp(v[2 * i + 2], v[i]) == 1)
            return false;
    }
    return true;
}

int pop_heap_cmp(int *heap, unsigned *n, int (*cmp)(int a, int b))
{
    if (*n <= 0)
    {
        return 0;
    }
    int res = heap[0];
    (*n)--;
    heap[0] = heap[*n];
    heap[*n] = res;
    make_heap_cmp(heap, *n, cmp); 
    return res;
}

void heap_sort_cmp(int* tab, unsigned n, int (*cmp)(int a, int b))
{
    make_heap_cmp(tab, n, cmp);
    for (unsigned i = n - 1; i >= 1; i--)
    {
        int tmp = tab[0];
        tab[0] = tab[i];
        tab[i] = tmp;
        heapify_cmp(tab, 0, i, cmp);
    }
}

int main()
{
    int a[] = { 16, -2, 53, 45, -995, 666, 887,-148, 59, 10, 151, 412, -13 };
    unsigned asize = sizeof(a) / sizeof(*a);
    heap_sort_cmp(a, asize, decreasing);
    puts("output:");
    print_int_array(stdout, a, asize);
    return 0;
}

