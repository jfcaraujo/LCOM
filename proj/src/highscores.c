#include "highscores.h"

void (print_string)(Game* game, int x, int y, char * string, unsigned int size){
  xpm_image_t letter;
  for (unsigned int i = 0; i < size; i++){
    switch(string[i]){
      case 'a':{
        letter=game->A;
        break;}
      case 'b':{
        letter=game->B;
        break;}
      case 'c':{
        letter=game->C;
        break;}
      case 'd':{
        letter=game->D;
        break;}
      case 'e':{
        letter=game->E;
        break;}
      case 'f':{
        letter=game->F;
        break;}
      case 'g':{
        letter=game->G;
        break;}
      case 'h':{
        letter=game->H;
        break;}
      case 'i':{
        letter=game->I;
        break;}
      case 'j':{
        letter=game->J;
        break;}
      case 'k':{
        letter=game->K;
        break;}
      case 'l':{
        letter=game->L;
        break;}
      case 'm':{
        letter=game->M;
        break;}
      case 'n':{
        letter=game->N;
        break;}
      case 'o':{
        letter=game->O;
        break;}
      case 'p':{
        letter=game->P;
        break;}
      case 'q':{
        letter=game->Q;
        break;}
      case 'r':{
        letter=game->R;
        break;}
      case 's':{
        letter=game->S;
        break;}
      case 't':{
        letter=game->T;
        break;}
      case 'u':{
        letter=game->U;
        break;}
      case 'v':{
        letter=game->V;
        break;}
      case 'w':{
        letter=game->W;
        break;}
      case 'x':{
        letter=game->X;
        break;}
      case 'y':{
        letter=game->Y;
        break;}
      case 'z':{
        letter=game->Z;
        break;}
      case ' ':{
        letter=game->Space;
        break;}
      case '1':{
        letter=game->n1;
        break;}
      case '2':{
        letter=game->n2;
        break;}
      case '3':{
        letter=game->n3;
        break;}
      case '4':{
        letter=game->n4;
        break;}
      case '5':{
        letter=game->n5;
        break;}
      case '6':{
        letter=game->n6;
        break;}
      case '7':{
        letter=game->n7;
        break;}
      case '8':{
        letter=game->n8;
        break;}
      case '9':{
        letter=game->n9;
        break;}
      case '0':{
        letter=game->n0;
        break;}
      case ':': {
        letter = game->n_two_dots;
        break; }
      default:{
        letter = game->Space;
        break;}
    }
    print_sprite(x+(21*i),y,letter);
  }
}

void (save_highscores)(Game* game)
{
  if(game->highscoresSize == 0) return;
  FILE* fp;
  fp = fopen("/home/lcom/labs/proj/src/highscores.txt", "w");

  fprintf(fp, "%d", game->highscoresSize);

  for(unsigned int i = 0; i < game->highscoresSize; i++) {

    HighScore* highscore = &game->highscores[i];

    fprintf(fp, "\n%d %d %d %d %d %d %d %d", highscore->stats.current_round, highscore->stats.score, highscore->date.hour, highscore->date.minute, highscore->date.day, highscore->date.month, highscore->date.year,  highscore->playerName.size);
    fprintf(fp, "\n%s", highscore->playerName.name);
  }

  fclose(fp);
}

void (load_highscores)(Game* game)
{
  FILE* fp;

  fp = fopen("/home/lcom/labs/proj/src/highscores.txt", "r");

  if (fp == NULL) 
    return;

  fscanf(fp, "%d ", &game->highscoresSize);
  char buffer[100];

  for(unsigned int i = 0; i < game->highscoresSize; i++) {

    HighScore* highscore = &game->highscores[i];

    fscanf(fp, "%d %d %d %d %d %d %d %d", &highscore->stats.current_round, &highscore->stats.score,&highscore->date.hour, &highscore->date.minute, &highscore->date.day, &highscore->date.month, &highscore->date.year, &highscore->playerName.size);
    
    fgets(buffer, 100, fp);
    fgets(highscore->playerName.name, highscore->playerName.size + 1, fp);
  }
  
  fclose(fp);
}

