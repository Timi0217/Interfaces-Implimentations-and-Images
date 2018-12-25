/*
* Favour Okereke-Mba; FOKERE01
* Timi Dayo-Kayode; ODAYOK01
* COMP 40 = HW 2: PART A: BIT2
* 02/12/2018
*/
#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

/*Incomplete struct that contains a 2D array of bits and information about it*/
typedef struct Bit2_T *Bit2_T;

/*Function: Bit2_new
  Parameters: Two integer variables to represent the width, height
  Purpose: Creates a struct of type Bit2_T to hold our 1D representation
           of a 2D array and returns the struct
*/
extern Bit2_T Bit2_new(int width, int height);

/*Function: Bit2_free
  Parameters: A pointer to an instance of a  Bit2_T struct
  Purpose: Frees the allocated memory in the struct passed in to it as a 
           parameter. 
*/
extern void Bit2_free(Bit2_T *myArray);

/*Function: Bit2_Width
  Parameters: An instance of a Bit2_T struct
  Purpose: Returns the number of columns in the 2D array the struct contains
*/
extern int Bit2_width(Bit2_T myArray);

/*Function: Bit2_height
  Parameters: An instance of a Bit2_T struct 
  Purpose: Returns the number of rows in the 2D array the struct contains
*/
extern int Bit2_height(Bit2_T myArray); 

/*Function: Bit2_get
  Parameters: An instance of a Bit2_T struct, two integer variables to 
              represent the row and column.
  Purpose: returns the bit value at the (row, col) of the 2D array in
           the Bit2_T struct.
*/
extern int Bit2_get(Bit2_T myArray, int col, int row); 

/*Function: Bit2_put
  Parameters: An instance of a Bit2_T struct, three integer variables to 
              represent the row, column and Bit to be place at array(row, col).
  Purpose: puts the bit value at the (row, col) of the 2D array in
           the Bit2_T struct.
*/
extern int Bit2_put(Bit2_T myArray, int col, int row, int bit); 

/*Function: Bit2_map_col_major
  Parameters: An instance of a Bit2_T struct, a function pointer, and a void
              pointer.
  Purpose: Applies the function pointed to by apply to every element of the 
           2D array in the struct in column major order and passes in the 
           void pointer as an optional parameter to the function.
*/
extern void Bit2_map_col_major(Bit2_T myArray, void apply(int col, int row, 
                                Bit2_T a, int myBit, void *myParam), void *cl);

/*Function: Bit2_map_row_major
  Parameters: An instance of a Bit2_T struct, a function pointer, and a void
              pointer.
  Purpose: Applies the function pointed to by apply to every element of the 
           2D array in the struct in row major order and passes in the 
           void pointer as an optional parameter to the function.
*/
extern void Bit2_map_row_major(Bit2_T myArray, void apply(int col, int row, 
                                Bit2_T a, int myBit, void *myParam), void *cl);


#endif