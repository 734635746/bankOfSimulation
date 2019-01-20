#include<stdio.h>
#include<stdlib.h>
#include<time.h>

/**
	客户\事件结构体 
*/ 
typedef struct event{
	int type;//事件类型到达或者离开 0为到达，1为离开 
	int arriveTime;//到达时间
	int leaveTime;//离开时间
	int money;//存款数，正数为存款，复数为取款
	int num;//客户编号 
	event* next;
}event,*eventLink;
/**
	队列结构体 
*/
typedef struct QNode{
	eventLink front;//队头指针 
	eventLink rear; //队尾指针 	
}QNode,*Queue;

int total_money;//银行现存款
int closeTime;//银行结束营业的时间
int arriveMaxTime;//两个到达事件的时间间隔上限
int arriveMinTime; //两个到达事件的时间间隔下限
int dealMaxTime;//客户交易之间的时间上限
int dealMinTime; //客户交易之间的时间下限
int MaxMoney;//交易额上限
int currentTime=0;//当前时间
int totalTime;//客户逗留总时间
int counter=0;//客户总数
int number=1;//客户初始号码
int flag=1;//判断是否有窗口在处理
int TimeOfDeal;//交易时间
int MaxTime;//到达时间
Queue Q_en;//事件队列 
Queue Q1;//队列一 
Queue Q2;//队列二

//返回队首元素 
eventLink get_front(Queue &q){
	return q->front; 
}  

//入队 
void push(Queue &q,eventLink e){
	
	if(e==NULL){//结点指针为空 
		return;
	} 
	if(q->front==NULL){//队列为空 
		q->front=e;
		q->rear=e;
	}else{
		q->rear->next=e;
		q->rear=q->rear->next;
	}
	
} 
//销毁队列
void destoryQueue(Queue &q){
	eventLink e,e1;
	e=q->front;
	if(q->front==q->rear==NULL){//队空 
		return;
	}
	while(e!=NULL){
		e1=e->next;
		free(e);
		e=NULL;
		e=e1;
	} 
	
} 

//删除队首元素 
void pop(Queue &q){
	eventLink e;
	e = q->front; 
	if(q->front==NULL){//队列为空 
		return;
	}else if(q->front->next==NULL){//队列只有一个元素 
		q->front=q->rear=NULL;
	}else{
		q->front=q->front->next;
	}
	
	if(e!=NULL){
			free(e);//释放空间  
         	e=NULL;
	}

	
} 

//结束函数,营业时间到 
void closeBank(){
	destoryQueue(Q1);
	destoryQueue(Q2);
	destoryQueue(Q_en);
} 
//到达函数，随机产生顾客进入队列一同时，产生到达事件 
void arrive(){
	eventLink e,e1;
	e = (eventLink)malloc(sizeof(event));//分配空间 
	e1 = (eventLink)malloc(sizeof(event));//分配间
	if(e==NULL||e1==NULL){//分配空间失败 
		return;
	}
	e1->money=e->money =  (rand()%(2*MaxMoney)-MaxMoney);//随机交易金额
	e1->arriveTime=e->arriveTime = currentTime;//当前时间为到达时间
	e1->num=e->num = number;
	e->next=NULL;
	e1->next=NULL;
	push(Q1,e);//加入第一队列
	e1->type=0;//到达事件 
	push(Q_en,e1);//加入事件队列
	number++; //客户人数增加 

}
//存款函数 
void putMoney(){
	
	eventLink e,e1;
	int temptime; 
	e1 = (eventLink)malloc(sizeof(event));//分配空间 
	if(e1==NULL){//分配空间失败 
		return;
	}
	e =  get_front(Q1);//对头元素 
	total_money +=  e->money;//存款
	e1->type=1;//离开事件 
	e1->num=e->num;
	e1->money=e->money;
	temptime=rand()%(dealMaxTime-dealMinTime+1)+dealMinTime;//随机生成处理时间 
	e1->leaveTime = (e->arriveTime+temptime);//离开时间为到达时间+处理时间
	e1->next=NULL;
	push(Q_en,e1);//离开事件加入队列 
	counter++;
	totalTime+=e1->leaveTime-e->arriveTime;//刷新交易时间 
	pop(Q1);//Q1出队 
	TimeOfDeal=e1->leaveTime;//交易时间为客户的离开时间 
	flag=0;//窗口没有交易需要处理 
	
	
	 
} 

