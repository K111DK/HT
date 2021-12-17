//
// Created by 35802 on 2021/12/17.
//

#ifndef HF_UTILS_H
#define HF_UTILS_H
#include "stdlib.h"
#include "math.h"
#include "string.h"
char*DemicalToBinary(char*demical,int branch);
char*BinaryToDemical(char*binary);
char*BiChar(char*Binary);
char*BinaryAdd(char*add1,char *add2);
char*StringCut(char*a,int pos);
char*StringCombina(char*a,char*b);
int strcmpReg(char*a,char*b,int size);
int strcmpReg(char*a,char*b,int size){
    int i=0;
    for(i=0;i<size;++i){
        if(!a||!b){
            return 0;
        }
        if(b[i]==a[i]){
            continue;
        } else{
            return 0;
        }
    }
    return 1;
}

char*DemicalToBinary(char *demical,int Branch){
    int branch=(log2(Branch)-(int)log2(Branch)==0)?log2(Branch): log2(Branch)+1;
    int unitSize=branch+1;
    int u;
    char *temp;
    char*out=(char*) malloc(((strlen(demical)-1)*branch+1)*sizeof(char ));
    char*unit=(char*)malloc(unitSize*sizeof(char));
    char*unit1=(char*)malloc(unitSize*sizeof(char));
    int i=0;
    for(i=0;i<unitSize;++i){
        unit[i]='0';
        if(i==unitSize-2){
            unit[i]='1';
        }
        if(i==unitSize-1){
            unit[i]='\0';
        }
    }
    for(i=0;i<unitSize;++i){
        unit1[i]='0';
        if(i==unitSize-1){
            unit1[i]='\0';
        }
    }
    int j=0;
    for(j=0;j< strlen(demical);++j){
        u=demical[j]-48;
        int z=0;
        if(u!=0){
            for(z=0;z<u;++z){
                if(z==0){
                    temp= unit;
                    continue;
                }
                temp=BinaryAdd(temp, unit);
            }
        }
        else{
            temp=unit1;
        }
        for(z=0;z<unitSize;++z){
            out[j*branch+z]=temp[z];
        }
    }
    return out;
}


//char*BiChar(char*Binary){
//int outLen= strlen(Binary)/8+1;
//    char*outChar=(char *) malloc(sizeof (char)*outLen);
//    outChar[outLen-1]='\0';
//    int j=0;
//    int i=j=0;
//    char code=0x0;
//    char unit=0x01;
//    for(i=0;i<outLen-1;++i){
//        code=0x0;
//        unit=0x01;
//        for(j=(i+1)*8-1;j>=i*8;--j){
//            code=code|(Binary[j]==0?code:unit);
//            unit=unit<<1;
//        }
//        outChar[i]=code;
//    }
//    return outChar;
//}

char*BinaryAdd(char*add1,char *add2){//二进制字符串加法
    int l1=0,l2=0;
    l1= strlen(add1);
    l2= strlen(add2);
    int cin=0;
    int a1=0,a2=0;
    char *out=(char*) malloc(sizeof(char)*(l1+1));
    out[l1]='\0';
    if(l1==l2){
        int i=0;
        for(i= l1-1;i>=0;--i){
            a1=add1[i]-48;
            a2=add2[i]-48;
            switch (a1+a2+cin) {
                case 0: out[i]='0';
                    cin=0;break;
                case 1:out[i]='1';
                    cin=0;break;
                case 2:out[i]='0';
                    cin=1;break;
                case 3:out[i]='1';
                    cin=1;break;
            }
        }
        return out;
    }else{
        exit(0);
    }
}

