//----------------------------------------------------------------------------------------
//
//  T64SPL - A compiler for Twin64 - Parser
//
//----------------------------------------------------------------------------------------
// ...
//
//----------------------------------------------------------------------------------------
//
// T64SPL - A compiler for Twin64 - Parser
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
TypeDesc *booleanType;
TypeDesc *byteType;
TypeDesc *shortType;
TypeDesc *wordType;
TypeDesc *intType;


void parseExpr( ItemDesc *item );
void parseStatementSequence( );
void parseType( );
void parseProcedureCall( ItemDesc *item );


//----------------------------------------------------------------------------------------
void parserError( CompileErrors errId ) {

// ??? link annotate functions...
}

void parserWarning( ) {

// ??? link annotate functions...
}

//----------------------------------------------------------------------------------------
void acceptSymbol( TokenId tokId, CompileErrors err ) {

    if ( tokenId( ) == tokId ) nextToken( );
    else parserError( err );
}

void acceptIdent( ) {

    if ( tokenId( ) == TOK_IDENT ) nextToken( );
    else parserError( ERR_EXPECTED_COMMA_SYM );
}

void acceptComma( ) {

    if ( tokenId( ) == TOK_COMMA ) nextToken( );
    else parserError( ERR_EXPECTED_IDENT_SYM );
}

void acceptColon( ) {

    if ( tokenId( ) == TOK_COLON ) nextToken( );
    else parserError( ERR_EXPECTED_COLON_SYM );
}

void acceptLparen( ) {

    if ( tokenId( ) == TOK_LPAREN ) nextToken( );
    else parserError( ERR_EXPECTED_LPAREN_SYM );
}

void acceptRparen( ) {

    if ( tokenId( ) == TOK_RPAREN ) nextToken( );
    else parserError( ERR_EXPECTED_RPAREN_SYM );
}

void acceptLBracket( ) {

    if ( tokenId( ) == TOK_LPAREN ) nextToken( );
    else parserError( ERR_EXPECTED_LBRACK_SYM );
}

void acceptRBracket( ) {

    if ( tokenId( ) == TOK_RPAREN ) nextToken( );
    else parserError( ERR_EXPECTED_RBRACK_SYM );
}


//----------------------------------------------------------------------------------------
//
//  <ident> [ ":" <ident> ]
//
//  what to do with the identifier parsed ?
//----------------------------------------------------------------------------------------      
void parseQualifiedIdent( ) {

    if ( isToken( TOK_COLON )) {

        nextToken( );

        if ( isToken( TOK_IDENT )) {


        }
    }
} 

//----------------------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------------------
void parseExprList( ) {


}


//----------------------------------------------------------------------------------------
//    
// <designator> -> <qualIdent> { "." <ident> | "[" <exprList> "]" | "^" 
//----------------------------------------------------------------------------------------  
void parseDesignator( ) {

    nextToken( );
    if ( isToken( TOK_IDENT )) nextToken( );

    while ( isToken( TOK_PERIOD ) || 
            isToken( TOK_LBRACK ) ||
            isToken( TOK_MULT )) {

        if ( isToken( TOK_PERIOD )) {

            nextToken( );

            // ??? check ident ...
        }       
        else if ( isToken( TOK_LBRACK )) {

            nextToken( );
            parseExprList( );
            acceptSymbol( TOK_RBRACK, ERR_EXPECTED_RBRACK_SYM );
        }
        else if ( isToken( TOK_MULT )) {

            nextToken( );
        }
    }         
}     

