#include<stdio.h>
#include"main.h"
#include"fun.h"

int main(int argc, char const *argv[])
{
    LRC *head = NULL;
    char *filelrcstr[] = {"简单爱.lrc","一些规律.lrc","我们俩.lrc"};//在此添加歌词文件
    char *filesongstr[] = {"简单爱.mp3","一些规律.mp3","我们俩.mp3"};//在此添加歌曲文件
    int numstr = sizeof(filelrcstr)/sizeof(filelrcstr[0]);
    int width = WIDTH;//歌词播放器距左侧的整体宽度，可调整
    int num = 0;

    //屏幕格式
    screenfrnme(width);

    //歌曲曲目的选取
    num = songsheet(width,filesongstr,numstr);

    //打开文件，读取文件内容，并有序链表插入
    if(filelrc(&head,filelrcstr[num-1]))
    {
        printf("歌词显示失败\n");
        return -1;
    }

    //屏幕格式
    screenfrnme(width);

    //播放音乐
    mplayer_play(filesongstr[num-1]);

    //时间比较，屏幕输出
    lrctimeprit(head,width);

    free_link(head);
    return 0;
}

