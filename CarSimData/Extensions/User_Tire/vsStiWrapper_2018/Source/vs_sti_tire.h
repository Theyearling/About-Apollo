#ifndef VSP_TIRE_FORD_H_
#define VSP_TIRE_FORD_H_

#include "vs_def_api.h"

// STI interface functions
VS_API void vs_sti_calc(int itire, vs_real time,
  vs_real xw0, vs_real yw0, vs_real zw0,
  vs_real yaw, vs_real pitch, vs_real roll,
  vs_real vwx, vs_real vwy, vs_real vwz,
  vs_real avwx, vs_real avwy, vs_real avwz,
  vs_real rot, vs_real w,
  vs_real *fxw0, vs_real *fyw0, vs_real *fzw0,
  vs_real *mxw0, vs_real *myw0, vs_real *mzw0,
  vs_real **infVar); //ok
VS_API void vs_sti_calc2(int iwheel, vs_real ldual, vs_real time,
  vs_real xw0, vs_real yw0, vs_real zw0,
  vs_real yaw, vs_real pitch, vs_real roll,
  vs_real vwx, vs_real vwy, vs_real vwz,
  vs_real avwx, vs_real avwy, vs_real avwz,
  vs_real rot, vs_real w,
  vs_real *fxw0, vs_real *fyw0, vs_real *fzw0,
  vs_real *mxw0, vs_real *myw0, vs_real *mzw0,
  vs_real **infVar, vs_real **infVar2);   //ok
VS_API void vs_sti_echo(FILE *ifile, int itire); //ok
VS_API void vs_sti_exit(int itire);              //ok  
VS_API void vs_sti_install(int nTires, int duals);   //ok
VS_API vs_bool vs_sti_scan(char *key, char *buffer); //ok
VS_API vs_real vs_sti_loaded_radius(int itab, vs_real fz, vs_real omegar);
VS_API void vs_sti_SetVisible(void);


// VS functions
typedef void(*VS_STI_WRITE_TO_LOGFILE_FUNC)(const int level, const char *format, ...);
typedef int* (*VS_STI_GET_VAR_PTR_INT_FUNC)(const char *keyword);
typedef vs_real* (*VS_STI_GET_VAR_PTR_FUNC)(const char *keyword);
typedef void(*VS_STI_WRITEI_FUNC)(FILE *ifile, char *key, int value, char *doc);
typedef void(*VS_STI_WRITE_ECHO_LINE_FUNC)(FILE *ifile, const char *key, const char *svalue, const char *doc);
typedef int(*VS_STI_SET_SYM_ATTRIBUTE_FUNC)(const int id, const vs_sym_attr_type type, const void *att);
typedef void(*VS_STI_PRINTF_ERROR_FUNC)(const char *format, ...);
typedef void(*VS_STI_INSTALL_FREE_FUNCTION_FUNC)(void(*func) (void));
typedef int(*VS_STI_DEFINE_SV_FUNC)(char *keyword, vs_real *real, vs_real value, char *units, char *desc);
typedef int(*VS_STI_DEFINE_OUT_FUNC)(char *shortname, char *longname, vs_real *real,
  char *units, char *genname, char *bodyname, char *comp);
typedef vs_bool(*VS_STI_HAVE_KEYWORD_FUNC)(const char *keyword);
typedef char* (*VS_STI_GET_FILEBASE_NAME_FUNC)(void);
typedef vs_real(*VS_STI_TYPARR_FUNC)(int itire, int x);
typedef int (*VS_STI_GET_ALLTIRESALIKE_FUNC)(void);
typedef void(*VS_STI_TIRES_SET_PARS_FUNC)(int itire, vs_real rre, vs_real r_free, vs_real kt_ext,
  vs_real kt2_ext, vs_real iyy, vs_real ms);
typedef void(*VS_STI_PRINT_ECHO_FUNC)(_Inout_ FILE * _File, _In_z_ _Printf_format_string_ const char * _Format, ...);
typedef void(*VS_STI_GET_ROAD_CONTACT_EXT_FUNC)(vs_real y, vs_real x, int id, vs_real *z,
  vs_real *dzdy, vs_real *dzdx, vs_real *muy, vs_real *mux);
typedef char* (*VS_STI_DATABASE_DIR_FUNC)(void);
typedef void(*VS_STI_SET_TYPARR_VISIBLE_FUNC)(int itire, int x);

#endif
