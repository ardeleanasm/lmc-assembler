#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#ifdef DEBUG
#define debug_print(fmt, ...)\
  do { fprintf(stderr, "%s:%d:%s(): "fmt, __FILE__, \
               __LINE__, __func__, __VA_ARGS__);} while(0)
#else
#define debug_print

#endif


#define LINE_BUFFER_MAX_SIZE 512
#define TOKEN_MAX_SIZE 32
#define NUMBER_OF_INSTRUCTIONS 12

typedef enum
{
  HLT = 0,
  ADD,
  SUB,
  STA,
  RESERVED,
  LDA,
  BRA,
  BRZ,
  BRP,
  INP = 901,
  OUT = 902,
  OTC = 922,
  DAT = 999
}mnemonic_t;


typedef struct
{
  char *mnemonic_name;
  mnemonic_t mnemonic_code;
}mnemonic_map_t;


typedef struct
{
  size_t line_number;
  char label[16];
  mnemonic_t mnemonic;
  union
  {
    int16_t addr;
    char var[16];
  }address; 
}instruction_t;

static const mnemonic_map_t MNEMONIC_MAP[] = {
  {.mnemonic_name = "HLT", .mnemonic_code = HLT},
  {.mnemonic_name = "ADD", .mnemonic_code = ADD},
  {.mnemonic_name = "SUB", .mnemonic_code = SUB},
  {.mnemonic_name = "STA", .mnemonic_code = STA},
  {.mnemonic_name = "LDA", .mnemonic_code = LDA},
  {.mnemonic_name = "BRA", .mnemonic_code = BRA},
  {.mnemonic_name = "BRZ", .mnemonic_code = BRZ},
  {.mnemonic_name = "BRP", .mnemonic_code = BRP},
  {.mnemonic_name = "INP", .mnemonic_code = INP},
  {.mnemonic_name = "OUT", .mnemonic_code = OUT},
  {.mnemonic_name = "OTC", .mnemonic_code = OTC},
  {.mnemonic_name = "DAT", .mnemonic_code = DAT},

};


static int tokenize(char **tokens_array, size_t n, size_t line_number)
{
  int i = 0;
  int j = 0;
  instruction_t instruction;

  memset(&instruction, 0, sizeof(instruction));
  
  instruction.line_number = line_number;

  //TODO: Do it another way
  for (i = 0; i < n; i++) {
    char *token = tokens_array[i];
    for (j = 0; j < NUMBER_OF_INSTRUCTIONS; j++) {
      if (strncmp(token, MNEMONIC_MAP[j].mnemonic_name, strlen(token)) == 0) {
        instruction.mnemonic = MNEMONIC_MAP[j].mnemonic_code;
        printf("%d %s\n", MNEMONIC_MAP[j].mnemonic_code, token);
        break;
      }
      
    }
    if (i==0) {
      if ((token[0]>='a' && token[0]<=z) || (token[0]>='A' && token[0]<='Z')){
        // it's a label
      }
      else {
        //is invalid to start with value
      }
      //it's a label
    }
    else {
      if ((token[0]>='a' && token[0]<=z) || (token[0]>='A' && token[0]<='Z')){
        // it's a label of an address
      }
      else {
        // it's a value
      }
    }
    
    
    
    
  }
  return 0;
}


static int parse_line(char *line, int line_number)
{

  
  if (*line == '/' && *(line+1) == '/') {
    return 0;
  }


  
  char token[TOKEN_MAX_SIZE];

  memset(token, 0, TOKEN_MAX_SIZE * sizeof(char));

  
  char **tokens_array = malloc(sizeof(char*));
  if (tokens_array == NULL) {
    fprintf(stderr, "Could not allocate memory for tokens_array\n");

    return -2;
  }
  
  size_t tokens_array_length = 0;
  size_t i = 0;
  while (*line) {
    
    if (*line == '/' && *(line+1) == '/') {
      break;
    }
    
    if (*line != ' ' && *line != '\n' && *line != '\t') {
      strncpy(token+i,line,1);   
      i++;   
    }
    else {
      if (*token) {

        tokens_array[tokens_array_length] = malloc(strlen(token)+1);
        if (tokens_array[tokens_array_length] == NULL) {
          fprintf(stderr, "Could not allocate memory for token in tokens_array\n");
          return -2;
        }
        memset(tokens_array[tokens_array_length],0,strlen(token)+1);
        strncpy(tokens_array[tokens_array_length], token, strlen(token));
        tokens_array_length++;
        char **tokens_array_buffer = realloc(tokens_array, tokens_array_length*sizeof(char)+1);
        if (tokens_array_buffer == NULL) {
          free(tokens_array);
          fprintf(stderr, "Could not reallocate memory for tokens_array\n");
          return -2;
        }
        tokens_array = tokens_array_buffer;
        //reset token
        memset(token, 0, strlen(token));

        i = 0;
      }
      
       
    }
    line++;
  }
  tokenize(tokens_array, tokens_array_length, line_number);
  i = 0;
  for (; i < tokens_array_length; i++) {
    free(tokens_array[i]);
  }
  
  free(tokens_array);

  return 0;
}


int main(int argc, char **argv)
{
  char *file="test.asm";
  FILE *fp = fopen(file,"r");
  if (fp == NULL) {
    fprintf(stderr, "File opening failed!\n");
    return -1;
  }

  char line_buffer[LINE_BUFFER_MAX_SIZE];
  size_t i = 0;
  while (fgets(line_buffer, LINE_BUFFER_MAX_SIZE, fp)) {
    if (parse_line(line_buffer,i)) {
      fclose(fp);
      exit(1);
    }
    i++;
  }

  fclose(fp);
  return 0;
}

