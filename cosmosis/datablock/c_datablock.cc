#include <assert.h>
#include <complex.h>
#include <string.h>

#include "datablock.hh"
#include "section.hh"
#include "entry.hh"
#include "c_datablock.h"

using cosmosis::DataBlock;
using cosmosis::Section;
using cosmosis::Entry;
using cosmosis::complex_t;
using std::string;
using std::vector;

extern "C"
{
  // This seems to be the appropriate incantation to export this
  extern const int cosmosis_enum_size = sizeof(datablock_type_t);

  c_datablock* make_c_datablock(void)
  {
    return new cosmosis::DataBlock();
  }

  _Bool c_datablock_has_section(c_datablock const* s, const char* name)
  {
    if (s == nullptr || name == nullptr) return false;
    DataBlock const* p = static_cast<DataBlock const*>(s);
    return p->has_section(name);
  }

  // This is not for public use!  
  // If you want to delete a section you are 
  // probably doing something wrong.
  int c_datablock_delete_section(c_datablock * s, const char * section)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    auto p = static_cast<DataBlock *>(s);
    return p->delete_section(section);

  }

  int c_datablock_num_sections(c_datablock const* s)
  {
    if (s == nullptr) return -1;
    auto p = static_cast<DataBlock const*>(s);
    return p->num_sections();
  }

  int c_datablock_num_values(
    c_datablock const* s, const char* section)
  {
    if (s == nullptr) return -1;
    if (section == nullptr) return -1;
    DataBlock const* p = static_cast<DataBlock const*>(s);
    return p->num_values(section);
  }

  _Bool c_datablock_has_value(c_datablock const* s,
                                         const char* section,
                                         const char* name)
  {
    if (s == nullptr) return false;
    if (section == nullptr) return false;
        if (s == nullptr) return false;
    if (section == nullptr) return false;
if (name == nullptr) return false;
    DataBlock const* p = static_cast<DataBlock const*>(s);
    return p->has_val(section, name);
  }

  int c_datablock_get_array_length(c_datablock const* s,
                                   const char* section,
                                   const char* name)
  {
    if (s==nullptr || section==nullptr || name==nullptr) return -1;
    DataBlock const* p = static_cast<DataBlock const*>(s);
    return p->get_size(section, name);
  }

  const char* c_datablock_get_section_name(c_datablock const* s, int i)
  {
    if (i < 0) return nullptr;
    auto n = static_cast<size_t>(i);
    DataBlock const* p = static_cast<DataBlock const*>(s);
    if (n >= p->num_sections()) return nullptr;
    return p->section_name(n).c_str();
  }

  const char* c_datablock_get_value_name(c_datablock const* s, 
    const char* section, int j)
  {
    if (j < 0) return nullptr;
    DataBlock const* p = static_cast<DataBlock const*>(s);
    return p->value_name(section, j).c_str();
  }

  const char* c_datablock_get_value_name_by_section_index(c_datablock const* s, 
    int i, int j)
  {
    if (i<0 || j<0) return nullptr;
    DataBlock const* p = static_cast<DataBlock const*>(s);
    return p->value_name(i, j).c_str();
  }

  DATABLOCK_STATUS destroy_c_datablock(c_datablock* s)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    DataBlock* p = static_cast<DataBlock*>(s);
    // The call to clear() is not really necessary, but to aid in
    // debugging incorrect use of the C interface (especially to help
    // detect premature calls to destroy_c_datablock), it seems
    // useful.
    p->clear();
    delete p;
    return DBS_SUCCESS;
  }

  DATABLOCK_STATUS c_datablock_get_type(c_datablock const * s,
                                        const char* section,
                                        const char* name,
                                        datablock_type_t * t)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    DataBlock const* p = static_cast<DataBlock const*>(s);
    return p->get_type(section, name, *t);
  }

  DATABLOCK_STATUS
  c_datablock_get_int(c_datablock * s,
		      const char* section,
		      const char* name,
		      int* val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;

    auto p = static_cast<DataBlock *>(s);
    return p->get_val(section, name, *val);
  }

  DATABLOCK_STATUS
  c_datablock_get_int_default(c_datablock * s,
                              const char* section,
                              const char* name,
                              int def,
                              int* val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;

    auto p = static_cast<DataBlock *>(s);
    return p->get_val(section, name, def, *val);
  }


  DATABLOCK_STATUS
  c_datablock_get_bool(c_datablock * s,
          const char* section,
          const char* name,
          bool* val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;

    auto p = static_cast<DataBlock *>(s);
    return p->get_val(section, name, *val);
  }

  DATABLOCK_STATUS
  c_datablock_get_bool_default(c_datablock * s,
                              const char* section,
                              const char* name,
                              bool def,
                              bool* val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;

    auto p = static_cast<DataBlock *>(s);
    return p->get_val(section, name, def, *val);
  }



  DATABLOCK_STATUS
  c_datablock_get_double(c_datablock * s,
			 const char* section,
			 const char* name,
			 double* val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;

    auto p = static_cast<DataBlock *>(s);
    return p->get_val(section, name, *val);
  }

  DATABLOCK_STATUS
  c_datablock_get_double_default(c_datablock * s,
                                 const char* section,
                                 const char* name,
                                 double def,
                                 double* val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;

    auto p = static_cast<DataBlock *>(s);
    return p->get_val(section, name, def, *val);
  }

  DATABLOCK_STATUS
  c_datablock_get_complex(c_datablock * s,
			  const char* section,
			  const char* name,
			  double _Complex* val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;

    auto p = static_cast<DataBlock *>(s);
    complex_t z;
    auto rc = p->get_val(section, name, z);
    // C11 provides a function macro to create a double _Complex from
    // real and imaginary parts, but we don't require a C11-compliant
    // compiler. I would expect
    //
    //    *val = z.real() + z.imag() * _Complex_I;
    //
    // to work, but it produces a compilation failure with
    // GCC 4.8.2.  The cast below is unattractive, but works because
    // C++11 promises layout compatibility between
    // std::complex<double> and double[2], and C makes the similar
    // guarantee for double _Complex.
    if (rc == DBS_SUCCESS) *val = * reinterpret_cast<double _Complex*>(&z);
    return rc;
  }

  DATABLOCK_STATUS
  c_datablock_get_complex_default(c_datablock * s,
                                  const char* section,
                                  const char* name,
                                  double _Complex def,
                                  double _Complex* val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;

    auto p = static_cast<DataBlock *>(s);
    complex_t default_z(def);
    complex_t z;
    auto rc = p->get_val(section, name, default_z, z);
    // See comment in c_datablock_get_complex for an explanation of this
    // reinterpret_cast.
    if (rc == DBS_SUCCESS) *val = * reinterpret_cast<double _Complex*>(&z);
    return rc;
  }

  DATABLOCK_STATUS
  c_datablock_get_string(c_datablock * s,
                         const char* section,
                         const char* name,
                         char**  val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;

    auto p = static_cast<DataBlock *>(s);
    string tmp;
    auto rc = p->get_val(section, name, tmp);
    if (rc != DBS_SUCCESS) return rc;
    *val = strdup(tmp.c_str());
    if (*val == nullptr) return DBS_MEMORY_ALLOC_FAILURE;
    return DBS_SUCCESS;
  }

  DATABLOCK_STATUS
  c_datablock_get_string_default(c_datablock * s,
                                 const char* section,
                                 const char* name,
                                 const char* def,
                                 char**  val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    /* Do we need a new enumeration value for the following? */
    if (def == nullptr) return DBS_VALUE_NULL; 
    if (val == nullptr) return DBS_VALUE_NULL;

    auto p = static_cast<DataBlock *>(s);
    string tmp;
    string default_string(def);
    auto rc = p->get_val(section, name, default_string, tmp);
    if (rc != DBS_SUCCESS) return rc;
    *val = strdup(tmp.c_str());
    if (*val == nullptr) return DBS_MEMORY_ALLOC_FAILURE;
    return DBS_SUCCESS;
  }

  DATABLOCK_STATUS
  c_datablock_get_int_array_1d(c_datablock * s,
                               const char* section,
                               const char* name,
                               int** val,
                               int* sz)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;
    if (sz == nullptr) return DBS_SIZE_NULL;

    auto p = static_cast<DataBlock *>(s);
    try {
      vector<int> const& r = p->view<vector<int>>(section, name);
      *val = static_cast<int*>(malloc(r.size() * sizeof(int)));
      if (*val ==nullptr) return DBS_MEMORY_ALLOC_FAILURE;
      std::copy(r.cbegin(), r.cend(), *val);
      *sz = r.size();
      return DBS_SUCCESS;
    }
    catch (DataBlock::BadDataBlockAccess const&) { return DBS_SECTION_NOT_FOUND; }
    catch (Section::BadSectionAccess const&) { return DBS_NAME_NOT_FOUND; }
    catch (Entry::BadEntry const&) { return DBS_WRONG_VALUE_TYPE; }
    catch (...) { return DBS_LOGIC_ERROR; }
    return DBS_LOGIC_ERROR;
  }

  DATABLOCK_STATUS
  c_datablock_get_double_array_1d(c_datablock * s,
                               const char* section,
                               const char* name,
                               double** val,
                               int* sz)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;
    if (sz == nullptr) return DBS_SIZE_NULL;

    auto p = static_cast<DataBlock *>(s);
    try {
      vector<double> const& r = p->view<vector<double>>(section, name);
      *val = static_cast<double*>(malloc(r.size() * sizeof(double)));
      if (*val ==nullptr) return DBS_MEMORY_ALLOC_FAILURE;
      std::copy(r.cbegin(), r.cend(), *val);
      *sz = r.size();
      return DBS_SUCCESS;
    }
    catch (DataBlock::BadDataBlockAccess const&) { return DBS_SECTION_NOT_FOUND; }
    catch (Section::BadSectionAccess const&) { return DBS_NAME_NOT_FOUND; }
    catch (Entry::BadEntry const&) { return DBS_WRONG_VALUE_TYPE; }
    catch (...) { return DBS_LOGIC_ERROR; }
    return DBS_LOGIC_ERROR;
  }

