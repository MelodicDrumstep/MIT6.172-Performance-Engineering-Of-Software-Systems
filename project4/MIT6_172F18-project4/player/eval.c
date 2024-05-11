// Copyright (c) 2015 MIT License by 6.172 Staff

#include "./eval.h"

#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "./move_gen.h"
#include "./tbassert.h"

// -----------------------------------------------------------------------------
// Evaluation
// -----------------------------------------------------------------------------

typedef int32_t ev_score_t;  // Static evaluator uses "hi res" values

int RANDOMIZE;

int PCENTRAL;
int PBETWEEN;
int PCENTRAL;
int KFACE;
int KAGGRESSIVE;
int MOBILITY;
int LCOVERAGE;

// Heuristics for static evaluation - described in the google doc
// mentioned in the handout.

// PCENTRAL heuristic: Bonus for Pawn near center of board
ev_score_t pcentral(fil_t f, rnk_t r) {
  double df = BOARD_WIDTH / 2 - f - 1;
  if (df < 0) {
    df = f - BOARD_WIDTH / 2;
  }
  double dr = BOARD_WIDTH / 2 - r - 1;
  if (dr < 0) {
    dr = r - BOARD_WIDTH / 2;
  }
  double bonus = 1 - sqrt(df * df + dr * dr) / (BOARD_WIDTH / sqrt(2));
  return PCENTRAL * bonus;
}


// returns true if c lies on or between a and b, which are not ordered
bool between(int c, int a, int b) {
  bool x = ((c >= a) && (c <= b)) || ((c <= a) && (c >= b));
  return x;
}

// PBETWEEN heuristic: Bonus for Pawn at (f, r) in rectangle defined by Kings at the corners
ev_score_t pbetween(position_t* p, fil_t f, rnk_t r) {
  bool is_between =
    between(f, fil_of(p->kloc[WHITE]), fil_of(p->kloc[BLACK])) &&
    between(r, rnk_of(p->kloc[WHITE]), rnk_of(p->kloc[BLACK]));
  return is_between ? PBETWEEN : 0;
}


// KFACE heuristic: bonus (or penalty) for King facing toward the other King
ev_score_t kface(position_t* p, fil_t f, rnk_t r) {
  square_t sq = square_of(f, r);
  piece_t x = p->board[sq];
  color_t c = color_of(x);
  square_t opp_sq = p->kloc[opp_color(c)];
  int delta_fil = fil_of(opp_sq) - f;
  int delta_rnk = rnk_of(opp_sq) - r;
  int bonus;

  switch (ori_of(x)) {
  case NN:
    bonus = delta_rnk;
    break;

  case EE:
    bonus = delta_fil;
    break;

  case SS:
    bonus = -delta_rnk;
    break;

  case WW:
    bonus = -delta_fil;
    break;

  default:
    bonus = 0;
    tbassert(false, "Illegal King orientation.\n");
  }

  return (bonus * KFACE) / (abs(delta_rnk) + abs(delta_fil));
}

// KAGGRESSIVE heuristic: bonus for King with more space to back
ev_score_t kaggressive(position_t* p, fil_t f, rnk_t r) {
  square_t sq = square_of(f, r);
  piece_t x = p->board[sq];
  color_t c = color_of(x);
  tbassert(ptype_of(x) == KING, "ptype_of(x) = %d\n", ptype_of(x));

  square_t opp_sq = p->kloc[opp_color(c)];
  fil_t of = fil_of(opp_sq);
  rnk_t _or = (rnk_t) rnk_of(opp_sq);

  int delta_fil = of - f;
  int delta_rnk = _or - r;

  int bonus = 0;

  if (delta_fil >= 0 && delta_rnk >= 0) {
    bonus = (f + 1) * (r + 1);
  } else if (delta_fil <= 0 && delta_rnk >= 0) {
    bonus = (BOARD_WIDTH - f) * (r + 1);
  } else if (delta_fil <= 0 && delta_rnk <= 0) {
    bonus = (BOARD_WIDTH - f) * (BOARD_WIDTH - r);
  } else if (delta_fil >= 0 && delta_rnk <= 0) {
    bonus = (f + 1) * (BOARD_WIDTH - r);
  }

  return (KAGGRESSIVE * bonus) / (BOARD_WIDTH * BOARD_WIDTH);
}

