#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 2048

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bb_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fb_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t r_lock = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t f_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t bb_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t fb_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t r_cond = PTHREAD_COND_INITIALIZER;

typedef struct bb_player {
  pthread_t ptid;  // thread id returned by pthread_create
  int field_slot_id;
  int id;            // we manage this "name" of the player
  int nthreads;      // number of threads for this sport
  int times_played;  // times a thread played
  int starter;       // whether a thread will act as a leader to start game
  int found_slot;    // whether thread found a position
} bb_player;

typedef struct fb_player {
  pthread_t ptid;  // thread id returned by pthread_create
  int field_slot_id;
  int id;            // we manage this "name" of the player
  int nthreads;      // number of threads for this sport
  int times_played;  // times a thread played
  int starter;       // whether a thread will act as a leader to start game
  int found_slot;    // whether thread found a position
} fb_player;

typedef struct r_player {
  pthread_t ptid;  // thread id returned by pthread_create
  int field_slot_id;
  int id;            // we manage this "name" of the player
  int nthreads;      // number of threads for this sport
  int times_played;  // times a thread played
  int starter;       // whether a thread will act as a leader to start game
  int found_slot;    // whether thread found a position
} r_player;

typedef struct field {
  int current_sport;  // if 0, empty; if 1, baseball; if 2, football; if 3,
                      // rugby

  // number of players for each sport on field
  int bb_players_num;
  int fb_players_num;
  int r_players_num;
  int r_games_in_row;  // how many games rugby has played (fairness)

  // number of games played by each sport
  int bb_games;
  int fb_games;
  int r_games;

  // stores threads when they play
  int bb_slot[18];
  int fb_slot[22];
  int r_slot[30];

  // last sport played
  int last_sport;
} field;

// the field we will use
field f_info;

void initializeField(field *f) {
  // initialize field struct
  f->current_sport = 0;
  f->bb_players_num = 0;
  f->fb_players_num = 0;
  f->r_players_num = 0;
  f->bb_games = 0;
  f->fb_games = 0;
  f->r_games = 0;
  f->last_sport = 0;
  f->r_games_in_row = 0;
  for (int i = 0; i < 18; i++) {
    f->bb_slot[i] = 0;
  }
  for (int i = 0; i < 22; i++) {
    f->fb_slot[i] = 0;
  }
  for (int i = 0; i < 30; i++) {
    f->r_slot[i] = 0;
  }
}

void *baseball(void *args) {
  bb_player *bb_info = (bb_player *)args;

  while (bb_info->times_played < 2) {
    int rand_nap_time = (rand() % (10 - 5 + 1)) + 5;
    sleep(rand_nap_time);

    pthread_mutex_lock(&bb_lock);

    bb_info->found_slot = 0;
    // If team is full, wait
    while (f_info.bb_players_num >= 18) {
      pthread_cond_wait(&bb_cond, &bb_lock);
    }
    // If team is not full, try to find a spot on the team
    // Once team becomes full, break this loop...
    if (f_info.bb_players_num < 18) {
      for (int i = 0; i < 18; i++) {
        if (f_info.bb_slot[i] == 0) {
          f_info.bb_slot[i] = bb_info->id;
          bb_info->field_slot_id = i;
          // printf("    Baseball thread %d found a slot at %d out
          // of 18.\n",bb_info->id, i+1);
          f_info.bb_players_num++;
          bb_info->found_slot = 1;
          if (f_info.bb_players_num == 18 && bb_info->found_slot == 1) {
            bb_info->starter = 1;
            printf("    Baseball team ready! Leader: %d\n", bb_info->id);
          }
          break;
        }
      }
    }

    pthread_mutex_unlock(&bb_lock);

    if (bb_info->starter == 1) {
      // Attempt to play
      pthread_mutex_lock(&lock);

      // Wait if the field isnt empty or if baseball just played
      while ((f_info.current_sport != 0) || f_info.last_sport == 1) {
        // printf("Waiting: current_sport=%d, last_sport=%d\n",
        // f_info.current_sport, f_info.last_sport);
        pthread_cond_wait(&f_cond, &lock);
      }
      // Once we break the while, wake up the sleeping threads and update sport
      f_info.current_sport = 1;

      // Play
      int gameTime = (rand() % (5 - 2 + 1)) + 2;
      printf("    Baseball game started!\n");
      for (int i = 0; i < 18; i++) {
        printf("    Baseball player %d is playing at position %d.\n",
               f_info.bb_slot[i], i + 1);
      }
      sleep(gameTime);

      pthread_mutex_lock(&bb_lock);
      // When a player is finished, release it from slot, update variables
      for (int i = 0; i < 18; i++) {
        f_info.bb_slot[i] = 0;
      }
      f_info.bb_players_num = 0;
      f_info.bb_games++;
      f_info.last_sport = 1;
      f_info.current_sport = 0;
      bb_info->starter = 0;
      f_info.r_games_in_row = 0;

      printf("    Baseball game ended!\n");

      // Wake up the other waiting baseball threads so they can start to form
      // team again
      pthread_cond_broadcast(&bb_cond);
      pthread_mutex_unlock(&bb_lock);

      // Wake up the other waiting teams
      pthread_cond_broadcast(&f_cond);

      pthread_mutex_unlock(&lock);
    } else {
      pthread_mutex_lock(&bb_lock);
      while (f_info.bb_players_num != 0) {
        pthread_cond_wait(&bb_cond, &bb_lock);
      }
      pthread_mutex_unlock(&bb_lock);
    }
    // Nap in between games, reset for each thread
    bb_info->field_slot_id = 0;
    bb_info->found_slot = 0;
    bb_info->times_played++;
    sleep((rand() % (4 - 2 + 1)) + 2);
  }
  // Exit after playing twice
  printf("Baseball player %d has played twice. Exiting.\n", bb_info->id);
  return;
}

