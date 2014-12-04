/*
 * Copyrigt (C) 2014 Haihua Electronics Enterprise (Chian) Co. Ltd.
 *
 * File Name       : struct_list.c
 * Author          : LiJian
 * Created         : 2014.11.17
 * Description     : ����һ����̬����ʵ��ɾ�������빦�ܣ�����ӡ����
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

//����һ����̬����
struct student *creat(void)
{
	struct student *head;
	struct student *p1, *p2;
	n = 0;
	p1 = p2 = ( struct student * )malloc(LEN); /*����һ���µ�Ԫ*/
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

//�������
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

//ɾ�����
struct student *del(struct student *head, long num)
{
	struct student *p1,*p2;
	if(head == NULL){
		printf("\n list null! \n");
		goto end;
	}
	p1 = head;
	while(num != p1->num && p1->next != NULL ){           //p1ָ��Ĳ�����Ҫ�ҵĽ�㣬���Һ��滹�н��
		p2 = p1;                                        
		p1 = p1->next;                                    //p1����һ�����
	}
	if(num == p1->num){                                   //�ҵ��� 
		if(p1 == head)
			head = p1->next;                              //��p1ָ������׽�㣬�ѵڶ�������ַ����head
		else
			p2->next = p1->next;                          //������һ����ַ����ǰһ����ַ
	printf("delete:%ld\n",num);
	n = n - 1;
	}
	else
		printf("%ld not been found! \n",num);             //�Ҳ����ý��
end:
	return(head);
}

//����Ĳ���
struct student *insert(struct student *head,struct student *stud)
{
	struct student *p0,*p1,*p2;
	p1 = head;                                            //ʹp1ָ���һ�����
	p0 = stud;                                            //p0ָ��Ҫ����Ľ��
	if(head == NULL){                                     //ԭ���������ǿ�����
		head = p0;
		p0->next = NULL;                                  //ʹp0ָ��Ľ����Ϊͷ���
	}
	else{
		while( (p0->num > p1->num)&&(p1->next != NULL) ){
			p2 = p1;                                      //ʹp2ָ��ղ�p1ָ��Ľ��
			p1 = p1->next;                                //p1����һ�����
		}
		if(p0->num <= p1->num){
			if(head == p1)
				head = p0;								  //�嵽ԭ����һ�����֮ǰ
			else
				p2->next = p0;                            //�嵽p2ָ��Ľ��֮��
		p0->next = p1;
		}
		else{
			p1->next = p0;
			p0->next = NULL;                              //�嵽���Ľ��֮��
		}
	}
	n = n + 1;                                            //�������1
	return(head);
}

int main(int argc, char *argv[])
{
	struct student *head,*stu;
	long del_num;

	printf("\n input records: \n");
	head = creat();                                       /*������������ͷָ��*/
	print(head);                                          /*���ȫ�����*/

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
