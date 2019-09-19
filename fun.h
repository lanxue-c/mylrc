#ifndef __FUN_H__
#define __FUN_H__

    typedef struct lrc{
        int time_min;//歌词时间分钟
        int time_sec;//歌词时间秒
        int time_ms;//歌词时间 （10ms）为单位
        char buf[256];//歌词内容
        struct lrc *next;//下一链表地址
    }LRC;

    extern int filelrc(LRC **head,char *filename);//打开文件，读取文件内容，并有序链表插入
    extern int insert_link(LRC **head,LRC *new_lrc);//链表的有序插入
    extern int free_link(LRC *head);//链表的释放
    extern LRC *myprint(LRC *head,int num);//输出链表的内容
    extern int screenfrnme(int width);//屏幕格式
    extern int lrctimeprit(LRC *head,int width);//时间比较，屏幕输出//时间单位为10ms
    extern int printspeed(LRC *head,int mytime,int width);//进度条动画//光碟动画
    extern int songsheet(int width,char **filesong,int numstr);//歌单界面

#endif