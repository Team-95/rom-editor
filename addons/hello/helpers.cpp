#include "helpers.h"
#include "constantsTeam.h"
#include <string>

unsigned char* ReadRom(FILE *rom, unsigned int offset, int amountOfBytes) 
{
	unsigned char * bytes = new unsigned char[30];
	fseek(rom, offset, SEEK_SET);
	for (int i = 0; i < amountOfBytes; i++)
	{
		bytes[i] = fgetc(rom);
	}

	return bytes;
}

void WriteRom(FILE *rom, unsigned int offset, unsigned char* values, int amountOfBytes )
{
	fseek(rom, offset, SEEK_SET);
	for (int i = 0; i < amountOfBytes; i++){
		fputc(values[i], rom);
	}
}

unsigned char BytesToChar (unsigned char* arr)
{
	unsigned char temp = arr[0];
	delete arr;				// was created by ReadFromRom
	return temp;
}

unsigned short BytesToShort(unsigned char* arr)
{
	unsigned short byte1 = arr[0];
	unsigned short byte2 = arr[1];
	
	delete arr;
	return (byte1 << 8) | byte2;
}

unsigned int BytesToInt(unsigned char* arr)
{
	unsigned int byte1 = arr[0];
	unsigned int byte2 = arr[1];
	unsigned int byte3 = arr[2];
	unsigned int byte4 = arr[3];

	delete arr;
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | (byte4);
}

unsigned char* ShortToBytes(unsigned short val)
{
	unsigned char * bytes = new unsigned char[2];
	bytes[0] = val >> 8;
	bytes[1] = val & 0x00ff;
	return bytes;
}

unsigned char* IntToBytes(unsigned int val)
{
	unsigned char * bytes = new unsigned char[4];
	bytes[0] = val >> 24;
	bytes[1] = val >> 16;
	bytes[2] = val >> 8;
	bytes[3] = val & 0x000000ff;
		
	return bytes;
}

unsigned char StringToChar(string s)
{
	int value = atoi(s.c_str());
	return value;
}

unsigned short StringToShort(string s)
{
	int value = atoi(s.c_str());
	return value;
}

void CopyFile(FILE* source, FILE* dest)
{
	int ch;
	while ((ch = fgetc(source)) != EOF)
		fputc(ch, dest);
}

void TeamOffsetsFromAbbreviation(string cellVal, unsigned int &teamOffset, unsigned int &menuOffset)
{
	if (cellVal == "HOU")
	{
		teamOffset = T_HOUSTON;
		menuOffset = M_HOUSTON;
	}
	else if (cellVal == "ATL")
	{
		teamOffset = T_ATLANTA;
		menuOffset = M_ATLANTA;
	}
	else if (cellVal == "BOS")
	{
		teamOffset = T_BOSTON;
		menuOffset = M_BOSTON;
	}
	else if (cellVal == "BRK")
	{
		teamOffset = T_NEWJERSEY;
		menuOffset = M_NEWJERSEY;
	}
	else if (cellVal == "CHI")
	{
		teamOffset = T_CHICAGO;
		menuOffset = M_CHICAGO;
	}
	else if (cellVal == "CHO")
	{
		teamOffset = T_CHARLOTTE;
		menuOffset = M_CHARLOTTE;
	}
	else if (cellVal == "CLE")
	{
		teamOffset = T_CLEVELAND;
		menuOffset = M_CLEVELAND;
	}
	else if (cellVal == "DAL")
	{
		teamOffset = T_DALLAS;
		menuOffset = M_DALLAS;
	}
	else if (cellVal == "DEN")
	{
		teamOffset = T_DENVER;
		menuOffset = M_DENVER;
	}
	else if (cellVal == "DET")
	{
		teamOffset = T_DETROIT;
		menuOffset = M_DETROIT;
	}
	else if (cellVal == "GSW")
	{
		teamOffset = T_GOLDENSTATE;
		menuOffset = M_GOLDENSTATE;
	}
	else if (cellVal == "IND")
	{
		teamOffset = T_INDIANA;
		menuOffset = M_INDIANA;
	}
	else if (cellVal == "LAC")
	{
		teamOffset = T_LAC;
		menuOffset = M_LAC;
	}
	else if (cellVal == "LAL")
	{
		teamOffset = T_LAL;
		menuOffset = M_LAL;
	}
	else if (cellVal == "MEM")
	{
		teamOffset = T_STARSWEST;
		menuOffset = M_STARSWEST;
	}
	else if (cellVal == "MIA")
	{
		teamOffset = T_MIAMI;
		menuOffset = M_MIAMI;
	}
	else if (cellVal == "MIL")
	{
		teamOffset = T_MILWAUKEE;
		menuOffset = M_MILWAUKEE;
	}
	else if (cellVal == "MIN")
	{
		teamOffset = T_MINNESOTA;
		menuOffset = M_MINNESOTA;
	}
	else if (cellVal == "NOP")
	{
		//teamOffset = T_STARSEAST;
		//menuOffset = M_STARSEAST;
	}
	else if (cellVal == "NYK")
	{
		teamOffset = T_NEWYORK;
		menuOffset = M_NEWYORK;
	}
	else if (cellVal == "OKC")
	{
		teamOffset = T_SEATTLE;
		menuOffset = M_SEATTLE;
	}
	else if (cellVal == "ORL")
	{
		teamOffset = T_ORLANDO;
		menuOffset = M_ORLANDO;
	}
	else if (cellVal == "PHI")
	{
		teamOffset = T_PHILADELPHIA;
		menuOffset = M_PHILADELPHIA;
	}
	else if (cellVal == "PHO")
	{
		teamOffset = T_PHOENIX;
		menuOffset = M_PHOENIX;
	}
	else if (cellVal == "POR")
	{
		teamOffset = T_PORTLAND;
		menuOffset = M_PORTLAND;
	}
	else if (cellVal == "SAC")
	{
		teamOffset = T_SACRAMENTO;
		menuOffset = M_SACRAMENTO;
	}
	else if (cellVal == "SAS")
	{
		teamOffset = T_SANANTONIO;
		menuOffset = M_SANANTONIO;
	}
	else if (cellVal == "TOR")
	{
		teamOffset = T_STARSEAST;
		menuOffset = M_STARSEAST;
	}
	else if (cellVal == "UTA")
	{
		teamOffset = T_UTAH;
		menuOffset = M_UTAH;
	}
	else if (cellVal == "WAS")
	{
		teamOffset = T_WASHINGTON;
		menuOffset = M_WASHINGTON;
	}
}

