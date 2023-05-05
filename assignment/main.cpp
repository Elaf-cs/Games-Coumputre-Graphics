
#include<windows.h>
#include<gl\glut.h>
#include <iostream>
#include <ctime>
#include <conio.h>
#include <vector>
#include <string>

#pragma warning(disable:4996)

using namespace std;

int c = 0;
float red = 1.0f, blue = 1.0f, green = 1.0f;

GLuint texture;

int scene = 0;

int Box_x = 0;
int Box_y = 0;

int currentbox = 0;

int temp = 0;

int game = -1;

bool moving = false;

bool selected[64] = { false, false, false, false, false,false,false,false
                     ,false, false, false, false, false,false,false,false,
    false, false, false, false, false,false,false,false
                     ,false, false, false, false, false,false,false,false,
    false, false, false, false, false,false,false,false
                     ,false, false, false, false, false,false,false,false,
    false, false, false, false, false,false,false,false
                     ,false, false, false, false, false,false,false,false
};

int coordinates[64][2] = { 
 {7.5, 0} ,{22.5, 0} ,{37.5, 0} ,{52.5, 0} ,{67.5, 0} ,{82.5, 0} , {97.5, 0}  ,{112.5, 0}, {7.5, 15} ,{22.5, 15} ,{37.5, 15} ,{52.5, 15} ,{67.5, 15} ,{82.5, 15} , {97.5, 15}  ,{112.5, 15}
,{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0}
,{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0} 
, {7.5, 90} ,{22.5, 90} ,{37.5, 90} ,{52.5, 90} ,{67.5, 90} ,{82.5, 90} , {97.5, 90}  ,{112.5, 90} , {7.5, 105} ,{22.5, 105} ,{37.5, 105} ,{52.5, 105} ,{67.5, 105} ,{82.5, 105} , {97.5, 105}  ,{112.5, 105} };

std::vector<string> pawns_Ids = { "8","9","10","11","12","13","14","15","B8","B9","B10","B11","B12","B13","B14","B15" };
std::vector<string> Bishop_Ids = { "120","150","B120","B150" };
std::vector<string> rook_Ids = { "100" ,"170", "B100" ,"B170" };
std::vector<string> Knight_Ids = { "110" , "160","B110" , "B160" };
std::vector<string> Queen_Ids = { "130" ,"B130" };
std::vector<string> king_Ids = { "140" , "B140"};

std::vector<string> board = {
"100","110","120","130","140","150","160","170","8","9","10","11","12","13","14","15",
"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0",
"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0",
"B8","B9","B10","B11","B12","B13","B14","B15","B100","B110","B120","B130","B140","B150","B160","B170"
};


GLuint LoadTexture(const char* filename)
{
    GLuint texture;
    int width, height;
    unsigned char* data;

    FILE* file;
    file = fopen(filename, "rb");

    if (file == NULL) return 0;
    width = 1280;
    height = 720;
    data = (unsigned char*)malloc(width * height * 3);
    //int size = fseek(file,);
    fread(data, width * height * 3, 1, file);
    fclose(file);

    for (int i = 0; i < width * height; ++i)
    {
        int index = i * 3;
        unsigned char B, R;
        B = data[index];
        R = data[index + 2];

        data[index] = R;
        data[index + 2] = B;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);

    return texture;
}


void shift(int arr[], int size) {
    for (int i = size - 2; i >= 0; i--)
    {
        arr[i + 1] = arr[i];
    }
}
//عرفت المتغيرات المهمه في اللعبه 
//char input;
enum input { top = 1, down, rigth, Left };
struct map { int width, heigth, fruitY, fruitX; };
struct snack { int headx, heady, tail, tailx[50], taily[50]; };
struct player { int score; bool lose; };
map map1; snack snack1; player player1;
input n1;

