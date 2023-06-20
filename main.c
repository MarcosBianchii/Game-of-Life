#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ncurses.h>
#include <ctype.h>
#include <time.h>

uint64_t N;
uint64_t M;

uint8_t n_neighbours(bool m[N][M], int64_t x, int64_t y) {
    uint8_t sum = 0;
    for (int64_t i = x-1; i < x+2; i++)
        for (int64_t j = y-1; j < y+2; j++) {
            if (i < 0 || j < 0 || i >= N || j >= M)
                continue;
            
            switch (sum) {
                case 4: return 4;
                default: sum += i == x && j == y ? 0 : m[i][j];
            }
        }
    
    return sum;
}

bool check_cell(bool m[N][M], int64_t i, int64_t j) {
    uint8_t n = n_neighbours(m, i, j);
    return m[i][j] ? (n == 2 || n == 3) : (n == 3);
}

void print_matrix(bool m[N][M]) {
    for (uint64_t i = 0; i < N; i++)
        for (uint64_t j = 0; j < M; j++)
            mvaddch(i, j, m[i][j] ? '#' : ' ');
    refresh();
}

void game(bool m[N][M]) {
    for (int64_t i = 0; i < N; i++)
        for (int64_t j = 0; j < M; j++)
            m[i][j] = check_cell(m, i, j);
    print_matrix(m);
}

void init(bool m[N][M]) {
    attron(COLOR_PAIR(rand() % 8));
    
    for (int64_t i = 0; i < N; i++)
        for (int64_t j = 0; j < M; j++)
            m[i][j] = M/N < N/(rand() % 20*M + 1);
}

void colors_init() {
    start_color();
    attron(A_BOLD);
    use_default_colors();
    init_pair(0, COLOR_BLACK,   -1);
    init_pair(1, COLOR_RED,     -1);
    init_pair(2, COLOR_GREEN,   -1);
    init_pair(3, COLOR_YELLOW,  -1);
    init_pair(4, COLOR_BLUE,    -1);
    init_pair(5, COLOR_MAGENTA, -1);
    init_pair(6, COLOR_CYAN,    -1);
    init_pair(7, COLOR_WHITE,   -1);
}

void curses_init() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(55);
    notimeout(stdscr, true);
    colors_init();
    getmaxyx(stdscr, N, M);
}

int main() {
    srand(time(NULL));
    curses_init();
    bool m[N][M];
    init(m);

    int c = 0;
    do (c = tolower(getch())) == 'r' ? init(m) : game(m);
    while (c != 'q');

    endwin();
    return 0;
}
