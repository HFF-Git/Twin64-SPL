//----------------------------------------------------------------------------------------
//
//  T64SPL - A compiler for Twin64 - Annotations
//
//----------------------------------------------------------------------------------------
// ...
//
//----------------------------------------------------------------------------------------
//
// T64SPL - A compiler for Twin64 - Annotations
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

Arena       *annotationArena = nullptr;
Annotation  *annotationFirst = nullptr;
Annotation  *annotationLast  = nullptr;

unsigned    annotationCount  = 0;
unsigned    errorCount       = 0;
unsigned    warningCount     = 0;
unsigned    noteCount        = 0;
unsigned    infoCount        = 0;

//----------------------------------------------------------------------------------------
// Add an annotation.
//
//----------------------------------------------------------------------------------------
void addAnnotation( AnnotationType type,

                    unsigned errorId,
                    unsigned srcLineNum,
                    unsigned srcLineCol,
                    unsigned length,
                    const char *message ) {

    Annotation *ann = static_cast<Annotation *>(

        arenaAlloc( annotationArena, sizeof( Annotation )));

    if ( ann == nullptr ) {

        fprintf( stderr, "Fatal error: cannot allocate annotation\n" );

        exit( 1 );

    }

    ann->type    = type;

    ann->srcLineNum = srcLineNum;
    ann->srcLineCol = srcLineCol;
    ann->length  = length;
    ann->errorId = errorId;

    snprintf( ann->fileName, sizeof( ann->fileName ), "%s",

              srcFileGetName( ));

    // snprintf( ann->message, sizeof( ann->message ), "%s", message );

    // Append annotation to list.

    if ( annotationFirst == nullptr ) {

        annotationFirst = ann;
    }
    else {

        annotationLast->next = ann;
    }

    annotationLast = ann;
    annotationCount++;

    // Update counters.

    switch ( type ) {

        case ANN_ERROR:   errorCount++;   break;
        case ANN_WARNING: warningCount++; break;
        case ANN_NOTE:    noteCount++;    break;
        case ANN_INFO:    infoCount++;    break;
    }
}

}; // namespace

//----------------------------------------------------------------------------------------
// Initialize annotation module.
//
//----------------------------------------------------------------------------------------
void annotationsInit( Arena *arena ) {

    annotationArena = arena;
    annotationFirst = nullptr;
    annotationLast  = nullptr;

    annotationCount = 0;
    errorCount   = 0;
    warningCount = 0;
    noteCount    = 0;
    infoCount    = 0;
}

//----------------------------------------------------------------------------------------
//
// Release annotation storage.
//
//----------------------------------------------------------------------------------------
void annotationsDone( ) {

    annotationFirst     = nullptr;
    annotationLast      = nullptr;
    annotationCount     = 0;

    errorCount          = 0;
    warningCount        = 0;
    noteCount           = 0;
    infoCount           = 0;

    arenaDestroy( annotationArena );
}

//----------------------------------------------------------------------------------------
// Add compiler error.
//
//----------------------------------------------------------------------------------------
void compErr( unsigned errorId,
              unsigned srcLineNum,
              unsigned srcLineCol,
              unsigned length,
              const char *message ) {

    addAnnotation( ANN_ERROR, errorId, srcLineNum, srcLineCol, length, message );
}

//----------------------------------------------------------------------------------------
// Add compiler warning.
//
//----------------------------------------------------------------------------------------
void compWarn( unsigned errorId,
               unsigned srcLineNum,
              unsigned srcLineCol,
               unsigned length,
               const char *message ) {

    addAnnotation( ANN_WARNING, errorId, srcLineNum, srcLineCol, length, message );
}

//----------------------------------------------------------------------------------------
// Add compiler note.
//
//----------------------------------------------------------------------------------------
void compNote( unsigned errorId,
               unsigned srcLineNum,
              unsigned srcLineCol,
               unsigned length,
               const char *message ) {

    addAnnotation( ANN_NOTE, errorId, srcLineNum, srcLineCol, length, message );
}

//----------------------------------------------------------------------------------------
// Add compiler information.
//
//----------------------------------------------------------------------------------------
void compInfo( unsigned errorId,
               unsigned srcLineNum,
              unsigned srcLineCol,
               unsigned length,
               const char *message ) {

    addAnnotation( ANN_INFO, errorId, srcLineNum, srcLineCol, length, message );
}

//----------------------------------------------------------------------------------------
// Return number of annotations.
//
//----------------------------------------------------------------------------------------
unsigned annotationsGetCount( ) {

    return( annotationCount );
}

//----------------------------------------------------------------------------------------
// Return annotation by index.
//
//----------------------------------------------------------------------------------------
const Annotation *annotationsGetFirst( ) {

    return( annotationFirst );
}

//----------------------------------------------------------------------------------------
//
// Return diagnostic counters.
//
//----------------------------------------------------------------------------------------
unsigned annotationsGetErrorCount( ) {

    return( errorCount );
}

unsigned annotationsGetWarningCount( ) {

    return( warningCount );
}

unsigned annotationsGetNoteCount( ) {

    return( noteCount );
}

unsigned annotationsGetInfoCount( ) {

    return( infoCount );
}
