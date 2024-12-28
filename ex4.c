#include <stdio.h>
#include <string.h>

#define WEIGHTS_ARRAY_SIZE 5
#define MAXIMAL_GRID_SIZE 20
#define MAXIMAL_GRID_SIZE_CROSSWORD 30
#define MAX_SLOTS 100
#define MAX_NUM_OF_WORDS 100
#define MAX_LENGTH_OF_WORD 15

typedef struct {
    int x;
    int y;
    char area;
} Point;

typedef struct {
    int row;
    int column;
    int length;
    char direction;
    char word[MAX_LENGTH_OF_WORD];
} Slot;

int task1RobotPaths(int column, int row);
int getWeightsInput(float weights[][WEIGHTS_ARRAY_SIZE], int size);
float task2HumanPyramid(float weights[][WEIGHTS_ARRAY_SIZE], int row, int column);
void getRidOfAllRemainingChars();
int task3ParenthesisValidators(char expectedClosing, int sumOfOpenParenthesis);
int task4QueensBattle(Point queensPoints[MAXIMAL_GRID_SIZE],
    char puzzle[MAXIMAL_GRID_SIZE][MAXIMAL_GRID_SIZE], int boardDimension, int column);
int placeQueen(Point queensPoints[MAXIMAL_GRID_SIZE],
    char puzzle[MAXIMAL_GRID_SIZE][MAXIMAL_GRID_SIZE], int boardDimension, int row, int col);
int ifCanPlaceWord(int row, int column, int length, char direction,
    char word[MAX_LENGTH_OF_WORD],
    char grid[MAXIMAL_GRID_SIZE_CROSSWORD][MAXIMAL_GRID_SIZE_CROSSWORD], int currentChar);

void inputWord(int row, int column, int length, char direction,
    char word[MAX_LENGTH_OF_WORD],
    char grid[MAXIMAL_GRID_SIZE_CROSSWORD][MAXIMAL_GRID_SIZE_CROSSWORD], int currentChar);

int task5CrosswordGenerator(Slot slotDetails[], int numOfWords, int slots,
    char words[][MAX_LENGTH_OF_WORD], int currentSlot,
    char grid[MAXIMAL_GRID_SIZE_CROSSWORD][MAXIMAL_GRID_SIZE_CROSSWORD],
    int wordsUsed[MAX_NUM_OF_WORDS]);

int checkWord(Slot slotDetails[], int numOfWords, int slots,
    char words[][MAX_LENGTH_OF_WORD], int currentSlot,
    int currentWord, char grid[MAXIMAL_GRID_SIZE_CROSSWORD][MAXIMAL_GRID_SIZE_CROSSWORD],
    int wordsUsed[MAX_NUM_OF_WORDS]);
