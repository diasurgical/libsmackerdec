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

#include <stdlib.h>

#ifdef USE_SDL3
#include <SDL3/SDL_endian.h>
#include <SDL3/SDL_iostream.h>
#else
#include <SDL.h>
#endif

namespace SmackerCommon {

#ifdef USE_SDL3
bool FileStream::Open(SDL_IOStream *rwops)
#else
bool FileStream::Open(SDL_RWops *rwops)
#endif
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
	if (Is_Open()) {
#if USE_SDL3
		SDL_CloseIO(rwops);
#else
		SDL_RWclose(rwops);
#endif
	}
	rwops = nullptr;
}

int32_t FileStream::ReadBytes(uint8_t *data, uint32_t nBytes)
{
	const size_t bytesRead =
#if USE_SDL3
	    SDL_ReadIO(rwops, data, nBytes);
#else
	    SDL_RWread(rwops, reinterpret_cast<void *>(data), 1, nBytes);
#endif
	is_eos = bytesRead == 0;
	return static_cast<int32_t>(bytesRead);
}

uint32_t FileStream::ReadUint32LE()
{
	Uint32 value;
	int32_t bytesRead = ReadBytes(reinterpret_cast<uint8_t *>(&value), 4);
	if (bytesRead < 4)
		return 0;
#ifdef USE_SDL3
	return SDL_Swap32LE(value);
#else
	return SDL_SwapLE32(value);
#endif
}

uint32_t FileStream::ReadUint32BE()
{
	Uint32 value;
	int32_t bytesRead = ReadBytes(reinterpret_cast<uint8_t *>(&value), 4);
	if (bytesRead < 4)
		return 0;
#ifdef USE_SDL3
	return SDL_Swap32BE(value);
#else
	return SDL_SwapBE32(value);
#endif
}

uint16_t FileStream::ReadUint16LE()
{
	Uint16 value;
	int32_t bytesRead = ReadBytes(reinterpret_cast<uint8_t *>(&value), 2);
	if (bytesRead < 2)
		return 0;
#ifdef USE_SDL3
	return SDL_Swap16LE(value);
#else
	return SDL_SwapLE16(value);
#endif
}

uint16_t FileStream::ReadUint16BE()
{
	Uint16 value;
	int32_t bytesRead = ReadBytes(reinterpret_cast<uint8_t *>(&value), 2);
	if (bytesRead < 2)
		return 0;
#ifdef USE_SDL3
	return SDL_Swap16BE(value);
#else
	return SDL_SwapBE16(value);
#endif
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
	switch (direction) {
	case kSeekStart:
#ifdef USE_SDL3
		return SDL_SeekIO(rwops, static_cast<Sint64>(offset), SDL_IO_SEEK_SET) != -1;
#else
		return SDL_RWseek(rwops, static_cast<Sint64>(offset), RW_SEEK_SET) >= 0;
#endif
	case kSeekCurrent:
#ifdef USE_SDL3
		return SDL_SeekIO(rwops, static_cast<Sint64>(offset), SDL_IO_SEEK_CUR) != -1;
#else
		return SDL_RWseek(rwops, static_cast<Sint64>(offset), RW_SEEK_CUR) >= 0;
#endif
	case kSeekEnd:
#ifdef USE_SDL3
		return SDL_SeekIO(rwops, static_cast<Sint64>(offset), SDL_IO_SEEK_END) != -1;
#else
		return SDL_RWseek(rwops, static_cast<Sint64>(offset), RW_SEEK_END) >= 0;
#endif
	}
	return false;
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
#ifdef USE_SDL3
	return static_cast<int32_t>(SDL_TellIO(rwops));
#else
	return static_cast<int32_t>(SDL_RWtell(rwops));
#endif
}

} // close namespace SmackerCommon
