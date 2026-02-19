/* 19/02/2026
 *
 * Simple "chronometer" / "clock" written in C by wilrak0v
 *
 * It uses ncurses so it isn't working on non-unix systems.
 *
 * There are some problems like the rain which is slowly when you toggle the vue to "chronometer".
 *
 * It was written in 2h and 12 min (with pauses into this time)
 *
 * Don't open it on a big terminal because it will take all of your cpu performance...
 * I have to do some improvements for that.
 *
 * */

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#define WIN_WINDTH 12
#define WIN_HEIGHT 3 
#define MAX_DROPS 25

typedef struct {
    int h;
    int m;
    int s;
} my_time;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    int x, y;
    int speed;
} Drop;

Drop rain[MAX_DROPS];

void init_rain(int width, int height)
{
    for (int i = 0 ; i < MAX_DROPS ; i++)
    {
        rain[i].x = rand() % width;
        rain[i].y = rand() % height;
        rain[i].speed = (rand() % 2) + 1;
    }
}

void update_rain(int width, int height)
{
    for (int i = 0; i < MAX_DROPS ; i++)
    {
        rain[i].y += rain[i].speed;
        if (rain[i].y >= height)
        {
            rain[i].y = 0;
            rain[i].x = rand() % width;
        }
    }
}

void draw_rain()
{
    for (int i = 0; i < MAX_DROPS ; i++)
        mvaddch(rain[i].y, rain[i].x, '*');
}

int main()
{
    time_t now; 
    struct tm *time_info;
    int width;
    int height;
    int is_starting = 0;
    int is_hour_vue = 1;
    int chrono_sec = 0;
    char c;
    char *msg = "[T]oggle | [R]eset | [S]tart | [Q]uit";
    // Init ncurses
    initscr();
    curs_set(0);
    nodelay(stdscr, TRUE);
    timeout(1000);

    getmaxyx(stdscr, height, width);
    Position win_pos = {(width - WIN_WINDTH) / 2, (height - WIN_HEIGHT) / 2};
    
    time(&now);
    time_info = localtime(&now);

    WINDOW *win = newwin(WIN_HEIGHT, WIN_WINDTH, win_pos.y, win_pos.x);

    my_time chronometer = { 0, 0, 0 };
    my_time drawed;

    init_rain(width, height); 

    while (1)
    {
        time(&now);
        time_info = localtime(&now);
        c = getch();
        
        if (c == 'q') break;
        if (c == 't') is_hour_vue = !is_hour_vue;
        if (c == 's') is_starting = !is_starting;
        if (c == 'r') {
            chronometer = (my_time){ 0, 0, 0 };
            chrono_sec = 0;
        }

        if (is_hour_vue)
        {
            timeout(50);
            drawed = (my_time){ time_info->tm_hour, time_info->tm_min, time_info->tm_sec };
        }
        else
        {
            timeout(1000);
            chronometer.s = chrono_sec;
            drawed = (my_time) {chronometer.h, chronometer.m, chronometer.s};
        }

        if (!is_starting)
        {
            msg = "[T]oggle | [R]eset | [S]tart | [Q]uit";
        }
        else
        {
            msg = "[T]oggle | [R]eset | [S]top | [Q]uit"; 
            chrono_sec++;
        }

        if ((chrono_sec % 60) == 0 && chrono_sec != 0)
        {
            chrono_sec = 0;
            chronometer.m++;
        }
        if ((chronometer.m % 60) == 0 && chronometer.m != 0)
        {
            chronometer.m = 0;
            chronometer.h++;
        }
        update_rain(width, height);

        erase();
        draw_rain();
        box(stdscr, 0, 0);
        box(win, 0, 0);
        mvwprintw(win, 1, 2, "%02d:%02d:%02d", drawed.h, drawed.m, drawed.s);
       
        wnoutrefresh(stdscr);
        wnoutrefresh(win);
        doupdate();

        mvprintw(height-1, win_pos.x-strlen(msg)/3, "%s", msg);

        refresh();
        wrefresh(win);
        usleep(500);
    }
    curs_set(1);
    endwin();
    return 0;
}
