//
// Created by 35802 on 2021/12/17.
//

#ifndef HF_COMPRESS_H
#define HF_COMPRESS_H
#include "BasicStruct.h"
#include "utils.h"
CompressInfo *MapConstruct(double basicUnitSize,int BranchSize,char*originPath);
HuffmanTree *HuffmanTreeConstruct(CompressInfo*CInfo);
void CompressUnitInsert(char InsertUnit[MaxUnitSize],CompressInfo*CInfo);
void HuffCodeConstruct(HuffmanTree*HTree,CompressInfo*CInfo);
void HuffmanTreePrint(HuffmanTree*HTree);
void CompressFileGen(CompressInfo*CInfo,HuffmanTree*HTree,char*originPath,char*targetPath);
void HuffmanCode(HuffmanNode*node,int num,int branch,char* preChar,CompressInfo*CInfo);
CompressInfo *HeadInfoRead(FILE*fp);
void HeadInfoWrite(FILE*fp,CompressInfo*Info);
void WriteString(FILE*fp,char*string);
char*GetEle(char *input,int mode,CompressInfo*CInfo);
char*ReadString(FILE*fp,double basicUnitSize,int file_size);
char*GenCompressString(FILE*fp,double basicUnitSize,char*preString,CompressInfo*CInfo);
int FindInMap(char*string,CompressInfo*CInfo);
void DeCompress(char*originPath,char*targetPath);
HuffmanNode *HuffSearch(HuffmanNode *HF,CompressInfo*CInfo,char*preString);
CompressInfo *MapConstruct(double basicUnitSize,int BranchSize,char*originPath){
    FILE *fp;
    char *p;
    int EXTERN=0;
    printf("Map constructing 1\n");
    fp= fopen(originPath,"rb+");//打开目标文件
    printf("origin--%x",fp);
    if(fp==NULL){
        printf("fail to open\n");
        exit(0);
    }
    CompressNode *head;
    CompressInfo *CInfo=(CompressInfo*) malloc(sizeof (CompressInfo));
    CInfo->bit=(log2(BranchSize)-(int)log2(BranchSize)==0)?log2(BranchSize): log2(BranchSize)+1;
    CInfo->FileSize= file_size(originPath);
    CInfo->BasicUnitSize=basicUnitSize;
    CInfo->HuffBranch=BranchSize;
    CInfo->TotalCharNum=0;
    CInfo->UnitNum=0;
    CInfo->completeSize=0;
    CInfo->Extension=(char*) malloc(sizeof (char)*20);
    CInfo->name=(char *) malloc(sizeof (char )*80);
    CInfo->UnitSet=(CompressNode*) malloc(sizeof (CompressNode)*INIT_MAX_SIZE);
    //记录后缀名及文件名
    int i,j,z;
    i= strlen(originPath);//从文件最末往前遍历直至'.'，目的是截取后缀名
    for(j=i-1;originPath[j]!='.';--j){}
    CInfo->Extension=StringCombina(&originPath[j],NULL);//截取文件名
    for(z=j;z>=0&&originPath[z]!='\\';--z){}
    ++z;
    CInfo->name=StringCombina(&originPath[z],NULL);
    CInfo->name=StringCut(CInfo->name,j-1-z);
    //if(basicUnitSize-(double)((int)basicUnitSize)!=0)
    FILE *fp0=fopen(originPath,"rb+");
    printf("1--%x\n",fp0);
    printf("2--%x\n",fp);
    char *temp;
    char *temp1;
    int presize;
    printf("Map constructing2\n"); //构建字符记录表
    i=0;
    while(ftell(fp) < CInfo->FileSize){//未到文件末时
        temp= ReadString(fp,basicUnitSize,CInfo->FileSize);//读字符串，长度为BasicNum*2字节，保证整字节
        if(ftell(fp) < CInfo->FileSize) {//未到文件末，证明读到的不是尾串
            char*new;
            new = BiChConverse(temp, 1, (int) (basicUnitSize * 2));//basicUnitSize*2*8
            temp=new;
        }else{//已到文件末，证明读到的是尾串，需要特殊处理
            temp = BiChConverse(temp, 1, temp[(int)(basicUnitSize*2)]);
        }
        if(ftell(fp) != CInfo->FileSize){//对于非文末串
            temp1= StringCut(temp,(int)(basicUnitSize*8-1));//将两个基本单元二进制字符串中间切半
            CompressUnitInsert(temp1,CInfo);//记入记录表
            ++CInfo->TotalCharNum;//总单元数++
            CompressUnitInsert(temp,CInfo);//记入记录表
            ++CInfo->TotalCharNum;
//            free(temp);//释放单元
//            free(temp1);//
        }else{
            CInfo->UnitSet[CInfo->UnitNum].unit=StringCombina(temp,NULL);
            CInfo->UnitSet[CInfo->UnitNum].appearNum=1;
            ++CInfo->UnitNum;
            ++CInfo->TotalCharNum;
//            free(temp);
        }
        EXTERN+=1;
    }
    printf("Map construct done! with size %d\n",EXTERN);
    fclose(fp);
    return CInfo;
}

