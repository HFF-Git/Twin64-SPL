//----------------------------------------------------------------------------------------
//
//  T64SPL - A compiler for Twin64 - ConfigOpt
//
//----------------------------------------------------------------------------------------
// ...
//
//----------------------------------------------------------------------------------------
//
// T64SPL - A compiler for Twin64 - ConfigOpt
// Copyright (C) 2020 - 2026 Helmut Fieres
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <http://www.gnu.org/licenses/>.
//
//----------------------------------------------------------------------------------------
#include "Compiler.h"

//----------------------------------------------------------------------------------------
// Local data for command line option parsing.
//
//----------------------------------------------------------------------------------------
namespace {

char sourceFileName[ MAX_FILE_PATH_SIZE ] = { 0 };
char logFileName[ MAX_FILE_PATH_SIZE ]    = { 0 };
char objectFileName[ MAX_FILE_PATH_SIZE ] = { 0 };
char asmFileName[ MAX_FILE_PATH_SIZE ]    = { 0 };

bool verboseFlag = false;

//----------------------------------------------------------------------------------------
//
// Command line option definitions.
//
//----------------------------------------------------------------------------------------
enum {

    CL_ARG_VAL_HELP = 1,
    CL_ARG_VAL_VERSION,
    CL_ARG_VAL_VERBOSE,
    CL_ARG_VAL_LOG_FILE,
    CL_ARG_VAL_OBJECT_FILE,
    CL_ARG_VAL_ASM_FILE
};

enum {

    CL_OPT_NO_ARGUMENT,
    CL_OPT_REQUIRED_ARGUMENT
};

struct CmdLineOptions {

    const char *name;
    int         argOpt;
    int         val;
};

const CmdLineOptions optionTable[ ] = {

    { "help",       CL_OPT_NO_ARGUMENT,       CL_ARG_VAL_HELP },
    { "version",    CL_OPT_NO_ARGUMENT,       CL_ARG_VAL_VERSION },
    { "verbose",    CL_OPT_NO_ARGUMENT,       CL_ARG_VAL_VERBOSE },
    { "logfile",    CL_OPT_REQUIRED_ARGUMENT, CL_ARG_VAL_LOG_FILE },
    { "objectfile", CL_OPT_REQUIRED_ARGUMENT, CL_ARG_VAL_OBJECT_FILE },
    { "asmfile",    CL_OPT_REQUIRED_ARGUMENT, CL_ARG_VAL_ASM_FILE },

    { nullptr,      CL_OPT_NO_ARGUMENT,       0 }
};

//----------------------------------------------------------------------------------------
//
// Copy string into destination buffer.
//
//----------------------------------------------------------------------------------------
bool copyFileName( char *dest, const char *src ) {

    if ( src == nullptr || strlen( src ) >= MAX_FILE_PATH_SIZE ) {

        fprintf( stderr, "Invalid or excessively long file name\n" );
        return( false );
    }

    strcpy( dest, src );
    return( true );
}

//----------------------------------------------------------------------------------------
// Print compiler version.
//
//----------------------------------------------------------------------------------------
void printVersion( ) {

    printf( "Twin64 Compiler Version %s, PatchLevel %d\n",
            COMPILER_VERSION, COMPILER_PATCH_LEVEL );
}

//----------------------------------------------------------------------------------------
// Print command line help.
//
//----------------------------------------------------------------------------------------
void printHelp( ) {

    printVersion( );

    printf( "\nUsage: T64SPL [options] source-file\n\n" );

    printf( "Options:\n" );
    printf( "  --help                 Display this help message\n" );
    printf( "  --version              Display program version\n" );
    printf( "  --verbose              Enable verbose output\n" );
    printf( "  --logfile=<file>       Specify log file\n" );
    printf( "  --objectfile=<file>    Specify object file\n" );
    printf( "  --asmfile=<file>       Specify assembler output file\n" );
}

//----------------------------------------------------------------------------------------
// Find command line option.
//
//----------------------------------------------------------------------------------------
const CmdLineOptions *findOption( const char *name, size_t nameLen ) {

    for ( int i = 0; optionTable[ i ].name != nullptr; i++ ) {

        if ( strlen( optionTable[ i ].name ) == nameLen &&
             strncmp( optionTable[ i ].name, name, nameLen ) == 0 ) {

            return( &optionTable[ i ] );
        }
    }

    return( nullptr );
}

}; // namespace


