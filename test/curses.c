#include <math.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h> 
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h> 
#include <memory.h>
#include <sys/types.h>
#include <ctype.h>
#include <curses.h>

#include <fcntl.h>
#include <dirent.h>
#include <curses.h>
#include <sys/dir.h>
#include <term.h>

#define	COMMAND_LEN		500

char checkdate[20];
char checkmon[10];

#define false 0
#define true 1

int checkDate(char *date)
{   
    char year[5],month[3],day[3];
    int  Maxdate;
    /* 1. 均为数字 */
    /*if( NumberCheck(date)==FALSE )
      return 0xf050;*/

    /* 2. 年号在1990和2100之间 */
    memset(year,0,5);
    strncpy(year,date,4);
    if(atoi(year)<1990 || atoi(year)>2100) 
        return(false); /*0xf050;*/

    /* 3. 月为1，3，5，7，8，10，12时，最大日为31；
       月为4，6，9，11时，最大日为30；
       月为2时，若为闰年，最大日为28；若不为闰年，最大日为29。
       */
    memset(month,0,3);
    strncpy(month,&date[4],2);
    memset(day,0,3);
    strncpy(day,&date[6],2);

    switch(atoi(month))
    {   
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if(atoi(day)<1||atoi(day)>31)	
                return(false); 
            else
                return(true); 
        case 2:
            Maxdate=Feb(date);/*year*/
            if(atoi(day)<1||atoi(day)>Maxdate)	
                return(false); 
            else
                return(true); 
        case 4:
        case 6:
        case 9:
        case 11:
            if(atoi(day)<1||atoi(day)>30)	
                return(false); 
            else
                return(true); 
        default:
            return(false); 
    }
}
int checkMon(char *mon)
{   
    char year[5],month[3];

    /* 2. 年号在1990和2100之间 */
    memset(year,0,5);
    strncpy(year,mon,4);
    if(atoi(year)<1990 || atoi(year)>2100) 
        return(false); /*0xf050;*/


    memset(month,0,3);
    strncpy(month,&mon[4],2);
    if(atoi(month)<1 || atoi(month)>12) 
        return(false); /*0xf050;*/

}

/* date格式为 yyyymmdd ,本子程序判断二月的天数，若为
   闰年，则返回29；若不为闰年，则返回28。特别注意：此方法
   只对1990～2100适用。*/
int Feb(char *date)
{
    int  year;
    /*for(i=0;i<4;i++)
      if(isdigit(date[i])==0) 
      return FALSE;
      */
    year=(int)((date[3]-'0')+10*(date[2]-'0')+
            100*(date[1]-'0')+1000*(date[0]-'0')); 
    if(year%4==0)    return 29;
    else             return 28;				   
}

/*接受键盘输入*/ 
void keyboard_input(char input_str[COMMAND_LEN])/*读入键盘输入*/
{
    int	k;
    char	input_key;
    k=0;
    while(1)
    {
        input_key = getch();
        if (input_key == '\n')/*回车表示输入完毕*/
        {
            input_str[k] = '\0';
            break;
        }
        else if (input_key == '\b')/*遇到退格键*/
        {
            k--;
        }
        else/*其他全部读入*/
        {
            input_str[k] = input_key;
            k++;
        }
        printf("%c",input_key);/*将读入的内容返回至标准输出上*/
        refresh();
    }
}

int main(int argc ,char * argv[])
{
    char cmd_line[COMMAND_LEN];
    char cp[2],busi_type[10];
    int busi_flag;
    int fd;
    memset(busi_type,0,10);
    memset(cmd_line,0,COMMAND_LEN);

    initscr();  /*启动curses 模式*/
    noecho(); /*用户所敲的输入无所回应，保持沉默*/

    do
    {
        clear();
        move(10,0);
        refresh();
        printf("\r请选择要处理的业务： ");	  
        move(12,0);
        refresh();
        printf("\r请输入(1->ISP；2->VOC)：");
        refresh();    
        keyboard_input(cp);
    } while ( strcmp(cp,"1")!=0 && strcmp(cp,"2")!=0);
    busi_flag=atoi(cp);
    sprintf(cmd_line,"%s %s ","/LJCJ_FS2/USERS/ycl01/voc_isp/bin",cp);

    do
    {
        clear();
        move(10,0);
        refresh();
        printf("\r请选择要处理的数据源：");	  
        move(12,0);
        refresh();
        printf("\r请输入(1->正式数据；2->预估数据)：");
        refresh();    
        keyboard_input(cp);
    }while ( strcmp(cp,"1")!=0 && strcmp(cp,"2")!=0);
    strcat(cmd_line,cp);

    if (busi_flag==1)
    {
        fd=0;
        do /*输入日期,并要求格式正确*/
        {
            clear();
            move(10,0);
            refresh();
            if (fd > 0) printf("\rISP月份格式不正确,重新输入！请输入要处理的文件月份！(yyyymm)：");
            else        printf("\r请输入要处理的文件月份！(yyyymm)：");
            refresh();
            keyboard_input(checkmon);
            fd++;
        }while (checkMon(checkmon)==false);
        strcat(cmd_line," ");
        strcat(cmd_line,checkmon);
    }
    else
    {
        fd=0;
        do /*输入起始日期,并要求格式正确*/
        {
            clear();
            move(10,0);
            refresh();
            if (fd > 0) printf("\rVOC日期格式不正确,重新输入！请输入要处理的文件起始日期！(yyyymmdd)：");
            else        printf("\r请输入要处理的文件起始日期！(yyyymmdd)：");
            refresh();
            keyboard_input(checkdate);
            fd++;
        }while (checkDate(checkdate)==false);
        strcat(cmd_line," ");
        strcat(cmd_line,checkdate);	  
    }

    do /*输入起始日期,并要求格式正确*/
    {
        clear();
        move(10,0);
        refresh();
        printf("\r请输入要处理的业务：");
        refresh();  
        keyboard_input(busi_type);
    }while(0);
    strcat(cmd_line," ");
    strcat(cmd_line,busi_type);	  
    printf("%s\n",cmd_line);
    endwin(); /*关闭curses 模式*/

}
