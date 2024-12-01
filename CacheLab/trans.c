/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if(M == 32)
    {
        int i, j, k;
        int a, b, c, d, e, f, g, h;
        for(i = 0; i < M; i += 8)
        {
            for(j = 0; j < N; j += 8)
            {
                for(k = i; k < i + 8; k++)
                {
                    a = A[k][0+j];  b = A[k][1+j];  c = A[k][2+j];  d = A[k][3+j];
                    e = A[k][4+j];  f = A[k][5+j];  g = A[k][6+j];  h = A[k][7+j];
                    B[0+j][k] = a;  B[1+j][k] = b;  B[2+j][k] = c;  B[3+j][k] = d;
                    B[4+j][k] = e;  B[5+j][k] = f;  B[6+j][k] = g;  B[7+j][k] = h;
                }
            }
        }
    }
    else if(M == 64)
    {
        int i, j, k, l;
        int a, b, c, d, e, f, g, h;
        for(i = 0; i < 64; i += 8)
        {
            for(j = 0; j < 64; j += 8)
            {
                for(k = i; k < i + 4; k++)
                {
                    a = A[k][0+j];  b = A[k][1+j];  c = A[k][2+j];  d = A[k][3+j];
                    e = A[k][4+j];  f = A[k][5+j];  g = A[k][6+j];  h = A[k][7+j];

                    B[0+j][k] = a;    B[1+j][k] = b;    B[2+j][k] = c;    B[3+j][k] = d;
                    B[0+j][4+k] = e;  B[1+j][4+k] = f;  B[2+j][4+k] = g;  B[3+j][4+k] = h;
                }

                for(l = j; l < j + 4; l++)
                {
                    a = A[4+i][l];  b = A[5+i][l];  c = A[6+i][l];  d = A[7+i][l];
                    e = B[l][4+i];  f = B[l][5+i];  g = B[l][6+i];  h = B[l][7+i];

                    B[l][4+i] = a;    B[l][5+i] = b;    B[l][6+i] = c;    B[l][7+i] = d;
                    B[4+l][0+i] = e;  B[4+l][1+i] = f;  B[4+l][2+i] = g;  B[4+l][3+i] = h;
                }

                for(k = i + 4; k < i + 8; k++)
                {
                    a = A[k][4+j];  b = A[k][5+j];  c = A[k][6+j];  d = A[k][7+j];
                    B[4+j][k] = a;  B[5+j][k] = b;  B[6+j][k] = c;  B[7+j][k] = d;
                }

            }
        }
    }
    else if(M == 61)
    {
        int i, j, k, l, t;
        for(i = 0; i < N; i += 22)
        {
            for(j = 0; j < M; j += 22)
            {
                for(k = i; k < i + 22 && k < N; k++)
                {
                    for(l = j; l < j + 22 && l < M; l++)
                    {
                        t = A[k][l];
                        B[l][k] = t;
                    }
                }
            }
        }
    }
    
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