int (check_new_highscore)(Game* game)
{
  for(unsigned int i = 0; i < game->highscoresSize; i++) {
    if((game->stats->score) > (game->highscores[i].stats.score))
      return i;
  }
    
  return game->highscoresSize;
}

void(write_new_highscore)(Game* game, unsigned int position)
{
  Date date = getCurrentDate();

  if (game->highscoresSize == position) {

    game->highscores[position].date = date;
    game->highscores[position].stats = *(game->stats);
    game->highscores[position].playerName.size = game->name->size;
    strcpy(game->highscores[position].playerName.name, game->name->name);

    game->highscoresSize++;
  }
  else {
    for (unsigned int i=game->highscoresSize;i>position;i--){
      if (i==3) continue;
      game->highscores[i].date = game->highscores[i-1].date;
      game->highscores[i].stats = game->highscores[i-1].stats;
      game->highscores[i].playerName.size = game->highscores[i-1].playerName.size;
      strcpy(game->highscores[i].playerName.name,game->highscores[i-1].playerName.name);
    }
    game->highscores[position].date = date;
    game->highscores[position].stats = *(game->stats);
    game->highscores[position].playerName.size = game->name->size;
    strcpy(game->highscores[position].playerName.name, game->name->name);

    if (game->highscoresSize < 3) game->highscoresSize++;
  }
}

void (print_date)(Game* game, Date* date, int x, int y)
{
  char number[2];
  if ((get_number_length((int) date->month)+(get_number_length((int) date->day)))==4)
    x=545;

  //Day
  sprintf(number, "%d", date->day);
  print_string(game, x, y, number, get_number_length(date->day));

  //Slash
  x += 21 * get_number_length(date->day);
  print_sprite(x,y,game->slash);

  //Month
  x += 11;
  sprintf(number, "%d", date->month);
  print_string(game, x, y, number, get_number_length((int) date->month));

  //Slash
  x += 21 * get_number_length(date->month);
  print_sprite(x,y,game->slash);

  //Year
  x += 11;
  sprintf(number, "%d", date->year);
  print_string(game, x, y, number, get_number_length((int) date->year));
  
  //Trace
  x += 21 * get_number_length((int) date->year);
  print_sprite(x,y,game->minus);

  //Hour
  x += 11;
  sprintf(number, "%d", date->hour);
  if (get_number_length((int) date->hour)==1){
    number[1] = number[0];
    number[0] = '0';
  }

  print_string(game, x, y, number, 2);

  //Two dots
  x += 21 * 2; 
  print_sprite(x,y,game->n_two_dots);

  //Minute
  x += 11;
  sprintf(number, "%d", date->minute);
  if (get_number_length((int) date->minute)==1){
    number[1]=number[0];
    number[0] = '0';
  }

  print_string(game, x, y, number, 2);
}

void (print_highscores)(Game* game)
{
  char score[12];
  char final_round[4];
  
  //Initial coordinates
  int x = 100;
  int y = 185;

  for(unsigned int i = 0; i < game->highscoresSize; i++) {
    y += 90;
    x = 120;

    HighScore* highscore = &game->highscores[i];

    sprintf(score, "%d", highscore->stats.score);
    sprintf(final_round, "%d", highscore->stats.current_round);

    x -= 10 * (highscore->playerName.size);
    if (x < 0) x = 0;
    print_string(game, x, y, highscore->playerName.name, highscore->playerName.size);

    x = 390 - 10 * (get_number_length(highscore->stats.current_round) + get_number_length(highscore->stats.score));

    if (x < 280) x = 280;
    print_string(game, x, y, final_round, get_number_length(highscore->stats.current_round));
    if (highscore->stats.current_round>9) x+=21;
    if (highscore->stats.current_round>99) x+=21;
    print_sprite(x + 21, y, game->minus);
    print_string(game, x + 32, y, score, get_number_length(highscore->stats.score));

    x = 565;
    print_date(game, &(highscore->date), x, y);
  }
}