// Marks the path/line-of-sight of the laser until it hits a piece or goes off
// the board.
//
// p : Current board state.
// c : Color of king shooting laser.
// laser_map : End result will be stored here. Every square on the
//             path of the laser is marked with mark_mask.
// mark_mask : What each square is marked with.
void mark_laser_path(position_t* p, color_t c, char* laser_map,
                     char mark_mask) {
  square_t sq = p->kloc[c];
  int bdir = ori_of(p->board[sq]);

  tbassert(ptype_of(p->board[sq]) == KING,
           "ptype: %d\n", ptype_of(p->board[sq]));
  laser_map[sq] |= mark_mask;

  while (true) {
    sq += beam_of(bdir);
    laser_map[sq] |= mark_mask;
    tbassert(sq < ARR_SIZE && sq >= 0, "sq: %d\n", sq);

    switch (ptype_of(p->board[sq])) {
    case EMPTY:  // empty square
      break;
    case PAWN:  // Pawn
      bdir = reflect_of(bdir, ori_of(p->board[sq]));
      if (bdir < 0) {  // Hit back of Pawn
        return;
      }
      break;
    case KING:  // King
      return;  // sorry, game over my friend!
      break;
    case INVALID:  // Ran off edge of board
      return;
      break;
    default:  // Shouldna happen, man!
      tbassert(false, "Not cool, man.  Not cool.\n");
      break;
    }
  }
}

// Marks the path/line-of-sight of the laser until it hits a piece or goes off
// the board.
// Increment for each time you touch a square with the laser
//
// p : Current board state.
// c : Color of king shooting laser.
// laser_map : End result will be stored here. Every square on the
//             path of the laser is marked with mark_mask.
// mark_mask : What each square is marked with.
void add_laser_path(position_t* p, color_t c, float* laser_map) {
  square_t sq = p->kloc[c];
  int bdir = ori_of(p->board[sq]);
  int length = 1;

  tbassert(ptype_of(p->board[sq]) == KING,
           "ptype: %d\n", ptype_of(p->board[sq]));
  // laser_map[sq] += touch_weight;

  while (true) {
    sq += beam_of(bdir);

    // set laser map to min
    if(laser_map[sq] > length) {
      laser_map[sq] = length;
    }
    length++;
    
    tbassert(sq < ARR_SIZE && sq >= 0, "sq: %d\n", sq);

    switch (ptype_of(p->board[sq])) {
    case EMPTY:  // empty square
      break;
    case PAWN:  // Pawn
      bdir = reflect_of(bdir, ori_of(p->board[sq]));
      if (bdir < 0) {  // Hit back of Pawn
        return;
      }

      // if bouncing off an opposing pawn, add extra to the length of path
      // because the opponent can affect it 
      if (color_of(p->board[sq]) != c) {
        length += 2;
      }
      break;
    case KING:  // King
      return;  // sorry, game over my friend!
      break;
    case INVALID:  // Ran off edge of board
      return;
      break;
    default:  // Shouldna happen, man!
      tbassert(false, "Not cool, man.  Not cool.\n");
      break;
    }
  }
}

float mult_dist(square_t a, square_t b) {
  float delta_fil = abs(fil_of(a) - fil_of(b));
  float delta_rnk = abs(rnk_of(a) - rnk_of(b));
  if (delta_fil == 0 && delta_rnk == 0) {
    return 2;
  }
  float x = ( 1 / ( delta_fil + 1 ) ) * ( 1 / ( delta_rnk + 1 ) );
  return x;
}

// Manhattan distance
int manhattan_dist(square_t a, square_t b) {
  int delta_fil = abs(fil_of(a) - fil_of(b));
  int delta_rnk = abs(rnk_of(a) - rnk_of(b));
  return delta_fil + delta_rnk;
}

