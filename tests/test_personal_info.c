#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "test.h"
#include "../src/core/personal_info.h"

int main()
{
   struct personal_info *info;
   char *data = TESTCASES_DIR"/csv.csv";

   info = personal_read(data);
   printf("cardno:%d\nexpire:%s\nbalance:%.2lf\nstate:%d\nfaculty:%s\n",
          info->cardno, info->expire, info->balance, info->state, info->faculty);
   info->balance += 50;
   printf("cardno:%d\nexpire:%s\nbalance:%.2lf\nstate:%d\nfaculty:%s\n",
          info->cardno, info->expire, info->balance, info->state, info->faculty);
   personal_save(data, info);
   personal_destory(info);

   info = personal_read(data);
   printf("cardno:%d\nexpire:%s\nbalance:%.2lf\nstate:%d\nfaculty:%s\n",
          info->cardno, info->expire, info->balance, info->state, info->faculty);
   personal_destory(info);

   return 0;
}
