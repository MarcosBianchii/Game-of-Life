#include <ctype.h>
#include <locale.h>
#include <ncursesw/ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INIT_ALIVE_CHANCE 12

uint32_t N;
uint32_t M;

int *table[2][2] = {
    {L" "     , L"\u2584"},
    {L"\u2580", L"\u2588"},
};

uint8_t n_neighbours(bool m[N][M], int64_t x, int64_t y) {
    register int64_t row;
    register int64_t col;
    register uint8_t sum = 0;

    for (int64_t i = 0; i < 3; i++) {
        row = (x + i - 1) % N;
        for (int64_t j = 0; j < 3; j++) {
            col = (y + j - 1) % M;

            if (row == x && col == y) {
                continue;
            }

            if ((sum += m[row][col]) == 4) {
                return 4;
            }
        }
    }
    
    return sum;
}

bool check_cell(bool m[N][M], int64_t i, int64_t j) {
    uint8_t n = n_neighbours(m, i, j);
    return (m[i][j] ? n == 2 : false) || n == 3;
}

void print_matrix(bool n[N][M]) {
    wchar_t buf[M]; buf[M] = 0;

    for (uint32_t i = 0; i < N; i += 2) {
        for (uint32_t j = 0; j < M; j++) {
            buf[j] = *table[n[i][j]][n[i + 1][j]];
        }

        mvwprintw(stdscr, i / 2, 0, "%ls", buf);
    }

    refresh();
}

void game(bool n[N][M], bool m[N][M]) {
    for (uint32_t i = 0; i < N; i++) {
        for (uint32_t j = 0; j < M; j++) {
            n[i][j] = check_cell(m, i, j);
        }
    }

    memcpy(m, n, sizeof(bool) * N * M);
    print_matrix(n);
}

void init(bool n[N][M], bool m[N][M]) {
    attron(COLOR_PAIR(rand() % 7 + 1));

    for (uint32_t i = 0; i < N; i++) {
        for (uint32_t j = 0; j < M; j++) {
            n[i][j] = (rand() % 100) < INIT_ALIVE_CHANCE;
        }
    }

    memcpy(m, n, sizeof(bool) * N * M);
}

void colors_init() {
    start_color();
    attron(A_BOLD);
    use_default_colors();
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
    timeout(70);
    colors_init();
    getmaxyx(stdscr, N, M);
    N *= 2;
}

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "");
    curses_init();

    bool n[N][M];
    bool m[N][M];
    init(n, m);

    int c = 0;
    do (c = tolower(getch())) == 'r' ? init(n, m) : game(n, m);
    while (c != 'q');

    endwin();
    return 0;
}
