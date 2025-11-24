#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs165-obj.h"

FILE *ifile, *ofile;


#if 0
int test()
{
  return 0;
}


int main(int argc, char *argv[])
{
  A_t *a, *a2;
  B_t *b, *b2;
  char string[] = "ffooooooooooooooooooooooooo";
  
  a = (A_t *)malloc(sizeof(A_t));
  b = (B_t *)malloc(sizeof(B_t));

  a->len = 0;

  b->a = a;
  b->fn = test;

  // void *realloc(void *ptr, size_t size);
  a2 = realloc(a, sizeof(A_t)+sizeof(string));
  printf("a: 0x%x; a2: 0x%x; b->a: 0x%x\n", a, a2, b->a);
  
  // create B's to see if any are inside of the space of the original A
  for ( int i=0; i < 100; i++) {
    b2 = (B_t *)malloc(sizeof(B_t));
    printf("b: 0x%x; b2: 0x%x; b->a: 0x%x\n", b, b2, b->a);

    if ( b->a == b2 ) {
      fprintf(ifile, "new b is located at a\n");
    }
  }
}
#endif

/*****************************
 notes:

 (1) strncpy attack - string at end
 keep length separate from string value

 (2) change pointer - write at that location
     write len
     write from b

 (3) 

 op:
 add
 change len, val, or string

args: op, id, type, val
if len or string are longer, then need to realloc
  


 *****************************/


system_t objects;



/*********************************

Command processing:
Find command and validate arg count
Find user object and launch command
For each user cmd, parse spec to find args
     Find sub-object (index and qtype)
     Apply to sub object(s)
     And apply effect to user object

**********************************/

int find_command( char *cmdbuf, int len )
{
  int ct = strct( cmdbuf, len );

  switch( cmdbuf[0] ) {
  case 'a':   // add object 
    if ( ct != 3 ) {
      return -1;
    }
    return OBJ_ADD;
    break;
  case 'r':   // remove object
    if ( ct != 2 ) {
      return -1;
    }
    return OBJ_REM;
    break;
  case 'v':   // set value
    if ( ct != 3 ) {
      return -1;
    }
    return OBJ_VAL;
    break;
  case 'l':   // set length
    if ( ct != 3 ) {
      return -1;
    }
    return OBJ_LEN;
    break;
  case 's':   // set string
    if ( ct != 3 ) {
      return -1;
    }
    return OBJ_STR;
    break;
  case 'o':   // set object
    if ( ct != 3 ) {
      return -1;
    }
    return OBJ_OBJ;
    break;
  case 'd':   // display
    if ( ct != 2 ) {
      return -1;
    }
    return OBJ_DIS;
    break;
  case '#':   // comment lines
  case '%':
  case '\n':
    return SYSTEM_SKIP;
    break;
  }

  return -1;
}


int apply_command( int cmd, char *args )
{
  char arg1[MAX_STRING], arg2[MAX_STRING];
  int res = 0;
  
  switch( cmd ) {
  case OBJ_ADD:
    if (( sscanf( args, "%s %s", arg1, arg2 )) != 2 ) {
	return -1;
    }
    res = make_obj( arg1, arg2 );
    fprintf( ofile, "OBJ ADD %s %s: result %s\n", arg1, arg2, (( res < 0 ) ? "FAIL" : "PASS") );
    return res;
    break;
  case OBJ_REM:
    if (( sscanf( args, "%s", arg1 )) != 1 ) {
	return -1;
    }
    res = remove_obj( arg1 );
    fprintf( ofile, "OBJ REMOVE %s : result %s\n", arg1, (( res < 0 ) ? "FAIL" : "PASS") );
    return res;
    break;
  case OBJ_VAL:
    if (( sscanf( args, "%s %s", arg1, arg2 )) != 2 ) {
	return -1;
    }
    res = set_obj_value( arg1, arg2 );  
    fprintf( ofile, "OBJ VALUE %s %s : result %s\n", arg1, arg2, (( res < 0 ) ? "FAIL" : "PASS") );
    return res;
    break;
  case OBJ_LEN:
    if (( sscanf( args, "%s %s", arg1, arg2 )) != 2 ) {
	return -1;
    }
    res = set_obj_length( arg1, arg2 );  
    fprintf( ofile, "OBJ LENGTH %s %s : result %s\n", arg1, arg2, (( res < 0 ) ? "FAIL" : "PASS") );
    return res;
    break;
  case OBJ_STR:
    if (( sscanf( args, "%s %s", arg1, arg2 )) != 2 ) {
	return -1;
    }
    res = set_obj_string( arg1, arg2 );
    fprintf( ofile, "OBJ STRING %s %s : result %s\n", arg1, arg2, (( res < 0 ) ? "FAIL" : "PASS") );
    return res;
    break;
  case OBJ_OBJ:
    if (( sscanf( args, "%s %s", arg1, arg2 )) != 2 ) {
	return -1;
    }
    res = set_obj_object( arg1, arg2 );
    fprintf( ofile, "OBJ OBJECT %s %s : result %s\n", arg1, arg2, (( res < 0 ) ? "FAIL" : "PASS") );
    return res;
    break;
  case OBJ_DIS:
    if (( sscanf( args, "%s ", arg1 )) != 1 ) {
	return -1;
    }
    res = display_obj( arg1 );
    fprintf( ofile, "OBJ DISPLAY %s: result %s\n", arg1, (( res < 0 ) ? "FAIL" : "PASS") );
    return res;
    break;
  default:
    fprintf( ofile, "Err: apply_command: Invalid command\n");
    return -1;
    break;
  }
}



