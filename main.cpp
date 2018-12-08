#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <cstring>

#define RED 0
#define GREEN 1
#define BLUE 2

using namespace std;
int bmpwidth,bmpheight,linebyte;
int colornum;
unsigned char ***newBmpBuf;
unsigned char ***pBmpBuf;  //存储图像数据
unsigned char *pColorBuf;  //存储图像数据
int newbmpheight,newbmpwidth;
BITMAPFILEHEADER fileheader;
BITMAPINFOHEADER infoHead;
bool readBmp(char *bmpName) {

    FILE *fp;
    if( (fp = fopen(bmpName,"rb")) == NULL){
        cout<<"The file "<<bmpName<<"was not opened"<<endl;
        return FALSE;
    }
    if(!fread(&fileheader,sizeof(BITMAPFILEHEADER),1,fp)){
        cout<<"跳转失败"<<endl;
        return FALSE;
    }

    fread(&infoHead,sizeof(BITMAPINFOHEADER),1,fp);   //从fp中读取BITMAPINFOHEADER信息到infoHead中,同时fp的指针移动

    if((sizeof(infoHead)+ sizeof(fileheader))!=fileheader.bfOffBits){
        cout<<fileheader.bfOffBits<<" "<<infoHead.biSize<<"  "<< sizeof(fileheader)<<"  "<< sizeof(char);
        colornum = (fileheader.bfOffBits-(infoHead.biSize+ sizeof(fileheader)))/ sizeof(char);
        pColorBuf = new unsigned char[colornum];
        fread(pColorBuf,sizeof(char),colornum,fp);
    }
    bmpwidth = infoHead.biWidth;
    bmpheight = infoHead.biHeight;
    linebyte = (bmpwidth*infoHead.biBitCount/8+3)/4*4; //计算每行的字节数，24：该图片是24位的bmp图，3：确保不丢失像素
    cout<<infoHead.biBitCount;
    linebyte = 512; //计算每行的字节数，24：该图片是24位的bmp图，3：确保不丢失像素
    cout<<bmpwidth<<" "<<bmpheight<<"  "<< sizeof(fileheader)<<"  "<< sizeof(infoHead)<<"   "<<
        fileheader.bfOffBits<<infoHead.biCompression<<endl;
    pBmpBuf = new unsigned char** [bmpheight];
    for(int i=0;i<bmpheight;i++){
        pBmpBuf[i] = new unsigned char* [bmpwidth];
        for(int j=0;j<bmpwidth;j++){
            pBmpBuf[i][j] = new unsigned char [infoHead.biBitCount/8];
        }
    }
    for(int i=0;i<bmpheight;i++){
        for(int j=0;j<bmpwidth;j++){
//            cout<<sizeof(char)<<"  "<<infoHead.biBitCount/8<<endl;
            fread(pBmpBuf[i][j],sizeof(char),infoHead.biBitCount/8,fp);
//            cout<<(int)pBmpBuf[i][j][0]<<"   "<<(int)pBmpBuf[i][j][1]<<"   "<<(int)pBmpBuf[i][j][2]<<"   ";
//            cout<<(int)pBmpBuf[i][j][0];
        }
    }

    fclose(fp);   //关闭文件
    return TRUE;
}



void writeBmp(){
    char writeFileName[] = "..\\lena_C_cut.bmp";
    BITMAPFILEHEADER fileheader_cut = fileheader;
    BITMAPINFOHEADER infoHead_cut = infoHead;
    double times = 2;
    newbmpheight = bmpheight*times;
    newbmpwidth = bmpwidth*times;
    fileheader_cut.bfSize = (int)(double)(fileheader_cut.bfSize - sizeof(infoHead)- sizeof(fileheader))*times*times;
    infoHead_cut.biHeight = (int)(double)infoHead_cut.biHeight*times;
    infoHead_cut.biWidth = (int)(double)infoHead_cut.biWidth*times;
    infoHead_cut.biSizeImage = (int)(double)infoHead_cut.biSizeImage*times;
    FILE *fp;
    fp=fopen("..\\lena_C_cut.bmp","wb");
    fwrite(&fileheader_cut,sizeof(fileheader_cut),1,fp);
    fwrite(&infoHead_cut,sizeof(infoHead_cut),1,fp);

    cout<<bmpheight<<linebyte;
    if((sizeof(infoHead)+ sizeof(fileheader))!=fileheader.bfOffBits){
        fwrite(pColorBuf, sizeof(char),colornum,fp);
    }

    cout<<linebyte<<"sxa"<<bmpheight<<endl;


    if(times==2){
        for(int i=0;i<bmpheight;i++){
            for(int j=0;j<bmpwidth;j++){
                fwrite(pBmpBuf[i][j],sizeof(char),(infoHead.biBitCount/8),fp);
                fwrite(pBmpBuf[i][j],sizeof(char),(infoHead.biBitCount/8),fp);
            }
            for(int j=0;j<bmpwidth;j++){
                fwrite(pBmpBuf[i][j],sizeof(char),(infoHead.biBitCount/8),fp);
                fwrite(pBmpBuf[i][j],sizeof(char),(infoHead.biBitCount/8),fp);
            }
        }
    }
    if(times==0.5){
        for(int i=0;i<bmpheight;i = i+2){
            for(int j=0;j<bmpwidth;j = j+2){
//            cout<<i<<j;
                fwrite(pBmpBuf[i][j],sizeof(char),infoHead.biBitCount/8,fp);
            }
        }
    }


//    cout<<(int)pBmpBuf[0];
    fclose(fp);
}
void solve()
{
    char readFileName[] = "..\\lena_C.bmp";
    if(FALSE == readBmp(readFileName))
        cout<<"readfile error!"<<endl;
    writeBmp();
}

int main()
{
    solve();

    return 0;
}
