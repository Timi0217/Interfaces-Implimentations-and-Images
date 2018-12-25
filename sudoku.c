#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "uarray2.h"
#include "pnmrdr.h"
#include <stdbool.h>

enum { STDIN = 1, COM_ARG = 2};

/*struct used to validate the whole sudoku board*/
typedef struct{
        int count;
        int sum;
        char repeatCheck[10];
}validator;

/*
Function: imageCheck
Parameters: Pnmrdr_mapdata myImage
Purpose: Validates the file type provides and returns true or 
        false depending on the type
*/
bool imageCheck(Pnmrdr_mapdata myImage);

/*
Function: structResest
Parameters: a pointer to a validator struct
Purpose: resets the values of the elements of the struct 
*/
void structReset(validator *boardCheck);

/*
Function: freeData
Parameters: two pointers to pointers that point to 2D UArrays
Purpose: deallocates the memory used for the 2D arrays
*/
void freeData(UArray2_T *myArray, UArray2_T *group);

/*
Function: readImage
Parameters: two pointers to pointers that point to 2D UArrays, 
            Pnmrdr_mapdata imageData, Pnmrdr_T analyze
Purpose: reads in the sudoku board as is into a 2D array and converts each
         box group of 9 in a sudoku board into rows and places that data in a 
         seperate 2D array
*/
bool readImage(UArray2_T *myArray, UArray2_T *group, 
        Pnmrdr_mapdata imageData, Pnmrdr_T analyze);

/*
Function: functionCaller
Parameters: Pnmrdr_mapdata imageData, Pnmrdr_T analyze
Purpose: calls a sequence of other functions to help validate 
        the sudoku board
*/
int functionCaller(Pnmrdr_mapdata imageData, Pnmrdr_T analyze);

/*
Function: validateSudoku
Parameters: an int that represents the number of command line arguments, the 
            file name and a file pointer
Purpose: opens the sudoku file passed in and kick starts the 
        program by calling other functions
*/
int validateSudoku(int argc, char const *argv[], FILE *myFile);

/*
Function: groupPos
Parameters: two pointers to ints and three pointers to chars
Purpose: converts each box group of 9 in a sudoku board into rows and places 
         that data row format in a 2D array
*/
void groupPos(int *row1, int *col1, char *column, char *strRow, char *RESET);

/*
Function: boardValidate
Parameters: two ints representing a row and a column, a pointer to a 2D array
        a pointer to the value at (row, col) in the 2D array and a pointer
        to a validator struct
Purpose: this is a map function that is mapped both row major and col major 
         over the 2D array that contains the sudoku board as is, and is mapped 
         row major over the 2D array that contains the boxes in row format
*/
void boardValidate(int row, int col, UArray2_T myArray, void *val, void *info);

int main(int argc, char const *argv[])
{
        FILE *image;
        image = NULL;
        return validateSudoku(argc, argv, image);
}

int validateSudoku(int argc, char const *argv[], FILE *myFile){
        Pnmrdr_T analyze;
        int result;
        if (argc == STDIN) {
                analyze = Pnmrdr_new(stdin);
        }
        //Reading in file as a command line argument
        else if (argc == COM_ARG) {
                myFile = fopen(argv[1], "rb");
                if (myFile == NULL)
                        exit(EXIT_FAILURE);
                analyze = Pnmrdr_new(myFile);
        }
        else
                exit(EXIT_FAILURE);

        Pnmrdr_mapdata imageData = Pnmrdr_data(analyze);
        result = functionCaller(imageData, analyze);
        fclose(myFile);
        Pnmrdr_free(&analyze);
        return result;
}

