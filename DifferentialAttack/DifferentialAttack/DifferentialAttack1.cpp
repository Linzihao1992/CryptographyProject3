#include "stdafx.h"

#include "DifferentialAttack.h"
int main()
{
	printf("\t\t����������3��DES���в�ֹ�����������\n\n");
	int i;
	TextPair PlainPair[3],CipherPair[3];
	FILE *fin=NULL;
	printf("˵������Ϊ����Ķ������ļ�\"input.bin\"��Ӧ�����¸�ʽ��֯����������ƽ�ʧ�ܣ�\n\n");
	printf("����11����12\n����21����22\n����31����32\n����11����12\n����21����22\n����31����32\n");
	printf("��������i1������i2�ĵ�32λ��ͬ(1<=i<=3)��\n\n");
	printf("�밴�������ʼ�ƽ���Կ����\n\n");
	_getch();
	while (1)
	{	
		if (NULL==(fin=fopen("input.bin","rb")))
		{
			printf("���ļ�ʧ�ܣ���ȷ��\"input.bin\"�ļ��������ͬһ���£�\n");
			printf("����������ԡ���\n");
			_getch();
			continue;
		}
		break;
	}
	for (i=0;i<3;i++)
	{
		fread(PlainPair[i].First,sizeof(ElemType),8,fin);//�����i�����Ķ�
		fread(PlainPair[i].Second,sizeof(ElemType),8,fin);
	}
	for (i=0;i<3;i++)
	{
		fread(CipherPair[i].First,sizeof(ElemType),8,fin);//�����i�����Ķ�
		fread(CipherPair[i].Second,sizeof(ElemType),8,fin);
	}
/*	TextPair PlainPair[3]={{
								{0x74,0x85,0x02,0xCD,0x38,0x45,0x10,0x97},
								{0x38,0x74,0x75,0x64,0x38,0x45,0x10,0x97}
							},
							{
								{0x48,0x69,0x11,0x02,0x6A,0xCD,0xFF,0x31},
								{0x37,0x5B,0xD3,0x1F,0x6A,0xCD,0xFF,0x31}
							},
							{
								{0x35,0x74,0x18,0xDA,0x01,0x3F,0xEC,0x86},
								{0x12,0x54,0x98,0x47,0x01,0x3F,0xEC,0x86}
							}};
	TextPair CipherPair[3]={{
								{0x03,0xC7,0x03,0x06,0xD8,0xA0,0x9F,0x10},
								{0x78,0x56,0x0A,0x09,0x60,0xE6,0xD4,0xCB}
							},
							{
								{0x45,0xFA,0x28,0x5B,0xE5,0xAD,0xC7,0x30},
								{0x13,0x4F,0x79,0x15,0xAC,0x25,0x34,0x57}
							},
							{
								{0xD8,0xA3,0x1B,0x2F,0x28,0xBB,0xC5,0xCF},
								{0x0F,0x31,0x7A,0xC2,0xB2,0x3C,0xB9,0x44}
							}};*/
	ElemType PossibleKey[56]={0};//���ܵ�56λ��Կ���
	CrackKey(PlainPair,CipherPair,PossibleKey);//�ƽ���Կ��������浽PossibleKey��
	if (!SearchKey(PossibleKey,PlainPair[0].First,CipherPair[0].First))
	{
		printf("�ƽ�ʧ�ܣ����������ĶԸ�ʽ�Ƿ���ȷ��\n\n");
	}
	system("pause");
	return 0;
}

void Permutation(ElemType *data,int dataNum,const ElemType permutationTable[])//�û�����
{
	int i;
	ElemType *temp=(ElemType*)malloc(sizeof(ElemType)*dataNum);
	for (i=0;i<dataNum;i++)
	{
		*(temp+i)=permutationTable[i]?*(data+permutationTable[i]-1):'?';//�û�����Ϊ0��λ�ô���ȷ������'?'��ʾ
	}
	memcpy(data,temp,sizeof(ElemType)*dataNum);
	free(temp);
}

void ExtendPermutation(const ElemType data[32],ElemType result[48])//��չ�û�
{
	int i;
	for (i=0;i<48;i++)
	{
		result[i]=data[EP[i]-1];
	}
}

