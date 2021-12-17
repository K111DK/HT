//
// Created by 35802 on 2021/12/17.
//
#ifndef HF_BASICSTRUCT_H
#define HF_BASICSTRUCT_H
#include <stdio.h>
#include "utils.h"
#define MaxUnitSize 40
#define Node_SINGAL -1
#define INIT_MAX_SIZE 16000
typedef struct CompressNode{
    char *unit;//单元翻译过来的二进制码字符串
    char *HuffCode;//huffman编码的二进制形式
    unsigned int appearNum;//该单元出现次数
}CompressNode;typedef struct CompressInfo{
    double BasicUnitSize;//基本单元的大小
    int HuffBranch;//(huffman树叉数)
    int TotalCharNum;//文件大小(包含多少基本单元)
    int UnitNum;//单元种类数
    int FileSize;
    char *name;//文件名
    char  *Extension;//后缀名
    CompressNode*UnitSet;//符号单元
    int bit;
    int completeSize;
}CompressInfo;
typedef struct HuffmanNode{
    int num;
    int value;
    int code;
    struct HuffmanNode*Child;
}HuffmanNode;
typedef struct HuffmanTree{
    HuffmanNode *Head;
    int deep;
    int branch;
}HuffmanTree;
typedef struct DynamicArray{
    HuffmanNode**Array;
    int PreviousSize;
}DynamicArray;
DynamicArray* Init(int initSize){
    DynamicArray*Darray=(DynamicArray*) malloc(sizeof (DynamicArray));
    Darray->PreviousSize=initSize;
    Darray->Array=(HuffmanNode**) malloc(sizeof(HuffmanNode*)*initSize);
}

void AssertArray(DynamicArray*Darray,int num,HuffmanNode*node){
    if(num>=Darray->PreviousSize){
        Darray->PreviousSize=num+1;
        Darray->Array= realloc(Darray->Array,(num+1)*sizeof (HuffmanNode*));
    }
    Darray->Array[num]=node;
}

HuffmanNode* ReadArray(DynamicArray*Darray,int num){
    if(num>=Darray->PreviousSize){
        return NULL;
    }else{
        if(Darray->Array[num]==NULL){
            printf("1\n");
        }
        return Darray->Array[num];
    }
}
void DelectArray(DynamicArray*Darray,int num){
    if(num==285){
        printf("1\n");
    }
    if(num>=Darray->PreviousSize){
        return;
    }else{
        Darray->Array[num]=NULL;
    }
}
#endif //HF_BASICSTRUCT_H
