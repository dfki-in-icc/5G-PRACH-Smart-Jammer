#ifndef FLEXRIC_CONFIGURATION_FILE_H
#define FLEXRIC_CONFIGURATION_FILE_H 

typedef struct {
  char conf_file[128];
  char libs_dir[128];
} args_t;

char* get_near_ric_ip(args_t);
int is_regular_file(const char *path);
void print_usage(const char*);
int parse_args(int argc, char** argv, args_t* args);

#endif