void gg() {
    srand(time(NULL));
    map1.fruitX = rand() % (map1.width - 2) + 1;//هذه الميثود استخدمها عشان يطلعلي الاكل تبع الثعبان في مكان عشوائي 
    map1.fruitY = rand() % (map1.heigth - 2) + 1;
}
void start() {
    //معلومات اللوحة تبع اللعبه
    map1.width = 40;
    map1.heigth = 20;
    gg();
    //.......................................
    // معلومات الثعبان
    snack1.headx = map1.width / 2;
    snack1.heady = map1.heigth / 2;
    snack1.tail = 0;
    //.......................................
    // معلومات اللاعب
    player1.score = 0;
    player1.lose = false;
}
void drawThePlay() {
    system("cls");
    for (int i = 0; i < map1.heigth; i++)
    {
        for (int j = 0; j < map1.width; j++)
        {
            if (i == 0 || i == map1.heigth - 1) cout << "*";
            else if (j == 0 || j == map1.width - 1) cout << "*";
            else if (i == snack1.heady && j == snack1.headx) cout << "O";
            else if (i == map1.fruitY && j == map1.fruitX) cout << "$";
            else {
                bool printed = false;
                for (int z = 0; z < snack1.tail; z++)
                {
                    if (snack1.tailx[z] == j && snack1.taily[z] == i)
                    {
                        cout << "o";
                        printed = true;
                        break;
                    }
                }
                if (!printed) cout << " ";
            }
        }
        cout << endl;
    }
    cout << "Player Score: " << player1.score << endl;

}
void moveInfo() {
    if (_kbhit()) {
        char c = _getch();
        switch (c)
        {
        case 't': n1 = top; break;
        case 'd': n1 = down; break;
        case 'r': n1 = rigth; break;
        case 'l': n1 = Left; break;
        case 'e': exit(0);
        }
    }
}
void move() {
    shift(snack1.tailx, 50);
    shift(snack1.taily, 50);
    snack1.tailx[0] = snack1.headx;
    snack1.taily[0] = snack1.heady;
    switch (n1)
    {
    case top:snack1.heady--; break;
    case down:snack1.heady++; break;
    case rigth:snack1.headx++; break;
    case Left:snack1.headx--; break;
    }
    if (snack1.heady >= map1.heigth || snack1.heady <= 0 || snack1.headx >= map1.width || snack1.headx <= 0)
        player1.lose = true;
    if (snack1.headx == map1.fruitX && snack1.heady == map1.fruitY) {
        gg();
        player1.score += 1;
        snack1.tail++;
    }
}


void init()
{
    // For displaying the window color
    glClearColor(0, 1, 1, 0);
    // Choosing the type of projection
    glMatrixMode(GL_PROJECTION);
    // for setting the transformation which here is 2D
    gluOrtho2D(0, 120, 0, 120);

    glEnable(GL_TEXTURE_2D);

    texture = LoadTexture("back.bmp");
}

void pawn(float x1 , float y1 , float angle , string color , float scale_x , float scale_y , float scale_z) {
    glPushMatrix();

    glTranslatef(x1, y1, 0);
    glScalef(scale_x, scale_y, scale_z);
    glRotatef(angle, 0, 0, 1);

    if (color.find('B') != std::string::npos) {
        glColor3f(0, 0, 0);
    }
    else
    {
        glColor3f(1, 1, 1);
      
    }
 

    int x = -4.5;
    int y = 0;
    glBegin(GL_POLYGON);
    glVertex2d(x, 7 + y);
    glVertex2d(x, 7 + y);
    glVertex2d(x, 5 + y);
    glVertex2d(x + 10, 5 + y);
    glVertex2d(x + 10, 7 + y);
    glVertex2d(x + 9, 8 + y);
    glVertex2d(x + 9, 8 + y);
    glVertex2d(x + 1, 8 + y);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 7, 10 + y);
    glVertex2d(x + 5, 22 + y);
    glVertex2d(x + 3, 10 + y);
    glVertex2d(x + 1, 8 + y);
    glVertex2d(x + 9, 8 + y);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 8, 18 + y);
    glVertex2d(x + 8, 22 + y);
    glVertex2d(x + 2, 22 + y);
    glVertex2d(x + 2, 18 + y);
    glEnd();


    glPopMatrix();
}

void Bishop(float x1, float y1, float angle, string color, float scale_x, float scale_y, float scale_z) {
    glPushMatrix();

    glTranslatef(x1, y1, 0);
    glScalef(scale_x, scale_y, scale_z);

    glRotatef(angle, 0, 0, 1);

    if (color.find('B') != std::string::npos) {
        glColor3f(0, 0, 0);
    }
    else
    {
        glColor3f(1, 1, 1);

    }

    int x = -5;
    glBegin(GL_POLYGON);
    glVertex2d(x, 7);
    glVertex2d(x, 7);
    glVertex2d(x, 5);
    glVertex2d(x + 10, 5);
    glVertex2d(x + 10, 7);
    glVertex2d(x + 9, 8);
    glVertex2d(x + 9, 8);
    glVertex2d(x + 1, 8);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 7, 10);
    glVertex2d(x + 6, 22);
    glVertex2d(x + 4, 22);
    glVertex2d(x + 3, 10);
    glVertex2d(x + 1, 8);
    glVertex2d(x + 9, 8);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 7, 20);
    glVertex2d(x + 7, 21);
    glVertex2d(x + 5, 25);
    glVertex2d(x + 3, 21);
    glVertex2d(x + 3, 20);
    glEnd();


    glPopMatrix();
}

