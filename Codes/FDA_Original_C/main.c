#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "pgmfiles.h"
#include "diff2d.h"

// Funções auxiliares de tempo
static inline long ns_from_ms(long ms){ return ms*1000000L; }
static inline double ms_from_ns(long long ns){ return (double)ns/1e6; }

static inline struct timespec ts_add(struct timespec a, long ns){
    struct timespec r = a;
    r.tv_nsec += ns;
    while(r.tv_nsec >= 1000000000L){ r.tv_nsec -= 1000000000L; r.tv_sec++; }
    return r;
}
static inline long long ts_diff_ns(struct timespec end, struct timespec start){
    long long s = (long long)(end.tv_sec - start.tv_sec)*1000000000LL;
    s += (end.tv_nsec - start.tv_nsec);
    return s;
}
static inline struct timespec now_monotonic(void){
    struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t;
}

// Compilar: gcc -o fda pgmtolist.c pgmfiles.c diff2d.c main.c -lm

int main (int argc, char **argv) {
    char row[80];
    float **matrix;
    int i, j;
    long imax;
    float lambda;
    int result;
    eightBitPGMImage *PGMImage;

    // ---- definindo variáveis de teste ----
    char* m = "arm_hand.pgm";
    lambda = 30;
    imax = 30;

    // ---- cronômetro geral ----
    struct timespec t_start = now_monotonic();
    struct timespec t1, t2;

    // ---- leitura da imagem ----
    PGMImage = (eightBitPGMImage *) malloc(sizeof(eightBitPGMImage));
    if (!argv[1]) {
        strcpy(PGMImage->fileName, m);
    } else {
        strcpy(PGMImage->fileName, argv[1]);
    }

    t1 = now_monotonic();
    result = read8bitPGM(PGMImage);
    t2 = now_monotonic();
    printf("Tempo leitura: %.3f ms\n", ms_from_ns(ts_diff_ns(t2, t1)));

    if(result < 0) {
        printPGMFileError(result);
        exit(result);
    }

    // ---- alocação da matriz ----
    matrix = (float **) malloc (PGMImage->x * sizeof(float *));
    if (matrix == NULL) { 
        printf("not enough storage available\n");
        exit(1);
    } 
    for (i=0; i<PGMImage->x; i++) {
        matrix[i] = (float *) malloc (PGMImage->y * sizeof(float));
        if (matrix[i] == NULL) { 
            printf("not enough storage available\n");
            exit(1);
        }
    }

    // ---- cópia da imagem para matriz ----
    t1 = now_monotonic();
    for (i=0; i<PGMImage->x; i++)
        for (j=0; j<PGMImage->y; j++)
            matrix[i][j] = (float) *(PGMImage->imageData + (i*PGMImage->y) + j); 
    t2 = now_monotonic();
    printf("Tempo cópia inicial: %.3f ms\n", ms_from_ns(ts_diff_ns(t2, t1)));

    // ---- processamento FDA ----
    printf("contrast parameter lambda: %f\n", lambda);
    printf("number of iterations: %ld\n", imax);

    t1 = now_monotonic();
    for (i=1; i<=imax; i++) {
        diff2d (0.5, lambda, PGMImage->x, PGMImage->y, matrix); 
    }
    t2 = now_monotonic();
    printf("Tempo processamento (FDA): %.3f ms\n", ms_from_ns(ts_diff_ns(t2, t1)));

    // ---- cópia da matriz de volta para imagem ----
    t1 = now_monotonic();
    for (i=0; i<PGMImage->x; i++)
        for (j=0; j<PGMImage->y; j++)
            *(PGMImage->imageData + i*PGMImage->y + j) = (char) matrix[i][j];
    t2 = now_monotonic();
    printf("Tempo cópia final: %.3f ms\n", ms_from_ns(ts_diff_ns(t2, t1)));

    // ---- escrita ----
    if (!argv[2]) {
        strcpy(PGMImage->fileName, "saida.pgm");
    } else {
        strcpy(PGMImage->fileName, argv[2]);
    }

    t1 = now_monotonic();
    write8bitPGM(PGMImage);
    t2 = now_monotonic();
    printf("Tempo escrita: %.3f ms\n", ms_from_ns(ts_diff_ns(t2, t1)));

    // ---- tempo total ----
    struct timespec t_end = now_monotonic();
    printf("Tempo TOTAL: %.3f ms\n", ms_from_ns(ts_diff_ns(t_end, t_start)));

    // ---- desalocações ----
    for (i=0; i<PGMImage->x; i++)
        free(matrix[i]);
    free(matrix);

    free(PGMImage->imageData);
    free(PGMImage);

    return 0;
}