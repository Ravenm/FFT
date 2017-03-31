/*
	Austin Cullar
	Andrew Nash
	FFT
	HPC - Colmenares
	This program will use a Radix 2 Fast Fourier Transform to
	compute real and imaginary components for each N
	README:
		The algorithm is transcribed directly from lecture 19 on D2L.
		I used a constant approximation of pi since one didn't seem to exist in c already.

		The even and odd functions *could* be combined with the use of a boolean, but I
		thought leaving them seperate might more easily lend the program to parallelization.
		The algorithm works by sending the full array to both functions, allowing each to compute
		their own portion of the array. The result is then sent back in the form of a complex number.
		The last step involves adding the parts together to achieve the final result.
*/

#include<stdio.h>
#include<math.h>

#define N 8 //not sure if this is to be hard coded or not
#define pi 3.141593

/*
	struct to be used to represent complex numbers
*/
struct complex{
    double real;
    double imag;
};

/*
	This function was written in reference to Lecture 19
	The following is to represent the calculation of the even
	part of the FFT.
*/
struct complex evenFFT(struct complex x[N], int k){
    int i;
    struct complex result = {0, 0}, r;
    for(i=0; i<=(N/2)-1; i++){
        r.real = cos((2*pi*((2*i)+1)*k)/N);
        r.imag = -sin((2*pi*((2*i)+1)*k)/N);
        //multiply (for debugging, change '+=' to '=' to see intermediate calculations for n(i))
        result.real += (x[2*i].real*r.real - x[2*i].imag*r.imag);
        result.imag += (x[2*i].imag*r.real + x[2*i].real*r.imag);
    }
    return result;
}

/*
	This function will be used to compute the odd part of
	the FFT.
*/
struct complex oddFFT(struct complex x[N], int k){
    int i;
    struct complex result = {0, 0}, r;
    for(i=0; i<=(N/2)-1; i++){
        r.real = cos((2*pi*((2*i)+1)*k)/N);
        r.imag = -sin((2*pi*((2*i)+1)*k)/N);
        //multiply
        result.real += (x[2*i+1].real*r.real - x[2*i+1].imag*r.imag);
        result.imag += (x[2*i+1].imag*r.real + x[2*i+1].real*r.imag);
    }
    return result;
}

struct complex twiddleFact(int k)
{
    struct complex result;
    result.real = cos(2*pi*k/N);
    result.imag = -sin(2*pi*k/N);

    return result;
}

int main(){
    FILE *fp;
    fp = fopen("FFTdata.dat", "r");
    struct complex x[N], odd, even, twiddle;
    struct complex out[N];
    int i =0;

    //add data from file:
    while(!feof(fp)){
        fscanf(fp, "%lf%lf", &x[i].real, &x[i].imag);
        i++;
    }
    while(i < N)
    {
        x[i].real = 0;
        x[i].imag = 0;
        i++;
    }
    //computation
    for(int k = 0; k < (N/2); k++)
    {
        even = evenFFT(x, k); //even part
        odd = oddFFT(x, k); //odd part
        twiddle = twiddleFact(k);

        out[k].real = even.real + (twiddle.real * odd.real);
        out[k].imag = even.imag + (twiddle.imag * odd.imag);

        out[k + (k/N)].real = even.real + (twiddle.real * odd.real);
        out[k + (k/N)].imag = even.imag - (twiddle.imag * odd.imag);

    }

    for(int j = 0; j < N; j++)
    {
        printf("FINAL: %lf + %lfi\n", out[j].real, out[j].imag);
    }

    return 0;
}