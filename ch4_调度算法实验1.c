/* ʵ��Ŀ����Ҫ��
   ����C���ԣ�����ģ��OS���̵��Ⱥ͹������Ҫ��������������Ļ���������OS���̵��Ȼ���ԭ��
*/
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
/*********************������ȫ�����ݽṹ�ͱ���***********************/
/*PCB �ṹ*/
struct PCB{
  int pname;
  int pri;
  int runtime;
  int waittime;
  struct PCB *next;
} pcb[7];
/* ����ָ��*/
struct PCB *running;
/*����du�ȼ���������ͷָ��*/
struct PCB *Hready;
/*�����ȼ�����ͷָ��*/
struct PCB *Lready;
/*�ȴ�����ͷָ��*/
struct PCB *wait;
int sig=0;
/**************************�����Ǻ���˵��****************************/
/*����ѭ��ʵ���ӳ�*/
void delay();
/*ģ�����3��9*/
void proc(struct PCB *running);
/*��node���뵽head��ָʾ�Ķ��е�β��*/
void InsertIntoQueueTail(struct PCB ** head,struct PCB *node);
/*���̵��Ⱥ���*/
int proc_switch();
/*���̵ȴ�����*/
void proc_wait();
/*���̻��Ѻ���*/
int proc_wakeup();
/************************�����Ǻ������弰ע��************************/
/*������*/
main() {
	int i;
	/*��ʼ������������3��9���õ����ȼ����ȴ�ʱ��Ϊ0��
	���β�������ȼ�����*/
	for(i = 0;i < 7;i++){
		pcb[i].pname = i+3;
		pcb[i].pri = 0;
		pcb[i].waittime = 0;
		InsertIntoQueueTail(&Lready,&pcb[i]);
    }
	/*�ȴ����к͸����ȼ�����Ϊ��*/
	wait = NULL;
	Hready=NULL;
	printf("\nThe process_switch begin:\n");
	/*ģ����̵��ȿ�ʼ*/
	for(;;) 	{
	switch(sig){
		case 0:/*�޽��̵ȴ����ȣ���ӡ��Ϣ������*/
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
/*���ܣ��ӳ�һ��ʱ��Ƭ*/
/*��ڲ�������*/
/*���ڲ�������*/
void delay() {
	int i,j;
	for(i=0;i<20000;i++)
	for(j=0;j<10000;j++) { 	}
}
/*���ܣ�����3��9*/
/*��ڲ���������ָ��*/
/*���ڲ�������*/
void proc(struct PCB * running)  {
	int i;
	srand( (unsigned)time( NULL ) );
	/*��ʾ��ǰ���еĽ��̵�id*/
	printf("\nNow Process %d is running\n",running->pname);
	/*��ǰ����ִ��running->runtime��ʱ��Ƭ*/
	for(i=running->runtime;i>0;i--){
		/*��ʾʣ���ʱ��Ƭ*/
		printf("%d time slice(s) left\n",i);
		/*�ӳ�*/
		delay();
		proc_wakeup();
		/*����һ��1��1000������������������С��100����ǰ���̵ȴ���*/
		if((rand()%1000+1)<100){
			printf("Process %d begins to wait.\n",running->pname);
			sig=1;
			return;
		}
	}
	/*��ʾʱ��Ƭ�ľ�������תΪ�����ȼ�����״̬*/
	printf("Time slices for process %d exhausted.\n",running->pname);
	InsertIntoQueueTail(&Hready,running);
	sig=0;
	return;
}
/*���ܣ���һ���ڵ�������β��*/
/*��ڲ���������ͷָ���ַhead����������node*/
/*���ڲ�������*/
void InsertIntoQueueTail(struct PCB **head,struct PCB *node) {
	struct PCB *p;
	node->next=NULL;
	/*���������Ϊ��*/
	if(*head==NULL){
		*head=node;
		return;
	} else {/*��������в�Ϊ��*/
		p=*head;
		/*�ҵ����е����һ�����*/
		while(p->next!=NULL) p=p->next;
		p->next=node;
	}
}
/*���ܣ����̵���*/
/*��ڲ�������*/
/*���ڲ����������ȳɹ�������1�����򷵻�0*/
int proc_switch() {
	/*�������ȼ��������к͵����ȼ��������о�Ϊ�գ���ѭ��ִ�н��̻���*/
	while(Hready == NULL && Lready == NULL)
	if(!proc_wakeup()) return 0;
	/*�������ȼ��������зǿգ���ִ�����һ�����̣�����2��ʱ��Ƭ*/
	if(Hready != NULL){
		running = Hready;
		Hready = Hready -> next;
		running->runtime = 2;
	} else{  /*�������ȼ���������Ϊ�գ���ִ�е����ȼ��������еĵ�һ�����̣�
 	            ����5��ʱ��Ƭ*/
		running = Lready;
		Lready=Lready -> next;
		running -> runtime = 5;
	}
	/*����Ƚ��̵�id����sig*/
	sig = running -> pname;
	return 1;
}
/*���ܣ����̵ȴ�������ǰ���н����ø����ȼ����ȴ�ʱ��Ϊ20��
����ȴ�����β��*/
/*��ڲ�������*/
/*���ڲ�������*/
void proc_wait() {
	struct PCB *p;
	running->pri=1;
	running->waittime=20;
	InsertIntoQueueTail(&wait,running);
	sig=0;
	return;
}
/*���ܣ����̻���*/
/*��ڲ�������*/
/*���ڲ��������ȴ�����Ϊ�գ��򷵻�0�����򷵻�1*/
int proc_wakeup() {
	struct PCB *p,*last,*MoveToReady;
	p = wait;
	/*�ȴ�����Ϊ�գ�����0*/
	if(p == NULL) return 0;
	/*�ӳ�*/
	delay();
	/*�ȴ�������ÿ�����̵ĵȴ�ʱ���1*/
	while(p != NULL){
		p -> waittime -= 1;
		p=p->next;
	}
	p=wait;
	/*�ӵȴ�������ժ���ȴ�ʱ��Ϊ0�Ľ��̣����뵽�����ȼ��������е�β��*/
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
