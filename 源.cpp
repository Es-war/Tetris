#include <graphics.h>
#include <conio.h>
#include <cmath>
#include <ctime>
#include <windows.h>

const double pi = acos(-1);

DWORD time_now[2]; //��¼�ϴ�ִ�в�����ʱ��
bool flag_next[2]; //�Ƿ������¸�����
bool flag_over[2]; //������Ϸ�Ƿ����
int next_c_idx[2]; //�¸��������ɫ
int next_s_idx[2]; //�¸����������
int next_d_idx[2]; //�¸��������״
int now_c_idx[2];  //��ǰ�������ɫ
int now_s_idx[2];  //��ǰ���������
int now_d_idx[2];  //��ǰ�������״
int now_mp_x[2];   //��¼��ǰ�����x����
int now_mp_y[2];   //��¼��ǰ�����y����
int score[2];      //��¼��ҳɼ�

COLORREF tmp_line_color;
LINESTYLE tmp_line_style;
COLORREF tmp_text_color;
LOGFONT tmp_text_style;
COLORREF tmp_fill_color;

COLORREF colors[7] = { RGB(113, 187, 238), RGB(134, 221, 243), RGB(254, 178, 218), RGB(132, 116, 199), RGB(174, 143, 220), RGB(211, 149, 224), RGB(118, 140, 206) };
int mp[2][10][15];         //��¼�������䶨����ȫ�巽��λ����Ϣ������ͼ
COLORREF mp_c[2][10][15];  //��¼�������䶨����ȫ�巽�����ɫ��Ϣ
wchar_t score_tips[2][30]; //�����Ϣ
wchar_t over_tips[50];     //
int mp_tmp[10][15];        //��¼�������䶨����ȫ�巽��λ����Ϣ������ͼ
COLORREF mp_c_tmp[10][15];

struct Square
{
    int dir[4][4][2];
};

Square squares[7] = {
    {0, -2, 0, -1, 0, 0, 1, 0, 0, -1, 1, -1, 2, -1, 0, 0, 0, -2, 1, -2, 1, -1, 1, 0, 0, 0, 1, 0, 2, 0, 2, -1},      //  L ��
    {0, 0, 1, 0, 1, -1, 1, -2, 0, -1, 0, 0, 1, 0, 2, 0, 0, -2, 0, -1, 0, 0, 1, -2, 0, -1, 1, -1, 2, -1, 2, 0},      //  L �ͣ�����
    {0, -1, 0, 0, 1, -1, 1, 0, 0, -1, 0, 0, 1, -1, 1, 0, 0, -1, 0, 0, 1, -1, 1, 0, 0, -1, 0, 0, 1, -1, 1, 0},       // �� ��
    {0, 0, 1, -1, 1, 0, 2, 0, 1, -2, 1, -1, 1, 0, 2, -1, 0, -1, 1, -1, 1, 0, 2, -1, 0, -1, 1, -2, 1, -1, 1, 0},     // ɽ ��
    {0, -3, 0, -2, 0, -1, 0, 0, 0, -3, 1, -3, 2, -3, 3, -3, 0, -3, 0, -2, 0, -1, 0, 0, 0, -3, 1, -3, 2, -3, 3, -3}, //  | ��
    {0, -1, 1, -1, 1, 0, 2, 0, 0, -1, 0, 0, 1, -2, 1, -1, 0, -1, 1, -1, 1, 0, 2, 0, 0, -1, 0, 0, 1, -2, 1, -1},     //  Z ��
    {0, 0, 1, -1, 1, 0, 2, -1, 1, -2, 1, -1, 2, -1, 2, 0, 0, 0, 1, -1, 1, 0, 2, -1, 1, -2, 1, -1, 2, -1, 2, 0}      //  Z �ͣ�����
};                                                                                                                  //�������͡���״

