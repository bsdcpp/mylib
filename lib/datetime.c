#include "jlib.h"

/*********************
 * 获取月份(Month) MM
 * param: cSysMonth-月份
 **********************/
void GetSysMonth(char *cSysMonth)
{
    time_t tTimer;
    struct tm *tBlock = NULL;

    tTimer = time(NULL);
    tBlock = localtime(&tTimer);

    sprintf(cSysMonth, "%02d", tBlock->tm_mon + 1);
}

/*********************
 * 获取上月份(Month) MM
 * param: cSysMonth-月份
 **********************/
void GetSysLastMonth(char *cSysMonth)
{
    time_t tTimer;
    int sysMon;
    struct tm *tBlock = NULL;

    tTimer = time(NULL);
    tBlock = localtime(&tTimer);

    sysMon = tBlock->tm_mon;

    sprintf(cSysMonth, "%02d", (sysMon == 0) ? 12 : sysMon);
}

/****************************
 *  获取完整系统时间
 *  格式：yyyy-mm-dd  hh:mm:ss 
 *****************************/
void GetSysTime(char *ods_datetime)/*yyyy-mm-dd  hh:mm:ss*/
{
    struct tm *newtime = NULL;
    time_t long_time;
    time( &long_time );/*取一天的时间*/
    newtime = localtime( &long_time );/*把日期和时间转变为结构*/
    sprintf(ods_datetime,"%04d-%02d-%02d  %02d:%02d:%02d",newtime->tm_year + 1900,
            newtime->tm_mon+1,newtime->tm_mday,
            newtime->tm_hour,newtime->tm_min,newtime->tm_sec);
}

/****************************
 * 获取当天日期 YYYYMMDD
 * param: cSysDate-当天日期
 *****************************/
void GetSysDate(char *cSysDate)
{
    time_t tTimer;
    struct tm *tBlock = NULL;

    tTimer = time(NULL);
    tBlock = localtime(&tTimer);

    sprintf(cSysDate, "%04d%02d%02d", tBlock->tm_year + 1900, tBlock->tm_mon + 1, tBlock->tm_mday);
}

/**************************
 * 获取前一天日期 YYYYMMDD
 * param: cSysDate-前一天日期
 ***************************/
void GetLastDate(char *cSysDate)
{
    time_t tTimer;
    struct tm *tBlock = NULL;

    tTimer = time(NULL)-24*3600;
    tBlock = localtime(&tTimer);

    sprintf(cSysDate, "%04d%02d%02d", tBlock->tm_year + 1900, tBlock->tm_mon + 1, tBlock->tm_mday);
}

/* 
 * 写日志 
 * msg:记录的内容
 * type: business type[ plus date type ]
 * */
void writeLog(char *msg, char *type)
{
    FILE *fp;
    char yyyymmdd[20+1];
    char filePath[300+1];
    memset(filePath, 0, sizeof(filePath));
    memset(yyyymmdd, 0, sizeof(yyyymmdd));

    GetSysDate(yyyymmdd);

    sprintf(filePath, "%s/err_%s_%s.log", LOG_PATH, type, yyyymmdd);
    fp = fopen(filePath, "a");
    if (fp) {
        strncat(msg, "\n", 1);
        fwrite(msg, strlen(msg),1, fp);
        fclose(fp);
    } else {
        more_msg("cannot write log");
    }
}

