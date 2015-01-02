#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
const int IP[]={1,5,2,0,3,7,4,6};			//��ʼ�û�
const int IP_Inv[]={3,0,2,4,6,1,7,5};			//��ʼ���û�
const int P10[]={2,4,1,6,3,9,0,8,7,5};		//P10
const int P8[]={5,2,6,3,7,4,9,8};			//P8
const int EP[]={3,0,1,2,1,2,3,0};			//��չ�û�
const int S[2][4][4]={{{1,0,3,2},				//S��S0
						{3,2,1,0},
						{0,2,1,3},
						{3,1,3,2}},
						{{0,1,2,3},				//S��S1			
						{2,0,1,3},
						{3,0,1,0},
						{2,1,0,3}}};
const int P4[]={1,3,2,0};
void Permutation(int *data,int dataNum,const int permutationTable[]);	//ͨ�õ��û�������������IP��IP�桢P4��P10
void ExtendPermutation(const int data[4],int result[8]);				//��չ�û�
void Permutation8(const int data[10],int result[8]);					//P8�û�
void XOR(int data1[],int data2[],int result[],int dataNum);			//�����
void Rol(int *data,int dataNum,int times);				//ѭ�����ƺ���������dataNum��Ҫ�ƶ���������Ŀ��times�����ƴ���
void CreateSubKey(int key[10],int subKey[2][8]);			//��Կ���ɺ���������key��������Կ��subKey�������ɵ�ȫ������Կ
void ByteToBit(const char byte,int bit[8]);				//�ֽ�ת���ɱ��أ�bit[0]~bit[7]�洢byte�ĸ�λ����λ
void BitToByte(const int bit[8],char *byte);			//����ת�����ֽ�
void SBOX(int input[4],int output[2],int boxNum);		//S�У�����boxNum����ڼ���S��
void EncryptByte(char *byte,int subKey[2][8]);			//����һ�ֽ�����
void DecryptByte(char *byte,int subKey[2][8]);			//����һ�ֽ�����
void EncryptFile(FILE *PlainFile,FILE *EncryptFile,int Key[10]);			//�����ļ�
void DecryptFile(FILE *EncryptFile,FILE *PlainFile,int Key[10]);			//�����ļ�
int main()
{
	printf("\t\t������������S-DES�ӽ��ܹ��ߡ�����������\n\n");
	while(1)
	{
		char OpNum,FileName[255]={0};
		FILE *fin=NULL,*fout=NULL;
		int i,Key[10]={0};
		printf("��ѡ��Ҫ���еĲ�����1-���� 2-���� 3-�˳�\n");
		do
		{
			OpNum=_getch();
		}while(!('1'==OpNum||'2'==OpNum||'3'==OpNum));
		switch(OpNum)
		{
		case '1':
			while (1)
			{
				printf("\n������Ҫ���ܵ��ļ�����������չ������");
				scanf("%s",FileName);
				if (NULL==(fin=fopen(FileName,"rb")))
				{
					printf("\n���ļ�ʧ�ܣ������ԣ�\n");
					continue;
				}
				break;
			}
			while (1)
			{
				printf("\n������Ҫ���������ļ�����������չ������");
				scanf("%s",FileName);
				if (NULL==(fout=fopen(FileName,"wb")))
				{
					printf("\n�����ļ�ʧ�ܣ������ԣ�\n");
					continue;
				}
				break;
			}
			while(1)
			{
				printf("\n������10bit������Կ��");
				fflush(stdin);//��ջ�����
				for (i=0;i<10;i++)
				{
					scanf("%c",&Key[i]);
					if (!('0'==Key[i]||'1'==Key[i]))
						break;
					Key[i]-='0';
				}
				if (10==i)
					break; 
				else
				{
					printf("\n���������Կ�������������룡\n");
				}
			}
			EncryptFile(fin,fout,Key);
			fclose(fin);
			fclose(fout);
			printf("\n���ܽ����������ļ�%s��\n\n",FileName);
			break;
		case '2':
			while (1)
			{
				printf("\n������Ҫ���ܵ��ļ�����������չ������");
				scanf("%s",FileName);
				if (NULL==(fin=fopen(FileName,"rb")))
				{
					printf("\n���ļ�ʧ�ܣ������ԣ�\n");
					continue;
				}
				break;
			}
			while (1)
			{
				printf("\n������Ҫ���������ļ�����������չ������");
				scanf("%s",FileName);
				if (NULL==(fout=fopen(FileName,"wb")))
				{
					printf("\n�����ļ�ʧ�ܣ������ԣ�\n");
					continue;
				}
				break;
			}
			while(1)
			{
				printf("\n������10bit������Կ��");
				fflush(stdin);
				for (i=0;i<10;i++)
				{
					scanf("%c",&Key[i]);
					if (!('0'==Key[i]||'1'==Key[i]))
						break;
					Key[i]-='0';
				}
				if (10==i)
					break; 
				else
				{
					printf("\n���������Կ�������������룡\n");
				}
			}
			DecryptFile(fin,fout,Key);
			fclose(fin);
			fclose(fout);
			printf("\n���ܽ����������ļ�%s��\n\n",FileName);
			break;
		case '3':
			exit(0);
			break;
		default:
			break;
		}
	}
	int a[]={1,0,1,0,0,0,0,0,1,0};
	int b[2][8];
	CreateSubKey(a,b);
	for (int i=0;i<2;i++)
	{
		for(int j=0;j<8;j++)
			printf("%d ",b[i][j]);
		printf("\n");
	}
	return 0;
}

