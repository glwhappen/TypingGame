/*************************************
    游戏名称:   打字游戏
    单线程，玩的过程中会卡顿
    但是已经是一个完整的游戏了，适合初学者入门
*************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>

#define LETTER_COUNT 30 //总共多少个单词下落
#define SPEED 500       //单词下落的速度
#define BULLET_SPEED 30 //子弹射击的速度

/***************初始化***************/
//设计字母、子弹
typedef struct tag_letter
{
    //位置
    int x, y;
    //字母
    char ch;
    //是否显示 0-不显示，1-显示
    int isDisplay;
    //生命 0-死亡，1-生存
    int life;
}Letter;

Letter letters[LETTER_COUNT];
Letter bullet;
//游戏字母初始化
void initLetters()
{
    srand((unsigned)time(NULL));
    for(int i = 0; i < LETTER_COUNT; i++)
    {
        letters[i].ch = rand() % 26 + 'A';
        letters[i].x = rand() % 80;
        letters[i].y = - i * 2;
        letters[i].life = 1;
    }
}
//子弹初始化
void initBullet(int x)
{
    bullet.ch = 12;
    bullet.x = x;
    bullet.y = 25;
    bullet.life = 1;
}

/***************功能函数***************/
//定位到屏幕的某个位置
void gotoxy(int x, int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(handle, coord);
}

//绘制字母
void drawLetter(char ch, int x, int y)
{
    gotoxy(x, y);
    printf("%c", ch);
}

//用来判断所有字母是否应该显示
void judgeLetterDisplay()
{
    for(int i = 0; i < LETTER_COUNT; i++)
    {
        if(letters[i].life == 0)
        {
            letters[i].isDisplay = 0;
        }
        else
        {
            if(letters[i].y < 0 || letters[i].y > 24)
            {
                letters[i].isDisplay = 0;
            }
            else
            {
                letters[i].isDisplay = 1;
            }
        }
    }
}

//letterMoving:让所有字母下降
void letterMoving()
{
    judgeLetterDisplay();
    for(int i = 0; i < LETTER_COUNT; i++)
    {
        if(letters[i].y < 25)
        {
            if(letters[i].isDisplay == 1)
            {
                drawLetter(' ', letters[i].x, letters[i].y);
                drawLetter(letters[i].ch, letters[i].x, letters[i].y + 1);
            }
            else
            {
                drawLetter(' ', letters[i].x, letters[i].y);
            }
            letters[i].y++;
        }
        else
        {
            drawLetter(' ', letters[i].x, letters[i].y);
        }
    }
}
//子弹飞行
void bulletMoving()
{
    if(bullet.life == 1)
    {
        drawLetter(' ', bullet.x, bullet.y);
        drawLetter(bullet.ch, bullet.x, bullet.y - 1);
    }
    bullet.y--;
}
//计算答案
int calcResult()
{
    int counter = 0;
    for(int i = 0; i < LETTER_COUNT; i++)
    {
        if(letters[i].life == 0)
        {
            counter++;
        }
    }
    return counter;
}
//隐藏控制台光标
void hideCursor()
{
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO CursorInfo;
        GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
        CursorInfo.bVisible = 0; //隐藏控制台光标
        SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
}

//提示信息
void menu()
{
    printf("一个迷你的打字游戏\n\n把输入法调成英文状态就可以开始玩了\n\n");
    printf("%d个字母即将来袭！！！\n\n", LETTER_COUNT);
    system("pause");
    system("cls");
}


int main()
{
    hideCursor();
    menu();
    initLetters();
    while(letters[LETTER_COUNT - 1].y < 25)
    {
        bullet.life = 1;
        if(_kbhit())//获取键盘按键
        {
            char ch = _getch();
            ch = toupper(ch);
            for(int k = 0; k < LETTER_COUNT; k++)//遍历每个子弹，看看有没有对应按键的字母
            {
                if(letters[k].isDisplay == 1 && letters[k].ch == ch)
                {
                    initBullet(letters[k].x);
                    while(bullet.y > 0)
                    {
                        bulletMoving();
                        if(bullet.y == letters[k].y)
                        {
                            letters[k].life = 0;
                            bullet.life = 0;
                            drawLetter(' ', bullet.x, bullet.y);
                        }
                        Sleep(BULLET_SPEED);
                    }
                    break;
                }
            }
        }
        Sleep(SPEED);
        letterMoving();
    }
    /*******************游戏结束******************/
    gotoxy(30, 12);
    printf("总共有%d个字母，你打掉了%d个",LETTER_COUNT, calcResult());
    getchar();
    return 0;
}
