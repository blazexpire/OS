#include<stdio.h>  
  
#define PROCESS_AMOUNT 5     //��������  
#define RESOURCE_AMOUNT 3    //��Դ����  
  
//ϵͳ�ṩ�ĸ�����Դ�������  
int Resoure_Max[3]={10,5,7};       
  
//����������Ҫ�������Դ����  
int Max[PROCESS_AMOUNT][RESOURCE_AMOUNT]={  
    {7,5,3},  
    {3,2,2},  
    {9,0,2},  
    {2,2,2},  
    {4,3,3},  
};      
  
//�Է����ÿ�����̵���Դ����        
int Allocation[PROCESS_AMOUNT][RESOURCE_AMOUNT]={  
    {0,1,0},  
    {2,0,2},  
    {3,0,2},  
    {2,1,1},  
    {0,0,2},  
};      
  
//�������̻���Ҫ����Դ����  
int Need[PROCESS_AMOUNT][RESOURCE_AMOUNT];      
  
//������������ʾϵͳ���ṩ����Դ����        
int Work[RESOURCE_AMOUNT];   
  
//��¼���������Ƿ���ɣ�0��ʾδ��ɣ�1��ʾ���                          
int Finish[PROCESS_AMOUNT]={0};      
  
//��¼��������ִ����ɵ�˳��                  
int Sort[PROCESS_AMOUNT];                           
  
  
//�ж��Ƿ�ȫ����Ѱ�Ұ�ȫ����  
int is_safe();  
  
//��Need�����в����Ƿ�����ĸ�����Դ������С��Work�������Դ���������ҳɹ������±꣬���ɹ�����һ������  
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
        printf("�ҵ������İ�ȫϵ�У�\n");  
        for(i=0;i<PROCESS_AMOUNT;i++)  
            printf("%-3d",Sort[i]);  
        printf("\n");  
    }else  
        printf("��ʱ�̲���ȫ����\n");  
}  
  
//�ж��Ƿ�ȫ����Ѱ�Ұ�ȫ����  
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
  
//��Need�����в����Ƿ�����ĸ�����Դ������С��Work�������Դ���������ҳɹ������±꣬���ɹ�����һ������  
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
