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
#include "MStdLib.h"
#include "MFile.h"
#include "MFileOps.h"
#include "MBuffer.h"
#include "MBinFileType.h"


//******************************************************
//**  Module Elements
//******************************************************
static const int GMaxHeaderSize=1024;

////////////////////////////////////////////////
struct GFileTypes
	{
	MBinFileTypeEnum FileType;
	const char *FileInfo;
	const char *FileExtension;
	const char *HeaderPrefix;
	bool (*ExtraCheck)(const unsigned char data[GMaxHeaderSize],const char *&extrainfo);
	};


//////////////////////////////////////////////////////
static bool GIsMSDocFile(const unsigned char data[GMaxHeaderSize],const char *&extrainfo);
static bool GIsMSExec(const unsigned char data[GMaxHeaderSize],const char *&extrainfo);
static bool GIsMSExcellFile(const unsigned char data[GMaxHeaderSize],const char *&extrainfo);
static bool GIsMSExcellFile2(const unsigned char data[GMaxHeaderSize],const char *&extrainfo);
static bool GIsMSPowerPointFile(const unsigned char data[GMaxHeaderSize],const char *&extrainfo);
static bool GIsMSPowerPointFile2(const unsigned char data[GMaxHeaderSize],const char *&extrainfo);

//////////////////////////////////////////////////////
// Last Updated 20080302U15
// See Ref: http://www.garykessler.net/library/file_sigs.html
static GFileTypes GArray[]=
	{
	MBFTE_UNKNOWN,0,0,0,NULL,
	MBFTE_AVI,"AVI format",".avi","52494646",NULL,
	MBFTE_ACCDB,"MS Access 2007 file",".accdb","000100005374616e6461726420414345204442",NULL,
	MBFTE_BMP,"Bitmap format",".bmp","424d",NULL,
	MBFTE_BZ,"Bzip",".bz","425a",NULL,
	MBFTE_CHM,"MS HTML Compiled Help",".chm","4954534603000000",NULL,
	MBFTE_CUR,"Windows Cursor File",".ico","00000200",NULL,
	MBFTE_DOC,"MS Office Doc File",".doc","d0cf11e0a1b11ae1",GIsMSDocFile,
	MBFTE_ELF,"Unix elf","","7f454c46",NULL,
	MBFTE_EXE,"MS-DOS, OS/2 or MS Windows",".exe","4d5a",GIsMSExec,
	MBFTE_FIG,"Xfig format",".fig","23464947",NULL,
	MBFTE_FITS,"FITS format",".fits","53494d504c45",NULL,
	MBFTE_GIF,"GIF format",".gif","47494638",NULL,
	MBFTE_GKS,"Graphics Kernel System",".gks","474b534d",NULL,
	MBFTE_GZ,"gzip format",".gz","1f8b",NULL,
	MBFTE_HLP,"Windows Help",".hlp","3F5F0300",NULL,
	MBFTE_ICO,"Windows Icon File",".ico","00000100",NULL,
	MBFTE_ITC,"ITC (CMU WM) format",".itc","f10040bb",NULL,
	MBFTE_JPG1,"JPEG",".jpg","ffd8ffe0",NULL,
	MBFTE_JPG2,"JPEG for Digital Camera",".jpg","ffd8ffe1",NULL,
	MBFTE_MDB,"MS Access Database",".mdb","000100005374616E64617264204A657420444200",NULL,
	MBFTE_MP3,"MPEG Audio Stream Layer III",".mp3","ff",NULL,
	MBFTE_NIF,"NIFF (Navy TIFF)",".nif","49494e31",NULL,
	MBFTE_OBJ,"MS Common Object File Format(COFF)",".obj","4c01",NULL,
	MBFTE_OGG,"Ogg Vorbis Codec Compressed",".ogg","4F67675300020000000000000000",NULL,
	MBFTE_PDF,"Adobe Portable Document Format",".pdf","25504446",NULL,
	MBFTE_PGP1,"pgp public ring","","9900",NULL,
	MBFTE_PGP2,"pgp security ring","","9501",NULL,
	MBFTE_PGP3,"pgp security ring","","9500",NULL,
	MBFTE_PGP4,"pgp encrypted data","","a600",NULL,
	MBFTE_PM,"PM format",".pm","56494557",NULL,
	MBFTE_PNG,"PNG format",".png","89504e47",NULL,
	MBFTE_PPT,"MS Office Power Point File",".ppt","d0cf11e0a1b11ae1",GIsMSPowerPointFile,
	MBFTE_PPT,"MS Office Power Point File",".ppt","d0cf11e0a1b11ae1",GIsMSPowerPointFile2,
	MBFTE_PS,"Postscript format",".ps","2521",NULL,
	MBFTE_RAS,"Sun Rasterfile",".ras","59a66a95",NULL,
	MBFTE_RGB,"IRIS rgb format",".rgb","01da",NULL,
	MBFTE_RTF,"Rich Text format",".rtf","7b5c72746631",NULL,
	MBFTE_TAR,"TAR (POSIX)",".tar","7573746172",NULL,
	MBFTE_TIFB,"TIFF format (Motorola - big endian)",".tif","4d4d002a",NULL,
	MBFTE_TIFL,"TIFF format (Intel - little endian)",".tif","49492a00",NULL,
	MBFTE_WAV,"WAV Audio",".wav","52494646",NULL,
	MBFTE_XBM,"X11 Bitmap format",".xbm","",NULL,
	MBFTE_XCF,"XCF Gimp file structure",".xcf","67696d70207863662076",NULL,
	MBFTE_XLS,"MS Office Excel File",".xls","d0cf11e0a1b11ae1",GIsMSExcellFile,
	MBFTE_XLS,"MS Office Excel File",".xls","d0cf11e0a1b11ae1",GIsMSExcellFile2,
	MBFTE_XPM,"XPM format",".xpm","2f2a2058504d202a2f",NULL,
	MBFTE_Z,"Compress",".Z","1f9d",NULL,
	MBFTE_ZIP,"pkzip format",".zip","504b0304",NULL,

	// Last Entry
	MBFTE_UNKNOWN,0,0,0,NULL
	};


