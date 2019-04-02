#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/mman.h>

struct params {
    int *vet, l, r, size;
};

// Global variables
int *vet;

// Prototypes
void qsThreadedWrapper(int *vet, int N, int size);
void *qsThreaded(void *args);
struct params *fillStruct(int *vet, int l, int r, int size);

int main(int argc, char *argv[]) {

    int fd, size, len, pg;
    struct stat sb;

    // Check arguments
    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments.\n\nUsage: %s file size\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse argument
    size = atoi(argv[2]);

    // Open file
    if ((fd = open(argv[1], O_RDWR, 0644)) == -1) {
        perror("Error open()");
        exit(EXIT_FAILURE);
    }

    // Gather infos
    if (fstat(fd, &sb) == -1) {
        perror("Error fstat()");
        exit(EXIT_FAILURE);
    }
    len = sb.st_size;
    pg = getpagesize();
    printf("Page size: %d\nFile size: %d\n", pg, len);

    // Create memory map
    if ((vet = mmap(0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (void *) -1) {
        perror("Error mmap()");
        exit(EXIT_FAILURE);
    }

    // Start quicksorting
    qsThreadedWrapper(vet, len/sizeof(int), size);

    // Release resources
    munmap(vet, len);
    close(fd);

    return 0;
}

// Quicksort wrapper
void qsThreadedWrapper(int *vet, int N, int size) {

    struct params *par;
    pthread_t tid;

    par = fillStruct(vet, 0, N-1, size);

    pthread_create(&tid, NULL, qsThreaded, (void *) par);
    pthread_join(tid, NULL);

    free(par);

    return;
}

// Quicksort without threads
void quicksort(int *vet, int l, int r) {

    int i, j, x, tmp;

    if (r <= l) {
        return;
    }

    x = vet[l];
    i = l - 1;
    j = r + 1;

    while (i < j) {
        while (vet[--j] > x);
        while (vet[++i] < x);
        if (i < j) {
            tmp = vet[i];
            vet[i] = vet[j];
            vet[j] = tmp;
        }
    }

    quicksort(vet, l, j);
    quicksort(vet, j + 1, r);
}

// Quicksort with threads until a subvector of size elements
void *qsThreaded(void *args) {

    int *vet, l, r, size, i, j, x, tmp;
    struct params *par, *par1, *par2;
    pthread_t tid[2];

    par = (struct params *) args;
    vet = par->vet;
    l = par->l;
    r = par->r;
    size = par->size;

    if (r <= l) {
        pthread_exit(NULL);
    }
   
    x = vet[l];
    i = l - 1;
    j = r + 1;

    while (i < j) {
        while (vet[--j] > x);
        while (vet[++i] < x);
        if (i < j) {
            tmp = vet[i];
            vet[i] = vet[j];
            vet[j] = tmp;
        }
    }

    if (r - l < size) {
        quicksort(vet, l, j);
        quicksort(vet, j + 1, r);
    } else {
        par1 = fillStruct(vet, l, j, size);
        par2 = fillStruct(vet, j + 1, r, size);

        pthread_create(&(tid[0]), NULL, qsThreaded, (void *) par1);
        pthread_create(&(tid[1]), NULL, qsThreaded, (void *) par2);
        pthread_join(tid[0], NULL);
        pthread_join(tid[1], NULL);

        free(par1);
        free(par2);
    }

    pthread_exit(NULL);
}

// Write params inside a struct to be passed to threads
struct params *fillStruct(int *vet, int l, int r, int size) {

    struct params *par;

    par = malloc(sizeof(struct params));
    par->vet = vet;
    par->l = l;
    par->r = r;
    par->size = size;

    return par;
}