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

#ifndef _SmackerFileStream_h_
#define _SmackerFileStream_h_

#include <stdint.h>
#include <string>

#ifdef USE_SDL3
#include <SDL3/SDL_iostream.h>
#else
#include <SDL.h>
#endif

namespace SmackerCommon {

class FileStream
{
	public:
		FileStream()
		    : rwops(nullptr)
		{
		}

		~FileStream()
		{
			Close();
		}

#ifdef USE_SDL3
	    bool Open(SDL_IOStream *rwops);
#else
	    bool Open(SDL_RWops *rwops);
#endif
	    bool Is_Open();
		void Close();

		int32_t ReadBytes(uint8_t *data, uint32_t nBytes);

		uint32_t ReadUint32LE();
		uint32_t ReadUint32BE();

		uint16_t ReadUint16LE();
		uint16_t ReadUint16BE();

		uint8_t ReadByte();

		enum SeekDirection{
			kSeekCurrent = 0,
			kSeekStart   = 1,
			kSeekEnd     = 2
		};

		bool Seek(int32_t offset, SeekDirection = kSeekStart);
		bool Skip(int32_t offset);

		int32_t GetPosition();
		bool Is_Eos();

	private:
#ifdef USE_SDL3
	    SDL_IOStream *rwops;
#else
	    SDL_RWops *rwops;
#endif
	    bool is_eos;
};

} // close namespace SmackerCommon

#endif
