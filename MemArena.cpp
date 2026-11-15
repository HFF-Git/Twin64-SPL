//----------------------------------------------------------------------------------------
//
//  T64SPL - A compiler for Twin64 - Memory Arenas
//
//----------------------------------------------------------------------------------------
// ...
//
//----------------------------------------------------------------------------------------
//
// T64SPL - A compiler for Twin64 - memory Arenas
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
// Local data for memory arenas.
//
//----------------------------------------------------------------------------------------
namespace {

struct ArenaBlock {

    ArenaBlock *next;

    size_t      size;
    size_t      used;

    alignas( max_align_t ) unsigned char data[ 1 ];
};

const size_t DEFAULT_BLOCK_SIZE = 64 * 1024;

//----------------------------------------------------------------------------------------
// Allocate a new memory block.
//
//----------------------------------------------------------------------------------------
ArenaBlock *newArenaBlock( size_t size ) {

    size_t allocSize = offsetof( ArenaBlock, data ) + size;

    ArenaBlock *block = static_cast<ArenaBlock *>( malloc( allocSize ));

    if ( block == nullptr ) {

        fprintf( stderr, "Fatal error: out of memory\n" );
        exit( 1 );
    }

    block->next = nullptr;
    block->size = size;
    block->used = 0;

    return( block );
}

}; // namespace


//----------------------------------------------------------------------------------------
//
// Arena descriptor.
//
//----------------------------------------------------------------------------------------
struct Arena {

    ArenaBlock  *first;
    ArenaBlock  *current;
    size_t      blockSize;
};

//----------------------------------------------------------------------------------------
//
// Create arena.
//
//----------------------------------------------------------------------------------------
Arena *arenaCreate( size_t blockSize ) {

    Arena *arena = static_cast<Arena *>( malloc( sizeof( Arena )));

    if ( arena == nullptr ) {

        fprintf( stderr, "Fatal error: out of memory\n" );
        exit( 1 );
    }

    arena->first     = nullptr;
    arena->current   = nullptr;
    arena->blockSize = blockSize;

    return( arena );
}

//----------------------------------------------------------------------------------------
// Destroy arena.
//
//----------------------------------------------------------------------------------------
void arenaDestroy( Arena *arena ) {

    if ( arena == nullptr ) return;

    ArenaBlock *block = arena->first;

    while ( block != nullptr ) {

        ArenaBlock *next = block->next;

        free( block );
        block = next;
    }

    free( arena );
}

//----------------------------------------------------------------------------------------
// Reset arena.
//
//----------------------------------------------------------------------------------------
void arenaReset( Arena *arena ) {

    if ( arena == nullptr ) return;

    for ( ArenaBlock *block = arena->first;
          block != nullptr;
          block = block->next ) {

        block->used = 0;
    }

    arena->current = arena->first;
}

//----------------------------------------------------------------------------------------
// Allocate memory from arena.
//
//----------------------------------------------------------------------------------------
void *arenaAlloc( Arena *arena, size_t size ) {

    if ( arena == nullptr || size == 0 ) return( nullptr );

    size_t alignment = alignof( max_align_t );

    size = ( size + alignment - 1 ) & ~( alignment - 1 );


    ArenaBlock *block = arena->current;

    while ( true ) {

        if ( block != nullptr && block->size - block->used >= size ) {

            void *ptr = block->data + block->used;

            block->used += size;
            arena->current = block;

            return( ptr );
        }

        if ( block != nullptr && block->next != nullptr ) {

            block = block->next;
            continue;
        }

        size_t newSize = arena->blockSize;

        if ( newSize < size ) newSize = size;

        ArenaBlock *newBlock = newArenaBlock( newSize );

        if ( arena->first == nullptr ) {

            arena->first = newBlock;
        }
        else {

            ArenaBlock *last = arena->first;

            while ( last->next != nullptr ) last = last->next;

            last->next = newBlock;
        }

        block = newBlock;
    }
}

//----------------------------------------------------------------------------------------
//
// Allocate and clear memory.
//
//----------------------------------------------------------------------------------------
void *arenaAllocZero( Arena *arena, size_t size ) {

    void *ptr = arenaAlloc( arena, size );

    if ( ptr != nullptr ) memset( ptr, 0, size );

    return( ptr );
}