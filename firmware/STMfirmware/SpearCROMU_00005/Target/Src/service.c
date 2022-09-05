/*

Author: James Nuttall (james@cromulence.co)

Copyright (c) 2014 Cromulence LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include <libcgc.h>
#include "stdlib_cgc.h"
#include "mymath.h"
#include "service.h"

/*
This CB emulates a chess board. The board is an 8x8 array; the pieces are unique chars saved within the array.

User takes turns between white and black side, moving pieces according to chess rules (mostly).

The pieces do not 'take' each other, they will only switch positions.
Pawns do not take diagonally, they only move forward one step at a time.
As in chess, pieces can not jump over each other; the knight is the only exception to this rule.

To exploit:
If a white knight makes it to 7,8 and then tries to display the board (the '9' option), it will crash. 
As seen in the pov.xml.
*/


// global variables for use by all other functions
char (*ptr_array)[WIDTH][HEIGHT]; // pointer to board array 
char current_team = WHITE_TEAM;  // white/black

// Returns color of piece (BLACK_PIECE, WHITE_PIECE, or EMPTY)
int getColor(char piece)
{
	if (piece <= QUEEN_WHITE && piece >= PAWN_WHITE)
		return WHITE_PIECE;
	if (piece <= QUEEN_BLACK && piece >= PAWN_BLACK)
		return BLACK_PIECE;
	return EMPTY;
}

// display contents of board
void displayboard(void)
{
    for(int i = HEIGHT - 1; i >= 0; i--)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            transmit(0, (char*)&(*ptr_array)[j][i], 1, NULL);
        }
        //printf("\n");
    }
}

// place all the pieces on the board
void initboard()
{
	// all the empty squares get a '.'
	for(int i = 0; i < HEIGHT; i++)
	{
		for(int j = 0; j < WIDTH; j++)
		{
			(*ptr_array)[i][j] = EMPTY;
		}
	}

	// putting WHITE pieces in position
	(*ptr_array)[0][0] = ROOK_WHITE;
	(*ptr_array)[1][0] = KNIGHT_WHITE;
	(*ptr_array)[2][0] = BISHOP_WHITE;
	(*ptr_array)[3][0] = QUEEN_WHITE;
	(*ptr_array)[4][0] = KING_WHITE;
	(*ptr_array)[5][0] = BISHOP_WHITE;
	(*ptr_array)[6][0] = KNIGHT_WHITE;
	(*ptr_array)[7][0] = ROOK_WHITE;

	(*ptr_array)[0][1] = PAWN_WHITE;
	(*ptr_array)[1][1] = PAWN_WHITE;
	(*ptr_array)[2][1] = PAWN_WHITE;
	(*ptr_array)[3][1] = PAWN_WHITE;
	(*ptr_array)[4][1] = PAWN_WHITE;
	(*ptr_array)[5][1] = PAWN_WHITE;
	(*ptr_array)[6][1] = PAWN_WHITE;
	(*ptr_array)[7][1] = PAWN_WHITE;

	// putting BLACK pieces in position
	(*ptr_array)[0][6] = PAWN_BLACK;
	(*ptr_array)[1][6] = PAWN_BLACK;
	(*ptr_array)[2][6] = PAWN_BLACK;
	(*ptr_array)[3][6] = PAWN_BLACK;
	(*ptr_array)[4][6] = PAWN_BLACK;
	(*ptr_array)[5][6] = PAWN_BLACK;
	(*ptr_array)[6][6] = PAWN_BLACK;
	(*ptr_array)[7][6] = PAWN_BLACK;

	(*ptr_array)[0][7] = ROOK_BLACK;
	(*ptr_array)[1][7] = KNIGHT_BLACK;
	(*ptr_array)[2][7] = BISHOP_BLACK;
	(*ptr_array)[3][7] = QUEEN_BLACK;
	(*ptr_array)[4][7] = KING_BLACK;
	(*ptr_array)[5][7] = BISHOP_BLACK;
	(*ptr_array)[6][7] = KNIGHT_BLACK;
	(*ptr_array)[7][7] = ROOK_BLACK;
}

// returns the piece at the given location
char getPiece(int x,int y)
{
	return (*ptr_array)[x][y];
}