//----------------------------------------------------------------------------------------
//
//  <factor> -> <number> |
//  <ident> |
//  <funcId> “(“ [ argList> ] ")" |
//  "NOT" <factor> |
//  "(" <expr> ")" |
//----------------------------------------------------------------------------------------
void parseFactor( ItemDesc *item ) {

    // ??? sync point on token stream...

    if ( isToken( TOK_INT )) {

        nextToken( );
    }
    else if ( isToken( TOK_STR )) {

        nextToken( );
    }
    else if ( isToken( TOK_IDENT )) {

        //need to check type of ident

        nextToken( );
        if ( isToken( TOK_LPAREN )) {

            parseProcedureCall( item );
        }

        acceptSymbol( TOK_RPAREN, ERR_EXPECTED_RPAREN_SYM );
    }
    else if ( isToken( TOK_NOT )) {

        nextToken( );
        parseFactor( item );
    }
    else if ( isToken( TOK_LPAREN )) {

        nextToken( );
        parseExpr( item );
        acceptSymbol( TOK_RPAREN, ERR_EXPECTED_RPAREN_SYM );
    }
    else {

        // error, unexpected token ...
       
        nextToken( );
    }
}
           
//----------------------------------------------------------------------------------------
//    
//  <term> -> <factor> { <facOp> <factor> }
//  facOp> -> "*" | "/" | "MOD"
//----------------------------------------------------------------------------------------
void parseTerm( ItemDesc *item ) {

    ItemDesc lItem;

    parseFactor( item );

    while (( isToken( TOK_MULT )) ||
            ( isToken( TOK_DIV )) ||
            ( isToken( TOK_MOD ))) {

        nextToken( );
        parseFactor( &lItem );
    }    
}
        
//----------------------------------------------------------------------------------------
//    
//  <simpleExpr> -> [ "+" | "-" ] <term> { <termOp> <term> }
//  <termOp> -> "+" | "-"
//----------------------------------------------------------------------------------------
    void parseSimpleExpr( ItemDesc *item ) {
        
    if ( isToken( TOK_PLUS )) {

        nextToken( );
    }
    else if ( isToken( TOK_MINUS )) {

        nextToken( );
    }
    
    parseTerm( item );

    while (( isToken( TOK_PLUS )) || ( isToken( TOK_MINUS ))) {
        
        nextToken( );
        parseTerm( item );
    }
}

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void parseRelationExpr( ItemDesc *item ) {

    parseSimpleExpr( item );
    
    if (( isToken( TOK_EQ ))  || 
        ( isToken( TOK_NE ))  ||
        ( isToken( TOK_LT ))  || 
        ( isToken( TOK_GT ))  ||
        ( isToken( TOK_LE ))  ||
        ( isToken( TOK_GE ))) {
            
        nextToken( );
        parseSimpleExpr( item );
    }
}

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void parseNotExpr( ItemDesc *item ) {

    if ( isToken( TOK_NOT )) {

        nextToken( );
        parseNotExpr( item );
    }
    else {

        parseRelationExpr( item );
    }
}

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void parseAndExpr( ItemDesc *item ) {
    
    parseNotExpr( item );
            
    while ( isToken( TOK_AND )) {

        nextToken( );
        parseNotExpr( item );
    }
}

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void parseOrExpr( ItemDesc *item ) {

    parseAndExpr( item );
    
    while ( isToken( TOK_OR )) {
        
        nextToken( );
        parseAndExpr( item );
    }
} 

//----------------------------------------------------------------------------------------
//    
//  <expr> -> <simpleExpr> [ <relOp> <simpleExpr> ]
//  <relOp> -> "==" | "!=" | "<" | ">" | "<=" | ">="
//
//  <expr>
//     <orExpr>
//         <andExpr>
//             <notExpr>
//                 <relationExpr>
//                     <simpleExpr>
//                         <term>
//                            <factor>
//
//----------------------------------------------------------------------------------------
void parseExpr( ItemDesc *item ) {

        parseOrExpr( item );
}

//----------------------------------------------------------------------------------------
//    
//<constExpr> -> <expr>
//----------------------------------------------------------------------------------------
void parseConstExpr(ItemDesc *item ) {

    parseExpr( item );
    //test for  constant value ...
}

//----------------------------------------------------------------------------------------
//
//  <assignStmt> -> <ident> ":=" <expr>
//----------------------------------------------------------------------------------------  
void parseAssignStatement(  ) {

    ItemDesc item;

    nextToken( );
    parseDesignator( );
    acceptSymbol( TOK_ASSIGN, ERR_EXPECTED_ASSIGN_SYM );
    parseExpr( &item );
}

