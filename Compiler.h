//----------------------------------------------------------------------------------------
//
//  T64SPL - A compiler for Twin64 - Declarations
//
//----------------------------------------------------------------------------------------
// ...
//
//----------------------------------------------------------------------------------------
//
// T64SPL - A compiler for Twin64 - Declarations
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
#pragma once

//----------------------------------------------------------------------------------------
// Mac and Windows know different include files and procedure names for some 
// POSIX routines. Learned the hard way that these files better come really early
// in the project. All libraries and modules depend on these basic constants and
// type definitions and include the "T64-Common.h" file early on.
//
//----------------------------------------------------------------------------------------
#if __APPLE__
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#else
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <windows.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <iostream>
#include <io.h>
#endif

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
#include "Version.h"

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
const int MAX_TOKEN_NAME     = 32;
const int MAX_FILE_PATH_SIZE = 256;
const int MAX_TOKEN_STR_SIZE = 256;

//----------------------------------------------------------------------------------------
//
// Command line processing.
//
//----------------------------------------------------------------------------------------
enum CmdLineResult {

    CMD_LINE_OK,
    CMD_LINE_HELP,
    CMD_LINE_VERSION,
    CMD_LINE_ERROR
};

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
enum TokenId {

    TOK_NIL,
    TOK_EOS,
    TOK_EOF,
    TOK_ERR,

    TOK_IDENT,
    TOK_NUM,
    TOK_STR,

    TOK_COMMA,
    TOK_COLON,
    TOK_PERIOD,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACK,
    TOK_RBRACK,
    TOK_EQUAL,
    TOK_EQ,
    TOK_NE,
    TOK_LNOT,
    TOK_LE,
    TOK_LT,
    TOK_GE,
    TOK_GT,
    TOK_PLUS,
    TOK_MINUS,
    TOK_MULT,
    TOK_DIV,
    TOK_MOD,
    TOK_LAND,
    TOK_AND,
    TOK_LOR,
    TOK_OR,
    TOK_XOR,
    TOK_NEG,

    TOK_IF, 
    TOK_ELSE,
    




};




enum ScopeDescType {

    SCOPE_DESC_NONE,
    PREDEFINED_SCOPE,
    COMP_UNIT_SCOPE,
    IMPORT_SCOPE,
    MODULE_SCOPE,
    FUNC_SCOPE,
    WITH_SCOPE
};

enum ObjDescType {

    OBJ_DESC_NONE,
    OBJ_DESC_CONST,
    OBJ_DESC_VAR,
    OBJ_DESC_PARAM,
    OBJ_DESC_TYPE,
    OBJ_DESC_FUNC,
    OBJ_DESC_MODULE
};

enum TypeDescType {

    TYPE_DESC_NONE,
    TYOE_DESC_ARRAY,
    TYPE_DESC_RECORD,
    TYPE_DESC_POINTER,
    TYPE_DESC_SET,
    TYPE_DESC_STRING
};

enum ItemDescType {

    ITEM_DESC_NONE,
   
};

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
// ??? annotation 


enum CompileErrors {

    NO_ERR                          = 0,
    ERR_STRING_TOO_LONG             = 1,
    ERR_INVALID_UNICODE_ESCAPE      = 2,
    ERR_INVALID_NUM                 = 3,
    ERR_INVALID_HEX_ESCAPE          = 4,
    ERR_EXPECTED_CLOSING_QUOTE      = 5,
    ERR_INVALID_CHAR_IN_IDENT       = 6,
    ERR_EXPECTED_COMMA              = 7,
    ERR_EXPECTED_COLON              = 8,
    ERR_EXPECTED_LPAREN             = 9,
    ERR_EXPECTED_RPAREN             = 10,
    ERR_FILE_NOT_FOUND              = 11,


};


//----------------------------------------------------------------------------------------
// Annotations.
//
//----------------------------------------------------------------------------------------
enum AnnotationType {

    ANN_ERROR,
    ANN_WARNING,
    ANN_NOTE,
    ANN_INFO
};

struct Annotation {

    Annotation      *next;
    AnnotationType  type;

    char           fileName[ MAX_FILE_PATH_SIZE ];
    unsigned       srcLineNum;
    unsigned       srcLineCol;
    unsigned       length;

    unsigned       errorId;

    

    //  char           message[ MAX_ERROR_MESSAGE_SIZE ];
};

//----------------------------------------------------------------------------------------
// Forwards.
//
//----------------------------------------------------------------------------------------
struct ObjDesc;
struct ScopeDesc;

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
struct TypeDesc {