void rook(float x1, float y1, float angle, string color, float scale_x, float scale_y, float scale_z) {
    glPushMatrix();

    glTranslatef(x1, y1, 0);
    glScalef(scale_x, scale_y, scale_z);
    glRotatef(angle, 0, 0, 1);

    if (color.find('B') != std::string::npos) {
        glColor3f(0, 0, 0);
    }
    else
    {
        glColor3f(1, 1, 1);

    }


    int x = -4.5;
    glBegin(GL_POLYGON);
    glVertex2d(x, 7);
    glVertex2d(x, 7);
    glVertex2d(x, 5);
    glVertex2d(x + 10, 5);
    glVertex2d(x + 10, 7);
    glVertex2d(x + 9, 8);
    glVertex2d(x + 9, 8);
    glVertex2d(x + 1, 8);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 7, 10);
    glVertex2d(x + 6, 22);
    glVertex2d(x + 4, 22);
    glVertex2d(x + 3, 10);
    glVertex2d(x + 1, 8);
    glVertex2d(x + 9, 8);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 9, 20);
    glVertex2d(x + 9, 22);
    glVertex2d(x + 1, 22);
    glVertex2d(x + 1, 20);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 9, 20);
    glVertex2d(x + 9, 23);
    glVertex2d(x + 7, 23);
    glVertex2d(x + 7, 20);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 6, 20);
    glVertex2d(x + 6, 23);
    glVertex2d(x + 4, 23);
    glVertex2d(x + 4, 20);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 3, 20);
    glVertex2d(x + 3, 23);
    glVertex2d(x + 1, 23);
    glVertex2d(x + 1, 20);
    glEnd();


    glPopMatrix();
}

void Knight(float x1, float y1, float angle, string color, float scale_x, float scale_y, float scale_z) {
    glPushMatrix();

    glTranslatef(x1, y1, 0);
    glScalef(scale_x, scale_y, scale_z);
    glRotatef(angle, 0, 0, 1);

    if (color.find('B') != std::string::npos) {
        glColor3f(0, 0, 0);
    }
    else
    {
        glColor3f(1, 1, 1);

    }

    int x = -4.5;
    glBegin(GL_POLYGON);
    glVertex2d(x + 1, 7);
    glVertex2d(x, 7);
    glVertex2d(x, 5);
    glVertex2d(x + 10, 5);
    glVertex2d(x + 10, 7);
    glVertex2d(x + 9, 8);
    glVertex2d(x + 1, 8);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 9, 8);
    glVertex2d(x + 9, 21);
    glVertex2d(x + 3, 13);
    glVertex2d(x + 3, 10);
    glVertex2d(x + 1, 8);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 9, 21);
    glVertex2d(x + 8, 23);
    glVertex2d(x + 7, 21);
    glVertex2d(x + 4, 20);
    glVertex2d(x + 1, 18);
    glVertex2d(x + 1, 17);
    glVertex2d(x + 3, 17);
    glVertex2d(x + 6, 16);
    glEnd();


    glPopMatrix();
}

void Queen(float x1, float y1, float angle, string color, float scale_x, float scale_y, float scale_z) {
    glPushMatrix();

    glTranslatef(x1, y1, 0);
    glScalef(scale_x, scale_y, scale_z);
    glRotatef(angle, 0, 0, 1);

    if (color.find('B') != std::string::npos) {
        glColor3f(0, 0, 0);
    }
    else
    {
        glColor3f(1, 1, 1);

    }

    int x = -4.5;
    glBegin(GL_POLYGON);
    glVertex2d(x + 1, 7);
    glVertex2d(x, 7);
    glVertex2d(x, 5);
    glVertex2d(x + 10, 5);
    glVertex2d(x + 10, 7);
    glVertex2d(x + 9, 7);
    glVertex2d(x + 9, 8);
    glVertex2d(x + 1, 8);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 7, 10);
    glVertex2d(x + 6, 22);
    glVertex2d(x + 4, 22);
    glVertex2d(x + 3, 10);
    glVertex2d(x + 1, 8);
    glVertex2d(x + 9, 8);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 3, 23);
    glVertex2d(x + 3, 22);
    glVertex2d(x + 7, 22);
    glVertex2d(x + 7, 23);
    glVertex2d(x + 8, 25);
    glVertex2d(x + 6, 25);
    glVertex2d(x + 5, 28);
    glVertex2d(x + 4, 25);
    glVertex2d(x + 2, 25);
    glEnd();


    glPopMatrix();
}