//----------------------------------------------------------------------------------------
//
//  <ifStmt> -> "IF" <expr> "THEN" <stmtSeq>
//  { "ELSEIF" <expr> "THEN" <stmtSeq> }
//  [ "ELSE" <stmtSeq> ]
//  "END"
//
//----------------------------------------------------------------------------------------
void parseIfStatement(  ) {

    ItemDesc item; 
   
    acceptSymbol( TOK_LPAREN, ERR_EXPECTED_LPAREN_SYM );

    parseExpr( &item );

    // ??? check for boolean result ... 
    // ??? what about short-circuit evaluation ?
    // ??? perhaps we need something else than just expression ...
        
    acceptSymbol( TOK_RPAREN, ERR_EXPECTED_RPAREN_SYM );
    acceptSymbol( TOK_THEN, ERR_EXPECTED_THEN_SYM );

    parseStatementSequence( );

    while ( isToken( TOK_ELSEIF )) {

        nextToken( );
        acceptSymbol( TOK_LPAREN, ERR_EXPECTED_LPAREN_SYM );

        parseExpr( &item );

        // ??? check for boolean result ... 
        // ??? what about short-circuit evaluation ?
        // ??? perhaps we need something else than just expression ...
        
        acceptSymbol( TOK_RPAREN, ERR_EXPECTED_RPAREN_SYM );
        acceptSymbol( TOK_THEN, ERR_EXPECTED_THEN_SYM );

        parseStatementSequence( );
    }
        
    if ( isToken( TOK_ELSE )) {

        nextToken( );
        parseStatementSequence( );
    }
}

//----------------------------------------------------------------------------------------
//
//  <caseList> -> ....
//  <caseListItem> -> { "WHEN" <constExpr> ":" <stmtSeq> "END" }
//----------------------------------------------------------------------------------------  
void parseCaseListItem(  ) {

    ItemDesc item;

    nextToken( );
    acceptSymbol( TOK_WHEN, ERR_EXPECTED_WHEN_SYM );

    parseConstExpr( &item );
    acceptSymbol( TOK_COLON, ERR_EXPECTED_COLON_SYM );
    parseStatementSequence( );
    acceptSymbol( TOK_END, ERR_EXPECTED_END_SYM );
}

//----------------------------------------------------------------------------------------
//
//  <caseList> -> ....
//---------------------------------------------------------------------------------------- 
void parseCaseList(  ) {

}

//----------------------------------------------------------------------------------------
//
//  <caseStmt> -> "CASE" <expr> "OF" <caseList> [ "ELSE" <stmtSeq> ] "END"
//----------------------------------------------------------------------------------------  
void parseCaseStatement( ) {

    ItemDesc item;

    nextToken( );
    parseExpr( &item );
    acceptSymbol( TOK_OF, ERR_EXPECTED_OF_SYM );
    
    parseCaseList( );

    if ( isToken( TOK_ELSE )) {

        nextToken( );
        parseStatementSequence( );
    }

    acceptSymbol( TOK_END, ERR_EXPECTED_END_SYM );   
}

//----------------------------------------------------------------------------------------
//
// 
//  <whileStmt> -> "WHILE" <expr> "DO" <stmtSeq> "END"
//----------------------------------------------------------------------------------------  
void parseWhileStatement(  ) {

    ItemDesc item;

    nextToken( );
    acceptSymbol( TOK_LPAREN, ERR_EXPECTED_LPAREN_SYM );

    parseExpr( &item );
    
    acceptSymbol( TOK_RPAREN, ERR_EXPECTED_RPAREN_SYM );
    acceptSymbol( TOK_DO, ERR_EXPECTED_DO_SYM );
    
    parseStatementSequence( );
    acceptSymbol( TOK_END, ERR_EXPECTED_END_SYM );
}