int main() {
    int task = -1;
    do {
        printf("Choose an option:\n"
               "1. Robot Paths\n"
               "2. The Human Pyramid\n"
               "3. Parenthesis Validation\n"
               "4. Queens Battle\n"
               "5. Crossword Generator\n"
               "6. Exit\n");

        if (scanf(" %d", &task)) {
            switch (task) {
                case 6:
                    printf("Goodbye!\n");
                    break;
                case 1: {
                    printf("Please enter the coordinates of the robot (column, row):\n");
                    int column, row;
                    scanf(" %d %d", &column, &row);
                    if (column < 0 || row < 0) {
                        printf("The total number of paths the robot can take to reach home is: 0\n");
                        break;
                    }
                    int totalPaths = task1RobotPaths(column, row);
                    printf("The total number of paths the robot can take to reach home is: %d\n", totalPaths);
                    break;
                }
                case 2: {
                    float weights[WEIGHTS_ARRAY_SIZE][WEIGHTS_ARRAY_SIZE] = {0};
                    printf("Please enter the weights of the cheerleaders:\n");
                    int isValid = getWeightsInput(weights, WEIGHTS_ARRAY_SIZE);
                    if (isValid == 1)
                        break;
                    int x = 1;
                    printf("The total weight on each cheerleader is:\n");
                    for (int i = 0; i < 5; ++i) {
                        for (int j = 0; j < x; ++j) {
                            printf("%.2f ", task2HumanPyramid(weights, i, j));
                        }
                        printf("\n");
                        x++;
                    }
                    break;
                }
                case 3: {
                    printf("Please enter a term for validation:\n");
                    //clean buffer so the recursion works
                   // scanf("%*[^\n]"); //remove all characters until \n
                    //scanf("%*c"); //remove also \n
                    //first input will deal with the case in which the user has a \n before entering the input
                    char firstInput;
                    scanf("%c", &firstInput);
                    int isBalanced;
                    //if first char is a closing par then not balanced
                    if(firstInput == '}' || firstInput == ']' || firstInput == '>' || firstInput == ')')
                    {
                        printf("The parentheses are not balanced correctly.\n");
                        break;
                    }
                    //if were starting with a open par then get the closing par and call the recursion with it
                    if(firstInput == '{' || firstInput == '[' || firstInput == '<' || firstInput == '(')
                    {
                        char closing;
                        if (firstInput == '{')
                            closing = '}';
                        else if (firstInput == '[')
                            closing = ']';
                        else if (firstInput == '(')
                            closing = ')';
                        else
                            closing = '>';
                        isBalanced = task3ParenthesisValidators(closing, 1);
                    }
                        //else start the recurssion regularly
                    else
                        isBalanced = task3ParenthesisValidators('0', 0);
                    if (isBalanced == 1)
                        printf("The parentheses are balanced correctly.\n");
                    else
                    { //read all charachters before printing
                        printf("The parentheses are not balanced correctly.\n");
                    }

                    break;
                }
                case 4: {
                    int boardDimension = 0;
                    printf("Please enter the board dimensions:\n");
                    scanf("%d", &boardDimension);
                    char puzzle[MAXIMAL_GRID_SIZE][MAXIMAL_GRID_SIZE]; //puzzle map
                    Point queenPoints[MAXIMAL_GRID_SIZE];//this will retain all the queen positions and area
                    for (int i = 0; i < boardDimension; ++i) {
                        queenPoints[i].area = '\0';
                        queenPoints[i].x = -1;
                        queenPoints[i].y = -1;
                    }
                    printf("Please enter a %d*%d puzzle board:\n", boardDimension, boardDimension);
                    for (int rowPos = 0; rowPos < boardDimension; ++rowPos) {
                        for (int columnPos = 0; columnPos < boardDimension; ++columnPos) {
                            char input = 0;
                            scanf(" %c", &input);
                            puzzle[rowPos][columnPos] = input;
                        }
                    }
                    int result = task4QueensBattle(queenPoints, puzzle, boardDimension, 0);
                    if (result == 0) {
                        printf("This puzzle cannot be solved.\n");
                    } else {
                        printf("Solution:\n");
                        //if valid then print the board
                        for (int row = 0; row < boardDimension; ++row) {
                            for (int column = 0; column < boardDimension; ++column) {
                                int isQueenPoint = 0;
                                for (int queens = 0; queens < boardDimension; ++queens) {
                                    if (queenPoints[queens].x == row && queenPoints[queens].y == column) {
                                        isQueenPoint = 1;
                                        break;
                                    }
                                }
                                if (isQueenPoint == 1)
                                    printf("X ");
                                else
                                    printf("* ");
                            }
                            printf("\n");
                        }
                    }
                    break;
                }
                case 5: {
                    int gridSize;
                    int slots;
                    char crossword[MAXIMAL_GRID_SIZE_CROSSWORD][MAXIMAL_GRID_SIZE_CROSSWORD];
                    Slot slotDetails[MAX_SLOTS];
                    char words[MAX_NUM_OF_WORDS][MAX_LENGTH_OF_WORD];
                    int numOfWords;
                    int wordsUsed [MAX_NUM_OF_WORDS];
                        //make all words equal to 0
                    for (int i = 0; i < numOfWords; ++i)
                    {
                        wordsUsed[i] = 0;
                    }
                    printf("Please enter the dimensions of the crossword grid:\n");
                    scanf("%d", &gridSize);
                    printf("Please enter the number of slots in the crossword:\n");
                    scanf("%d", &slots);
                    printf("Please enter the details for each slot (Row, Column, Length, Direction):\n");
                    for (int i = 0; i < slots; ++i) {
                        scanf(" %d %d %d %c", &slotDetails[i].row, &slotDetails[i].column,
                            &slotDetails[i].length, &slotDetails[i].direction);
                    }
                    printf("Please enter the number of words in the dictionary:\n");
                    do {
                        scanf("%d", &numOfWords);
                        if (numOfWords >= slots)
                            break;
                        printf("The dictionary must contain at least %d words. Please enter a valid dictionary "
                               "size:\n", slots);
                    } while (1);
                    printf("Please enter the words for the dictionary:\n");
                    for (int i = 0; i < numOfWords; i++) {
                        scanf("%s", words[i]);
                    }

                    // Initialize the crossword grid with empty spaces
                    for (int i = 0; i < MAXIMAL_GRID_SIZE_CROSSWORD; i++) {
                        for (int j = 0; j < MAXIMAL_GRID_SIZE_CROSSWORD; j++) {
                            crossword[i][j] = ' ';
                        }
                    }

                    int result = task5CrosswordGenerator(slotDetails, numOfWords, slots, words,
                        0, crossword, wordsUsed);
                    if (result) {
                        for (int i = 0; i < gridSize; i++) {
                            printf("| ");
                            for (int j = 0; j < gridSize; j++) {
                                if (crossword[i][j] == ' ') {
                                    printf("# | ");
                                } else {
                                    printf("%c | ", crossword[i][j]);
                                }
                            }
                            printf("\n");
                        }
                    } else {
                        printf("This crossword cannot be solved.\n");
                    }
                    break;
                }
                default:
                    printf("Please choose a task number from the list.\n");
                    break;
            }
        } else {
            scanf("%*s");
        }
    } while (task != 6);
}

