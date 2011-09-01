#ifndef FFMPEG_FAS_PRIVATE_H
#define FFMPEG_FAS_PRIVATE_H



#ifdef _MSC_VER
#define fas_export_decl __declspec(dllexport)
#define fas_import_decl __declspec(dllimport)
#else
#define fas_export_decl
#define fas_import_decl
#endif

#ifdef fas_compile_lib
#define fas_export fas_export_decl
#else
#define fas_export fas_import_decl
#endif

#endif //incl. guards
