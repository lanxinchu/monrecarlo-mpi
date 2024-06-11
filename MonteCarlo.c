#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<mpi.h>

long long int get_num(long long int n)
{
    double x,y,d;   //模拟生成的点(x,y)，以及该点到原点的距离d
    long long int num = 0, i;   //落入四分之一圆的点数
    srand(time(NULL));   //随机种子
    for(i=0; i<n; i++){
        x=(double)rand()/(double)RAND_MAX;
        y=(double)rand()/(double)RAND_MAX;
        d=x*x+y*y;
        if(d<=1)
            num += 1;
    }
    return num;
}

int main(int argc,char** argv){
    long long int n;
    int size,rank;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Status status;
    MPI_Init(&argc,&argv); //初始化
    MPI_Comm_size(comm,&size);  //进程总数
    MPI_Comm_rank(comm,&rank);  //当前进程标识
    sscanf(argv[1],"%lld",&n);  //通过参数获取一共模拟多少个点

    clock_t startTime, endTime;   //并行时间
    startTime=clock();
    MPI_Bcast(&n,1,MPI_LONG_LONG,0,comm);   //将总数广播到每个进程
    
    long long int Num, num;   //所有进程的落入四分之一圆区域的总点数与每个进程的落入四分之一圆区域的总点数
    num = get_num(n/size);  //计算每个进程能均分的点数
    MPI_Barrier(comm);
    MPI_Reduce(&num,&Num,1,MPI_LONG_LONG,MPI_SUM,0,comm);   //将每个进程计算的结果求和
    if(rank==0){
        if(num%size != 0)   //根进程计算不能整除的点数
            Num += get_num(n%size);
        double pi=(double)Num/(double)n*4;   //根据开头分析的公式计算圆周率
        endTime=clock();
        printf("pi=%lf\n",pi);
        printf("并行计算时间time=%ds\n",(int)(endTime-startTime)/1000/size);  //输出并行时间
    }
    
    MPI_Finalize();
    return 0;
}