//////////////////////////////////////////////////////
// Hold Information of the PE Format from microsoft
struct GPEMachineIDInfo
	{
	unsigned int MachineID;
	const char *Info;
	};


////////////////////////////////////////////////////////////////
static GPEMachineIDInfo GPEMachineInfoList[]=
	{
	0x0,"Any Machine Type",
	0x1d3,"Matsushita AM33",
	0x8664,"AMD x64 - 64 bit",
	0x1c0,"ARM little endian",
	0xaa64,"ARM64 little endian",
	0x1c4,"ARM Thumb-2 little endian",
	0xebc,"EFI byte code",
	0x14c,"Intel 386 -32 bit",
	0x200,"Intel Itanium processor family",
	0x9041,"Mitsubishi M32R little endian",
	0x266,"MIPS16",
	0x366,"MIPS with FPU",
	0x466,"MIPS16 with FPU",
	0x1f0,"Power PC little endian",
	0x1f1,"Power PC with floating point support",
	0x166,"MIPS little endian",
	0x5032,"RISC-V 32-bit address space",
	0x5064,"RISC-V 64-bit address space",
	0x5128,"RISC-V 128-bit address space",
	0x1a2,"Hitachi SH3",
	0x1a3,"Hitachi SH3 DSP",
	0x1a6,"Hitachi SH4",
	0x1a8,"Hitachi SH5",
	0x1c2,"Thumb",
	0x169,"MIPS little-endian WCE v2",
	0,0	
	};


////////////////////////////////////////////////////////////////
static bool GIsMSDocFile(const unsigned char data[GMaxHeaderSize],const char *&extrainfo)
	{
	// Sub header
	if(data[512]!=0xEC) { return false; }
	if(data[513]!=0xA5) { return false; }
	if(data[514]!=0xC1) { return false; }
	if(data[515]!=0x00) { return false; }

	return true;
	}


////////////////////////////////////////////////////////////////
static bool GIsMSExec(const unsigned char data[GMaxHeaderSize],const char *&extrainfo)
	{
	// PECOFF Documentation from Microsoft
	const int peoffset=data[0x3c]+256*data[0x3c+1];
	if(peoffset<0 || peoffset>1000) { return true; }

	// Check PE Header
	if(data[peoffset]!=0x50) { return false; }
	if(data[peoffset+1]!=0x45) { return false; }
	if(data[peoffset+2]!=0) { return false; }
	if(data[peoffset+3]!=0) { return false; }

	const unsigned int machineid=256*data[peoffset+5]+data[peoffset+4];

	for(int i=0;GPEMachineInfoList[i].Info!=0;++i)
		{
		const GPEMachineIDInfo *item=GPEMachineInfoList+i;
		MStdAssert(item!=0);
	
		if(item->MachineID!=machineid) { continue; }

		extrainfo=item->Info;
		return true;
		}
	
	return true;
	}


