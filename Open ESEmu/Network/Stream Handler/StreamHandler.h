#pragma once
#ifndef _STREAMHANDLER_H_
#define _STREAMHANDLER_H_

#include <iostream>
#include <vector>
#include <ctime>
#include "..\..\Common\Configuration\ConfigReader.h"
using namespace std;
#pragma warning (disable: 4996)
#pragma warning (disable: 4018)
#pragma warning (disable: 4081)
#pragma warning (disable: 4302)
#pragma warning (disable: 4554)

#define SECURE_BUFFER (this->m_isSafe)

template <typename T>
static void SwapEnd(T& var)
{
	T varSwapped;

	for (long i = 0; i < static_cast<long>(sizeof(var)); i++)
		((char*)(&varSwapped))[sizeof(var) - 1 - i] = ((char*)(&var))[i];

	for (long i = 0; i < static_cast<long>(sizeof(var)); i++)
		((char*)(&var))[i] = ((char*)(&varSwapped))[i];

}

class StreamHandler
{
private:

public:
	class Reader
	{
	private:
		unsigned int	m_Index;
		unsigned char*	m_Buffer;
		unsigned int	m_OriginalSize;
		bool			m_isSafe;

	public:

		Reader(char* _buffer, unsigned int _size)
		{
			this->m_Index = NULL;
			this->m_Buffer = (unsigned char*)malloc(_size);
			this->m_OriginalSize = _size;
			memset(this->m_Buffer, 0x00, _size);

			memcpy(this->m_Buffer, _buffer, _size);
			this->m_isSafe = true;
		}

		~Reader()
		{
			if (SECURE_BUFFER)
			{
				memset(this->m_Buffer, 0x00, this->m_OriginalSize);
				delete(this->m_Buffer);
				this->m_Index = NULL;
				this->m_OriginalSize = NULL;
				this->m_isSafe = NULL;
			}
		}

		void ReadByte(unsigned char& PlaceHolder)
		{
			if (SECURE_BUFFER)
			{
				this->m_Index += sizeof(unsigned char);
				PlaceHolder = this->m_Buffer[this->m_Index - 1];
			}
		}

		void ReadShort(unsigned short& PlaceHolder)
		{
			if (SECURE_BUFFER)
			{
				this->m_Index += sizeof(unsigned short);
				PlaceHolder = ((this->m_Buffer[this->m_Index - 2] << 8) | this->m_Buffer[this->m_Index - 1]);
			}
		}

		void ReadInt(unsigned int& PlaceHolder)
		{
			if (SECURE_BUFFER)
			{
				this->m_Index += sizeof(unsigned int);
				PlaceHolder = ((this->m_Buffer[this->m_Index - 4] << 24) | (this->m_Buffer[this->m_Index - 3] << 16) | (this->m_Buffer[this->m_Index - 2] << 8) | this->m_Buffer[this->m_Index - 1]);
			}
		}

		template <class T>
		void Read(T& PlaceHolder)
		{
			if (SECURE_BUFFER)
			{
				PlaceHolder = (T)(&this->m_Buffer + this->m_Index);
				this->m_Index += sizeof(T);
				return;
			}
			return;
		}

		void strncpy_m(char* dest, char* src, unsigned int size)
		{
			for (int i = 0; i < size; i++)
				dest[i] = src[this->m_Index++];
		}

		char* ReadString(int Size, char* String)
		{
			if (SECURE_BUFFER)
			{
				if (Size > 0 | Size < 255)
				{
					//char* String = (char*)malloc(Size);
					memset(String, 0x00, Size);

					strncpy_m(String, (char *)(this->m_Buffer), Size);
					return String;
				} else
					return "";
			} else
				return "";
		}

		void Skip(int Amount)
		{
			if (SECURE_BUFFER)
			{
				this->m_Index += Amount;
			}
		}

		std::string ReadUniString(unsigned int Size)
		{
			if (SECURE_BUFFER)
			{
				if (Size > 1)
				{
					std::string str;

					for (auto i = this->m_Index; i < this->m_Index + Size; i += 2)
						str += static_cast<char>(this->m_Buffer[i]);
					str += '\0';

					this->m_Index += Size;

					return str;
				}
			}
			return "";
		}

		unsigned int GetSize()
		{
			return this->m_OriginalSize;
		}

		unsigned char* GetBuffer()
		{
			return this->m_Buffer;
		}

		unsigned int GetIndex()
		{
			return this->m_Index;
		}

		void Erase(int Amount)
		{
			memset((char*)this->m_Buffer[this->m_Index], 0x00, Amount);
		}

		void EraseFrom(int From, int To)
		{
			memset((char*)this->m_Buffer[From], 0x00, To);
		}

		void AssignNewBuffer(unsigned char* _buffer)
		{
			this->m_Index = NULL;
			this->m_Buffer = _buffer;
		}
	};

	class Writer
	{
	private:
		char*			m_Buffer = nullptr;
		unsigned int	m_Size = NULL;
		unsigned int	m_Index = NULL;
		bool			m_isSafe = false;

	public:

		Writer(unsigned int _size, bool skipSizeHeader)
		{
			if (_size > 0 | _size < 2048)
			{
				this->m_Buffer = (char*)malloc(_size);
				memset(this->m_Buffer, 0x00, _size);

				this->m_Size = _size;
				if (skipSizeHeader)
					this->m_Index = 2;
				else
					this->m_Index = 0;
				this->m_isSafe = true;
			}
		}

