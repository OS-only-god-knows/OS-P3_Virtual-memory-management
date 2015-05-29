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

/* 产生访存请求 */
void do_request()
{
	int i=0;
	int type=0;
	printf("程序：");
	scanf("%d",&i);
	printf("地址：");
	scanf("%u",&ptr_memAccReq->virAddr);
	if(ptr_memAccReq->virAddr>VIRTUAL_MEMORY_SIZE/2){
		printf("Address over\n");
		}
	if(i==2)ptr_memAccReq->virAddr=ptr_memAccReq->virAddr*2;
	printf("类型：\n读取/写入/执行:1/2/3");
	scanf("%d",&type);
	if(type==1){
		ptr_memAccReq->reqType = REQUEST_READ;
	}
	else if(type==2){
		ptr_memAccReq->reqType = REQUEST_WRITE;
		printf("写入值：");
		scanf("%02X",&ptr_memAccReq->value);
	}
	else if(type==3){
		ptr_memAccReq->reqType = REQUEST_EXECUTE;
	}
	/* 随机产生请求地址 */
	//ptr_memAccReq->virAddr = random() % ((VIRTUAL_MEMORY_SIZE/2)*(i+1));
	/* 随机产生请求类型 */
	/*switch (random() % 3)
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
	}	*/
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