////////////////////////////////////////////////////////////////
static bool GIsMSExcellFile(const unsigned char data[GMaxHeaderSize],const char *&extrainfo)
	{
	// Sub header
	if(data[512]!=0xFD) { return false; }
	if(data[513]!=0xFF) { return false; }
	if(data[514]!=0xFF) { return false; }
	if(data[515]!=0xFF) { return false; }

	if(data[516]!=0x10 
			&& data[516]!=0x22
			&& data[516]!=0x23
			&& data[516]!=0x28 
			&& data[516]!=0x29 )
		{  return false; }

	if(data[517]!=0x02) { return false; }

	return true;
	}


////////////////////////////////////////////////////////////////
static bool GIsMSExcellFile2(const unsigned char data[GMaxHeaderSize],const char *&extrainfo)
	{
	// This is just a guess for my files
	// Sub header
	if(data[512]!=0x09) { return false; }
	if(data[513]!=0x08) { return false; }
	if(data[514]!=0x10) { return false; }
	if(data[515]!=0x00) { return false; }

	return true;
	}



/////////////////////////////////////////////////////////////////
static bool GIsMSPowerPointFile(const unsigned char data[GMaxHeaderSize],const char *&extrainfo)
	{
	if(data[512]!=0xFD) { return false;  }
	if(data[513]!=0xFF) { return false;  }
	if(data[514]!=0xFF) { return false;  }
	if(data[515]!=0xFF) { return false;  }

	if(data[516]!=0x0E && data[516]!=0x1C && data[516]!=0x43)
		{  return false; }

	if(data[517]!=0x02) { return false;  }

	return true;
	}


/////////////////////////////////////////////////////////////////
static bool GIsMSPowerPointFile2(const unsigned char data[GMaxHeaderSize],const char *&extrainfo)
	{
	if(data[512]!=0xFD) { return false;  }
	if(data[513]!=0xFF) { return false;  }
	if(data[514]!=0xFF) { return false;  }
	if(data[515]!=0xFF) { return false;  }

	return true;
	}


////////////////////////////////////////////////////////////////
static int GHexStringToBinary(const char *input,char output[],int outputsize)
	{
	int index;
	for(index=0;input[index]!=0;index+=2)
		{
		if(input[index+1]==0)
			{
			return 0;
			}

		output[index>>1]=16*MStdFromHex(input[index])
				+MStdFromHex(input[index+1]);
		}

	return index>>1;
	}


////////////////////////////////////////////////////////////////
static bool GIsFile(const char *filename)
	{
	MFileInput fin;
	if(fin.Create(filename)==false)
		{  return false; }

	fin.Destroy();
	return true;
	}


//******************************************************
//**  MBinFileType class
//******************************************************
void MBinFileType::ClearObject(void)
	{
	mExtraInfo=0;
	mIndex=0;
	}


////////////////////////////////////////////////
MBinFileType::MBinFileType(void)
	{  ClearObject();  }


///////////////////////////////////////////////
MBinFileType::MBinFileType(const char *filename)
	{
	ClearObject();
	if(Create(filename)==false)
		{
		return;
		}
	}


///////////////////////////////////////////////
MBinFileType::MBinFileType(const char *memory,int maxsize)
	{
	ClearObject();
	if(Create(memory,maxsize)==false)
		{	
		return;
		}
	}