//----------------------------------------------------------------------------------------
// Process command line options.
//
//----------------------------------------------------------------------------------------
CmdLineResult processCmdLineOptions( int argc, char *argv[ ] ) {

    bool optionsEnabled = true;

    // Reset configuration.

    sourceFileName[ 0 ] = '\0';
    logFileName[ 0 ]    = '\0';
    objectFileName[ 0 ] = '\0';
    asmFileName[ 0 ]    = '\0';

    verboseFlag = false;

    for ( int i = 1; i < argc; i++ ) {

        const char *arg = argv[ i ];

        if ( optionsEnabled && strcmp( arg, "--" ) == 0 ) {

            optionsEnabled = false;
            continue;
        }

        if ( optionsEnabled && strncmp( arg, "--", 2 ) == 0 ) {

            const char *name = arg + 2;
            const char *eq   = strchr( name, '=' );

            size_t nameLen = eq ? static_cast<size_t>( eq - name )
                                : strlen( name );

            const CmdLineOptions *opt = findOption( name, nameLen );

            if ( opt == nullptr ) {

                fprintf( stderr, "Unknown option '%s'\n", arg );
                return( CMD_LINE_ERROR );
            }

            const char *optArg = nullptr;

            if ( opt -> argOpt == CL_OPT_REQUIRED_ARGUMENT ) {

                if ( eq != nullptr ) {

                    optArg = eq + 1;
                }
                else if ( i + 1 < argc ) {

                    optArg = argv[ ++i ];
                }
                else {

                    fprintf( stderr, "Option '--%s' requires an argument\n",
                             opt -> name );
                    return( CMD_LINE_ERROR );
                }

                if ( optArg[ 0 ] == '\0' ) {

                    fprintf( stderr, "Option '--%s' requires an argument\n",
                             opt -> name );
                    return( CMD_LINE_ERROR );
                }
            }
            else if ( eq != nullptr ) {

                fprintf( stderr, "Option '--%s' does not accept an argument\n",
                         opt -> name );
                return( CMD_LINE_ERROR );
            }

            switch ( opt -> val ) {

                case CL_ARG_VAL_HELP: {

                    printHelp( );
                    return( CMD_LINE_HELP );

                } break;

                case CL_ARG_VAL_VERSION: {

                    printVersion( );
                    return( CMD_LINE_VERSION );

                } break;

                case CL_ARG_VAL_VERBOSE: {

                    verboseFlag = true;

                } break;

                case CL_ARG_VAL_LOG_FILE: {

                    if ( !copyFileName( logFileName, optArg )) return( CMD_LINE_ERROR );

                } break;

                case CL_ARG_VAL_OBJECT_FILE: {

                    if ( !copyFileName( objectFileName, optArg )) return( CMD_LINE_ERROR );

                } break;

                case CL_ARG_VAL_ASM_FILE: {

                    if ( !copyFileName( asmFileName, optArg )) return( CMD_LINE_ERROR );

                } break;
            }

            continue;
        }

        if ( sourceFileName[ 0 ] != '\0' ) {

            fprintf( stderr, "Only one source file is allowed\n" );
            return( CMD_LINE_ERROR );
        }

        if ( !copyFileName( sourceFileName, arg )) return( CMD_LINE_ERROR );
    }

    if ( sourceFileName[ 0 ] == '\0' ) {

        fprintf( stderr, "No source file specified\n\n" );
        printHelp( );
        return( CMD_LINE_ERROR );
    }

    return( CMD_LINE_OK );
}

//----------------------------------------------------------------------------------------
// Accessors.
//
//----------------------------------------------------------------------------------------
const char *getSourceFile( ) {

    return( sourceFileName );
}

const char *getLogFile( ) {

    return( logFileName );
}

const char *getObjectFile( ) {

    return( objectFileName );
}

const char *getAsmFile( ) {

    return( asmFileName );
}

bool getVerboseFlag( ) {

    return( verboseFlag );
}
