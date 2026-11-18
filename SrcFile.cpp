//----------------------------------------------------------------------------------------
//
//  T64SPL - A compiler for Twin64 - SrcFile
//
//----------------------------------------------------------------------------------------
// ...
//
//----------------------------------------------------------------------------------------
//
// T64SPL - A compiler for Twin64 - SrcFile
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
// Name space for local routines.
//
//----------------------------------------------------------------------------------------
namespace {

FILE            *srcFile                          = nullptr;
char            srcFilePath[ MAX_FILE_PATH_SIZE ] = { 0 };
char            srcFileName[ MAX_FILE_PATH_SIZE ] = { 0 };

char            srcFileLine[ MAX_INPUT ]          = { 0 };

unsigned        srcFileLineIndex                  = 1;
unsigned        srcFileCharIndex                  = 0;

size_t          srcFileLineLength                 = 0;
unsigned        srcFileLinePos                    = 0;

}; // namespace

//----------------------------------------------------------------------------------------
// Open source file.
//
//----------------------------------------------------------------------------------------
void openSrcFile( const char *filePath ) {

    srcFile = fopen( filePath, "r" );

    if ( srcFile == nullptr ) {

        fprintf( stderr, "Cannot open source file: %s\n", filePath );
        return;
    }

    const char *sep = strrchr( filePath, '/' );
    const char *sep2 = strrchr( filePath, '\\' );

    if ( sep == nullptr || ( sep2 != nullptr && sep2 > sep )) sep = sep2;

    if ( sep != nullptr ) {

        size_t len = ( sep - filePath ) + 1;

        snprintf( srcFilePath, sizeof( srcFilePath ), "%.*s",
                  static_cast<int>( len ), filePath );

        snprintf( srcFileName, sizeof( srcFileName ), "%s", sep + 1 );
    }
    else {

        srcFilePath[ 0 ] = '\0';
        snprintf( srcFileName, sizeof( srcFileName ), "%s", filePath );
    }

    srcFileLineIndex  = 1;
    srcFileCharIndex  = 0;

    srcFileLineLength = 0;
    srcFileLinePos    = 0;

    srcFileLine[ 0 ] = '\0';
}

//----------------------------------------------------------------------------------------
// Close source file.
//
//----------------------------------------------------------------------------------------
void closeSrcFile( ) {

    if ( srcFile != nullptr ) {

        fclose( srcFile );
        srcFile = nullptr;
    }
}

//----------------------------------------------------------------------------------------
// "srcFileGetChar" returns the next character from the source file.
//
//----------------------------------------------------------------------------------------
char srcFileGetChar( ) {

    if ( srcFile == nullptr ) return( 0 );

    if ( srcFileLinePos >= srcFileLineLength ) {

        if ( fgets( srcFileLine, sizeof( srcFileLine ), srcFile ) == nullptr ) {

            return( 0 );
        }

        srcFileLineLength = strlen( srcFileLine );
        srcFileLinePos    = 0;
    }

    char ch = srcFileLine[ srcFileLinePos++ ];

    if ( ch == '\n' ) {

        srcFileLineIndex ++;
        srcFileCharIndex = 0;

        return( '\n' );
    }

    srcFileCharIndex ++;
    return( ch );
}

//----------------------------------------------------------------------------------------
//
// Accessors.
//
//----------------------------------------------------------------------------------------
const char *srcFileGetPath( ) {

    return( srcFilePath );
}

const char *srcFileGetName( ) {

    return( srcFileName );
}

unsigned srcFileGetLineNum( ) {

    return( srcFileLineIndex );
}

unsigned srcFileGetCharIndex( ) {

    return( srcFileCharIndex );
}