/*********************************
  
  Helper functions

**********************************/

int helper_process_index(char* index)
{
  int processed_index = atoi(index);
  if (processed_index < 0 || processed_index >= MAX_OBJECTS)
    return -1; // fail
  return processed_index;
}

int helper_get_type(char *index)
{
  int processed_index = helper_process_index(index);
  return objects.type[processed_index];
}


/*********************************
  
  Display functions

**********************************/

int a_display( A_t *a, int ind )
{
  // overview
  fprintf( ofile, "\nObject %d of type A\n", ind);

  // fields
  fprintf( ofile, "field val: %d\n", a->val );
  fprintf( ofile, "field len: %d\n", a->len );
  fprintf( ofile, "field string: %s\n", a->string );
	 
  return 0;
}


int b_display( B_t *b, int ind )
{
  // overview
  fprintf( ofile, "\nDisplay object %d of type B\n", ind);

  if ( !b ) {
      fprintf( ofile, "\nNo such object %d of type B\n", ind);
      return -1;
  }
  
 // fields
  fprintf( ofile, "field obj: %s\n", ((b->obj != NULL) ? "PRESENT" : "NONE" ));

  // get A's index and display
  for ( int i = 0; i < MAX_OBJECTS; i++ ) {
    if (( b->obj != NULL ) && ( b->obj == (A_t *)objects.object[i] )) {
      fprintf( ofile, "Displaying obj index: %d\n...", i );
      a_display( b->obj, i );
      return 0;
    }
  }

  // print no match
  fprintf( ofile, "No match for object A\n");

  return 0;
}

/*********************************
  
  Command processing 

**********************************/

int display_obj( char *index )
{
  int ind = atoi(index);
  A_t *a = NULL;
  B_t *b = NULL;
  int ty = 0;
  
  /*** TASK: Convert index to object and object type ***/
  ty = helper_get_type(index);
  // printf("\ntype: %d\n", ty);
  // display in type specific way
  switch( ty ) {
  case TYPE_A:
    a = (A_t *)objects.object[ind];
    return(a_display(a, ind));
    break;
  case TYPE_B:
    b = (B_t *)objects.object[ind];
    a = ((B_t *)objects.object[ind])->obj;
    return b->display(b, ind);
    //return b_display(b, ind); OLD CODE
    break;
  default:
    return -2;
  }

  return -3;  // should not reach
}


int set_obj_object( char *index, char *obj_index )
{

  /*** TASK: Implement ***/
  
  int parent_id = atoi(index);
  int child_id = atoi(obj_index);

  if (parent_id < 0 || parent_id >= MAX_OBJECTS || 
      child_id < 0 || child_id >= MAX_OBJECTS) {
    return -1;
  }

  void *parent_ptr = objects.object[parent_id];
  int parent_type = objects.type[parent_id];

  void *child_ptr = objects.object[child_id];
  int child_type = objects.type[child_id];

  if (parent_ptr == NULL || child_ptr == NULL) {
    return -1;
  }
  
  if (parent_type != TYPE_B || child_type != TYPE_A) {
    return -1;
  }
  
  B_t *parent_b = (B_t *)parent_ptr;
  A_t *child_a = (A_t *)child_ptr;

  parent_b->obj = child_a;
  
  return 0;
}


