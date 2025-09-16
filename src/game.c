#include "game.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_t *game, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_t *game, unsigned int snum);
static char next_square(game_t *game, unsigned int snum);
static void update_tail(game_t *game, unsigned int snum);
static void update_head(game_t *game, unsigned int snum);

/* Task 1 */
game_t *create_default_game() {
  // TODO: Implement this function.
  game_t *game = malloc(sizeof(game_t));
  game->num_rows = 18;
  game->board = malloc(sizeof(char*) * 18);
  const char *rows[18] = {
      "####################",
      "#                  #",
      "# d>D    *         #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "####################",
  };

  for (int i = 0; i < game->num_rows; i++) {
      game->board[i] = malloc(sizeof(char) * 22);
      strcpy(game->board[i], rows[i]);
      game->board[i][20] = '\n';
      game->board[i][21] = '\0';
  }
  game->num_snakes = 1;
  game->snakes = malloc(sizeof(snake_t));
  game->snakes[0].tail_row = 2;
  game->snakes[0].tail_col = 2;
  game->snakes[0].head_row = 2;
  game->snakes[0].head_col = 4;
  game->snakes[0].live = true;
  return game;
}

/* Task 2 */
void free_game(game_t *game) {
  // TODO: Implement this function.
  for (int i = 0; i < game->num_rows; i++) {
    free(game->board[i]);
  }
  free(game->board);
  free(game->snakes);
  free(game);
  return;
}

/* Task 3 */
void print_board(game_t *game, FILE *fp) {
  // TODO: Implement this function.
  for (int i = 0; i < game->num_rows; i++) {
    fprintf(fp, "%s", game->board[i]);
  }
  return;
}

/*
  Saves the current game into filename. Does not modify the game object.
  (already implemented for you).
*/
void save_board(game_t *game, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(game, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_t *game, unsigned int row, unsigned int col) { return game->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_t *game, unsigned int row, unsigned int col, char ch) {
  game->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  // TODO: Implement this function.
  return (strchr("wasd", c) != NULL);
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
  return (strchr("WASDx", c) != NULL);
}
  
/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  return (strchr("wasd^<v>WASDx", c) != NULL);
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function.
  if (c == '^') {
    return 'w';
  } else if (c == '<') {
    return 'a';
  } else if (c == 'v') {
    return 's'; 
  } else if (c == '>') {
    return 'd';
  }  
  return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
  if (c == 'W') {
    return '^';
  } else if (c == 'A') {
    return '<';
  } else if (c == 'S') {
    return 'v';
  } else if (c == 'D') {
    return '>';
  }  
  return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  // TODO: Implement this function.
  if (c == 'v' || c == 's' || c == 'S') {
      return cur_row + 1;
  } else if (c == '^' || c == 'w' || c == 'W') {
      return cur_row - 1;
  }
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  // TODO: Implement this function.
  if (c == '>' || c == 'd' || c == 'D') {
      return cur_col + 1;
  } else if (c == '<' || c == 'a' || c == 'A') {
      return cur_col - 1;
  }
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_game. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_t *game, unsigned int snum) {
  // TODO: Implement this function.
  unsigned int col = game->snakes[snum].head_col; 
  unsigned int row = game->snakes[snum].head_row;
  unsigned int ncol = get_next_col(col, game->board[row][col]);
  unsigned int nrow = get_next_row(row, game->board[row][col]);
  return game->board[nrow][ncol];
}

