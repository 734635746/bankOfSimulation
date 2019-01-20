#include<stdio.h>
#include<stdlib.h>
#include<time.h>

/**
	�ͻ�\�¼��ṹ�� 
*/ 
typedef struct event{
	int type;//�¼����͵�������뿪 0Ϊ���1Ϊ�뿪 
	int arriveTime;//����ʱ��
	int leaveTime;//�뿪ʱ��
	int money;//�����������Ϊ������Ϊȡ��
	int num;//�ͻ���� 
	event* next;
}event,*eventLink;
/**
	���нṹ�� 
*/
typedef struct QNode{
	eventLink front;//��ͷָ�� 
	eventLink rear; //��βָ�� 	
}QNode,*Queue;

int total_money;//�����ִ��
int closeTime;//���н���Ӫҵ��ʱ��
int arriveMaxTime;//���������¼���ʱ��������
int arriveMinTime; //���������¼���ʱ��������
int dealMaxTime;//�ͻ�����֮���ʱ������
int dealMinTime; //�ͻ�����֮���ʱ������
int MaxMoney;//���׶�����
int currentTime=0;//��ǰʱ��
int totalTime;//�ͻ�������ʱ��
int counter=0;//�ͻ�����
int number=1;//�ͻ���ʼ����
int flag=1;//�ж��Ƿ��д����ڴ���
int TimeOfDeal;//����ʱ��
int MaxTime;//����ʱ��
Queue Q_en;//�¼����� 
Queue Q1;//����һ 
Queue Q2;//���ж�

//���ض���Ԫ�� 
eventLink get_front(Queue &q){
	return q->front; 
}  

//��� 
void push(Queue &q,eventLink e){
	
	if(e==NULL){//���ָ��Ϊ�� 
		return;
	} 
	if(q->front==NULL){//����Ϊ�� 
		q->front=e;
		q->rear=e;
	}else{
		q->rear->next=e;
		q->rear=q->rear->next;
	}
	
} 
//���ٶ���
void destoryQueue(Queue &q){
	eventLink e,e1;
	e=q->front;
	if(q->front==q->rear==NULL){//�ӿ� 
		return;
	}
	while(e!=NULL){
		e1=e->next;
		free(e);
		e=NULL;
		e=e1;
	} 
	
} 

//ɾ������Ԫ�� 
void pop(Queue &q){
	eventLink e;
	e = q->front; 
	if(q->front==NULL){//����Ϊ�� 
		return;
	}else if(q->front->next==NULL){//����ֻ��һ��Ԫ�� 
		q->front=q->rear=NULL;
	}else{
		q->front=q->front->next;
	}
	
	if(e!=NULL){
			free(e);//�ͷſռ�  
         	e=NULL;
	}

	
} 

//��������,Ӫҵʱ�䵽 
void closeBank(){
	destoryQueue(Q1);
	destoryQueue(Q2);
	destoryQueue(Q_en);
} 
//���ﺯ������������˿ͽ������һͬʱ�����������¼� 
void arrive(){
	eventLink e,e1;
	e = (eventLink)malloc(sizeof(event));//����ռ� 
	e1 = (eventLink)malloc(sizeof(event));//�����
	if(e==NULL||e1==NULL){//����ռ�ʧ�� 
		return;
	}
	e1->money=e->money =  (rand()%(2*MaxMoney)-MaxMoney);//������׽��
	e1->arriveTime=e->arriveTime = currentTime;//��ǰʱ��Ϊ����ʱ��
	e1->num=e->num = number;
	e->next=NULL;
	e1->next=NULL;
	push(Q1,e);//�����һ����
	e1->type=0;//�����¼� 
	push(Q_en,e1);//�����¼�����
	number++; //�ͻ��������� 

}
//���� 
void putMoney(){
	
	eventLink e,e1;
	int temptime; 
	e1 = (eventLink)malloc(sizeof(event));//����ռ� 
	if(e1==NULL){//����ռ�ʧ�� 
		return;
	}
	e =  get_front(Q1);//��ͷԪ�� 
	total_money +=  e->money;//���
	e1->type=1;//�뿪�¼� 
	e1->num=e->num;
	e1->money=e->money;
	temptime=rand()%(dealMaxTime-dealMinTime+1)+dealMinTime;//������ɴ���ʱ�� 
	e1->leaveTime = (e->arriveTime+temptime);//�뿪ʱ��Ϊ����ʱ��+����ʱ��
	e1->next=NULL;
	push(Q_en,e1);//�뿪�¼�������� 
	counter++;
	totalTime+=e1->leaveTime-e->arriveTime;//ˢ�½���ʱ�� 
	pop(Q1);//Q1���� 
	TimeOfDeal=e1->leaveTime;//����ʱ��Ϊ�ͻ����뿪ʱ�� 
	flag=0;//����û�н�����Ҫ���� 
	
	
	 
} 

