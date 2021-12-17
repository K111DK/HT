//
// Created by 35802 on 2021/12/17.
//

#ifndef HF_DECOMPRESS_H
#define HF_DECOMPRESS_H
void HeadInfoWrite(FILE*fp,CompressInfo*Info){
    int num=0;
    fwrite(Info,sizeof (CompressInfo),1,fp);
    num=strlen(Info->name)+1;
    fwrite(&num,sizeof (int),1,fp);
    fwrite(Info->name,strlen(Info->name)+1,1,fp);
    num=strlen(Info->Extension)+1;
    fwrite(&num,sizeof (int),1,fp);
    fwrite(Info->Extension,strlen(Info->Extension)+1,1,fp);

    fwrite(Info->UnitSet,sizeof (CompressNode)*(Info->UnitNum),1,fp);
    int i=0;
    for(i=0;i<Info->UnitNum;++i){
        num=strlen(Info->UnitSet[i].unit)+1;
        fwrite(&num,sizeof (int),1,fp);
        fwrite(Info->UnitSet[i].unit, strlen(Info->UnitSet[i].unit)+1,1,fp);
        num=strlen(Info->UnitSet[i].HuffCode)+1;
        fwrite(&num,sizeof (int),1,fp);
        fwrite(Info->UnitSet[i].HuffCode, strlen(Info->UnitSet[i].HuffCode)+1,1,fp);
    }
}
char*GenCompressString(FILE*fp,double basicUnitSize,char*preString,CompressInfo*CInfo){//由原串得到huffman串
    char *temp;
    char *temp1;
    temp=ReadString(fp,basicUnitSize,CInfo->FileSize);//读字符串，长度为BasicNum*2字节，保证整字节
    if((ftell(fp) >= CInfo->FileSize)) {//到文件末，证明读到的是尾串
        temp = BiChConverse(temp, 1, temp[(int)(basicUnitSize*2)]);
        temp = StringCombina(CInfo->UnitSet[CInfo->UnitNum-1].HuffCode,NULL);
    } else{//证明读到的非尾串
        temp = BiChConverse(temp, 1, (int)(basicUnitSize*2));
        temp1= StringCut(temp,(int)(basicUnitSize*2*8)/2-1);
        temp = StringCombina(CInfo->UnitSet[FindInMap(temp1,CInfo)].HuffCode,CInfo->UnitSet[FindInMap(temp,CInfo)].HuffCode);
    }
    temp = StringCombina(preString,temp);
    if(*preString!='\0')free(preString);
    return temp;
}
HuffmanNode *HuffSearch(HuffmanNode *node,CompressInfo*CInfo,char*preString){
    if(strlen(preString)<CInfo->bit||node->code==Node_SINGAL){
        exit(114514);
    }else{
        char *now= StringCut(preString,CInfo->bit-1);
        char *a=BinaryToDemical(now);
        int c=*a-48;
        return &node->Child[c];
    }
}
void DeCompress(char*oriPath,char*tarPath){
    FILE *fori= fopen(oriPath,"rb+");
    CompressInfo *CInfo= HeadInfoRead(fori);
    HuffmanTree *HT=HuffmanTreeConstruct(CInfo);
    HuffmanCode(HT->Head,1,CInfo->HuffBranch,"\0",CInfo);
    char *outname= StringCombina(tarPath, StringCombina(CInfo->name,CInfo->Extension));
    FILE *ftar= fopen(outname,"wb+");
    HuffmanNode *Hn=HT->Head;
    char*preString="\0";
    char*preBiString="\0";
    char*write;
    while(!feof(fori)){
        preString= StringCombina(preString, ReadString(fori,(double )CInfo->bit*4, file_size(oriPath)));
        while(strlen(preString)>=3){
            if(!feof(fori)||CInfo->completeSize==0){
                if(Hn->code==Node_SINGAL){
                    preBiString= StringCombina(preBiString,CInfo->UnitSet[Hn->num].unit);
                    Hn=HT->Head;
                }else{
                    Hn= HuffSearch(Hn,CInfo,preString);
                }
            }
        }
        if(strlen(preBiString)>=8){
            write=BiChConverse(StringCut(preString, 8*((int)strlen(preString)/8)-1),0,0);
            fwrite(write, sizeof (char), strlen(write),ftar);
        }
    }
    if(CInfo->completeSize!=0){
        preString= StringCut(preString,CInfo->bit-CInfo->completeSize-1);
    }
    fclose(fori);
    fclose(ftar);
}
#endif //HF_DECOMPRESS_H
