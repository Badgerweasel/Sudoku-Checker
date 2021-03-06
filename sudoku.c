#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define true 1
#define false 0

struct data
{
  int row;
  int column;
  int block;
  int (*sudoku)[9];
  bool valid;
  int counter;
  int rowMax;
  int columnMax;
};
  
void *checkSudoku(void *arg);
void *checkRow(struct data *arg);
void *checkColumn(struct data *arg);
void *checkBlock(struct data *arg);


int main()
{
  //2D array to hold sudoku grid
  int sudoku[9][9];
  pthread_t checkers[27];
  int threadCount = 0;
  struct data *storage[27];
  int err;
  
  //parse input into 2d sudoku grid
  for(int i = 0; i < 9; i++){
    for(int j = 0; j < 9; j++){
      scanf("%d", &sudoku[i][j]);
    }
  }

  for(int i = 0; i < 9; i++)
    {
      
      struct data *checkRow = malloc(sizeof(struct data));
      (*checkRow).valid = false;
      checkRow->row = i+1;
      checkRow->column = 0;
      checkRow->block = 0;
      checkRow->counter = 0;
      checkRow->sudoku = sudoku;
      
      storage[threadCount] = checkRow;

      err = pthread_create(&checkers[threadCount], NULL, checkSudoku, (void*) checkRow);
      if(err != 0)
	{
	  printf("Create failed :[%s]\n", strerror(err));
	}
      threadCount++;
      
      struct data *checkColumn = malloc(sizeof(struct data));
      (*checkColumn).valid = false;
      checkColumn->row = 0;
      checkColumn->column = i+1;
      checkColumn->block = 0;
      checkColumn->counter = 0;
      checkColumn->sudoku = sudoku;

      storage[threadCount] = checkColumn;
      
      err = pthread_create(&checkers[threadCount], NULL, checkSudoku, (void*) checkColumn);
      if(err != 0)
	{
	  printf("Create failed :[%s]\n", strerror(err));
	}
      threadCount++;

      struct data *checkBlock = malloc(sizeof(struct data));
      (*checkBlock).valid = false;
      checkBlock->row = 0;
      checkBlock->column = 0;
      checkBlock->block = i+1;
      checkBlock->counter = 0;
      checkBlock->sudoku = sudoku;

      storage[threadCount] = checkBlock;
      
      err = pthread_create(&checkers[threadCount], NULL, checkSudoku, (void*) checkBlock);
      if(err != 0)
	{
	  printf("Create failed :[%s]\n", strerror(err));
	}
      threadCount++;
	
    }

  for(int i = 0; i < threadCount; i++)
    {
      if((err = pthread_join(checkers[i], NULL)) != 0)
	{
	  printf("Join failed :[%s]\n", strerror(err));
	}
    }

  bool valid = true;
  for(int i = 0; i < threadCount; i++)
    {
      if(!storage[i]->valid)
	{
	  valid = false;
	}
      free(storage[i]);
    }

  if(valid)
    {
      printf("This is a valid sudoku\n");
    }
  else
    {
      printf("This is not a valid sudoku\n");
    }

  
  return 0;
}

void *checkSudoku(void *checkData){
	struct data *sudokuData = (struct data*) checkData;

	if(sudokuData->row != 0){
		checkRow(sudokuData);
		if(!(*sudokuData).valid)
		  printf("row %d is invalid \n", sudokuData->row);
	}
	else if(sudokuData->column != 0){
		checkColumn(sudokuData);
                if(!(*sudokuData).valid)
		  printf("column %d is invalid \n", sudokuData->column);
	}
	else if(sudokuData->block != 0){
		checkBlock(sudokuData);
		if(!(*sudokuData).valid)
		  {
		    if(sudokuData->block == 1){
		      printf("upper left subgrid is invalid \n");
		    }
		    else if(sudokuData->block == 2){
		      printf("upper middle subgrid is invalid \n");
		    }
		    else if(sudokuData->block == 3){
		      printf("upper right subgrid is invalid \n");
		    }
		    else if(sudokuData->block == 4){
		      printf("middle left subgrid is invalid \n");
		    }
		    else if(sudokuData->block == 5){
		      printf("center subgrid is invalid \n");
		    }
		    else if(sudokuData->block == 6){
		      printf("middle right subgrid is invalid \n");
		    }
		    else if(sudokuData->block == 7){
		      printf("bottom left subgrid is invalid \n");
		    }
		    else if(sudokuData->block == 8){
		      printf("bottom middle subgrid is invalid \n");
		    }
		    else if(sudokuData->block == 9){
		      printf("bottom right subgrid is invalid \n");
		    }
		  }
	}
	else{
		printf("not row, column, or block \n");
	}
}

