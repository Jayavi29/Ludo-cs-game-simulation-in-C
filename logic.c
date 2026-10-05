#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "types.h"

player Player[NUM_OF_PLAYERS];
int diceRoll()
{
    return (rand() % 6) + 1;
}

int nextplayer(int currentPlayer)  
{
    return (currentPlayer + 1) % 4;
}

void InitializePlayer (void)
{
    for (int i = 0; i < NUM_OF_PLAYERS; i++)
    {
        if (i == 0)
        {
            strcpy(Player[i].color, "Red");
            Player[i].startingPosition = RED_X;
            Player[i].homeStraightEntery = RED_APPROACH;

        }

        if (i == 1)
        {
            strcpy(Player[i].color, "Green");
            Player[i].startingPosition = GREEN_X;
            Player[i].homeStraightEntery = GREEN_APPROACH;
        }

        if (i == 2)
        {
            strcpy(Player[i].color, "Yellow");
            Player[i].startingPosition = YELLOW_X;
            Player[i].homeStraightEntery = YELLOW_APPROACH;
        }

        if (i == 3)
        {
            strcpy(Player[i].color, "Blue");
            Player[i].startingPosition = BLUE_X;
            Player[i].homeStraightEntery = BLUE_APPROACH;
        }

        Player[i].piecesInBase = 4;
        Player[i].piecesInPath = 0;
        Player[i].piecesInHome = 0;
        
        for (int j = 0; j < NUM_OF_PIECES; j++)
        {
            Player[i].Piece[j].index = j + 1;
            Player[i].Piece[j].location = -2;  
            Player[i].Piece[j].captureCounter = 0;
        }
    }    
}

bool EligibleToHomeStraight(int currentPosition, int expectedLocation, int currentPlayer) 
{
    return (expectedLocation >= -10 && expectedLocation <= -5);
}

void getPiecesInPath(int p, int pathPieces[])
{
    int count = Player[p].piecesInPath;
    int index = 0;

    for (int i = 0; i < NUM_OF_PIECES; i++)
    {
        pathPieces[i] = -1;
    }
    for (int i = 0; i < count; i++) 
    {
        if (Player[p].Piece[i].location != -2 && Player[p].Piece[i].location != -1) 
        {
            pathPieces[index] = i; 
            index++;
        }
    }
}

void positionMaker (int index, int player, int new_postion)
{
    int num = new_postion - Player[player].homeStraightEntery;
    Player[player].Piece[index].location = -11 + num;
}

void updatePosition (int p)
{
    Player[p].piecesInPath = 0;
    Player[p].piecesInHome = 0;
    Player[p].piecesInBase = 4;
    for (int i = 0; i < NUM_OF_PIECES; i++)
    {
        if (Player[p].Piece[i].location != -2 && Player[p].Piece[i].location != -1)
        {
            Player[p].piecesInPath++;
            Player[p].piecesInBase--;
        }

        else if (Player[p].Piece[i].location == -1)
        {
            Player[p].piecesInHome++;
            Player[p].piecesInPath--;
        }
    }    
}

bool isCapture(int currentPlayer, int targetLocation, int* capturedPlayer, int* capturedPiece) 
{
    for (int p = 0; p < NUM_OF_PLAYERS; p++) 
    {
        if (p == currentPlayer) {continue;} // Skip the current player's pieces

        for (int pieceIndex = 0; pieceIndex < NUM_OF_PIECES; pieceIndex++) 
        {
            if (Player[p].Piece[pieceIndex].location == targetLocation) 
            {
                *capturedPlayer = p;
                *capturedPiece = pieceIndex;
                return true;
            }
        }
    }
    return false;
}

void reset(int capturedPlayer, int capturedPiece) 
{
    Player[capturedPlayer].Piece[capturedPiece].location = -2; 
    Player[capturedPlayer].piecesInBase++;
    Player[capturedPlayer].piecesInPath--;
    printf("%s player's piece %d is returned to the base.\n",
           Player[capturedPlayer].color, capturedPiece + 1);
}

