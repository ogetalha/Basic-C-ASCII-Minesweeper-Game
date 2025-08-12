#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

/// Constants
#define MAX_BOARD_SIZE 16
#define MINE_VALUE -1
#define FLAGGED_CELL 'F'

/// State variables for the game
int i, j; /// Index counters for loops 
int board_Size, num_Mines; /// Board size and number of mines (determined by difficulty level)
int game_Board[MAX_BOARD_SIZE][MAX_BOARD_SIZE]; /// Main game board
int visible_Cells[MAX_BOARD_SIZE][MAX_BOARD_SIZE]; /// Visible Cells
int cursor_X = 0, cursor_Y = 0; /// Current cursor position (game uses keystrokes directly from the user to move between cells instead of using coordinates)
int remaining_Cells; /// Remaining cells to reveal

/// Prototypes
void display_Intro(); 
void choose_Difficulty_Level(); 
void initialize_Board();
void reveal_Adjacent_Cells(int x, int y); 
void draw_Board();
void move_Cursor(char direction);
void flag_Cell();
	

/// Displaying the game introduction and prompt for difficulty level
void display_Intro() {
	
    puts("@@@@@@@@@@   @@@  @@@  @@@  @@@@@@@@   @@@@@@   @@@  @@@  @@@  @@@@@@@@  @@@@@@@@  @@@@@@@   @@@@@@@@  @@@@@@@\n"
         " @@@@@@@@@@@  @@@  @@@@ @@@  @@@@@@@@  @@@@@@@   @@@  @@@  @@@  @@@@@@@@  @@@@@@@@  @@@@@@@@  @@@@@@@@  @@@@@@@@\n"
         " @@! @@! @@!  @@!  @@!@!@@@  @@!       !@@       @@!  @@!  @@!  @@!       @@!       @@!  @@@  @@!       @@!  @@@\n"
         " !@! !@! !@!  !@!  !@!!@!@!  !@!       !@!       !@!  !@!  !@!  !@!       !@!       !@!  @!@  !@!       !@!  @!@\n"
         " @!! !!@ @!@  !!@  @!@ !!@!  @!!!:!    !!@@!!    @!!  !!@  @!@  @!!!:!    @!!!:!    @!@@!@!   @!!!:!    @!@!!@!\n"
         " !@!   ! !@!  !!!  !@!  !!!  !!!!!:     !!@!!!   !@!  !!!  !@!  !!!!!:    !!!!!:    !!@!!!    !!!!!:    !!@!@!\n"
         " !!:     !!:  !!:  !!:  !!!  !!:            !:!  !!:  !!:  !!:  !!:       !!:       !!:       !!:       !!: :!!\n"
         " :!:     :!:  :!:  :!:  !:!  :!:           !:!   :!:  :!:  :!:  :!:       :!:       :!:       :!:       :!:  !:!\n"
         " :::     ::    ::   ::   ::   :: ::::  :::: ::    :::: :: :::    :: ::::   :: ::::   ::        :: ::::  ::   :::\n"
         " :      :    :    ::    :   : :: ::   :: : :      :: :  : :    : :: ::   : :: ::    :        : :: ::    :   : :\n\n\n"
          "                                         Welcome to Minesweeper!\n");
		                                         
    choose_Difficulty_Level();
}

/// Prompting the user to choose the difficulty level and set the game parameters
void choose_Difficulty_Level() {
	
    char choice;
    
    puts("                                 Please choose your desired difficulty.\n\n"   
	     "                                                 1. Easy\n"
	     "                                                2. Medium\n"                           
		 "                                                 3. Hard\n");
		 
    while (1) {
        choice = getch();
        switch (choice) {
            case '1':
                board_Size = 8;
                num_Mines = 10;
                return;
            case '2':
                board_Size = 12;
                num_Mines = 30;
                return;
            case '3':
                board_Size = 16;
                num_Mines = 45;
                return;
            default:
                printf("\nInvalid choice. Please press 1 for Easy Mode, 2 for Medium and 3 Hard Mode.");
                break;
        }
    }
}

 /// Initializing the game board with mines and number of adjacent mines
void initialize_Board() {
	
    int k, x, y;

    /// Resetting the board and visible Cells
    for (i = 0; i < board_Size; i++) {
        for (j = 0; j < board_Size; j++) {
            game_Board[i][j] = 0;
            visible_Cells[i][j] = 0;
        }
    }

    /// Planting mines randomly on the board
    srand(time(NULL));
    for (k = 0; k < num_Mines; k++) {
        do {
            x = rand() % board_Size;
            y = rand() % board_Size;
        } while (game_Board[x][y] == MINE_VALUE);
        game_Board[x][y] = MINE_VALUE;
    }

    /// Calculating the number of adjacent mines for each cell
    for (i = 0; i < board_Size; i++) {
        for (j = 0; j < board_Size; j++) {
            if (game_Board[i][j] != MINE_VALUE) {
                int adjacent_Mines = 0;
                for (x = i - 1; x <= i + 1; x++) {
                    for (y = j - 1; y <= j + 1; y++) {
                        if (x >= 0 && x < board_Size && y >= 0 && y < board_Size && game_Board[x][y] == MINE_VALUE) {
                            adjacent_Mines++;
                        }
                    }
                }
                game_Board[i][j] = adjacent_Mines;
            }
        }
    }
}

