/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

      1. The origin of this software must not be misrepresented; you must not
      claim that you wrote the original software. If you use this software
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

      2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original software.

      3. This notice may not be removed or altered from any source
      distribution.
*/

/* "Pure C" implementation of a standalone S1 compiler
 * 
 * DISCLAIMER: This implementation serves mainly as an example on using
 * the Shader1 C API; the "reference compiler" (e.g. feature-wise)
 * is the C++ implementation.
 */

#include "s1/Backend.h"
#include "s1/CompiledProgram.h"
#include "s1/Library.h"
#include "s1/Options.h"
#include "s1/ProgramDiagnostics.h"
#include "s1/ResultCode.h"
#include "s1/String.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char* result_code_string (s1_ResultCode code)
{
  const char* error_str;
  static char buf[16];

  error_str = s1_get_result_code_str (code);
  if (error_str) return error_str;
  sprintf (buf, "0x%.8x", code);
  return buf;
}

static const char* lib_error_string (s1_Library* lib)
{
  s1_ResultCode error = s1_library_get_last_error (lib);
  return result_code_string (error);
}

static void print_lib_error (const char* message, s1_Library* lib)
{
  fprintf (stderr, "%s: %s\n", message, lib_error_string (lib));
}

static void print_syntax (const char* exec_name)
{
  fprintf (stderr, "Syntax: %s [input filename]\n", exec_name);
}

static void print_diagnostics (s1_ProgramDiagnostics* program_diagnostics)
{
  size_t i, diag_num = s1_programdiagnostics_get_count (program_diagnostics);
  s1_String* diag_str;
  for (i = 0; i < diag_num; i++)
  {
    switch (s1_programdiagnostics_get_class (program_diagnostics, i))
    {
    case S1_DIAGNOSTIC_INVALID:
      break;
    case S1_DIAGNOSTIC_ERROR:
      diag_str = s1_programdiagnostics_create_description (program_diagnostics, i);
      fprintf (stderr, "error: %s\n", s1_string_u8 (diag_str));
      s1_release (diag_str);
      break;
    case S1_DIAGNOSTIC_WARNING:
      diag_str = s1_programdiagnostics_create_description (program_diagnostics, i);
      fprintf (stderr, "warning: %s\n", s1_string_u8 (diag_str));
      s1_release (diag_str);
      break;
    }
  }
}

enum
{
  /* Optimization level compiler picks by default */
  DEFAULT_OPTIMIZATION_LEVEL = 1,
  /* Optimization level compiler picks if optimization is requested, but no particular options (-O)s */
  DEFAULT_ENABLE_OPTIMIZATION_LEVEL = 2
};