void *football(void *args) {
  fb_player *fb_info = (fb_player *)args;

  while (fb_info->times_played < 2) {
    // fb_player * fb_info = (fb_player * ) args;

    int rand_nap_time = (rand() % (10 - 5 + 1)) + 5;
    sleep(rand_nap_time);

    pthread_mutex_lock(&fb_lock);
    fb_info->found_slot = 0;

    // Wait if the team is already full
    while (f_info.fb_players_num >= 22) {
      pthread_cond_wait(&fb_cond, &fb_lock);
    }
    // If field is not full, try to find a spot
    if (f_info.fb_players_num < 22) {
      for (int i = 0; i < 22; i++) {
        if (f_info.fb_slot[i] == 0) {
          f_info.fb_slot[i] = fb_info->id;
          fb_info->field_slot_id = i;

          // printf("        Football thread %d found a slot at %d out
          // of 22.\n",fb_info->id, i+1);
          f_info.fb_players_num++;
          fb_info->found_slot = 1;
          if (f_info.fb_players_num == 22 && fb_info->found_slot == 1) {
            fb_info->starter = 1;
            printf("        Football team ready! Leader: %d\n", fb_info->id);
          }
          break;
        }
      }
    }
    pthread_mutex_unlock(&fb_lock);

    if (fb_info->starter == 1) {
      // Attempt to play
      pthread_mutex_lock(&lock);

      // Wait if the field isnt empty or if football just played
      while ((f_info.current_sport != 0) || f_info.last_sport == 2) {
        pthread_cond_wait(&f_cond, &lock);
      }
      f_info.current_sport = 2;

      // Play
      int gameTime = (rand() % (5 - 2 + 1)) + 2;
      printf("        Football game started!\n");
      for (int i = 0; i < 22; i++) {
        printf("        Football player %d is playing at position %d.\n",
               f_info.fb_slot[i], i + 1);
      }
      sleep(gameTime);

      pthread_mutex_lock(&fb_lock);

      // When a player is finished, release it from slot
      for (int i = 0; i < 22; i++) {
        f_info.fb_slot[i] = 0;
      }
      f_info.fb_players_num = 0;

      f_info.fb_games++;
      f_info.last_sport = 2;
      f_info.current_sport = 0;
      fb_info->starter = 0;
      f_info.r_games_in_row = 0;

      printf("        Football game ended!\n");
      // Wake up the other waiting football threads so they can start to form
      // team again
      pthread_cond_broadcast(&fb_cond);
      pthread_mutex_unlock(&fb_lock);

      pthread_cond_broadcast(&f_cond);
      pthread_mutex_unlock(&lock);

    } else {
      pthread_mutex_lock(&fb_lock);
      // If there are still players on field, sleep until all threads have to
      // wake up
      while (f_info.fb_players_num != 0) {
        pthread_cond_wait(&fb_cond, &fb_lock);
      }
      pthread_mutex_unlock(&fb_lock);
    }
    // Nap in between games, reset for each thread
    fb_info->field_slot_id = 0;
    fb_info->found_slot = 0;
    fb_info->times_played++;
    sleep((rand() % (4 - 2 + 1)) + 2);
  }
  // Exit after playing twice
  printf("Football player %d has played twice. Exiting.\n", fb_info->id);
  return;
}


