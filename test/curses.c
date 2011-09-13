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
    /* 1. ��Ϊ���� */
    /*if( NumberCheck(date)==FALSE )
      return 0xf050;*/

    /* 2. �����1990��2100֮�� */
    memset(year,0,5);
    strncpy(year,date,4);
    if(atoi(year)<1990 || atoi(year)>2100) 
        return(false); /*0xf050;*/

    /* 3. ��Ϊ1��3��5��7��8��10��12ʱ�������Ϊ31��
       ��Ϊ4��6��9��11ʱ�������Ϊ30��
       ��Ϊ2ʱ����Ϊ���꣬�����Ϊ28������Ϊ���꣬�����Ϊ29��
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

    /* 2. �����1990��2100֮�� */
    memset(year,0,5);
    strncpy(year,mon,4);
    if(atoi(year)<1990 || atoi(year)>2100) 
        return(false); /*0xf050;*/


    memset(month,0,3);
    strncpy(month,&mon[4],2);
    if(atoi(month)<1 || atoi(month)>12) 
        return(false); /*0xf050;*/

}

/* date��ʽΪ yyyymmdd ,���ӳ����ж϶��µ���������Ϊ
   ���꣬�򷵻�29������Ϊ���꣬�򷵻�28���ر�ע�⣺�˷���
   ֻ��1990��2100���á�*/
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

/*���ܼ�������*/ 
void keyboard_input(char input_str[COMMAND_LEN])/*�����������*/
{
    int	k;
    char	input_key;
    k=0;
    while(1)
    {
        input_key = getch();
        if (input_key == '\n')/*�س���ʾ�������*/
        {
            input_str[k] = '\0';
            break;
        }
        else if (input_key == '\b')/*�����˸��*/
        {
            k--;
        }
        else/*����ȫ������*/
        {
            input_str[k] = input_key;
            k++;
        }
        printf("%c",input_key);/*����������ݷ�������׼�����*/
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

    initscr();  /*����curses ģʽ*/
    noecho(); /*�û����õ�����������Ӧ�����ֳ�Ĭ*/

    do
    {
        clear();
        move(10,0);
        refresh();
        printf("\r��ѡ��Ҫ�����ҵ�� ");	  
        move(12,0);
        refresh();
        printf("\r������(1->ISP��2->VOC)��");
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
        printf("\r��ѡ��Ҫ���������Դ��");	  
        move(12,0);
        refresh();
        printf("\r������(1->��ʽ���ݣ�2->Ԥ������)��");
        refresh();    
        keyboard_input(cp);
    }while ( strcmp(cp,"1")!=0 && strcmp(cp,"2")!=0);
    strcat(cmd_line,cp);

    if (busi_flag==1)
    {
        fd=0;
        do /*��������,��Ҫ���ʽ��ȷ*/
        {
            clear();
            move(10,0);
            refresh();
            if (fd > 0) printf("\rISP�·ݸ�ʽ����ȷ,�������룡������Ҫ������ļ��·ݣ�(yyyymm)��");
            else        printf("\r������Ҫ������ļ��·ݣ�(yyyymm)��");
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
        do /*������ʼ����,��Ҫ���ʽ��ȷ*/
        {
            clear();
            move(10,0);
            refresh();
            if (fd > 0) printf("\rVOC���ڸ�ʽ����ȷ,�������룡������Ҫ������ļ���ʼ���ڣ�(yyyymmdd)��");
            else        printf("\r������Ҫ������ļ���ʼ���ڣ�(yyyymmdd)��");
            refresh();
            keyboard_input(checkdate);
            fd++;
        }while (checkDate(checkdate)==false);
        strcat(cmd_line," ");
        strcat(cmd_line,checkdate);	  
    }

    do /*������ʼ����,��Ҫ���ʽ��ȷ*/
    {
        clear();
        move(10,0);
        refresh();
        printf("\r������Ҫ�����ҵ��");
        refresh();  
        keyboard_input(busi_type);
    }while(0);
    strcat(cmd_line," ");
    strcat(cmd_line,busi_type);	  
    printf("%s\n",cmd_line);
    endwin(); /*�ر�curses ģʽ*/

}