void *checkRow(struct data *checkRow)
{
  while((*checkRow).counter < 9)
    {
      while((*checkRow).column < 9)
	{
	  if((*checkRow).sudoku[(*checkRow).row - 1][(*checkRow).column] == (*checkRow).counter + 1)
	    {
	      (*checkRow).valid = true;
	    }
	  (*checkRow).column++;
	}
      if(!(*checkRow).valid)
	{
	  return 0;
	}
      if((*checkRow).counter < 8)
	{
	  (*checkRow).valid = false;
	}
      (*checkRow).counter++;
      (*checkRow).column = 0;
    }
}

void *checkColumn(struct data *checkColumn)
{
  while((*checkColumn).counter < 9)
    {
      while((*checkColumn).row < 9)
	{
	  if((*checkColumn).sudoku[(*checkColumn).row][(*checkColumn).column - 1] == (*checkColumn).counter + 1)
	    {
	      (*checkColumn).valid = true;
	    }
	  (*checkColumn).row++;
	}
      if(!(*checkColumn).valid)
	{
	  return 0;
	}
      if((*checkColumn).counter < 8)
	{
	  (*checkColumn).valid = false;
	}
      (*checkColumn).counter++;
      (*checkColumn).row = 0;
    }
  }

void *checkBlock(struct data *checkBlock)
{

  if((*checkBlock).block == 1){
                (*checkBlock).row = 0;
		(*checkBlock).rowMax = 3;
		(*checkBlock).column = 0;
		(*checkBlock).columnMax = 3;
	}
	else if((*checkBlock).block == 2){
	        (*checkBlock).row = 0;
		(*checkBlock).rowMax = 3;
                (*checkBlock).column = 3;
		(*checkBlock).columnMax = 6;
        }
	else if((*checkBlock).block == 3){
	        (*checkBlock).row = 0;
		(*checkBlock).rowMax = 3;
                (*checkBlock).column = 6;
		(*checkBlock).columnMax = 9;
        }
	else if((*checkBlock).block == 4){
	        (*checkBlock).row = 3;
		(*checkBlock).rowMax = 6;
		(*checkBlock).column = 0;
		(*checkBlock).columnMax = 3;
	}
	else if((*checkBlock).block == 5){
	        (*checkBlock).row = 3;
		(*checkBlock).rowMax = 6;
                (*checkBlock).column = 3;
		(*checkBlock).columnMax = 6;
        }
	else if((*checkBlock).block == 6){
	        (*checkBlock).row = 3;
		(*checkBlock).rowMax = 6;
                (*checkBlock).column = 6;
		(*checkBlock).columnMax = 9;
        }
	else if((*checkBlock).block == 7){
	        (*checkBlock).row = 6;
		(*checkBlock).rowMax = 9;
                (*checkBlock).column = 0;
		(*checkBlock).columnMax = 3;
        }
	else if((*checkBlock).block == 8){
	        (*checkBlock).row = 6;
		(*checkBlock).rowMax = 9;
                (*checkBlock).column = 3;
		(*checkBlock).columnMax = 6;
        }
	else if((*checkBlock).block == 9){
	        (*checkBlock).row = 6;
		(*checkBlock).rowMax = 9;
                (*checkBlock).column = 6;
		(*checkBlock).columnMax = 9;
        }

  while((*checkBlock).counter < 9)
    {
      while((*checkBlock).row < (*checkBlock).rowMax)
	{
	  while((*checkBlock).column < (*checkBlock).columnMax)
	    {
	      if((*checkBlock).sudoku[(*checkBlock).row][(*checkBlock).column] == (*checkBlock).counter + 1)
		{
		  (*checkBlock).valid = true;
		}
	      (*checkBlock).column++;
	    }
	  (*checkBlock).column -= 3;
	  (*checkBlock).row++;
	}
      if(!(*checkBlock).valid)
	{
	  return 0;
	}
      if((*checkBlock).counter < 8)
	{
	  (*checkBlock).valid = false;
	}
      (*checkBlock).counter++;
      (*checkBlock).row -= 3;
      
    }
}
