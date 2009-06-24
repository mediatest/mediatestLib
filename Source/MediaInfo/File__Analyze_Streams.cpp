// File__Analyze - Base for analyze files
// Copyright (C) 2007-2009 Jerome Martinez, Zen@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include "MediaInfo/MediaInfo_Internal.h"
#include "MediaInfo/MediaInfo_Config.h"
#include "ZenLib/File.h"
#include "ZenLib/BitStream_LE.h"
#include <cmath>
using namespace std;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
extern MediaInfo_Config Config;
//---------------------------------------------------------------------------

//***************************************************************************
// Preparation des streams
//***************************************************************************

//---------------------------------------------------------------------------
size_t File__Analyze::Stream_Prepare (stream_t KindOfStream)
{
    //Integrity
    if (KindOfStream>Stream_Max)
        return Error;

    //Clear
    if (KindOfStream==Stream_Max)
    {
        StreamKind_Last=Stream_Max;
        StreamPos_Last=(size_t)-1;
        return 0;
    }

    //Add a stream
    (*Stream)[KindOfStream].resize((*Stream)[KindOfStream].size()+1);
    (*Stream_More)[KindOfStream].resize((*Stream_More)[KindOfStream].size()+1);

    StreamKind_Last=KindOfStream;
    StreamPos_Last=(*Stream)[KindOfStream].size()-1;

    //Fill with already ready data
    for (size_t Pos=0; Pos<Fill_Temp.size(); Pos++)
        if (Fill_Temp(Pos, 0).IsNumber())
            Fill(StreamKind_Last, StreamPos_Last, Fill_Temp(Pos, 0).To_int32u(), Fill_Temp(Pos, 1));
        else
            Fill(StreamKind_Last, StreamPos_Last, Fill_Temp(Pos, 0).To_UTF8().c_str(), Fill_Temp(Pos, 1));
    Fill_Temp.clear();

    return (*Stream)[KindOfStream].size()-1; //The position in the stream count
}