/*
  Task 4.3

  Helper function for update_game. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_t *game, unsigned int snum) {
  // TODO: Implement this function.
  unsigned int row = game->snakes[snum].head_row;
  unsigned int col = game->snakes[snum].head_col;
  char c = game->board[row][col];
  unsigned int nrow = get_next_row(row, c);
  unsigned int ncol = get_next_col(col, c);
  game->board[row][col] = head_to_body(c);
  game->board[nrow][ncol] = c;
  game->snakes[snum].head_row = nrow;
  game->snakes[snum].head_col = ncol;
  return;
}

/*
  Task 4.4

  Helper function for update_game. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_t *game, unsigned int snum) {
  // TODO: Implement this function.
  unsigned int row = game->snakes[snum].tail_row;
  unsigned int col = game->snakes[snum].tail_col;
  char c = game->board[row][col];
  unsigned int nrow = get_next_row(row, c);
  unsigned int ncol = get_next_col(col, c);
  game->board[nrow][ncol] = body_to_tail(game->board[nrow][ncol]);
  game->board[row][col] = ' ';
  game->snakes[snum].tail_row = nrow;
  game->snakes[snum].tail_col = ncol;
  return;
}

/* Task 4.5 */
void update_game(game_t *game, int (*add_food)(game_t *game)) {
  // TODO: Implement this function.
  // each snakes move one move and if crashes, die, and head is replaced with x
  // if touch fruit, then update head
  for (unsigned int i = 0; i < game->num_snakes; i++) {
      char c = next_square(game, i);
      if (c == '*') {
          update_head(game, i);
          add_food(game);
      } else if (c == ' ') {
          update_head(game, i);
          update_tail(game, i);
      } else {
          set_board_at(game, game->snakes[i].head_row, game->snakes[i].head_col, 'x');
          game->snakes[i].live = false;
      }
   }
  return;
}

/* Task 5.1 */
char *read_line(FILE *fp) {
  // TODO: Implement this function.
  size_t line_size = 54;
  char *line = malloc(line_size);
  size_t len = 0;
  while (1) {
    char *result = fgets(line + len, (int)(line_size - len), fp);
    if (result == NULL) {
        if (len == 0) {
            free(line);
            return NULL;
        } else {
            break;
        }
    }
    len += strlen(line + len);
        if (strchr(line, '\n') != NULL) {
            break;
        }
        line_size *= 2;
        char *new_line = realloc(line, line_size);
        line = new_line;
  }
  return line;
}

/* Task 5.2 */
game_t *load_board(FILE *fp) {
  // TODO: Implement this function.
  game_t *game = malloc(sizeof(game_t));
  game->num_rows = 0;
  size_t cap = 8;
  game->board = malloc(sizeof(char*) * cap);
  while (1) {
      char* line = read_line(fp);
      if (line == NULL) {
        break;
      }
      game->board[game->num_rows] = line;
      game->num_rows += 1;
      if (game->num_rows == cap) {
          cap *= 2;
          char **new_board = realloc(game->board, sizeof(char*) * cap);
          game->board = new_board;
      }
  }
  game->num_snakes = 0;
  game->snakes = NULL;
  return game;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_t *game, unsigned int snum) {
  // TODO: Implement this function.
  unsigned int row = game->snakes[snum].tail_row;
  unsigned int col = game->snakes[snum].tail_col;
  while (1) {
      char c = game->board[row][col];
      if(is_head(c)) {
        game->snakes[snum].head_row = row;
        game->snakes[snum].head_col = col;
        break;
      }
      row = get_next_row(row, c);
      col = get_next_col(col, c);
  }
  return;
}

/* Task 6.2 */
game_t *initialize_snakes(game_t *game) {
  // TODO: Implement this function.
  game->snakes = malloc(sizeof(snake_t));
  game->num_snakes = 0;
  unsigned int *s = &(game->num_snakes);
  for (unsigned int i = 0; i < game->num_rows; i++) {
      for (unsigned int j = 0; j < strlen(game->board[i]); j++) {
          char c = game->board[i][j];
          if (is_tail(c)) {
              game->snakes[*s].tail_row = i;
              game->snakes[*s].tail_col = j;
              find_head(game, *s);
              *s += 1;
              snake_t* new_snakes = realloc(game->snakes, sizeof(snake_t) * ((*s) + 1));
              game->snakes = new_snakes;
          }
      }
  }
  return game;
}