//----------------------------------------------------------------------------------------
//
//  <repeatStmt> -> "REPEAT" <stmtSeq> "UNTIL" <expr> "END"
//----------------------------------------------------------------------------------------  
void parseRepeatStatement( ) {

    ItemDesc item;

    nextToken( );
    parseStatementSequence( );

    acceptSymbol( TOK_UNTIL, ERR_EXPECTED_UNTIL_SYM );

    parseExpr( &item );   
}

//----------------------------------------------------------------------------------------
//
//  <withStmt> -> "WITH" <designator> "DO" <stmtSeq> "END"
//
// ‚??? not sure yet what the with statement will do ... 
//----------------------------------------------------------------------------------------   
void parseWithStatement( ) {

    nextToken( );
    parseDesignator( );
    acceptSymbol( TOK_DO, ERR_EXPECTED_DO_SYM );
    parseStatementSequence( );
    acceptSymbol( TOK_END, ERR_EXPECTED_END_SYM );
}

//----------------------------------------------------------------------------------------
//
//  <continueStmt> -> "CONTINUE"
//
// ??? only valid in a WHILE, REPEAT statement.
//----------------------------------------------------------------------------------------   
void parseContinueStatement( ) {

    nextToken( );
}

//----------------------------------------------------------------------------------------
//
//  <breakStmt> -> "BREAK"
//
// ??? only valid in a WHILE, REPEAT and LOOP statement.
//----------------------------------------------------------------------------------------   
 void parseBreakStatement( ) {

    nextToken( );
}

//----------------------------------------------------------------------------------------
//
//  <returnStmt> -> "RETURN"
//
// ??? only valid in procedure body statement.
//----------------------------------------------------------------------------------------   
void parseReturnStatement( ) {

    ItemDesc item;

    nextToken( );
    parseExpr( &item );
}

//----------------------------------------------------------------------------------------
//    
//  <argList> -> "(" [ <expr> ] { "," <expr> } ")"
//----------------------------------------------------------------------------------------  
void parseArgList( ) {

    ItemDesc item;

    acceptSymbol( TOK_LPAREN, ERR_EXPECTED_LPAREN_SYM );
   
    if( ! isToken( TOK_RPAREN )) {

        parseExpr( &item );

        while ( isToken( TOK_COMMA )) {

            nextToken( );
            parseExpr( &item );
        }
    }
    
    acceptSymbol( TOK_RPAREN, ERR_EXPECTED_RPAREN_SYM );
}

//----------------------------------------------------------------------------------------
//    
//  <procCall> -> <ident> [ argList ]
//----------------------------------------------------------------------------------------  
void parseProcedureCall( ItemDesc *item ) {

    nextToken( );
    parseArgList( );
}

//----------------------------------------------------------------------------------------
//
//
//  <stmtSeq> -> <stmt> { å<stmt> }
//
//  <stmt> -> <assignStmt> |
//  <ifStmt> |
//  <caseStmt> |
//  <whileStmt> |
//  <repeatStmt> |
//  <withStmt> |
//  <continueStmt> |å
//  <breakStmt> |
//  <exitStmt> |
//  <returnStmt>
//----------------------------------------------------------------------------------------  
void parseStatementSequence( ) {

    while ( true ) {

        if ( isToken( TOK_IDENT ))
            parseAssignStatement( );
        else if ( isToken( TOK_IF ))   
            parseIfStatement( );
        else if ( isToken( TOK_CASE ))
            parseCaseStatement( );
        else if ( isToken( TOK_WHILE )) 
            parseWhileStatement( );
        else if ( isToken( TOK_REPEAT ))
            parseRepeatStatement( );
        else if ( isToken( TOK_WITH ))
            parseWithStatement( );
        else if ( isToken( TOK_CONTINUE ))
            parseContinueStatement( );
        else if ( isToken( TOK_BREAK ))
            parseBreakStatement( );
        else if ( isToken( TOK_RETURN ))
            parseReturnStatement( );

        // ??? how to skip to a valid beginning ?
        // ??? how to end ???
    }
}

