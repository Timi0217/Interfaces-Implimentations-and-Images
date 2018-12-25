/*
* Favour Okereke-Mba; FOKERE01
* Timi Dayo-Kayode; ODAYOK01
* COMP 40 = HW 2: PART A: BIT@
* 02/12/2018
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "assert.h"
#include "bit.h"
#include "bit2.h"

/*Incomplete struct that contains a 2D array of bits and information about it*/
struct Bit2_T {
        int column;
        int row;
        Bit_T oneDVector;
};


/*Function: index
  Parameters: Three integer variables to represent the width, row and column
  Purpose: Translates a position in 2D format (x, y) to a singular index in our
           1D representation of a 2D array and returns this translation.
*/
int index(int width,int row, int col){
        return ((row * width) + col);
}


/*Function: Bit2_new
  Parameters: Two integer variables to represent the width, height
  Purpose: Creates a struct of type Bit2_T to hold our 1D representation
           of a 2D array and returns the struct
*/
Bit2_T Bit2_new(int width, int height){
        Bit2_T myVector = malloc(sizeof(struct Bit2_T));
        myVector -> column = width;
        myVector -> row = height;
        myVector -> oneDVector = Bit_new((width * height));
        return myVector;
}

/*Function: Bit2_free
  Parameters: A pointer to an instance of a  Bit2_T struct
  Purpose: Frees the allocated memory in the struct passed in to it as a 
           parameter. 
*/
void Bit2_free(Bit2_T *myVector){
        assert(myVector != NULL && *myVector != NULL);
        Bit2_T temp;
        temp = *myVector;
        Bit_free(&(temp -> oneDVector));
        free(temp);
}


/*Function: Bit2_Width
  Parameters: An instance of a Bit2_T struct
  Purpose: Returns the number of columns in the 2D array the struct contains
*/
int Bit2_width(Bit2_T myVector){
        return myVector -> column;
}


/*Function: Bit2_height
  Parameters: An instance of a Bit2_T struct 
  Purpose: Returns the number of rows in the 2D array the struct contains
*/
int Bit2_height(Bit2_T myVector){
        return myVector -> row;
}

/*Function: Bit2_get
  Parameters: An instance of a Bit2_T struct, two integer variables to 
              represent the row and column.
  Purpose: returns the bit value at the (row, col) of the 2D array in
           the Bit2_T struct.
*/
int Bit2_get(Bit2_T myVector, int col, int row){
        int pos = index((myVector -> column), row, col);
        return Bit_get((myVector -> oneDVector), pos);
}


/*Function: Bit2_put
  Parameters: An instance of a Bit2_T struct, three integer variables to 
              represent the row, column and Bit to be place at array(row, col).
  Purpose: puts the bit value at the (row, col) of the 2D array in
           the Bit2_T struct.
*/
int Bit2_put(Bit2_T myVector, int col, int row, int bit){
        int pos = index((myVector -> column), row, col);
        return Bit_put((myVector -> oneDVector), pos, bit);
}


/*Function: Bit2_map_col_major
  Parameters: An instance of a Bit2_T struct, a function pointer, and a void
              pointer.
  Purpose: Applies the function pointed to by apply to every element of the 
           2D array in the struct in column major order and passes in the 
           void pointer as an optional parameter to the function.
*/
void Bit2_map_col_major(Bit2_T myVector, void apply(int col, int row, 
                                Bit2_T a, int myBit, void *myParam), void *cl){
        int col, row;
        int bitValue;
        for (col = 0; col < myVector -> column; col ++ ){
                for (row = 0; row < myVector -> row; row ++){
                        bitValue = Bit2_get(myVector, col, row);
                        apply(col, row, myVector, bitValue, cl);
                }
        }
}


/*Function: Bit2_map_row_major
  Parameters: An instance of a Bit2_T struct, a function pointer, and a void
              pointer.
  Purpose: Applies the function pointed to by apply to every element of the 
           2D array in the struct in row major order and passes in the 
           void pointer as an optional parameter to the function.
*/
void Bit2_map_row_major(Bit2_T myVector, void apply(int col, int row, 
                                Bit2_T a, int myBit, void *myParam), void *cl){
        int row, col;
        int bitValue;
        for (row = 0; row < myVector -> row; row ++ ){
                for (col = 0; col < myVector -> column; col ++){
                        bitValue = Bit2_get(myVector, col, row);
                        apply(col, row, myVector, bitValue, cl);
                }
        }
}