void king(float x1, float y1, float angle, string color, float scale_x, float scale_y, float scale_z) {
    glPushMatrix();

    glTranslatef(x1, y1, 0);
    glScalef(scale_x, scale_y, scale_z);
    glRotatef(angle, 0, 0, 1);

    if (color.find('B') != std::string::npos) {
        glColor3f(0, 0, 0);
    }
    else
    {
        glColor3f(1, 1, 1);

    }

    int x = -4.5;
    glBegin(GL_POLYGON);
    glVertex2d(x + 1, 7);
    glVertex2d(x, 7);
    glVertex2d(x, 5);
    glVertex2d(x + 10, 5);
    glVertex2d(x + 10, 7);
    glVertex2d(x + 9, 7);
    glVertex2d(x + 9, 8);
    glVertex2d(x + 1, 8);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 7, 10);
    glVertex2d(x + 6, 22);
    glVertex2d(x + 4, 22);
    glVertex2d(x + 3, 10);
    glVertex2d(x + 1, 8);
    glVertex2d(x + 9, 8);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 3, 23);
    glVertex2d(x + 3, 22);
    glVertex2d(x + 7, 22);
    glVertex2d(x + 7, 23);
    glVertex2d(x + 8, 25);
    glVertex2d(x + 6, 25);
    glVertex2d(x + 5, 28);
    glVertex2d(x + 4, 25);
    glVertex2d(x + 2, 25);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(x + 4, 28);
    glVertex2d(x + 3, 28);
    glVertex2d(x + 3, 27);
    glVertex2d(x + 7, 27);
    glVertex2d(x + 7, 28);
    glVertex2d(x + 6, 28);
    glVertex2d(x + 6, 29);
    glVertex2d(x + 4, 29);
    glEnd();


    glPopMatrix();
}