void CompressUnitInsert(char *InsertUnit,CompressInfo*CInfo){//将基本符号单元插入
    int i=0;
    if(CInfo->UnitNum==INIT_MAX_SIZE-1000){
        CInfo->UnitSet= realloc(CInfo->UnitSet,((int)INIT_MAX_SIZE/2+CInfo->UnitNum)*sizeof (CompressNode));
    }
    if(!InsertUnit){
        return;
    }
    if(CInfo->TotalCharNum==0){//表中无单元
//        CInfo->UnitSet= (CompressNode *)malloc(sizeof (CompressNode));
        CInfo->UnitSet[0].unit=StringCombina(InsertUnit,NULL);
        CInfo->UnitSet->appearNum=1;
        ++CInfo->UnitNum;
        return;
    }
    else{//单元集中已存在
        CompressNode *node=CInfo->UnitSet;
        for(i=0;i<CInfo->UnitNum;++i){
            if(strcmp(node->unit,InsertUnit)==0){//若在字库中匹配到
                ++node->appearNum;//该单元+1
//                    if(strcmp(node->unit,"01100010")==0){
//
//                    }
                return;
            }
            ++node;
        }//不存在
//        CInfo->UnitSet=realloc(CInfo->UnitSet,(CInfo->UnitNum+3)*sizeof (CompressNode));
//            if(!CInfo->UnitSet){
//                exit(0);
//            }

        CInfo->UnitSet[CInfo->UnitNum].unit=StringCombina(InsertUnit,NULL);
        CInfo->UnitSet[CInfo->UnitNum].appearNum=1;
        ++CInfo->UnitNum;
        return;
    }
}
char* ReadString(FILE*fp,double basicUnitSize,int file_size){//读取basic*2单元
    char c;
    int i=0;
    char *chSet=(char*) malloc((int)(basicUnitSize*2+10)*sizeof(char));//尾部+'\0'
    *chSet='\0';
    int previousNum=0;
    while((ftell(fp) < file_size)&&previousNum<(int)(basicUnitSize*2)){
        chSet[previousNum]=c;
        c=fgetc(fp);
        if((c&0x00)==c){

        }
        previousNum++;
    }//取满单元
    if((ftell(fp) < file_size)){//非文件尾，加\0
        chSet[previousNum]='\0';
        return chSet;
    }else{//否
        chSet[previousNum]='\0';
        chSet= realloc(chSet,(int)(basicUnitSize*2+40)*sizeof(char));
        if(!chSet){
            exit(11451415);
        }
        chSet[(int)(basicUnitSize*2)]=previousNum;//最后一个字符的下标+1
        return chSet;
    }
}