void Permutation(int *data,int dataNum,const int permutationTable[])
{
	int i,*temp=(int*)malloc(sizeof(int)*dataNum);
	for (i=0;i<dataNum;i++)
	{
		*(temp+i)=*(data+permutationTable[i]);
	}
	memcpy(data,temp,sizeof(int)*dataNum);
	free(temp);
}

void ExtendPermutation(const int data[4],int result[8])
{
	int i;
	for (i=0;i<8;i++)
	{
		result[i]=data[EP[i]];
	}
}

void Permutation8(const int data[10],int result[8])
{
	int i;
	for (i=0;i<8;i++)
	{
		*(result+i)=*(data+P8[i]);
	}
}

void XOR(int data1[],int data2[],int result[],int dataNum)			//�����
{
	int i;
	for(i=0;i<dataNum;i++)
	{
		result[i]=data1[i]^data2[i];
	}
}

void Rol(int *data,int dataNum,int times)
{
	int *temp=(int *)malloc(sizeof(int)*times);
	memcpy(temp,data,sizeof(int)*times);					//����Ҫѭ���ƶ���λ
	memcpy(data,data+times,sizeof(int)*(dataNum-times));
	memcpy(data+dataNum-times,temp,sizeof(int)*times);
	free(temp);
}

void CreateSubKey(int key[10],int subKey[2][8])				//��Կ���ɺ���������key��������Կ��subKey�������ɵ�ȫ������Կ
{
	Permutation(key,10,P10);		//P10�û�

	Rol(key,5,1);					//��Կ����λѭ������һλ
	Rol(key+5,5,1);					//��Կ����λѭ������һλ
	Permutation8(key,subKey[0]);	//P8�û��õ�����ԿK1

	Rol(key,5,2);					//��Կ����λѭ��������λ
	Rol(key+5,5,2);					//��Կ����λѭ��������λ
	Permutation8(key,subKey[1]);	//P8�û��õ�����ԿK2
}

void ByteToBit(const char byte,int bit[8])
{
	int i;
	for(i=0;i<8;i++)
	{
		bit[7-i]=(byte>>i)&0x01;
	}
}
void BitToByte(const int bit[8],char *byte)
{
	int i;
	*byte=0;
	for(i=0;i<8;i++)
	{
		*byte=(*byte<<1)|bit[i];
	}
}

void SBOX(int input[4],int output[2],int boxNum)
{
	int i,temp=S[boxNum][input[0]*2+input[3]][input[1]*2+input[2]];
	output[0]=output[1]=0;
	for(i=0;temp;i++)					//ת���ɶ�����
	{
		output[2-i-1]=temp%2;
		temp/=2;
	}
}