void chessboard()
{
 


    for (GLint x1 = 0; x1 < 120; x1 += 15)
    {
        for (GLint y1 = 0; y1 < 120; y1 += 15)
        {
            if (((x1 + y1) / 15) & 1) {
                glColor3f(.6, .6, .6);
            }
            else {
                glColor3f(.2, .2, .2);
            }
            glRecti(x1, y1, (x1 + 15), (y1 + 15));
        }
    }


    glFlush();

    if (moving) {
        glColor3f(0,1, 0);
    }
    else
    {
        glColor3f(0, 0, 1);
    }
    
    glRecti(Box_x, Box_y, (Box_x + 15), (Box_y + 15));


    if (std::find(board.begin(), board.end(), king_Ids[0]) != board.end() && std::find(board.begin(), board.end(), king_Ids[1]) != board.end()) {

        for (int i = 0; i <= 63; i++) {
            if (selected[i] == false) {
                if (std::find(pawns_Ids.begin(), pawns_Ids.end(), board[i]) != pawns_Ids.end()) {
                    pawn(coordinates[i][0], coordinates[i][1], 0, board[i], 0.5, 0.5, 0.5);
                }
                else if (std::find(Bishop_Ids.begin(), Bishop_Ids.end(), board[i]) != Bishop_Ids.end()) {
                    Bishop(coordinates[i][0], coordinates[i][1], 0, board[i], 0.5, 0.5, 0.5);
                }
                else if (std::find(rook_Ids.begin(), rook_Ids.end(), board[i]) != rook_Ids.end()) {
                    rook(coordinates[i][0], coordinates[i][1], 0, board[i], 0.5, 0.5, 0.5);
                }
                else if (std::find(Knight_Ids.begin(), Knight_Ids.end(), board[i]) != Knight_Ids.end()) {
                    Knight(coordinates[i][0], coordinates[i][1], 0, board[i], 0.5, 0.5, 0.5);
                }
                else if (std::find(Queen_Ids.begin(), Queen_Ids.end(), board[i]) != Queen_Ids.end()) {
                    Queen(coordinates[i][0], coordinates[i][1], 0, board[i], 0.5, 0.5, 0.5);
                }
                else if (std::find(king_Ids.begin(), king_Ids.end(), board[i]) != king_Ids.end()) {
                    king(coordinates[i][0], coordinates[i][1], 0, board[i], 0.5, 0.5, 0.5);
                }
            }
            else {
                if (std::find(pawns_Ids.begin(), pawns_Ids.end(), board[i]) != pawns_Ids.end()) {
                    pawn((Box_x + (Box_x + 15)) / 2, Box_y, 0, board[i], 0.5, 0.5, 0.5);
                }
                else if (std::find(Bishop_Ids.begin(), Bishop_Ids.end(), board[i]) != Bishop_Ids.end()) {
                    Bishop((Box_x + (Box_x + 15)) / 2, Box_y, 0, board[i], 0.5, 0.5, 0.5);
                }
                else if (std::find(rook_Ids.begin(), rook_Ids.end(), board[i]) != rook_Ids.end()) {
                    rook((Box_x + (Box_x + 15)) / 2, Box_y, 0, board[i], 0.5, 0.5, 0.5);
                }
                else if (std::find(Knight_Ids.begin(), Knight_Ids.end(), board[i]) != Knight_Ids.end()) {
                    Knight((Box_x + (Box_x + 15)) / 2, Box_y, 0, board[i], 0.5, 0.5, 0.5);
                }
                else if (std::find(Queen_Ids.begin(), Queen_Ids.end(), board[i]) != Queen_Ids.end()) {
                    Queen((Box_x + (Box_x + 15)) / 2, Box_y, 0, board[i], 0.5, 0.5, 0.5);
                }
                else if (std::find(king_Ids.begin(), king_Ids.end(), board[i]) != king_Ids.end()) {
                    king((Box_x + (Box_x + 15)) / 2, Box_y, 0, board[i], 0.5, 0.5, 0.5);
                }
            }
        }
    }
    else
    {
        std::cout << "\n\n the king is dead \n\n";
        exit(0);
    }



   
   
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    if (scene == 0) {
        glBindTexture(GL_TEXTURE_2D, texture);
        glPushMatrix();
        glTranslatef(-5, -10, 0);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1);
        glVertex2f(0, 130);
        glTexCoord2f(1, 1);
        glVertex2f(130, 130);
        glTexCoord2f(1, 0);
        glVertex2f(130, 0);
        glTexCoord2f(0, 0);
        glVertex2f(0, 0);
        glEnd();
        glPopMatrix();
    }
    else if (scene == 1)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_TEXTURE_2D);
        chessboard();
    }
    else if (scene == 2) {
       

        start();
        while (!player1.lose) {
            drawThePlay();
            moveInfo();
            move();
            Sleep(90);
        }
        system("pause");
    }

    glutSwapBuffers();
}



void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 13: // ASCII value of "Enter" key
        if (moving == false) {
            temp = currentbox;
            selected[currentbox] = true;
            moving = true;
        }
        else
        {
     
            selected[temp] = false;

            board[currentbox] = board[temp];

            std::cout << "currentbox : " << currentbox << "\n";
            std::cout << "temp : " << temp << "\n";

            coordinates[currentbox][0] = (Box_x + (Box_x + 15)) / 2;
            coordinates[currentbox][1] = Box_y;

            std::cout << "coordinates[currentbox][0] : " << coordinates[currentbox][0] << "\n";
            std::cout << "coordinates[currentbox][1]  : " << coordinates[currentbox][1] << "\n";
            moving = false;

            board[temp] = "0";
        }
        
        break;

    case 'C' : 
        scene = 1;
        break;
    case 'S':
        scene = 2;
        break;


    default:
        break;
    }
    display();
}



void arrowKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        Box_y = Box_y + 15;
        currentbox = currentbox + 8;
        break;
    case GLUT_KEY_DOWN:
        Box_y = Box_y - 15;
        currentbox = currentbox - 8;
        break;
    case GLUT_KEY_LEFT:
        Box_x = Box_x - 15;
        currentbox = currentbox - 1;
        break;
    case GLUT_KEY_RIGHT:
        Box_x = Box_x + 15;
        currentbox = currentbox + 1;
        break;

    }
    display();
}



int main(int agrc, char** argv)
{

    glutInit(&agrc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   
    glutInitWindowPosition(100, 100);

    glutInitWindowSize(700, 650);

    glutCreateWindow("Chess Game and snake game");

    glutSpecialFunc(arrowKeys);
    glutKeyboardFunc(keyboard);

    init();

    glutDisplayFunc(display);


    glutMainLoop();

    return 1;

}