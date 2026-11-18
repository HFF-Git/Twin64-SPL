//----------------------------------------------------------------------------------------
//
//  T64SPL - A compiler for Twin64 - SymTab
//
//----------------------------------------------------------------------------------------
// ...
//
//----------------------------------------------------------------------------------------
//
// T64SPL - A compiler for Twin64 - SymTab
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

    ObjDesc *comEnvScope;  // "COMP_ENV"

    TypeDesc *booleanType( );
    TypeDesc *byteType( );
    TypeDesc *shortType( );
    TypeDesc *wordType( );
    TypeDesc *intType( );


};

#if 0
    #-------------------------------------------------------------------------------------
    # "enterObj" will add an object to the current scope. We check for duplicates
    # and redefinition of predefined objects.
    # 
    #-------------------------------------------------------------------------------------
    def enterObj( self, scope : ObjDesc, obj : ObjDesc ):

        if ( compEnvScope.lookupLocal( obj.name ) ):
            self.annotate( Msg.ERR_REDEFINE_DEFINED )

        elif ( scope.lookupLocal( obj.name )):
            self.annotate( Msg.ERR_DUPLICATE_IDENT )
        
        else:
            scope.enter( obj )
#endif


ScopeDesc *makeScopeDesc( ) {

}

ObjDesc *makeObjDesc( ) {

}

TypeDesc *makeTypeDesc( ) {

}

char *makeStr( const char *inputStr ) {


}

void freeScopeDesc( ScopeDesc *scopeDesc ) {

}

void freeObjDesc( ObjDesc *objDesc ) {

}

void freeTypeDesc( TypeDesc *typeDesc ) {

}

void addToScope( ScopeDesc *scopeDesc, ObjDesc *objDesc ) {

}




