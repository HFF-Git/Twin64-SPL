//----------------------------------------------------------------------------------------
//
//  T64SPL - A compiler for Twin64 - Scanner
//
//----------------------------------------------------------------------------------------
// The scanner builds token from the source file input.
//
//----------------------------------------------------------------------------------------
//
// T64SPL - A compiler for Twin64 - Scanner
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
// Keyword data structures. The keyword table is organized as an array of key
// words sorted by name length. The order of keywords with the same name length 
// does not matter. The key table index array entries point to the beginning
// of the group of keywords with the same name length. 
//
//----------------------------------------------------------------------------------------
constexpr int MAX_KEY_WORDS         = 100;
constexpr int MAX_KEY_WORD_INDEX    = 16;

struct KeyTableEntry {

    TokenId tokId;
    char    name[ MAX_TOKEN_NAME ];
};

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
char            currentChar                             = ' ';
TokenId         currentTokId                            = TOK_NIL;
int64_t         currentTokIntVal                        = 0;
char            currentTokStrVal[ MAX_TOKEN_STR_SIZE ]  = { 0 };

KeyTableEntry   keyTab[ MAX_KEY_WORDS ];
int             keyTabIndex[ MAX_KEY_WORD_INDEX ];

//----------------------------------------------------------------------------------------
// ??? should rather be in annotation ?

void compErr( CompileErrors errId ) {

    
}

void nextChar( ) {

    currentChar = srcFileGetChar( );
}


//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void upshiftStr( char *str ) {
    
    size_t len = strlen( str );
    
    if ( len > 0 ) {
        
        for ( size_t i = 0; i < len; i++ ) {

            str[ i]  = static_cast<char>(
                            toupper(static_cast<unsigned char>(str[ i ])));
        }
    }
}

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void addChar( char *buf, size_t size, char ch ) {
    
    size_t len = strlen( buf );
    
    if ( len + 1 < size ) {
        
        buf[ len ]     = ch;
        buf[ len + 1 ] = 0;
    }
}