class paint
{
public:
    void endGame();                          //���ƽ�������
    void initEnviroment();                   //��ʼ������
    void drawGameBG();                       //������Ϸ����
    void drawLeftSide();                     //������Ϸ����
    void drawRightSide();                    //������Ϸ�Ҳ��
    void drawMap();                          //���Ƶ�ͼ
    void drawSquareNext(int num);            //����Ԥ�����ڵķ���
    void drawItem(int x, int y, COLORREF c); //���Ʒ���
    void drawSquareNow(int num);             //���Ƶ�ǰ�������䶨����ȫ�巽��
};
void paint::endGame()
{
    for (int i = 0; i < 541; ++i)
    {
        setlinecolor(RGB(195, 214, 242));
        line(0, i, 1210, i);
    }
    settextcolor(RGB(65, 105, 225));
    settextstyle(90, 0, L"Arial Black");
    if (score[0] > score[1])
    {
        outtextxy(285, 50, L"The First Player Win");
    }
    else if (score[0] < score[1])
    {
        outtextxy(270, 50, L"The Second Player Win");
    }
    else
    {
        outtextxy(300, 50, L"The Score Was Tied");
    }
}
void paint::initEnviroment()
{
    // ��������
    initgraph(1210, 540);
    HWND hwnd = GetHWnd();
    SetWindowText(hwnd, L"����˹");
    SetWindowPos(hwnd, HWND_TOP, 0, 20, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

    // ��ͼģʽ����
    setbkmode(TRANSPARENT);

    // ���������
    srand(time(NULL));
}
void paint::drawGameBG()
{
    COLORREF tmp = getlinecolor();

    for (int i = 0; i < 541; ++i)
    {
        setlinecolor(RGB(209, 234, 245));
        line(241, 540 - i, 600, 540 - i);
        line(611, 540 - i, 970, 540 - i);
    }

    setlinecolor(tmp);
}
void paint::drawLeftSide()
{
    tmp_line_color = getlinecolor();
    getlinestyle(&tmp_line_style);
    tmp_text_color = getlinecolor();
    gettextstyle(&tmp_text_style);

    for (int i = 0; i < 541; ++i)
    {
        setlinecolor(RGB(195, 214, 242));
        line(0, i, 240, i);
    }
    settextcolor(RGB(65, 105, 225));
    settextstyle(30, 0, L"���Ŀ���");
    outtextxy(48, 300, L"���ƣ�A");
    outtextxy(48, 330, L"���ƣ�D");
    outtextxy(48, 360, L"���Σ�W");
    outtextxy(48, 390, L"���䣺S");

    //����Ԥ����
    setlinecolor(WHITE);
    rectangle(48, 36, 240 - 48, 36 + 144);
    drawSquareNext(0);
    setlinecolor(0x7FFFAA);
    rectangle(48, 36, 240 - 48, 36 + 144);

    swprintf(score_tips[0], 29, L"�÷֣�%d", score[0]);
    outtextxy(48, 200, score_tips[0]);

    setlinecolor(tmp_line_color);
    setlinestyle(&tmp_line_style);
    settextcolor(tmp_text_color);
    settextstyle(&tmp_text_style);
}
void paint::drawRightSide()
{
    tmp_line_color = getlinecolor();
    getlinestyle(&tmp_line_style);
    tmp_text_color = getlinecolor();
    gettextstyle(&tmp_text_style);

    for (int i = 0; i < 541; ++i)
    {
        setlinecolor(RGB(195, 214, 242));
        line(971, i, 1210, i);
    }
    settextcolor(RGB(65, 105, 225));
    settextstyle(30, 0, L"���Ŀ���");
    outtextxy(970 + 48, 300, L"���ƣ���");
    outtextxy(970 + 48, 330, L"���ƣ���");
    outtextxy(970 + 48, 360, L"���Σ���");
    outtextxy(970 + 48, 390, L"���䣺��");

    //����Ԥ����
    setlinecolor(WHITE);
    rectangle(970 + 48, 36, 1210 - 48, 36 + 144);
    drawSquareNext(1);
    setlinecolor(0x7FFFAA);
    rectangle(970 + 48, 36, 1210 - 48, 36 + 144);

    swprintf(score_tips[1], 29, L"�÷֣�%d", score[1]);
    outtextxy(970 + 48, 200, score_tips[1]);

    setlinecolor(tmp_line_color);
    setlinestyle(&tmp_line_style);
    settextcolor(tmp_text_color);
    settextstyle(&tmp_text_style);
}
void paint::drawMap()
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 15; j++)
        {
            if (mp[0][i][j])
                drawItem(240 + i * 36, j * 36, mp_c[0][i][j]);
            if (mp[1][i][j])
                drawItem(610 + i * 36, j * 36, mp_c[1][i][j]);
        }
}
void paint::drawItem(int x, int y, COLORREF c)
{
    // �������
    // ʵ�ʳߴ磺32 * 32����λ�����أ�
    // �߿���ɫ����ɫ
    // Բ�ǰ뾶��4����λ�����أ�
    // �ڲ���ࣺ2����λ�����أ�
    tmp_fill_color = getfillcolor();

    const int r = 6;
    const int p = 2;

    int up_l_x = x + p + r;
    int up_r_x = x + 36 - p - r;
    int up___y = y + p;

    int down_l_x = x + p + r;
    int down_r_x = x + 36 - p - r;
    int down___y = y + 36 - p;

    int left_u_y = y + p + r;
    int left_d_y = y + 36 - p - r;
    int left___x = x + p;

    int right_u_y = y + p + r;
    int right_d_y = y + 36 - p - r;
    int right___x = x + 36 - p;

    line(up_l_x, up___y, up_r_x, up___y);
    line(down_l_x, down___y, down_r_x, down___y);
    line(left___x, left_u_y, left___x, left_d_y);
    line(right___x, right_u_y, right___x, right_d_y);
    arc(x + p, y + p, x + p + 2 * r, y + p + 2 * r, pi / 2, pi);
    arc(x + 36 - p, y + p, x + 36 - p - 2 * r, y + p + 2 * r, 0, pi / 2);
    arc(x + p, y + 36 - p, x + p + 2 * r, y + 36 - p - 2 * r, -pi, -pi / 2);
    arc(x + 36 - p, y + 36 - p, x + 36 - p - 2 * r, y + 36 - p - 2 * r, -pi / 2, 0);

    setfillcolor(c);
    floodfill(x + p + r + 1, y + p + r + 1, WHITE);

    setfillcolor(tmp_fill_color);
}
void paint::drawSquareNext(int num)
{
    int tmp_x = 48, tmp_y = 36 + 108;
    if (num)
    {
        tmp_x += 970;
    }
    COLORREF c = colors[next_c_idx[num]];
    drawItem(tmp_x + squares[next_s_idx[num]].dir[next_d_idx[num]][0][0] * 36, tmp_y + squares[next_s_idx[num]].dir[next_d_idx[num]][0][1] * 36, c);
    drawItem(tmp_x + squares[next_s_idx[num]].dir[next_d_idx[num]][1][0] * 36, tmp_y + squares[next_s_idx[num]].dir[next_d_idx[num]][1][1] * 36, c);
    drawItem(tmp_x + squares[next_s_idx[num]].dir[next_d_idx[num]][2][0] * 36, tmp_y + squares[next_s_idx[num]].dir[next_d_idx[num]][2][1] * 36, c);
    drawItem(tmp_x + squares[next_s_idx[num]].dir[next_d_idx[num]][3][0] * 36, tmp_y + squares[next_s_idx[num]].dir[next_d_idx[num]][3][1] * 36, c);
}
void paint::drawSquareNow(int num)
{
    int x = 240;
    if (num)
        x = 610;
    drawItem((now_mp_x[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][0][0]) * 36 + x, (now_mp_y[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][0][1]) * 36, colors[now_c_idx[num]]);
    drawItem((now_mp_x[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][1][0]) * 36 + x, (now_mp_y[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][1][1]) * 36, colors[now_c_idx[num]]);
    drawItem((now_mp_x[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][2][0]) * 36 + x, (now_mp_y[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][2][1]) * 36, colors[now_c_idx[num]]);
    drawItem((now_mp_x[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][3][0]) * 36 + x, (now_mp_y[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][3][1]) * 36, colors[now_c_idx[num]]);
}