void getMoney(){//ȡ���
	 eventLink e,e1;
	 e1 = (eventLink)malloc(sizeof(event));//����ռ�
	 if(e1==NULL){//����ռ�ʧ�� 
		return;
	}
	 e =  get_front(Q1);//ȡQ1��ͷԪ�� 
	 
	 if((-(e->money))>total_money){//�ʽ���,����ڶ����еȴ� 
	 
	 	 e1->arriveTime = e->arriveTime;
	 	 e1->money = e->money;
	 	 e1->num = e->num;
	 	 e1->next=NULL;
	 	 push(Q2,e1);//�ͻ�����ڶ����� 
	 	 pop(Q1);//��Q1ɾ���ͻ� 
	 	
	 }else{//�ʽ��㹻��ִ��ȡ��
	 	
	 	total_money +=e->money;//ȡ�� 
		e1->type=1;//�뿪�¼� 
		e1->num=e->num;
		e1->money=e->money; 
		e1->leaveTime= (e->arriveTime+rand()%(dealMaxTime-dealMinTime+1)+dealMinTime);//�뿪ʱ��Ϊ����ʱ��+�������ʱ�� 
	 	e1->next=NULL;
		push(Q_en,e1);//�뿪�¼������¼����� 
	 	TimeOfDeal=e1->leaveTime;//����ʱ��Ϊ�ͻ����뿪ʱ��
	 	counter++;//�ͻ��������� 
	 	totalTime+=e1->leaveTime-e->arriveTime;//ˢ�¿ͻ��ܶ���ʱ�� 
	 	pop(Q1);//�ͻ��뿪 
		flag=0;
	 } 
	
}
//���������������������Ƿ��п��Դ����Ԫ�� 
eventLink search(Queue &q,int m){
	eventLink e,e1;
	e = q->front;
	while(q->front!=NULL){//�������� 
		if((-(q->front->money))<=total_money){//�ʽ���㣬��ǰ�ͻ���ȡ�� 
			if(q->front==q->rear){
				e1=q->front;
				q->front=q->rear=NULL;
				return e1;
			}else{
				e1=q->front;
				q->front=q->front->next;
				return e1;
			}	
		}else{//�ʽ��㣬������һ���ͻ� 
			if(q->front==q->rear){
				
			}else{
				q->rear->next=q->front;
				q->rear=q->rear->next;
				q->front=q->rear->next;
				q->rear->next=NULL;
			}
			
			
		}
		
		if(q->front==e){
				return NULL; 
			}
	}
	return NULL;
}
//���������ڶ�����Ѱ�ҿɴ����Ԫ�ز����� 
void findAndDeal(){
	eventLink e,e1;
	int randTemp;
	while((e=search(Q2,total_money))!=NULL){
		e1 = (eventLink)malloc(sizeof(event));//����ռ�
	 	if(e1==NULL){//����ռ�ʧ�� 
		return;
    	}
		total_money+=e->money;//ȡ��
		e1->type=1;//�뿪�¼� 
		e1->num=e->num;
		e1->money=e->money;
		randTemp = rand()%(dealMaxTime-dealMinTime+1)+dealMinTime;//�������ʱ��
		e1->leaveTime=currentTime+ randTemp;//�뿪ʱ��=��ǰʱ��+����ʱ��
		e1->next=NULL;
		push(Q_en,e1);//�����¼����� 
		TimeOfDeal+=randTemp;//ˢ�½���ʱ��
		counter++;//�ͻ��������� 
		totalTime+=e1->leaveTime-e->arriveTime;//ˢ���ܶ���ʱ�� 
		free(e);//�ͷſռ� 
		e=NULL; 
		
	} 
	flag=0;
}
//���г�ʼ�� 
void initQueue(){
	Q1 = (Queue)malloc(sizeof(QNode));
	Q2 = (Queue)malloc(sizeof(QNode));
	Q_en = (Queue)malloc(sizeof(QNode));
	if(Q1==NULL||Q2==NULL||Q_en==NULL){
		return;
	}
	Q1->front=Q1->rear=NULL;
	Q2->front=Q2->rear=NULL;
	Q_en->front=Q_en->rear=NULL;
}
int main(){
	printf("*************************************************\n");
	printf("*************************************************\n");
	printf("********  17���������ѧ�뼼����5����  **********\n");
	printf("********       ѧ�ţ�3117004622        **********\n");
	printf("********            ���ѱ�             **********\n");
	printf("*************************************************\n");
	printf("*************************************************\n");
	printf("*************************************************\n");
	printf("*************************************************\n");
	printf("********  ��ӭʹ������ģ��ϵͳ,��ѡ��  **********\n");
	printf("********          1.����ģ��           **********\n");
	printf("********            0.�˳�             **********\n");
	printf("*************************************************\n");
	printf("*************************************************\n");
	int n;
	int s_p,s_g,n_p,n_g;
	
	scanf("%d",&n); 
	while(n==1){
		srand(time(NULL));
		
		printf("���������г�ʼ��\n");
		scanf("%d",&total_money);
		while(total_money<=0){
			printf("���г�ʼ����С�ڵ����㣡����������\n");
			 scanf("%d",&total_money);
		}
		
		printf("����������Ӫҵʱ�䣺\n");
		scanf("%d",&closeTime);
		while(closeTime<=0){
			printf("����Ӫҵʱ�䲻��С�ڵ����㣡����������\n");
			 scanf("%d",&closeTime);
		}
		
		printf("��������󵽴�ʱ������\n");
		scanf("%d",&arriveMaxTime);
		while(arriveMaxTime<=0||arriveMaxTime>closeTime){
			printf("��󵽴�ʱ��������С�ڵ����㣬�Ҳ��ܴ�������Ӫҵʱ�䣡����������\n");
			 scanf("%d",&arriveMaxTime);
		}
		
		printf("��������С����ʱ������\n");
		scanf("%d",&arriveMinTime);
		while(arriveMinTime<=0||arriveMinTime>arriveMaxTime||arriveMinTime>closeTime){
			printf("��С����ʱ��������С�ڵ����㣬���ܴ�����󵽴�ʱ�������Ҳ��ܴ�������Ӫҵʱ�䣡����������\n");
			 scanf("%d",&arriveMinTime);
		}
		printf("�����������ʱ�䣺\n"); 
		scanf("%d",&dealMaxTime);
		while(dealMaxTime<=0||dealMaxTime>closeTime){
			printf("�����ʱ�䲻��С�ڵ����㣬�Ҳ��ܴ�������Ӫҵʱ�䣡����������\n");
			 scanf("%d",&dealMaxTime);
		}
		
		printf("��������С����ʱ�䣺\n"); 
		scanf("%d",&dealMinTime);
		while(dealMinTime<=0||dealMinTime>dealMaxTime||dealMinTime>closeTime){
			printf("��С����ʱ�䲻��С�ڵ����㣬���ܴ��������ʱ�������Ҳ��ܴ�������Ӫҵʱ�䣡����������\n");
			 scanf("%d",&dealMinTime);
		}
		
		printf("����������׶����ޣ�\n");
		scanf("%d",&MaxMoney);
		while(MaxMoney<=0){
			printf("����׶����޲���С�ڵ����㣡����������\n");
			 scanf("%d",&MaxMoney);
		}
		printf("\n");
		MaxTime+=rand()%(arriveMaxTime-arriveMinTime+1)+arriveMinTime;
		initQueue();//���г�ʼ��
		while(currentTime<closeTime){
		
			++currentTime;
			if(TimeOfDeal<currentTime){
			 
				TimeOfDeal=currentTime;
			}
			if(TimeOfDeal==currentTime){
				
				flag=1;//�д����ڴ����� 
			}
		
			if(MaxTime==currentTime) { 
				arrive(); 
				MaxTime+=rand()%(arriveMaxTime-arriveMinTime+1)+arriveMinTime;
			}
			
		
			if(flag==1&&Q1->front!=NULL){
				if(Q1->front->money>=0){
					putMoney();//���ô��� 
					findAndDeal();//ÿ����һ�ο�͵��ò��Һ�����������2 
				}else{
					getMoney();//����ȡ���
				} 
			
			}
		
		}
	
		
		printf("�ͻ�����    �¼�����    ʱ��    ������\n");
		s_p=0;
		s_g=0;
		n_p=0;
		n_g=0;
		while(Q_en->front!=NULL){
			if(Q_en->front->type==1){//�뿪
			printf("%d           %s         %d        %d\n",Q_en->front->num,"�뿪",Q_en->front->leaveTime,Q_en->front->money); 
				if(Q_en->front->money>=0){
					s_p++;//�ɹ���� 
				}else{
					s_g++; //�ɹ�ȡ��
				}		
			}else{
				printf("%d           %s         %d        %d\n",Q_en->front->num,"����",Q_en->front->arriveTime,Q_en->front->money); 
				if(Q_en->front->money>=0){
					n_p++;//��Ҫ��� 
				}else{
					n_g++; //��Ҫȡ��
				}
			
			} 
			pop(Q_en);
		}
		while(Q1->front!=NULL)
		{	totalTime+=(closeTime-Q1->front->arriveTime);
			counter++;
			eventLink e;
			e=get_front(Q1);
			pop(Q1);
			
		}
			while(Q2->front!=NULL)
		{	totalTime+=(closeTime-Q2->front->arriveTime);
			counter++;
			eventLink e;
			e=get_front(Q1);
			pop(Q2);
			
		}
		closeBank();//Ӫҵʱ�䵽���ý������� 
		printf("\n");
		printf("��Ҫ���Ŀͻ�������%d���ˣ�\n",n_p);
		printf("��Ҫȡ��Ŀͻ�������%d���ˣ�\n",n_g);
		printf("�ɹ����Ŀͻ�������%d���ˣ�\n",s_p);
		printf("�ɹ�ȡ��Ŀͻ�������%d���ˣ�\n",s_g);
		printf("�ͻ�������ʱ�䣺%d�����ӣ�\n",totalTime);
		printf("�Ӵ��ͻ�������%d���ˣ�\n",counter); 
		printf("�ͻ�����ƽ��ʱ�䣺%d�����ӣ�\n",totalTime/counter);
		printf("���н�ҵ��%d��Ԫ��\n",total_money);
		
		printf("\n");
		printf("����Ϊ����ģ����\n");
		break; 
	}
	system("pause");
	return 0;
}

 
