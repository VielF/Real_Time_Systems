#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "diff2d.h"

// ---- Funções auxiliares de tempo ----
static inline double ms_from_ns(long long ns){ return (double)ns/1e6; }
static inline long long ts_diff_ns(struct timespec end, struct timespec start){
    long long s = (long long)(end.tv_sec - start.tv_sec)*1000000000LL;
    s += (end.tv_nsec - start.tv_nsec);
    return s;
}
static inline struct timespec now_monotonic(void){
    struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t;
}


/*--------------------------------------------------------------------------*/

float dco (float v, float w, float lambda)
/* diffusivity */
{
    float result = 0.0, temp_result = 0.0;

    temp_result = (float)fabs(v-w);
    temp_result = pow(temp_result,0.2);
    temp_result = temp_result/lambda;
    if(temp_result != 0.0){
        temp_result = -(temp_result/5.0);
    }
    result = exp(temp_result);

    return (result);
}


/*--------------------------------------------------------------------------*/


void diff2d(float ht, float lambda, long nx, long ny, float **f)
{
    long    i, j;                                     /* loop variables */
    float   qC, qN, qNE, qE, qSE, qS, qSW, qW, qNW;   /* weights */
    float   **g;                                      /* work copy of f */

    // Cronômetros
    struct timespec t1, t2;
    long long copy_ns=0, border_ns=0, cond_ns=0, free_ns=0;

    /* ---- allocate storage for g ---- */
    g = (float **) malloc ((nx+2) * sizeof(float *));
    if (g == NULL) {
        printf("not enough storage available\n");
        exit(1);
    }
    for (i=0; i<=nx+1; i++) {
        g[i] = (float *) malloc ((ny+2) * sizeof(float));
        if (g[i] == NULL) {
            printf("not enough storage available\n");
            exit(1);
        }
    }

    /* ---- copy f into g ---- */
    t1 = now_monotonic();
    for (i=1; i<=nx; i++)
        for (j=1; j<=ny; j++)
            g[i][j] = f[i-1][j-1];
    t2 = now_monotonic();
    copy_ns = ts_diff_ns(t2,t1);

    /* ---- create dummy boundaries ---- */
    t1 = now_monotonic();
    for (i=1; i<=nx; i++) {
        g[i][0]    = g[i][1];
        g[i][ny+1] = g[i][ny];
    }
    for (j=0; j<=ny+1; j++) {
        g[0][j]    = g[1][j];
        g[nx+1][j] = g[nx][j];
    }
    t2 = now_monotonic();
    border_ns = ts_diff_ns(t2,t1);

    /* ---- diffusive averaging ---- */
    t1 = now_monotonic();
    for (i=1; i<=nx; i++) {
        for (j=1; j<=ny; j++) {
            /* calculate weights (condutância + exponenciais) */
            qN  = (1.0 - exp(-8.0 * ht * dco(g[i][j], g[i  ][j+1], lambda))) / 8.0;
            qNE = (1.0 - exp(-8.0 * ht * dco(g[i][j], g[i+1][j+1], lambda))) / 8.0;
            qE  = (1.0 - exp(-8.0 * ht * dco(g[i][j], g[i+1][j  ], lambda))) / 8.0;
            qSE = (1.0 - exp(-8.0 * ht * dco(g[i][j], g[i+1][j-1], lambda))) / 8.0;
            qS  = (1.0 - exp(-8.0 * ht * dco(g[i][j], g[i  ][j-1], lambda))) / 8.0;
            qSW = (1.0 - exp(-8.0 * ht * dco(g[i][j], g[i-1][j-1], lambda))) / 8.0;
            qW  = (1.0 - exp(-8.0 * ht * dco(g[i][j], g[i-1][j  ], lambda))) / 8.0;
            qNW = (1.0 - exp(-8.0 * ht * dco(g[i][j], g[i-1][j+1], lambda))) / 8.0;
            qC  = 1.0 - qN - qNE - qE - qSE - qS - qSW - qW - qNW;

            /* weighted averaging (atualização da imagem) */
            f[i-1][j-1] = qNW * g[i-1][j+1] + qN * g[i][j+1] + qNE * g[i+1][j+1] +
                          qW  * g[i-1][j  ] + qC * g[i][j  ] + qE  * g[i+1][j  ] +
                          qSW * g[i-1][j-1] + qS * g[i][j-1] + qSE * g[i+1][j-1];
        }
    }
    t2 = now_monotonic();
    cond_ns = ts_diff_ns(t2,t1); // inclui condutâncias + atualização

    /* ---- disallocate storage for g ---- */
    t1 = now_monotonic();
    for (i=0; i<=nx+1; i++)
        free(g[i]);
    free(g);
    t2 = now_monotonic();
    free_ns = ts_diff_ns(t2,t1);

    /* ---- imprimir tempos da iteração ---- */
    printf("FDA -> copia: %.3f ms | borda: %.3f ms | cond+update: %.3f ms | free: %.3f ms\n",
           ms_from_ns(copy_ns), ms_from_ns(border_ns), ms_from_ns(cond_ns), ms_from_ns(free_ns));

    return;
}
