#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm> 
using namespace std;
typedef double dato;
typedef vector<vector<dato> > matrix;


void llenar(matrix &A, matrix &B, matrix &R, int tam)
{
    A.resize(tam);
    B.resize(tam);
    R.resize(tam);
    for(int k = 0 ; k < tam ; ++k )
    {
        A[k].resize(tam);
        B[k].resize(tam);
        R[k].resize(tam);
    }


    for(int i=0;i<tam;++i)
    {
        for(int j=0;j<tam;++j)
        {
            int num = rand() % 7 + 1;
            A[i][j]= num;
            B[i][j]= num + 2;
            R[i][j]= 0;
        }
    }
}

matrix mul_matrix(matrix A, matrix B, int tam)
{   matrix resp=A;
    for(int i=0;i<tam;++i)
    {
        for(int j=0;j<tam;++j)
        {
            resp[i][j]=0;
            for(int k=0;k<tam;++k)
            {
                resp[i][j]+=A[i][k]*B[k][j];
            }
        }
    }
    return resp;
}

void mul_bloques( matrix A, matrix B, int tam,  matrix &resp )
{
    int n_bloques = 20;
    for (int i = 0; i < tam; i += n_bloques) 
    {
         for (int j = 0; j < tam; j += n_bloques) 
         {
             for (int k = 0; k < tam; k += n_bloques) 
             {
                 for (int x = i; x < min( i + n_bloques, tam ); x++) 
                 {
                     for (int y = j; y < min( j + n_bloques, tam ); y++) 
                     {   
                         int acumulador = 0;      
                         for (int z = k; z < min( k + n_bloques, tam ); z++) 
                         {

                             acumulador += A[x][z] * B[z][y];

                         }
                         resp[x][y] += acumulador; 
                     } 
                 }
             }
         }
     }                  
}

void imprimir(matrix n, int tam)
{
    for(int i=0;i<tam;++i)
    {
        for(int j=0;j<tam;++j)
        {
            cout<<n[i][j]<<" ";
        }
        cout<<endl;
    }
}


int main()
{
    matrix A,B,resp1,resp2;
    int tam=300;
    llenar(A,B,resp2,tam);
    //resp1=mul_matrix(A,B,tam);
    mul_bloques(A,B,tam,resp2);
    //imprimir(resp2,tam);
    return 0;
}
