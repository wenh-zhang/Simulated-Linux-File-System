/*
alloc_release.cpp
���ݿ��i-�ڵ�ķ��������
*/
#include"func.h"

//�������ݿ飬�����׵�ַ�����ؿ�ʼ�̿�����startblock_index
long alloc_blocks(unsigned int fileBlocks, unsigned int& startblock_index) {
	int Address = -1;
	if (sbGroups[0].allFreeBlocks < fileBlocks)return Address;
	int count = 0;//�ۼ����������̿�
	int i, j;
	int nAvailIndex = 0;		//�������ݿ�λ������
	int nBlockGroupIndex = 0;		//�׸����ݿ����λ������
	bool bBlockGroup[BLOCK_GROUPS_NUM];		//�����������Ϣ��Ҫ�޸�
	int nBlockGroupNum[BLOCK_GROUPS_NUM];	//���˶��ٿ�

	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
	{
		bBlockGroup[i] = false;
		nBlockGroupNum[i] = 0;
	}
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
	{
		for (j = 0; j < BLOCKS_EACH; j++)	//�������ݿ�
		{
			if (bsBlockBmp[(i*BLOCKS_EACH + j)] == NOT_USED)
			{
				count++;
				bBlockGroup[i] = true;
				nBlockGroupNum[i]++;
				if (count == 1)
				{
					Address =sbGroups[i].BlockAddr + j * BLOCK_SIZE;
					nAvailIndex = i * BLOCKS_EACH + j;
					nBlockGroupIndex = i;
					startblock_index = i * BLOCKS_EACH + j;
				}
			}
			else	//û��������
			{
				//��ԭ�����¿�ʼ
				count = 0;
				for (int k = nBlockGroupIndex;k <= i;k++) {
					bBlockGroup[k] = false;
					nBlockGroupNum[k] = 0;
				}
			}
			if (count == fileBlocks) break;
		}
		if (count == fileBlocks) break;
	}
	if (count != fileBlocks)
	{
		Address = -1;
		return Address;
	}
	//����ɹ������޸���Ӧ����Ϣ
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		sbGroups[i].allFreeBlocks -= fileBlocks;
	j = nAvailIndex + fileBlocks;
	for (i = nAvailIndex;i < j; i++)
		bsBlockBmp[i] = USED;
	for (i = nBlockGroupIndex;i < BLOCK_GROUPS_NUM; i++)
		if (bBlockGroup[i]) sbGroups[i].FreeBlocksNum-= nBlockGroupNum[i];
	return Address;
}

//����i-�ڵ㣬���ط����i-�ڵ��
int alloc_inodes() {
	int i_index = -1;//i�ڵ��
	int bg_index;//��ȡi�ڵ����ڵ����ݿ�
	for (int i = 0;i < BLOCK_GROUPS_NUM;i++) {
		for (int j = 0;j < INODES_EACH;j++) {
			if (bsInodeBmp[i * BLOCK_GROUPS_NUM + j] == NOT_USED) {
				i_index = i * BLOCK_GROUPS_NUM + j;
				bg_index = i;
				break;
			}
		}
		if (i_index != -1) break;
	}
	//����ɹ������޸���Ӧ����Ϣ
	if (i_index != -1) {
		bsInodeBmp[i_index] = USED;
		for (int i = 0;i < BLOCK_GROUPS_NUM;i++)
			sbGroups[i].allFreeInodes -= 1;
		sbGroups[bg_index].FreeInodesNum -= 1;
	}
	return i_index;
}

//�������ݿ�
void release_blocks(unsigned int nIndex) {
	for (int i = 0;i < BLOCK_GROUPS_NUM;i++) {
		sbGroups[i].allFreeBlocks += indTable[nIndex].haveBlocks;
	}
	unsigned int startblock = (indTable[nIndex].BlockAddr - indTable[0].BlockAddr) / BLOCK_SIZE;
	for (unsigned int i = startblock;i < startblock + indTable[nIndex].haveBlocks;i++) {
		sbGroups[i / BLOCKS_EACH].FreeBlocksNum++;
		bsBlockBmp[i] = NOT_USED;
	}

}

void release_inodes(unsigned int nIndex) {
	for (int i = 0;i < BLOCK_GROUPS_NUM;i++) {
		sbGroups[i].allFreeInodes++;
	}
	bsInodeBmp[nIndex] = NOT_USED;
	sbGroups[nIndex / INODES_EACH].FreeInodesNum++;
}