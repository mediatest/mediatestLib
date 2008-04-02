//***************************************************************************
// Class File__Base
//***************************************************************************

class File__Analyze : public File__Base
{
public :
    //***************************************************************************
    // Constructor/Destructor
    //***************************************************************************

    File__Analyze();
    virtual ~File__Analyze();

    //***************************************************************************
    // Open
    //***************************************************************************

    int     Open_File (const Ztring &File_Name); //1 if succeed, 0 if problem with format
    void    Open_Buffer_Init        (                    int64u File_Size, int64u File_Offset=0);
    void    Open_Buffer_Init        (File__Analyze* Sub);
    void    Open_Buffer_Init        (File__Analyze* Sub, int64u File_Size, int64u File_Offset=0);
    void    Open_Buffer_Continue    (                    const int8u* Buffer, size_t Buffer_Size);
    void    Open_Buffer_Continue    (File__Analyze* Sub, const int8u* Buffer, size_t Buffer_Size);
    void    Open_Buffer_Finalize    (bool NoBufferModification=false);
    void    Open_Buffer_Finalize    (File__Analyze* Sub);

protected :
    //***************************************************************************
    // Buffer
    //***************************************************************************

    //Buffer
    virtual void Read_Buffer_Init (); //Temp, should be in File__Base caller
    virtual void Read_Buffer_Continue (); //Temp, should be in File__Base caller
    virtual void Read_Buffer_Unsynched (); //Temp, should be in File__Base caller
    virtual void Read_Buffer_Finalize (); //Temp, should be in File__Base caller
    bool Buffer_Parse();

    //***************************************************************************
    // BitStream init
    //***************************************************************************

    void BS_Begin();
    void BS_Begin_LE(); //Little Endian version
    void BS_End();
    void BS_End_LE(); //Little Endian version

    //***************************************************************************
    // File Header
    //***************************************************************************

    //File Header - Management
    bool FileHeader_Manage ();

    //File Header - Begin
    virtual bool FileHeader_Begin ()                                            {return true;};

    //File Header - Parse
    virtual void FileHeader_Parse ()                                            {Element_DoNotShow();};

    //***************************************************************************
    // Header
    //***************************************************************************

    //Header - Management
    bool Header_Manage ();

    //Header - Begin
    virtual bool Header_Begin ()                                                {return true;};

    //Header - Parse
    virtual void Header_Parse ()                                                {};

    //Header - Info
    void Header_Fill_Code (int64u Code);
    inline void Header_Fill_Code (int64u Code, const Ztring &Name) {Header_Fill_Code(Code);};
    void Header_Fill_Size (int64u Size);

    //***************************************************************************
    // Data
    //***************************************************************************

    //Header - Management
    bool Data_Manage ();

    //Data - Parse
    virtual void Data_Parse ()                                                  {};

    //Data - Info
    inline void Data_Info (const Ztring &) {};

    //Data - Get info
    size_t Data_Remain ()                                                       {return (size_t)Element_Size-(Element_Offset+BS->Offset_Get());};
    size_t Data_BS_Remain ()                                                    {return (size_t)BS->Remain();};

    //Data - Detect EOF
    virtual void Detect_EOF ()                                                  {};
    bool EOF_AlreadyDetected;

    //Data - Helpers
    inline void Data_Finnished(const char*)                                     {Data_GoTo(File_Size);}
    void Data_GoTo     (int64u GoTo);
    inline void Data_GoTo     (int64u GoTo, const char*)                        {Data_GoTo(GoTo);}

    //***************************************************************************
    // Elements
    //***************************************************************************

    //Elements - Begin
    void Element_Begin ();
    inline void Element_Begin (const Ztring &Name, int64u Size=(int64u)-1) {Element_Begin(Size);}
    void Element_Begin (int64u Size);
    inline void Element_Begin (const char *Name, int64u Size=(int64u)-1) {Element_Begin(Size);}

    //Elements - Name
    inline void Element_Name (const Ztring &) {}
    inline void Element_Name (const char*) {}

    //Elements - Info
    inline void Element_Info (const Ztring &Parameter) {}
    inline void Element_Info (const char*   Parameter) {}
    inline void Element_Info (const char*   Parameter, const char*   Measure) {}
    inline void Element_Info (int8s         Parameter, const char*   Measure=NULL) {}
    inline void Element_Info (int8u         Parameter, const char*   Measure=NULL) {}
    inline void Element_Info (int16s        Parameter, const char*   Measure=NULL) {}
    inline void Element_Info (int16u        Parameter, const char*   Measure=NULL) {}
    inline void Element_Info (int32s        Parameter, const char*   Measure=NULL) {}
    inline void Element_Info (int32u        Parameter, const char*   Measure=NULL) {}
    inline void Element_Info (int64s        Parameter, const char*   Measure=NULL) {}
    inline void Element_Info (int64u        Parameter, const char*   Measure=NULL) {}
    inline void Element_Info (int128u       Parameter, const char*   Measure=NULL) {}
    #ifdef NEED_SIZET
    inline void Element_Info (size_t        Parameter, const char*   Measure=NULL) {}
    #endif //NEED_SIZET
    inline void Element_Info (float32       Parameter, int8u AfterComma=3, const char*   Measure=NULL) {}
    inline void Element_Info (float64       Parameter, int8u AfterComma=3, const char*   Measure=NULL) {}

