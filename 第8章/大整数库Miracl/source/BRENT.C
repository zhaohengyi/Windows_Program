/*
 *   Program to factor big numbers using Brent-Pollard method.
 *   See "An Improved Monte Carlo Factorization Algorithm"
 *   by Richard Brent in BIT Vol. 20 1980 pp 176-184
 *
 *   Copyright (c) 1988-1997 Shamus Software Ltd.
 */

#include <stdio.h>
#include <miracl.h>

#define mr_min(a,b) ((a) < (b)? (a) : (b))

int main()
{  /*  factoring program using Brents method */
    long k,r,i,m,iter;
    big x,y,z,n,q,ys,c3;
    char *mem;
#ifndef MR_NOFULLWIDTH
    mirsys(50,0);
#else
    mirsys(50,MAXBASE);
#endif

    mem=memalloc(7);      /* allocate space for 7 bigs */
    x=mirvar_mem(mem,0);  /* bigs have index from 0-6  */
    y=mirvar_mem(mem,1);
    ys=mirvar_mem(mem,2);
    z=mirvar_mem(mem,3);
    n=mirvar_mem(mem,4);
    q=mirvar_mem(mem,5);
    c3=mirvar_mem(mem,6);
    convert(3,c3);

    printf("input number to be factored\n");
    cinnum(n,stdin);
    if (isprime(n))
    {
        printf("this number is prime!\n");
        return 0;
    }
    m=10L;
    r=1L;
    iter=0L;
    do
    {
        printf("iterations=%5ld",iter);
        convert(1,q);
        do
        {
            copy(y,x);
            for (i=1L;i<=r;i++)
                mad(y,y,c3,n,n,y);
            k=0;
            do
            {
                iter++;
                if (iter%10==0) printf("\b\b\b\b\b%5ld",iter);
                fflush(stdout);  
                copy(y,ys);
                for (i=1L;i<=mr_min(m,r-k);i++)
                {
                    mad(y,y,c3,n,n,y);
                    subtract(y,x,z);
                    mad(z,q,q,n,n,q);
                }
                egcd(q,n,z);
                k+=m;
            } while (k<r && size(z)==1);
            r*=2;
        } while (size(z)==1);
        if (compare(z,n)==0) do 
        { /* back-track */
            mad(ys,ys,c3,n,n,ys);
            subtract(ys,x,z);
        } while (egcd(z,n,z)==1);
        if (!isprime(z))
             printf("\ncomposite factor ");
        else printf("\nprime factor     ");
        cotnum(z,stdout);
        if (compare(z,n)==0) return 0;
        divide(n,z,n);
        divide(y,n,n);
    } while (!isprime(n));
    printf("prime factor     ");
    cotnum(n,stdout);
    memkill(mem,7);     /* delete all 7 bigs */
    return 0;
}

