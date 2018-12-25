/*
* Favour Okereke-Mba; FOKERE01
* Timi Dayo-Kayode; ODAYOK01
* COMP 40 = HW 2: PART D: Black Edges
* 02/12/2018
*/

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "pnmrdr.h"
#include "bit2.h"
#include <stdbool.h>
#include "seq.h"

/*
Function: pbmread
Parameters: a file pointer
Purpose: Reads in bits from a pbm file and returns a 2D bit array
*/
Bit2_T pbmread(FILE *myFile);

/*
Function: bitQueue
Parameters: a 2D bit array
Purpose: detects black edges on the edge of the bit array and returns a queue
         containing those edges
*/
Seq_T bitQueue(Bit2_T myBitmap);

/*
Function: unblacken
Parameters: a 2D bit array, a queue containing black edges
Purpose: turns all black edges in the bitmap to white
*/
void unblacken(Bit2_T myBitmap, Seq_T blackSeq);

/*
Function: queuePush
Parameters: a queue containing black edges, two intrs representing a row and 
           and column
Purpose: adds a back edge at row, col to the queue
*/
void queuePush(Seq_T blackSeq, int row, int col);

/*
Function:  middleBlack
Parameters: A 2D bit array, a queue containing black edges, two intrs 
            representing a row and and column
Purpose: handles cases of black edges that are not on the edge
*/
void middleBlack(Bit2_T myBitmap, Seq_T blackSeq, int row, int col);

/*
Function: pbmwrite
Parameters: A 2D bit array, a pinter to the file name
Purpose: writes the final bit array to a pbm file 
*/
void pbmwrite(Bit2_T bitmap, char const *fileName);

/*
Function: is corner
Parameters: four ints representing height of the 2D but array,  width of the 
            2D but array, and a row, col pair
Purpose: checks if the position represented by the row, col pair is a corner in 
         the 2D bit array
*/
bool isCorner(int height, int width, int row, int col);

/*
Function: unblackPos
Parameters: A 2D bit array, a queue containing black edges, two intrs 
            representing a row and and column
Purpose: links non-edge black edges with edge blackedges and pushes them onto
         the queue
*/
Seq_T unblackPos(Seq_T blackSeq, Bit2_T myBitmap, int row, int col);


/*Struct that contains the row and col information of a blak edge*/
typedef struct{
        int row;
        int col;
}blackEdge;

/*white bit*/
const int BIT = 0;

int main(int argc, char const *argv[])
{
        FILE *myFile;
        Bit2_T myBitmap;
        Seq_T blackSeq;
        if (argc == 1)
                myFile = stdin;
        else if (argc == 2)
                myFile = fopen(argv[1], "rb");
        else if (argc > 2){
                fprintf(stderr, 
                "unblackedges: Invalid number of command line arguments\n");
                exit(EXIT_FAILURE);
        }

        /*getting a 2D bit array*/
        myBitmap = pbmread(myFile);

        /*getting the queue of edges*/
        blackSeq = bitQueue(myBitmap);

        /*removing black edges*/
        unblacken(myBitmap, blackSeq);

        /*writing the updated pbm file to stdout*/
        pbmwrite(myBitmap, argv[1]);

        /*handling memory deallocation*/
        if (argc == 2)
                fclose(myFile);
        Bit2_free(&myBitmap);
        Seq_free(&blackSeq);
        return 0;
}

Bit2_T pbmread(FILE *myFile){
        Pnmrdr_T analyze;
        Pnmrdr_mapdata bitData;
        Bit2_T tempBit;
        int i, j, pixel, check = 0;
        analyze = Pnmrdr_new(myFile);
        bitData = Pnmrdr_data(analyze);
        assert(bitData.type == Pnmrdr_bit);
        assert(bitData.width > 0 && bitData.height > 0);
        tempBit = Bit2_new(bitData.width, bitData.height);
        for(i = 0; i < (int) bitData.height; i++){
                for (j = 0; j < (int) bitData.width; j++){
                        pixel = Pnmrdr_get(analyze);
                        if (pixel != 1 && pixel != 0){
                                check = 1;
                        }
                        else
                                Bit2_put(tempBit, j, i, pixel);         
                }
        }
        Pnmrdr_free(&analyze);
        if (check != 0){
                fprintf(stderr, "unblackedges: invalid pbm file\n");
                Bit2_free(&tempBit);
                fclose(myFile);
                exit(EXIT_FAILURE);
        }
        return tempBit;
}