    //Elements - End
    void Element_End ();
    inline void Element_End (const Ztring &Name, int64u Size=(int64u)-1) {Element_End(Size);}
    void Element_End (int64u Size);
    inline void Element_End (const char *Name, int64u Size=(int64u)-1) {Element_End(Size);}

    //Elements - Preparation of element from external app
    void Element_Prepare (int64u Size);

protected :
    //Element - Common
    void   Element_End_Common_Flush();
    Ztring Element_End_Common_Flush_Build();
public :

    //***************************************************************************
    // Param
    //***************************************************************************

    //TODO: put this in Ztring()
    Ztring ToZtring(const char* Value, size_t Value_Size=Unlimited, bool Utf8=true)
    {
        if (Utf8)
            return Ztring().From_UTF8(Value, Value_Size);
        else
            return Ztring().From_Local(Value, Value_Size);
    }
    #define VALUE(Value) \
        Ztring::ToZtring(Value, 16).MakeUpperCase()+_T(" (")+Ztring::ToZtring(Value, 10).MakeUpperCase()+_T(")")

    //Param - Main
    inline void Param      (const Ztring &Parameter, const Ztring& Value) {}
    inline void Param      (const char*   Parameter, const Ztring& Value) {}
    inline void Param      (const char*   Parameter, const std::string& Value) {}
    inline void Param      (const char*   Parameter, const char*   Value, size_t Value_Size=Unlimited, bool Utf8=true) {}
    inline void Param      (const char*   Parameter, const int8u*  Value, size_t Value_Size=Unlimited, bool Utf8=true) {}
    inline void Param      (const char*   Parameter, bool   Value) {}
    inline void Param      (const char*   Parameter, int8u  Value) {}
    inline void Param      (const char*   Parameter, int8s  Value) {}
    inline void Param      (const char*   Parameter, int16u Value) {}
    inline void Param      (const char*   Parameter, int16s Value) {}
    inline void Param      (const char*   Parameter, int32u Value) {}
    inline void Param      (const char*   Parameter, int32s Value) {}
    inline void Param      (const char*   Parameter, int64u Value) {}
    inline void Param      (const char*   Parameter, int64s Value) {}
    inline void Param      (const char*   Parameter, int128u Value) {}
    inline void Param_UUID (const char*   Parameter, int128u Value) {}
    #ifdef NEED_SIZET
    inline void Param      (const char*   Parameter, size_t Value, intu Radix=16) {}
    #endif //NEED_SIZET
    inline void Param      (const char*   Parameter, float32 Value, intu AfterComma=3) {}
    inline void Param      (const char*   Parameter, float64 Value, intu AfterComma=3) {}
    inline void Param      (const char*   Parameter, float80 Value, intu AfterComma=3)  {}
    inline void Param      (const int32u  Parameter, const Ztring& Value) {}
    inline void Param      (const int16u  Parameter, const Ztring& Value) {}

    //Param - Info
    inline void Param_Info (const Ztring &Parameter) {}
    inline void Param_Info (const char*   Parameter) {}
    inline void Param_Info (const char*   Parameter, const char*   Measure) {}
    inline void Param_Info (int64u        Parameter, const char*   Measure=NULL) {}
    inline void Param_Info (int64s        Parameter, const char*   Measure=NULL) {}
    inline void Param_Info (int32u        Parameter, const char*   Measure=NULL) {}
    inline void Param_Info (int32s        Parameter, const char*   Measure=NULL) {}
    inline void Param_Info (int16u        Parameter, const char*   Measure=NULL) {}
    inline void Param_Info (int16s        Parameter, const char*   Measure=NULL) {}
    inline void Param_Info (int8u         Parameter, const char*   Measure=NULL) {}
    inline void Param_Info (int8s         Parameter, const char*   Measure=NULL) {}
    inline void Param_Info (float32       Parameter, int8u AfterComma=3, const char*   Measure=NULL) {}
    inline void Param_Info (float64       Parameter, int8u AfterComma=3, const char*   Measure=NULL) {}
    inline void Param_Info (float80       Parameter, int8u AfterComma=3, const char*   Measure=NULL) {}
    #ifdef NEED_SIZET
    inline void Param_Info (size_t        Parameter, const char*   Measure=NULL) {}
    #endif //NEED_SIZET

    //***************************************************************************
    // Information
    //***************************************************************************

    inline void Info (const Ztring& Value) {}

    //***************************************************************************
    // Big Endian
    //***************************************************************************