/// Revealing adjacent Cells recursively starting from the given cell
void reveal_Adjacent_Cells(int x, int y) {
	
    /// Checking if the cell is out of bounds, already visible, or a mine
    if (x < 0 || x >= board_Size || y < 0 || y >= board_Size || visible_Cells[x][y] || game_Board[x][y] == MINE_VALUE) {
        return;
    }

    /// Marking the cell as visible and decrement the remaining Cells count
    visible_Cells[x][y] = 1;
    remaining_Cells--;

    /// If the cell has no adjacent mines, reveal all adjacent Cells recursively
    if (game_Board[x][y] == 0) {
        for (i = x - 1; i <= x + 1; i++) {
            for (j = y - 1; j <= y + 1; j++) {
                reveal_Adjacent_Cells(i, j);
            }
        }
    }
}

/// Flag/unflag the cell at the current cursor position
void flag_Cell() {
    if (visible_Cells[cursor_Y][cursor_X] == 0) {
        visible_Cells[cursor_Y][cursor_X] = FLAGGED_CELL;
    } else if (visible_Cells[cursor_Y][cursor_X] == FLAGGED_CELL) {
        visible_Cells[cursor_Y][cursor_X] = 0;
    }
}

/// Drawing the game board on the console
void draw_Board() {
	
	int row, col;
	
    system("cls"); /// Clearing the screen

    /// Printing top border
    printf("  ");
    for (i = 0; i < board_Size; i++) {
        printf("+---");
    }
    printf("+\n");

    /// Printing board rows
    for (row = 0; row < board_Size; row++) {
        printf("  "); /// Print left padding
        printf("|"); /// Print left border
        for (col = 0; col < board_Size; col++) {
            /// Printing cursor position indicator
            if (row == cursor_Y && col == cursor_X) {
                printf("[");
            } else {
                printf(" ");
            }

            /// Printing cell content
            if (visible_Cells[row][col] == FLAGGED_CELL) {
                printf("F ");
            } else if (visible_Cells[row][col]) {
                /// If the cell is a mine, print '*'
                if (game_Board[row][col] == MINE_VALUE) {
                    printf("* ");
                } else {
                    printf("%d ", game_Board[row][col]);
                }
            } else {
                printf(". ");
            }

            /// Printing closing cursor position indicator
            if (row == cursor_Y && col == cursor_X) {
                printf("]");
            } else {
                printf(" ");
            }
        }
        printf("|"); /// Printing right border
        printf("\n");
    }

    /// Printing bottom border
    printf("  ");
    for (j= 0; j < board_Size; j++) {
        printf("+---");
    }
    printf("+\n");
}

/// Moving the cursor in the specified direction
void move_Cursor(char direction) {
    switch (direction) {
        case 'w':
        case 'W':
            /// Moving cursor up
            cursor_Y = (cursor_Y - 1 + board_Size) % board_Size;
            break;
        case 's':
        case 'S':
            /// Moving cursor down
            cursor_Y = (cursor_Y + 1) % board_Size;
            break;
        case 'a':
        case 'A':
            /// Moving cursor left
            cursor_X = (cursor_X - 1 + board_Size) % board_Size;
            break;
        case 'd':
        case 'D':
            /// Moving cursor right
            cursor_X = (cursor_X + 1) % board_Size;
            break;
    }
}

/// Revealing all cell values at a win or loss
void reveal_Cells() {
	
    for (i = 0; i < board_Size; i++) {
        for (j = 0; j < board_Size; j++) {
            visible_Cells[i][j] = 1;
        }
    }
    draw_Board();
}

int main() {
	
    char move;
    int play_Again = 1;
    int change_Difficulty = 0;
    
    display_Intro();
    
    while (play_Again) {
    	
        initialize_Board();
        cursor_X = 0;
        cursor_Y = 0;
        remaining_Cells = board_Size * board_Size - num_Mines;
        draw_Board();
        
        while (1) {
        	
            printf("Use WASD to move, 'o' to open, 'p' to flag/unflag\n");
            move = getch();
            
            if (move == 'o' || move == 'O') {
                /// Opening the cell at the current cursor position
                if (game_Board[cursor_Y][cursor_X] == MINE_VALUE) {
                	reveal_Cells();
                    printf("You hit a mine! Game Over!\n");
                    break;
                }
                reveal_Adjacent_Cells(cursor_Y, cursor_X);
                draw_Board();
                if (remaining_Cells == 0) {
                	reveal_Cells();
                    printf("CONGRATS! You won!!!\n");
                    break;
                }
            } else if (move == 'p' || move == 'P') {
                /// Flag/unflag the cell at the current cursor position
                flag_Cell();
                draw_Board();
            } else if (move == 'u' || move == 'U') {
                /// Secret button to win the game
                remaining_Cells = 0;
                reveal_Cells();
                printf("CONGRATS! You won using the secret button!!!\n");
                break;
            } else {
                /// Moving the cursor
                move_Cursor(move);
                draw_Board();
            }
        }
        
        /// Loop for asking to play again
        while (1) {
            printf("Play again? (y/n): \n\n");
            char choice = getch();
            if (choice == 'y' || choice == 'Y') {
            	/// Changing difficulty
                	printf("Change difficulty? (y/n): \n\n");
	            	move = getch();
	            	if (move == 'y' || move == 'Y') {
	            		choose_Difficulty_Level();
	            		break;
	            	} else if (move == 'n' || move == 'N') {
	            		play_Again = 1;
	            		break;	
					} else {
						 printf("\nInvalid input.\nPlease enter 'y' for yes or 'n' for no: \n\n");
					}
				
            } else if (choice == 'n' || choice == 'N') {
                play_Again = 0;
                break;
            } else {
                printf("\nInvalid input.\nPlease enter 'y' for yes or 'n' for no.\n");
            }
        }
    }
    return 0;
}
