#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int** csv_to_mat(char* filepath);
void sudoku_print(int** sudoku);
int check_num(int** sudoku, int num, int index);
int solve(int** sudoku, int index);

int main() {
    printf("\n--------- Test 1 ---------\n");
    int** sudoku = csv_to_mat("./test/s1.txt");
    sudoku_print(sudoku);
    solve(sudoku, 0);
    sudoku_print(sudoku);

    printf("\n--------- Test 2 ---------\n");
    sudoku = csv_to_mat("./test/s2.txt");
    sudoku_print(sudoku);
    solve(sudoku, 0);
    sudoku_print(sudoku);

    printf("\n--------- Test 3 ---------\n");
    sudoku = csv_to_mat("./test/s3.txt");
    sudoku_print(sudoku);
    solve(sudoku, 0);
    sudoku_print(sudoku);

    free(sudoku);
    return 0;
}

int check_num(int** sudoku, int num, int index) {
    int row = index / 9;
    int col = index % 9;
    //---------------------- checking for lines and columns -----------------------
    for (int x = 0; x < 9; x++) if (num == sudoku[row][x] || num == sudoku[x][col]) return 0;
    //-------------- checking for the 3x3 boxe around my coordinates --------------
    //? e.g.: row = 1, col = 3
    int h_del = 3*(row/3 + 1); //? horizontal delimiter          | e.g.: h_del = 3; 
    int v_del = 3*(col/3 + 1); //? vertical delimiter            | e.g.: v_del = 6;
    int h_start = 3*(row/3);   //? horizontal starting variable  | e.g.: i = 0;
    int v_start = 3*(col/3);   //? vertical starting variable    | e.g.: j = 3;

    //? nested loops to iterate in the 3x3 box
    for (int i = h_start; i < h_del; ++i) 
        for (int j = v_start; j < v_del; ++j)
            if (num == sudoku[i][j]) return 0;
    //-----------------------------------------------------------------------------
    return 1; // if the number seccessed the previous tests is ok!
}

int solve(int** sudoku, int index) {
    if (index > 80) return 1;               //? if it reaches 80, the sudoku is complete!
    int row = index / 9;
    int col = index % 9;
    //sudoku_print(sudoku); //enable for debugging
    if (sudoku[row][col] != 0) return solve(sudoku, index+1);
    for (int num = 1; num <= 9; num++) {
        if (check_num(sudoku, num, index)) {
            sudoku[row][col] = num;         //? if the num is ok inserts it in 'sudoku'.
            if (solve(sudoku, index+1)) {
                solve(sudoku, index+1);     //? if this scenario is ok, is choosen this way.
                return 1;
            }
            else sudoku[row][col] = 0;      //? else try another num.
        }
    }
    return 0;
}


int** csv_to_mat(char* filepath) {
    int** mat = (int**) malloc(9*8);
    int** tmp = mat;
    for (int i = 0; i < 9; i++) {
        mat[i] = (int*) malloc(9*4);
    }
    FILE* f = fopen(filepath, "r");
    if (!f) {
        printf("An error occurred!");
        return NULL;
    }
    char* line = (char*) malloc(20);

    for (int r = 0; r < 9; r++) {
        line = fgets(line, 18, f);
        char* value = strtok(line, ",");
        if (value != NULL) {
                if (value == " ") mat[r][0] = 0;
                mat[r][0] = atoi(value);
            }
        for (int c = 1; c < 9; c++) {
            value = strtok(NULL, ",");
            if (value != NULL) {
                if (value == " ") mat[r][c] = 0;
                else mat[r][c] = atoi(value);
            }
            else strtok(NULL, "\n");
        }
        line = fgets(line, 18, f);
    }
    return mat;
}

void sudoku_print(int** sudoku) {
    if (!sudoku) return;
    char* tmp = (char*) malloc(3);
    char* buffer = (char*) malloc(730);
    buffer = strcat(buffer, "╔═══════╦═══════╦═══════╗\n");
    for (int row = 0; row < 9; row++){
        buffer = strcat(buffer, "║");
        for (int col = 0; col < 9; col++){
            sprintf(tmp, " %d", sudoku[row][col]);
            if (strcmp(tmp, " 0") == 0) 
                buffer = strcat(buffer, "  ");
            else buffer = strcat(buffer, tmp);
            if ((col+1) % 3 == 0) strcat(buffer, " ║");;
            if (col == 8) buffer = strcat(buffer, "\n");
        } 
        if (row == 2 || row == 5)
            buffer = strcat(buffer, "╠═══════╬═══════╬═══════╣\n");
    }
    buffer = strcat(buffer, "╚═══════╩═══════╩═══════╝\n\0");

    //system("clear");
    printf(buffer);
}