/*
 * Copyrigt (C) 2014 Haihua Electronics Enterprise (Chian) Co. Ltd.
 *
 * File Name       : pointer.c
 * Author          : LiJian
 * Created         : 2014.11.18
 * Description     : 指针练习
 * Last Modify     : 
*/

#include "common.h"

void test_print()
{
	int *test_pointer, *abs_pointer;
	int test_value, abs_value ,temp;

	test_value = 128;                      //test_value 存放变量
	abs_value  = 100;
	test_pointer = &test_value;            //test_pointer 存放地址
	abs_pointer  = &abs_value;

	printf("test_value = %d \n", test_value);
	printf("*test_pointer = %d \n", *test_pointer );
	printf("abs_value = %d \n", abs_value);
	printf("*abs_pointer = %d \n", *abs_pointer );

	temp = *test_pointer;
	printf("temp = %d \n", temp);

	temp = 100;
	*abs_pointer = temp;
	printf("temp = %d \n", temp);
	printf("*abs_pointer = %d \n", *abs_pointer);

	test_pointer = &*abs_pointer;
	//分解动作
	//test_pointer = &*&abs_value;
	//test_pointer = &abs_value;

	printf("-----------------------------------\n");
	printf("test_value = %d \n", test_value);
	printf("*test_pointer = %d \n", *test_pointer );
}

void swap(int *p1 , int *p2)
{
	int temp;
	temp = *p1;
	*p1  = *p2;
	*p2  = temp;
}

//指针变量作为函数参数
void swap_print()
{
	int a, b;
	int *pointer_a, *pointer_b;

	a = 101;
	b = 201;
	pointer_a = &a;
	pointer_b = &b;

	printf("a = %d, b = %d \n",a,b);
	printf("*pointer_a = %d, *pointer_b = %d \n",*pointer_a,*pointer_b);

	printf("\n--------------swap()---------------\n");
	if(a < b) swap(pointer_a, pointer_b);
	printf("a = %d, b = %d \n",a,b);
	printf("*pointer_a = %d, *pointer_b = %d \n",*pointer_a,*pointer_b);
}


int main(int argc, char *argv[])
{
	//test_print();
	//swap_print();
#if 0
	int i, array[10]={1,2,3,4,5,6,7,8,9,0};
	printf("The array is :\n");
	for(i = 0;i < 10; i++){
		printf("%d",array[i]);	
	}
	printf("\n-------------------\n");
	int *p;
	p = array;
	for(i = 0;i < 10; i++){
		printf("%d",*p++);	
	}
	printf("\n-------------------\n");
#endif
	
	int a = 112, b = -1;
	float c = 3.14;
	int *d = &a;
	float *e = &c;
	int *ld;
	float *le;

	ld = &a;
	le = &c;

	printf("a = %d\n",a);
	printf("b = %d\n",b);
	printf("c = %f\n",c);
    printf("d = %d\n",d);
    printf("e = %d\n",e);
	printf("-------------------\n");
	printf("ld = %d\n",*d);
    printf("le = %f\n",*e);
	printf("-------------------\n");
    printf("*d = %d\n",*d);
    printf("*e = %f\n",*e);

	return 0;
}