class player
{
private:
    int num;

public:
    player(int n = 0)
    {
        num = n;
    }
    void moveRotate();                              //������ת
    void moveLeft();                                //��������
    void moveRight();                               //��������
    bool checkPut(int mp_x, int mp_y, int dir_idx); //���ı��Ƿ����
    void recordSquareNow();                         //���µ�ͼ
    void initDatasPerSquare();                      //��ʼ���µķ���
    void initDatasPerRound();                       //��ʼ��ÿ����Ϸ
    int execClear();                                //����
    bool checkOver();                               //����Ƿ����������Ϸ
};
void player::moveRotate()
{
    // ����ʣ��������״��������ת��������ת״̬
    for (int i = 1; i <= 3; ++i)
        if (checkPut(now_mp_x[num], now_mp_y[num], (now_d_idx[num] + i) % 4))
        {
            now_d_idx[num] = (now_d_idx[num] + i) % 4;
            break;
        }
}
void player::moveLeft()
{
    // �����ܷ�����
    if (checkPut(now_mp_x[num] - 1, now_mp_y[num], now_d_idx[num]))
        --now_mp_x[num];
}
void player::moveRight()
{
    // �����ܷ�����
    if (checkPut(now_mp_x[num] + 1, now_mp_y[num], now_d_idx[num]))
        ++now_mp_x[num];
}
void player::recordSquareNow()
{
    int sq_x[4], sq_y[4];
    for (int i = 0; i < 4; i++)
    {
        sq_x[i] = now_mp_x[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][i][0];
        sq_y[i] = now_mp_y[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][i][1];
    }
    for (int i = 0; i < 4; i++)
    {
        if (sq_y[i] >= 0)
        {
            mp[num][sq_x[i]][sq_y[i]] = 1;
            mp_c[num][sq_x[i]][sq_y[i]] = colors[now_c_idx[num]];
        }
    }
}
void player::initDatasPerSquare()
{
    if (flag_over[num])
    {
        flag_next[num] = false;
        return;
    }
    now_mp_x[num] = 5;
    now_mp_y[num] = -1;
    flag_next[num] = false;
    now_c_idx[num] = next_c_idx[num];
    now_s_idx[num] = next_s_idx[num];
    now_d_idx[num] = next_d_idx[num];
    next_c_idx[num] = rand() % 7;
    next_s_idx[num] = rand() % 7;
    next_d_idx[num] = rand() % 4;
}
void player::initDatasPerRound()
{
    memset(mp, 0, sizeof(mp));
    memset(mp, 0, sizeof(mp_c));
    time_now[num] = GetTickCount();
    next_c_idx[num] = rand() % 7;
    next_s_idx[num] = rand() % 7;
    next_d_idx[num] = rand() % 4;
    flag_over[num] = false;
    flag_next[num] = false;
    score[num] = 0;
}
bool player::checkPut(int mp_x, int mp_y, int dir_idx)
{
    int sq_x[4];
    int sq_y[4];
    for (int i = 0; i < 4; ++i)
    {
        sq_x[i] = mp_x + squares[now_s_idx[num]].dir[dir_idx][i][0];
        sq_y[i] = mp_y + squares[now_s_idx[num]].dir[dir_idx][i][1];
    }

    // ������Խ�硢�·�Խ�硢�ظ�ռ��
    for (int i = 0; i < 4; ++i)
    {
        if (sq_x[i] < 0 || sq_x[i] > 9 || sq_y[i] > 14)
            return false;
        if (sq_y[i] < 0) // �������Ϸ���
            continue;
        if (mp[num][sq_x[i]][sq_y[i]])
            return false;
    }
    return true;
}
int player::execClear()
{
    //�����ʱ��ͼ�е�����
    //��ҵ������
    int clearNum = 0;
    memset(mp_tmp, 0, sizeof(mp_tmp));
    memset(mp_c_tmp, 0, sizeof(mp_c_tmp));

    int cnt_j = 14;
    for (int j = 14; j >= 0; j--)
    {
        int cnt_i = 0;
        for (int i = 0; i < 10; i++)
            if (mp[num][i][j])
                cnt_i++;
        if (cnt_i != 10)
        {
            for (int i = 0; i < 10; i++)
            {
                mp_tmp[i][cnt_j] = mp[num][i][j];
                mp_c_tmp[i][cnt_j] = mp_c[num][i][j];
            }
            cnt_j--;
        }
        else
        {
            clearNum++;
        }
    }
    score[num] += 100.0 * clearNum * (1 + clearNum * 0.25); //����Խ��÷��ۼ�Խ��
    for (int j = 0; j < 15; ++j)
        for (int i = 0; i < 10; ++i)
        {
            mp[num][i][j] = mp_tmp[i][j];
            mp_c[num][i][j] = mp_c_tmp[i][j];
        }
    return clearNum;
}
bool player::checkOver()
{
    for (int i = 0; i < 10; i++)
        if (mp[num][i][0])
            return true;
    return false;
}

