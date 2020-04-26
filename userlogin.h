#ifndef USERLOGIN_H
#define USERLOGIN_H


class Userlogin
{
public:
    Userlogin();
};
class CDBase
{
public:
CDBase();

~CDBase();

bool CheckUserAuth(char *name, char *pwd); //判断用户名、密码是否匹配

bool query(char *name); //名字是否匹配

void WriteUserInfo(char *name, char *pwd);

private:
struct Node
{
Node():_pnext( NULL){};
Node(char *n,char *p):_pnext(NULL);
char _name[20];
char _pwd[20];
Node *_pnext;
void sexNext(Node *p);

Node* getNext();

Node *_phead;

class CGUI
{
public:
CGUI():_running(true){}
void Run();
void ShowMainMenu();
void DoLogin();           //登陆页面

void DoRegister();            //注册页面
void DoExit();                   //退出页面

private:
char _name[20];
char _pwd[20];
bool _running;


typedef void (CGUI::*PFUNC)();
typedef struct _TableItem;
int choice;
PFUNC pfunc;
}


#endif // USERLOGIN_H

int main(){
    CGUI gui;
    gui.Run();


    return 0;
};