HuffmanTree *HuffmanTreeConstruct(CompressInfo*CInfo){
    int nodeData[CInfo->UnitNum][2];//存放单元数据
    HuffmanNode *node;
    int i=0;
    printf("Tree constructing\n");

    DynamicArray *Darray;//动态指针数组，存放结点指针
    Darray= Init(CInfo->UnitNum*10000);

    for(i=0;i<CInfo->UnitNum;++i){
        nodeData[i][0]=i;//编号
        nodeData[i][1]=CInfo->UnitSet[i].appearNum;//权值
        node=(HuffmanNode*) malloc(sizeof (HuffmanNode));
        node->num=i;
        node->value=CInfo->UnitSet[i].appearNum;
        node->code=Node_SINGAL;
        AssertArray(Darray,i,node);//Darray存放HuffmanTree临时结点
    }

    HuffmanTree *head=(HuffmanTree*) malloc(sizeof (HuffmanTree));
    head->branch=CInfo->HuffBranch;
    head->Head=(HuffmanNode*) malloc(sizeof (HuffmanNode));

    int exten=0;
    int out_flag=0;
    while(!out_flag){


        int j=0;
        int flag=0;
        int temp;
        int start;
        ++exten;

        for(i=0;i<CInfo->UnitNum;++i){//将所有结点按权值从小到大排序
            temp=0;
            for(flag=j=i;j<CInfo->UnitNum;++j){
                if(nodeData[flag][1]>nodeData[j][1]){
                    flag=j;
                }
            }
            if(flag!=i){
                temp=nodeData[flag][1];
                nodeData[flag][1]=nodeData[i][1];
                nodeData[i][1]=temp;
                temp=nodeData[flag][0];
                if(temp==258){

                }
                nodeData[flag][0]=nodeData[i][0];
                nodeData[i][0]=temp;
            }
        }


        node=(HuffmanNode*) malloc(sizeof (HuffmanNode));//新开结点
        node->value=0;
        node->num=0;
        node->code=0;
        node->Child=(HuffmanNode*) malloc(sizeof (HuffmanNode)*(CInfo->HuffBranch+10));
        if(node->Child==NULL){
            exit(11);
        }
        HuffmanNode *node1;
        node1=node->Child;
        for(i=0;nodeData[i][1]==0&&i<CInfo->UnitNum;++i){}//找到最小的非0单元位置
        if(i==CInfo->UnitNum-1){
            head->Head= ReadArray(Darray,nodeData[i][0]);
            printf("Tree constructing done!\n");
            return head;
        }
        if(CInfo->UnitNum-i<CInfo->HuffBranch){//若所有非0单元不足n个，往回取0单元不足
            j=i-(CInfo->HuffBranch-CInfo->UnitNum+i);//TODO 原单元集比叉数少时会错误
            out_flag=1;
        }else{
            j=i;
        }

        //node1为第一个child
        for(start=j+CInfo->HuffBranch-1;start>=j;--start){
            node->value+=nodeData[start][1];//把各点权值加到新结点
            if(nodeData[start][1]!=0) {//将选中单元作为新节点孩子
                node1=ReadArray(Darray,nodeData[start][0]);
//                DelectArray(Darray, nodeData[start][0]);
//                node->Child[start-j].value=node1->value;
//                node->Child[start-j].num=node1->value;
//                node->Child[start-j].code=1;
                node->Child[CInfo->HuffBranch-1-start+j]=*node1;
            } else{
                node1= NULL;
                node->Child[CInfo->HuffBranch-1-start+j].num=0;
                node->Child[CInfo->HuffBranch-1-start+j].value=0;
                node->Child[CInfo->HuffBranch-1-start+j].code=0;
            }
            nodeData[start][0]=0;//信息置0
            nodeData[start][1]=0;
        }
        //新节点信息
        nodeData[j][0]=CInfo->UnitNum+exten;
        nodeData[j][1]=node->value;
        node->num=CInfo->UnitNum+exten;//编号
        AssertArray(Darray,CInfo->UnitNum+exten,node);
    }

    head->Head= ReadArray(Darray,CInfo->UnitNum+exten);
    printf("Tree constructing done!\n");
    return head;
}


void HuffCodePrint(HuffmanNode*node,CompressInfo*CInfo,int layer){
    int nodeData[CInfo->UnitNum][2];//存放单元数据
    int i=0;
    for(i=0;i<CInfo->UnitNum;++i){
        nodeData[i][0]=i;//编号
        nodeData[i][1]=CInfo->UnitSet[i].appearNum;//权值
    }
    int j=0;
    int flag=0;
    int temp;
    int start;
    for(i=0;i<CInfo->UnitNum;++i){//将所有结点按权值从小到大排序
        temp=0;
        for(flag=j=i;j<CInfo->UnitNum;++j){
            if(nodeData[flag][1]>nodeData[j][1]){
                flag=j;
            }
        }
        if(flag!=i){
            temp=nodeData[flag][1];
            nodeData[flag][1]=nodeData[i][1];
            nodeData[i][1]=temp;
            temp=nodeData[flag][0];
            if(temp==258){

            }
            nodeData[flag][0]=nodeData[i][0];
            nodeData[i][0]=temp;
        }
    }
    int size=0;
    int val=0;
    int sum=0;
    printf("Huffcode\n");
    for(i=0;i<CInfo->UnitNum;++i){
        size= strlen(CInfo->UnitSet[nodeData[i][0]].HuffCode);
        val= CInfo->UnitSet[nodeData[i][0]].appearNum;
        sum+=size*val;
        printf("Char %s Num:%d value:%d HuffCode:%s\n",BiChConverse(CInfo->UnitSet[i].unit,0,0),nodeData[i][0],CInfo->UnitSet[nodeData[i][0]].appearNum,CInfo->UnitSet[nodeData[i][0]].HuffCode);
    }
    printf("Origin size of the file is %d kb\n",((int)(CInfo->TotalCharNum*CInfo->BasicUnitSize))/1024);
    printf("Estimated size of Compressed file is %d kb\n",sum/(8*1024));
}
void HuffmanCode(HuffmanNode*node,int num,int branch,char *preChar,CompressInfo *CInfo){
    int i=0;
    char *a;
    char*b;
    for(i=0;i<branch;++i){
        char *a=(char*) malloc(sizeof (char)*2);
        a[1]='\0';
        a[0]=i+48;//该分支编号
        char *next;
        if(*preChar=='\0'){//输入时直接加入分支
            next=a;
        }else{//否则合并之前的串
            next=StringCombina(preChar,a);
        }
        if((node->Child[i].num)==(node->Child[i].code)){//该分支非结点
            continue;
        }
        else{//否则叶子/非叶子
            if((node->Child[i].code)!=Node_SINGAL){
                HuffmanCode(&node->Child[i],num,branch,next,CInfo);
            } else{
                CInfo->UnitSet[node->Child[i].num].HuffCode= DemicalToBinary(next,CInfo->HuffBranch);
            }
        }
    }
}

