#include <stdio.h>
#include <math.h>

double f(double x){
    return (2.5*x*x-0.1)/(tan(x)+sin(x));
}

int main(){
    double a=4;
    double b=6;
    int arrayN[3]={10,100,1000};
    for(int i=0;i<3;i++){
        int N=arrayN[i];
        double h=(b-a)/N;
        double aArray[N+1];
        int k=0;
        for(int k=0; k<=N;k++){
            double x=a+k*h;
            aArray[k]=f(x);
        }
        double I1=aArray[0]/2;
        double I2=aArray[0]+aArray[N];
        for(int i=1;i<N;i++){ 
            I1+=aArray[i];

            
        for(int j = 1; j < N; j += 2) {
            I2+=4 * aArray[j];
        }
        for(int j = 2; j < N; j += 2) {
            I2+=2 * aArray[j];
        }
            }
        I1+=aArray[N]/2;
        I1*=h;
        I2*=(h/3);

    printf("Для N= %d ,I1=%f, I2=%f\n",N,I1,I2);
    } 
    return 0;
}