class player_1 : public player
{
public:
    player_1(int n = 0) : player(n)
    {
    }
    void moveDown();    //�����½�
    void addRow(int n); //���Է�������
};
void player_1::moveDown()
{
    // �����ܷ�����
    if (checkPut(now_mp_x[0], now_mp_y[0] + 1, now_d_idx[0]))
    {
        ++now_mp_y[0];
        return;
    }
    // ����������˵����鷽�顰�������ˣ�ִ�����²���
    // 1����ʾ���Կ�ʼ��һ������
    // 2���������¼��map��ͼ��
    // 3���ж��Ƿ��������
    // 4���ж����к���Ϸ�Ƿ����
    flag_next[0] = true;
    recordSquareNow();
    int clearNum = execClear();
    addRow(clearNum);
    if (checkOver())
        flag_over[0] = true;
}
void player_1::addRow(int n)
{
    if (n == 0 || flag_over[1])
        return;
    now_mp_y[1] -= n;
    int cnt_j = 14 - n;
    for (int j = 14; j >= 0; j--)
    {

        for (int i = 0; i < 10; i++)
        {
            mp_tmp[i][cnt_j] = mp[1][i][j];
            mp_c_tmp[i][cnt_j] = mp_c[1][i][j];
        }
        cnt_j--;
    }
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < 10; i++)
        {
            if (rand() % 2)
            {
                mp_tmp[i][14 - j] = 1;
                mp_c_tmp[i][14 - j] = colors[rand() % 7];
            }
        }
    }
    for (int j = 0; j < 15; ++j)
        for (int i = 0; i < 10; ++i)
        {
            mp[1][i][j] = mp_tmp[i][j];
            mp_c[1][i][j] = mp_c_tmp[i][j];
        }
}