//----------------------------------------------------------------------------------------
// 
//  <arrayType> -> ARRAY <constExpr> { "," <constExpr> } OF <type> ";"
//
// ??? a recursive thing to handle array dimensions ?
//----------------------------------------------------------------------------------------  
void parseArrayType( ) {

    ItemDesc item;

    parseConstExpr( &item );

    while ( isToken( TOK_COMMA )) {

        nextToken( );
        parseExpr( &item );
    }

    acceptSymbol( TOK_OF, ERR_EXPECTED_OF_SYM );
    parseType( );        
}

//----------------------------------------------------------------------------------------
//
//
//  <fieldList> -> { <ident> ":" <type> }
//----------------------------------------------------------------------------------------  
void parseFieldList( ) {

    while ( isToken( TOK_IDENT )) {
        
        nextToken( );
        acceptSymbol( TOK_COLON, ERR_EXPECTED_COLON_SYM );

        parseType( );
    }
}
    
//----------------------------------------------------------------------------------------
//
//
//  <recordType> -> "RECORD" [ "(" <ident> ")" ] <fieldList> "END"
//----------------------------------------------------------------------------------------  
void parseRecordType( ) {

    if ( isToken( TOK_LPAREN )) {

        nextToken( );
        if ( isToken( TOK_IDENT )) {

            nextToken( );
        }
        else {

        }

        acceptSymbol( TOK_RPAREN, ERR_EXPECTED_RPAREN_SYM );
    }

    parseFieldList( );

    acceptSymbol( TOK_END, ERR_EXPECTED_END_SYM );
}

//----------------------------------------------------------------------------------------
//
//
//  <pointerType> -> "POINTER" "TO" <type>
//----------------------------------------------------------------------------------------  
void parsePointerType( ) {

    nextToken( );
    acceptSymbol( TOK_TO, ERR_EXPECTED_TO_SYM );

    parseType( );
}

//---------------------------------------------------------------------------------------- 
//
//
//  <stringType> -> "STRING" <constExpr>
//----------------------------------------------------------------------------------------  
void parseStringType( ) {


}

//----------------------------------------------------------------------------------------
// 
//  <type> -> <scalarType> | <structuredType>
//  <scalarType> -> "BYTE" | "SHORT" | "WORD" | "DOUBLE" | "INT"
//  <structuredType> -> <arrayType> | <recordType> | <stringType> | <pointerType>
//
//  ??? should we return a type descriptor ?
//  ??? what is the scope passed ?
//----------------------------------------------------------------------------------------  
void parseType( ) {

    TypeDesc tDesc;

    //??? need to create a type descriptor or let the handler do it ?

    if ( isToken( TOK_IDENT )) {

        // ??? lookup type ...
        nextToken( );
    }

    else if ( isToken( TOK_ARRAY )) {

        nextToken( );
        parseArrayType( );
    }
    else if ( isToken( TOK_RECORD )) {

        nextToken( );
        parseRecordType( );
    }
    else if ( isToken( TOK_STRING )) {

        nextToken( );
        parseStringType( );
    }
    else if ( isToken( TOK_POINTER )) {

        nextToken( );
        parsePointerType( );
    }
    else {

        // ??? error... expected a type declaration...
    }
}

//----------------------------------------------------------------------------------------
//
//
//  <constDecl> -> { <ident> "=" <expr> }+
//---------------------------------------------------------------------------------------- 
void parseConstDeclaration( ) {

    ItemDesc item;

    while ( isToken( TOK_IDENT )) {

        // ??? create a const object... enter into scope

        nextToken( );
        acceptSymbol( TOK_EQ, ERR_EXPECTED_EQ_SYM );
        parseConstExpr( &item ); 

        // check expression result
        // get the expression value to the object ...
    }
}
           
