#ifndef COSMOSIS_C_DATABLOCK_H
#define COSMOSIS_C_DATABLOCK_H

/*
  DATABLOCK_STATUS is type of the status codes returned by the C
  datablock interface functions that return status codes. In all cases
  the user can rely upon 0 indicating success, and nonzero indicating
  some type of failure. The exact numeric values of codes indicating
  failure should not be relied upon.
 */
typedef enum {
  DBS_SUCCESS = 0,
  DBS_DATABLOCK_NULL,
  DBS_SECTION_NAME_NULL,
  DBS_SECTION_NOT_FOUND,
  DBS_NAME_NULL,
  DBS_NAME_NOT_FOUND,
  DBS_NAME_ALREADY_EXISTS,
  DBS_VALUE_NULL,
  DBS_WRONG_VALUE_TYPE,
  DBS_MEMORY_ALLOC_FAILURE
} DATABLOCK_STATUS;

typedef void c_datablock;
c_datablock* make_c_datablock(void);

/*
  bool c_datablock_has_section(c_datablock const* s, const char* name);
  bool c_datablock_has_value(c_datablock const* s, const char* section, const char* name);
  int c_datablock_num_sections(....);
  DATABLOCK_STATUS c_datablock_get_section_name(..., int isection);
*/

DATABLOCK_STATUS destroy_c_datablock(c_datablock* s);

/*
  Return 0 if a double named 'name' is found. We do no conversions of type.
  1: section not found.
  2: name not found
  3: wrong type
  4: memory allocation failure.
  5: section name is null
  6: name is null, section is not null
  7: val is null.
  8: s is null.

  If the return status is nonzero, the value written into 'val' is
  not defined.
*/
DATABLOCK_STATUS
c_datablock_get_double(c_datablock const* s, const char* name, double* val);

DATABLOCK_STATUS
c_datablock_get_int(c_datablock const* s, const char* name, int* val);

/* Only scalars have default in the C and Fortran interfaces. */
DATABLOCK_STATUS
c_datablock_get_double_default(c_datablock const* s, const char* name, 
			       double* val, double dflt);


/*
  Return 0 if the put worked, and nonzero to indicate failure.
  1: name already exists
  2: memory allocation failure
*/
DATABLOCK_STATUS
c_datablock_put_double(c_datablock* s, const char* name, double val);

DATABLOCK_STATUS
c_datablock_put_int(c_datablock* s, const char* name, int val);

/*
  Return 0 if the put worked, and nonzero to indicate failure.
  1: name does not already exist.
  2: memory allocation failure.
  3: replace of wrong type.
*/
DATABLOCK_STATUS
c_datablock_replace_double(c_datablock* s, const char* name, double val);


#if 0
/* Return 0 if the put worked, and nonzero to indicate failure */
DATABLOCK_STATUS
c_datablock_get_double_array_1d(c_datablock const* s, const char* name,
				double** array, int* size);

DATABLOCK_STATUS
c_datablock_get_double_array_1d_preallocated(c_datablock const* s, const char* name,
					     double* array,
					     int* size,
					     int maxsize);

DATABLOCK_STATUS
c_datablock_put_double_array_1d(c_datablock* s, const char* name,
				double* array, int sz);
#endif

#endif
