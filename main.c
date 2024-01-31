#include <ctype.h>
#include <locale.h>
#include <ncursesw/ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INIT_ALIVE_CHANCE 12
#define TIMEOUT 70

size_t SIZE = 1000;
unsigned N;
unsigned M;


const int *table[2][2] = {
    {L" ", L"▄"},
    {L"▀", L"█"},
};


static inline unsigned
positive_mod(int num, unsigned m)
{
    int mod = num % (int) m;
    return m * (mod < 0) + mod;
}


unsigned
n_neighbours(bool m[N][M], unsigned x, unsigned y)
{
    register unsigned row;
    register unsigned col;
    register unsigned sum = 0;

    for (int i = -1; i < 2; i++) {
        row = positive_mod(x + i, N);
        for (int j = -1; j < 2; j++) {
            col = positive_mod(y + j, M);

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


bool
check_cell(bool m[N][M], unsigned i, unsigned j)
{
    unsigned n = n_neighbours(m, i, j);
    return (m[i][j] ? n == 2 : false) || n == 3;
}


void
print_matrix(bool n[N][M])
{
    wchar_t buf[M]; buf[M] = 0;

    for (unsigned i = 0; i < N; i += 2) {
        for (unsigned j = 0; j < M; j++) {
            buf[j] = *table[n[i][j]][n[i + 1][j]];
        }

        mvwprintw(stdscr, i / 2, 0, "%ls", buf);
    }

    refresh();
}


void
game(bool n[N][M], bool m[N][M])
{
    for (unsigned i = 0; i < N; i++) {
        for (unsigned j = 0; j < M; j++) {
            n[i][j] = check_cell(m, i, j);
        }
    }

    memcpy(m, n, N * M * sizeof(bool));
    print_matrix(n);

    // Update for resizes.
    getmaxyx(stdscr, N, M);
    N *= 2;
}


void
colors_init()
{
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


void
curses_init()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(TIMEOUT);
    colors_init();
    getmaxyx(stdscr, N, M);
    N *= 2;
}


void
init(bool n[N][M], bool m[N][M])
{
    attron(COLOR_PAIR(rand() % 7 + 1));

    for (unsigned i = 0; i < N; i++) {
        for (unsigned j = 0; j < M; j++) {
            n[i][j] = (rand() % 100) < INIT_ALIVE_CHANCE;
        }
    }

    memcpy(m, n, N * M * sizeof(bool));
}


int
main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "");
    curses_init();

    bool n[SIZE][SIZE];
    bool m[SIZE][SIZE];
    init(n, m);

    int c = 0;
    while ((c = tolower(getch())) != 'q') {
        c == 'r' ? init(n, m) : game(n, m);
    }

    endwin();
    return 0;
}