////////////////////////////////////////////////
MBinFileType::~MBinFileType(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MBinFileType::Create(const char *filename)
	{
	Destroy();

	// Get File Size
	MFileOps fileops(true);
	if(fileops.Exists(filename)==false)
		{
		Destroy();
		return false;
		}

	if(fileops.IsDirectory(filename)==true)
		{
		Destroy();
		return false;
		}

	// Read the data from the file
	MFileInput fileinput;
	if(fileinput.Create(filename)==false)
		{
		Destroy();
		return false;
		}

	// read the first
	MBuffer buffer(GMaxHeaderSize);
	buffer.Clear(0);

	if(fileinput.ReadToBuffer(buffer.GetBuffer(),buffer.GetSize()-1)<=0)
		{
		Destroy();
		return false;
		}

	fileinput.Destroy();

	// Run Through Array
	MBuffer sigdata(1000);
	for(mIndex=1;GArray[mIndex].FileInfo!=NULL;++mIndex)
		{
		int datacnt=GHexStringToBinary(GArray[mIndex].HeaderPrefix
				,sigdata.GetBuffer(),sigdata.GetSize()-1);

		if(datacnt==0) { continue;  }

		// Check signatures
		bool foundfile=true;
		int i;
		for(i=0;i<datacnt;++i)
			{
			if(sigdata[i]!=buffer[i])
				{
				foundfile=false;
				break;
				}
			}

		if(foundfile==false)
			{ continue;  }

		// Now Check if extra check is nexessary
		if(GArray[mIndex].ExtraCheck!=NULL && 
				GArray[mIndex].ExtraCheck((unsigned char *)buffer.GetBuffer(),mExtraInfo)==false)
			{ continue; }

		return true;
		}

	mIndex=0;
	return true;
	}


////////////////////////////////////////////////
bool MBinFileType::Create(const char *memory,int maxsize)
	{
	Destroy();

	// Run Through Array
	char sigdata[100];
	for(mIndex=1;GArray[mIndex].FileInfo!=NULL;++mIndex)
		{
		int datacnt=GHexStringToBinary(GArray[mIndex].HeaderPrefix
				,sigdata,sizeof(sigdata) );

		if(datacnt==0) { continue;  }

		if(datacnt>maxsize) { continue;  }

		// Check signatures
		bool foundfile=true;
		int i;
		for(i=0;i<datacnt;++i)
			{
			if(sigdata[i]!=memory[i])
				{
				foundfile=false;
				break;
				}
			}

		if(foundfile==true)
			{
			return true;
			}
		}

	mIndex=0;
	return true;	
	}


////////////////////////////////////////////////
bool MBinFileType::Destroy(void)
	{
	ClearObject();
	return true;
	}


/////////////////////////////////////////////////
MBinFileTypeEnum MBinFileType::GetFileType(void)
	{
	return GArray[mIndex].FileType;
	}


////////////////////////////////////////////////
const char *MBinFileType::GetFileInfo(void)
	{
	return GArray[mIndex].FileInfo;
	}


//////////////////////////////////////////////////
const char *MBinFileType::GetFileExtension(void)
	{
	return GArray[mIndex].FileExtension;
	}


//////////////////////////////////////////////////
const char *MBinFileType::GetExtraInfo(void)
	{
	return mExtraInfo;
	}


//******************************************************
//**  MBinFileTypeInfo class
//******************************************************
void MBinFileTypeInfo::ClearObject(void)
	{
	}


///////////////////////////////////////////////
MBinFileTypeInfo::MBinFileTypeInfo(bool create)
	{
	ClearObject();
	if(create==true && Create()==false)
		{
		return;
		}
	}


///////////////////////////////////////////////
MBinFileTypeInfo::~MBinFileTypeInfo(void)
	{  Destroy();  }


///////////////////////////////////////////////
bool MBinFileTypeInfo::Create(void)
	{
	Destroy();

	return true;
	}


//////////////////////////////////////////////
bool MBinFileTypeInfo::Destroy(void)
	{
	ClearObject();
	return true;
	}


/////////////////////////////////////////////
int MBinFileTypeInfo::GetCount(void)
	{
	const int GCount=sizeof(GArray)/sizeof(GFileTypes)-2;
	return GCount;
	}


//////////////////////////////////////////////
const char *MBinFileTypeInfo::GetFileInfo(int index)
	{
	const int GCount=sizeof(GArray)/sizeof(GFileTypes)-2;
	if(index<0 || index>=GCount)
		{
		return NULL;
		}

	return GArray[index+1].FileInfo;
	}


///////////////////////////////////////////////
const char *MBinFileTypeInfo::GetFileExtension(int index)
	{
	const int GCount=sizeof(GArray)/sizeof(GFileTypes)-2;
	if(index<0 || index>=GCount)
		{
		return NULL;
		}

	return GArray[index+1].FileExtension;
	}


///////////////////////////////////////////////
const char *MBinFileTypeInfo::GetSignature(int index)
	{
	const int GCount=sizeof(GArray)/sizeof(GFileTypes)-2;
	if(index<0 || index>=GCount)
		{
		return NULL;
		}

	return GArray[index+1].HeaderPrefix;
	}


