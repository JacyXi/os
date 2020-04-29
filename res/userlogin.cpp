#include "userlogin.h"

#include<assert.h>
#include <iostream>
#include "stdio.h"
#include "string.h"
using namespace std;

//定义变量
char reg_name[30]="",reg_pwd[10]="";
char on_name[30],on_pwd[10];


int strcmp (const char *s1,const char *s2) //比较字符串是否相同
{
    assert((s1!=NULL)&&(s2!=NULL));

    while(*s1!='\0'&&*s2!='\0')//字符串前面部分都相同
    {
        if(*s1-*s2>0){
            return 1;
        }
        if(*s1-*s2<0){
            return -1;
            }
        s1++;
        s2++;
    }
    //谁先为'\0',谁就小
    if(*s1=='\0'&&*s2!='\0'){

        return -1;
        }
    if (*s2=='\0'&&*s1!='\0'){
        return 1;
    }
    return 0;              //同时为'\0'
}





//用户注册系统
void regist()
{
    //清屏
    system("pause");
    system("cls");

    cout<<"\n\n\t\t\t User Login system \n\n";
    while(true)
    {
        //输入用户名
        printf("\t\t Please enter the user name [no more than 10 characters]：");
        scanf("%s",reg_name);

        //判断用户名
        if(strlen(reg_name)<=10)
        {
            while(true)
            {
                //enter the password
                printf("\n\t\t please enter the password[no more than 10 digits]：");
                scanf("%s",reg_pwd);

                //判断密码
                if(strlen(reg_pwd)==10)
                {
                    printf("\n\n\t\t Successfully registered. Your user name is% s and password is %s\n\n",reg_name,reg_pwd);
                    break;
                }
                else
                {
                    printf("\n\t\t The password is too long, please re-enter \n",strlen(reg_pwd));
                }
            }
            break;
        }
        else
        {
            printf("\n\t\t The user name is too long, please re-enter",strlen(reg_name));
        }
    }
}

//判断是否注册
int judge()
{
    if(strcmp(reg_name,"")==0&&strcmp(reg_pwd,"")==0)
    {
        printf("\n\n\t\t You haven't registered yet. Please register first！\n\n");
        return 0;
    }
    else
    {
        return 1;
    }
}

//用户登录
void dl()
{
    int i;
    system("pause");
    system("cls");

    printf("\n\n\t\t\t User Login system \n\n");
    //三次登录机会
    for(i=1;i<=3;i++)
    {
        printf("\t\t please enter the user name :");
        scanf("%s",on_name);
        printf("\n\t\t please enter the password :");
        scanf("%s",on_pwd);
        if(strcmp(reg_name,on_name)==0&&strcmp(reg_pwd,on_pwd)==0)
        {
            printf("\n\n\t\t Login succeeded \n\n");
            break;
        }
        else
        {
            printf("\n\n\t\t Login failed, please login again, you have% d more opportunities \n\n",3-i);
        }

    }

}
int main()//
{
    //定义变量
    int type;
    while(true)
    {
        system("pause");
        system("cls");
        //输出界面
        printf("\n\n\t\t\t User login system \n\n");

        printf("\t\t\t1: regist \n");
        printf("\t\t\t2: login \n");
        printf("\t\t\t0: exit \n\n");

        //choice the function
        printf("\t\t please choice the function type ：");
        scanf("%d",&type);

        //判断
        switch(type)
        {
            case 1:regist();break;
            case 2:
                if(judge()==1)
                {
                    dl();
                }
                break;
            case 0:exit(1);break;
            default:
                printf("\n\t\t The function type you entered is wrong, please re-enter！\n");
        }

    }
    return 0;

}
