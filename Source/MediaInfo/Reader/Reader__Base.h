// Reader_File - All information about media files
// Copyright (C) 2002-2010 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Give information about a lot of media files
// Dispatch the file to be tested by all containers
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef Reader__BaseH
#define Reader__BaseH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/MediaInfo_Internal.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
/// @brief Reader__Base
//***************************************************************************

class Reader__Base
{
public :
    //Constructor/Destructor
    virtual ~Reader__Base() {}

    //Format testing
    virtual size_t Format_Test(MediaInfo_Internal* MI, const String &File_Name)=0;
    virtual size_t Format_Test_PerParser_Continue (MediaInfo_Internal* MI) {return 0;};
    virtual size_t Format_Test_PerParser_Seek (MediaInfo_Internal* MI, size_t Method, int64u Value) {return 0;};
};

} //NameSpace
#endif