unsigned char StringHexToChar(string s)
{
	unsigned char retVal = 0;

	if (s.length() == 2)
	{
		if (s[0] == '1')
		{
			retVal += 0x10;
		}
		else if (s[0] == '2')
		{
			retVal += 0x20;
		}

		if (s[1] >= '0' && s[1] <= '9')
		{
			retVal += s[1] - '0';
		}
		else if (s[1] >= 'A' && s[1] <= 'F')
		{
			retVal += s[1] - 'A' + 10;
		}
		else if (s[1] >= 'a' && s[1] <= 'f')
		{
			retVal += s[1] - 'f' + 10;
		}
	}
	else
	{
		retVal += s[0] - '0';
	}

	return retVal;
}

int GetText(unsigned int address, FILE* romRead, unsigned char* text, int length) {
	int size = 0;
	int ch;

	fseek(romRead, address, SEEK_SET);

	for (int i = 0; ; i++) {
		ch = fgetc(romRead);
	 	text[i] = ch;
		if(ch == 0 || (length > -1 && i == length - 1)) {
			text[length] = 0;
			break;
		}
	 	size++;
	}

	return size;
}

TeamAddressSet::TeamAddressSet()
{

}

TeamAddressSet::TeamAddressSet(unsigned int dataAddress, unsigned int menuAddress)
{
	this->DataAddress = dataAddress;
	this->MenuAddress = menuAddress;
}

TeamAddressSet* TeamAddressSet::GetAddresses()
{
	TeamAddressSet addresses[30];

	addresses[0] = TeamAddressSet(T_0, M_0);
	addresses[1] = TeamAddressSet(T_1, M_1);
	addresses[2] = TeamAddressSet(T_2, M_2);
	addresses[3] = TeamAddressSet(T_3, M_3);
	addresses[4] = TeamAddressSet(T_4, M_4);
	addresses[5] = TeamAddressSet(T_5, M_5);
	addresses[6] = TeamAddressSet(T_6, M_6);
	addresses[7] = TeamAddressSet(T_7, M_7);
	addresses[8] = TeamAddressSet(T_8, M_8);
	addresses[9] = TeamAddressSet(T_9, M_9);
	addresses[10] = TeamAddressSet(T_10, M_10);
	addresses[11] = TeamAddressSet(T_11, M_11);
	addresses[12] = TeamAddressSet(T_12, M_12);
	addresses[13] = TeamAddressSet(T_13, M_13);
	addresses[14] = TeamAddressSet(T_14, M_14);
	addresses[15] = TeamAddressSet(T_15, M_15);
	addresses[16] = TeamAddressSet(T_16, M_16);
	addresses[17] = TeamAddressSet(T_17, M_17);
	addresses[18] = TeamAddressSet(T_18, M_18);
	addresses[19] = TeamAddressSet(T_19, M_19);
	addresses[20] = TeamAddressSet(T_20, M_20);
	addresses[21] = TeamAddressSet(T_21, M_21);
	addresses[22] = TeamAddressSet(T_22, M_22);
	addresses[23] = TeamAddressSet(T_23, M_23);
	addresses[24] = TeamAddressSet(T_24, M_24);
	addresses[25] = TeamAddressSet(T_25, M_25);
	addresses[26] = TeamAddressSet(T_26, M_26);
	addresses[27] = TeamAddressSet(T_27, M_27);
	addresses[28] = TeamAddressSet(T_28, M_28);
	addresses[29] = TeamAddressSet(T_29, M_29);

	return addresses;
}