/*
  DATABLOCK_STATUS
  c_datablock_get_complex_array_1d(c_datablock * s,
                               const char* section,
                               const char* name,
                               double _Complex** val,
                               int* sz)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;
    if (sz == nullptr) return DBS_SIZE_NULL;

    auto p = static_cast<DataBlock *>(s);
    try {
      vector<double _Complex> const& r = p->view<vector<double _Complex>>(section, name);
      *val = static_cast<double _Complex*>(malloc(r.size() * sizeof(double _Complex)));
      if (*val ==nullptr) return DBS_MEMORY_ALLOC_FAILURE;
      std::copy(r.cbegin(), r.cend(), *val);
      *sz = r.size();
      return DBS_SUCCESS;
    }
    catch (DataBlock::BadDataBlockAccess const&) { return DBS_SECTION_NOT_FOUND; }
    catch (Section::BadSectionAccess const&) { return DBS_NAME_NOT_FOUND; }
    catch (Entry::BadEntry const&) { return DBS_WRONG_VALUE_TYPE; }
    catch (...) { return DBS_LOGIC_ERROR; }
    return DBS_LOGIC_ERROR;
  }
*/


  DATABLOCK_STATUS
  c_datablock_get_int_array_1d_preallocated(c_datablock * s,
                                            const char* section,
                                            const char* name,
                                            int* val,
                                            int* sz,
                                            int maxsize)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;
    if (sz == nullptr) return DBS_SIZE_NULL;

    auto p = static_cast<DataBlock *>(s);
    vector<int> const& r = p->view<vector<int>>(section, name);
    *sz = r.size();
    if (r.size() > static_cast<size_t>(maxsize)) return DBS_SIZE_INSUFFICIENT;
    std::copy(r.cbegin(), r.cend(), val);
    // If we are asked to clear out the remainder of the input buffer,
    // the following line should be used.
    //    std::fill(val + *sz, val+maxsize, 0);
    return DBS_SUCCESS;
  }


  DATABLOCK_STATUS
  c_datablock_get_double_array_1d_preallocated(c_datablock * s,
                                            const char* section,
                                            const char* name,
                                            double* val,
                                            int* sz,
                                            int maxsize)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;
    if (sz == nullptr) return DBS_SIZE_NULL;

    auto p = static_cast<DataBlock *>(s);
    vector<double> const& r = p->view<vector<double>>(section, name);
    *sz = r.size();
    if (r.size() > static_cast<size_t>(maxsize)) return DBS_SIZE_INSUFFICIENT;
    std::copy(r.cbegin(), r.cend(), val);
    // If we are asked to clear out the remainder of the input buffer,
    // the following line should be used.
    //    std::fill(val + *sz, val+maxsize, 0);
    return DBS_SUCCESS;
  }


