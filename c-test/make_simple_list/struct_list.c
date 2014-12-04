/*
 * Copyrigt (C) 2014 Haihua Electronics Enterprise (Chian) Co. Ltd.
 *
 * File Name       : struct_list.c
 * Author          : LiJian
 * Created         : 2014.11.17
 * Description     : 建立一个动态链表，实现删除、插入功能，并打印出来
 * Last Modify     : 2014.11.17 16:36 by lijian
*/
//#include <stdio.h>
//#include <malloc.h>
#include "common.h"

#define NULL 0
#define LEN sizeof(struct student)

struct student
{
	long num;
	float score;
	struct student *next;
};

int n;

//创建一个动态链表
struct student *creat(void)
{
	struct student *head;
	struct student *p1, *p2;
	n = 0;
	p1 = p2 = ( struct student * )malloc(LEN); /*开辟一个新单元*/
	scanf("%ld , %f",&p1->num,&p1->score);
	head=NULL;
	while(p1->num != 0)
	{
		n = n+1;
		if(n == 1)
			head = p1;
		else 
			p2->next = p1;
		p2 = p1;
		p1 = (struct student *)malloc(LEN);
		scanf("%ld, %f",&p1->num, &p1->score);
	}
	p2->next = NULL;
	return(head);
};

//输出链表
void print(struct student *head)
{
	struct student *p;
	printf("\n Now,These %d records are :\n",n);
	p = head;
	if(head != NULL)
		do
		{
			printf("%ld %5.1f\n",p->num,p->score);
			p = p->next;
		}
		while (p != NULL);
}

//删除结点
struct student *del(struct student *head, long num)
{
	struct student *p1,*p2;
	if(head == NULL){
		printf("\n list null! \n");
		goto end;
	}
	p1 = head;
	while(num != p1->num && p1->next != NULL ){           //p1指向的不是所要找的结点，并且后面还有结点
		p2 = p1;                                        
		p1 = p1->next;                                    //p1后移一个结点
	}
	if(num == p1->num){                                   //找到了 
		if(p1 == head)
			head = p1->next;                              //若p1指向的是首结点，把第二个结点地址赋给head
		else
			p2->next = p1->next;                          //否则将下一结点地址赋给前一结点地址
	printf("delete:%ld\n",num);
	n = n - 1;
	}
	else
		printf("%ld not been found! \n",num);             //找不到该结点
end:
	return(head);
}

//链表的插入
struct student *insert(struct student *head,struct student *stud)
{
	struct student *p0,*p1,*p2;
	p1 = head;                                            //使p1指向第一个结点
	p0 = stud;                                            //p0指向要插入的结点
	if(head == NULL){                                     //原来的链表是空链表
		head = p0;
		p0->next = NULL;                                  //使p0指向的结点作为头结点
	}
	else{
		while( (p0->num > p1->num)&&(p1->next != NULL) ){
			p2 = p1;                                      //使p2指向刚才p1指向的结点
			p1 = p1->next;                                //p1后移一个结点
		}
		if(p0->num <= p1->num){
			if(head == p1)
				head = p0;								  //插到原来第一个结点之前
			else
				p2->next = p0;                            //插到p2指向的结点之后
		p0->next = p1;
		}
		else{
			p1->next = p0;
			p0->next = NULL;                              //插到最后的结点之后
		}
	}
	n = n + 1;                                            //结点数加1
	return(head);
}

int main(int argc, char *argv[])
{
	struct student *head,*stu;
	long del_num;

	printf("\n input records: \n");
	head = creat();                                       /*建立链表，返回头指针*/
	print(head);                                          /*输出全部结点*/

	printf("\n input the deleted number: \n");
	scanf("%ld",&del_num);
	head = del(head,del_num);
	print(head);
/*
	while(del_num != 0)
	{
		head = del(head,del_num);
		print(head);
		printf("\n input the deleted number: \n");
		scanf("%ld",&del_num);
	}
*/
	printf("\n input the inserted records: \n");
	stu = (struct student *)malloc(LEN);
	scanf("%ld,%f ",&stu->num,&stu->score);
	head = insert(head,stu);
	print(head);
/*
	while(stu->num != 0)
	{
		head = insert(head,stu);
		print(head);
		printf("\n input the inserted records: \n");
		stu = (struct student *)malloc(LEN);
		scanf("%ld,%f ",&stu->num,&stu->score);
	}
*/
	return 0;
}
