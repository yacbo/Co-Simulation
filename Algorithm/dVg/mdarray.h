/* 
 * mdarray.h 
*/  
  
#ifndef MDARRAY_H_  
#define MDARRAY_H_  
  
#include"stdio.h"
#include"malloc.h"
#include"math.h" 
#include "string.h"
#include<iostream>
#include <fstream>
#include "algorithm_global.h"
using namespace std;

int matrixInversion(double *a, int n);//矩阵求逆
int matrixTrans(double *res, double *src, int num_len, int num_col);//矩阵转置
void Matrix_Mul(double *Mul1,int Mul1_m,double *Mul2,int Mul2_n,int nm,double *Mul);//矩阵乘法

int ALGORITHMSHARED_EXPORT Write_Matrix(const char *file_add, const char *name,double *data,int num_len,int num_col);//写矩阵文件
int ALGORITHMSHARED_EXPORT Write_Matrix(const char *file_add, const char *name,int *data,int num_len,int num_col);
void Test();
  
#endif /* MDARRAY_H_ */  
