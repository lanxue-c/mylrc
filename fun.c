#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"fun.h"
#include"console.h"

//打开文件，读取文件内容，并有序链表插入
int filelrc(LRC **head,char *filename)
{
    int i = 0;

    //打开文件读取内容
    FILE *fp = fopen(filename,"r");
    if(!fp)
    {
        perror("fopenfile");
        return -1;
    }
    fseek(fp,0,SEEK_END);
    int len = ftell(fp);
    rewind(fp);
    char *buf = (char *)calloc(1,len+1);
    fread(buf,len,1,fp);
    fclose(fp);

    //格式化读取文件前4行
    char *p_buf = strtok(buf,"\r\n");
    if(!p_buf)
    {
        perror("strtok");
        return -2;
    }
    LRC *new_lrc = (LRC *)calloc(1,sizeof(LRC));
    sscanf(p_buf,"%*[^:]:%[^]]",new_lrc->buf);
    insert_link(head,new_lrc);
    for(i = 1; i < 4; i++)
    {
        p_buf = strtok(NULL,"\r\n");
        if(!p_buf)
        {
            perror("strtok");
            return -2;
        }
        new_lrc = (LRC *)calloc(1,sizeof(LRC));
        sscanf(p_buf,"%*[^:]:%[^]]",new_lrc->buf);
        insert_link(head,new_lrc);
    }

    //格式化读取后面歌词
    while(1)
    {
        char strbuf[256] = "";
        p_buf = strtok(NULL,"\r\n");
        if(!p_buf)
        {
            break;
        }
        char *str = strrchr(p_buf,']');
        if(!str)
        {
            break;
        }
        new_lrc = (LRC *)calloc(1,sizeof(LRC));
        strcpy(new_lrc->buf,(str+1));
        strcpy(strbuf,(str+1));
        str = strchr(p_buf,'[');
        sscanf(str,"[%d:%d.%d]",&new_lrc->time_min,&new_lrc->time_sec,&new_lrc->time_ms);
        insert_link(head,new_lrc);
        str = strchr(str+1,'[');
        while(str)
        {
            new_lrc = (LRC *)calloc(1,sizeof(LRC));
            strcpy(new_lrc->buf,strbuf);
            sscanf(str,"[%d:%d.%d]",&new_lrc->time_min,&new_lrc->time_sec,&new_lrc->time_ms);
            insert_link(head,new_lrc);
            str = strchr(str+1,'[');
        }
    }

    free(buf);
}

//链表的有序插入
int insert_link(LRC **head,LRC *new_lrc)
{
    if(*head == NULL)
    {
        *head = new_lrc;
        return 0;
    }
    LRC *move = *head;
    LRC *formove = NULL;
    while(move)
    {
        if(move->time_min > new_lrc->time_min)
        {
            if(!formove)
            {
                new_lrc->next = *head;
                *head = new_lrc;
            }
            else
            {
                formove->next = new_lrc;
                new_lrc->next = move;
            }
            return 0;
        }
        else if(move->time_min == new_lrc->time_min)
        {
            if(move->time_sec > new_lrc->time_sec)
            {
                if(!formove)
                    {
                        new_lrc->next = *head;
                        *head = new_lrc;
                    }
                    else
                    {
                        formove->next = new_lrc;
                        new_lrc->next = move;
                    }
                    return 0;
            }
            else if(move->time_sec == new_lrc->time_sec)
            {
                if(move->time_ms > new_lrc->time_ms)
                {
                    if(!formove)
                    {
                        new_lrc->next = *head;
                        *head = new_lrc;
                    }
                    else
                    {
                        formove->next = new_lrc;
                        new_lrc->next = move;
                    }
                    return 0;
                }
            }
        }
        formove = move;
        move = move->next;
    }
    formove->next = new_lrc;

    return 0;
}

int free_link(LRC *head)//链表的释放
{
    LRC *move = head;
    while(move)
    {
        head = move->next;
        free(move);
        move = head;
    }

    return 0;
}

//屏幕格式
int screenfrnme(int width)
{
    int i = 0;
    system("clear");
    set_fg_color(COLOR_CYAN);
    cusor_moveto(width+1,1);
    printf("╔");
    cusor_moveto(width+50,1);
    printf("╗");
    cusor_moveto(width+2,1);
    for(i = 0; i < 48;i++)
        printf("═");
    for(i = 0; i < 14; i++)
    {
        cusor_moveto(width+1,2+i);
        printf("║");
        cusor_moveto(width+50,2+i);
        printf("║");
    }
    cusor_moveto(width+1,16);
    printf("╚");
    cusor_moveto(width+50,16);
    printf("╝");
    cusor_moveto(width+2,16);
    for(i = 0; i < 48;i++)
        printf("═");
    cusor_moveto(width+2,13);
    for(i = 0; i < 48;i++)
        printf("_");
    cusor_moveto(width+18,15);
    printf("◀       ■      ▶");
    cusor_moveto(width+3,3);
    printf("   ∴∷∴ ");
    cusor_moveto(width+3,4);
    printf("  ∶∷◉∷∶ ");
    cusor_moveto(width+3,5);
    printf("   ∵∷∵ ");
    fflush(stdout);
}

