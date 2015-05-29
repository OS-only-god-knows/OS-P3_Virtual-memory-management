#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>
#include <math.h>
#include <signal.h> //
#include<fcntl.h>  // 
#include "vmm.h"

Ptr_MemoryAccessRequest ptr_memAccReq;

void input_request();
// ------------------------------------------------------------------------
// 自动生成一个命令
/* 产生访存请求 */
void do_request()
{
	int i=0;
	i=random() % 2;
	/* 随机产生请求地址 */
	ptr_memAccReq->virAddr = random() % (VIRTUAL_MEMORY_SIZE/2);
	if(i==1)ptr_memAccReq->virAddr=ptr_memAccReq->virAddr+VIRTUAL_MEMORY_SIZE/2;
	/* 随机产生请求类型 */
	switch (random() % 3)
	{
		case 0: //读请求
		{
			ptr_memAccReq->reqType = REQUEST_READ;
			printf("产生请求：\n访存程序：%d\t地址：%u\t类型：读取\n", i+1,ptr_memAccReq->virAddr);
			break;
		}
		case 1: //写请求
		{
			ptr_memAccReq->reqType = REQUEST_WRITE;
			/* 随机产生待写入的值 */
			ptr_memAccReq->value = random() % 0xFFu;
			printf("产生请求：\n访存程序：%d\t地址：%u\t类型：写入\t值：%02X\n", i+1, ptr_memAccReq->virAddr, ptr_memAccReq->value);
			break;
		}
		case 2:
		{
			ptr_memAccReq->reqType = REQUEST_EXECUTE;
			printf("产生请求：\n访存程序：%d\t地址：%u\t类型：执行\n", i+1, ptr_memAccReq->virAddr);
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
	int k , v, i;
	printf("请输入访存程序：\n");
	scanf("%d",&i);
	printf("请输入访存请求地址：（无符号整数）\n");
	scanf("%u",&ptr_memAccReq->virAddr);
	if(i==2)ptr_memAccReq->virAddr=ptr_memAccReq->virAddr+VIRTUAL_MEMORY_SIZE/2;
	printf("请输入访存请求类型：（0表示读，1表示写，2表示执行)\n");
	scanf("%d",&k);
	switch (k)
	{
		case 0: //读请求
		{
			ptr_memAccReq->reqType = REQUEST_READ;
			break;
		}
		case 1: //写请求
		{
			ptr_memAccReq->reqType = REQUEST_WRITE;
			/* 产生待写入的值 */
			printf("请输入待写入的值：\n");
			scanf("%02X",&v);
			ptr_memAccReq->value = v % 0xFFu;
			break;
		}
		case 2:
		{
			ptr_memAccReq->reqType = REQUEST_EXECUTE;
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
		while (c != '\n')
		{
			c = getchar();  // 如果不退出，需要在这里过滤掉其他字符
		}
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
		close(fd);
	}
//	close(fd);
	return 0;
}