void movePiece(int currentPlayer, int roll) 
{
    int pathPieces[NUM_OF_PIECES];
    for (int i = 0; i < NUM_OF_PIECES; i++) 
    {
        pathPieces[i] = -1;
    }
    getPiecesInPath(currentPlayer, pathPieces);
    int can_capture_locations_count = 0;
    int can_capture_locations[NUM_OF_PIECES];
    CaptureInfo captureInfos[NUM_OF_PIECES];

    for (int i = 0; i < NUM_OF_PIECES; i++) 
    {
        if (pathPieces[i] == -1) {continue;}

        int current_position = Player[currentPlayer].Piece[pathPieces[i]].location;
        int expected_location = current_position + roll;
        int capturedPlayer = -1;
        int capturedPiece = -1;

        if (current_position >= 0 && !EligibleToHomeStraight(current_position, expected_location, currentPlayer)) 
        {
            if (isCapture(currentPlayer, expected_location, &capturedPlayer, &capturedPiece)) 
            {
                captureInfos[can_capture_locations_count].capturedPlayer = capturedPlayer;
                captureInfos[can_capture_locations_count].capturedPiece = capturedPiece;
                can_capture_locations[can_capture_locations_count] = pathPieces[i];
                can_capture_locations_count++;
            }
        } 
        else if (current_position >= -10 && current_position <= -5) 
        {
            if (expected_location == -5) 
            {
                Player[currentPlayer].Piece[pathPieces[i]].location = HOME;
                printf("%s player moves piece %d to HOME by %d units.\n", Player[currentPlayer].color, pathPieces[i] + 1, roll);
                updatePosition(currentPlayer);
            } 
            else if (expected_location < -5) 
            {
                Player[currentPlayer].Piece[pathPieces[i]].location = expected_location;
                printf("%s player moves piece %d from home path %d to home path %d by %d units.\n",
                Player[currentPlayer].color, pathPieces[i] + 1, current_position, expected_location, roll);
            } 
            else 
            {
                printf("Ignoring the throw and moving to the next player.\n");
            }
            continue;
        } 
        else 
        {
            Player[currentPlayer].Piece[pathPieces[i]].location = (expected_location % BOARD_SIZE + BOARD_SIZE) % BOARD_SIZE;
            printf("%s player moves piece %d from location %d to %d by %d units.\n",
            Player[currentPlayer].color, pathPieces[i] + 1, current_position, Player[currentPlayer].Piece[pathPieces[i]].location, roll);
        }
    }

    // Handle capturing logic
    if (can_capture_locations_count > 0) 
    {
        for (int i = 0; i < can_capture_locations_count; i++) 
        {
            int pieceIndex = can_capture_locations[i];
            int current_position = Player[currentPlayer].Piece[pieceIndex].location;
            int expected_location = current_position + roll;
            Player[currentPlayer].Piece[pieceIndex].location = (expected_location % BOARD_SIZE + BOARD_SIZE) % BOARD_SIZE;

            reset(captureInfos[i].capturedPlayer, captureInfos[i].capturedPiece);

            printf("%s player moves piece %d to %d, captures color %s piece %d, and returns it to the base.\n",
                   Player[currentPlayer].color, pieceIndex + 1, Player[currentPlayer].Piece[pieceIndex].location,
                   Player[captureInfos[i].capturedPlayer].color, captureInfos[i].capturedPiece + 1);
        }
    }

    // Handle entering the board from base
    if (can_capture_locations_count == 0 && Player[currentPlayer].piecesInBase > 0 && roll == 6) 
    {
        int new_piece = -1;
        for (int i = 0; i < NUM_OF_PIECES; i++) 
        {
            if (Player[currentPlayer].Piece[i].location == -2) 
            {
                new_piece = i;
                break;
            }
        }
        if (new_piece != -1) 
        {
            Player[currentPlayer].Piece[new_piece].location = Player[currentPlayer].startingPosition;
            printf("%s player moves piece %d to the starting point.\n", Player[currentPlayer].color, new_piece + 1);
        }
        updatePosition(currentPlayer);
    }

    // Handle general movement if no capture is possible
    if (can_capture_locations_count == 0 && Player[currentPlayer].piecesInBase == 0) 
    {
        for (int i = 0; i < NUM_OF_PIECES; i++) 
        {
            if (Player[currentPlayer].Piece[i].location != -2 && Player[currentPlayer].Piece[i].location != -1) 
            {
                int current_position = Player[currentPlayer].Piece[i].location;
                int expected_location = current_position + roll;
                Player[currentPlayer].Piece[i].location = (expected_location % BOARD_SIZE + BOARD_SIZE) % BOARD_SIZE;
                printf("%s player moves piece %d from location %d to %d by %d units.\n",
                       Player[currentPlayer].color, i + 1, current_position, Player[currentPlayer].Piece[i].location, roll);
                break;
            }
        }
    }
}