// Checks for a collision
// returns COLLISION or NO_COLLISION
int checkNoCollision(int s_x, int s_y, int d_x, int d_y)
{
	Move move = {.src_x = s_x, .src_y = s_y, .dst_x = d_x, .dst_y = d_y};
	int src_x = move.src_x;;
	int src_y = move.src_y;
	
	int dst_x = move.dst_x;
	int dst_y = move.dst_y;

	int squares[WIDTH][2]; // squares between src and dst
	int square_cnt = 0; 

	char piece = getPiece(src_x, src_y);

	// pieces cannot pass through each other (except KNIGHT)
	if (piece == KNIGHT_WHITE || piece == KNIGHT_BLACK)
	{
		// make sure the knight isn't landing on a piece of its own color
		if (getColor(piece) != getColor(getPiece(dst_x,dst_y)))
			return NO_COLLISION;
		else
		{
			return COLLISION;
		}
	}
	// if piece is white and the destination is white, collision
	if (getColor(piece) == WHITE_PIECE && getColor(getPiece(dst_x,dst_y)) == WHITE_PIECE )
		return COLLISION;

	// if piece is black and the destination is black, collision
	if (getColor(piece) == BLACK_PIECE && getColor(getPiece(dst_x,dst_y)) == BLACK_PIECE)
		return COLLISION;

	// determine squares between src and dst on a FILE
	if (move.src_x == move.dst_x)
	{
		if (move.src_y < move.dst_y)
		{
			for(int i = move.src_y+1; i <= move.dst_y; i++)
			{
				squares[square_cnt][0] = move.src_x;
				squares[square_cnt++][1] = i;
			}
		}
		else
			for(int i = move.src_y-1; i >= move.dst_y; i--)
			{
				squares[square_cnt][0] = move.src_x;
				squares[square_cnt++][1] = i;
			}
	}
	// determine squares between src and dst on a RANK
	else if (move.src_y == move.dst_y)
	{
		if (move.src_x < move.dst_x)
		{
			for(int i = move.src_x+1; i <= move.dst_x; i++)
			{
				squares[square_cnt][0] = i;
				squares[square_cnt++][1] = move.src_y;
			}
		}
		else
			for(int i = move.src_x-1; i >= move.dst_x; i--)
			{
				squares[square_cnt][0] = i;
				squares[square_cnt++][1] = move.src_y;
			}
	}
	else
	{ 
		// determine squares between src and dst on a DIAGONAL
		int num = abs(move.dst_x - move.src_x);
		if (num == abs(move.dst_y - move.src_y))
		{
			for (int i = 1; i <= num; i++)
			{
				if (dst_y > src_y && dst_x > src_x)
				{
					squares[square_cnt][0] = src_x + i;
					squares[square_cnt++][1] = src_y + i;
				}
				else if (dst_y > src_y && dst_x < src_x)
				{
					squares[square_cnt][0] = src_x - i;
					squares[square_cnt++][1] = src_y + i;
				}
				else if (dst_y < src_y && dst_x > src_x)
				{
					squares[square_cnt][0] = src_x + i;
					squares[square_cnt++][1] = src_y - i;
				}
				else if (dst_y < src_y && dst_x < src_x)
				{
					squares[square_cnt][0] = src_x - i;
					squares[square_cnt++][1] = src_y - i;
				}
			}
		}
	}

	int boop = 0; // track how many pieces we go through in transit
	// now 'squares' has a list of all squares being passed through
	for (int i = 0; i < square_cnt; i++)
	{
		// if we've already seen a piece in the way and we're still 
		//   trying to move, collision
		if (boop >= 1)
			return COLLISION;

		char spot = getPiece(squares[i][0],squares[i][1]);

		if (spot == EMPTY)
		{
			continue; // no penalty for empty squares. keep going
		}

		if (getColor(piece) != getColor(spot))
		{
			boop++; // we ran into one enemy piece. track that we did, becuase we can't go through 2 or more
		}
		else
		{
			return COLLISION;
		}	
	}

	return NO_COLLISION;
}

