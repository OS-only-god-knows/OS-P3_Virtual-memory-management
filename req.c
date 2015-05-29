#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include<sys/types.h>
#include<fcntl.h>
#include "vmm.h"

Ptr_MemoryAccessRequest ptr_memAccReq;

void input_request();
void do_request();

// ------------------------------------------------------------------------
// 自动生成一个命令
/* 产生访存请求 */
void do_request()
{
	/* 随机产生请求地址 */
	ptr_memAccReq->virAddr = random() % VIRTUAL_MEMORY_SIZE;
	/* 随机产生请求类型 */
	switch (random() % 3)
	{
		case 0: //读请求
		{
			ptr_memAccReq->reqType = REQUEST_READ;
			printf("产生请求：\n地址：%u\t类型：读取\n", ptr_memAccReq->virAddr);
			break;
		}
		case 1: //写请求
		{
			ptr_memAccReq->reqType = REQUEST_WRITE;
			/* 随机产生待写入的值 */
			ptr_memAccReq->value = random() % 0xFFu;
			printf("产生请求：\n地址：%u\t类型：写入\t值：%02X\n", ptr_memAccReq->virAddr, ptr_memAccReq->value);
			break;
		}
		case 2:
		{
			ptr_memAccReq->reqType = REQUEST_EXECUTE;
			printf("产生请求：\n地址：%u\t类型：执行\n", ptr_memAccReq->virAddr);
			break;
		}
		default:
			break;
	}	
}


// --------------------------------------------------------------------------
// 支持手动输入命令：
void input_request()
{
	int k , v;
	printf("请输入访存请求地址：（无符号整数）\n");
	scanf("%u",&ptr_memAccReq->virAddr);
	printf("请输入访存请求类型：（0表示读，1表示写，2表示执行\n");
	scanf("%d",&k);
	switch (k)
	{
		case 0: //读请求
		{
			ptr_memAccReq->reqType = REQUEST_READ;
			printf("产生请求：\n地址：%u\t类型：读取\n", ptr_memAccReq->virAddr);
			break;
		}
		case 1: //写请求
		{
			ptr_memAccReq->reqType = REQUEST_WRITE;
			/* 产生待写入的值 */
			printf("请输入待写入的值：\n");
			scanf("%02X",&v);
			ptr_memAccReq->value = v % 0xFFu;
			printf("产生请求：\n地址：%u\t类型：写入\t值：%02X\n", ptr_memAccReq->virAddr, ptr_memAccReq->value);
			break;
		}
		case 2:
		{
			ptr_memAccReq->reqType = REQUEST_EXECUTE;
			printf("产生请求：\n地址：%u\t类型：执行\n", ptr_memAccReq->virAddr);
			break;
		}
		default:
			printf("无效输入\n");
			exit(0);
			break;
	}	
	
}


// ----------------------------------------------------------------
// 主函数
int main()
{
	char c;
	int fd;
	ptr_memAccReq = (Ptr_MemoryAccessRequest) malloc(sizeof(MemoryAccessRequest));
	
	while(1)
	{
		printf("请选择：\n");
//		printf("按A自动生成一个请求命令,按W手动输入一个请求命令\n");
		printf("	按A自动生成一个请求命令\n");
		printf("	按W手动输入一个请求命令\n");
		printf("	按Q退出程序\n");
		c = getchar();
		switch(c)
		{
		case 'A': 
		case 'a':
			do_request(); // 自动产生一个请求
			break;
		case 'W':
		case 'w':
			input_request(); // 手动输入一个请求
			break;
		case 'Q':
		case 'q':
			break;
		default :
			printf("无效输入\n");
		}
		if( c == 'Q' || c == 'q')
		{
			break; // 退出
		}
//		if ((c = getchar()) == 'A' || c == 'a')
//		{
//			do_request(); // 自动产生一个请求
//		}
		while (c != '\n')
		{
			c = getchar();  // 如果不退出，需要在这里过滤掉其他字符
		}
/*
		if ((c = getchar()) == 'W' || c == 'w')
		{
			input_request(); // 手动输入一个请求
		}
		while (c != '\n')
		{
			c = getchar();
		}

		if ((c = getchar()) == 'Q' || c == 'q')
		{
			break ; // 退出
		}
		while (c != '\n')
		{
			c = getchar();
		}
*/

// 	do_request(); // 产生一个请求

		/* 打开fifo文件 */
		if((fd=open("/tmp/server",O_WRONLY))<0)
		{
			printf("fifo open failed");
		}
		/* 向fifo写数据 */
		if(write(fd,ptr_memAccReq,sizeof(MemoryAccessRequest))<0)
		{
			printf("write to fifo failed");
		}
	}
	close(fd);
	return 0;
}












