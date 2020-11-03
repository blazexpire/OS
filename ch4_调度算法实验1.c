/* 实验目的与要求：
   运用C语言，开发模拟OS进程调度和管理程序，要求熟练掌握链表的基本操作和OS进程调度基本原理。
*/
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
/*********************以下是全局数据结构和变量***********************/
/*PCB 结构*/
struct PCB{
  int pname;
  int pri;
  int runtime;
  int waittime;
  struct PCB *next;
} pcb[7];
/* 运行指针*/
struct PCB *running;
/*高优du先级就绪队列头指针*/
struct PCB *Hready;
/*低优先级队列头指针*/
struct PCB *Lready;
/*等待队列头指针*/
struct PCB *wait;
int sig=0;
/**************************以下是函数说明****************************/
/*利用循环实现延迟*/
void delay();
/*模拟进程3－9*/
void proc(struct PCB *running);
/*将node插入到head所指示的队列的尾部*/
void InsertIntoQueueTail(struct PCB ** head,struct PCB *node);
/*进程调度函数*/
int proc_switch();
/*进程等待函数*/
void proc_wait();
/*进程唤醒函数*/
int proc_wakeup();
/************************以下是函数定义及注释************************/
/*主函数*/
main() {
	int i;
	/*初始化，创建进程3－9，置低优先级，等待时间为0，
	依次插入低优先级队列*/
	for(i = 0;i < 7;i++){
		pcb[i].pname = i+3;
		pcb[i].pri = 0;
		pcb[i].waittime = 0;
		InsertIntoQueueTail(&Lready,&pcb[i]);
    }
	/*等待队列和高优先级队列为空*/
	wait = NULL;
	Hready=NULL;
	printf("\nThe process_switch begin:\n");
	/*模拟进程调度开始*/
	for(;;) 	{
	switch(sig){
		case 0:/*无进程等待调度，打印信息并返回*/
			if(!proc_switch()) {
				printf("No Process to run,press any key to return:\n");
				getchar();
			}
			break;
		case 1:proc_wait();
			break;
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:proc(running);
		break;
		default:printf("\nerror!");
		exit(-1);
    }
    }
}
/*功能：延迟一个时间片*/
/*入口参数：无*/
/*出口参数：无*/
void delay() {
	int i,j;
	for(i=0;i<20000;i++)
	for(j=0;j<10000;j++) { 	}
}
/*功能：进程3－9*/
/*入口参数：运行指针*/
/*出口参数：无*/
void proc(struct PCB * running)  {
	int i;
	srand( (unsigned)time( NULL ) );
	/*显示当前运行的进程的id*/
	printf("\nNow Process %d is running\n",running->pname);
	/*当前进程执行running->runtime个时间片*/
	for(i=running->runtime;i>0;i--){
		/*显示剩余的时间片*/
		printf("%d time slice(s) left\n",i);
		/*延迟*/
		delay();
		proc_wakeup();
		/*产生一个1到1000的随机数，若该随机数小余100，当前进程等待，*/
		if((rand()%1000+1)<100){
			printf("Process %d begins to wait.\n",running->pname);
			sig=1;
			return;
		}
	}
	/*显示时间片耗尽，进程转为低优先级就绪状态*/
	printf("Time slices for process %d exhausted.\n",running->pname);
	InsertIntoQueueTail(&Hready,running);
	sig=0;
	return;
}
/*功能：将一个节点插入队列尾部*/
/*入口参数：队列头指针地址head，待插入结点node*/
/*出口参数：无*/
void InsertIntoQueueTail(struct PCB **head,struct PCB *node) {
	struct PCB *p;
	node->next=NULL;
	/*被插入队列为空*/
	if(*head==NULL){
		*head=node;
		return;
	} else {/*被插入队列不为空*/
		p=*head;
		/*找到队列的最后一个结点*/
		while(p->next!=NULL) p=p->next;
		p->next=node;
	}
}
/*功能：进程调度*/
/*入口参数：无*/
/*出口参数：若调度成功，返回1，否则返回0*/
int proc_switch() {
	/*若高优先级就绪队列和低优先级就绪队列均为空，则循环执行进程唤醒*/
	while(Hready == NULL && Lready == NULL)
	if(!proc_wakeup()) return 0;
	/*若高优先级就绪队列非空，则执行其第一个进程，分配2个时间片*/
	if(Hready != NULL){
		running = Hready;
		Hready = Hready -> next;
		running->runtime = 2;
	} else{  /*若高优先级就绪队列为空，则执行低优先级就绪队列的第一个进程，
 	            分配5个时间片*/
		running = Lready;
		Lready=Lready -> next;
		running -> runtime = 5;
	}
	/*别调度进程的id赋给sig*/
	sig = running -> pname;
	return 1;
}
/*功能：进程等待。将当前运行进程置高优先级，等待时间为20，
插入等待队列尾部*/
/*入口参数：无*/
/*出口参数：无*/
void proc_wait() {
	struct PCB *p;
	running->pri=1;
	running->waittime=20;
	InsertIntoQueueTail(&wait,running);
	sig=0;
	return;
}
/*功能：进程唤醒*/
/*入口参数：无*/
/*出口参数：若等待队列为空，则返回0，否则返回1*/
int proc_wakeup() {
	struct PCB *p,*last,*MoveToReady;
	p = wait;
	/*等待队列为空，返回0*/
	if(p == NULL) return 0;
	/*延迟*/
	delay();
	/*等待队列中每个进程的等待时间减1*/
	while(p != NULL){
		p -> waittime -= 1;
		p=p->next;
	}
	p=wait;
	/*从等待队列中摘除等待时间为0的进程，插入到高优先级就绪队列的尾部*/
	while(p!=NULL){
		if(p -> waittime == 0){
			MoveToReady = p;
			if (p == wait) 
				wait = p->next;
			else
				last -> next = p->next;
			p = p -> next;
			InsertIntoQueueTail(&Hready,MoveToReady);
		} else {
			p = p -> next;
		}
	}
	sig =0;
	return 1;
}