    void Get_B1   (int8u   &Info);
    void Get_B2   (int16u  &Info);
    void Get_B3   (int32u  &Info);
    void Get_B4   (int32u  &Info);
    void Get_B5   (int64u  &Info);
    void Get_B6   (int64u  &Info);
    void Get_B7   (int64u  &Info);
    void Get_B8   (int64u  &Info);
    void Get_B16  (int128u &Info);
    void Get_BF4  (float32 &Info);
    void Get_BF8  (float64 &Info);
    void Get_BF10 (float80 &Info);
    inline void Get_B1   (int8u   &Info, const char*) {Get_B1(Info);}
    inline void Get_B2   (int16u  &Info, const char*) {Get_B2(Info);}
    inline void Get_B3   (int32u  &Info, const char*) {Get_B3(Info);}
    inline void Get_B4   (int32u  &Info, const char*) {Get_B4(Info);}
    inline void Get_B5   (int64u  &Info, const char*) {Get_B5(Info);}
    inline void Get_B6   (int64u  &Info, const char*) {Get_B6(Info);}
    inline void Get_B7   (int64u  &Info, const char*) {Get_B7(Info);}
    inline void Get_B8   (int64u  &Info, const char*) {Get_B8(Info);}
    inline void Get_B16  (int128u &Info, const char*) {Get_B16(Info);}
    inline void Get_BF4  (float32 &Info, const char*) {Get_BF4(Info);}
    inline void Get_BF8  (float64 &Info, const char*) {Get_BF8(Info);}
    inline void Get_BF10 (float80 &Info, const char*) {Get_BF10(Info);}
    void Peek_B1  (int8u   &Info);
    void Peek_B2  (int16u  &Info);
    void Peek_B3  (int32u  &Info);
    void Peek_B4  (int32u  &Info);
    void Peek_B5  (int64u  &Info);
    void Peek_B6  (int64u  &Info);
    void Peek_B7  (int64u  &Info);
    void Peek_B8  (int64u  &Info);
    void Peek_B16 (int128u &Info);
    void Peek_BF4 (float32 &Info);
    void Peek_BF8 (float64 &Info);
    void Peek_BF10(float64 &Info);
    void Skip_B1  ();
    void Skip_B2  ();
    void Skip_B3  ();
    void Skip_B4  ();
    void Skip_B5  ();
    void Skip_B6  ();
    void Skip_B7  ();
    void Skip_B8  ();
    void Skip_BF4 ();
    void Skip_BF8 ();
    void Skip_B16 ();
    inline void Skip_B1  (               const char*) {Element_Offset+=1;}
    inline void Skip_B2  (               const char*) {Element_Offset+=2;}
    inline void Skip_B3  (               const char*) {Element_Offset+=3;}
    inline void Skip_B4  (               const char*) {Element_Offset+=4;}
    inline void Skip_B5  (               const char*) {Element_Offset+=5;}
    inline void Skip_B6  (               const char*) {Element_Offset+=6;}
    inline void Skip_B7  (               const char*) {Element_Offset+=7;}
    inline void Skip_B8  (               const char*) {Element_Offset+=8;}
    inline void Skip_BF4 (               const char*) {Element_Offset+=4;}
    inline void Skip_BF8 (               const char*) {Element_Offset+=8;}
    inline void Skip_B16 (               const char*) {Element_Offset+=16;}
    #define Info_B1(_INFO, _NAME)   int8u   _INFO; Get_B1 (_INFO, _NAME)
    #define Info_B2(_INFO, _NAME)   int16u  _INFO; Get_B2 (_INFO, _NAME)
    #define Info_B3(_INFO, _NAME)   int32u  _INFO; Get_B3 (_INFO, _NAME)
    #define Info_B4(_INFO, _NAME)   int32u  _INFO; Get_B4 (_INFO, _NAME)
    #define Info_B5(_INFO, _NAME)   int64u  _INFO; Get_B5 (_INFO, _NAME)
    #define Info_B6(_INFO, _NAME)   int64u  _INFO; Get_B6 (_INFO, _NAME)
    #define Info_B7(_INFO, _NAME)   int64u  _INFO; Get_B7 (_INFO, _NAME)
    #define Info_B8(_INFO, _NAME)   int64u  _INFO; Get_B8 (_INFO, _NAME)
    #define Info_B16(_INFO, _NAME)  int128u _INFO; Get_B16(_INFO, _NAME)
    #define Info_BF4(_INFO, _NAME)  float32 _INFO; Get_BF4(_INFO, _NAME)
    #define Info_BF8(_INFO, _NAME)  float64 _INFO; Get_BF8(_INFO, _NAME)
    #define Info_BF10(_INFO, _NAME) float80 _INFO; Get_BF8(_INFO, _NAME)

    //***************************************************************************
    // Little Endian
    //***************************************************************************

