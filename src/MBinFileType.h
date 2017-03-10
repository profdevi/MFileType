/*    
    MFileType.exe : Shows the type of a file
    Copyright (C) 2017  Comine.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

//v1.8 copyright Comine.com 20170310F0657
#ifndef MBinFileType_h
#define MBinFileType_h

////////////////////////////////////////////////////
enum MBinFileTypeEnum
	{
	MBFTE_AVI,		// "AVI format",".avi","52494646",NULL,
	MBFTE_ACCDB,	// "MS Access 2007 file",".accdb","000100005374616e6461726420414345204442",NULL,  http://www.garykessler.net/library/file_sigs.html
	MBFTE_BMP,		// "Bitmap format",".bmp","424d",NULL,
	MBFTE_BZ,		// "Bzip",".bz","425a",NULL,
	MBFTE_CHM,		// "MS HTML Compiled Help",".chm","4954534603000000",NULL,
	MBFTE_CUR,		// "Windows Cursor File",".ico","00000200",NULL,
	MBFTE_DOC,		// "MS Office Doc File",".doc","D0CF11E0A1B11AE1",NULL,
	MBFTE_ELF,		// "Unix elf","","7f454c46",NULL,
	MBFTE_EXE,		// "MS-DOS, OS/2 or MS Windows",".exe","4d5a",NULL,
	MBFTE_FIG,		// "Xfig format",".fig","23464947",NULL,
	MBFTE_FITS,		// "FITS format",".fits","53494d504c45",NULL,
	MBFTE_GIF,		// "GIF format",".gif","47494638",NULL,
	MBFTE_GKS,		// "Graphics Kernel System",".gks","474b534d",NULL,
	MBFTE_GZ,		// "gzip format",".gz","1f8b",NULL,
	MBFTE_HLP,		// "Windows Help",".hlp","3F5F0300",NULL,
	MBFTE_ICO,		// "Windows Icon File",".ico","00000100",NULL,
	MBFTE_ITC,		// "ITC (CMU WM) format",".itc","f10040bb",NULL,
	MBFTE_JPG1,		// "JPEG File",".jpg","ffd8ffe0",NULL,
	MBFTE_JPG2,		// "JPEG for Digital Camera",".jpg","ffd8ffe1",NULL,
	MBFTE_MDB,		// "MS Access Database",".mdb","000100005374616E64617264204A657420444200",NULL, 
	MBFTE_MP3,		// "MPEG Audio Stream Layer III",".mp3","ff",NULL,
	MBFTE_NIF,		// "NIFF (Navy TIFF)",".nif","49494e31",NULL,
	MBFTE_OBJ,		// "MS Common Object File Format(COFF)",".obj","4c01",NULL,
	MBFTE_OGG,		// "Ogg Vorbis Codec Compressed",".ogg","4F67675300020000000000000000",NULL,
	MBFTE_PDF,		// "Adobe Portable Document Format",".pdf","25504446",NULL,
	MBFTE_PGP1,		// "pgp public ring","","9900",NULL,
	MBFTE_PGP2,		// "pgp security ring","","9501",NULL,
	MBFTE_PGP3,		// "pgp security ring","","9500",NULL,
	MBFTE_PGP4,		// "pgp encrypted data","","a600",NULL,
	MBFTE_PM,		// "PM format",".pm","56494557",NULL,
	MBFTE_PNG,		// "PNG format",".png","89504e47",NULL,
	MBFTE_PPT,		// "MS Office Power Point File",".ppt","D0CF11E0A1B11AE1",NULL,
	MBFTE_PS,		// "Postscript format",".ps","2521",NULL,
	MBFTE_RAS,		// "Sun Rasterfile",".ras","59a66a95",NULL,
	MBFTE_RGB,		// "IRIS rgb format",".rgb","01da",NULL,
	MBFTE_RTF,		// "Rich Text format",".rtf","7b5c72746631",NULL,
	MBFTE_TAR,		// "TAR (POSIX)",".tar","7573746172",NULL,
	MBFTE_TIFB,		// "TIFF format (Motorola - big endian)",".tif","4d4d002a"},NULL,
	MBFTE_TIFL,		// "TIFF format (Intel - little endian)",".tif","49492a00"},NULL,
	MBFTE_WAV,		// "WAV Audio",".wav","52494646"},NULL,
	MBFTE_XBM,		// "X11 Bitmap format",".xbm",""},NULL,
	MBFTE_XCF,		// "XCF Gimp file structure",".xcf","67696d70207863662076"},NULL,
	MBFTE_XLS,		// "MS Office Excel File",".xls","D0CF11E0A1B11AE1"},NULL,
	MBFTE_XPM,		// "XPM format",".xpm","2f2a2058504d202a2f"},NULL,
	MBFTE_Z,		// "Compress",".Z","1f9d"},NULL,
	MBFTE_ZIP,		// "pkzip format",".zip","504b0304"},NULL,

	MBFTE_UNKNOWN	// First Entry
	};

//******************************************************
//**  MBinFileType class
//******************************************************
class MBinFileType
	{
	////////////////////////////////////////////////
	int mIndex;
	const char *mExtraInfo;
	
	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MBinFileType(void);
	MBinFileType(const char *filename);
	MBinFileType(const char *memory,int maxsize);
	~MBinFileType(void);
	bool Create(const char *filename);
	bool Create(const char *memory,int maxsize);
	bool Destroy(void);
	MBinFileTypeEnum GetFileType(void);
	const char *GetFileInfo(void);
	const char *GetFileExtension(void);
	const char *GetExtraInfo(void);
	};


//******************************************************
//**  MBinFileTypeInfo class
//******************************************************
class MBinFileTypeInfo
	{
	////////////////////////////////////////////////
	void ClearObject(void);

	////////////////////////////////////////////////	
	public:
	MBinFileTypeInfo(bool create=false);
	~MBinFileTypeInfo(void);
	bool Create(void);
	bool Destroy(void);
	int GetCount(void);
	const char *GetFileInfo(int index);
	const char *GetFileExtension(int index);
	const char *GetSignature(int index);
	};


#endif // MBinFileType_h