/*
  DATABLOCK_STATUS
  c_datablock_get_complex_array_1d_preallocated(c_datablock * s,
                                            const char* section,
                                            const char* name,
                                            double _Complex * val,
                                            int* sz,
                                            int maxsize)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;
    if (sz == nullptr) return DBS_SIZE_NULL;

    auto p = static_cast<DataBlock *>(s);
    vector<double _Complex> const& r = p->view<vector<double _Complex>>(section, name);
    *sz = r.size();
    if (r.size() > static_cast<size_t>(maxsize)) return DBS_SIZE_INSUFFICIENT;
    std::copy(r.cbegin(), r.cend(), val);
    // If we are asked to clear out the remainder of the input buffer,
    // the following line should be used.
    //    std::fill(val + *sz, val+maxsize, 0);
    return DBS_SUCCESS;
  }
*/


  DATABLOCK_STATUS
  c_datablock_put_int(c_datablock* s,
          const char* section,
          const char* name,
          int val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;

    auto p = static_cast<DataBlock*>(s);
    return p->put_val(section, name, val);
  }

  DATABLOCK_STATUS
  c_datablock_put_bool(c_datablock* s,
          const char* section,
          const char* name,
          bool val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;

    auto p = static_cast<DataBlock*>(s);
    return p->put_val(section, name, val);
  }

  DATABLOCK_STATUS
  c_datablock_put_double(c_datablock* s,
			 const char* section,
			 const char* name,
			 double val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;

    auto p = static_cast<DataBlock*>(s);
    return p->put_val(section, name, val);
  }

  DATABLOCK_STATUS
  c_datablock_put_complex(c_datablock* s,
			  const char* section,
			  const char* name,
			  double _Complex val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;

    auto p = static_cast<DataBlock*>(s);
    complex_t z(val);
    return p->put_val(section, name, z);
  }

  DATABLOCK_STATUS
  c_datablock_put_string(c_datablock* s,
			 const char* section,
			 const char* name,
			 const char* val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == NULL) return DBS_VALUE_NULL;

    auto p = static_cast<DataBlock*>(s);
    return p->put_val(section, name, string(val));
  }

  DATABLOCK_STATUS
  c_datablock_put_int_array_1d(c_datablock* s,
                               const char* section,
                               const char* name,
                               int const*  val,
                               int sz)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == NULL) return DBS_VALUE_NULL;
    if (sz < 0) return DBS_SIZE_NEGATIVE;

    auto p = static_cast<DataBlock*>(s);
    return p->put_val(section, name, vector<int>(val, val+sz));
  }

  DATABLOCK_STATUS
  c_datablock_put_double_array_1d(c_datablock* s,
                               const char* section,
                               const char* name,
                               double const*  val,
                               int sz)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == NULL) return DBS_VALUE_NULL;
    if (sz < 0) return DBS_SIZE_NEGATIVE;

    auto p = static_cast<DataBlock*>(s);
    return p->put_val(section, name, vector<double>(val, val+sz));
  }

