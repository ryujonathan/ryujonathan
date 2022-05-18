#include <stdio.h>
#include <stdbool.h>

void printBoard(char board[][26], int n) {
    printf("  ");
  	for(int i = 0; i < n; i++) {
		printf("%c", (char)(97 + i));
 	}
	printf("\n");
	for(int i = 0; i < n; i++) {
		printf("%c ", (char)(97 + i));
		for(int j = 0; j < n; j++) {
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
}

bool positionInBounds(int n, int row, int col) {
    return ((row >= 0) && (col >= 0) && (row < n) && (col < n));
}

bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol){
	int currRow = row + deltaRow, currCol = col + deltaCol, count = 0;
	char opponent;
	if(colour == 'W') {
		opponent = 'B';
	} else if(colour == 'B') {
		opponent = 'W';
	} else {
		return false;
	}
	if(positionInBounds(n, row, col)) {
		if(board[row][col] == '-') {
			for(currRow, currCol; positionInBounds(n, currRow, currCol); currRow += deltaRow, currCol += deltaCol) {
                if(board[currRow][currCol] == '-') return false;
                else if(board[currRow][currCol] == colour && count > 0) return true;
                else if(board[currRow][currCol] == colour) return false;
                else count++;
            }
		} else return false;
	} else return false;
	return false;			
}

bool legal(char board[][26], int n, int row, int col, char colour) {
    return checkLegalInDirection(board, n, row, col, colour, 1, 0) || checkLegalInDirection(board, n, row, col, colour, 1, 1) || checkLegalInDirection(board, n, row, col, colour, -1, 0) || checkLegalInDirection(board, n, row, col, colour, -1, -1) || checkLegalInDirection(board, n, row, col, colour, 0, 1) || checkLegalInDirection(board, n, row, col, colour, 0, -1) || checkLegalInDirection(board, n, row, col, colour, 1, -1) || checkLegalInDirection(board, n, row, col, colour, -1, 1);
}

void makeMove(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol){
	char opponent;
	if(colour == 'W') {
		opponent = 'B';
	} else if(colour == 'B') {
		opponent = 'W';
	}
    if(checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)) {
		int currRow = row + deltaRow, currCol = col + deltaCol;
		while(positionInBounds(n, currRow, currCol)) {
			if(board[currRow][currCol] == opponent) {
				board[currRow][currCol] = colour;
				currRow += deltaRow;
				currCol += deltaCol;
			} else {
				break;
			}
		}
	}
}

void moveDo(char board[][26], int n, int row, int col, char color) {
    if(board[row][col] == '-') {
		makeMove(board, n, row, col, color, 1, 0);
		board[row][col] = '-';
		makeMove(board, n, row, col, color, 1, 1);
		board[row][col] = '-';
		makeMove(board, n, row, col, color, 1, -1);
		board[row][col] = '-';
		makeMove(board, n, row, col, color, -1, 0);
		board[row][col] = '-';
		makeMove(board, n, row, col, color, -1, 1);
		board[row][col] = '-';
		makeMove(board, n, row, col, color, -1,-1);
		board[row][col] = '-';
		makeMove(board, n, row, col, color, 0, 1);
		board[row][col] = '-';
		makeMove(board, n, row, col, color, 0, -1);
		board[row][col] = color;
	}
}

int score(char board[][26], int n, int row, int col, char color, int deltaRow, int deltaCol){
    int count = 0;
    if(checkLegalInDirection(board, n, row, col, color, deltaRow, deltaCol)){
    	do{
            count++;
			row += deltaRow; col += deltaCol;
		} while(board[row][col] != color);
	}
    return count;
}

int totalBaseScore(char board[][26], int n, int row, int col, char color) {
    return score(board, n, row, col, color, 1, 0) + score(board, n, row, col, color, 1, 1) + score(board, n, row, col, color, 1, -1) + score(board, n, row, col, color, 0, 1) + score(board, n, row, col, color, 0, -1) + score(board, n, row, col, color, -1, 0) + score(board, n, row, col, color, -1, 1) + score(board, n, row, col, color, -1, -1);
}