//function for exercise one, it will give the sum of ways to get from a x,y point to 0,0
int task1RobotPaths(int column, int row) {
    int closerColumn = 0, closerRow = 0;
    if (column == 0 && row == 0) //if we are in (0,0) it means we found a way, therefore return that we found one more
        return 1;
    if (column != 0) //check if we can still go more to the left
        closerColumn = task1RobotPaths(column - 1, row);
    if (row != 0)//check if we can go more down
        closerRow = task1RobotPaths(column, row - 1);
    return closerColumn + closerRow;
}

//function that will get input for the array
//it will return 0 if implementation was successful and 1 if no
int getWeightsInput(float weights[][WEIGHTS_ARRAY_SIZE], int size) {
    int weightsRequired = 1;// this will count how much inputed weights we need in every row
    for (int row = 0; row < size; ++row) {
        for (int weight = 0; weight < weightsRequired; ++weight) {
            float getWeight;
            scanf("%f", &getWeight);
            //if a weight is negative stop the function and the case
            if (getWeight < 0) {
                printf("Negative weights are not supported.\n");
                return 1;
            }
            weights[row][weight] = getWeight;
        }
        weightsRequired++;
    }
    return 0;
}

//function that will do q.2 which is to check the weight on a specific person who people are above
float task2HumanPyramid(float weights[][WEIGHTS_ARRAY_SIZE], int row, int column) {
    //if we are on the highest tower, return the highest person's weight
    if (row == 0 && column == 0)
        return weights[row][column];
    //if row and column arent 0, that means that if row == 0 and column !=0 we have no person on that position
    //therefore return 0
    if (row == 0 || column > row)
        return 0;
    float leftSum;
    if (column == 0) {
        leftSum = task2HumanPyramid(weights, row - 1, column);
        return weights[row][column] + leftSum / 2;
    }
    leftSum = task2HumanPyramid(weights, row - 1, column);
    float rightSum = task2HumanPyramid(weights, row - 1, column - 1);

    return leftSum / 2 + rightSum / 2 + weights[row][column];
}
void getRidOfAllRemainingChars()
{
    char input;
    scanf("%c", &input);
    if(input == '\n')
        return;
    getRidOfAllRemainingChars();
}
//function that will check whether parenthesis is correctly placed in a given word
int task3ParenthesisValidators(char expectedClosing, int sumOfOpenParenthesis) {
    char input;
    scanf("%c", &input);

    if (input == '\n') {
        //if no more open parenthesises then return 1 which is true
        if (sumOfOpenParenthesis == 0)
            return 1;
        return 0;
    }

    if (input == '{' || input == '[' || input == '(' || input == '<') {
        //closing will be the closing par of his opening one
        char closing;
        if (input == '{')
            closing = '}';
        else if (input == '[')
            closing = ']';
        else if (input == '(')
            closing = ')';
        else
            closing = '>';

        //call the recursion and check if we find a closing par
        if (task3ParenthesisValidators(closing, sumOfOpenParenthesis + 1) == 0)
        {
            return 0;
        }
        //if we found an closing par then just rerun the recursion
        return task3ParenthesisValidators(expectedClosing, sumOfOpenParenthesis);
    }
    if (input == '}' || input == ']' || input == ')' || input == '>') {
        //if sum is zero when we have a closing par then return false
        //if the closing par is not the one we expected return false
        if (sumOfOpenParenthesis <= 0 || input != expectedClosing)
        {
            //if it fails we need to check if there are still inputs to be EATEN therfore call a function which
            //will consume them all
            getRidOfAllRemainingChars();
            return 0;
        }
        return 1;
    }
    //if no { [ < ( } ] > ) found then just rerun the recursion
    return task3ParenthesisValidators(expectedClosing, sumOfOpenParenthesis);
}
//function that will check if we can place a queen in a specific area
int checkPoint(Point queensPoints[MAXIMAL_GRID_SIZE], char puzzle[MAXIMAL_GRID_SIZE][MAXIMAL_GRID_SIZE], int x,
    int y, int currentPoint) {
    if (currentPoint < 0)
        return 1;
    //if its -1 then that means no queen is there, so recall function
    if (queensPoints[currentPoint].x == -1)
        return checkPoint(queensPoints, puzzle, x, y, currentPoint - 1);

    // check if same row/col
    if (x == queensPoints[currentPoint].x || y == queensPoints[currentPoint].y)
        return 0;

      // check if this area already exists in one of the queens
    if (puzzle[x][y] == queensPoints[currentPoint].area)
        return 0;

    // ccheck if we have a queen that is diagonal close to the x,y
    int xDiagonal = x - queensPoints[currentPoint].x;
    int yDiagonal = y - queensPoints[currentPoint].y;
    if ((xDiagonal == 1 && yDiagonal == 1) || (xDiagonal == 1 && yDiagonal == -1)
        || (xDiagonal == -1 && yDiagonal == 1) || (xDiagonal == -1 && yDiagonal == -1))
        return 0;

    // keep recusrion untill no more queens
    return checkPoint(queensPoints, puzzle, x, y, currentPoint - 1);
}