void playGame(int firstPlayer) {
    int currentPlayer = firstPlayer;
    int roundcounter = 0;

    while (1) 
    {
        roundcounter++;
        int six_count = 0;
        
        for (int i = 0; i < 3; i++) 
        {
            int roll = diceRoll();
            printf("%s player rolled %i\n", Player[currentPlayer].color, roll);

            if (roll == 6 && six_count < 3) {
                if (Player[currentPlayer].piecesInBase == NUM_OF_PIECES) {
                    Player[currentPlayer].Piece[0].location = Player[currentPlayer].startingPosition;
                    printf("%s player moves piece 1 to the starting point\n", Player[currentPlayer].color);
                    updatePosition(currentPlayer);
                    printf("%s player now has %d pieces on the board and %d pieces on the base\n",
                           Player[currentPlayer].color, Player[currentPlayer].piecesInPath, Player[currentPlayer].piecesInBase);
                    
                } else {
                    movePiece(currentPlayer, roll);
                }

                six_count++;
            } 
            else if (roll == 6 && six_count == 3) 
            {
                break; // Go to the next player
            } 
            else if (roll < 6) 
            {
                updatePosition(currentPlayer);
                if (Player[currentPlayer].piecesInBase == NUM_OF_PIECES) { break;}
                    
                else 
                {
                    movePiece(currentPlayer, roll);
                    break;
                }
            }
        }

        if (roundcounter == 300) 
        {
            for (int i = 0; i < NUM_OF_PIECES; i++) 
            {
                Player[currentPlayer].Piece[i].location = HOME;
            }
            updatePosition(currentPlayer);
            printf("%s player wins!\n", Player[currentPlayer].color);
            break; // Exit the game loop
        }

        
        else {currentPlayer = nextplayer(currentPlayer);}
        
    }
}

                          

int main()
{
    printf("The Red player has four (04) pieces named R1, R2, R3, R4\n");
    printf("The Green player has four (04) pieces named G1, G2, G3, G4\n");
    printf("The Yellow player has four (04) pieces named Y1, Y2, Y3, Y4\n");
    printf("The Blue player has four (04) piece named B1, B2, B3, B4\n");

    InitializePlayer();
    //to find the values of the first round before starting the game
    int max = 0;
    int maxPlayer = 0;
    srand(time(NULL));
    
    for (int i = 0; i < NUM_OF_PLAYERS; i++)
    {
        int num = diceRoll();
        printf("%s rolls %d\n", Player[i].color, num);

        if (max < num)
        {
            max = num;
            maxPlayer = i;
        }
    }
    printf("%s player has the highest roll and will begin the game.\n", Player[maxPlayer].color);

    //to print the order of the first round
    printf("The order of a single round is ");
    printf("%s", Player[maxPlayer].color);

    for (int i = 0; i < NUM_OF_PLAYERS - 1; i++)
    {
        if (i < 2) {printf(", ");}
        if (i == 2) {printf(" and ");}
        printf("%s",Player[nextplayer(maxPlayer + i)].color);
    }
    printf("\n");

    playGame(maxPlayer);
    return 0;
}    

  