    void Get_L1  (int8u   &Info);
    void Get_L2  (int16u  &Info);
    void Get_L3  (int32u  &Info);
    void Get_L4  (int32u  &Info);
    void Get_L5  (int64u  &Info);
    void Get_L6  (int64u  &Info);
    void Get_L7  (int64u  &Info);
    void Get_L8  (int64u  &Info);
    void Get_L16 (int128u &Info);
    void Get_LF4 (float32 &Info);
    void Get_LF8 (float64 &Info);
    inline void Get_L1  (int8u   &Info, const char*) {Get_B1(Info);}
    inline void Get_L2  (int16u  &Info, const char*) {Get_B2(Info);}
    inline void Get_L3  (int32u  &Info, const char*) {Get_B3(Info);}
    inline void Get_L4  (int32u  &Info, const char*) {Get_B4(Info);}
    inline void Get_L5  (int64u  &Info, const char*) {Get_B5(Info);}
    inline void Get_L6  (int64u  &Info, const char*) {Get_B6(Info);}
    inline void Get_L7  (int64u  &Info, const char*) {Get_B7(Info);}
    inline void Get_L8  (int64u  &Info, const char*) {Get_B8(Info);}
    inline void Get_L16 (int128u &Info, const char*) {Get_L16(Info);}
    inline void Get_LF4 (float32 &Info, const char*) {Get_LF4(Info);}
    inline void Get_LF8 (float64 &Info, const char*) {Get_LF8(Info);}
    void Peek_L1 (int8u   &Info);
    void Peek_L2 (int16u  &Info);
    void Peek_L3 (int32u  &Info);
    void Peek_L4 (int32u  &Info);
    void Peek_L5 (int64u  &Info);
    void Peek_L6 (int64u  &Info);
    void Peek_L7 (int64u  &Info);
    void Peek_L8 (int64u  &Info);
    void Peek_L16(int128u &Info);
    void Peek_LF4(float32 &Info);
    void Peek_LF8(float64 &Info);
    void Skip_L1  ();
    void Skip_L2  ();
    void Skip_L3  ();
    void Skip_L4  ();
    void Skip_L5  ();
    void Skip_L6  ();
    void Skip_L7  ();
    void Skip_L8  ();
    void Skip_LF4 ();
    void Skip_LF8 ();
    void Skip_L16 ();
    inline void Skip_L1  (               const char*) {Element_Offset+=1;}
    inline void Skip_L2  (               const char*) {Element_Offset+=2;}
    inline void Skip_L3  (               const char*) {Element_Offset+=3;}
    inline void Skip_L4  (               const char*) {Element_Offset+=4;}
    inline void Skip_L5  (               const char*) {Element_Offset+=5;}
    inline void Skip_L6  (               const char*) {Element_Offset+=6;}
    inline void Skip_L7  (               const char*) {Element_Offset+=7;}
    inline void Skip_L8  (               const char*) {Element_Offset+=8;}
    inline void Skip_LF4 (               const char*) {Element_Offset+=4;}
    inline void Skip_LF8 (               const char*) {Element_Offset+=8;}
    inline void Skip_L16 (               const char*) {Element_Offset+=16;}
    #define Info_L1(_INFO, _NAME)  int8u   _INFO; Get_L1 (_INFO, _NAME)
    #define Info_L2(_INFO, _NAME)  int16u  _INFO; Get_L2 (_INFO, _NAME)
    #define Info_L3(_INFO, _NAME)  int32u  _INFO; Get_L3 (_INFO, _NAME)
    #define Info_L4(_INFO, _NAME)  int32u  _INFO; Get_L4 (_INFO, _NAME)
    #define Info_L5(_INFO, _NAME)  int64u  _INFO; Get_L5 (_INFO, _NAME)
    #define Info_L6(_INFO, _NAME)  int64u  _INFO; Get_L6 (_INFO, _NAME)
    #define Info_L7(_INFO, _NAME)  int64u  _INFO; Get_L7 (_INFO, _NAME)
    #define Info_L8(_INFO, _NAME)  int64u  _INFO; Get_L8 (_INFO, _NAME)
    #define Info_L16(_INFO, _NAME) int128u _INFO; Get_L16(_INFO, _NAME)
    #define Info_LF4(_INFO, _NAME) float32 _INFO; Get_LF4(_INFO, _NAME)
    #define Info_LF8(_INFO, _NAME) float64 _INFO; Get_LF8(_INFO, _NAME)

    //***************************************************************************
    // UUID
    //***************************************************************************

    void Get_UUID (int128u &Info);
    inline void Get_UUID (int128u &Info, const char*) {Get_UUID(Info);}
    void Peek_UUID(int128u &Info);
    inline void Skip_UUID(               const char*) {Element_Offset+=16;}
    #define Info_UUID(_INFO, _NAME) int128u _INFO; Get_UUID(_INFO, _NAME)

    //***************************************************************************
    // EBML
    //***************************************************************************

    void Get_EB (int64u &Info, const char* Name);
    void Get_ES (int64s &Info, const char* Name);
    void Skip_EB(              const char* Name);
    void Skip_ES(              const char* Name);
    #define Info_EB(_INFO, _NAME) int64u _INFO; Get_EB(_INFO, _NAME)
    #define Info_ES(_INFO, _NAME) int64s _INFO; Get_ES(_INFO, _NAME)

    //***************************************************************************
    // Variable Length Value
    //***************************************************************************

    void Get_VL (int64u &Info, const char* Name);
    void Get_SL (int64s &Info, const char* Name);
    void Skip_VL(              const char* Name);
    void Skip_SL(              const char* Name);
    #define Info_VL(_INFO, _NAME) int64u _INFO; Get_VL(_INFO, _NAME)
    #define Info_SL(_INFO, _NAME) int64s _INFO; Get_SL(_INFO, _NAME)

    //***************************************************************************
    // Exp-Golomb
    //***************************************************************************

    void Get_UE (int32u &Info, const char* Name);
    void Get_SE (int32s &Info, const char* Name);
    void Skip_UE(              const char* Name);
    void Skip_SE(              const char* Name);
    #define Info_UE(_INFO, _NAME) int32u _INFO; Get_UE(_INFO, _NAME)
    #define Info_SE(_INFO, _NAME) int32s _INFO; Get_SE(_INFO, _NAME)

