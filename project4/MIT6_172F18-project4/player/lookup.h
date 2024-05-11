// Copyright (c) 2018 MIT License by 6.172 Staff

#ifndef LOOKUP_H
#define LOOKUP_H

#define OPEN_BOOK_DEPTH 2

int lookup_sizes[OPEN_BOOK_DEPTH] = {
  2, 4
};

// Move histories are paired with their best moves in the lookup tables
// IE: lookup_table_depth_n[] = {move_history1, best_move1, move_history2, best_move2, ...}
char* lookup_table_depth_1[] = {"", "g4L"};
char* lookup_table_depth_2[] = {"g4L", "b3L", "f3L", "b3L"};

char** lookup_tables[OPEN_BOOK_DEPTH] = {
    lookup_table_depth_1,
    lookup_table_depth_2
};

#endif // LOOKUP_H
