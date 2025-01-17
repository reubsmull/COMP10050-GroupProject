#include "initialTokens.h"
#include "gameIO.h"
#include "stackMethods.h"

int onlyAvailablePlacement(cell board[][MAX_COLUMNS], const int placedTokenCount, enum colour playerColour)
{
    int invalidRows[6] = {0};
    int invalidRowcounter = 0;

    for(int row = 0; row < MAX_ROWS; row++)
    {
        int stackValue = countStack(board[row][0].stackPtr);
        if(stackValue != ((placedTokenCount/MAX_ROWS)))
        {
            invalidRows[row] = 1;
            invalidRowcounter++;
        }
    }

    if(invalidRowcounter == 5)
    {
        int validRow;
        for(int row = 0; row < MAX_ROWS; row++)
        {
            if(invalidRows[row] != 1)
            {
                validRow = row;
            }
        }
        if(returnTopValue(board[validRow][0].stackPtr) == playerColour)
        {
            return validRow;
        }
    }

    return -1;
}

int isValidPlacement(cell board[][MAX_COLUMNS], const int placedTokenCount, enum colour playerColour)
{
    bool validPlacement = false; // Flag for valid placement status
    int rowChoice = 0; // Integer variable for storing user's choice of row

    while(validPlacement != true) // While the user has not selected a valid row
    {
        printInstruction("Which row do you wish to place your token in?\n"); // Instruct the user to pick a row
        validInput(&rowChoice, 1, 6); // Validate that their input is an integer and lies within the range 1 - 6
        int stackValue = countStack(board[rowChoice-1][0].stackPtr);
        int topValue = returnTopValue(board[rowChoice-1][0].stackPtr);

        if(placedTokenCount < 6) // If not all of the rows have a token on them
        {
            if(isStackEmpty(board[rowChoice-1][0].stackPtr) == 1) // Only allow user to pick a row that has an empty stack
            {
                validPlacement = true; // Set boolean to true
                printf("Chosen a -1.\n");
            }
            else
            {
                printError("Please select an empty row to minimise stack size.\n"); // Print an appropriate error message to the user
            }
        }

        else if((stackValue == ((placedTokenCount/MAX_ROWS))) && (topValue != playerColour)) // If the chosen row's stack counter is equal to the average token count per row
        {
            validPlacement = true; // Set boolean to true
            printf("Chosen a non-empty stack.\n");
        }

        else if(onlyAvailablePlacement(board, placedTokenCount, playerColour) != -1)
        {
            int onlyValidRow = onlyAvailablePlacement(board, placedTokenCount, playerColour);
            printf("Please select any row except %d:\n", onlyValidRow);
            validInput(&rowChoice, 1, 6);
            if(rowChoice != onlyValidRow)
            {
                validPlacement = true;
                printf("You've picked a valid row (these rules are funny, huh).");
            }
        }

        else
        {
            printError("Please select a valid row to minimise stack size.\n"); // Print an appropriate error message to the user
        }
    }

    return (rowChoice-1); // Returns the (valid) row selected by the user
}

void userPlaceToken(cell board[][MAX_COLUMNS], player playerList[], int player, int row, int column)
{
    /*
    token temp; // Initialises a temporary token
    temp = pop(playerList[player].user_stack, &playerList[player].topOfStack); // Pops the top token from the user's stack and stores it in temp
    push(board[row][column].stack, &board[row][column].topOfStack, temp); // Pushes this token onto the chosen cell stack
    */
    moveToken(&playerList[player].userStack, &board[row][column].stackPtr);
}

void initialTokenPlacement(cell board[][MAX_COLUMNS], const int totalPlayers, player playerList[])
{
    int placedTokenCount = 0;
    int rowChoice = -1;

    for(int round = 0; round < 4; round++)
    {
        for(int player = 0; player < totalPlayers; player++)
        {
            printInstruction("%s's turn:\n", playerList[player].username);
            rowChoice = isValidPlacement(board, placedTokenCount, playerList[player].userColour);
            userPlaceToken(board, playerList, player, rowChoice, 0);
            placedTokenCount++;
        }
    }

}

void obstacleCheck(cell board[][MAX_COLUMNS], int obstacleLocations[])
{
    bool noTokensInColumn[7] = {true, true, true, true, true, true, true};
    bool flag = false;

    /* First check if every column has no tokens in it */
    for(int column = 0; column < 7; column++)
    {
        for(int row = 0; row < MAX_ROWS; row++)
        {
            if(isStackEmpty(board[row][column].stackPtr) != 1) // If the stack is not empty
            {
                noTokensInColumn[column] = false; // Set the relevant boolean to false
            }
        }
    }

    /* Now check if every column before each obstacle has no tokens in it */
    for(int obstacle = 0; obstacle < 6; obstacle++)
    {
        for(int columnCheck = 0; columnCheck < obstacleLocations[obstacle] && flag == false; columnCheck++)
        {
            if(noTokensInColumn[columnCheck] == false)
            {
                flag = true;
            }
        }

        if(flag == false)
        {
            board[obstacle][obstacleLocations[obstacle]].obstacle = inactive;
        }
    }

}