//***************************************************************************
// Filling
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, const Ztring &Value, bool Replace)
{
    //Integrity
    if (StreamKind>Stream_Max)
        return;

    //Handling values with \r\n inside
    if (Value.find(_T('\r'))!=string::npos || Value.find(_T('\n'))!=string::npos)
    {
        Ztring NewValue=Value;
        NewValue.FindAndReplace(_T("\r\n"), _T(" / "), 0, Ztring_Recursive);
        NewValue.FindAndReplace(_T("\r"), _T(" / "), 0, Ztring_Recursive);
        NewValue.FindAndReplace(_T("\n"), _T(" / "), 0, Ztring_Recursive);
        if (NewValue.size()>=3 && NewValue.rfind(_T(" / "))==NewValue.size()-3)
            NewValue.resize(NewValue.size()-3);
        Fill(StreamKind, StreamPos, Parameter, NewValue, Replace);
        return;
    }

    //Handle Value before StreamKind
    if (StreamKind==Stream_Max || StreamPos>=(*Stream)[StreamKind].size())
    {
        ZtringList NewList;
        NewList.push_back(Ztring().From_Number(Parameter));
        NewList.push_back(Value);
        Fill_Temp.push_back(NewList);
        return; //No streams
    }

    Ztring &Target=(*Stream)[StreamKind][StreamPos](Parameter);
    if (Target.empty() || Replace)
        Target=Value; //First value
    else if (Value.empty())
        Target.clear(); //Empty value --> clear other values
    else
    {
        Target+=MediaInfoLib::Config.TagSeparator_Get();
        Target+=Value;
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, const Ztring &Value, bool Replace)
{
    //Integrity
    if (StreamKind>Stream_Max || Parameter==NULL || Parameter[0]=='\0')
        return;

    //Handling values with \r\n inside
    if (Value.find(_T('\r'))!=string::npos || Value.find(_T('\n'))!=string::npos)
    {
        Ztring NewValue=Value;
        NewValue.FindAndReplace(_T("\r\n"), _T(" / "), 0, Ztring_Recursive);
        NewValue.FindAndReplace(_T("\r"), _T(" / "), 0, Ztring_Recursive);
        NewValue.FindAndReplace(_T("\n"), _T(" / "), 0, Ztring_Recursive);
        if (NewValue.size()>=3 && NewValue.rfind(_T(" / "))==NewValue.size()-3)
            NewValue.resize(NewValue.size()-3);
        Fill(StreamKind, StreamPos, Parameter, NewValue, Replace);
        return;
    }

    //Handle Value before StreamKind
    if (StreamKind==Stream_Max || StreamPos>=(*Stream)[StreamKind].size())
    {
        ZtringList NewList;
        NewList.push_back(Ztring().From_UTF8(Parameter));
        NewList.push_back(Value);
        Fill_Temp.push_back(NewList);
        return; //No streams
    }

    //Handling of well known parameters
    size_t Pos=MediaInfoLib::Config.Info_Get(StreamKind).Find(Ztring().From_Local(Parameter));
    if (Pos!=Error)
    {
        Fill(StreamKind, StreamPos, Pos, Value, Replace);
        return;
    }

    //Handling of unknown parameters
    if (Value.empty())
    {
        if (Replace)
        {
            size_t Pos=(*Stream_More)[StreamKind][StreamPos].Find(Ztring().From_UTF8(Parameter), Info_Name);
            if (Pos!=(size_t)-1)
                (*Stream_More)[StreamKind][StreamPos].erase((*Stream_More)[StreamKind][StreamPos].begin()+Pos); //Empty value --> remove the line
        }
    }
    else
    {
        Ztring &Target=(*Stream_More)[StreamKind][StreamPos](Ztring().From_UTF8(Parameter), Info_Text);
        if (Target.empty() || Replace)
        {
            Target=Value; //First value
            (*Stream_More)[StreamKind][StreamPos](Ztring().From_UTF8(Parameter), Info_Options)=_T("Y NT");
        }
        else
        {
            Target+=MediaInfoLib::Config.TagSeparator_Get();
            Target+=Value;
        }
    }
}

//---------------------------------------------------------------------------
const Ztring &File__Analyze::Retrieve (stream_t StreamKind, size_t StreamPos, size_t Parameter, info_t KindOfInfo)
{
    //Integrity
    if (StreamKind>=Stream_Max
     || StreamPos>=(*Stream)[StreamKind].size()
     || Parameter>=(*Stream)[StreamKind][StreamPos].size())
        return MediaInfoLib::Config.EmptyString_Get();

    if (KindOfInfo!=Info_Text)
        return MediaInfoLib::Config.Info_Get(StreamKind, Parameter, KindOfInfo);
    return (*Stream)[StreamKind][StreamPos](Parameter);
}

//---------------------------------------------------------------------------
const Ztring &File__Analyze::Retrieve (stream_t StreamKind, size_t StreamPos, const char* Parameter, info_t KindOfInfo)
{
    //Integrity
    if (StreamKind>=Stream_Max
     || StreamPos>=(*Stream)[StreamKind].size()
     || Parameter==NULL
     || Parameter[0]=='\0')
        return MediaInfoLib::Config.EmptyString_Get();

    if (KindOfInfo!=Info_Text)
        return MediaInfoLib::Config.Info_Get(StreamKind, Parameter, KindOfInfo);
    size_t Parameter_Pos=MediaInfoLib::Config.Info_Get(StreamKind).Find(Ztring().From_Local(Parameter));
    if (Parameter_Pos==Error)
    {
        Parameter_Pos=(*Stream_More)[StreamKind][StreamPos].Find(Ztring().From_Local(Parameter));
        if (Parameter_Pos==Error)
            return MediaInfoLib::Config.EmptyString_Get();
        return (*Stream_More)[StreamKind][StreamPos](Parameter_Pos, 1);
    }
    return (*Stream)[StreamKind][StreamPos](Parameter_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Clear (stream_t StreamKind, size_t StreamPos, const char* Parameter)
{
    //Integrity
    if (StreamKind>=Stream_Max
     || StreamPos>=(*Stream)[StreamKind].size()
     || Parameter==NULL
     || Parameter[0]=='\0')
        return;

    size_t Parameter_Pos=MediaInfoLib::Config.Info_Get(StreamKind).Find(Ztring().From_Local(Parameter));
    if (Parameter_Pos==Error)
    {
        Parameter_Pos=(*Stream_More)[StreamKind][StreamPos].Find(Ztring().From_Local(Parameter));
        if (Parameter_Pos==Error)
            return;
        (*Stream_More)[StreamKind][StreamPos](Parameter_Pos, 1).clear();
    }

    (*Stream)[StreamKind][StreamPos](Parameter_Pos).clear();
}

//---------------------------------------------------------------------------
void File__Analyze::Clear (stream_t StreamKind, size_t StreamPos, size_t Parameter)
{
    //Integrity
    if (StreamKind>=Stream_Max
     || StreamPos>=(*Stream)[StreamKind].size())
        return;

    //Normal
    if (Parameter<(*Stream)[StreamKind][StreamPos].size())
    {
        (*Stream)[StreamKind][StreamPos][Parameter].clear();
        return;
    }

    //More
    Parameter-=(*Stream)[StreamKind][StreamPos].size(); //For having Stream_More position
    if (Parameter<(*Stream_More)[StreamKind][StreamPos].size())
    {
        (*Stream_More)[StreamKind][StreamPos].erase((*Stream_More)[StreamKind][StreamPos].begin()+Parameter);
        return;
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Clear (stream_t StreamKind, size_t StreamPos)
{
    //Integrity
    if (StreamKind>=Stream_Max
     || StreamPos>=(*Stream)[StreamKind].size())
        return;

    (*Stream)[StreamKind].erase((*Stream)[StreamKind].begin()+StreamPos);
}

//---------------------------------------------------------------------------
void File__Analyze::Clear (stream_t StreamKind)
{
    //Integrity
    if (StreamKind>=Stream_Max)
        return;

    (*Stream)[StreamKind].clear();
}

//---------------------------------------------------------------------------
void File__Analyze::Fill_Flush()
{
    Stream_Prepare(Stream_Max); //clear filling
    Fill_Temp.clear();
}

//---------------------------------------------------------------------------
size_t File__Analyze::Merge(MediaInfo_Internal &ToAdd, bool Erase)
{
    size_t Count=0;
    for (size_t StreamKind=(size_t)Stream_General; StreamKind<(size_t)Stream_Max; StreamKind++)
    {
        size_t StreamPos_Count=ToAdd.Count_Get((stream_t)StreamKind);
        for (size_t StreamPos=0; StreamPos<StreamPos_Count; StreamPos++)
        {
            //Prepare a new stream
            Stream_Prepare((stream_t)StreamKind);

            //Merge
            size_t Pos_Count=ToAdd.Count_Get((stream_t)StreamKind, StreamPos);
            for (size_t Pos=0; Pos<Pos_Count; Pos++)
            {
                Fill((stream_t)StreamKind, StreamPos, Ztring(ToAdd.Get((stream_t)StreamKind, StreamPos, Pos, Info_Name)).To_UTF8().c_str(), ToAdd.Get((stream_t)StreamKind, StreamPos, Pos), true);
            }

            //Clearing duplicates
            Clear((stream_t)StreamKind, StreamPos, "Count");
            Clear((stream_t)StreamKind, StreamPos, "StreamCount");
            Clear((stream_t)StreamKind, StreamPos, "StreamKind");
            Clear((stream_t)StreamKind, StreamPos, "StreamKind/String");
            Clear((stream_t)StreamKind, StreamPos, "StreamKindID");
            Clear((stream_t)StreamKind, StreamPos, "StreamKindPos");

            if ((stream_t)StreamKind==Stream_General)
            {
                Clear(Stream_General, StreamPos, (size_t)General_GeneralCount);
                Clear(Stream_General, StreamPos, (size_t)General_VideoCount);
                Clear(Stream_General, StreamPos, (size_t)General_AudioCount);
                Clear(Stream_General, StreamPos, (size_t)General_TextCount);
                Clear(Stream_General, StreamPos, (size_t)General_ChaptersCount);
                Clear(Stream_General, StreamPos, (size_t)General_ImageCount);
                Clear(Stream_General, StreamPos, (size_t)General_MenuCount);
                Clear(Stream_General, StreamPos, (size_t)General_CompleteName);
                Clear(Stream_General, StreamPos, (size_t)General_FolderName);
                Clear(Stream_General, StreamPos, (size_t)General_FileName);
                Clear(Stream_General, StreamPos, (size_t)General_FileExtension);
                Clear(Stream_General, StreamPos, (size_t)General_File_Created_Date);
                Clear(Stream_General, StreamPos, (size_t)General_File_Created_Date_Local);
                Clear(Stream_General, StreamPos, (size_t)General_File_Modified_Date);
                Clear(Stream_General, StreamPos, (size_t)General_File_Modified_Date_Local);
            }

            Count++;
        }
    }

    return Count;
}

//---------------------------------------------------------------------------
size_t File__Analyze::Merge(File__Analyze &ToAdd, bool Erase)
{
    size_t Count=0;
    for (size_t StreamKind=(size_t)Stream_General+1; StreamKind<(size_t)Stream_Max; StreamKind++)
        for (size_t StreamPos=0; StreamPos<(*ToAdd.Stream)[StreamKind].size(); StreamPos++)
        {
            //Prepare a new stream
            Stream_Prepare((stream_t)StreamKind);

            //Merge
            Merge(ToAdd, (stream_t)StreamKind, StreamPos, StreamPos_Last, Erase);

            Count++;
        }
    return Count;
}

//---------------------------------------------------------------------------
size_t File__Analyze::Merge(File__Analyze &ToAdd, stream_t StreamKind, size_t StreamPos_From, size_t StreamPos_To, bool Erase)
{
    //Integrity
    if (&ToAdd==NULL || StreamKind>=Stream_Max || !ToAdd.Stream || StreamPos_From>=(*ToAdd.Stream)[StreamKind].size())
        return 0;

    //Destination
    while (StreamPos_To>=(*Stream)[StreamKind].size())
        Stream_Prepare(StreamKind);

    //Specific stuff
    Ztring FrameRate_Temp, PixelAspectRatio_Temp, DisplayAspectRatio_Temp;
    if (StreamKind==Stream_Video)
    {
        PixelAspectRatio_Temp=Retrieve(Stream_Video, StreamPos_Last, Video_PixelAspectRatio); //We want to keep the PixelAspectRatio_Temp of the video stream
        DisplayAspectRatio_Temp=Retrieve(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio); //We want to keep the DisplayAspectRatio_Temp of the video stream
        FrameRate_Temp=Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate); //We want to keep the FrameRate of AVI 120 fps
    }

    //Merging
    size_t Count=0;
    size_t Size=ToAdd.Count_Get(StreamKind, StreamPos_From);
    for (size_t Pos=0; Pos<Size; Pos++)
    {
        const Ztring &ToFill_Value=ToAdd.Get(StreamKind, StreamPos_From, Pos);
        if (!ToFill_Value.empty() && (Erase || Get(StreamKind, StreamPos_From, Pos).empty()))
        {
            if (Pos<MediaInfoLib::Config.Info_Get(StreamKind).size())
                Fill(StreamKind, StreamPos_To, Pos, ToFill_Value, true);
            else
                (*Stream_More)[StreamKind][StreamPos_To].push_back((*ToAdd.Stream_More)[StreamKind][StreamPos_From].Read(Pos-MediaInfoLib::Config.Info_Get(StreamKind).size()));
            Count++;
        }
    }

    //Specific stuff
    if (StreamKind==Stream_Video)
    {
        if (!PixelAspectRatio_Temp.empty() || !DisplayAspectRatio_Temp.empty())
        {
            if (PixelAspectRatio_Temp!=Retrieve(Stream_Video, StreamPos_Last, Video_PixelAspectRatio))
                Fill(Stream_Video, StreamPos_Last, Video_PixelAspectRatio_Original, Retrieve(Stream_Video, StreamPos_Last, Video_PixelAspectRatio), true);
            Fill(Stream_Video, StreamPos_Last, Video_PixelAspectRatio, PixelAspectRatio_Temp, true);
        }
        if (!DisplayAspectRatio_Temp.empty() || !PixelAspectRatio_Temp.empty())
        {
            if (DisplayAspectRatio_Temp!=Retrieve(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio))
                Fill(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio_Original, Retrieve(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio), true);
            Fill(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio, DisplayAspectRatio_Temp, true);
        }
        if (!FrameRate_Temp.empty())
        {
            if (FrameRate_Temp!=Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate))
                Fill(Stream_Video, StreamPos_Last, Video_FrameRate_Original, Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate), true);
            Fill(Stream_Video, StreamPos_Last, Video_FrameRate, FrameRate_Temp, true);
        }
    }

    return 1;
}

} //NameSpace

