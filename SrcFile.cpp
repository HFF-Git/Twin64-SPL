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

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
FILE            *srcFile                                = nullptr;
char            srcFileName[ MAX_FILE_PATH_SIZE ]       = { 0 };
unsigned        srcFileLineIndex                        = 0;
unsigned        srcFileCharIndex                        = 0;

char            srcFileLine[ MAX_INPUT ]                = { 0 };
char            currentChar                             = ' ';

}; // namespace


void openSrcFile( const char *filePath ) {

    srcFile = fopen( filePath, "r" );
    
    if ( srcFile == nullptr ) {
        
        // ??? what to report ...   
    }
}


void closeSrcFile( ) {

    if ( srcFile != nullptr ) {
        
        fclose( srcFile );
        srcFile = nullptr;
    }
}


char srcFileGetChar( ) {

    if ( srcFile == nullptr ) return( 0 );

        
    int ch = fgetc( srcFile );
    
    if ( ch == EOF ) {

        currentChar = 0;
    }
    else if ( ch == '\n' ) {
        
        srcFileLineIndex ++;
        srcFileCharIndex = 0;
        currentChar      = ' ';
    }
    else {
        
        srcFileCharIndex ++;
        currentChar = static_cast<char>( ch );
    }
}

unsigned srcFileGetLineNum( ) {

    return( srcFileLineIndex );
}

unsigned  srcFileGetCharIndex( ) {

    return( srcFileCharIndex );
}
