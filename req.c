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

int main()
{
	int fd;
	ptr_memAccReq = (Ptr_MemoryAccessRequest) malloc(sizeof(MemoryAccessRequest));
	do_request(); // 产生一个请求

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
	return 0;
}