int task4QueensBattle(Point queensPoints[MAXIMAL_GRID_SIZE], char puzzle[MAXIMAL_GRID_SIZE][MAXIMAL_GRID_SIZE],
    int boardDimension, int col) {
    //all columns have a queen therfore turn 1(true)
    if (col == boardDimension) {
        return 1;
    }
    return placeQueen(queensPoints, puzzle, boardDimension, 0, col);
}
//this function will place a queen in every row
int placeQueen(Point queensPoints[MAXIMAL_GRID_SIZE], char puzzle[MAXIMAL_GRID_SIZE][MAXIMAL_GRID_SIZE],
    int boardDimension, int row, int col) {

    if (row == boardDimension) {
        return 0;
    }
    //we currently have col-1 queens, so check col-1 amount of queens
    if (checkPoint(queensPoints, puzzle, row, col, col - 1)) {
        //if x, y are valid make(no queens near) that point a queen
        queensPoints[col].x = row;
        queensPoints[col].y = col;
        queensPoints[col].area = puzzle[row][col];
        //now that we found another queen, we will find a queen in the next column
        //recall the recursion and now check if in every column we can have a queen
        if (task4QueensBattle(queensPoints, puzzle, boardDimension, col + 1) == 1) {
            return 1;
        }
        //in one column we cannot find a queen, therfore we need to reset the queen since she doesnt
        //solve the puzzle
        queensPoints[col].x = -1;
        queensPoints[col].y = -1;
        queensPoints[col].area = '\0';
    }
    //try finding a solution with the next row
    return placeQueen(queensPoints, puzzle, boardDimension, row + 1, col);
}