		Writer(char* _buffer, unsigned int _size)
		{
			if (_size < 1 * (1024 * 1024 * 1024))
			{
				this->m_Buffer = (char*)malloc(_size);
				this->m_Size = _size;
				memset(this->m_Buffer, 0x00, _size);
				memcpy(this->m_Buffer, _buffer, this->m_Size);
				this->m_Index = 2;

				this->m_isSafe = true;
			}
			else
				_ASSERT(false && "Invalid buffer size");
		}

		~Writer()
		{
			if (SECURE_BUFFER)
			{
				memset(this->m_Buffer, 0x00, this->m_Size);
				delete(this->m_Buffer);
				this->m_Index = NULL;
				this->m_Size = NULL;
				this->m_isSafe = NULL;
			}
		}

		template <class T>
		void Write(const T value)
		{
			if (SECURE_BUFFER)
			{
				memcpy((this->m_Buffer) + this->m_Index, &value, sizeof(T));
				this->m_Index += sizeof(T);
			}
		}

		void WriteString(char* String, unsigned int Size)
		{
			if (SECURE_BUFFER)
			{
				strncpy(((char*)this->m_Buffer + this->m_Index), (const char*)String, Size);
				this->m_Index += Size;
			}
		}

		void WriteUniString(char* String, unsigned int Size)
		{
			if (SECURE_BUFFER)
			{
				if (Size > 0)
				{
					this->Write<unsigned int>((Size * 2));
					std::vector<unsigned char> finalized(0);

					for (auto i = 0; i <= Size; i++)
					{
						finalized.push_back(String[i]);
						if (i != Size)
							finalized.push_back(0x00);
					}

					for (int k = 0; k < (Size * 2); k++)
						memcpy((this->m_Buffer + this->m_Index) + k, (const unsigned char*)&finalized[k], sizeof(unsigned char));

					this->m_Index += Size * 2;
				}
			}
		}

		void WriteBuffer(char* Buffer, unsigned int Size)
		{
			if (SECURE_BUFFER)
			{
				if (Size > 0)
				{
					memcpy(((unsigned char*)this->m_Buffer) + this->m_Index, Buffer, Size);
					this->m_Index += Size;
				}
			}
		}

		void Skip(int Amount)
		{
			if (SECURE_BUFFER)
			{
				this->m_Index += Amount;
			}
		}

		void WriteOPCode(unsigned char _upper, unsigned char _lower)
		{
			if (SECURE_BUFFER)
			{
				memcpy((this->m_Buffer) + this->m_Index, (const void*)&_upper, sizeof(unsigned char));
				this->m_Index += sizeof(unsigned char);

				memcpy((this->m_Buffer) + this->m_Index, (const void*)&_lower, sizeof(unsigned char));
				this->m_Index += sizeof(unsigned char);
			}
		}

		void writeEmptyPrologue(unsigned char _upper, unsigned char _lower)
		{
			this->WriteBuffer("\x00\x00\x01\x01\x00\x00\x00\x01\x40\x00\x00\x9D\xC9\x86\x9F\x67\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 32);
			this->WriteOPCode(_upper, _lower);
			this->WriteBuffer("\x00\x00\x00\x00", 4);
		}

		void writePrologue(unsigned char _upper, unsigned char _lower)
		{
			this->WriteBuffer("\x00\x00\x01\x01\x00\x00\x00\x01\x40\x00\x00\x9D\xC9\x86\x9F\x67\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 32);
			this->WriteOPCode(_upper, _lower);
			this->WriteBuffer("\x00\x00\x00\x00\x00", 5);
		}

		void writeEpilogue()
		{
			this->m_Buffer[34] = ((this->m_Index - 39) >> 24) & 0xFF;
			this->m_Buffer[35] = ((this->m_Index - 39) >> 16) & 0xFF;
			this->m_Buffer[36] = ((this->m_Index - 39) >> 8) & 0xFF;
			this->m_Buffer[37] = (this->m_Index - 39) & 0xFF;

			int length = 8 - (this->m_Index % 8) - 1;

			if (length == 0) 
				length = 8;

			for (int i = 1; i <= length; i++) {
				this->m_Buffer[this->m_Index] = i;
				this->m_Index++;
			}

			this->m_Buffer[this->m_Index] = length;
			this->m_Index++;
		}

		unsigned int GetSize()
		{
			return this->m_Size;
		}

		char* GetBuffer()
		{
			return this->m_Buffer;
		}

		unsigned int GetIndex()
		{
			return this->m_Index;
		}

		void Erase(int Amount)
		{
			memset((char*)this->m_Buffer[this->m_Index], 0x00, Amount);
		}

		void EraseFrom(int From, int To)
		{
			memset((char*)this->m_Buffer[From], 0x00, To);
		}
	};

	Reader* r;
	Writer* w;

	StreamHandler(unsigned char* buff, unsigned int size)
	{
		if (size <= g_ConfigManager->getInstance()->getNumber(g_ConfigManager->MAX_TCP_WINDOW))
		{
			this->r = new Reader((char*)buff, size);
			//this->w = new Writer((char*)buff, size);
			for (int i = 0; i < size; i++)
				printf("%02X ", buff[i]);
		} else
			printf("[!] Refused buffer due to TCP Window too high (420).\n");
	}

	void setWriter(unsigned char* buff, unsigned int size)
	{
		this->w = new Writer((char*)buff, size);
	}

	void setReader(unsigned char* buff, unsigned int size)
	{
		this->r = new Reader((char*)buff, size);
	}

	void setReaderBuffer(unsigned char* buff)
	{
		this->r->AssignNewBuffer(buff);
	}

	std::string getCurrentDateTime()
	{
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "%Y%m%d%I%M%S", timeinfo);
		std::string str(buffer);
		return buffer;
	}

};

#endif