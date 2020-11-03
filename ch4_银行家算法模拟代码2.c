#include<stdio.h>  
  
#define PROCESS_AMOUNT 5     //进程数量  
#define RESOURCE_AMOUNT 3    //资源数量  
  
//系统提供的各类资源的最大量  
int Resoure_Max[3]={10,5,7};       
  
//各个进程需要的最大资源数量  
int Max[PROCESS_AMOUNT][RESOURCE_AMOUNT]={  
    {7,5,3},  
    {3,2,2},  
    {9,0,2},  
    {2,2,2},  
    {4,3,3},  
};      
  
//以分配给每个进程的资源数量        
int Allocation[PROCESS_AMOUNT][RESOURCE_AMOUNT]={  
    {0,1,0},  
    {2,0,2},  
    {3,0,2},  
    {2,1,1},  
    {0,0,2},  
};      
  
//各个进程还需要的资源数量  
int Need[PROCESS_AMOUNT][RESOURCE_AMOUNT];      
  
//工作向量，表示系统可提供的资源数量        
int Work[RESOURCE_AMOUNT];   
  
//记录各个进程是否完成，0表示未完成，1表示完成                          
int Finish[PROCESS_AMOUNT]={0};      
  
//记录各个进程执行完成的顺序                  
int Sort[PROCESS_AMOUNT];                           
  
  
//判断是否安全，即寻找安全序列  
int is_safe();  
  
//在Need矩阵中查找是否所需的各个资源数量都小于Work数组的资源数量，查找成功返回下标，不成功返回一个负数  
int search_need();  
  
void main()    
{  
    int i,j;  
    Work[0]=Resoure_Max[0];  
    Work[1]=Resoure_Max[1];  
    Work[2]=Resoure_Max[2];  
  
    for(i=0;i<PROCESS_AMOUNT;i++)  
        for(j=0;j<RESOURCE_AMOUNT;j++){  
           Need[i][j]=Max[i][j]-Allocation[i][j];  
           if(j==0)  
               Work[j]-=Allocation[i][j];  
           else if(j==1)  
               Work[j]-=Allocation[i][j];  
           else if(j==2)  
               Work[j]-=Allocation[i][j];  
        }  
    if(is_safe()){  
        printf("找到这样的安全系列：\n");  
        for(i=0;i<PROCESS_AMOUNT;i++)  
            printf("%-3d",Sort[i]);  
        printf("\n");  
    }else  
        printf("此时刻不安全！！\n");  
}  
  
//判断是否安全，即寻找安全序列  
int is_safe()  
{  
    int  i,index,j;  
    for(i=0;i<PROCESS_AMOUNT;i++){  
        index=search_need();  
        if(index<0)  
            return 0;  
        else{  
            for(j=0;j<RESOURCE_AMOUNT;j++)  
               Work[j]+=Allocation[index][j];  
            Finish[index]=1;  
            Sort[i]=index;  
        }  
    }  
    return 1;  
}  
  
//在Need矩阵中查找是否所需的各个资源数量都小于Work数组的资源数量，查找成功返回下标，不成功返回一个负数  
int search_need()  
{  
    int i,j;  
    for(i=0;i<PROCESS_AMOUNT;i++){  
        if(Finish[i]==1)  
            continue;  
        for(j=0;j<RESOURCE_AMOUNT;j++)  
            if(Need[i][j]>Work[j])  
                break;  
        if(j==RESOURCE_AMOUNT)  
            return i;  
    }  
    return -1;  
} 