    //***************************************************************************
    // Interleaved Exp-Golomb
    //***************************************************************************

    void Get_UI (int32u &Info, const char* Name);
    void Get_SI (int32s &Info, const char* Name);
    void Skip_UI(              const char* Name);
    void Skip_SI(              const char* Name);
    #define Info_UI(_INFO, _NAME) int32u _INFO; Get_UI(_INFO, _NAME)
    #define Info_SI(_INFO, _NAME) int32s _INFO; Get_SI(_INFO, _NAME)

    //***************************************************************************
    // Characters
    //***************************************************************************

    void Get_C1 (int8u  &Info, const char* Name);
    void Get_C2 (int16u &Info, const char* Name);
    void Get_C3 (int32u &Info, const char* Name);
    void Get_C4 (int32u &Info, const char* Name);
    void Get_C5 (int64u &Info, const char* Name);
    void Get_C6 (int64u &Info, const char* Name);
    void Get_C7 (int64u &Info, const char* Name);
    void Get_C8 (int64u &Info, const char* Name);
    void Skip_C1(              const char* Name);
    void Skip_C2(              const char* Name);
    void Skip_C3(              const char* Name);
    void Skip_C4(              const char* Name);
    void Skip_C5(              const char* Name);
    void Skip_C6(              const char* Name);
    void Skip_C7(              const char* Name);
    void Skip_C8(              const char* Name);
    #define Info_C1(_INFO, _NAME) Ztring _INFO; Get_C1(_INFO, _NAME)
    #define Info_C2(_INFO, _NAME) Ztring _INFO; Get_C2(_INFO, _NAME)
    #define Info_C3(_INFO, _NAME) Ztring _INFO; Get_C3(_INFO, _NAME)
    #define Info_C4(_INFO, _NAME) Ztring _INFO; Get_C4(_INFO, _NAME)
    #define Info_C5(_INFO, _NAME) Ztring _INFO; Get_C5(_INFO, _NAME)
    #define Info_C6(_INFO, _NAME) Ztring _INFO; Get_C6(_INFO, _NAME)
    #define Info_C7(_INFO, _NAME) Ztring _INFO; Get_C7(_INFO, _NAME)
    #define Info_C8(_INFO, _NAME) Ztring _INFO; Get_C8(_INFO, _NAME)

    //***************************************************************************
    // Text
    //***************************************************************************

    void Get_Local  (int64u Bytes, Ztring      &Info, const char* Name);
    void Get_String (int64u Bytes, std::string &Info, const char* Name);
    void Get_UTF8   (int64u Bytes, Ztring      &Info, const char* Name);
    void Get_UTF16  (int64u Bytes, Ztring      &Info, const char* Name);
    void Get_UTF16B (int64u Bytes, Ztring      &Info, const char* Name);
    void Get_UTF16L (int64u Bytes, Ztring      &Info, const char* Name);
    void Peek_Local (int64u Bytes, Ztring      &Info);
    void Peek_String(int64u Bytes, std::string &Info);
    void Skip_Local (int64u Bytes,                    const char* Name);
    void Skip_String(int64u Bytes,                    const char* Name);
    void Skip_UTF8  (int64u Bytes,                    const char* Name);
    void Skip_UTF16B(int64u Bytes,                    const char* Name);
    void Skip_UTF16L(int64u Bytes,                    const char* Name);
    #define Info_Local(_BYTES, _INFO, _NAME)  Ztring _INFO; Get_Local (_BYTES, _INFO, _NAME)
    #define Info_UTF8(_BYTES, _INFO, _NAME)   Ztring _INFO; Get_UTF8  (_BYTES, _INFO, _NAME)
    #define Info_UTF16B(_BYTES, _INFO, _NAME) Ztring _INFO; Get_UTF16B(_BYTES, _INFO, _NAME)
    #define Info_UTF16L(_BYTES, _INFO, _NAME) Ztring _INFO; Get_UTF16L(_BYTES, _INFO, _NAME)

    //***************************************************************************
    // PAscal strings
    //***************************************************************************

    void Get_PA (std::string &Info, const char* Name);
    void Peek_PA(std::string &Info);
    void Skip_PA(                   const char* Name);
    #define Info_PA(_INFO, _NAME) Ztring _INFO; Get_PA (_INFO, _NAME)

    //***************************************************************************
    // Unknown
    //***************************************************************************

    inline void Skip_XX(int64u Bytes, const char*) {Element_Offset+=Bytes;}

    //***************************************************************************
    // Flags
    //***************************************************************************

    void Get_Flags (int64u Flags, size_t Order, bool  &Info, const char* Name);
    void Get_Flags (int64u ValueToPut,          int8u &Info, const char* Name);
    void Skip_Flags(int64u Flags, size_t Order,              const char* Name);
    void Skip_Flags(int64u ValueToPut,                       const char* Name);

    //***************************************************************************
    // BitStream
    //***************************************************************************