/*
  DATABLOCK_STATUS
  c_datablock_put_complex_array_1d(c_datablock* s,
                               const char* section,
                               const char* name,
                               double _Complex const*  val,
                               int sz)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == NULL) return DBS_VALUE_NULL;
    if (sz < 0) return DBS_SIZE_NEGATIVE;

    auto p = static_cast<DataBlock*>(s);
    return p->put_val(section, name, vector<double _Complex>(val, val+sz));
  }
*/


  DATABLOCK_STATUS
  c_datablock_replace_int(c_datablock* s,
        const char* section,
        const char* name,
        int val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;

    auto p = static_cast<DataBlock*>(s);
    return p->replace_val(section, name, val);
  }

  DATABLOCK_STATUS
  c_datablock_replace_bool(c_datablock* s,
        const char* section,
        const char* name,
        bool val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;

    auto p = static_cast<DataBlock*>(s);
    return p->replace_val(section, name, val);
  }


  DATABLOCK_STATUS
  c_datablock_replace_double(c_datablock* s,
			     const char* section,
			     const char* name,
			     double val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;

    auto p = static_cast<DataBlock*>(s);
    return p->replace_val(section, name, val);
  }

  DATABLOCK_STATUS
  c_datablock_replace_complex(c_datablock* s,
			     const char* section,
			     const char* name,
			     double _Complex val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;

    auto p = static_cast<DataBlock*>(s);
    std::complex<double> z { val };
    return p->replace_val(section, name, z);
  }

  DATABLOCK_STATUS
  c_datablock_replace_string(c_datablock* s,
			     const char* section,
			     const char* name,
			     const char* val)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == NULL) return DBS_VALUE_NULL;

    auto p = static_cast<DataBlock*>(s);
    return p->replace_val(section, name, string(val));
  }

  DATABLOCK_STATUS
  c_datablock_replace_int_array_1d(c_datablock* s,
                                   const char* section,
                                   const char* name,
                                   int const* val,
                                   int sz)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;
    if (sz  < 0) return DBS_SIZE_NEGATIVE;

    auto p = static_cast<DataBlock*>(s);
    return p->replace_val(section, name, vector<int>(val, val+sz));
  }

  DATABLOCK_STATUS
  c_datablock_replace_double_array_1d(c_datablock* s,
                                   const char* section,
                                   const char* name,
                                   double const* val,
                                   int sz)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;
    if (sz  < 0) return DBS_SIZE_NEGATIVE;

    auto p = static_cast<DataBlock*>(s);
    return p->replace_val(section, name, vector<double>(val, val+sz));
  }