Seq_T bitQueue(Bit2_T myBitmap){
        int row, col;
        int height = Bit2_height(myBitmap), width = Bit2_width(myBitmap);
        Seq_T mySeq = Seq_new(width);
        /*reading in black edges on the edge rows*/
        for (row = 0; row < height; row += (height - 1)){
                for (col = 0; col < width; col++){
                        if (Bit2_get(myBitmap, col, row) == 1){
                                Bit2_put(myBitmap, col, row, BIT);
                                blackEdge *myEdge = malloc(sizeof(blackEdge));
                                myEdge->row = row;
                                myEdge->col = col;
                                Seq_addhi(mySeq, myEdge);
                        }
                }
        }
        /*reading in black edges on the edge columns*/
        for (col = 0; col < width; col += (width - 1)){
                for (row = 1; row < (height - 1); row++){
                        if (Bit2_get(myBitmap, col, row) == 1){
                                Bit2_put(myBitmap, col, row, BIT);
                                blackEdge *myEdge = malloc(sizeof(blackEdge));
                                myEdge->row = row;
                                myEdge->col = col;
                                Seq_addhi(mySeq, myEdge);
                        }               
                }       
        }
        return mySeq;
}


void unblacken(Bit2_T myBitmap, Seq_T blackSeq){
        int row, col;
        blackEdge *currPosition;
        int height = Bit2_height(myBitmap), width = Bit2_width(myBitmap);
        /*picking black edges off the queue untill it is empty*/
        while(Seq_length(blackSeq) > 0){
                currPosition = Seq_remlo(blackSeq);
                row = currPosition->row;
                col = currPosition->col;
                free(currPosition);
                if(isCorner(height, width, row, col))
                        Bit2_put(myBitmap, col, row, BIT);
                else
                        blackSeq = unblackPos(blackSeq, myBitmap, row, col);
        }
}

bool isCorner(int height, int width, int row, int col){
        if(row == 0 && col == 0)
                return true;
        else if(row == 0 && col == (width -1))
                return true;
        else if(row == (height - 1) && col == 0)
                return true;
        else if(row == (height - 1) && col == (width - 1))
                return true;
        else
                return false;
}

Seq_T unblackPos(Seq_T blackSeq, Bit2_T myBitmap, int row, int col){
        int height = Bit2_height(myBitmap), width = Bit2_width(myBitmap);
        if (row == 0){
                if((Bit2_get(myBitmap, col, (row + 1)) == 1)){
                        queuePush(blackSeq, (row + 1), col);
                        Bit2_put(myBitmap, col, row + 1, BIT);
                }               
        }
        else if (row == (height - 1)){
                if ((Bit2_get(myBitmap, col, (height - 2)) == 1)){
                        queuePush(blackSeq, (height - 2), col);
                        Bit2_put(myBitmap, col, height - 2, BIT);
                }
        }
        else if (col == 0){
                if ((Bit2_get(myBitmap, (col + 1), row) == 1)){
                        queuePush(blackSeq, row, (col + 1));
                        Bit2_put(myBitmap, col + 1, col, BIT);
                }
        }
        else if (col == (width - 1)){
                if ((Bit2_get(myBitmap, (width - 2), row) == 1)){
                        queuePush(blackSeq, row, (width - 2));
                        Bit2_put(myBitmap, width - 2, row, BIT);
                }
        }
        else
                middleBlack(myBitmap, blackSeq, row, col);
        return blackSeq;
}

void middleBlack(Bit2_T myBitmap, Seq_T blackSeq, int row, int col){
        if ((Bit2_get(myBitmap, (col - 1), row) == 1)){
                queuePush(blackSeq, row, (col - 1));
                Bit2_put(myBitmap, col - 1, row, BIT);
        }
        if ((Bit2_get(myBitmap, (col + 1), row) == 1)){
                queuePush(blackSeq, row, (col + 1));
                Bit2_put(myBitmap, col + 1, row, BIT);
        }
        if ((Bit2_get(myBitmap, col, (row - 1)) == 1)){
                queuePush(blackSeq, (row - 1), col);
                Bit2_put(myBitmap, col, row - 1, BIT);
        }
        if ((Bit2_get(myBitmap, col, (row + 1)) == 1)){
                queuePush(blackSeq, (row + 1), col);
                Bit2_put(myBitmap, col, row + 1, BIT);
        }
}

void queuePush(Seq_T blackSeq, int row, int col){
        blackEdge *myPtr;
        myPtr = malloc(sizeof(blackEdge));
        myPtr->row = row;
        myPtr->col = col;
        Seq_addhi(blackSeq, myPtr);
}

void pbmwrite(Bit2_T bitMap, char const *fileName){
        int row, col;
        int height = Bit2_height(bitMap), width = Bit2_width(bitMap);
        printf("P1\n# %s\n%d %d\n", fileName, width, height);
        for (row = 0; row < height; row++){
                for (col = 0; col < width; col++)
                {
                        if (row > 0 && row % 70 == 0)
                                printf("%d\n", Bit2_get(bitMap, col, row));
                        else
                                printf("%d ", Bit2_get(bitMap, col, row));
                }
                printf("\n");
        }
}