    void Get_BS (size_t Bits, int32u  &Info);
    void Get_SB (             bool    &Info);
    void Get_S1 (size_t Bits, int8u   &Info);
    void Get_S2 (size_t Bits, int16u  &Info);
    void Get_S3 (size_t Bits, int32u  &Info);
    void Get_S4 (size_t Bits, int32u  &Info);
    void Get_S5 (size_t Bits, int64u  &Info);
    void Get_S6 (size_t Bits, int64u  &Info);
    void Get_S7 (size_t Bits, int64u  &Info);
    void Get_S8 (size_t Bits, int64u  &Info);
    inline void Get_BS (size_t Bits, int32u  &Info, const char*) {Get_BS(Bits, Info);}
    inline void Get_SB (             bool    &Info, const char*) {Get_SB(      Info);}
    inline void Get_S1 (size_t Bits, int8u   &Info, const char*) {Get_S1(Bits, Info);}
    inline void Get_S2 (size_t Bits, int16u  &Info, const char*) {Get_S2(Bits, Info);}
    inline void Get_S3 (size_t Bits, int32u  &Info, const char*) {Get_S3(Bits, Info);}
    inline void Get_S4 (size_t Bits, int32u  &Info, const char*) {Get_S4(Bits, Info);}
    inline void Get_S5 (size_t Bits, int64u  &Info, const char*) {Get_S5(Bits, Info);}
    inline void Get_S6 (size_t Bits, int64u  &Info, const char*) {Get_S6(Bits, Info);}
    inline void Get_S7 (size_t Bits, int64u  &Info, const char*) {Get_S7(Bits, Info);}
    inline void Get_S8 (size_t Bits, int64u  &Info, const char*) {Get_S8(Bits, Info);}
    void Peek_BS(size_t Bits, int32u  &Info);
    void Peek_SB(              bool    &Info);
    void Peek_S1(size_t Bits, int8u   &Info);
    void Peek_S2(size_t Bits, int16u  &Info);
    void Peek_S3(size_t Bits, int32u  &Info);
    void Peek_S4(size_t Bits, int32u  &Info);
    void Peek_S5(size_t Bits, int64u  &Info);
    void Peek_S6(size_t Bits, int64u  &Info);
    void Peek_S7(size_t Bits, int64u  &Info);
    void Peek_S8(size_t Bits, int64u  &Info);
    void Skip_BS(size_t Bits);
    void Skip_SB(           );
    void Skip_S1(size_t Bits);
    void Skip_S2(size_t Bits);
    void Skip_S3(size_t Bits);
    void Skip_S4(size_t Bits);
    void Skip_S5(size_t Bits);
    void Skip_S6(size_t Bits);
    void Skip_S7(size_t Bits);
    void Skip_S8(size_t Bits);
    inline void Skip_BS(size_t Bits,                const char*) {BS->Skip8(Bits);}
    inline void Skip_SB(                            const char*) {BS->Skip1(1);}
    inline void Skip_S1(size_t Bits,                const char*) {BS->Skip1(Bits);}
    inline void Skip_S2(size_t Bits,                const char*) {BS->Skip2(Bits);}
    inline void Skip_S3(size_t Bits,                const char*) {BS->Skip4(Bits);}
    inline void Skip_S4(size_t Bits,                const char*) {BS->Skip4(Bits);}
    inline void Skip_S5(size_t Bits,                const char*) {BS->Skip8(Bits);}
    inline void Skip_S6(size_t Bits,                const char*) {BS->Skip8(Bits);}
    inline void Skip_S7(size_t Bits,                const char*) {BS->Skip8(Bits);}
    inline void Skip_S8(size_t Bits,                const char*) {BS->Skip8(Bits);}
    void Mark_0 ();
    void Mark_1 ();
    #define Info_BS(_BITS, _INFO, _NAME) int32u  _INFO; Get_BS(_BITS, _INFO, _NAME)
    #define Info_SB(_INFO, _NAME)        bool    _INFO; Get_SB(       _INFO, _NAME)
    #define Info_S1(_BITS, _INFO, _NAME) int8u   _INFO; Get_S1(_BITS, _INFO, _NAME)
    #define Info_S2(_BITS, _INFO, _NAME) int16u  _INFO; Get_S2(_BITS, _INFO, _NAME)
    #define Info_S3(_BITS, _INFO, _NAME) int32u  _INFO; Get_S4(_BITS, _INFO, _NAME)
    #define Info_S4(_BITS, _INFO, _NAME) int32u  _INFO; Get_S4(_BITS, _INFO, _NAME)
    #define Info_S5(_BITS, _INFO, _NAME) int64u  _INFO; Get_S5(_BITS, _INFO, _NAME)
    #define Info_S6(_BITS, _INFO, _NAME) int64u  _INFO; Get_S6(_BITS, _INFO, _NAME)
    #define Info_S7(_BITS, _INFO, _NAME) int64u  _INFO; Get_S7(_BITS, _INFO, _NAME)
    #define Info_S8(_BITS, _INFO, _NAME) int64u  _INFO; Get_S8(_BITS, _INFO, _NAME)