char*BiChConverse(char*inputS,int mode,int size){//字符和字符对应的二进制串转换，1为字符转二进制串(1模式下必须保证size1)，0为反过来
    char ch;
    char *out1="\0";
    char *out="\0";
    char *BiCode="\0";
    char *a=(char*) malloc(sizeof (char)*2);
    a[1]='\0';
    a[0]=0x00;
    char unit=0x01;
    if(!inputS){
        return NULL;
    }
    if(mode){
        int i=0;
        if(size!=0){
            while(i!=size){
                unit=0x01;
                out1="\0";
                while(unit!=0x00){
                    if((inputS[i]&unit)==unit){//证明该位为1
                        out1=StringCombina("1",out1);
                    }else{
                        out1=StringCombina("0",out1);
                    }
                    unit=unit<<1;
                }
                out=StringCombina(out,out1);
                ++i;
            }
        }else{
            unit=0x01;
            out1="\0";
            while(unit!=0x00){
                if((inputS[i]&unit)==unit){//证明该位为1
                    out1=StringCombina("1",out1);
                }else{
                    out1=StringCombina("0",out1);
                }
                unit=unit<<1;
            }
            out=StringCombina(out,out1);
        }
//        free(inputS);
        return out;
    } else{
        int len= strlen(inputS);
        if(len%8!=0){
            return NULL;
        }else{
            BiCode=StringCombina(BiCode,inputS);
            while(*BiCode!='\0'){
                char* a1;
                int i=0;
                a[0]=0x00;
                unit=0x01;
                a1=StringCut(BiCode,7);
                for(i=7;i>=0;--i){
                    if(a1[i]=='1'){
                        a[0]=a[0]|unit;
                    }
                    unit=unit<<1;
                }
                out=StringCombina(out,a);
            }
            return out;
        }
    }
}

char*StringCombina(char*a,char*b){
    char*c;
    if(!a&&!b){
        return NULL;
    }
    if(!b&&a){
        c = (char *) malloc(strlen(a) + 1);
    }else if(b&&!a){
        c=(char*) malloc(strlen(b)+1);
    }//局部变量，用malloc申请内存
    else{
        c=(char*) malloc(strlen(a)+ strlen(b)+1);
    }
    if (c == NULL) {
        printf("error\n");
        exit (11451415);
    }
    char *tempc = c; //把首地址存下来
    if(a){
        while (*a != '\0') {*c++ = *a++;}
    }
    if(b){
        while (*b != '\0') {*c++ = *b++;}
    }
    if(*c!='\0'){
        *c='\0';
    }
    //注意，此时指针c已经指向拼接之后的字符串的结尾'\0' !
    return tempc;//返回值是局部malloc申请的指针变量，需在函数调用结束后free之
}
char*StringCut(char*originString,int pos){
    if(originString==NULL){
        return NULL;
    }
    int Le= strlen(originString);
    if(pos+1>Le){
        return NULL;
    }else{
        char*out=(char*) malloc(sizeof (char)*(pos+2));
        int i=0;
        for(i=0;i<=pos;++i){
            out[i]=originString[i];
            originString[i]='\0';
        }
        out[i]='\0';
        int j=0;
        for(j=0;originString[i+j]!='\0';++j){
            originString[j]=originString[i+j];
            originString[i+j]='\0';
        }
        originString=realloc(originString, strlen(originString)+1);
        return out;
    }
}
char*BinaryToDemical(char*binary){
    if(binary==NULL){
        exit(114514);
    }
    int length= strlen(binary);
    int i;
    char *decimal=(char*) malloc(sizeof (char)*2);
    decimal[1]='\0';
    int sum=0;
    for(i=0;i<length;++i){
        sum+=(binary[i]=='1')? pow(2,i):0;
    }
    decimal[0]=sum+48;
    return decimal;
}
int file_size(char* filename)//获取文件名为filename的文件大小。
{
    FILE *fp = fopen(filename, "rb");//打开文件。
    int size;
    if(fp == NULL) // 打开文件失败
        return -1;
    fseek(fp, 0, SEEK_END);//定位文件指针到文件尾。
    size=ftell(fp);//获取文件指针偏移量，即文件大小。
    fclose(fp);//关闭文件。
    return size;
}
#endif //HF_UTILS_H
