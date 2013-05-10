// File_Pmp - Info for PMP  files
// Copyright (C) 2013-2013 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Contributor: Lionel Duchateau, kurtnoise@free.fr
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Pre-compilation
#include "MediaInfo/PreComp.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Setup.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_PMP_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Pmp.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Info
//***************************************************************************

//---------------------------------------------------------------------------
const char* Pmp_video_format(int8u video_format)
{
    switch (video_format)
    {
        case   0 : return "MPEG-4 Visual";
        case   1 : return "AVC";
        default  : return "";
    }
}

//---------------------------------------------------------------------------
const char* Pmp_audio_format(int8u audio_format)
{
    switch (audio_format)
    {
        case   0 : return "MPEG Audio";
        case   1 : return "AAC";
        default  : return "";
    }
}

//***************************************************************************
// Buffer - File header
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Pmp::FileHeader_Begin()
{
    //Synchro
    if (Buffer_Offset+4>Buffer_Size)
        return false;
    if (CC4(Buffer+Buffer_Offset)!=0x706D706D) //"pmpm"
        return false;

    return true;
}

//---------------------------------------------------------------------------
void File_Pmp::FileHeader_Parse()
{
     //Parsing
    int32u version, video_format, nb_frames, video_width, video_height, time_base_num, time_base_den, audio_format, sample_rate, channels;

    Skip_C4(                                                    "Signature");
    Get_L4 (version,                                            "Version");
    if (version==1)
    {
		Get_L4 (video_format,                                   "video_format");
		Get_L4 (nb_frames,                                      "number of frames");
		Get_L4 (video_width,                                    "video_width");
		Get_L4 (video_height,                                   "video_height");
		Get_L4 (time_base_num,                                  "time_base_num");
		Get_L4 (time_base_den,                                  "time_base_den");
		Skip_L4(                                                "number of audio streams");
		Get_L4 (audio_format,                                   "audio_format");
		Get_L4 (channels,                                       "channels");
		Skip_L4(                                                "unknown");
		Get_L4 (sample_rate,                                    "sample_rate");
    }

    FILLING_BEGIN();
        Accept("PMP");

        Fill(Stream_General, 0, General_Format, "PMP");

        if (version==1)
        {
            Stream_Prepare(Stream_Video);
            Fill(Stream_Video, 0, Video_Format, Pmp_video_format(video_format));
            Fill(Stream_Video, 0, Video_FrameCount, nb_frames);
            Fill(Stream_Video, 0, Video_Width, video_width);
            Fill(Stream_Video, 0, Video_Height, video_height);
            Fill(Stream_Video, 0, Video_FrameRate, (float)time_base_den / 100);
            Stream_Prepare(Stream_Audio);
            Fill(Stream_Audio, 0, Audio_Format, Pmp_audio_format(audio_format));
            Fill(Stream_Audio, 0, Audio_Channel_s_, channels);
            Fill(Stream_Audio, 0, Audio_SamplingRate, sample_rate);
        }

        //No more need data
        Finish("PMP");
    FILLING_END();
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_PMP_YES