int totalScore(char board[][26], int n, int row, int col, char color) {
	char opponent;
	int score = -2147483648;
	if(color == 'B') opponent = 'W';
	else if(color == 'W') opponent = 'B';
	if(legal(board, n, row, col, color)) {
		score = totalBaseScore(board, n, row, col, color) - totalBaseScore(board, n, row, col, opponent);
		if((row == 0 && col == 0) || (row == 0 && col == n - 1) || (row == n - 1 && col == 0) || (row == n - 1 && col == n - 1)) score += 600;
		else if((row == 1 && col == 1) || (row == 1 && col == 0) || (row == 0 && col == 1) || (row == 1 && col == n - 1) || (row == 1 && col == n - 2) || (row == 0 && col == n - 2) || (row == n - 1 && col == n - 2) || (row == n - 2 && col == n - 2) || (row == n - 2 && col == n - 1) || (row == n - 1 && col == 1) || (row == n - 2 && col == 1) || (row == n - 2 && col == 0))  score -= 700;
	}
	return score;
}

bool noValidMove(char board[][26], int n, char color) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(legal(board, n, i, j, color)) {
                return false;
            }
        }
    }
    return true;
}

bool humanMove(char board[][26], int n, char color, int* inv, char comp, int *cinv) {
    if(!noValidMove(board, n, color)) {
        if(*cinv == 0) {
            printf("%c player has no valid move.\n", comp);
            *cinv = 1;
        }
        char move[3];
        printf("Enter move for colour %c (RowCol): ", color);
        scanf("%s", move);
        if(legal(board, n, (int) (move[0] - 97), (int) (move[1] - 97), color)) {
            moveDo(board, n, (int) (move[0] - 97), (int) (move[1] - 97), color);
        } else {
            printf("Invalid move.\n");
            if(color == 'W') {
                printf("B player wins.\n");
            } else {
                printf("W player wins.\n");
            }
            return false;
        }
        return true;
    } else {
        *inv = 0;
        return true;
    }
}

void computerMove(char board[][26], int n, char color, int* inv) {
    int greatest = 0, greatestRow = 0, greatestCol = 0, scored;
    if(!noValidMove(board, n, color)) {
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                scored = totalScore(board, n, i, j, color);
                if(scored > greatest) {
                    greatest = scored;
                    greatestRow = i;
                    greatestCol = j;
                }
            }
        }
        moveDo(board, n, greatestRow, greatestCol, color);
        printf("Computer places %c at %c%c.\n", color, (char) (greatestRow + 97), (char) (greatestCol + 97));
    } else {
        *inv = 0;
    }
}

int countB(char board[][26], int n) {
    int count = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(board[i][j] == 'B') {
                count++;
            }
        }
    }
    return count;
}

int countW(char board[][26], int n) {
    int count = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(board[i][j] == 'W') {
                count++;
            }
        }
    }
    return count;
}

int main(void){
    int size, hInvalid = 1, cInvalid = 1;
    char computer, human, board[26][26], move[3];
    printf("Enter the board dimension: ");
    scanf("%d", &size);
    printf("Computer plays (B/W): ");
    scanf(" %c", &computer);
    if(computer == 'B' || computer == 'b') {
        computer = 'B';
        human = 'W';
    } else if (computer == 'W' || computer == 'w') {
        computer = 'W';
        human = 'B';
    }
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            board[i][j] = '-';
        }
    }
    board[size / 2][size / 2 - 1] = 'B';
    board[size / 2 - 1][size / 2] = 'B';
    board[size / 2 - 1][size / 2 - 1] = 'W';
    board[size / 2][size / 2] = 'W';
    printBoard(board, size);
    if(computer == 'B') {
        computerMove(board, size, computer, &cInvalid);
        printBoard(board, size);
    }
    while(true) {
        if(humanMove(board, size, human, &hInvalid, computer, &cInvalid)) {
            if((hInvalid == 0 && noValidMove(board, size, computer)) || (cInvalid == 0 && hInvalid == 0)) {
                int bCount = countB(board, size);
                int wCount = countW(board, size);
                if(bCount > wCount) {
                    printf("B player wins.\n");
                    break;
                } else if (bCount == wCount) {
                    printf("Draw!\n");
                    break;
                } else {
                    printf("W player wins.\n");
                    break;
                }
            } else if (hInvalid == 0) {
                printf("%c player has no valid move.\n", human);
                hInvalid = 1;
            } else if (cInvalid == 0) {
                printf("%c player has no valid move.\n", computer);
            } else {
                printBoard(board, size);
                computerMove(board, size, computer, &cInvalid);
                if(cInvalid == 1) printBoard(board, size);
            }
        } else {
            break;
        }
    }
    return 0;
}