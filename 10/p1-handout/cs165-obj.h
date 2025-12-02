#define SYSTEM_SKIP 0
#define OBJ_REM 1
#define OBJ_VAL 2
#define OBJ_LEN 3
#define OBJ_STR 4
#define OBJ_OBJ 5
#define OBJ_DIS 6
#define OBJ_ADD 7

#define MAX_STRING 12
#define MAX_OBJECTS 10

#define TYPE_A 1
#define TYPE_B 2
#define OTHER 0


#define USAGE "cs165-obj cmd-file output-file"


typedef struct system {
  void *object[MAX_OBJECTS];
  int type[MAX_OBJECTS];
} system_t;

typedef struct A {
  int len;
  int val;
  char string[0];
} A_t;

typedef struct B {
  A_t *obj;
  int (*display)( struct B *b, int index );
} B_t;


#define SPACE_CHAR( str,i )						\
	(( (str)[(i)] == ' ' ) || ( (str)[(i)] == '\t' ) || ( (str)[(i)] == '\n' )) 


static inline int strct( char *str, int len )
{
  int ct = 0;
  int instr = 0;
  int i;

  for ( i = 0; i < len; i++ ) {
    if (( !SPACE_CHAR( str, i )) && instr );
    else if (( !SPACE_CHAR( str, i )) && !instr ) { instr = 1, ct++; }
    else if ( SPACE_CHAR( str, i )) { instr = 0; }
    else { return -1; }
  }

  return ct;
}

extern int make_obj( char *index, char *type );
extern int remove_obj( char *index );
extern int set_obj_value( char *index, char *val );
extern int set_obj_length( char *index, char *len );
extern int set_obj_string( char *index, char *string );
extern int set_obj_object( char *index, char *obj_index );
extern int display_obj( char *index );