//时间比较，屏幕输出//时间单位为10ms
int lrctimeprit(LRC *head,int width)
{
    int i = 4,k = 0, j = 0;
    int mytime = 0;
    int mytimemin = 0,mytimesec = 0,mytimems = 0;
    int lrctimemin = 0,lrctimesec = 0,lrctimems = 0;
    struct timeval time_start, time_end;
    LRC *move = head;

    if(!move)
        return -1;
    cusor_hide();
    set_fg_color(COLOR_CYAN);
    cusor_moveto(width+20,2);
    printf("歌名：%s",move->buf);
    move = move->next;
    cusor_moveto(width+20,3);
    printf("歌手：%s",move->buf);
    move = move->next;
    cusor_moveto(width+20,4);
    printf("专辑：%s",move->buf);
    move = move->next;
    cusor_moveto(width+20,5);
    printf("制作：%s",move->buf);
    cusor_moveto(width+1,1);
    gettimeofday(&time_start, NULL);
    lrctimemin = myprint(head,4)->time_min;
    lrctimesec = myprint(head,4)->time_sec;
    lrctimems = myprint(head,4)->time_ms;
    if(myprint(head,4))
    {
        cusor_moveto(width+22-strlen(myprint(head,i)->buf)/5,9);
        printf("%s",myprint(head,i)->buf);
    }
    if(myprint(head,5))
    {
        cusor_moveto(width+22-strlen(myprint(head,i+1)->buf)/5,10);
        printf("%s",myprint(head,i+1)->buf);
    }
    if(myprint(head,6))
    {
        cusor_moveto(width+22-strlen(myprint(head,i+2)->buf)/5,11);
        printf("%s",myprint(head,i+2)->buf);
    }
    fflush(stdout);

    //逐个歌词时间比较
    while(1)
    {
        gettimeofday(&time_end, NULL);
        mytime = (time_end.tv_sec - time_start.tv_sec)*100 + (time_end.tv_usec - time_start.tv_usec)/10000;
        cusor_moveto(width+24,6);
        mytimemin = mytime/6000;
        mytimesec = mytime/100%60;
        mytimems = mytime%100;
        set_fg_color(COLOR_RED);
        printf("%02d:%02d",mytimemin,mytimesec);
        printspeed(head,mytime,width);
        set_fg_color(COLOR_CYAN);
        if((mytimemin >= lrctimemin) && (mytimesec >= lrctimesec) && (mytimems >= lrctimems))
        {
            for(k = 0; k < 5;k++)
            {
                cusor_moveto(width+2,k+7);
                for(j = 0; j < 48;j++)
                    printf(" ");
            }   
            if(myprint(head,i-2))
            {
                cusor_moveto(width+22-strlen(myprint(head,i-2)->buf)/5,7);
                printf("%s",myprint(head,i-2)->buf);
            }
            if(myprint(head,i-1))
            {
                cusor_moveto(width+22-strlen(myprint(head,i-1)->buf)/5,8);
                printf("%s",myprint(head,i-1)->buf);
            }
            if(myprint(head,i))
            {
                cusor_moveto(width+22-strlen(myprint(head,i)->buf)/5,9);
                printf("%s",myprint(head,i)->buf);
            }
            if(myprint(head,i+1))
            {
                cusor_moveto(width+22-strlen(myprint(head,i+1)->buf)/5,10);
                printf("%s",myprint(head,i+1)->buf);
                lrctimemin = myprint(head,i+1)->time_min;
                lrctimesec = myprint(head,i+1)->time_sec;
                lrctimems = myprint(head,i+1)->time_ms;
            }
            if(myprint(head,i+2))
            {
                cusor_moveto(width+22-strlen(myprint(head,i+2)->buf)/5,11);
                printf("%s",myprint(head,i+2)->buf);
            }
            i++;
        }

        //歌词逐字显示
        if(i > 4 && myprint(head,i) != NULL && myprint(head,i-1) != NULL)
        {
            int lrctime = 0,forlrctime = 0, len = 0, time_str = 0, j = 0,k = 0;
            lrctime = myprint(head,i)->time_min*6000 + myprint(head,i)->time_sec*100 + myprint(head,i)->time_ms;
            forlrctime = myprint(head,i-1)->time_min*6000 + myprint(head,i-1)->time_sec*100 + myprint(head,i-1)->time_ms;
            len = strlen(myprint(head,i-1)->buf);
            time_str = (lrctime - forlrctime)/len;
            set_fg_color(COLOR_RED);
            cusor_moveto(width+22-len/5,9);
            while(mytime >= forlrctime+time_str*j)
                j++;
            for(k = 0;k < j;k++)
            {
                printf("%c",(myprint(head,i-1)->buf)[k]);
            }
        }
        fflush(stdout);
        usleep(100);

        //最后一行歌词单独处理
        if(myprint(head,i) == NULL)
        {
            int len = 0, j = 0;
            len = strlen(myprint(head,i-1)->buf);
            while(1)
            {   
                j++;
                if(j > len)
                    break;
                set_fg_color(COLOR_RED);
                cusor_moveto(width+22-len/5,9);
                for(k = 0;k < j;k++)
                {
                    printf("%c",(myprint(head,i-1)->buf)[k]);
                }
                fflush(stdout);
                usleep(150*1000);
            }
            sleep(1);
            break;
        }
    }
    cusor_show();
    set_fg_color(COLOR_WHITE);
    cusor_moveto(1,17);

    return 0;
}

