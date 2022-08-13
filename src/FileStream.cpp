/*
 * libsmackerdec - Smacker video decoder
 * Copyright (C) 2011 Barry Duncan
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "FileStream.h"
#include <SDL_endian.h>
#include <stdlib.h>

namespace SmackerCommon {

bool FileStream::Open(SDL_RWops *rwops)
{
	this->rwops = rwops;
	is_eos = false;
	return rwops != nullptr;
}

bool FileStream::Is_Open()
{
	return rwops != nullptr;
}

void FileStream::Close()
{
	if (Is_Open())
		SDL_RWclose(rwops);
	rwops = nullptr;
}

int32_t FileStream::ReadBytes(uint8_t *data, uint32_t nBytes)
{
	size_t bytesRead = SDL_RWread(rwops, reinterpret_cast<void *>(data), 1, nBytes);
	is_eos = bytesRead == 0;
	return static_cast<int32_t>(bytesRead);
}

uint32_t FileStream::ReadUint32LE()
{
	Uint32 value;
	int32_t bytesRead = ReadBytes(reinterpret_cast<uint8_t *>(&value), 4);
	if (bytesRead < 4)
		return 0;
	return static_cast<uint32_t>(SDL_SwapLE32(value));
}

uint32_t FileStream::ReadUint32BE()
{
	Uint32 value;
	int32_t bytesRead = ReadBytes(reinterpret_cast<uint8_t *>(&value), 4);
	if (bytesRead < 4)
		return 0;
	return static_cast<uint32_t>(SDL_SwapBE32(value));
}

uint16_t FileStream::ReadUint16LE()
{
	Uint16 value;
	int32_t bytesRead = ReadBytes(reinterpret_cast<uint8_t *>(&value), 2);
	if (bytesRead < 2)
		return 0;
	return static_cast<uint16_t>(SDL_SwapLE16(value));
}

uint16_t FileStream::ReadUint16BE()
{
	Uint16 value;
	int32_t bytesRead = ReadBytes(reinterpret_cast<uint8_t *>(&value), 2);
	if (bytesRead < 2)
		return 0;
	return static_cast<uint16_t>(SDL_SwapBE16(value));
}

uint8_t FileStream::ReadByte()
{
	uint8_t value;
	int32_t bytesRead = ReadBytes(reinterpret_cast<uint8_t *>(&value), 1);
	if (bytesRead < 1)
		return 0;
	return value;
}

bool FileStream::Seek(int32_t offset, SeekDirection direction)
{
	int result = -1;
	if (kSeekStart == direction)
		result = SDL_RWseek(rwops, offset, SEEK_SET);
	else if (kSeekCurrent == direction)
		result = SDL_RWseek(rwops, offset, SEEK_CUR);
	else if (kSeekEnd == direction)
		result = SDL_RWseek(rwops, offset, SEEK_END);
	return result >= 0;
}

bool FileStream::Skip(int32_t offset)
{
	return Seek(offset, kSeekCurrent);
}

bool FileStream::Is_Eos()
{
	return is_eos;
}

int32_t FileStream::GetPosition()
{
	return static_cast<int32_t>(SDL_RWtell(rwops));
}

} // close namespace SmackerCommon
