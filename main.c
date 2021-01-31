


#include "stdio.h"
#include "conio.h"
#include "stdlib.h"

/*************字典*************/
/*
优先级：priority
大小：size
任务：task
状态：state
标记：sign
挂起：suspend
内存：RAM
*/
struct PCB_type
{
    int pid;                        //pid设定
    int priority;              //优先级
    int size;                //大小
    char task[10];         //进程任务内容
    int state;                      /* 0表示不在内存，1表示在内存，2表示挂起*/
    int child_sign;                         //标记子进程
    int fds_0;                          //0表示无权限，1表示有权限，2表示未被pipe调用，即无效状态
    int fds_1;                       //文件描述符************fds[0]读取数据，fds[1]写入数据
    char msg[20];               //进程内信息传递用
};
struct PCB_type RAM[20];             //定义20个”内存“的结构体
int num=0,suspend=0,sign,i,j,pid;               /*定义相关参数变量  num用于这里的空间存储判定
                    /创建新进程****/
void create()
{
    if(num>=20) /*判断是否有存储空间*/
    printf("\n 内存已满，请删除或挂起其他程序");
    else{
    for(i=0;i<20;i++)
        if(RAM[i].state==0) break; /*按顺序在内存中寻找创建新进程的空间*/
    printf("\n请输入新建进程的pid值\n");
    scanf("%d",&RAM[i].pid);
    for(j=0;j<i;j++) /*判断是否之前已存储相同pid进程*/
        if(RAM[j].pid==RAM[i].pid)
        {
            printf("\n该进程已存在于内存中");
            return;
        }
    printf("\n请输入新进程的优先级 \n" );
    scanf("%d",&RAM[i].priority);
    printf("\n请输入新进程的大小\n" );
    scanf("%d",&RAM[i].size);
    printf("\n请输入新进程的内容\n" );
    scanf("%s",&RAM[i].task);
    RAM[i].child_sign=0 ;
    RAM[i].state=1; /*将进程状态设为1，表示在内存中*/
    RAM[i].fds_0=2;//读取权限无效状态
    RAM[i].fds_1=2;//赋值权限无效状态
    num++;/*内存中进程数目加一*/
    }
}
/*查看当前运行进程******/
void run()
{
    sign=0;
    for(i=0;i<20;i++)
    if(RAM[i].state==1) /*若进程状态设为1，表示在内存中，输出查看*/
{
    printf("\n pid=%d   |",RAM[i].pid);
    printf(" priority=%d   |",RAM[i].priority);
    printf(" size=%d   |",RAM[i].size);
    printf(" task=%s   |",RAM[i].task);
    printf(" child_sign=%d   |",RAM[i].child_sign);
    if(RAM[i].fds_0==1)printf("读取权限已授权，");
    else if(RAM[i].fds_0==0)printf("读取权限未授权，");
    else if(RAM[i].fds_0==2) printf("读取权限未激活，");
    if(RAM[i].fds_1==1)printf("写入权限已授权。");
    else if(RAM[i].fds_1==0)printf("写入权限未授权。");
    else if(RAM[i].fds_1==2)printf("写入权限未激活。");
    sign=1;/*创建成功，标记为1 */
}
if(sign==0)
    printf("\n当前没有运行该进程\n" );/*标记为0，表示该进程未创建于内存中*/

}
/*换出进程*******/

void huanchu()
{
    if(num==0) /*num=0,表示内存中无进程*/
    {
    printf("\n当前没有运行的进程\n" );
    return;
    }
    printf("\n请输入要换出的进程的pid值\n" );
    scanf("%d",&pid);
    sign=0;
    for(i=0;i<20;i++)
    {
        if(pid==RAM[i].pid)
        {
            if(RAM[i].state==1)
            {
            RAM[i].state=2; /*将该进程的状态设为挂起*/
            suspend++;
            printf("\n该进程已成功挂起换出\n" );
            }
            else if(RAM[i].state==0)
                printf("\n要换出的进程不存在\n" );
            else printf("\n要换出的进程已经被挂起\n" );
            sign=1;
            break;
        }
    }
    if(sign==0) printf("\n要换出的进程不存在\n" );
}


