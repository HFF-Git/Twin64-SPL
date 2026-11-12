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
const int MAX_TOKEN_NAME = 32;
const int MAX_FILE_PATH_SIZE = 256;


// ??? options and flags...

//----------------------------------------------------------------------------------------
// Command line option argument types and structure. This is the "argc", "argv"
// parser used to parse long options (e.g. --option=value).
//
//----------------------------------------------------------------------------------------
enum SimCmdLineArgOptions : uint32_t {

    CL_OPT_NO_ARGUMENT,         
    CL_OPT_REQUIRED_ARGUMENT,   
    CL_OPT_OPTIONAL_ARGUMENT
};

enum SimCmdLineArgVal : uint32_t {

    CL_ARG_VAL_NIL,             
    CL_ARG_VAL_HELP,             
    CL_ARG_VAL_VERSION,
    CL_ARG_VAL_VERBOSE,        
    CL_ARG_VAL_CONFIG_FILE,      
    CL_ARG_VAL_LOG_FILE
};

struct CmdLineOptions {

    const char              *name;
    SimCmdLineArgOptions    argOpt;   
    int                     val;
};


//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
enum TokenType {

    TOKEN_NONE,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_COMMENT,
    TOKEN_EOF
};

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
enum TokenId {

    TOK_NIL,

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

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
// ??? source file info ...

//----------------------------------------------------------------------------------------
//
// ??? add file info ... 
// ??? how complex does a token need to be ? 
// ??? do we need a kind of class to better check groups of tokens ( e.g. relOp ? )
//----------------------------------------------------------------------------------------
struct TokenDesc {

    TokenId id;

    unsigned lineIndex;
    unsigned charIndex;
    
    union {

        int64_t val;
        char    *str; 

    } u;
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
ScopeDesc   *makeScopeDesc( );
ObjDesc     *makeObjDesc( );
TypeDesc    *makeTypeDesc( );

void        freeScopeDesc( ScopeDesc *scopeDesc );
void        freeObjDesc( ObjDesc *objDesc );
void        freeTypeDesc( TypeDesc *typeDesc ); 

void        addToScope( ScopeDesc *scopeDesc, ObjDesc *objDesc );

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void            initScanner( );
void            openSrcFile( );
void            nextToken( );
TokenDesc       *currentToken( );
bool            isToken( TokenId  id );
void            acceptToken( TokenId id );
void            acceptComma( );
void            acceptColon( );
void            acceptEqual( );
void            acceptLparen( );
void            acceptRparen( );
