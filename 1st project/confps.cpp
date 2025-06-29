#include <iostream>
#include <windows.h>


using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;
float fFOV = 3.14159 / 4.0; 
float fDepth = 16.0;

int nMapHeight = 16;
int nMapWidth  = 16;

int main(){
    //create screen buffer 
    wchar_t *screen = new wchar_t [nScreenWidth*nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    wstring map;

    map += L"################";
    map += L"#              #";
    map += L"#              #";
    map += L"#              #";
    map += L"#              #";
    map += L"#              #";
    map += L"#              #";
    map += L"#              #";
    map += L"#              #";
    map += L"#              #";
    map += L"#              #";
    map += L"#              #";
    map += L"#              #";
    map += L"#              #";
    map += L"#              #";
    map += L"################";

    while(1)
    {
        for(int x = 0; x < nScreenWidth; x++)
        {
            //for each column, calculate the projected ray angle into world space
            float fRayAngle = (fPlayerA - fFOV / 1.0f) + ((float)x / (float)nScreenWidth) * fFOV;
            
            float fDistanceToWall = 0;
            bool bHitWall = false;

            float fEyeX = sinf(fRayAngle); //Unit vector for ray in player space
            float fEyeY = cosf(fRayAngle);

            while(!bHitWall && fDistanceToWall < fDepth)
            {

                fDistanceToWall += 0.1f; 

                int nTestX =(int)(fPlayerX +fEyeX * fDistanceToWall);
                int nTestY =(int)(fPlayerY +fEyeY * fDistanceToWall);

                //Test if ray is out of bounds  
                if (nTestX < 0 || nTestX >= nMapwidth || nTestY < 0 || nTestY >= nMapHeight)
                {
                    bHitWall = true;  //Just set distance to to maximum depth 
                    fDistanceToWall = fDepth;
                }
                else 
                {
                    //Ray is inbounds so test to see if the ray cell is a wall block
                    if(map[nTestY * nMapWidth + nTestX] == '#')
                    {
                        bHitWall = true;
                    }
                }
            }
            
            //calculate distance to ceilling and floor 
            int nCeiling =(float)(nScreenHeight /2.0) - nScreenHeight /((float)fDistanceToWall);
            int nFloor = nScreenHeight - nCeiling; 

            for(int y = 0; y < nScreenHeight; y++)
            {
                if (y < nCeiling)
                    screen[y*nScreenWidth+x] = ' ';
                else if (y > nCeiling && y <= nFloor)
                    screen[y*nScreenWidth+x] = '#';
                else
                    screen[y*nScreenWidth+x] = ' ';
            }




        }
    
        

    screen[nScreenWidth * nScreenHeight - 1] = '\0';
    WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
     

    }
     return 0;

}