//杀死进程*******/
void kill()
{
    if(num==0)
    {
         printf("\n当前没有运行的进程\n" );
        return;
    }
    printf("\n请输入要杀死的进程的pid值\n" );
    scanf("%d",&pid);
    sign=0;
    for(i=0;i<20;i++)
    {
        if(pid==RAM[i].pid)
        {
            if(RAM[i].state==1)
            {
                RAM[i].state=0; //将该进程的状态设为不在内存中/
                num--; //内存中的进程数目减少一个/
                printf("\n该进程已成功杀死\n" );
            }
            else if(RAM[i].state==0)
                printf("\n要杀死的进程不存在\n" );
            else printf("\n要杀死的进程已经被挂起\n" );/*剩余状态为2，挂起*/
            sign=1;
            break;
        }
    }
    if(sign==0) printf("\n要杀死的进程不存在\n" );
}
/*唤醒进程*******/
void wake_up()
{
    if(num==0)
    {
        printf("当前没有运行的进程\n");
        return;
    }
    if(suspend==0) //suspend=0,表示没有挂起的进程/
    {
        printf("\n当前没有换出的进程\n");
        return;
    }
    printf("\n请输入要唤醒的进程的pid值:\n");
    scanf("%d",&pid);
    for(i=0;i<20;i++)
        {
        if(pid==RAM[i].pid)
            {
            if(RAM[i].state==2)
            {
                RAM[i].state=1; //将该进程的状态设为挂起/
                suspend--;
                num++;
                printf("\n该进程已成功唤醒\n" );
            }
            else if(RAM[i].state==0)
            printf("\n要唤醒的进程不存在\n" );
            else printf("\n要唤醒的进程已经在内存中\n" );
            }
        }
}
void winfork()//fork函数模拟--行为模式：子进程就返回0，非子进程返回该进程的子进程的pid
{
    /**********************判断结构***************/
    if(num==0)
    {
        printf("当前没有运行的进程,无法进行fork（）行为\n");
        return;
    }
    if (num==20)
    {
        printf("fork error!");
        return;//内存没空间创建子进程
    }
    /*******************************************/
    /**********模拟复制内容部分*********/

    printf("\n请输入要执行fork（）的进程的pid值:\n");    //选择父进程
    scanf("%d",&pid);                                           //此时的pid是父进程的pid
    sign=0;
    for(i=0;i<20;i++)
        {
        if(RAM[i].pid==pid)                                  //在20个内存里查找pid符合条件的父进程
            {
            if(RAM[i].child_sign==0)                                     //pid符合条件且其下没有子进程
                {
                printf("\n请输入子进程的pid\n");          //对新建的子进程的pid赋值
                scanf("%d",&RAM[num+1].pid);
                for(j=0;j<num;j++) /*判断是否之前已存储相同pid进程*/
                if(RAM[j].pid==RAM[num+1].pid)
                    {
                    printf("\n该进程pid已存在于内存中");
                    return;
                    }
                RAM[num+1].priority=RAM[i].priority;
                RAM[num+1].size=RAM[i].size;
                strcpy(RAM[num+1].task,RAM[i].task);
                RAM[num+1].state=1; /*将进程状态设为1，表示在内存中*/
                RAM[num+1].child_sign=0;
                RAM[i].child_sign=RAM[num+1].pid;
                if(RAM[i].fds_0!=2){//权限同步
                    RAM[num+1].fds_0=RAM[i].fds_0;
                    RAM[num+1].fds_1=RAM[i].fds_1;
                }
                num++;/*内存中进程数目加一*/
                sign=1;
                return;
                }
            }
        }
        if(sign==0) printf("\n执行失败！\n" );
}
//**************************************************进程间通信**********************************************//
/***********以下流程有关于函数实现pipe建立管道后建立子进程，并关闭父进程的
    管道本质为内核缓冲区，用户态来看是一个伪文件
    由于是伪文件，所以对于管道有两个文件描述符引用，一个表示读端，一个表示写端
    管道是基于字节流实现通信的
    管道依赖于文件系统，其生命周期随进程
    管道本身自带同步互斥
    fork()进程的时候，子进程会继承父进程的管道与其描述符
    单方向流动，半双工通信类型，且只作用于有血缘关系的进程之间
    *****************/