int functionCaller(Pnmrdr_mapdata imageData, Pnmrdr_T analyze){
        if(!(imageCheck(imageData)))
                return 1;
        UArray2_T normArray;
        UArray2_T groupArray;
        
        if(!(readImage(&normArray, &groupArray, imageData, analyze))){
                return 1;
        }

        validator boardCheck;
        structReset(&boardCheck);
        UArray2_map_row_major(normArray, boardValidate, &boardCheck);
        if (boardCheck.count != 9){
                freeData(&normArray, &groupArray);
                return 1;
        }
        structReset(&boardCheck);
        UArray2_map_col_major(normArray, boardValidate, &boardCheck);
        if (boardCheck.count != 9){
                freeData(&normArray, &groupArray);
                return 1;
        }
        structReset(&boardCheck);
        UArray2_map_row_major(groupArray, boardValidate, &boardCheck);
        if (boardCheck.count != 9){
                freeData(&normArray, &groupArray);
                return 1;
        }
        freeData(&normArray, &groupArray);
        return 0;
}

bool imageCheck(Pnmrdr_mapdata myImage){
        if (myImage.type != Pnmrdr_gray || 
                (myImage.width * myImage.height) <= 0){
               exit(EXIT_FAILURE);
        }

        if (myImage.denominator != 9)
               return false;
        else
               return true;
}

bool readImage(UArray2_T *myArray, UArray2_T *group, 
        Pnmrdr_mapdata imageData, Pnmrdr_T analyze){
        int row, col, num, row1, col1, check = 0;
        char pos[10] = "AAAAAAAAAA";
        char rowStrart[4] = "AAAA";
        char RESET = 'A';
        row = imageData.height;
        col = imageData.width;
        UArray2_T value = UArray2_new(col, row, (int)sizeof(int));
        UArray2_T value2 = UArray2_new(col, row, (int)sizeof(int));

        for (int i = 0; i < row; ++i){
                for (int j = 0; j < col; ++j)
                {
                        num = Pnmrdr_get(analyze);
                        if(num == 0 || num > 9){
                                check = 1;
                        }
                        *(int *)UArray2_at(value, j, i) = num;
                        groupPos(&row1, &col1, pos, rowStrart, &RESET);
                        *(int *)UArray2_at(value2, col1, row1) = num;
                 }
        }
        if (check == 1){
                freeData(&value, &value2);
                return false;
        }
        else{
                *myArray = value;
                *group = value2;
                return true;
        }
}            
                                                                        
void groupPos(int *row1, int *col1, char *column, char *strRow, char *RESET){
        *row1 = (int)(strRow[0] - 'A');
        strRow[1]++;
        if (strRow[1] == 'D'){
                strRow[0]++;
                strRow[2]++;
                strRow[1] = 'A';
                if (strRow[2] == 'D'){
                        if (strRow[3] == 'C'){
                                *RESET = strRow[0];
                                strRow[3] = 'A';
                                strRow[2] = 'A';
                        }
                        else{
                                strRow[0] = *RESET;
                                strRow[2] = 'A';
                                strRow[3]++;
                        }
                }
        }
        *col1 = (int)(column[*row1] - 'A');
        column[*row1]++;
}

void structReset(validator *boardCheck){
        boardCheck->sum = 0;
        boardCheck->count = 0;
        for (int i = 0; i < 10; i++)
        {
                boardCheck->repeatCheck[i] = 'F'; 
        }

}

void boardValidate(int row, int col, UArray2_T myArray, void *val, void *info){

        (void)row;
        (void)myArray;
        ((validator *)info)->sum += *(int *)val;

        if (((validator *)info)->repeatCheck[*(int *)val] == 'F'){
                ((validator *)info)->repeatCheck[*(int *)val] = 'T';
        }
        if (col == 8 || row == 8){
                if (((validator *)info)->sum == 45 && 
                        strcmp(((validator *)info)->repeatCheck, "FTTTTTTTT"))
                {
                        ((validator *)info)->count++;
                        ((validator *)info)->sum = 0;
                        for (int i = 0; i < 10; i++)
                        {
                                ((validator *)info)->repeatCheck[i] = 'F'; 
                        }

                }
        }
}

void freeData(UArray2_T *myArray, UArray2_T *group){
        UArray2_free(myArray);
        UArray2_free(group);
}