float laser_coverage(position_t* p, color_t color) {
  position_t np;
  sortable_move_t moves[MAX_NUM_MOVES];
  int num_moves = generate_all_with_color(p, moves, color);
  int i;

  float coverage_map[ARR_SIZE];

  // initialization
  for (int i = 0; i < ARR_SIZE; ++i) {
    coverage_map[i] = FLT_MAX;
  }

  // increment laser path for each possible move
  for(i = 0; i < num_moves; i++) {
    move_t mv = get_move(moves[i]);

    low_level_make_move(p, &np, mv); // make the move

    add_laser_path(&np, color, coverage_map);  // increment laser path
  }

  // get square of opposing king
  square_t king_sq = p->kloc[color];
  // get square of opposing king
  square_t opp_king_sq = p->kloc[opp_color(color)];

  // name it something besides laser_map
  // initialize laser map
  float result = 0;

  // add in everything on board
  for (fil_t f = 0; f < BOARD_WIDTH; ++f) {
    for (rnk_t r = 0; r < BOARD_WIDTH; ++r) {
      if (coverage_map[square_of(f, r)] < FLT_MAX) {
        // length of path divided by length of shortest possible path
        tbassert(manhattan_dist(king_sq, square_of(f, r)) <= coverage_map[square_of(f, r)], "f: %d, r: %d, dist = %d, map: %f\n", f, r, manhattan_dist(king_sq, square_of(f, r)), coverage_map[square_of(f, r)]);

        // printf("before: %f, dist: %d\n", coverage_map[square_of(f, r)], manhattan_dist(king_sq, square_of(f, r)));
        coverage_map[square_of(f, r)] = ((manhattan_dist(king_sq, square_of(f, r))) / (coverage_map[square_of(f, r)]));

        tbassert(mult_dist(square_of(f, r), opp_king_sq) > 0, "mult_distance must be positive");
        coverage_map[square_of(f, r)] *= mult_dist(square_of(f, r), opp_king_sq);
        result += coverage_map[square_of(f, r)];
      }
    }
  }

  // add in off-board weights
  // add in top row
  rnk_t r;
  fil_t f;
  r = -1;
  for(f = -1; f < BOARD_WIDTH + 1; f++) {
    result += mult_dist(square_of(f, r), opp_king_sq);
  }
  // add in bottom row
  r = BOARD_WIDTH;
  for(f = -1; f < BOARD_WIDTH + 1; f++) {
    result += mult_dist(square_of(f, r), opp_king_sq);
  }

  // add in left col (minus top and bottom)
  f = -1;
  for(r = 0; r < BOARD_WIDTH; r++) {
    result += mult_dist(square_of(f, r), opp_king_sq);
  }

  // add in right col (minus top and bottom)
  f = BOARD_WIDTH;
  for(r = 0; r < BOARD_WIDTH; r++) {
    result += mult_dist(square_of(f, r), opp_king_sq);
  }

  return result;
}

// MOBILITY heuristic: safe squares around king of given color.
int mobility(position_t* p, color_t color) {
  color_t c = opp_color(color);
  char laser_map[ARR_SIZE];

  for (int i = 0; i < ARR_SIZE; ++i) {
    laser_map[i] = 4;   // Invalid square
  }

  for (fil_t f = 0; f < BOARD_WIDTH; ++f) {
    for (rnk_t r = 0; r < BOARD_WIDTH; ++r) {
      laser_map[square_of(f, r)] = 0;
    }
  }

  mark_laser_path(p, c, laser_map, 1);  // find path of laser given that you aren't moving

  int mobility = 0;
  square_t king_sq = p->kloc[color];
  tbassert(ptype_of(p->board[king_sq]) == KING,
           "ptype: %d\n", ptype_of(p->board[king_sq]));
  tbassert(color_of(p->board[king_sq]) == color,
           "color: %d\n", color_of(p->board[king_sq]));

  if (laser_map[king_sq] == 0) {
    mobility++;
  }
  for (int d = 0; d < 8; ++d) {
    square_t sq = king_sq + dir_of(d);
    if (laser_map[sq] == 0) {
      mobility++;
    }
  }
  return mobility;
}