//----------------------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------------------
void appendChar(char *&out, char *end, char c ) {
   
    if ( out >= end )  throw( ERR_STRING_TOO_LONG );
    *out++ = c;
}

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void appendUTF8(char *&out, char *end, uint32_t cp) {

    if ( cp <= 0x7F ) {

        appendChar( out, end, static_cast<char>(cp));
    }
    else if ( cp <= 0x7FF ) {

        appendChar( out, end, static_cast<char>(0xC0 | (cp >> 6)));
        appendChar( out, end, static_cast<char>(0x80 | (cp & 0x3F)));
    }
    else if ( cp <= 0xFFFF ) {

        appendChar( out, end, static_cast<char>(0xE0 | (cp >> 12)));
        appendChar( out, end, static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        appendChar( out, end, static_cast<char>(0x80 | (cp & 0x3F)));
    }
    else if ( cp <= 0x10FFFF ) {

        appendChar( out, end, static_cast<char>(0xF0 | (cp >> 18)));
        appendChar( out, end, static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
        appendChar( out, end, static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        appendChar( out, end, static_cast<char>(0x80 | (cp & 0x3F)));
    }
    else throw( ERR_INVALID_UNICODE_ESCAPE );
}

//----------------------------------------------------------------------------------------
// A little helper to test for a HEX char.
//
//----------------------------------------------------------------------------------------
int hexValue( char c ) {

    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

//----------------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------------
void enterKeyWord( const char *name, TokenId tokId, int *pos ) {

    strncpy( keyTab[ *pos ].name, name, MAX_TOKEN_NAME );
    keyTab[ *pos ].tokId = tokId;
    *pos = *pos + 1;
}

//----------------------------------------------------------------------------------------
//
//
// ??? add a keyword and increment the array index. 
// after all keywords of the same length are entered, store the array index 
// in the keyword index table slot for the length.
//----------------------------------------------------------------------------------------
void buildKeyWordTable( ) {

    keyTabIndex[ 0 ] = 0;
    keyTabIndex[ 1 ] = 0;

    int pos = 0;
    enterKeyWord( "IF", TOK_IF, &pos );


    keyTabIndex[ 2 ] = pos; // ??? where does really point to ?

    enterKeyWord( "ELSE", TOK_ELSE, &pos );


}

//----------------------------------------------------------------------------------------
//
// ??? keywords are case insensitive ?
//----------------------------------------------------------------------------------------
TokenId lookupKeyWordTab( char *name ) {

    size_t index = strlen( name );

    if ( index < MAX_KEY_WORD_INDEX ) {

        // ??? start searching at the index where the keyTabIndex got us.
        // ??? if a match return the tokenId. 
        // ??? we stop at the index that indicates the next group.
    }

    return( TOK_IDENT );
}

//----------------------------------------------------------------------------------------
// "parseNum" will parse a number. We accept decimals, hexadecimals and binary 
// numbers. The numeric string can also contain "_" characters for readability.
// Hex numbers start with "0x", binary with "0b", decimals just with numeric 
// digits.
//----------------------------------------------------------------------------------------
void parseNum( ) {

    int     base        = 10;
    int     maxDigits   = 22;
    int     digits      = 0;
    int64_t tmpVal      = 0;

    currentTokId        = TOK_NUM;
    currentTokIntVal    = 0;
    
    if ( currentChar == '0' ) {
        
        nextChar( );
        if (( currentChar == 'X' ) || ( currentChar == 'x' )) {
            
            base        = 16;
            maxDigits   = 16;
            nextChar( );
        }
        else if (( currentChar == 'B' ) || ( currentChar == 'b' )) {
            
            base        = 2;
            maxDigits   = 64;
            nextChar( );
        }
        else if ( !isdigit( currentChar )) {

            return;
        }
    }
    
    do {
        
        if ( currentChar == '_' ) {
            
            nextChar( );
            continue;
        }
        else {

            if ( isdigit( currentChar )) {

                int digit = currentChar - '0';
                if ( digit >= base ) throw ( ERR_INVALID_NUM );

                tmpVal = ( tmpVal * base ) + digit;
            }
            else if (( base == 16         ) && 
                     ( currentChar >= 'a' ) && 
                     ( currentChar <= 'f' )) {

                tmpVal = ( tmpVal * base ) + currentChar - 'a' + 10;            
            }
            else if (( base == 16         ) && 
                     ( currentChar >= 'A' ) && 
                     ( currentChar <= 'F' )) {

                tmpVal = ( tmpVal * base ) + currentChar - 'A' + 10;
            }      
            else throw ( ERR_INVALID_NUM );
            
            nextChar( );
            digits ++;
            
            if ( digits > maxDigits ) throw ( ERR_INVALID_NUM );
        }
    }
    while (
        ( currentChar == '_' ) ||
        ( isdigit( currentChar )) ||
        ( base == 16 &&
          (( currentChar >= 'a' && currentChar <= 'f' ) ||
           ( currentChar >= 'A' && currentChar <= 'F' ) ) 
        )
    );

    currentTokIntVal = tmpVal;
}

//----------------------------------------------------------------------------------------
// "parseHex2" gets a two digit hex number.
//
//----------------------------------------------------------------------------------------
uint32_t parseHex2( ) {

    uint32_t value = 0;

    for ( int i = 0; i < 2; i++ ) {

        nextChar( );
        int h = hexValue( currentChar) ;
        if ( h < 0 ) throw( ERR_INVALID_HEX_ESCAPE );
        value = ( value << 4 ) | static_cast<uint32_t> ( h );
    }

    return value;
}

//----------------------------------------------------------------------------------------
// "parseHex4" gets a four digit hex number.
//
//----------------------------------------------------------------------------------------
uint32_t parseHex4( ) {

    uint32_t value = 0;

    for ( int i = 0; i < 4; i++ ) {

        nextChar( );
        int h = hexValue( currentChar );
        if ( h < 0 ) throw( ERR_INVALID_UNICODE_ESCAPE );
        value = ( value << 4 ) | static_cast<uint32_t> ( h );
    }

    return value;
}

//----------------------------------------------------------------------------------------
// "parseHex8" gets an eight digit hex number.
//
//----------------------------------------------------------------------------------------
uint32_t parseHex8( ) {

    uint32_t value = 0;

    for ( int i = 0; i < 8; i++ ) {

        nextChar( );
        int h = hexValue( currentChar );
        if ( h < 0 ) throw( ERR_INVALID_UNICODE_ESCAPE) ;
        value = ( value << 4 ) | static_cast<uint32_t> ( h );
    }

    return value;
}

//----------------------------------------------------------------------------------------
// "parseString" gets a string. We manage special characters inside the string 
// with the "\" prefix. Right now, we do not use strings, so the function is 
// perhaps for the future. We will just parse it, but record no result. One day,
// the entire simulator might use the string functions. Then we need it.
//
//----------------------------------------------------------------------------------------
void parseString( ) {

    currentTokId            = TOK_STR;
    currentTokStrVal[ 0 ]   = '\0';

    char *out = currentTokStrVal;
    char *end = currentTokStrVal + sizeof( currentTokStrVal ) - 1;

    do {

        nextChar( );

        while (( currentChar != 0 ) && ( currentChar != '"' )) {

            if ( currentChar == '\\' ) {

                nextChar( );

                if ( currentChar == 0 )
                    throw( ERR_EXPECTED_CLOSING_QUOTE );

                switch (currentChar) {

                    case 'n': appendChar( out, end, '\n' ); break;
                    case 't': appendChar( out, end, '\t' ); break;
                    case 'r': appendChar( out, end, '\r' ); break;
                    case '\\': appendChar( out, end, '\\' ); break;
                    case '"': appendChar( out, end, '"' ); break;

                    case 'x': {

                        uint32_t v = parseHex2( );
                        appendChar( out, end, static_cast<char> ( v )) ;
                        
                    } break;

                    case 'u': {

                        uint32_t codepoint = parseHex4( );

                        if ( codepoint >= 0xD800 && codepoint <= 0xDFFF )
                            throw( ERR_INVALID_UNICODE_ESCAPE );

                        appendUTF8( out, end, codepoint );
                        
                    } break;

                    case 'U': {

                        uint32_t cp = parseHex8( );
                        appendUTF8( out, end, cp );
                    
                    } break;

                    default: {

                        appendChar( out, end, currentChar );
                    
                    } break;
                }

            } 
            else {
                
                appendChar( out, end, currentChar );
            }

            nextChar( );
        }

        if ( currentChar != '"' ) throw( ERR_EXPECTED_CLOSING_QUOTE );

        nextChar( );
        while ( isspace( currentChar )) nextChar();

    } while ( currentChar == '"' );

    appendChar( out, end, '\0' ); 
}

//----------------------------------------------------------------------------------------
// "parseIdent" parses an identifier. It is a sequence of characters starting 
// with an alpha character. An identifier found in the token table will assume 
// the type and value of the token found. Any other identifier is just an 
// identifier symbol. 
//
//----------------------------------------------------------------------------------------
void parseIdent( ) {
    
    char identBuf[ MAX_TOKEN_STR_SIZE ] = "";
    
    while (( isalnum( currentChar )) || ( currentChar == '_' )) {
        
        addChar( identBuf, sizeof( identBuf ), currentChar );
        nextChar( );
    }

    currentTokId = lookupKeyWordTab( identBuf );

    if ( currentTokId == TOK_NIL ) {
        
        strncpy( currentTokStrVal, identBuf, MAX_TOKEN_STR_SIZE );
        currentTokId = TOK_IDENT;
    }
    else currentTokStrVal[ 0 ] = '\0';
}


}; // namespace


//****************************************************************************************
//****************************************************************************************


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

void initScanner( ) {

    currentChar             = ' ';
    currentTokId            = TOK_NIL;
    currentTokIntVal        = 0;
    currentTokStrVal[ 0 ]   = '\0';

    buildKeyWordTable( );
}

TokenId tokenId( ) {

    return( currentTokId );
}

int64_t tokenNumVal( ) {

    return( currentTokIntVal );
}

char *tokenStr( ) {

    return( currentTokStrVal );
}

bool isToken( TokenId  id ) {

    return( id == currentTokId );
}


//----------------------------------------------------------------------------------------
// "nextToken" is the entry point to the token business. It returns the next 
// token from the input string.
//
//----------------------------------------------------------------------------------------
void nextToken( ) {

    currentTokId = TOK_NIL;
  
    while (( currentChar == ' '  ) || ( currentChar == '\n' )) nextChar( );
    
    if ( isalpha( currentChar )) {
        
       parseIdent( );
    }
    else if ( isdigit( currentChar )) {
        
       parseNum( );
    }
    else if ( currentChar == '"' ) {
        
        parseString( );
    }
    else if ( currentChar == '.' ) {
        
        currentTokId   = TOK_PERIOD;
        nextChar( );
    }
    else if ( currentChar == ':' ) {
    
        currentTokId   = TOK_COLON;
        nextChar( );
    }
    else if ( currentChar == '=' ) {

        nextChar( );
        if ( currentChar == '=' ) {

            currentTokId   = TOK_EQ;
            nextChar( );
        }
        else {
        
            currentTokId   = TOK_EQUAL;
        }
    }
    else if ( currentChar == '!' ) {

        nextChar( );
        if ( currentChar == '=' ) {

            currentTokId   = TOK_NE;
            nextChar( );
        }
        else {
        
            currentTokId   = TOK_LNOT;
        }
    }
    else if ( currentChar == '<' ) {

        nextChar( );
        if ( currentChar == '=' ) {

            currentTokId   = TOK_LE;
            nextChar( );
        }
        else {
        
            currentTokId   = TOK_LT;
        }
    }
    else if ( currentChar == '>' ) {

        nextChar( );
        if ( currentChar == '=' ) {

            currentTokId   = TOK_GE;
            nextChar( );
        }
        else {
        
            currentTokId   = TOK_GT;
        }
    }
    else if ( currentChar == '+' ) {
        
        currentTokId = TOK_PLUS;
        nextChar( );
    }
    else if ( currentChar == '-' ) {
        
        currentTokId    = TOK_MINUS;
        nextChar( );
    }
    else if ( currentChar == '*' ) {
        
        currentTokId    = TOK_MULT;
        nextChar( );
    }
    else if ( currentChar == '/' ) {
        
        currentTokId    = TOK_DIV;
        nextChar( );
    }
    else if ( currentChar == '%' ) {
        
        currentTokId    = TOK_MOD;
        nextChar( );
    }
    else if ( currentChar == '&' ) {

        nextChar( );
        if ( currentChar == '&' ) {

            currentTokId   = TOK_LAND;
            nextChar( );
        }
        else {
        
            currentTokId   = TOK_AND;
        }
    }
    else if ( currentChar == '|' ) {
      
        nextChar( );
        if ( currentChar == '|' ) {

            currentTokId   = TOK_LOR;
            nextChar( );
        }
        else {
            currentTokId   = TOK_OR;
        }
    }
    else if ( currentChar == '^' ) {
        
        currentTokId    = TOK_XOR;
        nextChar( );
    }
    else if ( currentChar == '~' ) {
        
        currentTokId    = TOK_NEG;
        nextChar( );
    }
    else if ( currentChar == '(' ) {
        
        currentTokId    = TOK_LPAREN;
        nextChar( );
    }
    else if ( currentChar == ')' ) {
        
        currentTokId    = TOK_RPAREN;
        nextChar( );
    }
    else if ( currentChar == '[' ) {
        
        currentTokId    = TOK_LBRACK;
        nextChar( );
    }
    else if ( currentChar == ']' ) {
        
        currentTokId    = TOK_RBRACK;
        nextChar( );
    }
    else if ( currentChar == ',' ) {
        
        currentTokId    = TOK_COMMA;
        nextChar( );
    }
    else if ( currentChar == 0 ) {
        
        currentTokId    = TOK_EOS;
    }
    else {
    
        currentTokId = TOK_ERR;
        printf( "invalid ch: %d\n", currentChar );
        compErr ( ERR_INVALID_CHAR_IN_IDENT );
    }
}