void MapPrint(CompressInfo*CInfo){
    printf("total char num:%d\n",CInfo->TotalCharNum);
    printf("total unitset num:%d\n",CInfo->UnitNum);
    printf("file name %s\n",CInfo->name);
    printf("file extension %s\n",CInfo->Extension);
}
int FindInMap(char*string,CompressInfo*CInfo){
    int i=0;
    if(strlen(string)!=CInfo->BasicUnitSize*8){
        i=CInfo->UnitNum-1;
    }
    for(;i<CInfo->UnitNum;++i){
        if(strcmp(string,CInfo->UnitSet[i].unit)==0){
            return i;
        }
    }
    exit(11451415);
}
void CompressFileGen(CompressInfo*CInfo,HuffmanTree*HTree,char*originPath,char*targetPath){
    FILE *fpout,*fin;
    fpout= fopen(targetPath,"wb+");
    HeadInfoWrite(fpout,CInfo);
    fin= fopen(originPath,"rb+");
    char*preViousString="\0";
    char*writeS;
    int n=0;
    while((ftell(fpout) < CInfo->FileSize)){
        preViousString= GenCompressString(fin,CInfo->BasicUnitSize,preViousString,CInfo);
        int a=ftell(fpout);
        n=((int)strlen(preViousString))/8;//当binary单元数为大于等于双整字节时
        if(n>=1){
            writeS=StringCut(preViousString,n*(8)-1);
            writeS=BiChConverse(writeS,0,0);
            fwrite(writeS, strlen(writeS),1,fpout);
            free(writeS);
        }
    }
    if(strlen(preViousString)<8){
        int delta=8-strlen(preViousString);
        int i=0;
        char*complete=(char*) malloc(sizeof (char)*(delta+1));
        for(i=0;i<delta;++i){
            complete[i]='0';
        }
        complete[i]='\0';
        preViousString=StringCombina(preViousString,complete);
        CInfo->completeSize=delta;
    }
    writeS=BiChConverse(preViousString,0,0);
    fwrite(writeS, strlen(writeS),1,fpout);
    free(writeS);
    fclose(fpout);
    fclose(fin);
    printf("file gen done!\n");
};
CompressInfo *HeadInfoRead(FILE*fp){
    CompressInfo *Info=(CompressInfo*) malloc(sizeof (CompressInfo));
    int num=0;
    fread(Info,sizeof (CompressInfo),1,fp);

    fread(&num,sizeof (int),1,fp);
    fread(Info->name,strlen(Info->name)+1,1,fp);

    fread(&num,sizeof (int),1,fp);
    fread(Info->Extension,strlen(Info->Extension)+1,1,fp);

    fread(Info->UnitSet,sizeof (CompressNode)*(Info->UnitNum),1,fp);
    int i=0;
    for(i=0;i<Info->UnitNum;++i){
        fread(&num,sizeof (int),1,fp);
        fwrite(Info->UnitSet[i].unit, num,1,fp);

        fread(&num,sizeof (int),1,fp);
        fwrite(Info->UnitSet[i].HuffCode, num,1,fp);
    }
    return Info;
}
#endif //HF_COMPRESS_H