void EncryptByte(char *byte,int subKey[2][8])
{
	int i,j;
	int bit[8]={0},Left[4]={0},Right[4]={0},ER[8]={0},temp[4]={0};
	ByteToBit(*byte,bit);				//��һ�ֽ�����ת����8�������ݣ����к�������
	Permutation(bit,8,IP);				//���г�ʼ�û�
	memcpy(Left,bit,sizeof(int)*4);		//�õ�4λ����벿��Left
	memcpy(Right,bit+4,sizeof(int)*4);	//�õ�4λ���Ұ벿��Right

	for(i=0;i<2;i++)
	{
		ExtendPermutation(Right,ER);		//�Ұ벿���Ƚ�����չ�û��õ�8λER
		XOR(ER,subKey[i],ER,8);				//��ER������Կ������򣬽���浽ER��
		for(j=0;j<2;j++)
		{
			SBOX(ER+j*4,temp+j*2,j);		//����S��
		}
		Permutation(temp,4,P4);
		XOR(temp,Left,Left,4);				//����벿����S������Ľ����򲢽�����浽Left��
		if (1!=i)
		{
			memcpy(temp,Left,sizeof(int)*4);		//���Ҳ��ֽ���
			memcpy(Left,Right,sizeof(int)*4);
			memcpy(Right,temp,sizeof(int)*4);
		}
	}
	memcpy(bit,Left,sizeof(int)*4);			//�������������ֺϳ�
	memcpy(bit+4,Right,sizeof(int)*4);
	Permutation(bit,8,IP_Inv);				//��ʼ�û����û�
	BitToByte(bit,byte);
}

void DecryptByte(char *byte,int subKey[2][8])
{
	int i,j;
	int bit[8]={0},Left[4]={0},Right[4]={0},ER[8]={0},temp[4]={0};
	ByteToBit(*byte,bit);				//��һ�ֽ�����ת����8�������ݣ����к�������
	Permutation(bit,8,IP);				//���г�ʼ�û�
	memcpy(Left,bit,sizeof(int)*4);		//�õ�4λ����벿��Left
	memcpy(Right,bit+4,sizeof(int)*4);	//�õ�4λ���Ұ벿��Right

	for(i=1;i>=0;i--)
	{
		ExtendPermutation(Right,ER);		//�Ұ벿���Ƚ�����չ�û��õ�8λER
		XOR(ER,subKey[i],ER,8);				//��ER������Կ������򣬽���浽ER��
		for(j=0;j<2;j++)
		{
			SBOX(ER+j*4,temp+j*2,j);		//����S��
		}
		Permutation(temp,4,P4);
		XOR(temp,Left,Left,4);				//����벿����S������Ľ����򲢽�����浽Left��
		if (0!=i)
		{
			memcpy(temp,Left,sizeof(int)*4);		//���Ҳ��ֽ���
			memcpy(Left,Right,sizeof(int)*4);
			memcpy(Right,temp,sizeof(int)*4);
		}
	}
	memcpy(bit,Left,sizeof(int)*4);			//�������������ֺϳ�
	memcpy(bit+4,Right,sizeof(int)*4);
	Permutation(bit,8,IP_Inv);				//��ʼ�û����û�
	BitToByte(bit,byte);
}

void EncryptFile(FILE *PlainFile,FILE *EncryptFile,int Key[10])
{
	char byte=0;
	int SubKey[2][8]={0};
	CreateSubKey(Key,SubKey);			//��������Կ
	while(1)
	{
		byte=fgetc(PlainFile);				//���ļ���ȡһ�ֽ�����
		if (EOF==byte)	break;
		EncryptByte(&byte,SubKey);
		fputc(byte,EncryptFile);				//�����ܽ��д���ļ�
	}
}

void DecryptFile(FILE *EncryptFile,FILE *PlainFile,int Key[10])
{
	char byte=0;
	int SubKey[2][8]={0};
	CreateSubKey(Key,SubKey);			//��������Կ
	while(1)
	{
		byte=fgetc(EncryptFile);				//���ļ���ȡһ�ֽ�����
		if (EOF==byte)	break;
		DecryptByte(&byte,SubKey);				//����һ���ֽ�
		fputc(byte,PlainFile);				//�����ܽ��д���ļ�
	}
}