// Harmonic-ish distance: 1/(|dx|+1) + 1/(|dy|+1)
float h_dist(square_t a, square_t b) {
  //  printf("a = %d, FIL(a) = %d, RNK(a) = %d\n", a, FIL(a), RNK(a));
  //  printf("b = %d, FIL(b) = %d, RNK(b) = %d\n", b, FIL(b), RNK(b));
  int delta_fil = abs(fil_of(a) - fil_of(b));
  int delta_rnk = abs(rnk_of(a) - rnk_of(b));
  float x = (1.0 / (delta_fil + 1)) + (1.0 / (delta_rnk + 1));
  //  printf("max_dist = %d\n\n", x);
  return x;
}

// Static evaluation.  Returns score
score_t eval(position_t* p, bool verbose) {
  // seed rand_r with a value of 1, as per
  // http://linux.die.net/man/3/rand_r
  static __thread unsigned int seed = 1;
  // verbose = true: print out components of score
  ev_score_t score[2] = { 0, 0 };
  //  int corner[2][2] = { {INF, INF}, {INF, INF} };
  ev_score_t bonus;
  char buf[MAX_CHARS_IN_MOVE];

  for (fil_t f = 0; f < BOARD_WIDTH; f++) {
    for (rnk_t r = 0; r < BOARD_WIDTH; r++) {
      square_t sq = square_of(f, r);
      piece_t x = p->board[sq];
      color_t c = color_of(x);
      if (verbose) {
        square_to_str(sq, buf, MAX_CHARS_IN_MOVE);
      }

      switch (ptype_of(x)) {
      case EMPTY:
        break;
      case PAWN:
        // MATERIAL heuristic: Bonus for each Pawn
        bonus = PAWN_EV_VALUE;
        if (verbose) {
          printf("MATERIAL bonus %d for %s Pawn on %s\n", bonus, color_to_str(c), buf);
        }
        score[c] += bonus;

        // PBETWEEN heuristic
        bonus = pbetween(p, f, r);
        if (verbose) {
          printf("PBETWEEN bonus %d for %s Pawn on %s\n", bonus, color_to_str(c), buf);
        }
        score[c] += bonus;

        // PCENTRAL heuristic
        bonus = pcentral(f, r);
        if (verbose) {
          printf("PCENTRAL bonus %d for %s Pawn on %s\n", bonus, color_to_str(c), buf);
        }
        score[c] += bonus;
        break;

      case KING:
        // KFACE heuristic
        bonus = kface(p, f, r);
        if (verbose) {
          printf("KFACE bonus %d for %s King on %s\n", bonus,
                 color_to_str(c), buf);
        }
        score[c] += bonus;

        // KAGGRESSIVE heuristic
        bonus = kaggressive(p, f, r);
        if (verbose) {
          printf("KAGGRESSIVE bonus %d for %s King on %s\n", bonus, color_to_str(c), buf);
        }
        score[c] += bonus;
        break;
      case INVALID:
        break;
      default:
        tbassert(false, "Jose says: no way!\n");   // No way, Jose!
      }
    }
  }

  // LASER_COVERAGE heuristic
  float w_coverage = LCOVERAGE * laser_coverage(p, WHITE);
  score[WHITE] += (int) w_coverage;
  if (verbose) {
    printf("COVERAGE bonus %d for White\n",(int) w_coverage);
  }
  float b_coverage = LCOVERAGE * laser_coverage(p, BLACK);
  score[BLACK] += (int) b_coverage;
  if (verbose) {
    printf("COVERAGE bonus %d for Black\n",(int) b_coverage);
  }

  // score from WHITE point of view
  ev_score_t tot = score[WHITE] - score[BLACK];

  if (RANDOMIZE) {
    ev_score_t  z = rand_r(&seed) % (RANDOMIZE * 2 + 1);
    tot = tot + z - RANDOMIZE;
  }

  if (color_to_move_of(p) == BLACK) {
    tot = -tot;
  }

  return tot / EV_SCORE_RATIO;
}