//输出链表的内容
LRC *myprint(LRC *head,int num)
{
    int i = 0;
    LRC *move = head;
    if(num < 4)
        return NULL;
    for(i = 0; i < num; i++)
    {
        if(!move)
            return NULL;
        move = move->next;
    }
    return move;
}

//进度条动画//光碟动画
int printspeed(LRC *head,int mytime,int width)
{
    int myspeed = 0;
    LRC *move = head;
    while(move->next)
        move = move->next;
    set_fg_color(COLOR_MAGENTA);
    cusor_moveto(width+2,14);
    printf("%02d:%02d.%02d",mytime/6000,mytime/100%60,mytime%100);
    cusor_moveto(width+45,14);
    printf("%02d:%02d",move->time_min,move->time_sec);
    myspeed = move->time_min*6000 + move->time_sec*100 + move->time_ms;
    myspeed = (int)((float)mytime/(float)myspeed*41);
    cusor_moveto(width+2+myspeed,13);

    //进度条动画
    if(myspeed == 0)
    {
        printf("(");
        if(mytime/100%2 == 0)
        {
            set_fg_color(COLOR_RED);
            printf("❤");
            set_fg_color(COLOR_MAGENTA);
        }  
        else
            printf("❤");  
        printf(" ω ");
        if(mytime/100%2 == 0)
        {
            set_fg_color(COLOR_RED);
            printf("❤");
            set_fg_color(COLOR_MAGENTA);
        }   
        else
            printf("❤");  
        printf(")");
    }
    else
    {
        printf("\bφ(");
        if(mytime/100%2 == 0)
        {
            set_fg_color(COLOR_RED);
            printf("❤");
            set_fg_color(COLOR_MAGENTA);
        }  
        else
            printf("❤");  
        printf(" ω ");
        if(mytime/100%2 == 0)
        {
            set_fg_color(COLOR_RED);
            printf("❤");
            set_fg_color(COLOR_MAGENTA);
        }   
        else
            printf("❤");  
        printf(")");
    }

    //光碟动画
    if(mytime/10%4 == 0)
    {
        set_fg_color(COLOR_RED);
        cusor_moveto(width+8,3);
        printf("∴");
        cusor_moveto(width+6,5);
        printf("∵");
        set_fg_color(COLOR_CYAN);
    }
    else
    {
        set_fg_color(COLOR_WHITE);
        cusor_moveto(width+8,3);
        printf("∴");
        cusor_moveto(width+6,5);
        printf("∵");
    }
    if(mytime/10%4 == 2)
    {
        set_fg_color(COLOR_RED);
        cusor_moveto(width+6,3);
        printf("∴");
        cusor_moveto(width+8,5);
        printf("∵");
        set_fg_color(COLOR_CYAN);
    }
    else
    {
        set_fg_color(COLOR_WHITE);
        cusor_moveto(width+6,3);
        printf("∴");
        cusor_moveto(width+8,5);
        printf("∵");
    }
}

//歌单界面
int songsheet(int width,char **filesong,int numstr)
{
    int num = 0,i = 0;
    set_fg_color(COLOR_CYAN);
    cusor_moveto(width+10,3);
    printf("歌曲列表：");
    for(i = 0; i < numstr; i++)
    {
        cusor_moveto(width+10,5+i);
        printf("%d、%s",i+1,filesong[i]);
    }
    cusor_moveto(width+10,10);
    printf("请输入要播放的歌曲（歌曲前面的序号）:");
    while(num < 1 || num > numstr)
    {
        cusor_moveto(width+47,10);
        printf(" \b");
        cusor_moveto(width+47,10);
        scanf("%d",&num);
    }
    
    return num;
}