void getMoney(){//取款函数
	 eventLink e,e1;
	 e1 = (eventLink)malloc(sizeof(event));//分配空间
	 if(e1==NULL){//分配空间失败 
		return;
	}
	 e =  get_front(Q1);//取Q1对头元素 
	 
	 if((-(e->money))>total_money){//资金不足,加入第二队列等待 
	 
	 	 e1->arriveTime = e->arriveTime;
	 	 e1->money = e->money;
	 	 e1->num = e->num;
	 	 e1->next=NULL;
	 	 push(Q2,e1);//客户加入第二队列 
	 	 pop(Q1);//从Q1删除客户 
	 	
	 }else{//资金足够，执行取款
	 	
	 	total_money +=e->money;//取款 
		e1->type=1;//离开事件 
		e1->num=e->num;
		e1->money=e->money; 
		e1->leaveTime= (e->arriveTime+rand()%(dealMaxTime-dealMinTime+1)+dealMinTime);//离开时间为到达时间+随机处理时间 
	 	e1->next=NULL;
		push(Q_en,e1);//离开事件加入事件队列 
	 	TimeOfDeal=e1->leaveTime;//交易时间为客户的离开时间
	 	counter++;//客户人数增加 
	 	totalTime+=e1->leaveTime-e->arriveTime;//刷新客户总逗留时间 
	 	pop(Q1);//客户离开 
		flag=0;
	 } 
	
}
//搜索函数，搜索队列中是否有可以处理的元素 
eventLink search(Queue &q,int m){
	eventLink e,e1;
	e = q->front;
	while(q->front!=NULL){//遍历队列 
		if((-(q->front->money))<=total_money){//资金充足，当前客户可取款 
			if(q->front==q->rear){
				e1=q->front;
				q->front=q->rear=NULL;
				return e1;
			}else{
				e1=q->front;
				q->front=q->front->next;
				return e1;
			}	
		}else{//资金不足，查找下一个客户 
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
//处理函数，在队列中寻找可处理的元素并处理 
void findAndDeal(){
	eventLink e,e1;
	int randTemp;
	while((e=search(Q2,total_money))!=NULL){
		e1 = (eventLink)malloc(sizeof(event));//分配空间
	 	if(e1==NULL){//分配空间失败 
		return;
    	}
		total_money+=e->money;//取款
		e1->type=1;//离开事件 
		e1->num=e->num;
		e1->money=e->money;
		randTemp = rand()%(dealMaxTime-dealMinTime+1)+dealMinTime;//随机处理时间
		e1->leaveTime=currentTime+ randTemp;//离开时间=当前时间+处理时间
		e1->next=NULL;
		push(Q_en,e1);//加入事件队列 
		TimeOfDeal+=randTemp;//刷新交易时间
		counter++;//客户人数增加 
		totalTime+=e1->leaveTime-e->arriveTime;//刷新总逗留时间 
		free(e);//释放空间 
		e=NULL; 
		
	} 
	flag=0;
}
//队列初始化 
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
	printf("********  17级计算机科学与技术（5）班  **********\n");
	printf("********       学号：3117004622        **********\n");
	printf("********            刘友滨             **********\n");
	printf("*************************************************\n");
	printf("*************************************************\n");
	printf("*************************************************\n");
	printf("*************************************************\n");
	printf("********  欢迎使用银行模拟系统,请选择  **********\n");
	printf("********          1.进入模拟           **********\n");
	printf("********            0.退出             **********\n");
	printf("*************************************************\n");
	printf("*************************************************\n");
	int n;
	int s_p,s_g,n_p,n_g;
	
	scanf("%d",&n); 
	while(n==1){
		srand(time(NULL));
		
		printf("请输入银行初始存款：\n");
		scanf("%d",&total_money);
		while(total_money<=0){
			printf("银行初始金额不能小于等于零！请重新输入\n");
			 scanf("%d",&total_money);
		}
		
		printf("请输入银行营业时间：\n");
		scanf("%d",&closeTime);
		while(closeTime<=0){
			printf("银行营业时间不能小于等于零！请重新输入\n");
			 scanf("%d",&closeTime);
		}
		
		printf("请输入最大到达时间间隔：\n");
		scanf("%d",&arriveMaxTime);
		while(arriveMaxTime<=0||arriveMaxTime>closeTime){
			printf("最大到达时间间隔不能小于等于零，且不能大于银行营业时间！请重新输入\n");
			 scanf("%d",&arriveMaxTime);
		}
		
		printf("请输入最小到达时间间隔：\n");
		scanf("%d",&arriveMinTime);
		while(arriveMinTime<=0||arriveMinTime>arriveMaxTime||arriveMinTime>closeTime){
			printf("最小到达时间间隔不能小于等于零，不能大于最大到达时间间隔，且不能大于银行营业时间！请重新输入\n");
			 scanf("%d",&arriveMinTime);
		}
		printf("请输入最大处理时间：\n"); 
		scanf("%d",&dealMaxTime);
		while(dealMaxTime<=0||dealMaxTime>closeTime){
			printf("最大处理时间不能小于等于零，且不能大于银行营业时间！请重新输入\n");
			 scanf("%d",&dealMaxTime);
		}
		
		printf("请输入最小处理时间：\n"); 
		scanf("%d",&dealMinTime);
		while(dealMinTime<=0||dealMinTime>dealMaxTime||dealMinTime>closeTime){
			printf("最小处理时间不能小于等于零，不能大于最大处理时间间隔，且不能大于银行营业时间！请重新输入\n");
			 scanf("%d",&dealMinTime);
		}
		
		printf("请输入最大交易额上限：\n");
		scanf("%d",&MaxMoney);
		while(MaxMoney<=0){
			printf("最大交易额上限不能小于等于零！请重新输入\n");
			 scanf("%d",&MaxMoney);
		}
		printf("\n");
		MaxTime+=rand()%(arriveMaxTime-arriveMinTime+1)+arriveMinTime;
		initQueue();//队列初始化
		while(currentTime<closeTime){
		
			++currentTime;
			if(TimeOfDeal<currentTime){
			 
				TimeOfDeal=currentTime;
			}
			if(TimeOfDeal==currentTime){
				
				flag=1;//有窗口在处理交易 
			}
		
			if(MaxTime==currentTime) { 
				arrive(); 
				MaxTime+=rand()%(arriveMaxTime-arriveMinTime+1)+arriveMinTime;
			}
			
		
			if(flag==1&&Q1->front!=NULL){
				if(Q1->front->money>=0){
					putMoney();//调用存款函数 
					findAndDeal();//每存完一次款就调用查找函数，检查队列2 
				}else{
					getMoney();//调用取款函数
				} 
			
			}
		
		}
	
		
		printf("客户序列    事件类型    时间    处理金额\n");
		s_p=0;
		s_g=0;
		n_p=0;
		n_g=0;
		while(Q_en->front!=NULL){
			if(Q_en->front->type==1){//离开
			printf("%d           %s         %d        %d\n",Q_en->front->num,"离开",Q_en->front->leaveTime,Q_en->front->money); 
				if(Q_en->front->money>=0){
					s_p++;//成功存款 
				}else{
					s_g++; //成功取款
				}		
			}else{
				printf("%d           %s         %d        %d\n",Q_en->front->num,"到达",Q_en->front->arriveTime,Q_en->front->money); 
				if(Q_en->front->money>=0){
					n_p++;//需要存款 
				}else{
					n_g++; //需要取款
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
		closeBank();//营业时间到调用结束函数 
		printf("\n");
		printf("需要存款的客户人数：%d（人）\n",n_p);
		printf("需要取款的客户人数：%d（人）\n",n_g);
		printf("成功存款的客户人数：%d（人）\n",s_p);
		printf("成功取款的客户人数：%d（人）\n",s_g);
		printf("客户逗留总时间：%d（分钟）\n",totalTime);
		printf("接待客户总数：%d（人）\n",counter); 
		printf("客户逗留平均时间：%d（分钟）\n",totalTime/counter);
		printf("银行结业余额：%d（元）\n",total_money);
		
		printf("\n");
		printf("以上为银行模拟结果\n");
		break; 
	}
	system("pause");
	return 0;
}

 
