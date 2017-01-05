#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <iostream>
using namespace std;

const short C[]={30,50,80,100}; 

long	n;
long	m;
short	step;
clock_t	start,
		finish;

FILE	*fp=fopen("QS4.txt","wt+");

short get_C(long n)
{
	if		(n<=10)
		return (n>8)?8:n;
	else if (n<100)
		return n;
	else if (n<1000)
		return C[0];
	else if (n<10000)
		return C[1];
	else if (n<100000)
		return C[2];
	else
		return C[3];
}

int main()
{
	//=================================
	clock_t totalStart,totalFinish;
	double totalTime;
	totalStart=clock();
	//=================================



	printf("How many queens?\nn=");
	scanf("%ld",&n);
	const char TIMES = 1;
	double totaltime,sumtime = 0;
	void QS4();
	m = n - get_C(n);

	fprintf(fp,"THE NUM OF QUEENS=%ld ([row][column])", n);
	for(char run = 0; run < TIMES; run++)
	{
	fprintf(fp,"\nTHE %dth TRAIL\n",run+1);
	start = clock();
	printf("\ntimes = %d\nthe number of queens = %ld\nqueens with conflict free = %ld\n", run+1, n, m);

		QS4();

	finish = clock();
	totaltime	=	(double)(finish - start)/CLOCKS_PER_SEC;
	sumtime		+=	totaltime;
	printf( "total time = %-5.3g\n", totaltime);
	printf( "total step = %d\n", step);

	}//end for times

	std::cout<<"=======================================\nAverage time:"<<sumtime/TIMES<<std::endl;

	fclose(fp);

	totalFinish=clock();
	totalTime=(double)(totalFinish-totalStart);
	cout<<"======================================="<<endl;
	cout<<"Total running time: "<<totalTime/CLOCKS_PER_SEC<<endl;
	cout<<"======================================="<<endl;



	return 0;
}

unsigned long RandSeed = (unsigned)time(NULL);
unsigned long BRandom(long max) // random positions
{
	unsigned long x ;
	double i ;
	
	x = 0x7fffffff;
	x += 1;
	RandSeed *= ((unsigned long)134775813);
	RandSeed += 1;
	RandSeed = RandSeed % x;
	i = ((double)RandSeed) / (double)0x7fffffff;
	
	return (unsigned long)(max * i);
}

void swap(long &a, long &b)
{
	if (a!=b)
	{
		long t;
		t = a;
		a = b;
		b = t;
	}
}

void init_4(long queen[], long n, long m, long b[], long c[])
{
	long i,last;
	long z;
	bool *bb = (bool*)malloc(sizeof(bool) * (n+n));
	bool *cc = (bool*)malloc(sizeof(bool) * (n+n));

	for( i = 0; i < n; i++ )
	{
		b[i] = 0;
		c[i] = 0;
		bb[i] = false;
		cc[i] = false;
		queen[i] = i;
	}

	for( i = n; i < n+n; i++ )
	{
		b[i] = 0;
		c[i] = 0;
		bb[i] = false;
		cc[i] = false;
	}


	for( i = 0,last = n; i < m; i++,last--)		//n-c queens without conflict
	{
		do
		{
			z = i + BRandom(last);//BRandom: an random integer in [0,last)
		}
		while ( bb[i-queen[z]+n-1] || cc[i+queen[z]] );//there is no conflict on diagonal

		swap(queen[i], queen[z]);
		b[i-queen[i]+n-1]++;	//leading digonal
		c[i+queen[i]]++;	//counter digonal
		bb[i-queen[i]+n-1] = true;
		cc[i+queen[i]] = true;
	}


	for( i = m, last = n-m; i < n; i++, last--)	//c possible conflicts
	{
		z = i + BRandom(last);
		swap(queen[i], queen[z]);
		b[i-queen[i]+n-1]++;
		c[i+queen[i]]++;
	}

	free(bb);
	free(cc);
}

long sum(const long queen[], const long n, const long b[], const long c[])// the number of conflicts
{
	long ans = 0;
	long i;

	for (i = 0; i < n+n; i++)
	{
		if (b[i] > 1)
			ans += b[i] * (b[i]-1)/2;// on the diagonal C_b[i]_2
		if (c[i] > 1)
			ans += c[i] * (c[i]-1)/2;
	}

	return ans;
}


long delta(long i, long j, long b[], long c[], const long q[]/*queen*/, long n)
{
	long ans = 0;

	ans += 1 - b[i-q[i]+n-1];	//leading diagonal
	ans += 1 - c[i+q[i]];		//counter diagonal
	ans += 1 - b[j-q[j]+n-1];
	ans += 1 - c[j+q[j]];

	ans +=	b[j-q[i]+n-1];		//if j changes with i		
	ans +=	c[j+q[i]];
	ans +=	b[i-q[j]+n-1];
	ans +=	c[i+q[j]];

	if ( (i+q[i]==j+q[j]) || (i-q[i]==j-q[j]) )// the same diagonal
		ans += 2;
	return ans;
}

void QS4()
{
	long t;		//current conflicts
	long temp;	//delta conflicts
	long i,j;

    long* queen = (long*)malloc( sizeof(long) * n);
	long* b = (long*)malloc( sizeof(long) * (n+n) );
	long* c = (long*)malloc( sizeof(long) * (n+n) );

	init_4(queen,n,m,b,c);
	finish = clock();
	printf("init time  = %-5.3g\n",(double)(finish - start) / CLOCKS_PER_SEC);

	t = sum(queen,n,b,c);
	step = 0;
	printf("conflicts = %ld\n",t);

	while (t>0)
	{
		temp = 0;
		for (i = m; i < n; i++)		//only try c times, or reinitialize the positions
		{
		if ((b[i-queen[i]+n-1]==1) && (c[i+queen[i]]==1)) continue;	//no conflicts on diagonal
		else
			{
			for (j = 0; j < n; j++)
			if(i!=j)
			{
				temp = delta(i,j,b,c,queen,n);		//if delta<0, we can minimize the conflicts
				if (temp < 0) break;
			}
			if (temp < 0) break;
			}
		}
		if (temp < 0)
		{
			step++;
			b[i-queen[i]+n-1]--;
			c[i+queen[i]]--;
			b[j-queen[j]+n-1]--;
			c[j+queen[j]]--;

			swap(queen[i], queen[j]);

			b[i-queen[i]+n-1]++;
			c[i+queen[i]]++;
			b[j-queen[j]+n-1]++;
			c[j+queen[j]]++;

			t += temp;
		}		
		else		//if from m to n, all the i,q[i] have delta > 0, we shuffle the positions again
		{
			finish = clock();
			printf("time= %-5.3g\n",(double)(finish - start) / CLOCKS_PER_SEC);
			printf("ReInit...\n");
			init_4(queen,n,m,b,c);
			finish = clock();
			printf("init time  = %-5.3g\n",(double)(finish - start) / CLOCKS_PER_SEC);
			t = sum(queen,n,b,c);
			printf("confilicts = %ld\n",t);
		}
	}//end while
	
	for (i = 0; i < n; i++)
		{fprintf(fp, "[%ld][%ld]\t\t", i+1,queen[i]+1);}
	fprintf(fp,"\n");

	free(queen);
	free(b);
	free(c);
}