//----------------------------------------------------------------------------------------
//
//
// <typeDecl> -> { <ident> "=" <type> ";" }+
//---------------------------------------------------------------------------------------- 
void parseTypeDeclaration( ) {

    while ( isToken( TOK_IDENT )) {

        // ??? create type object, enter in scope

        nextToken( );
        acceptSymbol( TOK_EQ, ERR_EXPECTED_EQ_SYM );
        parseType( );  
    }
}

//----------------------------------------------------------------------------------------
//
//
//  <varDecl> -> { <ident> ":" <type> [ ":=" <constExpr> ] }+
//---------------------------------------------------------------------------------------- 
void parseVarDeclaration( ) {

    while ( isToken( TOK_IDENT )) {

        // ??? create VAR object, enter in scope ...

        nextToken( );
        acceptSymbol( TOK_COLON, ERR_EXPECTED_COLON_SYM );

        parseType( );

        if ( isToken( TOK_ASSIGN )) {

            ItemDesc item;

            nextToken( );
            parseConstExpr( &item );
        }
    }
}
   
//----------------------------------------------------------------------------------------
//
//  "BEGIN" <statementSeq> "END"
//----------------------------------------------------------------------------------------
void parseProcedureBody( ) {

    acceptSymbol( TOK_BEGIN, ERR_EXPECTED_BEGIN_SYM );
    parseStatementSequence( );
    acceptSymbol( TOK_END, ERR_EXPECTED_END_SYM );
}

//----------------------------------------------------------------------------------------
//
//
//  <parmList> -> { <ident> ":" <type> { "," <ident> ":" <type> } }
//----------------------------------------------------------------------------------------
void parseParmList( ) {

    while ( true ) {

        if ( isToken( TOK_IDENT )) {

            // ??? allocate parameter variable in scope...
        }
        else {
           
            // ??? error, expected ident.
        }

        acceptSymbol( TOK_COLON, ERR_EXPECTED_COLON_SYM );
        parseType( );

        if ( isToken( TOK_COMMA )) {

            nextToken( );
        }
        else if ( isToken( TOK_RPAREN )) {

            return;
        }
        else {

            // ??? error, expected an ident.
            // ??? return then ?
        }
    }
}
            
//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void parseProcedureDeclaration( ) {
    
    while( true ) {

        if ( isToken( TOK_CONST )) {

            nextToken( );
            parseConstDeclaration( );
        }
        else if ( isToken( TOK_TYPE )) {

            nextToken( );
            parseTypeDeclaration( );
        }
        else if ( isToken( TOK_VAR )) {

            nextToken( );
            parseVarDeclaration( );
        } 
        else if ( isToken( TOK_BEGIN )) {

            parseProcedureBody( );
        }
        else {

            // ??? expected procedure declaration ... 
            // ??? ship to where ?
        }
    }
}

//----------------------------------------------------------------------------------------
//
//
//  <procDecl> -> "PROCEDURE" <ident> "(" <parmList> ")" [ ":" <type> ]
//  [ <optionList> ]
//  [ <declaration> ]
//  "BEGIN"
//  <stmtSeq>
//  "END"
//----------------------------------------------------------------------------------------
void parseProcedure( ) {

    if ( isToken( TOK_IDENT )) {

        // ??? create procedure object ...
    }
    else {


    }
    
    nextToken( );
    acceptSymbol( TOK_LPAREN, ERR_EXPECTED_RPAREN_SYM );

    parseParmList( );
    
    acceptSymbol( TOK_RPAREN, ERR_EXPECTED_RPAREN_SYM );

    if ( isToken( TOK_COLON )) {

        nextToken( );
        parseType( );
    }

    acceptSymbol( TOK_SEMIKOLON, ERR_EXPECTED_SEMIKOLON_SYM );
    nextToken( );

    parseProcedureDeclaration( );
    parseProcedureBody( );
}

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void parseModuleBody( ) {

    while ( true ) {

        acceptSymbol( TOK_BEGIN, ERR_EXPECTED_BEGIN_SYM );

        parseStatementSequence( );

        acceptSymbol( TOK_END, ERR_EXPECTED_END_SYM );
    }

    // ??? make sure we sync...
}