int main (const int argc, const char* const argv[])
{
  int exit_code = 0;
  s1_Library* lib = NULL;
  s1_Options* compiler_options = NULL;
  s1_Backend* backend = NULL;
  s1_Program* program = NULL;
  s1_ResultCode error;
  int arg_num;
  const char* input_file_name = NULL;
  const char* entry_name = "main";
  const char* backend_str = "glsl";
  FILE* infile = NULL;
  char* source_str = NULL;
  size_t source_size = 0;
  
  error = s1_create_library (&lib);
  if (!S1_SUCCESSFUL(error))
  {
    fprintf (stderr, "failed to initialize library: %s\n", result_code_string (error));
    return 2;
  }

  compiler_options = s1_options_create (lib);
  if (!compiler_options)
  {
    exit_code = 2;
    print_lib_error ("failed to create compiler options", lib);
    goto cleanup_lib;
  }
  s1_options_set_opt_level (compiler_options, DEFAULT_OPTIMIZATION_LEVEL);
  /* Failure isn't that critical... */

  arg_num = 1;
  while (arg_num < argc)
  {
    const char* arg = argv[arg_num];
    if ((strcmp (arg, "--param-vertex") == 0)
        || (strcmp (arg, "--param-uniform") == 0)
        || (strcmp (arg, "--param-size") == 0))
    {
      /* Handled later */
      arg_num++;
    }
    else if (strcmp (arg, "--entry") == 0)
    {
      arg_num++;
      if (arg_num < argc)
        entry_name = argv[arg_num];
    }
    else if (strncmp (arg, "-O", 2) == 0)
    {
      const char* opt_str = arg+2;
      if (!*opt_str)
        s1_options_set_opt_level (compiler_options, DEFAULT_ENABLE_OPTIMIZATION_LEVEL);
      else if ((*opt_str >= '0') && (*opt_str <= '9'))
        s1_options_set_opt_level (compiler_options, *opt_str - '0');
      else
      {
        if (!s1_options_set_opt_flag_from_str (compiler_options, s1_u8 (opt_str)))
        {
          fprintf (stderr, "failed to set optimization flag %s: error %s\n",
                   opt_str, lib_error_string (lib));
        }
      }
    }
    else if (strcmp (arg, "--backend") == 0)
    {
      arg_num++;
      if (arg_num < argc)
        backend_str = argv[arg_num];
    }
    else
    {
      if (input_file_name != 0)
      {
        print_syntax (argv[0]);
        exit_code = 1;
        goto cleanup_options;
      }
      input_file_name = arg;
    }
    arg_num++;
  }

  if (input_file_name == 0)
  {
    print_syntax (argv[0]);
    exit_code = 1;
    goto cleanup_options;
  }
  
  backend = s1_backend_create (lib, s1_u8 (backend_str));
  if (!backend)
  {
    exit_code = 2;
    print_lib_error ("failed to set backend", lib);
    goto cleanup_options;
  }

  infile = fopen (input_file_name, "rb");
  if (!infile)
  {
    exit_code = 1;
    fprintf (stderr, "error %d opening %s\n", errno, input_file_name);
    goto cleanup_backend;
  }
  /* Check for BOM */
  {
    unsigned char bom_buf[3];
    if ((fread (bom_buf, 1, sizeof (bom_buf), infile) < sizeof (bom_buf))
      || (feof (infile) || (bom_buf[0] != 0xEF) || (bom_buf[1] != 0xBB) || (bom_buf[2] != 0xBF)))
    {
      /* Not a BOM, reset */
      fseek (infile, 0, SEEK_SET);
    }
  }
  {
    int start_pos = ftell (infile);
    fseek (infile, 0, SEEK_END);
    source_size = ftell (infile) - start_pos;
    fseek (infile, start_pos, SEEK_SET);

    source_str = malloc (source_size);
    if (fread (source_str, 1, source_size, infile) < source_size)
    {
      exit_code = 1;
      fprintf (stderr, "error %d reading from %s\n", ferror (infile), input_file_name);
      fclose (infile);
      goto cleanup_backend;
    }
  }
  fclose (infile);

  program = s1_program_create_from_string (lib, source_str, source_size, S1_COMPATIBILITY_LATEST);
  if (!program)
  {
    exit_code = 3;
    print_lib_error ("failed to create program", lib);
    goto cleanup_backend;
  }
  if (!s1_program_set_entry_function (program, s1_u8 (entry_name)))
  {
    exit_code = 3;
    print_lib_error ("failed to set entry function", lib);
    goto cleanup_program;
  }
  if (!s1_program_set_options (program, compiler_options))
  {
    exit_code = 3;
    print_lib_error ("failed to set program options", lib);
    goto cleanup_program;
  }

  arg_num = 1;
  while (arg_num < argc)
  {
    const char* arg = argv[arg_num];
    if (strcmp (arg, "--param-vertex") == 0)
    {
      arg_num++;
      if (arg_num < argc)
      {
        if (!s1_program_set_input_frequency (program, s1_u8 (argv[arg_num]), S1_FREQ_VERTEX))
        {
          fprintf (stderr, "failed to set input frequency for %s: error %s\n",
                    argv[arg_num], lib_error_string (lib));
        }
      }
    }
    else if (strcmp (arg, "--param-uniform") == 0)
    {
      arg_num++;
      if (arg_num < argc)
      {
        if (!s1_program_set_input_frequency (program, s1_u8 (argv[arg_num]), S1_FREQ_UNIFORM))
        {
          fprintf (stderr, "failed to set input frequency for %s: error %s\n",
                    argv[arg_num], lib_error_string (lib));
        }
      }
    }
    else if (strcmp (arg, "--param-size") == 0)
    {
      arg_num++;
      if (arg_num < argc)
      {
        const char* param = argv[arg_num];
        arg_num++;
        if (arg_num < argc)
        {
          unsigned size;
          char dummy;
          if (sscanf (argv[arg_num], "%u%c", &size, &dummy) != 1)
          {
            fprintf (stderr, "invalid array size: %s\n", argv[arg_num]);
          }
          else
          {      
            if (!s1_program_set_input_array_size (program, s1_u8 (param), size))
            {
              fprintf (stderr, "failed to set input array size for %s: error %s\n",
                        param, lib_error_string (lib));
            }
          }
        }
      }
    }
    else if (strcmp (arg, "--entry") == 0)
    {
      /* Skip */
      arg_num++;
    }
    else if (strcmp (arg, "--backend") == 0)
    {
      /* Skip */
      arg_num++;
    }
    else
    {
      /* Skip */
    }
    arg_num++;
  }

  print_diagnostics (s1_program_get_diagnostics (program));

  {
    s1_CompiledProgram* compiled_vp = s1_backend_generate_program (backend, program, S1_TARGET_VP, 0);
    if (!compiled_vp)
    {
      exit_code = 4;
      print_lib_error ("failed to generate vertex program", lib);
    }
    else
    {
      printf ("%s\n", s1_compiledprogram_get_string (compiled_vp));
      s1_release (compiled_vp);
    }
  }
  {
    s1_CompiledProgram* compiled_fp = s1_backend_generate_program (backend, program, S1_TARGET_FP, 0);
    if (!compiled_fp)
    {
      exit_code = 4;
      print_lib_error ("failed to generate fragment program", lib);
    }
    else
    {
      printf ("%s\n", s1_compiledprogram_get_string (compiled_fp));
      s1_release (compiled_fp);
    }
  }

cleanup_program:
  s1_release (program);
/*cleanup_source:*/
  if (source_str) free (source_str);
cleanup_backend:
  s1_release (backend);
cleanup_options:
  s1_release (compiler_options);
cleanup_lib:
  s1_release (lib);
  return exit_code;
}