class player_2 : public player
{
public:
    player_2(int n = 1) : player(n)
    {
    }
    void moveDown();
    void addRow(int n);
};
void player_2::moveDown()
{
    // �����ܷ�����
    if (checkPut(now_mp_x[1], now_mp_y[1] + 1, now_d_idx[1]))
    {
        ++now_mp_y[1];
        return;
    }
    // ����������˵����鷽�顰�������ˣ�ִ�����²���
    // 1����ʾ���Կ�ʼ��һ������
    // 2���������¼��map��ͼ��
    // 3���ж��Ƿ��������
    // 4���ж����к���Ϸ�Ƿ����
    flag_next[1] = true;
    recordSquareNow();
    int clearNum = execClear();
    addRow(clearNum);
    if (checkOver())
        flag_over[1] = true;
}
void player_2::addRow(int n)
{
    if (n == 0 || flag_over[0])
        return;
    now_mp_y[0] -= n;
    int cnt_j = 14 - n;
    for (int j = 14; j >= 0; j--)
    {

        for (int i = 0; i < 10; i++)
        {
            mp_tmp[i][cnt_j] = mp[0][i][j];
            mp_c_tmp[i][cnt_j] = mp_c[0][i][j];
        }
        cnt_j--;
    }
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < 10; i++)
        {
            if (rand() % 2)
            {
                mp_tmp[i][14 - j] = 1;
                mp_c_tmp[i][14 - j] = colors[rand() % 7];
            }
        }
    }
    for (int j = 0; j < 15; ++j)
        for (int i = 0; i < 10; ++i)
        {
            mp[0][i][j] = mp_tmp[i][j];
            mp_c[0][i][j] = mp_c_tmp[i][j];
        }
}