// check if a word can be in a specific location
int ifCanPlaceWord(int row, int column, int length, char direction, char word[MAX_LENGTH_OF_WORD],
    char grid[MAXIMAL_GRID_SIZE_CROSSWORD][MAXIMAL_GRID_SIZE_CROSSWORD], int currentChar) {
    if (length == 0) {
        return 1;
    }
    if (direction == 'H') {
        //if we found a charachter that if inside our desired word, but doesnt have the same charachter, return false
        if (grid[row][column] != ' ' && grid[row][column] != word[currentChar])
            return 0;
        //keep checking for the rest of the charachters
        return ifCanPlaceWord(row, column + 1, length - 1, direction, word, grid, currentChar+1);
    } else {
        //if we found a charachter that if inside our desired word, but doesnt have the same charachter, return false
        if (grid[row][column] != ' ' && grid[row][column] != word[currentChar])
            return 0;
        //keep checking for the rest of the charachters
        return ifCanPlaceWord(row + 1, column, length - 1, direction, word, grid, currentChar+1);
    }
}

// a function that will place a word in my grid
void inputWord(int row, int column, int length, char direction, char word[MAX_LENGTH_OF_WORD],
    char grid[MAXIMAL_GRID_SIZE_CROSSWORD][MAXIMAL_GRID_SIZE_CROSSWORD], int currentChar) {
    //place length times charachters, then stop
    if (length == 0)
        return;
    //make the grid equal to the location of the char
    grid[row][column] = word[currentChar];
    //if its H then we go over colums, if its rows go over rows
    if (direction == 'H') {
        inputWord(row, column + 1, length - 1, direction, word , grid, currentChar+1);
    } else {
        //if its V we need to go over the rows
        inputWord(row + 1, column, length - 1, direction, word, grid, currentChar+1);
    }
}

// function that will remove for me a word in the grid
void destroyWord(int row, int column, int length, char direction,
    char grid[MAXIMAL_GRID_SIZE_CROSSWORD][MAXIMAL_GRID_SIZE_CROSSWORD]) {
    //go length times to remove all charachters // x
    if (length == 0)
        return;             //hello
    //reset the position // x x x x x
                         // x x x x x
                         // x x x x x
    grid[row][column] = ' ';
    if (direction == 'H') {
        destroyWord(row, column + 1, length - 1, direction, grid);
    } else {
        destroyWord(row + 1, column, length - 1, direction, grid);
    }
}


int task5CrosswordGenerator(Slot slotDetails[], int numOfWords, int slots, char words[][MAX_LENGTH_OF_WORD],
    int currentSlot, char grid[MAXIMAL_GRID_SIZE_CROSSWORD][MAXIMAL_GRID_SIZE_CROSSWORD],
    int wordsUsed [MAX_NUM_OF_WORDS]) {
    //if we
    if (currentSlot == slots) {
        return 1;
    }
    return checkWord(slotDetails, numOfWords, slots, words, currentSlot, 0, grid, wordsUsed);
}

// function that will place every word(try) in the grid
int checkWord(Slot slotDetails[], int numOfWords, int slots, char words[][MAX_LENGTH_OF_WORD], int currentSlot,
    int currentWord, char grid[MAXIMAL_GRID_SIZE_CROSSWORD][MAXIMAL_GRID_SIZE_CROSSWORD],
    int wordsUsed [MAX_NUM_OF_WORDS]) {
    if (currentWord == numOfWords) {
        return 0;
    }
    //check if we can place the word in the slot
    if (wordsUsed[currentWord] != 1 &&(int) strlen(words[currentWord]) == (int) slotDetails[currentSlot].length &&
        ifCanPlaceWord(slotDetails[currentSlot].row,
        slotDetails[currentSlot].column, slotDetails[currentSlot].length, slotDetails[currentSlot].direction,
        words[currentWord], grid, 0)) {
        //if we can place it then of course place it.
        inputWord(slotDetails[currentSlot].row, slotDetails[currentSlot].column, slotDetails[currentSlot].length,
            slotDetails[currentSlot].direction, words[currentWord], grid, 0);
        wordsUsed[currentWord] = 1;
        //continue to every other word
        if (task5CrosswordGenerator(slotDetails, numOfWords, slots, words, currentSlot + 1, grid, wordsUsed)) {
            return 1;
        }
        destroyWord(slotDetails[currentSlot].row, slotDetails[currentSlot].column, slotDetails[currentSlot].length,
            slotDetails[currentSlot].direction, grid); //we didnt find slot for each word, so remove the word and retry
        wordsUsed[currentWord] = 0; //we deleted this word therfore it should be available again
    }
    //if it didnt work keep trying with the other word
    return checkWord(slotDetails, numOfWords, slots, words, currentSlot, currentWord + 1, grid, wordsUsed);
}