int set_obj_string( char *index, char *string ) // Even = Tyler
{
  /*** TASK: Implement ***/

  int processed_index = helper_process_index(index);
  
  // Assign val depending on type
  if(helper_get_type(index) == TYPE_A) { 
    strncpy(((A_t *)objects.object[processed_index])->string, string, strlen(string));
  } else if (helper_get_type(index) == TYPE_B) { 
    strncpy(((B_t *)objects.object[processed_index])->obj->string, string, strlen(string));
  } else {  
    return -1; 
  }

  return 0; 
}


int set_obj_length( char *index, char *len )
{
  /*** TASK: Implement ***/

  int i = atoi(index);
  int lenInt = atoi(len);
  
  if (i >= MAX_OBJECTS) {
      return -1;
  }

  void *obj_ptr = objects.object[i];

  if (obj_ptr == NULL) {
      return -1;
  }

  int obj_type = helper_get_type(index);

  if (obj_type == TYPE_A) {
    A_t *a = (A_t *)obj_ptr;

    int current_len = a->len;
    a->len = lenInt;

    if (lenInt > current_len) {
      A_t *new_a = realloc(a, sizeof(A_t) + lenInt);
      
      if (new_a == NULL) {
        a->len = current_len;
        return -1;
      }
      
      objects.object[i] = new_a;
    }
    return 0;
  } else {
    return -1;
  }
}

int set_obj_value( char *index, char *val ) // Even = Tyler
{
  /*** TASK: Implement ***/


  int processed_index = helper_process_index(index);
  
  // Assign val depending on type
  if(helper_get_type(index) == TYPE_A) {  // is TYPE_A
    ((A_t *)objects.object[processed_index])->val = atoi(val);
  } else if (helper_get_type(index) == TYPE_B) {  // is TYPE_B
    ((B_t *)objects.object[processed_index])->obj->val = atoi(val);
  } else { 
    return -1; 
  }

  return 0;  
}


int remove_obj( char *index )
{
/*** TASK: Implement ***/
  int i = atoi(index);

  if (i >= MAX_OBJECTS) {
    return -1;
  }

  if (objects.object[i] != NULL) {
    free(objects.object[i]);
    objects.object[i] = NULL;
    objects.type[i] = 0;
  }

  return 0;
}


int make_obj( char *index, char *type )
{
  /*** TASK: Implement ***/

  int processed_index = helper_process_index(index);

  if(strcmp(type, "A") == 0) {
    A_t *a = (A_t *)malloc(sizeof(A_t)); 

    a->len = 0; 
    a->val = 0;
    a->string[0] = '\0';

    objects.object[processed_index] = a;
    objects.type[processed_index] = TYPE_A;
  }
  else if (strcmp(type, "B") == 0) {  
      B_t *b = (B_t *)malloc(sizeof(B_t));

      b->obj = NULL;  
      b->display = b_display;

      objects.object[processed_index] = b;
      objects.type[processed_index] = TYPE_B;
  }
  else {
      return -1;  
  }
  return 0;
}


/*********************************

User API: op <user_index> <domain_index> <args>

Commands: 
make_obj <index> <type>
remove_obj <index>
set_obj_value <index> <val>
set_obj_length <index> <len>
set_obj_string <index> <string>
set_obj_object <index> <obj-index>
display_obj <index>

**********************************/


int apply_user_command( int cmd, char *cmdstr )
{
  char *args;
  int res = 0;
  
  // move cmdstr to args

  args = cmdstr;
  while ( (++args)[0] != ' ' );
  args++;

  // apply the command
	  
  res = apply_command( cmd, args );

  return res;
}


int main( int argc, char *argv[] )
{
        FILE *file;
	char *line = (char *)NULL;
	int cmd;
	char *cmdstr;
	int res;
	size_t len = MAX_STRING;

	// check usage
	if ( argc != 3 ) {
	  printf( "Main: Err: Incorrect usage: %s\n", USAGE );
	}

	// open fds
	if (( file = fopen( argv[1], "r" )) == NULL ) {
	  exit(-1);
	}

	ifile = file;

	if (( file = fopen( argv[2], "w+" )) == NULL ) {
	  exit(-1);
	}

	ofile = file;
	 
	// get input
	line = (char *)malloc( len );
	while (( res = getline( &line, &len, ifile )) >= 0 ) {
	  
	  cmdstr = line;
	  
	  // determine command from command string (first arg in line)

	  cmd = find_command( cmdstr, strlen( cmdstr ));
	  if ( cmd == SYSTEM_SKIP ) {
	    continue;
	  }	  
	  if ( cmd < 0 ) {
		exit( cmd );
	  }

	  apply_user_command( cmd, cmdstr );
	}

	exit( 0 );
}
