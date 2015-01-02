#include "stdafx.h"
#include "DES_3Round.h"

void ByteToBit(const unsigned char byte,ElemType bit[8])//��һ�ֽ�����ת���ɱ�����ʽ
{
	int i;
	for(i=0;i<8;i++)
	{
		bit[7-i]=(byte>>i)&0x01;
	}
}

void BitToByte(const ElemType Bit[8],unsigned char *Byte)//��8λ����ת����һ���ֽ�
{
	int i;
	for (i=0;i<8;i++)
	{
		(*Byte)=(*Byte<<1)|Bit[i];
	}
}

void Rol(ElemType *data,int dataNum,int times)
{
	ElemType *temp=(ElemType *)malloc(sizeof(ElemType)*times);
	memcpy(temp,data,sizeof(ElemType)*times);					//����Ҫѭ���ƶ����ұߵ�λ
	memcpy(data,data+times,sizeof(ElemType)*(dataNum-times));
	memcpy(data+dataNum-times,temp,sizeof(ElemType)*times);
	free(temp);
}

void XOR(ElemType data1[],ElemType data2[],ElemType result[],int dataNum)			//�����
{
	int i;
	for(i=0;i<dataNum;i++)
	{
		result[i]=data1[i]^data2[i];
	}
}

void Swap(ElemType *Data1,ElemType *Data2,int DataNum)//����Data1��Data2
{
	ElemType *temp=(ElemType*)malloc(sizeof(ElemType)*DataNum);
	memcpy(temp,Data1,sizeof(ElemType)*DataNum);
	memcpy(Data1,Data2,sizeof(ElemType)*DataNum);
	memcpy(Data2,temp,sizeof(ElemType)*DataNum);
	free(temp);
}

void UniversalPermutation(ElemType *data,int dataNum,const ElemType permutationTable[])//ͨ���û�����
{
	int i;
	ElemType *temp=(ElemType*)malloc(sizeof(ElemType)*dataNum);
	for (i=0;i<dataNum;i++)
	{
		*(temp+i)=*(data+permutationTable[i]-1);
	}
	memcpy(data,temp,sizeof(ElemType)*dataNum);
	free(temp);
}

void CompressExtendPermutation(ElemType Data[],ElemType Result[],
	const ElemType PermutationTable[],int ResultNum)//ѹ��/��չ�û�����������resultNum��ѹ��/��չ���λ��
{
	int i;
	for (i=0;i<ResultNum;i++)
	{
		Result[i]=Data[PermutationTable[i]-1];
	}
}

void CreateSubKey(ElemType Key[64],ElemType SubKey[ROUNDNUM][48])//����Կ���ɺ���
{
	int i;
	ElemType Key_56bit[56]={0};//����PC-1�û���56������Կ
	CompressExtendPermutation(Key,Key_56bit,PC_1,56);//PC-1�û�
	for (i=0;i<ROUNDNUM;i++)
	{
		Rol(Key_56bit,28,MoveTimes[i]);//ǰ28λѭ������
		Rol(Key_56bit+28,28,MoveTimes[i]);//��28λѭ������
		CompressExtendPermutation(Key_56bit,SubKey[i],PC_2,48);//PC-2�û�
	}
}

void SBOX(ElemType Input[48],ElemType Output[32])//S�У�����48λ�����32λ
{
	int i;
	for (i=0;i<8;i++)
	{
		int Row,Col;//S�е��к���
		ElemType tempOutput=0;
		Row=Input[i*6]*2+Input[i*6+5];
		Col=Input[i*6+1]*8+Input[i*6+2]*4+Input[i*6+3]*2+Input[i*6+4];
		tempOutput=S[i][Row][Col];
		Output[i*4]=(tempOutput&0x08)>>3;			//ת���ɱ�����ʽ
		Output[i*4+1]=(tempOutput&0x04)>>2;
		Output[i*4+2]=(tempOutput&0x02)>>1;
		Output[i*4+3]=tempOutput&0x01;
	}
}

void DES_Encrypt(unsigned char PlainText[8],unsigned char CipherText[8],
	ElemType Key[64])//ʹ��DES����64λ���ݣ�8�ֽڣ������Գ�ʼ�û��ͳ�ʼ���û�
{
	int i;
	ElemType SubKey[ROUNDNUM][48]={0};
	ElemType PlainBit[64]={0};//������ʽ����������
	ElemType ER[48]={0};//������չ�û�����Ұ벿������
	ElemType SboxOutput[32]={0};//S�е����
	CreateSubKey(Key,SubKey);//��������Կ
	for (i=0;i<8;i++)
	{
		ByteToBit(PlainText[i],PlainBit+i*8);//������ת���ɱ�����ʽ�������
	}
	for (i=0;i<ROUNDNUM;i++)
	{
		CompressExtendPermutation(PlainBit+32,ER,EP,48);//�������Ұ벿�ֽ�����չ�û�
		XOR(ER,SubKey[i],ER,48);//����չ�û�����ER������Կ������򣬽��������ER��
		SBOX(ER,SboxOutput);//����չ�û�����ER����S�еõ����SboxOutput
		UniversalPermutation(SboxOutput,32,P);//��S������Ľ������P�û�
		XOR(SboxOutput,PlainBit,PlainBit,32);//��������벿����P�û����������򣬽�����浽������벿��
		Swap(PlainBit,PlainBit+32,32);//�����������Ҳ���
	}
	for (i=0;i<8;i++)
	{
		BitToByte(PlainBit+i*8,CipherText+i);//���ս��ת�����ֽ�����
	}
}