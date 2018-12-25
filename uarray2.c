/*
* Favour Okereke-Mba; FOKERE01
* Timi Dayo-Kayode; ODAYOK01
* COMP 40 = HW 2: PART A: UARRAY
* 02/12/2018
*/
#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "uarray.h"
#include "uarray2.h"

/*Incomplete struct that contains our 2D array and information about it*/
struct UArray2_T {
        int column;
        int row;
        int size;
        UArray_T oneDArray;
};

/*Function: index
  Parameters: Three integer variables to represent the width, row and column
  Purpose: Translates a position in 2D format (x, y) to a singular index in our
           1D representation of a 2D array and returns this translation.
 */
int index(int width,int row, int col){
        return ((row * width) + col);
}

/*Function: UArray_new
  Parameters: Three integer variables to represent the width, height, and size
  Purpose: Creates a struct of type UArray2_T to hold our 1D representation
           of a 2D array and returns the struct
*/
UArray2_T UArray2_new(int width, int height, int size){
        UArray2_T myArray = malloc(sizeof(struct UArray2_T));
        myArray -> column = width;
        myArray -> row = height;
        myArray -> size = size;
        myArray -> oneDArray = UArray_new((width * height), size);
        return myArray;
}

/*Function: UArray2_free
  Parameters: A pointer to an instance of a  UArray2_T struct
  Purpose: Frees the allocated memory in the struct passed in to it as a 
           parameter. 
*/
void UArray2_free(UArray2_T *myArray){
        assert(myArray != NULL && *myArray != NULL);
        UArray2_T temp;
        temp = *myArray;
        UArray_free(&(temp -> oneDArray));
        free(temp);
}

/*Function: UArray2_Width
  Parameters: An instance of a UArray2_T struct
  Purpose: Returns the number of columns in the 2D array the struct contains
*/
int UArray2_width(UArray2_T myArray){
        return myArray -> column;
}

/*Function: UArray2_height
  Parameters: An instance of a UArray2_T struct 
  Purpose: Returns the number of rows in the 2D array the struct contains
*/
int UArray2_height(UArray2_T myArray){
        return myArray -> row;
}

/*Function: UArray2_size
  Parameters: An instance of a UArray2_T struct
  Purpose: Returns the byte size of the elements in the 2D array
*/
int UArray2_size(UArray2_T myArray){
        return myArray -> size;
}

/*Function: UArray2_at
  Parameters: An instance of a UArray2_T struct, two integer variables to
              represent the row and column.
  Purpose: returns a pointer to the value at the (row, col) of the 2D array in
           the UArray2_T struct.
*/
void *UArray2_at(UArray2_T myArray, int col, int row){
        int pos = index((myArray -> column), row, col);
        return UArray_at((myArray -> oneDArray), pos);
}

/*Function: UArray2_map_col_major
  Parameters: An instance of a UArray2_T struct, a function pointer, and a void
              pointer.
  Purpose: Applies the function pointed to by apply to every element of the 2D 
           array in the struct in column major order and passes in the void 
           pointer as an optional parameter to the function.
*/
void UArray2_map_col_major(UArray2_T myArray, void apply(int col, int row, 
                                UArray2_T a, void *p1, void *p2), void *cl){
        int col, row;
        void *pos;
        for (col = 0; col < myArray -> column; col ++ ){
                for (row = 0; row < myArray -> row; row ++){
                        pos = UArray2_at(myArray, col, row);
                        apply(col, row, myArray, pos, cl);
                }
        }
}

/*Function: UArray2_map_row_major
  Parameters: An instance of a UArray2_T struct, a function pointer, and a void
              pointer.
  Purpose: Applies the function pointed to by apply to every element of the 2D 
           array in the struct in row major order and passes in the void 
           pointer as an optional parameter to the function.
*/
void UArray2_map_row_major(UArray2_T myArray, void apply(int col, int row, 
                                UArray2_T a, void *p1, void *p2), void *cl){
        int row, col;
        void *pos;
        for (row = 0; row < myArray -> row; row ++ ){
                for (col = 0; col < myArray -> column; col ++){
                        pos = UArray2_at(myArray, col, row);
                        apply(col, row, myArray, pos, cl);
                }
        }
}
