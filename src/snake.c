#include <stdio.h>
#include <string.h>

#include "snake_utils.h"
#include "game.h"

int main(int argc, char *argv[]) {
  bool io_stdin = false;
  char *in_filename = NULL;
  char *out_filename = NULL;
  game_t *game = NULL;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i < argc - 1) {
      if (io_stdin) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      in_filename = argv[i + 1];
      i++;
      continue;
    } else if (strcmp(argv[i], "--stdin") == 0) {
      if (in_filename != NULL) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      io_stdin = true;
      continue;
    }
    if (strcmp(argv[i], "-o") == 0 && i < argc - 1) {
      out_filename = argv[i + 1];
      i++;
      continue;
    }
    fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
    return 1;
  }

  // Do not modify anything above this line.

  /* Task 7 */

  // Read board from file, or create default board
  FILE *fp = NULL;
  if (in_filename != NULL) {
    // TODO: Load the board from in_filename
    fp = fopen(in_filename, "r");
    // TODO: If the file doesn't exist, return -1
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", in_filename);
        return -1;
    }
    game = load_board(fp);
    // TODO: Then call initialize_snakes on the game you made
    initialize_snakes(game);
    // TODO: close file pointer
    fclose(fp);
  } else if (io_stdin) {
    // TODO: Load the board from stdin
    fp = stdin;
    game = load_board(fp);
    // TODO: Then call initialize_snakes on the game you made
    initialize_snakes(game);
  } else {
    // TODO: Create default game
    game = create_default_game();
  }

  // TODO: Update game. Use the deterministic_food function
  // (already implemented in snake_utils.h) to add food.
  update_game(game, deterministic_food);
  // Write updated board to file or stdout
  if (out_filename != NULL) {
    // TODO: Save the board to out_filename
    save_board(game, out_filename);
  } else {
    // TODO: Print the board to stdout
    print_board(game, stdout);
  }

  // TODO: Free the game
  free_game(game);
  return 0;
}
