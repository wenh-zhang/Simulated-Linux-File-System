/*
alloc_release.cpp
数据块和i-节点的分配与回收
*/
#include"func.h"

//分配数据块，返回首地址，传回开始盘块索引startblock_index
long alloc_blocks(unsigned int fileBlocks, unsigned int& startblock_index) {
	int Address = -1;
	if (sbGroups[0].allFreeBlocks < fileBlocks)return Address;
	int count = 0;//累计连续空闲盘块
	int i, j;
	int nAvailIndex = 0;		//可用数据块位置索引
	int nBlockGroupIndex = 0;		//首个数据块组的位置索引
	bool bBlockGroup[BLOCK_GROUPS_NUM];		//数据组的组信息需要修改
	int nBlockGroupNum[BLOCK_GROUPS_NUM];	//用了多少块

	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
	{
		bBlockGroup[i] = false;
		nBlockGroupNum[i] = 0;
	}
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
	{
		for (j = 0; j < BLOCKS_EACH; j++)	//连续数据块
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
			else	//没有连续的
			{
				//还原，重新开始
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
	//分配成功，则修改相应的信息
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		sbGroups[i].allFreeBlocks -= fileBlocks;
	j = nAvailIndex + fileBlocks;
	for (i = nAvailIndex;i < j; i++)
		bsBlockBmp[i] = USED;
	for (i = nBlockGroupIndex;i < BLOCK_GROUPS_NUM; i++)
		if (bBlockGroup[i]) sbGroups[i].FreeBlocksNum-= nBlockGroupNum[i];
	return Address;
}

//分配i-节点，传回分配的i-节点号
int alloc_inodes() {
	int i_index = -1;//i节点号
	int bg_index;//所取i节点所在的数据块
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
	//分配成功，则修改相应的信息
	if (i_index != -1) {
		bsInodeBmp[i_index] = USED;
		for (int i = 0;i < BLOCK_GROUPS_NUM;i++)
			sbGroups[i].allFreeInodes -= 1;
		sbGroups[bg_index].FreeInodesNum -= 1;
	}
	return i_index;
}

//回收数据块
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