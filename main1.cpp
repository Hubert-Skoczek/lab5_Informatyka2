
#include "math.h"
#include "stdio.h"
#include "winbgi2.h"
#include "stdlib.h"
#include "rk4.h"
double g=9.81;//zmienna globalna przyciągania ziemskiego
double l=1;//zmienna globalna długości wahadła
void rhs_fun(double t,double *X, double *F);
void veuler(double t, double *X, double h, int n,void (* rhs_fun)(double, double *,double *), double *X1);
void main()
{
	//
	double a0=60./180*3.14159;//warunek poczatkowy alfa0
	double w0=1;//warunek poczatkowy omega0
	double t=0;
	double X[2]={a0,w0};
	double F[2];
	double tk=10;//ostatnia wartość zmiennej niezależnej t
	double h=0.01;//krok czasowy	
	double X1[2];
	double E;
	double m=2;//masa
	int n=2;
	FILE* f;//otwarcie pliku z wynikami dla metody Eulera
	FILE* i;//otwarcie pliku z wynikami dla metody RK4
	FILE* q;//otwarcie pliku z wynikami Energii
	//
	graphics(800,800);
	scale(-a0-3,-w0-5,a0+3,w0+5);//wyskalowane dla danych warunków początkowych
	title("alfa(omega) zielony - euler, niebieski - vrk4","","");
	f=fopen("wyniki_euler.txt","w");
	i=fopen("wyniki_rk4.txt","w");
	q=fopen("energia.txt","w");
	//METODA EULERA
	fprintf(f,"t\t omega\t\t alfa\n");
	for(t;t<=tk;t+=h)
	{
		fprintf(f,"%.2lf\t",t);
		veuler(t,X,h,n,rhs_fun,X1);//obliczenia wykonywanie metodą Eulera
		if(X1[1]<0)//pętla stworzona po to aby wyniki w plikach zapisywały się równo w jednej kolumnie
		{
			fprintf(f," %lf\t",X1[1]);
		}else{
			fprintf(f," %lf\t\t",X1[1]);
		}
		fprintf(f," %lf\n",X1[0]);
		setcolor(0.5);//zielony
		circle(X1[0],X1[1],1);//wykres alfa(omega) dla metody Eulera
		X[0]=X1[0];
		X[1]=X1[1];
	}
	X[0]=a0;
	X[1]=w0;
	t=0;
	//METODA RK4 oraz ENERGIA
	fprintf(i,"t\t omega\t\t alfa\n");
	fprintf(q,"t\t energia\n");
	for(t;t<=tk;t+=h)
	{
		fprintf(i,"%.2lf\t",t);
		fprintf(q,"%.2lf\t",t);
		vrk4(t,X,h,n,rhs_fun,X1);//obliczenia wykonywanie metodą Eulera
		E=m*pow(l,2)/2*pow(X1[1],2)+m*g*l*(1-cos(X1[0]));//obliczanie Energii zgodnie z wzorem dla wartości obliczonych metodą RK4
		fprintf(q,"%lf\n",E);
		if(X1[1]<0)
		{
			fprintf(i," %lf\t",X1[1]);
		}else{
			fprintf(i," %lf\t\t",X1[1]);
		}
		fprintf(i," %lf\n",X1[0]);
		setcolor(0.0);//niebieski
		circle(X1[0],X1[1],1);//wykres alfa(omega) dla metody Eulera
		X[0]=X1[0];
		X[1]=X1[1];
	}
	//
	fclose(f);
	fclose(i);
	fclose(q);
	wait();
}
void rhs_fun(double t,double *X, double *F)
{
	F[1]=-g/l*sin(X[0]);//funkcja oblicz prawe strony równania dw/dt=-g/l*sin(a)
	F[0]=X[1];//da/dt/w
}
void veuler(double t, double *X, double h, int n,void (* rhs_fun)(double, double *,double *), double *X1)
{
	double F[2];//metoda Eulera zgodnie z algorytmem zawartym w instrukcji
	rhs_fun(t,X,F);
	X1[1]=X[1]+h*F[1];//w(t(i+1))=w(t(i))+h*F1(ai,wi,ti)
	X1[0]=X[0]+h*F[0];//a(t(i+1))=a(t(i))+h*F2(ai,wi,ti)
}