void ConstructTestSet(const ElemType E_1[6],const ElemType E_2[6],
	const ElemType C[4],int SBOXNum,ElemType Test[64])//������Լ���Test(E,E*,C)������SBOXNum��S�����
{
	ElemType E[6]={0};
	ElemType testE1=0;
	int i,j;
	XOR(E_1,E_2,E,6);			//E'=E xor E*��Ӧ�˴�ΪE=E_1 xor E_2
	for (testE1=0;testE1<64;testE1++)		//testE1��000000~111111���γ���
	{
		ElemType E1[6]={0},E2[6]={0};
		ElemType SBoxOutput_E1[4]={0},SBoxOutput_E2[4]={0},SBoxOutputXor[4]={0};
		for (i=0;i<6;i++)
		{
			E1[5-i]=(testE1>>i)&0x01;//��testE1ת��Ϊ6�������ݱ��浽E1��
			E2[5-i]=E1[5-i]^E[5-i];//E*=E xor E'�˴���ӦΪE2=E1 xor E
		}
		SBOX(E1,SBoxOutput_E1,SBOXNum);
		SBOX(E2,SBoxOutput_E2,SBOXNum);
		XOR(SBoxOutput_E1,SBoxOutput_E2,SBoxOutputXor,4);//S��������=S(E) xor S(E xor E')���˴���ӦΪ
												//SBoxOutputXor=S(SBoxOutput_E1) xor S(SBoxOutput_E2)
		for (i=0;i<4;i++)
		{
			if (SBoxOutputXor[i]!=C[i])
				break;
		}
		if (4==i)//�ȶԳɹ�����ǰE1��Ӧ��S����������C��ͬ
		{
			ElemType byteE_1=0;
			for (j=0;j<6;j++)
			{
				byteE_1=(byteE_1<<1)|E_1[j];//��E_1ת���ɶ�������ʽ
			}
			Test[byteE_1^testE1]++;//��E_1������õ�Test��Ԫ�أ���Ӧλ�ü�������1
		}
	}
}

void SBOX(ElemType input[6],ElemType output[4],int boxNum)
{
	int i;
	ElemType temp=S[boxNum][input[0]*2+input[5]][input[1]*8+input[2]*4+input[3]*2+input[4]];
	for(i=0;temp;i++)					//ת���ɶ�����
	{
		output[3-i]=temp%2;
		temp/=2;
	}
}

void XOR(const ElemType data1[],const ElemType data2[],ElemType result[],int dataNum)//�����
{
	int i;
	for(i=0;i<dataNum;i++)
		result[i]=data1[i]^data2[i];
}

void DecToBin(int decNum,ElemType result[],int bitNum)//��ʮ������decNumת����bitNumλ�����ƴ浽result��
{
	int i;
	for (i=0;decNum;i++)
	{
		result[bitNum-i-1]=decNum%2;
		decNum/=2;
	}
}

void Ror(ElemType *data,int dataNum,int times)//ѭ�����ƺ�������dataѭ������timesλ
{
	ElemType *temp=(ElemType*)malloc(sizeof(ElemType)*times);
	memcpy(temp,data+dataNum-times,sizeof(ElemType)*times);	//����Ҫѭ���ƶ������λ
	memcpy(data+times,data,sizeof(ElemType)*(dataNum-times));
	memcpy(data,temp,sizeof(ElemType)*times);
	free(temp);
}

