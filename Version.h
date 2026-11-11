//----------------------------------------------------------------------------------------
//
//  T64SPL - A compiler for Twin64 - Version
//
//----------------------------------------------------------------------------------------
// ...
//
//----------------------------------------------------------------------------------------
//
// T64SPL - A compiler for Twin64 - Version
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
// The compiler version data. The patch level is incremented for each commit to the 
// main branch.
//
//----------------------------------------------------------------------------------------
const char COMPILER_VERSION[ ] = "A.01.00";
const char COMPILER_GIT_BRANCH[ ] = "main";
const int  COMPILER_PATCH_LEVEL = 180;

#if __APPLE__
const bool COMPILER_ON_APPLE = true;
#else
const bool COMPILER_ON_APPLE = false;
#endif