void *rugby(void *args) {
  r_player *r_info = (r_player *)args;

  while (r_info->times_played < 2) {
    // Nap before entering
    int rand_nap_time = (rand() % (10 - 5 + 1)) + 5;
    sleep(rand_nap_time);

    pthread_mutex_lock(&r_lock);

    // Wait if the field is full
    while (f_info.r_players_num >= 30) {
      pthread_cond_wait(&r_cond, &r_lock);
    }

    // Make a pair
    r_info->found_slot = 0;
    for (int i = 0; i < 30; i += 2) {
      if (f_info.r_slot[i] == 0) {
        // First player in the pair
        f_info.r_slot[i] = r_info->id;
        r_info->field_slot_id = i;
        f_info.r_players_num++;
        r_info->found_slot = 1;
        r_info->starter = 1;
        // Wait for second player
        while (f_info.r_slot[i + 1] == 0) {
          pthread_cond_wait(&r_cond, &r_lock);
        }
        printf("  Pair ready! Rugby leader: %d\n", r_info->id);
        break;
      } else if (f_info.r_slot[i + 1] == 0) {
        // Second player in the pair
        f_info.r_slot[i + 1] = r_info->id;
        r_info->field_slot_id = i + 1;
        f_info.r_players_num++;
        r_info->found_slot = 1;
        r_info->starter = 0;
        pthread_cond_broadcast(&r_cond);
        break;
      }
    }

    pthread_mutex_unlock(&r_lock);

    // Start the game if the player is a leader
    if (r_info->starter == 1) {
      pthread_mutex_lock(&lock);

      // Wait if football or baseball is on the field or too many rugby games have happened
      while (f_info.r_games_in_row >= 1 || f_info.current_sport == 1 || f_info.current_sport == 2) {
        pthread_cond_wait(&f_cond, &lock);
      }
      f_info.current_sport = 3;
      if (f_info.last_sport == 3 && f_info.current_sport == 3) {
        f_info.r_games_in_row++;
      }

      // Each pair has their own gameplay time
      int gameTime = (rand() % (5 - 2 + 1)) + 2;
      printf("  Rugby game started!\n");
      sleep(gameTime);
      
      // Check-in to show parallelism
      pthread_mutex_lock(&r_lock);
      printf("  Check-in: Rugby players currently playing: ");
      for (int i = 0; i < 30; i += 2) {
        if (f_info.r_slot[i] != 0 && f_info.r_slot[i + 1] != 0) {
          printf("[Player %d at position %d, player %d at position %d]", f_info.r_slot[i], i+1, f_info.r_slot[i + 1], i + 2);
        }
      }
      printf("\n");
      pthread_mutex_unlock(&r_lock);

      pthread_mutex_lock(&r_lock);
      // Clear pairs
      for (int i = 0; i < 30; i += 2) {
        if (f_info.r_slot[i] != 0 && f_info.r_slot[i + 1] != 0) {
          f_info.r_slot[i] = 0;
          f_info.r_slot[i + 1] = 0;
          f_info.r_players_num -= 2;
        }
      }

      // Reset other things
      if (f_info.r_players_num == 0) {
        f_info.last_sport = 3;
        f_info.current_sport = 0;
      }

      f_info.r_games++;
      f_info.last_sport = 3;
      f_info.current_sport = 0;
      r_info->starter = 0;
      r_info->times_played++;

      printf("  Rugby game ended!\n");

      // Wake up waiting rugby players
      pthread_cond_broadcast(&r_cond);
      pthread_mutex_unlock(&r_lock);

      // Allow other sports onto field
      pthread_cond_broadcast(&f_cond);
      pthread_mutex_unlock(&lock);
    }

    // Nap between games
    r_info->field_slot_id = 0;
    r_info->found_slot = 0;
    sleep((rand() % (4 - 2 + 1)) + 2);
  }

  // Exit after playing twice
  printf("Rugby player %d has played twice. Exiting.\n", r_info->id);
  return NULL;
}



int main(void) {
  // read seed value from text file
  char read[SIZE];
  FILE *fp = fopen("seed.txt", "r");

  if (fp == NULL) {
    printf("File didn't open!");
  }
  printf("Reading seed value: ");
  while (fgets(read, SIZE, fp) != NULL) {
    printf(" %s ", read);
  }
  fclose(fp);
  int seed = atoi(read);
  printf("\nSeed as int: %d \n", seed);
  // seed srand
  srand(seed);

  bb_player *bb_info = (bb_player *)malloc(sizeof(bb_player) * 36);
  fb_player *fb_info = (fb_player *)malloc(sizeof(fb_player) * 44);
  r_player *r_info = (r_player *)malloc(sizeof(r_player) * 60);

  initializeField(&f_info);

  // make threads for each player
  for (int i = 0; i < 36; i++) {
    bb_info[i].id = i + 1;
    bb_info[i].nthreads = 36;

    if (pthread_create(&bb_info[i].ptid, NULL, baseball, (void *)&bb_info[i]) !=
        0) {
      fprintf(stderr, "Unable to create baseball thread!\n");
      exit(1);
    }
  }
  for (int i = 0; i < 44; i++) {
    fb_info[i].id = i + 1;
    fb_info[i].nthreads = 44;
    if (pthread_create(&fb_info[i].ptid, NULL, football, (void *)&fb_info[i]) !=
        0) {
      fprintf(stderr, "Unable to create football thread!\n");
      exit(1);
    }
  }
  for (int i = 0; i < 60; i++) {
    r_info[i].id = i + 1;
    r_info[i].nthreads = 60;
    if (pthread_create(&r_info[i].ptid, NULL, rugby, (void *)&r_info[i]) != 0) {
      fprintf(stderr, "Unable to create rugby thread!\n");
      exit(1);
    }
  }

  // join threads
  for (int i = 0; i < 36; i++) {
    pthread_join(bb_info[i].ptid, NULL);
  }

  for (int i = 0; i < 44; i++) {
    pthread_join(fb_info[i].ptid, NULL);
  }

  for (int i = 0; i < 60; i++) {
    pthread_join(r_info[i].ptid, NULL);
  }

  free(bb_info);
  free(fb_info);
  free(r_info);

  return 0;
}