void getCmd(player_1& one, player_2& two)
{
    bool over = false;
    while (true)
    {
        //ָ�ʱ
        DWORD time_tmp[2];
        time_tmp[0] = time_tmp[1] = GetTickCount();

        //������һ�Ƿ�ʱ��δִ����ȷָ��
        int speed1, speed2;
        if (score[0] <= 3000)
            speed1 = 1000 - score[0] / 10;
        else
            speed1 = 700;
        if (time_tmp[0] - time_now[0] >= speed1)
        {
            time_now[0] = time_tmp[0];
            one.moveDown();
            over = true;
        }
        //�����Ҷ��Ƿ�ʱ��δִ����ȷָ��
        if (score[1] <= 3000)
            speed2 = 1000 - score[1] / 10;
        else
            speed2 = 700;
        if (time_tmp[1] - time_now[1] >= speed2)
        {
            time_now[1] = time_tmp[1];
            two.moveDown();
            over = true;
        }
        if (over)
            break;
        //����ָ��
        if (_kbhit())
        {
            //��˴�Сд
            switch (_getch())
            {
            case 'W':
            case 'w':
                one.moveRotate();
                break;
            case 'A':
            case 'a':
                one.moveLeft();
                break;
            case 'D':
            case 'd':
                one.moveRight();
                break;
            case 'S':
            case 's':
                one.moveDown();
                break;
            case 72:
                two.moveRotate();
                break;
            case 75:
                two.moveLeft();
                break;
            case 77:
                two.moveRight();
                break;
            case 80:
                two.moveDown();
                break;
            }
        }

        // ����CPUռ��
        Sleep(20);
    }
}

int main()
{
    paint starpaint;
    player_1 playerOne(0);
    player_2 playerTwo(1);
    //��ʼ��Ϸ
    //���ƴ���

    while (true)
    {
        starpaint.initEnviroment();
        //ÿ����Ϸ�ĳ�ʼ��
        playerOne.initDatasPerRound();
        playerTwo.initDatasPerRound();

        //ÿ������ĳ�ʼ��
        playerOne.initDatasPerSquare();
        playerTwo.initDatasPerSquare();

        //��ʼһ����Ϸ
        while ((!flag_over[0]) || (!flag_over[1]))
        {
            if (flag_next[0])
                playerOne.initDatasPerSquare();
            if (flag_next[1])
                playerTwo.initDatasPerSquare();
            while ((!flag_next[0]) && (!flag_next[1]))
            {
                BeginBatchDraw();
                //����������ڿ�ʼ������ͼ��ִ�к��κλ�ͼ����������ʱ���������Ļ�ϣ�
                //ֱ��ִ�� FlushBatchDraw �� EndBatchDraw �Ž�֮ǰ�Ļ�ͼ�����
                starpaint.drawGameBG();

                starpaint.drawRightSide();
                starpaint.drawLeftSide();

                starpaint.drawMap();

                starpaint.drawSquareNow(0);
                starpaint.drawSquareNow(1);
                EndBatchDraw();

                getCmd(playerOne, playerTwo);
            }
        }

        //һ�ֽ���
        starpaint.endGame();
        swprintf(over_tips, 39, L"��Ϸ����\n���1�����յ÷֣�%d\n���2�����յ÷֣�%d\n�Ƿ������ھ֣�", score[0], score[1]);
        if (MessageBox(GetHWnd(), over_tips, L"�����ھ֣�", MB_ICONQUESTION | MB_YESNO) == IDNO)
            break;
    }
    return 0;
}