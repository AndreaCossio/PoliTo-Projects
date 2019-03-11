#include "quicksort.h"

void qsWrapper(int *vet, int N) {
    qs(vet, 0, N - 1);
}

void qs(int *vet, int l, int r) {
    int q;
    if (r <= l)
        return;
    q = partition(vet, l, r);
    qs(vet, l, q - 1);
    qs(vet, q + 1, r);
}

int partition(int *vet, int l, int r) {
    int i = l - 1, j = r, temp;
    int x = vet[r];
    for (;;) {
        while (vet[++i] < x);
        while (vet[--j] > x);
        if (j == l)
            break;
        if (i >= j)
            break;
        temp = vet[i];
        vet[i] = vet[j];
        vet[j] = temp;
    }
    temp = vet[i];
    vet[i] = vet[r];
    vet[r] = temp;
    return i;
}