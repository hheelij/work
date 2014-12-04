/*
 * Copyrigt (C) 2014 Haihua Electronics Enterprise (Chian) Co. Ltd.
 *
 * File Name       : main.c
 * Author          : LiJian
 * Created         : 2014.11.14
 * Description     : 简单链表操作,静态链表
 * Last Modify     : 2014.11.14 17:30 by lijian
 */

#include "common.h"

#define NULL 0

struct student{
   long num;
   float score;
   struct student *next;
};

void main()
{
   struct student zheng,li,wang,*head,*p;
   zheng.num = 211876;
   zheng.score = 99;
   li.num = 201357;
   li.score = 70;
   wang.num = 201411;
   wang.score = 59;
  
   head = &zheng;          //将结点zheng的起始地址赋给头指针head
   zheng.next = &li;       //将结点li的起始地址赋给结点zheng的next成员
   li.next = &wang;        //将结点wang的起始地址赋给结点li的next成员
   wang.next = NULL;       //wang结点的next成员不存放其他结点地址
   p = head;               //使p指针指向zheng结点

   do
     {
       printf("%ld %5.1f\n",p->num,p->score);   //输出p指向的结点的数据
       
       p = p->next;                             //使p指向下一结点
     }while(p != NULL);                         //输出完wang结点后p的值为NUL
}