// Attemps to perform the move, pieces are swapped if landed on
// Returns 1 if the move succeded
// Returns 0 if the move was illegal (based on the piece moved and the state of board)
int performMove(int s_x, int s_y, int d_x, int d_y)
{
	//Move move = {.src_x = s_x, .src_y = s_y, .dst_x = d_x, .dst_y = d_y};
	int src_x = s_x;
	int src_y = s_y;
	
	int dst_x = d_x;
	int dst_y = d_y;
	int piece = getPiece(src_x,src_y);

	if (current_team == WHITE_TEAM)	
	{
		// WHITE's turn
		if (getColor(piece) == BLACK_PIECE)
		{
			return 0; // trying to move a black piece during white's turn
		}
	}
	else
	{
		// BLACK's turn
		if (getColor(piece) == WHITE_PIECE)
		{
			return 0; // trying to move a white piece during black's turn
		}
	}

	
// NOTE: Vulnerability is here
// the knight move check is placed before the bounds check, which is the vulnerability
#ifndef PATCHED
	if (piece == KNIGHT_WHITE)
	{
		if (dst_x == src_x + 2 || dst_x == src_x - 2)
		{
			if (dst_y == src_y + 1 || dst_y == src_y - 1)
			{
				if (checkNoCollision(s_x, s_y, d_x, d_y))
				{
					swap(s_x, s_y, d_x, d_y);
					return 1;
				}
			}
		}
		if (dst_y == src_y + 2 || dst_y == src_y - 2)
		{
			if (dst_x == src_x + 1 || dst_x == src_x - 1)
			{
				if (checkNoCollision(s_x, s_y, d_x, d_y))
				{
					swap(s_x, s_y, d_x, d_y);
					return 1;
				}
			}
		}
		return 0;
	}
#endif

	// make sure they're moving within the bounds of the board
	if (dst_x > WIDTH - 1 || dst_x < 0 || dst_y > HEIGHT - 1 || dst_y < 0)
		return 0;

	if (piece == EMPTY)
		return 0;

	if (piece == PAWN_WHITE)
	{
		if (dst_x == src_x && dst_y == (src_y + 1))
		{
			if (checkNoCollision(s_x, s_y, d_x, d_y))
			{
				swap(s_x, s_y, d_x, d_y);
				return 1;
			}
		}
		else
			return 0;
	}
	if (piece == PAWN_BLACK)
	{
		if (dst_x == src_x && dst_y == (src_y - 1))
		{
			if (checkNoCollision(s_x, s_y, d_x, d_y))
			{
				swap(s_x, s_y, d_x, d_y);
				return 1;
			}
		}
		else
			return 0;
	}

// This is where the knight move check should happen (after the bounds check, above)
#ifdef PATCHED
	if (piece == KNIGHT_WHITE)
	{
		if (dst_x == src_x + 2 || dst_x == src_x - 2)
		{
			if (dst_y == src_y + 1 || dst_y == src_y - 1)
			{
				if (checkNoCollision(s_x, s_y, d_x, d_y))
				{
					swap(s_x, s_y, d_x, d_y);
					return 1;
				}
			}
		}
		if (dst_y == src_y + 2 || dst_y == src_y - 2)
		{
			if (dst_x == src_x + 1 || dst_x == src_x - 1)
			{
				if (checkNoCollision(s_x, s_y, d_x, d_y))
				{
					swap(s_x, s_y, d_x, d_y);
					return 1;
				}
			}
		}
		return 0;
	}
#endif

	if (piece == KNIGHT_BLACK)
	{
		if (dst_x == src_x + 2 || dst_x == src_x - 2)
		{
			if (dst_y == src_y + 1 || dst_y == src_y - 1)
			{
				if (checkNoCollision(s_x, s_y, d_x, d_y))
				{
					swap(s_x, s_y, d_x, d_y);
					return 1;
				}
			}
		}
		if (dst_y == src_y + 2 || dst_y == src_y - 2)
		{
			if (dst_x == src_x + 1 || dst_x == src_x - 1)
			{
				if (checkNoCollision(s_x, s_y, d_x, d_y))
				{
					swap(s_x, s_y, d_x, d_y);
					return 1;
				}
			}
		}
		return 0;
	}
	if (piece == BISHOP_WHITE || piece == BISHOP_BLACK)
	{
		// bishop traveling to the right
		int num = abs(dst_x - src_x);
		if (num == abs(dst_y - src_y))
		{	// good
			if (checkNoCollision(s_x, s_y, d_x, d_y))
			{
				swap(s_x, s_y, d_x, d_y);
				return 1;
			}
		}
		return 0;
	}
	if (piece == QUEEN_WHITE || piece == QUEEN_BLACK)
	{
		int num = abs(dst_x - src_x);
		// moving diagonal
		if (num == abs(dst_y - src_y))
		{	// good
			if (checkNoCollision(s_x, s_y, d_x, d_y))
			{
				swap(s_x, s_y, d_x, d_y);
				return 1;
			}
		}
		// moving on RANK or FILE
		if (dst_x == src_x || dst_y == src_y)
		{
			if (checkNoCollision(s_x, s_y, d_x, d_y))
			{
				swap(s_x, s_y, d_x, d_y);
				return 1;
			}
		}
		return 0;
	}
	if (piece == KING_WHITE || piece == KING_BLACK)
	{
		int diffx = dst_x - src_x;
		int diffy = dst_y - src_y;

		// king can only move 1 square.
		if (abs(diffx) > 1 || abs(diffy) > 1)
			return 0;

		if (checkNoCollision(s_x, s_y, d_x, d_y))
		{
			swap(s_x, s_y, d_x, d_y);
			return 1;
		}
		return 0;
	}
	if (piece == ROOK_BLACK || piece == ROOK_WHITE)
	{
		// moving on RANK or FILE
		if (dst_x == src_x || dst_y == src_y)
		{
			if (checkNoCollision(s_x, s_y, d_x, d_y))
			{
				swap(s_x, s_y, d_x, d_y);
				return 1;
			}
		}
		return 0;
	}

	return 0;
}

