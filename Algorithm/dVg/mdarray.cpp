#include "mdarray.h"

// 对矩阵求逆，结果保存在a中  
int matrixInversion(double *a, int n)  
{  
    int *is = new int[n];  
    int *js = new int[n];  
    int i,j,k;  
    double d,p;  
    for ( k = 0; k < n; k++)  
    {  
        d = 0.0;  
        for (i=k; i<=n-1; i++)  
            for (j=k; j<=n-1; j++)  
            {  
                p=fabs(a[i*n+j]);  
                if (p>d) { d=p; is[k]=i; js[k]=j;}  
            }  
            if ( 0.0 == d )  
            {  
                free(is); free(js); printf("err**not inv\n");  
                return(0);  
            }  
            if (is[k]!=k)  
                for (j=0; j<=n-1; j++)  
                {  
                    p=a[k*n+j];  
                    a[k*n+j]=a[is[k]*n+j];  
                    a[is[k]*n+j]=p;  
                }  
            if (js[k]!=k)  
                for (i=0; i<=n-1; i++)  
                {  
                    p=a[i*n+k];  
                    a[i*n+k]=a[i*n+js[k]];  
                    a[i*n+js[k]]=p;  
                }  
            a[k*n+k] = 1.0/a[k*n+k];  
            for (j=0; j<=n-1; j++)  
                if (j!=k)  
                {  
                    a[k*n+j] *= a[k*n+k];  
                }  
            for (i=0; i<=n-1; i++)  
                if (i!=k)  
                    for (j=0; j<=n-1; j++)  
                        if (j!=k)  
                        {  
                            a[i*n+j] -= a[i*n+k]*a[k*n+j];  
                        }  
            for (i=0; i<=n-1; i++)  
                if (i!=k)  
                {  
                    a[i*n+k] = -a[i*n+k]*a[k*n+k];  
                }  
    }  
    for ( k = n-1; k >= 0; k--)  
    {  
        if (js[k]!=k)  
            for (j=0; j<=n-1; j++)  
            {  
                p = a[k*n+j];  
                a[k*n+j] = a[js[k]*n+j];  
                a[js[k]*n+j]=p;  
            }  
            if (is[k]!=k)  
                for (i=0; i<=n-1; i++)  
                {   
                    p = a[i*n+k];  
                    a[i*n+k]=a[i*n+is[k]];  
                    a[i*n+is[k]] = p;  
                }  
    }  
    free(is); free(js);  
    return(1);  
}  

int matrixTrans(double *res, double *src, int num_len, int num_col)
{
	memset(res,0,num_len*num_col);

	int i=0;
	int j=0;
	for(i=0;i<num_len;i++)
	{
		for(j=0;j<num_col;j++)
		{
			res[j*num_len+i]=src[i*num_col+j];
		}
	}

	return 1;
}

/*
功能:求矩阵Mul1[Mul1_m][nm]与Mul2[nm][Mul2_n]的乘积Mul[Mul1_m][Mul2_n]
格式:Matrix_Mul(&Mul1[0][0],Mul1_m,&Mul2[0][0],Mul2_n,nm,&Mul[0][0])
*/
void Matrix_Mul(double *Mul1,int Mul1_m,double *Mul2,int Mul2_n,int nm,double *Mul)
{
      //Mul1[Mul1_m][nm]*Mul2[nm][Mul2_n]=Mul即矩阵的乘法
      int i,j,k,a,b,c,d;
      for (i=0,a=0,c=0;i<Mul1_m;i++)
      {
            for (j=0;j<Mul2_n;j++)
            {
                  b=a+j;
                  Mul[b]=0;
                  for (k=0,d=0;k<nm;k++)
                  {
                        Mul[b]+=Mul1[c+k]*Mul2[d+j];
                        d+=Mul2_n;
                  }
            }
            c+=nm;
            a+=Mul2_n;
      }
}

int Write_Matrix(const char *file_add, const char *name, double *data, int num_len, int num_col)
{
	ofstream ofile;
	ofile.open(file_add);
	ofile<<name<<'='<<endl;
	for(int i=0;i<num_len;i++)
	{
		for(int j=0;j<num_col;j++)
		{
			ofile<<data[i*num_col+j]<<"  ";
		}
		ofile<<endl;
	}

	ofile.close();

	return 1;
}

int Write_Matrix(const char *file_add, const char *name, int *data, int num_len, int num_col)
{
	ofstream ofile;
	ofile.open(file_add);
	ofile<<name<<'='<<endl;
	for(int i=0;i<num_len;i++)
	{
		for(int j=0;j<num_col;j++)
		{
			ofile<<data[i*num_col+j]<<"  ";
		}
		ofile<<endl;
	}

	ofile.close();

	return 1;
}


void Test()
{
	double A[4][4]={{2.6,4.5,8.9,3.2},{8.4,5.7,9.6,3.5},{2.4,6.2,3.7,5.8},{5,8,3.6,4.7}};
	double AT[4][4];
	
	double res[4][4];
	matrixTrans(&AT[0][0],&A[0][0],4,4);
	Matrix_Mul(&A[0][0],4,&AT[0][0],4,4,&res[0][0]);
	matrixInversion(&res[0][0],4);
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			cout<<res[i][j]<<' ';
		}
		cout<<endl;
	}
}
