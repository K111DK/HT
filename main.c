#include <stdio.h>
#include "DeCompress.h"
int main() {
//char *a="0100";
//char *b="1234";
//char *f= BinaryToDemical(a);
//f;
//    printf("%s\n",f);
    char*oriPath="C:\\Users\\35802\\Desktop\\HuffmanCompress\\实验08_信号处理及有限状态机.pdf";
    char*tarPath="C:\\Users\\35802\\Desktop\\HuffmanCompress\\实验08_信号处理及有限状态机.taz";
    printf("size of file %d\n", file_size(oriPath)/1024);
    FILE *fpin,*fpout;
    CompressInfo *CInfo;
    HuffmanTree *HT;
    char*pre="\0";
    CInfo=MapConstruct(1,2,oriPath);
    MapPrint(CInfo);
    HT=HuffmanTreeConstruct(CInfo);
    HuffmanCode(HT->Head,0,CInfo->HuffBranch,pre,CInfo);
    HuffCodePrint(HT->Head,CInfo,0);
    CompressFileGen(CInfo,HT,oriPath,tarPath);
    return 0;
}