    TypeDescType    typ;


};

struct ArrayTypeDesc : public TypeDesc {

    TypeDesc        *elemTypeDesc;
    int             elemCount;

};

struct RecordTypeDesc : public TypeDesc {

    ObjDesc         *fieldList;

};

struct PointerTypeDesc : public TypeDesc {

    TypeDesc        *pointedTypeDesc;

};

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
struct ObjDesc {

    ObjDescType     typ;
    char            *name;
    TypeDesc        *typeDesc;

    ObjDesc         *next;  // or a binary tree ?


};

struct ModuleObjDesc : public ObjDesc {

    // ??? visibility

    ScopeDesc       *scopeDesc;

    

};

struct FunctionObjDesc : public ObjDesc {

    // ??? visibility
    // ??? fileInfo

    ObjDesc         *paramList;
    ScopeDesc       *scopeDesc;

};

struct VarObjDesc : public ObjDesc {

    // ??? offset
    // ??? size
    // ??? align 
    // ??? fileInfo

};

struct TypeObjDesc : public ObjDesc {

    TypeDesc        *typeDesc;

};

struct ConstObjDesc : public ObjDesc {

    // ??? value
    // ??? type 

};

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
struct ScopeDesc {

    ScopeDescType  typ;
    ObjDesc        *objTree;

    // ??? attributes ...

};

//----------------------------------------------------------------------------------------
// Expressions work on an item descriptor.
//
//----------------------------------------------------------------------------------------
struct ItemDesc {

    ItemDescType    typ;

    
};


//----------------------------------------------------------------------------------------
// Function prototypes. We declare them here so every file can use them. Local functions
// are kept in the file anonymous name space.
//
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// Command line processing.
//
//----------------------------------------------------------------------------------------
CmdLineResult processCmdLineOptions( int argc, char *argv[ ] );

const char      *getSourceFile( );
const char      *getLogFile( );
const char      *getObjectFile( );
const char      *getAsmFile( );
bool            getVerboseFlag( );


//----------------------------------------------------------------------------------------
//
// Memory arenas.
//
//----------------------------------------------------------------------------------------

struct Arena;

Arena *arenaCreate( size_t blockSize );
void   arenaDestroy( Arena *arena );
void   arenaReset( Arena *arena );

void  *arenaAlloc( Arena *arena, size_t size );
void  *arenaAllocZero( Arena *arena, size_t size );

//----------------------------------------------------------------------------------------
// Annotation management.
//
//----------------------------------------------------------------------------------------
void annotationsInit( );
void annotationsDone( );

void compErr( unsigned errorId, 
              unsigned srcLineNum,
              unsigned srcLineColumn,
              unsigned length, 
              const char *message );

void compWarn( unsigned errorId, 
               unsigned srcLineNum,
               unsigned srcLineColumn,
               unsigned length, 
               const char *message );    

void compNote( unsigned errorId, 
               unsigned srcLineNum,
               unsigned srcLineColumn,
               unsigned length, 
               const char *message );

void compInfo( unsigned errorId, 
               unsigned srcLineNum,
               unsigned srcLineColumn,
               unsigned length, 
               const char *message );

unsigned       annotationsGetCount( );
unsigned       annotationsGetErrorCount( );
unsigned       annotationsGetWarningCount( );
unsigned       annotationsGetNoteCount( );
unsigned       annotationsGetInfoCount( );

const Annotation *annotationsGetFirst( );

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void            openSrcFile( const char *filePath );
void            closeSrcFile( );

const char      *srcFileGetPath( );
const char      *srcFileGetName( );

char            srcFileGetChar( );
unsigned        srcFileGetLineNum( );
unsigned        srcFileGetCharIndex( );

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void            initScanner( );
void            nextToken( );
TokenId         tokenId( );
bool            isToken( TokenId  id );

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
ScopeDesc       *makeScopeDesc( );
ObjDesc         *makeObjDesc( );
TypeDesc        *makeTypeDesc( );
char            *makeStr( const char *inputStr );

void            freeScopeDesc( ScopeDesc *scopeDesc );
void            freeObjDesc( ObjDesc *objDesc );
void            freeTypeDesc( TypeDesc *typeDesc ); 

void            addToScope( ScopeDesc *scopeDesc, ObjDesc *objDesc );

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void            acceptComma( );
void            acceptColon( );
void            acceptLparen( );
void            acceptRparen( );