/*
  DATABLOCK_STATUS
  c_datablock_replace_complex_array_1d(c_datablock* s,
                                   const char* section,
                                   const char* name,
                                   double _Complex const* val,
                                   int sz)
  {
    if (s == nullptr) return DBS_DATABLOCK_NULL;
    if (section == nullptr) return DBS_SECTION_NULL;
    if (name == nullptr) return DBS_NAME_NULL;
    if (val == nullptr) return DBS_VALUE_NULL;
    if (sz  < 0) return DBS_SIZE_NEGATIVE;

    auto p = static_cast<DataBlock*>(s);
    return p->replace_val(section, name, vector<double _Complex>(val, val+sz));
  }
*/



DATABLOCK_STATUS  c_datablock_put_double_grid(
  c_datablock* s,
  const char * section, 
  const char * name_x, int n_x, double * x,  
  const char * name_y, int n_y, double * y, 
  const char * name_z, double ** z)
{
    DATABLOCK_STATUS status=DBS_SUCCESS;

    // const int ndim = 2;
    // int dims[ndim] = {n_x, n_y};

    status = c_datablock_put_double_array_1d(s, section, name_x, x, n_x);
    if (status) {return status;}
    status = c_datablock_put_double_array_1d(s, section, name_y, y, n_y);
    if (status) {return status;}
    // status |= c_datablock_put_double_array(s, section, name_z, z, ndim, dims);
    int n_z = n_x * n_y;
    double * z_flat = (double*)malloc(sizeof(double)*n_z);
    int p=0;
    for (int i=0; i<n_x; i++){
      for (int j=0; j<n_y; j++){
        z_flat[p++] = z[i][j];
      }
    }
    status = c_datablock_put_double_array_1d(s, section, name_z, z_flat, n_z);
    if (status) {return status;}

    // We could rely on n_x and n_y to record in the block what ordering the array has.
    // But that would break down if n_x == n_y
    char sentinel_key[512];
    char sentinel_value[512];

    snprintf(sentinel_key, 512, "_cosmosis_order_%s",name_z);
    snprintf(sentinel_value, 512, "%s_cosmosis_order_%s",name_x, name_y);

    status = c_datablock_put_string(s, section, sentinel_key, sentinel_value);
    return status;
}

