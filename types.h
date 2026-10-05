#define NUM_OF_PLAYERS 4
#define NUM_OF_PIECES 4
#define YELLOW_APPROACH 0
#define YELLOW_X 2
#define BLUE_APPROACH 13
#define BLUE_X 15
#define RED_APPROACH 26
#define RED_X 28
#define GREEN_APPROACH 39
#define GREEN_X 41
#define BASE -2
#define HOME -1
#define CLOCKWISE -3
#define COUNTER_CLOCKWISE -4
#define MOVE_WHEN_BLOCK -5
#define BOARD_SIZE 52

typedef struct 
{
    short location;
    short index;
    short captureCounter;
}piece;

typedef struct 
{
    char color[10];
    piece Piece[NUM_OF_PIECES];
    short piecesInPath;
    short piecesInBase;
    short piecesInHome;
    short startingPosition;
    short homeStraightEntery;
}player;

typedef struct {
    int capturedPlayer;
    int capturedPiece;
} CaptureInfo;