//----------------------------------------------------------------------------------------
//
//
//  <moduleDeclaration> -> { 
//      <const-declaration> 
//      <type-declaration>
//      <var-declaration>
//      <proc-declaration>
//  }
//----------------------------------------------------------------------------------------
void parseModuleDeclaration( ) {

    while ( true ) {

        if ( isToken( TOK_CONST )) {

            nextToken( );
            parseConstDeclaration( );
        }
        else if ( isToken( TOK_TYPE )) {

            nextToken( );
            parseTypeDeclaration( );
        }
        else if ( isToken( TOK_VAR )) {

            nextToken( );
            parseVarDeclaration( );
        } 
        else if ( isToken( TOK_PROCEDURE )) {

            nextToken( );
            parseProcedure( );
        }
        else {

            // ??? check if we are in body begin sys, else error...
            
        }
    }
}
            
//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void parseModuleOptions( ) {

    nextToken( );

    // ??? tbd ...
}

//----------------------------------------------------------------------------------------
//
//
//  <moduleDecl> -> "MODULE" <ident> 
//  <moduleDeclaration> 
//  "BEGIN"
//  <stmtSeq>
//  "END"
//----------------------------------------------------------------------------------------
void parseModule( ) {

    if ( isToken( TOK_IDENT )) {

        // ??? allocate module object, else error...
    }

    nextToken( );

    parseModuleOptions( );
    parseModuleDeclaration( );
    parseModuleBody( );
}

//----------------------------------------------------------------------------------------
//
//
//  <importDecl> ->  "IMPORT" <ident> { "," <ident> }
//----------------------------------------------------------------------------------------
void parseImporDeclaration( ) {

    nextToken( );

    while ( true ) {

        if ( isToken( TOK_IDENT )) {

            // ??? process imports
        }
        else {

            // ??? error, expected module name 
        }
           
        nextToken( );

        if ( isToken( TOK_COMMA )) {

            nextToken( );
        }
        else {

            // ??? we have something else and are done ...

        }
    }
}

//----------------------------------------------------------------------------------------
//The compiler accepts a rich set of options. An option can appear at the 
//compile unit level, the module level, the procedure level and also between
//statements. An option itself will not generate any code, it will just 
//annotate and direct the compilation process.
//
//  <optionList> -> { <ident> [ "=" <constExpr> ] }
//----------------------------------------------------------------------------------------
void parseOptionList( ) {

    while ( isToken( TOK_IDENT )) {

        ItemDesc item;
        
        nextToken( );
        acceptSymbol( TOK_EQ, ERR_EXPECTED_EQ_SYM );

        parseConstExpr( &item );
    }
}

//----------------------------------------------------------------------------------------
// The compilation unit is created at the scope of the outer scope compilation
// environment. As source file can have one of more module declarations.
//
//  <compilationUnit> -> { <moduleDeclaration> }+
//----------------------------------------------------------------------------------------
void parseCompilationUnit( ) {

    nextToken( );

    // ??? parse imports before the loop ? 

    while ( true ) {

        if ( isToken( TOK_MODULE )) {

            nextToken( );
            parseModule( );
        }
        else if ( isToken( TOK_OPTION )) {
            
            nextToken( );
            parseOptionList( );
        }
        else if( isToken( TOK_EOF )) {

        }
        else {

            // ??? error, expected module declaration...
        }
    }
}
    
//----------------------------------------------------------------------------------------
//The compilation environment is the outer scope. It contains the predefined 
//objects of types, constants and procedures.
//
//----------------------------------------------------------------------------------------
void setupCompEnv( ) {

    // ??? create predefined types and const...
}

}; // name space



//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void parserInit( ) {


}

//----------------------------------------------------------------------------------------
// "parse" is the main routine that is called to parses a source file. It will
// return an object descriptor tree, starting at the compilation environment 
// scope.
//
//----------------------------------------------------------------------------------------
void parse( ) {

    setupCompEnv( );
    parseCompilationUnit( );
}