void communication()
{
     if(num==0)
    {
        printf("当前没有运行的进程,无法进行进程通信行为\n");
        return;
    }
    int f_pid,c_pid;//进行进程通信的进程的id(父子)
    char buf[20];//缓存区
    printf("\n 请输入父进程 pid\n");
    scanf("%d",&f_pid);//获取父进程pid

    /************对父进程的确认**********/
     for(i=0;i<20;i++)
        {
        if(RAM[i].pid==f_pid)                                  //在20个内存里查找pid符合条件的父进程
            {
            if(RAM[i].child_sign==0)                                     //pid符合条件且其下没有子进程建立管道
                {
                    if (num==20)return;//无进程空间建立子进程//
                    RAM[i].fds_0=1;     //激活读取权限
                    printf("进程（pid=%d）的读取权限已授权，",RAM[i].pid);
                    RAM[i].fds_1=1;     //激活写入权限
                    printf("进程（pid=%d）的写入权限已授权\n",RAM[i].pid);
                    /******为进程建立子进程并激活权限***********/
                    printf("\n请输入子进程的pid\n");          //对新建的子进程的pid赋值
                    scanf("%d",&RAM[num+1].pid);
                    for(j=0;j<num;j++) /*判断是否之前已存储相同pid进程*/
                    if(RAM[j].pid==RAM[num+1].pid)
                        {
                        printf("\n该pid已被占用");
                        return;
                        }
                    RAM[num+1].priority=RAM[i].priority;
                    RAM[num+1].size=RAM[i].size;
                    strcpy(RAM[num+1].task,RAM[i].task);
                    RAM[num+1].state=1; /*将进程状态设为1，表示在内存中*/
                    RAM[num+1].child_sign=0;
                    RAM[i].child_sign=RAM[num+1].pid;
                    if(RAM[i].fds_0!=2){//权限同步
                        RAM[num+1].fds_0=RAM[i].fds_0;
                        RAM[num+1].fds_1=RAM[i].fds_1;
                        printf("进程（pid=%d）的读取权限已同步，",RAM[num+1].pid);
                        printf("进程（pid=%d）的写入权限已同步\n",RAM[num+1].pid);
                        RAM[i].fds_0=0;
                        printf("进程（pid=%d）的读取权限已关闭，",RAM[i].pid);
                        RAM[num+1].fds_1=0;//关闭父进程的读段，关闭子进程的写段
                        printf("进程（pid=%d）的写入权限已关闭\n",RAM[num+1].pid);
                    }
                    num++;
                    sign=1;
//利用缓存区交流通信
                    printf("输入通信信息");
                    scanf("%s",RAM[i].msg);//只有父进程写入
                    strcpy(buf,RAM[i].msg);//交换进入缓存区
                    strcpy(RAM[num+1].msg,buf);//由缓存区传递至子进程
                    printf("\n 通信的信息是:%s\n",RAM[num+1].msg);//通过子进程读取信息
                    strcpy(buf,"");//清空缓存管道内的信息（只读一次销毁机制）
                    return;
                }
            else{/********为已有子进程激活权限********/
                    RAM[i].fds_0=1;     //激活读取权限
                    printf("进程（pid=%d）的读取权限已授权，",RAM[i].pid);
                    RAM[i].fds_1=1;     //激活写入权限
                    printf("进程（pid=%d）的写入权限已授权\n",RAM[i].pid);
                    c_pid=RAM[i].child_sign;//取得子进程的pid
                      for(j=0;j<20;j++)
                            {
                                if(RAM[j].pid==c_pid){
                                RAM[j].fds_0=RAM[i].fds_0;
                                RAM[j].fds_1=RAM[i].fds_1;
                                printf("进程（pid=%d）的读取权限已同步，",RAM[j].pid);
                                printf("进程（pid=%d）的写入权限已同步\n",RAM[j].pid);
                                RAM[i].fds_0=0;
                                printf("进程（pid=%d）的读取权限已关闭，",RAM[i].pid);
                                RAM[j].fds_1=0;
                                printf("进程（pid=%d）的写入权限已关闭\n",RAM[j].pid);
                                }//关闭父进程的读段，关闭子进程的写段
                        }
                    printf("输入通信信息");
                    scanf("%s",RAM[i].msg);//只有父进程写入
                    strcpy(buf,RAM[i].msg);//交换进入缓存区
                    strcpy(RAM[num+1].msg,buf);//由缓存区传递至子进程
                    printf("\n 通信的信息是:%s\n",RAM[num+1].msg);//通过子进程读取信息
                     strcpy(buf,"");//清空缓存管道内的信息（只读一次销毁机制）
                    return;
            }
            }
            else {
                printf("该pid的进程不存在于内存！");
                return;
            }
        }
}
//主程序*******/
void main()
{
    int option,n=1,i;
    for(i=0;i<20;i++)
    RAM[i].state=0; //使所有进程都初始设为不在内存中/
    while(n)
        {
        printf("\n*****************************************");
        printf("\n*    win系统下模拟进程控制演示系统    *");
        printf("\n*****************************************");
        printf("\n |1.创建新的进程      2.查看运行进程    |");
        printf("\n |3.换出某个进程      4.杀死运行进程    |");
        printf("\n |5.唤醒某个进程      6.win下模拟fork   |");
        printf("\n |7.通信             8.退出               |");
        printf("\n*****************************************");
        printf("\n请选择(1～8)");
        scanf("%d",&option);
        switch(option)
            {
                case 1: create( ); break;
            case 2 :run(); break;
            case 3 :huanchu();break;
            case 4 :kill();break;
            case 5 :wake_up();break;
            case 6: winfork();break;
            case 8 :exit(0);
            case 7:communication();break;
            default: n=0;
            break;
            }
        }
}
