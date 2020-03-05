#include "gaosixy.h"
#include <QDebug>
#include <stdlib.h>
#include <math.h>

GaoSiXY::GaoSiXY()
{
}

void GaoSiXY::solve(double**a,int n,double *recv)
{
    int m = 0;
    int i,j;

    for(j = 0; j < n; j++){
        double max = 0;
        double imax = 0;
        for(i = j; i < n; i++){
            if(imax < fabs(a[i][j])){
                imax = fabs(a[i][j]);
                max = a[i][j];//得到各行中所在列最大元素
                m = i;
            }
        }
        if(fabs(a[j][j]) != max) {
            double b = 0;
            for(int k = j;k < n + 1; k++){
                b = a[j][k];
                a[j][k] = a[m][k];
                a[m][k] = b;			}
        }
        for(int r = j;r < n + 1;r++){
            a[j][r] = a[j][r] / max;//让该行的所在列除以所在列的第一个元素，目的是让首元素为1
        }
        for(i = j + 1;i < n; i++){
            double c = a[i][j];
            if(c == 0)  continue;
            for(int s = j;s < n + 1;s++){
                //double tempdata = a[i][s];
                a[i][s] = a[i][s] - a[j][s] * c;//前后行数相减，使下一行或者上一行的首元素为0
            }
        }
    }
    for(i = n - 2; i >= 0; i--){
        for(j = i + 1;j < n; j++){
            a[i][n] = a[i][n] - a[j][n] * a[i][j];
        }
    }

    for(int k = 0; k < n; k++){
        recv[k] = a[k][n];
    }

}