    #define TEST_SB_GET(_CODE, _NAME) \
        { \
            Peek_SB(_CODE); \
            if (!_CODE) \
                Skip_SB(                                        _NAME); \
            else \
            { \
                Element_Begin(_NAME); \
                Skip_SB(                                        _NAME); \

    #define TEST_SB_SKIP(_NAME) \
        { \
            bool Temp; \
            Peek_SB(Temp); \
            if (!Temp) \
                Skip_SB(                                        _NAME); \
            else \
            { \
                Element_Begin(_NAME); \
                Skip_SB(                                        _NAME); \

    #define TESTELSE_SB_GET(_CODE, _NAME) \
        { \
            Peek_SB(_CODE); \
            if (_CODE) \
            { \
                Element_Begin(_NAME); \
                Skip_SB(                                        _NAME); \

    #define TESTELSE_SB_SKIP(_NAME) \
        { \
            bool Temp; \
            Peek_SB(Temp); \
            if (Temp) \
            { \
                Element_Begin(_NAME); \
                Skip_SB(                                        _NAME); \

    #define TESTELSE_SB_ELSE(_NAME) \
                Element_End(); \
            } \
            else \
            { \
                Skip_SB(                                        _NAME); \

    #define TESTELSE_SB_END() \
            } \
        } \

    #define TEST_SB_END() \
                Element_End(); \
            } \
        } \

    //***************************************************************************
    // Next code planning
    //***************************************************************************

    void NextCode_Add(int64u Code);
    void NextCode_Clear();
    void NextCode_Test();

    //***************************************************************************
    // Element trusting
    //***************************************************************************

    void Trusted_IsNot (const char* Reason);

    //***************************************************************************
    // Stream filling
    //***************************************************************************

    //Before filling the stream, the stream must be prepared
    size_t Stream_Prepare   (stream_t KindOfStream);
    void   General_Fill     (); //Special : pre-fill General with some important information

    //Fill with datas
    void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, const Ztring  &Value, bool Replace=false);
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, const std::string &Value, bool Utf8=true, bool Replace=false) {if (Utf8) Fill(StreamKind, StreamPos, Parameter, Ztring().From_UTF8(Value.c_str(), Value.size())); else Fill(StreamKind, StreamPos, Parameter, Ztring().From_Local(Value.c_str(), Value.size()), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, const char*    Value, size_t Value_Size=Unlimited, bool Utf8=true, bool Replace=false) {if (Utf8) Fill(StreamKind, StreamPos, Parameter, Ztring().From_UTF8(Value, Value_Size), Replace); else Fill(StreamKind, StreamPos, Parameter, Ztring().From_Local(Value, Value_Size), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, const wchar_t* Value, size_t Value_Size=Unlimited, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring().From_Unicode(Value, Value_Size), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int8u          Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int8s          Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int16u         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int16s         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int32u         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int32s         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int64u         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int64s         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, float32        Value, int8u AfterComma=3, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, AfterComma), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, float64        Value, int8u AfterComma=3, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, AfterComma), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, float80        Value, int8u AfterComma=3, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, AfterComma), Replace);}
    #ifdef NEED_SIZET
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, size_t         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    #endif //NEED_SIZET
    inline void Fill (const char* Parameter, const Ztring  &Value, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Replace);} //With the last set
    inline void Fill (const char* Parameter, const std::string &Value, bool Utf8=true, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Utf8, Replace);} //With the last set
    inline void Fill (const char* Parameter, const char*    Value, size_t ValueSize=Unlimited, bool Utf8=true, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, ValueSize, Utf8, Replace);} //With the last set
    inline void Fill (const char* Parameter, const int8u*   Value, size_t ValueSize=Unlimited, bool Utf8=true, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, (const char*)Value, ValueSize, Utf8, Replace);} //With the last set
    inline void Fill (const char* Parameter, const wchar_t* Value, size_t ValueSize=Unlimited, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, ValueSize, Replace);} //With the last set
    inline void Fill (const char* Parameter, int8u          Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, int8s          Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, int16u         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, int16s         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, int32u         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, int32s         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, int64u         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, int64s         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, float32        Value, int8u AfterComma=3, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, AfterComma, Replace);} //With the last set
    inline void Fill (const char* Parameter, float64        Value, int8u AfterComma=3, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, AfterComma, Replace);} //With the last set
    inline void Fill (const char* Parameter, float80        Value, int8u AfterComma=3, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, AfterComma, Replace);} //With the last set
    #ifdef NEED_SIZET
    inline void Fill (const char* Parameter, size_t         Value, int8u Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    #endif //NEED_SIZET
    void Fill (stream_t StreamKind, size_t StreamPos, const int32u Parameter, const Ztring  &Value, bool Replace=false);
    inline void Fill (int32u Parameter, const Ztring  &Value, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Replace);} //With the last set
    void Fill_HowTo (stream_t StreamKind, size_t StreamPos, const char* Parameter, const char* Value);
    inline void Fill_HowTo (const char* Parameter, const char* Value) {Fill_HowTo(StreamKind_Last, StreamPos_Last, Parameter, Value);} //With the last set
    ZtringListList Fill_Temp;
    void Fill_Flush ();

    //***************************************************************************
    // Filling
    //***************************************************************************

    //Actions
    void Finnished();
    int64u Element_Code_Get (size_t Level);
    int64u Element_TotalSize_Get (size_t LevelLess=0);
    bool Element_IsComplete_Get ();
    void Element_ThisIsAList ();
    void Element_WaitForMoreData ();
    void Element_DoNotTrust (const char* Reason);
    inline void Element_DoNotShow () {}
    inline void Element_Show () {}
    inline bool Element_Show_Get () {return false;}
    inline void Element_Show_Add (const Ztring &ToShow) {}

    //Status
    bool Element_IsOK ();
    bool Element_IsNotFinnished ();
    bool Element_IsWaitingForMoreData ();

    //Begin
    #define FILLING_BEGIN() if (Element_IsOK()) {

    //End
    #define FILLING_END() }

    //***************************************************************************
    // Merging
    //***************************************************************************

    //Utils