void CrackKey(TextPair PlainPair[3],TextPair CipherPair[3],ElemType PossibleKey[56])//��Կ�ƽ⺯��
{
	ElemType EL3_1[3][48]={0},EL3_2[3][48]={0};//��չ���L3��L3*�����ߵ����ֵ��E'
	ElemType SBoxOutput[3][32]={0};//���������ĶԵ�S�������򣬼�C'=C xor C* = P^-1(R3' xor L0')
	ElemType Test[8][64]={0};//�����Test����
	int i,j;
	for (i=0;i<3;i++)
	{
		for (j=0;j<4;j++)
		{
			ElemType R3=0,L0=0;
			R3=CipherPair[i].First[j+4]^CipherPair[i].Second[j+4];//R3'=R3 xor R3*
			L0=PlainPair[i].First[j]^PlainPair[i].Second[j];//L0'=L0 xor L0*
			ByteToBit(R3^L0,SBoxOutput[i]+j*8);//��������R3' xor L0'��ת����bit��ʽ����SBoxOutput
		}
	}
	for (i=0;i<3;i++)
	{
		Permutation(SBoxOutput[i],32,P_Inverse);//����P�û������û��õ�S��������	
	}
	for (i=0;i<3;i++)
	{
		ElemType L3_1[32],L3_2[32];
		for (j=0;j<4;j++)
		{
			ByteToBit(CipherPair[i].First[j],L3_1+j*8);//�������Ƶ�L3ת���ɱ�����ʽ�����û�
			ByteToBit(CipherPair[i].Second[j],L3_2+j*8);//�������Ƶ�L3*ת���ɱ�����ʽ�����û�
		}
		ExtendPermutation(L3_1,EL3_1[i]);//����E(L3)
		ExtendPermutation(L3_2,EL3_2[i]);//����E(L3*)
	}
	for (i=0;i<8;i++)//����8������Test��ȷ��K3
	{
		ConstructTestSet(EL3_1[0]+6*i,EL3_2[0]+6*i,SBoxOutput[0]+4*i,i,Test[i]);
		ConstructTestSet(EL3_1[1]+6*i,EL3_2[1]+6*i,SBoxOutput[1]+4*i,i,Test[i]);
		ConstructTestSet(EL3_1[2]+6*i,EL3_2[2]+6*i,SBoxOutput[2]+4*i,i,Test[i]);
	}
	for (i=0;i<8;i++)
	{
		for (j=0;j<64;j++)
		{
			if (3==Test[i][j])
			{
				DecToBin(j,PossibleKey+i*6,6);//������Test���Ľ���(������Ϊ3)��Ӧ��λ������ת�ɶ����ƴ浽�ƽ���Կ��
			}
		}
	}
	Permutation(PossibleKey,56,PC2_Inverse);
	Ror(PossibleKey,28,4);//ǰ28λѭ���ƶ�4λ����һ��1λ���ڶ���1λ��������2λ����4λ��
	Ror(PossibleKey+28,28,4);//��28λѭ���ƶ�4λ
	Permutation(PossibleKey,56,PC1_Inverse);//PC�û������û�

}

bool SearchKey(ElemType PossibleKey[54],ElemType PlainText[8],ElemType CipherText[8])
	//ö�ٿ�����Կ�����ֻҪĳһ�����������PlainText�õ��Ľ��������CipherText��ͬ�����
{
	ElemType i;
	for (i=0;i<256;i++)//��00000000b~11111111bö��2^8=256��δ֪λ
	{
		ElemType Bin[8]={0};
		ElemType tempKey[64]={0};
		ElemType tempCipherText[8]={0};//�洢ʹ�õ�ǰö�ٳ�����Կ���ܹ�������
		DecToBin(i,Bin,8);//ת���ɶ����ƴ洢
		int cnt=0,j;
		for (j=0;j<64;j++)
		{
			if (0==(j+1)%8)//У��λ�������
			{
				tempKey[j]='?';
			}
			else
			{
				tempKey[j]='?'==PossibleKey[j-j/8]?Bin[cnt++]:PossibleKey[j-j/8];//�����λ��ȷ���ͼ���tempKey������˳����Bin���λ����
				//����PossibleKey������Ҫ��j-j/8����Ϊÿ�л��һλУ��λ��tempKey������Ҫ��j/8
			}
		}
		DES_Encrypt(PlainText,tempCipherText,tempKey);//�õ�ǰö�ٵ���Կ�ƽ��һ�����ģ����Ƿ�����Ӧ�������
		if (0==memcmp(CipherText,tempCipherText,8*sizeof(ElemType)))//�����ǰ��Կ�ƽ�����Ӧ������ȣ��������һ��Կ
		{
			printf("�ƽ����56λ��Կ���£�\"?\"����У��λ���Լ�����Ӱ�졣\n");
			for (i=0;i<64;i++)
			{
				if (0==(i+1)%8)
					printf("%c\n",tempKey[i]);
				else 
					printf("%d ",tempKey[i]);
			}
			return 1;
		}
	}
	return 0;
}