// swap two array locations
void swap(int s_x, int s_y,int d_x,int d_y)
{
	char tmp;
	Move move = {.src_x = s_x, .src_y = s_y, .dst_x = d_x, .dst_y = d_y};
    tmp = (*ptr_array)[move.dst_x][move.dst_y];
    (*ptr_array)[move.dst_x][move.dst_y] = (*ptr_array)[move.src_x][move.src_y];
    (*ptr_array)[move.src_x][move.src_y] = tmp;	
}

// Verifies input is in correct move format x,y x,y
// return 1 if format is correct
// return 0 otherwise
int verifyFormat(char *buf, int len)
{
	
	buf[len] = '\x00';
	// #,# #,#
	if (buf[0] >= '0' && buf[0] <= '9')
		if (buf[1] == ',')
			if (buf[2] >= '0' && buf[2] <= '9')
				if (buf[3] == ' ')
					if (buf[4] >= '0' && buf[4] <= '9')
						if (buf[5] == ',')
							if (buf[6] >= '0' && buf[6] <= '9')
								return 1;

	return 0;
}

// receive input, parse into 'move' structure
// return GOOD_INPUT for good input
// return INVALID_INPUT for bad input
// return DISPLAY_BOARD for request to display board
int parseUserInput(Move *movers)
{
	size_t_cgc size = 0; // number of bytes user input
	char buf[15]; // will hold user input
	//int num = 0;

	Move tmp = {77,77,77,77}; // arbitrary default values. used later to ensure we received values

	bzero_cgc(buf,15);
	int status;
	status = receive_cgc( buf, sizeof(buf) - 1, &size);
	if (status != 0)
	{
		// Error on receive
		return INPUT_ERROR;
	}

	// Discard newline from manual input
	if (buf[size-1] == '\n')
	{
		size--;
	}

	if (size == 1)
	{
		if (buf[0] == '9')
		{
			// display board
			return DISPLAY_BOARD;
		}
	}
	else if (size == 3)
	{
		if (buf[0] == '6' && buf[1] == '6' && buf[2] == '6')
		{
			return END_GAME;
		}
	}
	else if (size != 7) 
	{
		//printf("incorrect input\n");
		return INVALID_INPUT;
	}

	if (!verifyFormat(buf, size))
	{
		//printf("incorrect input\n");
		return INVALID_INPUT;
	}

	for (int i = 0; i < sizeof(buf); i++)
	{	
		if (buf[i] <= '9' && buf[i] >= '0')
		{
			if (i == 0)
				tmp.src_x = atoi(&buf[i]);
			else if (i == 2)
				tmp.src_y = atoi(&buf[i]);
			else if (i == 4)
				tmp.dst_x = atoi(&buf[i]);
			else if (i == 6)
				tmp.dst_y = atoi(&buf[i]);
		}
	}
	
	if (tmp.src_x != 77 && tmp.src_y != 77 && tmp.dst_x != 77 && tmp.dst_y != 77)
	{
		movers->src_x = tmp.src_x;
		movers->src_y = tmp.src_y;
		movers->dst_x = tmp.dst_x;
		movers->dst_y = tmp.dst_y;
		return GOOD_INPUT;
	}
	else
	{
		return INVALID_INPUT;
	}
}

int test(void)
{
	// function pointer to print chess board
	void (*displayer)(); // overwriting the below array will destroy this function pointer
	// chess board
	char array[WIDTH][HEIGHT];

	displayer = displayboard;
	ptr_array = &array;

	initboard();
	//displayer();

	Move move = {.src_x = 0, .src_y = 0, .dst_x = 0, .dst_y = 0};
	
	while(1)
	{
		// print prompt
		if (current_team == WHITE_TEAM){
			//printf(PROMPT_WHITE);
		}
		else{
			//printf(PROMPT_BLACK);
		}
			

		// receive: x,y x,y
		int ret = parseUserInput(&move);
		if (ret == INVALID_INPUT)
		{
			//printf(WARNING);
			continue;
		}
		else if (ret == DISPLAY_BOARD)
		{
			//displayer();
			continue;
		}
		else if (ret == END_GAME)
		{
			//printf("good game\n");
			return(0);
		}
		else if (ret == INPUT_ERROR)
		{
			return(0);
		}

		if(performMove(move.src_x,move.src_y,move.dst_x, move.dst_y))
		{
			// Move suceeded
			//printf(OK);

			// Switch teams
			if (current_team == WHITE_TEAM)
				current_team = BLACK_TEAM;
			else
				current_team = WHITE_TEAM;
		}
		else
		{
			// Move was illegal
			//printf(NO);
		}
	}

	return 0;
}