public :
    size_t Merge(const File__Base &ToAdd); //Merge 2 File_Base
    size_t Merge(File__Base &ToAdd, stream_t StreamKind, size_t StreamPos_From, size_t StreamPos_To); //Merge 2 streams

    //***************************************************************************
    // Finalize
    //***************************************************************************

    //End
    void Finalize();

protected :

    void Finalize__All      (stream_t StreamKind);
    void Finalize__All      (stream_t StreamKind, size_t Pos);
    void Finalize_General   (size_t Pos);
    void Finalize_Video     (size_t Pos);
    void Finalize_Audio     (size_t Pos);
    void Finalize_Audio_BitRate (size_t Pos, ZenLib::Char* Parameter);
    void Finalize_Text      (size_t Pos);
    void Finalize_Chapters  (size_t Pos);
    void Finalize_Image     (size_t Pos);
    void Finalize_Menu      (size_t Pos);
    void Finalize_Tags      ();
    void Finalize_Final     ();
    void Finalize_Final_All (stream_t StreamKind);
    void Finalize_Final_All (stream_t StreamKind, size_t Pos, Ztring &Codec_List, Ztring &Language_List);

    //Utils - Finalize
    void PlayTime_PlayTime123   (const Ztring &Value, stream_t StreamKind, size_t StreamPos);
    void FileSize_FileSize123   (const Ztring &Value, stream_t StreamKind, size_t StreamPos);
    void Kilo_Kilo123           (const Ztring &Value, stream_t StreamKind, size_t StreamPos);
    void Value_Value123         (const Ztring &Value, stream_t StreamKind, size_t StreamPos);

    //***************************************************************************
    //
    //***************************************************************************

protected :
    //Save for speed improvement
    float Config_Details;

    //Configuration
    bool DataMustAlwaysBeComplete;  //Data must always be complete, else wait for more data
    bool MustUseAlternativeParser;  //Must use the second parser (example: for Data part)

    //Synchro
    bool MustParseTheHeaderFile;    //There is an header part, must parse it
    bool Synched;                   //Data is synched

    //Elements
    size_t Element_Level;           //Current level
    bool   Element_WantNextLevel;   //Want to go to the next leavel instead of the same level

    //Element
    int64u Element_Code;            //Code filled in the file, copy of Element[Element_Level].Code
    size_t Element_Offset;          //Position in the Element (without header)
    int64u Element_Size;            //Size of the Element (without header)

private :
    //***************************************************************************
    // Buffer
    //***************************************************************************

    void Buffer_Clear(); //Clear the buffer
    void Open_Buffer_Continue_Loop();
protected :
    //Buffer
    const int8u* Buffer;
    int8u* Buffer_Temp;
    size_t Buffer_Size;
    size_t Buffer_Size_Max;
    size_t Buffer_Offset; //Temporary usage in this parser
    size_t Buffer_Offset_Temp; //Temporary usage in this parser
    size_t Buffer_MinimumSize;
    size_t Buffer_MaximumSize;
    bool   Buffer_Init_Done;
    friend class File__Tags_Helper;
private :

    //***************************************************************************
    // Elements
    //***************************************************************************

    //Data
    size_t Data_Level;              //Current level for Data ("Top level")

    //Element
    BitStream* BS;                  //For conversion from bytes to bitstream
public : //TO CHANGE
    int64u Header_Size;             //Size of the header of the current element
private :

    //Elements
    size_t Element_Level_Base;      //From other parsers

    struct element_details
    {
        struct to_show
        {
            int64u Pos;             //Position of the element in the file
            int64u Size;            //Size of the element (including header and sub-elements)
            int64u Header_Size;     //Size of the header of the element
            Ztring Name;            //Name planned for this element
            Ztring Info;            //More info about the element
            Ztring Details;         //The main text
            bool   NoShow;          //Don't show this element
        };

        int64u  Code;               //Code filled in the file
        int64u  Next;               //
        bool    WaitForMoreData;    //This element is not complete, we need more data
        bool    UnTrusted;          //This element has a problem
        bool    IsComplete;         //This element is fully buffered, no need of more
        bool    InLoop;             //This element is in a parsing loop
        //to_show ToShow;
    };
    std::vector<element_details> Element;

    //NextCode
    std::map<int64u, bool> NextCode;

    //BookMarks
    size_t              BookMark_Element_Level;
    int64u              BookMark_GoTo;
    std::vector<int64u> BookMark_Code;
    std::vector<int64u> BookMark_Next;

public :
    void BookMark_Set(size_t Element_Level_ToGet=(size_t)-1);
    void BookMark_Get(size_t Element_Level_ToSet=(size_t)-1);
    virtual bool BookMark_Needed()                                              {return true;};
};