double ** allocate_2d_double(int nx, int ny){
  double ** z = (double**)malloc(nx*sizeof(double*));
  for (int i=0; i<nx; i++){
    z[i] = (double*)malloc(sizeof(double)*ny);
  }
  return z;
}

void deallocate_2d_double(double *** z, int nx){
  for (int i=0; i<nx; i++){
    free((*z)[i]);
  }
  free(*z);
  *z = NULL;
}

DATABLOCK_STATUS  c_datablock_get_double_grid(
  c_datablock* s,
  const char * section, 
  const char * name_x, int *n_x, double ** x,  
  const char * name_y, int *n_y, double ** y, 
  const char * name_z, double *** z)
{
    DATABLOCK_STATUS status;
    *x = NULL;
    *y = NULL;
    *z = NULL;
    int nx, ny;

    status = c_datablock_get_double_array_1d(s, section, name_x, x, &nx);
    if (status) {return status;}
    status = c_datablock_get_double_array_1d(s, section, name_y, y, &ny);
    if (status) {free(*x); *x=NULL; return status;}
    int n_z;
    double * z_flat;
    status = c_datablock_get_double_array_1d(s, section, name_z, &z_flat, &n_z);
    if (status) {free(*x); free(*y); *x=NULL; *y=NULL; return status;}
    double ** z_2d = allocate_2d_double(nx, ny);

    //Now we need to check if the ordering requested here is the same
    //as the saved ordering.  If not we need to transpose.
    char sentinel_key[512];
    char * sentinel_value;
    char sentinel_test[512];

    snprintf(sentinel_key, 512, "_cosmosis_order_%s",name_z);
    status = c_datablock_get_string(s,section, sentinel_key, &sentinel_value);
    if (status) {free(*x); free(*y); free(z_flat); *x=NULL; *y=NULL; deallocate_2d_double(&z_2d, nx); return status;}

    snprintf(sentinel_test, 512, "%s_cosmosis_order_%s",name_x, name_y);
    if (0==strncmp(sentinel_test, sentinel_value, 512)){
      // This indicates that the requested ordering is the same as the stored one.
      // So we do not need to do any flipping.
      for (int i=0; i<nx; i++){
        for (int j=0; j<ny; j++){
          z_2d[i][j] = z_flat[i*ny+j];
            if (i==0) printf("IN %d  %le\n", j, z_2d[i][j]);
        }
      }
    }
    else{
      snprintf(sentinel_test, 512, "%s_cosmosis_order_%s",name_y, name_x);
      if (0==strncmp(sentinel_test, sentinel_value, 512)){
        for (int i=0; i<nx; i++){
          for (int j=0; j<ny; j++){
            z_2d[i][j] = z_flat[j*nx+i];
          }
        }
      }
      else{
        // no match - something wrong. 
        status = DBS_WRONG_VALUE_TYPE;
        free(*x); 
        free(*y); 
        free(z_flat); 
        *x=NULL; 
        *y=NULL; 
        deallocate_2d_double(&z_2d, nx);
        free(sentinel_value);     
        return status;
      }
    }
  free(z_flat);

  *n_x = nx;
  *n_y = ny;
  *z = z_2d;
  return status;
}





} // extern "C"