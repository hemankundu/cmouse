#define _WIN32_WINNT 0x500
#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include <winable.h>
#include <time.h>
#define test
typedef struct ms
{
     int x,y,lc,rc,dc;
     //char bt;
}mStat;
typedef struct r
{
     char name[50];
     mStat val[5000];
     int count;
}record;
void printIns()
{
     printf("\nThis program will save a mose movement, and perform those action.\n"
               "Right click, Left click, and double Left click is supported.\n"
               "PRESS LEFT-SHIFT AFTER YOU FINISH READING A MOSE MOVE.\n\n");
}
void Clear()
{
     system("cls");
     printIns();
     getchar();
}
void getMousePosition (int *xPos, int *yPos)
{
     POINT cursorPos;
     GetCursorPos(&cursorPos);
     *xPos = cursorPos.x;
     *yPos = cursorPos.y;
}

void mouseRightClick ()
{
     INPUT Input = {0};
     Input.type = INPUT_MOUSE;
     Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
     SendInput(1,&Input,sizeof(INPUT));
     ZeroMemory(&Input,sizeof(INPUT));
     Input.type = INPUT_MOUSE;
     Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
     SendInput(1,&Input,sizeof(INPUT));
}

void mouseLeftClick ()
{
     INPUT Input = {0};
     Input.type = INPUT_MOUSE;
     Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
     SendInput(1,&Input,sizeof(INPUT));
     ZeroMemory(&Input,sizeof(INPUT));
     Input.type = INPUT_MOUSE;
     Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
     SendInput(1,&Input,sizeof(INPUT));
}

void mouseLeftDown ()
{
     INPUT Input = {0};
     ZeroMemory(&Input,sizeof(INPUT));
     Input.type = INPUT_MOUSE;
     Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
     SendInput(1,&Input,sizeof(INPUT));
}

void mouseLeftUp ()
{
     INPUT Input = {0};
     ZeroMemory(&Input,sizeof(INPUT));
     Input.type = INPUT_MOUSE;
     Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
     SendInput(1,&Input,sizeof(INPUT));
}

void mouseRightDown ()
{
     INPUT Input = {0};
     ZeroMemory(&Input,sizeof(INPUT));
     Input.type = INPUT_MOUSE;
     Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
     SendInput(1,&Input,sizeof(INPUT));
}

void mouseRightUp ()
{
     INPUT Input = {0};
     ZeroMemory(&Input,sizeof(INPUT));
     Input.type = INPUT_MOUSE;
     Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
     SendInput(1,&Input,sizeof(INPUT));
} 
void mouseDoubleClick()
{
     mouseLeftClick();
     mouseLeftClick();
}
void mouseMoveTo (int toX, int toY)
{
     double screenRes_width = GetSystemMetrics( SM_CXSCREEN )-1;
     double screenRes_height = GetSystemMetrics( SM_CYSCREEN )-1;
     double dx = toX*(65535.0f / screenRes_width);
     double dy = toY*(65535.0f / screenRes_height);
     INPUT Input = {0};
     Input.type = INPUT_MOUSE;
     Input.mi.dwFlags = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
     Input.mi.dx = (long) (dx);
     Input.mi.dy = (long) (dy);
     SendInput(1,&Input,sizeof(INPUT));
}
int keyPressed(int key){
    return (GetAsyncKeyState(key) & 0x8000 != 0);
}
void readMouse(record *rec)
{
     int lc,rc,shift=1,i=0,j;
     printf("\nReading..\n");
     for(;shift=!(keyPressed(VK_LSHIFT));)
     {
          lc=(keyPressed(VK_LBUTTON))?1:0;
          rc=(keyPressed(VK_RBUTTON))?1:0;
          if(lc||rc)
          {
               getMousePosition(&rec->val[i].x,&rec->val[i].y);
               if(lc)
               {  
                    clock_t before = clock(),msec;
                    int c=0;
                    do
                    {
                         clock_t difference = clock() - before;
                         msec = difference * 1000 / CLOCKS_PER_SEC;
                         //printf("%d\n",c);
                         lc=(keyPressed(VK_LBUTTON))?1:0;
                         rc=(keyPressed(VK_RBUTTON))?1:0;
                         c++;
                         if(lc||rc) break;
                    }while(msec<200);
                    if(!lc)
                    {
                         rec->val[i].lc=1;
                         rec->val[i].dc=0;
                         printf("Left click\n");
                    }
                    else
                    {
                         rec->val[i].dc=1;
                         rec->val[i].lc=0;
                         printf("Double Left click\n");
                    }    
               }
               else
                    rec->val[i].lc=0;
               if(rc)
               {
                    rec->val[i].rc=1;
                    printf("Right click\n");
               }
               else
                    rec->val[i].rc=0;    
               i++;
          }
     }
     rec->count=i;
     printf("\nDone reading.\n");
}
void writeMouse(record *rec)
{
     int i;
     printf("\nPerforming %s..\n",rec->name);
     for(i=0;i<rec->count;i++)
     {
          mouseMoveTo(rec->val[i].x,rec->val[i].y);
          if(rec->val[i].lc)
               mouseLeftClick();
          if(rec->val[i].rc)
               mouseRightClick();
          if(rec->val[i].dc)
               mouseDoubleClick();
          sleep(1);
     }
     printf("\nDone performing.\n");
     sleep(1);
}

int main()
{
     record rec[10];
     int recCount=0,ch=1,i;
     printIns();
     while(ch)
     {
          printf("[1] Add record\t[2] Perform\n[0]Exit\n: ");
          scanf("%d",&ch);
          switch(ch)
          {
               case 1:
                    readMouse(&rec[recCount]);
                    printf("\nEnter record name: ");
                    scanf("%s",rec[recCount].name);
                    printf("\nSaved.\n");
                    recCount++;
                    break;
               case 2:
                    for(i=0;i<recCount;i++)
                    {
                         printf("\n[%d] %s ",i+1,rec[i].name);
                    }
                    printf("\n%d record available.\nChoose one: ",recCount);
                    scanf("%d",&i);
                    if(i>=1&&i<=recCount)writeMouse(&rec[i-1]);
                    break;
          }
          Clear();
     }
     return 0;
}

