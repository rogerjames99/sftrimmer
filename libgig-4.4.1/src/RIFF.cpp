/***************************************************************************
 *                                                                         *
 *   libgig - C++ cross-platform Gigasampler format file access library    *
 *                                                                         *
 *   Copyright (C) 2003-2024 by Christian Schoenebeck                      *
 *                              <cuse@users.sourceforge.net>               *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this library; if not, write to the Free Software           *
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston,                 *
 *   MA  02111-1307  USA                                                   *
 ***************************************************************************/

#include <algorithm>
#include <set>
#include <string.h>

#include "RIFF.h"

#include "helper.h"

#if POSIX
# include <errno.h>
#endif

namespace RIFF {

// *************** Internal functions **************
// *

    /// Returns a human readable path of the given chunk.
    static String __resolveChunkPath(Chunk* pCk) {
        String sPath;
        for (Chunk* pChunk = pCk; pChunk; pChunk = pChunk->GetParent()) {
            if (pChunk->GetChunkID() == CHUNK_ID_LIST) {
                List* pList = (List*) pChunk;
                sPath = "->'" + pList->GetListTypeString() + "'" + sPath;
            } else {
                sPath = "->'" + pChunk->GetChunkIDString() + "'" + sPath;
            }
        }
        return sPath;
    }

    inline static bool _isValidHandle(File::Handle handle) {
        #if defined(WIN32)
        return handle != INVALID_HANDLE_VALUE;
        #else
        return handle;
        #endif
    }

    inline static void _close(File::Handle handle) {
        if (!_isValidHandle(handle)) return;
        #if POSIX
        close(handle);
        #elif defined(WIN32)
        CloseHandle(handle);
        #else
        fclose(handle);
        #endif
    }



// *************** progress_t ***************
// *

    progress_t::progress_t() {
        callback    = NULL;
        custom      = NULL;
        __range_min = 0.0f;
        __range_max = 1.0f;
    }

    /**
     * Divides this progress task into the requested amount of equal weighted
     * sub-progress tasks and returns a vector with those subprogress tasks.
     *
     * @param iSubtasks - total amount sub tasks this task should be subdivided
     * @returns subtasks
     */
    std::vector<progress_t> progress_t::subdivide(int iSubtasks) {
        std::vector<progress_t> v;
        for (int i = 0; i < iSubtasks; ++i) {
            progress_t p;
            __divide_progress(this, &p, iSubtasks, i);
            v.push_back(p);
        }
        return v;
    }

    /**
     * Divides this progress task into the requested amount of sub-progress
     * tasks, where each one of those new sub-progress tasks is created with its
     * requested individual weight / portion, and finally returns a vector
     * with those new subprogress tasks.
     *
     * The amount of subprogresses to be created is determined by this method
     * by calling @c vSubTaskPortions.size() .
     *
     * Example: consider you wanted to create 3 subprogresses where the 1st
     * subtask should be assigned 10% of the new 3 subprogresses' overall
     * progress, the 2nd subtask should be assigned 50% of the new 3
     * subprogresses' overall progress, and the 3rd subtask should be assigned
     * 40%, then you might call this method like this:
     * @code
     * std::vector<progress_t> subprogresses = progress.subdivide({0.1, 0.5, 0.4});
     * @endcode
     *
     * @param vSubTaskPortions - amount and individual weight of subtasks to be
     *                           created
     * @returns subtasks
     */
    std::vector<progress_t> progress_t::subdivide(std::vector<float> vSubTaskPortions) {
        float fTotal = 0.f; // usually 1.0, but we sum the portions up below to be sure
        for (int i = 0; i < vSubTaskPortions.size(); ++i)
            fTotal += vSubTaskPortions[i];

        float fLow = 0.f, fHigh = 0.f;
        std::vector<progress_t> v;
        for (int i = 0; i < vSubTaskPortions.size(); ++i) {
            fLow  = fHigh;
            fHigh = vSubTaskPortions[i];
            progress_t p;
            __divide_progress(this, &p, fTotal, fLow, fHigh);
            v.push_back(p);
        }
        return v;
    }



// *************** Chunk **************
// *

    Chunk::Chunk(File* pFile) {
        #if DEBUG_RIFF
        std::cout << "Chunk::Chunk(File* pFile)" << std::endl;
        #endif // DEBUG_RIFF
        chunkPos.ullPos = 0;
        pParent    = NULL;
        pChunkData = NULL;
        ullCurrentChunkSize = 0;
        ullNewChunkSize = 0;
        ullChunkDataSize = 0;
        ChunkID    = CHUNK_ID_RIFF;
        this->pFile = pFile;
    }

    Chunk::Chunk(File* pFile, file_offset_t StartPos, List* Parent) {
        #if DEBUG_RIFF
        std::cout << "Chunk::Chunk(File*,file_offset_t,List*),StartPos=" << StartPos << std::endl;
        #endif // DEBUG_RIFF
        this->pFile   = pFile;
        ullStartPos   = StartPos + CHUNK_HEADER_SIZE(pFile->FileOffsetSize);
        pParent       = Parent;
        chunkPos.ullPos = 0;
        pChunkData    = NULL;
        ullCurrentChunkSize = 0;
        ullNewChunkSize = 0;
        ullChunkDataSize = 0;
        ReadHeader(StartPos);
    }

    Chunk::Chunk(File* pFile, List* pParent, uint32_t uiChunkID, file_offset_t ullBodySize) {
        this->pFile      = pFile;
        ullStartPos      = 0; // arbitrary usually, since it will be updated when we write the chunk
        this->pParent    = pParent;
        chunkPos.ullPos  = 0;
        pChunkData       = NULL;
        ChunkID          = uiChunkID;
        ullChunkDataSize = 0;
        ullCurrentChunkSize = 0;
        ullNewChunkSize  = ullBodySize;
    }

    Chunk::~Chunk() {
        if (pChunkData) delete[] pChunkData;
    }

    void Chunk::ReadHeader(file_offset_t filePos) {
        #if DEBUG_RIFF
        std::cout << "Chunk::Readheader(" << filePos << ") ";
        #endif // DEBUG_RIFF
        ChunkID = 0;
        ullNewChunkSize = ullCurrentChunkSize = 0;

        const File::Handle hRead = pFile->FileHandle();

        #if POSIX
        if (lseek(hRead, filePos, SEEK_SET) != -1) {
            read(hRead, &ChunkID, 4);
            read(hRead, &ullCurrentChunkSize, pFile->FileOffsetSize);
        #elif defined(WIN32)
        LARGE_INTEGER liFilePos;
        liFilePos.QuadPart = filePos;
        if (SetFilePointerEx(hRead, liFilePos, NULL/*new pos pointer*/, FILE_BEGIN)) {
            DWORD dwBytesRead;
            ReadFile(hRead, &ChunkID, 4, &dwBytesRead, NULL);
            ReadFile(hRead, &ullCurrentChunkSize, pFile->FileOffsetSize, &dwBytesRead, NULL);
        #else
        if (!fseeko(hRead, filePos, SEEK_SET)) {
            fread(&ChunkID, 4, 1, hRead);
            fread(&ullCurrentChunkSize, pFile->FileOffsetSize, 1, hRead);
        #endif // POSIX
            #if WORDS_BIGENDIAN
            if (ChunkID == CHUNK_ID_RIFF) {
                pFile->bEndianNative = false;
            }
            #else // little endian
            if (ChunkID == CHUNK_ID_RIFX) {
                pFile->bEndianNative = false;
                ChunkID = CHUNK_ID_RIFF;
            }
            #endif // WORDS_BIGENDIAN
            if (!pFile->bEndianNative) {
                //swapBytes_32(&ChunkID);
                if (pFile->FileOffsetSize == 4)
                    swapBytes_32(&ullCurrentChunkSize);
                else
                    swapBytes_64(&ullCurrentChunkSize);
            }
            #if DEBUG_RIFF
            std::cout << "ckID=" << convertToString(ChunkID) << " ";
            std::cout << "ckSize=" << ullCurrentChunkSize << " ";
            std::cout << "bEndianNative=" << pFile->bEndianNative << std::endl;
            #endif // DEBUG_RIFF
            ullNewChunkSize = ullCurrentChunkSize;
        }
    }

    void Chunk::WriteHeader(file_offset_t filePos) {
        uint32_t uiNewChunkID = ChunkID;
        if (ChunkID == CHUNK_ID_RIFF) {
            #if WORDS_BIGENDIAN
            if (pFile->bEndianNative) uiNewChunkID = CHUNK_ID_RIFX;
            #else // little endian
            if (!pFile->bEndianNative) uiNewChunkID = CHUNK_ID_RIFX;
            #endif // WORDS_BIGENDIAN
        }

        uint64_t ullNewChunkSize = this->ullNewChunkSize;
        if (!pFile->bEndianNative) {
            if (pFile->FileOffsetSize == 4)
                swapBytes_32(&ullNewChunkSize);
            else
                swapBytes_64(&ullNewChunkSize);
        }

        const File::Handle hWrite = pFile->FileWriteHandle();

        #if POSIX
        if (lseek(hWrite, filePos, SEEK_SET) != -1) {
            write(hWrite, &uiNewChunkID, 4);
            write(hWrite, &ullNewChunkSize, pFile->FileOffsetSize);
        }
        #elif defined(WIN32)
        LARGE_INTEGER liFilePos;
        liFilePos.QuadPart = filePos;
        if (SetFilePointerEx(hWrite, liFilePos, NULL/*new pos pointer*/, FILE_BEGIN)) {
            DWORD dwBytesWritten;
            WriteFile(hWrite, &uiNewChunkID, 4, &dwBytesWritten, NULL);
            WriteFile(hWrite, &ullNewChunkSize, pFile->FileOffsetSize, &dwBytesWritten, NULL);
        }
        #else
        if (!fseeko(hWrite, filePos, SEEK_SET)) {
            fwrite(&uiNewChunkID, 4, 1, hWrite);
            fwrite(&ullNewChunkSize, pFile->FileOffsetSize, 1, hWrite);
        }
        #endif // POSIX
    }

    /**
     *  Returns the String representation of the chunk's ID (e.g. "RIFF",
     *  "LIST").
     */
    String Chunk::GetChunkIDString() const {
        return convertToString(ChunkID);
    }

    /**
     * This is an internal-only method which must not be used by any application
     * and might change at any time.
     *
     * Returns a reference (memory location) of the chunk's current file
     * (read/write) position variable which depends on the current value of
     * File::IsIOPerThread().
     */
    file_offset_t& Chunk::GetPosUnsafeRef() {
        if (!pFile->IsIOPerThread()) return chunkPos.ullPos;
        const std::thread::id tid = std::this_thread::get_id();
        return chunkPos.byThread[tid];
    }

    /**
     * Current read/write position within the chunk data body (starting with 0).
     *
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::GetPos() const {
        if (!pFile->IsIOPerThread()) return chunkPos.ullPos;
        const std::thread::id tid = std::this_thread::get_id();
        std::lock_guard<std::mutex> lock(chunkPos.mutex);
        return chunkPos.byThread[tid];
    }

    /**
     * Current, actual offset in file of current chunk data body read/write
     * position.
     *
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::GetFilePos() const {
        return ullStartPos + GetPos();
    }

    /**
     *  Sets the position within the chunk body, thus within the data portion
     *  of the chunk (in bytes).
     *
     *  <b>Caution:</b> the position will be reset to zero whenever
     *  File::Save() was called.
     *
     *  @param Where  - position offset (in bytes)
     *  @param Whence - optional: defines to what <i>\a Where</i> relates to,
     *                  if omitted \a Where relates to beginning of the chunk
     *                  data
     *  @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::SetPos(file_offset_t Where, stream_whence_t Whence) {
        #if DEBUG_RIFF
        std::cout << "Chunk::SetPos(file_offset_t,stream_whence_t)" << std::endl;
        #endif // DEBUG_RIFF
        std::lock_guard<std::mutex> lock(chunkPos.mutex);
        file_offset_t& pos = GetPosUnsafeRef();
        switch (Whence) {
            case stream_curpos:
                pos += Where;
                break;
            case stream_end:
                pos = ullCurrentChunkSize - 1 - Where;
                break;
            case stream_backward:
                pos -= Where;
                break;
            case stream_start: default:
                pos = Where;
                break;
        }
        if (pos > ullCurrentChunkSize) pos = ullCurrentChunkSize;
        return pos;
    }

    /**
     *  Returns the number of bytes left to read in the chunk body.
     *  When reading data from the chunk using the Read*() Methods, the
     *  position within the chunk data (that is the chunk body) will be
     *  incremented by the number of read bytes and RemainingBytes() returns
     *  how much data is left to read from the current position to the end
     *  of the chunk data.
     *
     *  @returns  number of bytes left to read
     *  @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::RemainingBytes() const {
        #if DEBUG_RIFF
        std::cout << "Chunk::Remainingbytes()=" << ullCurrentChunkSize - ullPos << std::endl;
        #endif // DEBUG_RIFF
        const file_offset_t pos = GetPos();
        return (ullCurrentChunkSize > pos) ? ullCurrentChunkSize - pos : 0;
    }

    /**
     *  Returns the actual total size in bytes (including header) of this Chunk
     *  if being stored to a file.
     *
     *  @param fileOffsetSize - RIFF file offset size (in bytes) assumed when 
     *                          being saved to a file
     */
    file_offset_t Chunk::RequiredPhysicalSize(int fileOffsetSize) {
        return CHUNK_HEADER_SIZE(fileOffsetSize) + // RIFF chunk header
               ullNewChunkSize + // chunks's actual data body
               ullNewChunkSize % 2; // optional pad byte
    }

    /**
     *  Returns the current state of the chunk object.
     *  Following values are possible:
     *  - RIFF::stream_ready :
     *    chunk data can be read (this is the usual case)
     *  - RIFF::stream_closed :
     *    the data stream was closed somehow, no more reading possible
     *  - RIFF::stream_end_reached :
     *    already reached the end of the chunk data, no more reading
     *    possible without SetPos()
     *
     *  @see File::IsIOPerThread() for multi-threaded streaming
     */
    stream_state_t Chunk::GetState() const {
        #if DEBUG_RIFF
        std::cout << "Chunk::GetState()" << std::endl;
        #endif // DEBUG_RIFF

        const File::Handle hRead = pFile->FileHandle();

        if (!_isValidHandle(hRead))
            return stream_closed;

        const file_offset_t pos = GetPos();
        if (pos < ullCurrentChunkSize)    return stream_ready;
        else                              return stream_end_reached;
    }

    /**
     *  Reads \a WordCount number of data words with given \a WordSize and
     *  copies it into a buffer pointed by \a pData. The buffer has to be
     *  allocated and be sure to provide the correct \a WordSize, as this
     *  will be important and taken into account for eventual endian
     *  correction (swapping of bytes due to different native byte order of
     *  a system). The position within the chunk will automatically be
     *  incremented.
     *
     *  @param pData      destination buffer
     *  @param WordCount  number of data words to read
     *  @param WordSize   size of each data word to read
     *  @returns          number of successfully read data words or 0 if end
     *                    of file reached or error occurred
     *  @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::Read(void* pData, file_offset_t WordCount, file_offset_t WordSize) {
        #if DEBUG_RIFF
        std::cout << "Chunk::Read(void*,file_offset_t,file_offset_t)" << std::endl;
        #endif // DEBUG_RIFF
        //if (ulStartPos == 0) return 0; // is only 0 if this is a new chunk, so nothing to read (yet)
        const file_offset_t pos = GetPos();
        if (pos >= ullCurrentChunkSize) return 0;
        if (pos + WordCount * WordSize >= ullCurrentChunkSize)
            WordCount = (ullCurrentChunkSize - pos) / WordSize;

        const File::Handle hRead = pFile->FileHandle();

        #if POSIX
        if (lseek(hRead, ullStartPos + pos, SEEK_SET) < 0) return 0;
        ssize_t readWords = read(hRead, pData, WordCount * WordSize);
        if (readWords < 1) {
            #if DEBUG_RIFF
            std::cerr << "POSIX read() failed: " << strerror(errno) << std::endl << std::flush;
            #endif // DEBUG_RIFF
            return 0;
        }
        readWords /= WordSize;
        #elif defined(WIN32)
        LARGE_INTEGER liFilePos;
        liFilePos.QuadPart = ullStartPos + pos;
        if (!SetFilePointerEx(hRead, liFilePos, NULL/*new pos pointer*/, FILE_BEGIN))
            return 0;
        DWORD readWords;
        ReadFile(hRead, pData, WordCount * WordSize, &readWords, NULL); //FIXME: does not work for reading buffers larger than 2GB (even though this should rarely be the case in practice)
        if (readWords < 1) return 0;
        readWords /= WordSize;
        #else // standard C functions
        if (fseeko(hRead, ullStartPos + pos, SEEK_SET)) return 0;
        file_offset_t readWords = fread(pData, WordSize, WordCount, hRead);
        #endif // POSIX
        if (!pFile->bEndianNative && WordSize != 1) {
            switch (WordSize) {
                case 2:
                    for (file_offset_t iWord = 0; iWord < readWords; iWord++)
                        swapBytes_16((uint16_t*) pData + iWord);
                    break;
                case 4:
                    for (file_offset_t iWord = 0; iWord < readWords; iWord++)
                        swapBytes_32((uint32_t*) pData + iWord);
                    break;
                case 8:
                    for (file_offset_t iWord = 0; iWord < readWords; iWord++)
                        swapBytes_64((uint64_t*) pData + iWord);
                    break;
                default:
                    for (file_offset_t iWord = 0; iWord < readWords; iWord++)
                        swapBytes((uint8_t*) pData + iWord * WordSize, WordSize);
                    break;
            }
        }
        SetPos(readWords * WordSize, stream_curpos);
        return readWords;
    }

    /**
     *  Writes \a WordCount number of data words with given \a WordSize from
     *  the buffer pointed by \a pData. Be sure to provide the correct
     *  \a WordSize, as this will be important and taken into account for
     *  eventual endian correction (swapping of bytes due to different
     *  native byte order of a system). The position within the chunk will
     *  automatically be incremented.
     *
     *  @param pData      source buffer (containing the data)
     *  @param WordCount  number of data words to write
     *  @param WordSize   size of each data word to write
     *  @returns          number of successfully written data words
     *  @throws RIFF::Exception  if write operation would exceed current
     *                           chunk size or any IO error occurred
     *  @see Resize()
     *  @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::Write(void* pData, file_offset_t WordCount, file_offset_t WordSize) {
        const File::HandlePair io = pFile->FileHandlePair();
        if (io.Mode != stream_mode_read_write)
            throw Exception("Cannot write data to chunk, file has to be opened in read+write mode first");
        const file_offset_t pos = GetPos();
        if (pos >= ullCurrentChunkSize || pos + WordCount * WordSize > ullCurrentChunkSize)
            throw Exception("End of chunk reached while trying to write data");
        if (!pFile->bEndianNative && WordSize != 1) {
            switch (WordSize) {
                case 2:
                    for (file_offset_t iWord = 0; iWord < WordCount; iWord++)
                        swapBytes_16((uint16_t*) pData + iWord);
                    break;
                case 4:
                    for (file_offset_t iWord = 0; iWord < WordCount; iWord++)
                        swapBytes_32((uint32_t*) pData + iWord);
                    break;
                case 8:
                    for (file_offset_t iWord = 0; iWord < WordCount; iWord++)
                        swapBytes_64((uint64_t*) pData + iWord);
                    break;
                default:
                    for (file_offset_t iWord = 0; iWord < WordCount; iWord++)
                        swapBytes((uint8_t*) pData + iWord * WordSize, WordSize);
                    break;
            }
        }
        #if POSIX
        if (lseek(io.hWrite, ullStartPos + pos, SEEK_SET) < 0) {
            throw Exception("Could not seek to position " + ToString(pos) +
                            " in chunk (" + ToString(ullStartPos + pos) + " in file)");
        }
        ssize_t writtenWords = write(io.hWrite, pData, WordCount * WordSize);
        if (writtenWords < 1) throw Exception("POSIX IO Error while trying to write chunk data");
        writtenWords /= WordSize;
        #elif defined(WIN32)
        LARGE_INTEGER liFilePos;
        liFilePos.QuadPart = ullStartPos + pos;
        if (!SetFilePointerEx(io.hWrite, liFilePos, NULL/*new pos pointer*/, FILE_BEGIN)) {
            throw Exception("Could not seek to position " + ToString(pos) +
                            " in chunk (" + ToString(ullStartPos + pos) + " in file)");
        }
        DWORD writtenWords;
        WriteFile(io.hWrite, pData, WordCount * WordSize, &writtenWords, NULL); //FIXME: does not work for writing buffers larger than 2GB (even though this should rarely be the case in practice)
        if (writtenWords < 1) throw Exception("Windows IO Error while trying to write chunk data");
        writtenWords /= WordSize;
        #else // standard C functions
        if (fseeko(io.hWrite, ullStartPos + pos, SEEK_SET)) {
            throw Exception("Could not seek to position " + ToString(pos) +
                            " in chunk (" + ToString(ullStartPos + pos) + " in file)");
        }
        file_offset_t writtenWords = fwrite(pData, WordSize, WordCount, io.hWrite);
        #endif // POSIX
        SetPos(writtenWords * WordSize, stream_curpos);
        return writtenWords;
    }

    /** Just an internal wrapper for the main <i>Read()</i> method with additional Exception throwing on errors. */
    file_offset_t Chunk::ReadSceptical(void* pData, file_offset_t WordCount, file_offset_t WordSize) {
        file_offset_t readWords = Read(pData, WordCount, WordSize);
        if (readWords != WordCount) throw RIFF::Exception("End of chunk data reached.");
        return readWords;
    }

    /**
     * Reads \a WordCount number of 8 Bit signed integer words and copies it
     * into the buffer pointed by \a pData. The buffer has to be allocated.
     * The position within the chunk will automatically be incremented.
     *
     * @param pData             destination buffer
     * @param WordCount         number of 8 Bit signed integers to read
     * @returns                 number of read integers
     * @throws RIFF::Exception  if an error occurred or less than
     *                          \a WordCount integers could be read!
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::ReadInt8(int8_t* pData, file_offset_t WordCount) {
        #if DEBUG_RIFF
        std::cout << "Chunk::ReadInt8(int8_t*,file_offset_t)" << std::endl;
        #endif // DEBUG_RIFF
        return ReadSceptical(pData, WordCount, 1);
    }

    /**
     * Writes \a WordCount number of 8 Bit signed integer words from the
     * buffer pointed by \a pData to the chunk's body, directly to the
     * actual "physical" file. The position within the chunk will
     * automatically be incremented. Note: you cannot write beyond the
     * boundaries of the chunk, to append data to the chunk call Resize()
     * before.
     *
     * @param pData             source buffer (containing the data)
     * @param WordCount         number of 8 Bit signed integers to write
     * @returns                 number of written integers
     * @throws RIFF::Exception  if an IO error occurred
     * @see Resize()
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::WriteInt8(int8_t* pData, file_offset_t WordCount) {
        return Write(pData, WordCount, 1);
    }

    /**
     * Reads \a WordCount number of 8 Bit unsigned integer words and copies
     * it into the buffer pointed by \a pData. The buffer has to be
     * allocated. The position within the chunk will automatically be
     * incremented.
     *
     * @param pData             destination buffer
     * @param WordCount         number of 8 Bit unsigned integers to read
     * @returns                 number of read integers
     * @throws RIFF::Exception  if an error occurred or less than
     *                          \a WordCount integers could be read!
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::ReadUint8(uint8_t* pData, file_offset_t WordCount) {
        #if DEBUG_RIFF
        std::cout << "Chunk::ReadUint8(uint8_t*,file_offset_t)" << std::endl;
        #endif // DEBUG_RIFF
        return ReadSceptical(pData, WordCount, 1);
    }

    /**
     * Writes \a WordCount number of 8 Bit unsigned integer words from the
     * buffer pointed by \a pData to the chunk's body, directly to the
     * actual "physical" file. The position within the chunk will
     * automatically be incremented. Note: you cannot write beyond the
     * boundaries of the chunk, to append data to the chunk call Resize()
     * before.
     *
     * @param pData             source buffer (containing the data)
     * @param WordCount         number of 8 Bit unsigned integers to write
     * @returns                 number of written integers
     * @throws RIFF::Exception  if an IO error occurred
     * @see Resize()
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::WriteUint8(uint8_t* pData, file_offset_t WordCount) {
        return Write(pData, WordCount, 1);
    }

    /**
     * Reads \a WordCount number of 16 Bit signed integer words and copies
     * it into the buffer pointed by \a pData. The buffer has to be
     * allocated. Endian correction will automatically be done if needed.
     * The position within the chunk will automatically be incremented.
     *
     * @param pData             destination buffer
     * @param WordCount         number of 16 Bit signed integers to read
     * @returns                 number of read integers
     * @throws RIFF::Exception  if an error occurred or less than
     *                          \a WordCount integers could be read!
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::ReadInt16(int16_t* pData, file_offset_t WordCount) {
        #if DEBUG_RIFF
        std::cout << "Chunk::ReadInt16(int16_t*,file_offset_t)" << std::endl;
        #endif // DEBUG_RIFF
        return ReadSceptical(pData, WordCount, 2);
    }

    /**
     * Writes \a WordCount number of 16 Bit signed integer words from the
     * buffer pointed by \a pData to the chunk's body, directly to the
     * actual "physical" file. The position within the chunk will
     * automatically be incremented. Note: you cannot write beyond the
     * boundaries of the chunk, to append data to the chunk call Resize()
     * before.
     *
     * @param pData             source buffer (containing the data)
     * @param WordCount         number of 16 Bit signed integers to write
     * @returns                 number of written integers
     * @throws RIFF::Exception  if an IO error occurred
     * @see Resize()
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::WriteInt16(int16_t* pData, file_offset_t WordCount) {
        return Write(pData, WordCount, 2);
    }

    /**
     * Reads \a WordCount number of 16 Bit unsigned integer words and copies
     * it into the buffer pointed by \a pData. The buffer has to be
     * allocated. Endian correction will automatically be done if needed.
     * The position within the chunk will automatically be incremented.
     *
     * @param pData             destination buffer
     * @param WordCount         number of 8 Bit unsigned integers to read
     * @returns                 number of read integers
     * @throws RIFF::Exception  if an error occurred or less than
     *                          \a WordCount integers could be read!
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::ReadUint16(uint16_t* pData, file_offset_t WordCount) {
        #if DEBUG_RIFF
        std::cout << "Chunk::ReadUint16(uint16_t*,file_offset_t)" << std::endl;
        #endif // DEBUG_RIFF
        return ReadSceptical(pData, WordCount, 2);
    }

    /**
     * Writes \a WordCount number of 16 Bit unsigned integer words from the
     * buffer pointed by \a pData to the chunk's body, directly to the
     * actual "physical" file. The position within the chunk will
     * automatically be incremented. Note: you cannot write beyond the
     * boundaries of the chunk, to append data to the chunk call Resize()
     * before.
     *
     * @param pData             source buffer (containing the data)
     * @param WordCount         number of 16 Bit unsigned integers to write
     * @returns                 number of written integers
     * @throws RIFF::Exception  if an IO error occurred
     * @see Resize()
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::WriteUint16(uint16_t* pData, file_offset_t WordCount) {
        return Write(pData, WordCount, 2);
    }

    /**
     * Reads \a WordCount number of 32 Bit signed integer words and copies
     * it into the buffer pointed by \a pData. The buffer has to be
     * allocated. Endian correction will automatically be done if needed.
     * The position within the chunk will automatically be incremented.
     *
     * @param pData             destination buffer
     * @param WordCount         number of 32 Bit signed integers to read
     * @returns                 number of read integers
     * @throws RIFF::Exception  if an error occurred or less than
     *                          \a WordCount integers could be read!
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::ReadInt32(int32_t* pData, file_offset_t WordCount) {
        #if DEBUG_RIFF
        std::cout << "Chunk::ReadInt32(int32_t*,file_offset_t)" << std::endl;
        #endif // DEBUG_RIFF
        return ReadSceptical(pData, WordCount, 4);
    }

    /**
     * Writes \a WordCount number of 32 Bit signed integer words from the
     * buffer pointed by \a pData to the chunk's body, directly to the
     * actual "physical" file. The position within the chunk will
     * automatically be incremented. Note: you cannot write beyond the
     * boundaries of the chunk, to append data to the chunk call Resize()
     * before.
     *
     * @param pData             source buffer (containing the data)
     * @param WordCount         number of 32 Bit signed integers to write
     * @returns                 number of written integers
     * @throws RIFF::Exception  if an IO error occurred
     * @see Resize()
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::WriteInt32(int32_t* pData, file_offset_t WordCount) {
        return Write(pData, WordCount, 4);
    }

    /**
     * Reads \a WordCount number of 32 Bit unsigned integer words and copies
     * it into the buffer pointed by \a pData. The buffer has to be
     * allocated. Endian correction will automatically be done if needed.
     * The position within the chunk will automatically be incremented.
     *
     * @param pData             destination buffer
     * @param WordCount         number of 32 Bit unsigned integers to read
     * @returns                 number of read integers
     * @throws RIFF::Exception  if an error occurred or less than
     *                          \a WordCount integers could be read!
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::ReadUint32(uint32_t* pData, file_offset_t WordCount) {
        #if DEBUG_RIFF
        std::cout << "Chunk::ReadUint32(uint32_t*,file_offset_t)" << std::endl;
        #endif // DEBUG_RIFF
        return ReadSceptical(pData, WordCount, 4);
    }

    /**
     * Reads a null-padded string of size characters and copies it
     * into the string \a s. The position within the chunk will
     * automatically be incremented.
     *
     * @param s                 destination string
     * @param size              number of characters to read
     * @throws RIFF::Exception  if an error occurred or less than
     *                          \a size characters could be read!
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    void Chunk::ReadString(String& s, int size) {
        char* buf = new char[size];
        ReadSceptical(buf, 1, size);
        s.assign(buf, std::find(buf, buf + size, '\0'));
        delete[] buf;
    }

    /**
     * Writes \a WordCount number of 32 Bit unsigned integer words from the
     * buffer pointed by \a pData to the chunk's body, directly to the
     * actual "physical" file. The position within the chunk will
     * automatically be incremented. Note: you cannot write beyond the
     * boundaries of the chunk, to append data to the chunk call Resize()
     * before.
     *
     * @param pData             source buffer (containing the data)
     * @param WordCount         number of 32 Bit unsigned integers to write
     * @returns                 number of written integers
     * @throws RIFF::Exception  if an IO error occurred
     * @see Resize()
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::WriteUint32(uint32_t* pData, file_offset_t WordCount) {
        return Write(pData, WordCount, 4);
    }

    /**
     * Reads one 8 Bit signed integer word and increments the position within
     * the chunk.
     *
     * @returns                 read integer word
     * @throws RIFF::Exception  if an error occurred
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    int8_t Chunk::ReadInt8() {
        #if DEBUG_RIFF
        std::cout << "Chunk::ReadInt8()" << std::endl;
        #endif // DEBUG_RIFF
        int8_t word;
        ReadSceptical(&word,1,1);
        return word;
    }

    /**
     * Reads one 8 Bit unsigned integer word and increments the position
     * within the chunk.
     *
     * @returns                 read integer word
     * @throws RIFF::Exception  if an error occurred
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    uint8_t Chunk::ReadUint8() {
        #if DEBUG_RIFF
        std::cout << "Chunk::ReadUint8()" << std::endl;
        #endif // DEBUG_RIFF
        uint8_t word;
        ReadSceptical(&word,1,1);
        return word;
    }

    /**
     * Reads one 16 Bit signed integer word and increments the position
     * within the chunk. Endian correction will automatically be done if
     * needed.
     *
     * @returns                 read integer word
     * @throws RIFF::Exception  if an error occurred
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    int16_t Chunk::ReadInt16() {
        #if DEBUG_RIFF
        std::cout << "Chunk::ReadInt16()" << std::endl;
        #endif // DEBUG_RIFF
        int16_t word;
        ReadSceptical(&word,1,2);
        return word;
    }

    /**
     * Reads one 16 Bit unsigned integer word and increments the position
     * within the chunk. Endian correction will automatically be done if
     * needed.
     *
     * @returns                 read integer word
     * @throws RIFF::Exception  if an error occurred
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    uint16_t Chunk::ReadUint16() {
        #if DEBUG_RIFF
        std::cout << "Chunk::ReadUint16()" << std::endl;
        #endif // DEBUG_RIFF
        uint16_t word;
        ReadSceptical(&word,1,2);
        return word;
    }

    /**
     * Reads one 32 Bit signed integer word and increments the position
     * within the chunk. Endian correction will automatically be done if
     * needed.
     *
     * @returns                 read integer word
     * @throws RIFF::Exception  if an error occurred
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    int32_t Chunk::ReadInt32() {
        #if DEBUG_RIFF
        std::cout << "Chunk::ReadInt32()" << std::endl;
        #endif // DEBUG_RIFF
        int32_t word;
        ReadSceptical(&word,1,4);
        return word;
    }

    /**
     * Reads one 32 Bit unsigned integer word and increments the position
     * within the chunk. Endian correction will automatically be done if
     * needed.
     *
     * @returns                 read integer word
     * @throws RIFF::Exception  if an error occurred
     * @see File::IsIOPerThread() for multi-threaded streamings
     */
    uint32_t Chunk::ReadUint32() {
        #if DEBUG_RIFF
        std::cout << "Chunk::ReadUint32()" << std::endl;
        #endif // DEBUG_RIFF
        uint32_t word;
        ReadSceptical(&word,1,4);
        return word;
    }

    /** @brief Load chunk body into RAM.
     *
     * Loads the whole chunk body into memory. You can modify the data in
     * RAM and save the data by calling File::Save() afterwards.
     *
     * <b>Caution:</b> the buffer pointer will be invalidated once
     * File::Save() was called. You have to call LoadChunkData() again to
     * get a new, valid pointer whenever File::Save() was called.
     *
     * You can call LoadChunkData() again if you previously scheduled to
     * enlarge this chunk with a Resize() call. In that case the buffer will
     * be enlarged to the new, scheduled chunk size and you can already
     * place the new chunk data to the buffer and finally call File::Save()
     * to enlarge the chunk physically and write the new data in one rush.
     * This approach is definitely recommended if you have to enlarge and
     * write new data to a lot of chunks.
     *
     * @returns a pointer to the data in RAM on success, NULL otherwise
     * @throws Exception if data buffer could not be enlarged
     * @see ReleaseChunkData()
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    void* Chunk::LoadChunkData() {
        if (!pChunkData && pFile->Filename != "" /*&& ulStartPos != 0*/) {
            File::Handle hRead = pFile->FileHandle();
            #if POSIX
            if (lseek(hRead, ullStartPos, SEEK_SET) == -1) return NULL;
            #elif defined(WIN32)
            LARGE_INTEGER liFilePos;
            liFilePos.QuadPart = ullStartPos;
            if (!SetFilePointerEx(hRead, liFilePos, NULL/*new pos pointer*/, FILE_BEGIN)) return NULL;
            #else
            if (fseeko(hRead, ullStartPos, SEEK_SET)) return NULL;
            #endif // POSIX
            file_offset_t ullBufferSize = (ullCurrentChunkSize > ullNewChunkSize) ? ullCurrentChunkSize : ullNewChunkSize;
            pChunkData = new uint8_t[ullBufferSize];
            if (!pChunkData) return NULL;
            memset(pChunkData, 0, ullBufferSize);
            #if POSIX
            file_offset_t readWords = read(hRead, pChunkData, GetSize());
            #elif defined(WIN32)
            DWORD readWords;
            ReadFile(hRead, pChunkData, GetSize(), &readWords, NULL); //FIXME: won't load chunks larger than 2GB !
            #else
            file_offset_t readWords = fread(pChunkData, 1, GetSize(), hRead);
            #endif // POSIX
            if (readWords != GetSize()) {
                delete[] pChunkData;
                return (pChunkData = NULL);
            }
            ullChunkDataSize = ullBufferSize;
        } else if (ullNewChunkSize > ullChunkDataSize) {
            uint8_t* pNewBuffer = new uint8_t[ullNewChunkSize];
            if (!pNewBuffer) throw Exception("Could not enlarge chunk data buffer to " + ToString(ullNewChunkSize) + " bytes");
            memset(pNewBuffer, 0 , ullNewChunkSize);
            if (pChunkData) {
                memcpy(pNewBuffer, pChunkData, ullChunkDataSize);
                delete[] pChunkData;
            }
            pChunkData       = pNewBuffer;
            ullChunkDataSize = ullNewChunkSize;
        }
        return pChunkData;
    }

    /** @brief Free loaded chunk body from RAM.
     *
     * Frees loaded chunk body data from memory (RAM). You should call
     * File::Save() before calling this method if you modified the data to
     * make the changes persistent.
     */
    void Chunk::ReleaseChunkData() {
        if (pChunkData) {
            delete[] pChunkData;
            pChunkData = NULL;
        }
    }

    /** @brief Resize chunk.
     *
     * Resizes this chunk's body, that is the actual size of data possible
     * to be written to this chunk. This call will return immediately and
     * just schedule the resize operation. You should call File::Save() to
     * actually perform the resize operation(s) "physically" to the file.
     * As this can take a while on large files, it is recommended to call
     * Resize() first on all chunks which have to be resized and finally to
     * call File::Save() to perform all those resize operations in one rush.
     *
     * <b>Caution:</b> You cannot directly write to enlarged chunks before
     * calling File::Save() as this might exceed the current chunk's body
     * boundary!
     *
     * @param NewSize - new chunk body size in bytes (must be greater than zero)
     * @throws RIFF::Exception  if \a NewSize is less than 1 or unrealistic large
     * @see File::Save()
     */
    void Chunk::Resize(file_offset_t NewSize) {
        if (NewSize == 0)
            throw Exception("There is at least one empty chunk (zero size): " + __resolveChunkPath(this));
        if ((NewSize >> 48) != 0)
            throw Exception("Unrealistic high chunk size detected: " + __resolveChunkPath(this));
        if (ullNewChunkSize == NewSize) return;
        ullNewChunkSize = NewSize;
    }

    /** @brief Write chunk persistently e.g. to disk.
     *
     * Stores the chunk persistently to its actual "physical" file.
     *
     * @param ullWritePos - position within the "physical" file where this
     *                     chunk should be written to
     * @param ullCurrentDataOffset - offset of current (old) data within
     *                              the file
     * @param pProgress - optional: callback function for progress notification
     * @returns new write position in the "physical" file, that is
     *          \a ullWritePos incremented by this chunk's new size
     *          (including its header size of course)
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    file_offset_t Chunk::WriteChunk(file_offset_t ullWritePos, file_offset_t ullCurrentDataOffset, progress_t* pProgress) {
        const file_offset_t ullOriginalPos = ullWritePos;
        ullWritePos += CHUNK_HEADER_SIZE(pFile->FileOffsetSize);

        const File::HandlePair io = pFile->FileHandlePair();

        if (io.Mode != stream_mode_read_write)
            throw Exception("Cannot write list chunk, file has to be opened in read+write mode");

        // if the whole chunk body was loaded into RAM
        if (pChunkData) {
            // make sure chunk data buffer in RAM is at least as large as the new chunk size
            LoadChunkData();
            // write chunk data from RAM persistently to the file
            #if POSIX
            lseek(io.hWrite, ullWritePos, SEEK_SET);
            if (write(io.hWrite, pChunkData, ullNewChunkSize) != ullNewChunkSize) {
                throw Exception("Writing Chunk data (from RAM) failed");
            }
            #elif defined(WIN32)
            LARGE_INTEGER liFilePos;
            liFilePos.QuadPart = ullWritePos;
            SetFilePointerEx(io.hWrite, liFilePos, NULL/*new pos pointer*/, FILE_BEGIN);
            DWORD dwBytesWritten;
            WriteFile(io.hWrite, pChunkData, ullNewChunkSize, &dwBytesWritten, NULL); //FIXME: won't save chunks larger than 2GB !
            if (dwBytesWritten != ullNewChunkSize) {
                throw Exception("Writing Chunk data (from RAM) failed");
            }
            #else
            fseeko(io.hWrite, ullWritePos, SEEK_SET);
            if (fwrite(pChunkData, 1, ullNewChunkSize, io.hWrite) != ullNewChunkSize) {
                throw Exception("Writing Chunk data (from RAM) failed");
            }
            #endif // POSIX
        } else {
            // move chunk data from the end of the file to the appropriate position
            int8_t* pCopyBuffer = new int8_t[4096];
            file_offset_t ullToMove = (ullNewChunkSize < ullCurrentChunkSize) ? ullNewChunkSize : ullCurrentChunkSize;
            #if defined(WIN32)
            DWORD iBytesMoved = 1; // we have to pass it via pointer to the Windows API, thus the correct size must be ensured
            #else
            int iBytesMoved = 1;
            #endif
            for (file_offset_t ullOffset = 0; ullToMove > 0 && iBytesMoved > 0; ullOffset += iBytesMoved, ullToMove -= iBytesMoved) {
                iBytesMoved = (ullToMove < 4096) ? int(ullToMove) : 4096;
                #if POSIX
                lseek(io.hRead, ullStartPos + ullCurrentDataOffset + ullOffset, SEEK_SET);
                iBytesMoved = (int) read(io.hRead, pCopyBuffer, (size_t) iBytesMoved);
                lseek(io.hWrite, ullWritePos + ullOffset, SEEK_SET);
                iBytesMoved = (int) write(io.hWrite, pCopyBuffer, (size_t) iBytesMoved);
                #elif defined(WIN32)
                LARGE_INTEGER liFilePos;
                liFilePos.QuadPart = ullStartPos + ullCurrentDataOffset + ullOffset;
                SetFilePointerEx(io.hRead, liFilePos, NULL/*new pos pointer*/, FILE_BEGIN);
                ReadFile(io.hRead, pCopyBuffer, iBytesMoved, &iBytesMoved, NULL);
                liFilePos.QuadPart = ullWritePos + ullOffset;
                SetFilePointerEx(io.hWrite, liFilePos, NULL/*new pos pointer*/, FILE_BEGIN);
                WriteFile(io.hWrite, pCopyBuffer, iBytesMoved, &iBytesMoved, NULL);
                #else
                fseeko(io.hRead, ullStartPos + ullCurrentDataOffset + ullOffset, SEEK_SET);
                iBytesMoved = fread(pCopyBuffer, 1, iBytesMoved, io.hRead);
                fseeko(io.hWrite, ullWritePos + ullOffset, SEEK_SET);
                iBytesMoved = fwrite(pCopyBuffer, 1, iBytesMoved, io.hWrite);
                #endif
            }
            delete[] pCopyBuffer;
            if (iBytesMoved < 0) throw Exception("Writing Chunk data (from file) failed");
        }

        // update this chunk's header
        ullCurrentChunkSize = ullNewChunkSize;
        WriteHeader(ullOriginalPos);

        if (pProgress)
            __notify_progress(pProgress, 1.0); // notify done

        // update chunk's position pointers
        ullStartPos = ullOriginalPos + CHUNK_HEADER_SIZE(pFile->FileOffsetSize);
        Chunk::__resetPos();

        // add pad byte if needed
        if ((ullStartPos + ullNewChunkSize) % 2 != 0) {
            const char cPadByte = 0;
            #if POSIX
            lseek(io.hWrite, ullStartPos + ullNewChunkSize, SEEK_SET);
            write(io.hWrite, &cPadByte, 1);
            #elif defined(WIN32)
            LARGE_INTEGER liFilePos;
            liFilePos.QuadPart = ullStartPos + ullNewChunkSize;
            SetFilePointerEx(io.hWrite, liFilePos, NULL/*new pos pointer*/, FILE_BEGIN);
            DWORD dwBytesWritten;
            WriteFile(io.hWrite, &cPadByte, 1, &dwBytesWritten, NULL);
            #else
            fseeko(io.hWrite, ullStartPos + ullNewChunkSize, SEEK_SET);
            fwrite(&cPadByte, 1, 1, io.hWrite);
            #endif
            return ullStartPos + ullNewChunkSize + 1;
        }

        return ullStartPos + ullNewChunkSize;
    }

    void Chunk::__resetPos() {
        std::lock_guard<std::mutex> lock(chunkPos.mutex);
        chunkPos.ullPos = 0;
        chunkPos.byThread.clear();
    }



// *************** List ***************
// *

    List::List(File* pFile) : Chunk(pFile) {
        #if DEBUG_RIFF
        std::cout << "List::List(File* pFile)" << std::endl;
        #endif // DEBUG_RIFF
        pSubChunks    = NULL;
        pSubChunksMap = NULL;
    }

    List::List(File* pFile, file_offset_t StartPos, List* Parent)
      : Chunk(pFile, StartPos, Parent) {
        #if DEBUG_RIFF
        std::cout << "List::List(File*,file_offset_t,List*)" << std::endl;
        #endif // DEBUG_RIFF
        pSubChunks    = NULL;
        pSubChunksMap = NULL;
        ReadHeader(StartPos);
        ullStartPos = StartPos + LIST_HEADER_SIZE(pFile->FileOffsetSize);
    }

    List::List(File* pFile, List* pParent, uint32_t uiListID)
      : Chunk(pFile, pParent, CHUNK_ID_LIST, 0) {
        pSubChunks    = NULL;
        pSubChunksMap = NULL;
        ListType      = uiListID;
    }

    List::~List() {
        #if DEBUG_RIFF
        std::cout << "List::~List()" << std::endl;
        #endif // DEBUG_RIFF
        DeleteChunkList();
    }

    void List::DeleteChunkList() {
        if (pSubChunks) {
            ChunkList::iterator iter = pSubChunks->begin();
            ChunkList::iterator end  = pSubChunks->end();
            while (iter != end) {
                delete *iter;
                iter++;
            }
            delete pSubChunks;
            pSubChunks = NULL;
        }
        if (pSubChunksMap) {
            delete pSubChunksMap;
            pSubChunksMap = NULL;
        }
    }

    /**
     *  Returns subchunk at supplied @a pos position within this chunk list.
     *  If supplied @a pos is out of bounds then @c NULL is returned. The
     *  returned subchunk can either by an ordinary data chunk or a list chunk.
     *
     *  @param pos - position of sought subchunk within this list
     *  @returns pointer to the subchunk or NULL if supplied position is not
     *           within the valid range of this list
     */
    Chunk* List::GetSubChunkAt(size_t pos) {
        if (!pSubChunks) LoadSubChunks();
        if (pos >= pSubChunks->size()) return NULL;
        return (*pSubChunks)[pos];
    }

    /**
     *  Returns subchunk with chunk ID <i>\a ChunkID</i> within this chunk
     *  list. Use this method if you expect only one subchunk of that type in
     *  the list. It there are more than one, it's undetermined which one of
     *  them will be returned! If there are no subchunks with that desired
     *  chunk ID, NULL will be returned.
     *
     *  @param ChunkID - chunk ID of the sought subchunk
     *  @returns         pointer to the subchunk or NULL if there is none of
     *                   that ID
     */
    Chunk* List::GetSubChunk(uint32_t ChunkID) {
        #if DEBUG_RIFF
        std::cout << "List::GetSubChunk(uint32_t)" << std::endl;
        #endif // DEBUG_RIFF
        if (!pSubChunksMap) LoadSubChunks();
        return (*pSubChunksMap)[ChunkID];
    }

    /**
     *  Returns sublist chunk with list type <i>\a ListType</i> at supplied
     *  @a pos position among all subchunks of type <i>\a ListType</i> within
     *  this chunk list. If supplied @a pos is out of bounds then @c NULL is
     *  returned.
     *
     *  @param pos - position of sought sublist within this list
     *  @returns pointer to the sublist or NULL if if supplied position is not
     *           within valid range
     */
    List* List::GetSubListAt(size_t pos) {
        if (!pSubChunks) LoadSubChunks();
        if (pos >= pSubChunks->size()) return NULL;
        for (size_t iCk = 0, iLst = 0; iCk < pSubChunks->size(); ++iCk) {
            Chunk* pChunk = (*pSubChunks)[iCk];
            if (pChunk->GetChunkID() != CHUNK_ID_LIST) continue;
            if (iLst == pos) return (List*) pChunk;
            ++iLst;
        }
        return NULL;
    }

    /**
     *  Returns sublist chunk with list type <i>\a ListType</i> within this
     *  chunk list. Use this method if you expect only one sublist chunk of
     *  that type in the list. If there are more than one, it's undetermined
     *  which one of them will be returned! If there are no sublists with
     *  that desired list type, NULL will be returned.
     *
     *  @param ListType - list type of the sought sublist
     *  @returns          pointer to the sublist or NULL if there is none of
     *                    that type
     */
    List* List::GetSubList(uint32_t ListType) {
        #if DEBUG_RIFF
        std::cout << "List::GetSubList(uint32_t)" << std::endl;
        #endif // DEBUG_RIFF
        if (!pSubChunks) LoadSubChunks();
        ChunkList::iterator iter = pSubChunks->begin();
        ChunkList::iterator end  = pSubChunks->end();
        while (iter != end) {
            if ((*iter)->GetChunkID() == CHUNK_ID_LIST) {
                List* l = (List*) *iter;
                if (l->GetListType() == ListType) return l;
            }
            iter++;
        }
        return NULL;
    }

    /**
     *  Returns the first subchunk within the list (which may be an ordinary
     *  chunk as well as a list chunk). You have to call this
     *  method before you can call GetNextSubChunk(). Recall it when you want
     *  to start from the beginning of the list again.
     *
     *  @returns  pointer to the first subchunk within the list, NULL
     *            otherwise
     *  @deprecated  This method is not reentrant-safe, use GetSubChunkAt()
     *               instead.
     */
    Chunk* List::GetFirstSubChunk() {
        #if DEBUG_RIFF
        std::cout << "List::GetFirstSubChunk()" << std::endl;
        #endif // DEBUG_RIFF
        if (!pSubChunks) LoadSubChunks();
        ChunksIterator = pSubChunks->begin();
        return (ChunksIterator != pSubChunks->end()) ? *ChunksIterator : NULL;
    }

    /**
     *  Returns the next subchunk within the list (which may be an ordinary
     *  chunk as well as a list chunk). You have to call
     *  GetFirstSubChunk() before you can use this method!
     *
     *  @returns  pointer to the next subchunk within the list or NULL if
     *            end of list is reached
     *  @deprecated  This method is not reentrant-safe, use GetSubChunkAt()
     *               instead.
     */
    Chunk* List::GetNextSubChunk() {
        #if DEBUG_RIFF
        std::cout << "List::GetNextSubChunk()" << std::endl;
        #endif // DEBUG_RIFF
        if (!pSubChunks) return NULL;
        ChunksIterator++;
        return (ChunksIterator != pSubChunks->end()) ? *ChunksIterator : NULL;
    }

    /**
     *  Returns the first sublist within the list (that is a subchunk with
     *  chunk ID "LIST"). You have to call this method before you can call
     *  GetNextSubList(). Recall it when you want to start from the beginning
     *  of the list again.
     *
     *  @returns  pointer to the first sublist within the list, NULL
     *            otherwise
     *  @deprecated  This method is not reentrant-safe, use GetSubListAt()
     *               instead.
     */
    List* List::GetFirstSubList() {
        #if DEBUG_RIFF
        std::cout << "List::GetFirstSubList()" << std::endl;
        #endif // DEBUG_RIFF
        if (!pSubChunks) LoadSubChunks();
        ListIterator            = pSubChunks->begin();
        ChunkList::iterator end = pSubChunks->end();
        while (ListIterator != end) {
            if ((*ListIterator)->GetChunkID() == CHUNK_ID_LIST) return (List*) *ListIterator;
            ListIterator++;
        }
        return NULL;
    }

    /**
     *  Returns the next sublist (that is a subchunk with chunk ID "LIST")
     *  within the list. You have to call GetFirstSubList() before you can
     *  use this method!
     *
     *  @returns  pointer to the next sublist within the list, NULL if
     *            end of list is reached
     *  @deprecated  This method is not reentrant-safe, use GetSubListAt()
     *               instead.
     */
    List* List::GetNextSubList() {
        #if DEBUG_RIFF
        std::cout << "List::GetNextSubList()" << std::endl;
        #endif // DEBUG_RIFF
        if (!pSubChunks) return NULL;
        if (ListIterator == pSubChunks->end()) return NULL;
        ListIterator++;
        ChunkList::iterator end = pSubChunks->end();
        while (ListIterator != end) {
            if ((*ListIterator)->GetChunkID() == CHUNK_ID_LIST) return (List*) *ListIterator;
            ListIterator++;
        }
        return NULL;
    }

    /**
     *  Returns number of subchunks within the list (including list chunks).
     */
    size_t List::CountSubChunks() {
        if (!pSubChunks) LoadSubChunks();
        return pSubChunks->size();
    }

    /**
     *  Returns number of subchunks within the list with chunk ID
     *  <i>\a ChunkId</i>.
     */
    size_t List::CountSubChunks(uint32_t ChunkID) {
        size_t result = 0;
        if (!pSubChunks) LoadSubChunks();
        ChunkList::iterator iter = pSubChunks->begin();
        ChunkList::iterator end  = pSubChunks->end();
        while (iter != end) {
            if ((*iter)->GetChunkID() == ChunkID) {
                result++;
            }
            iter++;
        }
        return result;
    }

    /**
     *  Returns number of sublists within the list.
     */
    size_t List::CountSubLists() {
        return CountSubChunks(CHUNK_ID_LIST);
    }

    /**
     *  Returns number of sublists within the list with list type
     *  <i>\a ListType</i>
     */
    size_t List::CountSubLists(uint32_t ListType) {
        size_t result = 0;
        if (!pSubChunks) LoadSubChunks();
        ChunkList::iterator iter = pSubChunks->begin();
        ChunkList::iterator end  = pSubChunks->end();
        while (iter != end) {
            if ((*iter)->GetChunkID() == CHUNK_ID_LIST) {
                List* l = (List*) *iter;
                if (l->GetListType() == ListType) result++;
            }
            iter++;
        }
        return result;
    }

    /** @brief Creates a new sub chunk.
     *
     * Creates and adds a new sub chunk to this list chunk. Note that the
     * chunk's body size given by \a ullBodySize must be greater than zero.
     * You have to call File::Save() to make this change persistent to the
     * actual file and <b>before</b> performing any data write operations
     * on the new chunk!
     *
     * @param uiChunkID  - chunk ID of the new chunk
     * @param ullBodySize - size of the new chunk's body, that is its actual
     *                      data size (without header)
     * @throws RIFF::Exception if \a ullBodySize equals zero
     */
    Chunk* List::AddSubChunk(uint32_t uiChunkID, file_offset_t ullBodySize) {
        if (ullBodySize == 0) throw Exception("Chunk body size must be at least 1 byte");
        if (!pSubChunks) LoadSubChunks();
        Chunk* pNewChunk = new Chunk(pFile, this, uiChunkID, 0);
        pSubChunks->push_back(pNewChunk);
        (*pSubChunksMap)[uiChunkID] = pNewChunk;
        pNewChunk->Resize(ullBodySize);
        ullNewChunkSize += CHUNK_HEADER_SIZE(pFile->FileOffsetSize);
        return pNewChunk;
    }

    /** @brief Moves a sub chunk witin this list.
     *
     * Moves a sub chunk from one position in this list to another
     * position in the same list. The pSrc chunk is placed before the
     * pDst chunk.
     *
     * @param pSrc - sub chunk to be moved
     * @param pDst - the position to move to. pSrc will be placed
     *               before pDst. If pDst is 0, pSrc will be placed
     *               last in list.
     */
    void List::MoveSubChunk(Chunk* pSrc, Chunk* pDst) {
        if (!pSubChunks) LoadSubChunks();
        for (size_t i = 0; i < pSubChunks->size(); ++i) {
            if ((*pSubChunks)[i] == pSrc) {
                pSubChunks->erase(pSubChunks->begin() + i);
                ChunkList::iterator iter =
                    find(pSubChunks->begin(), pSubChunks->end(), pDst);
                pSubChunks->insert(iter, pSrc);
                return;
            }
        }
    }

    /** @brief Moves a sub chunk from this list to another list.
     *
     * Moves a sub chunk from this list list to the end of another
     * list.
     *
     * @param pSrc - sub chunk to be moved
     * @param pDst - destination list where the chunk shall be moved to
     */
    void List::MoveSubChunk(Chunk* pSrc, List* pNewParent) {
        if (pNewParent == this || !pNewParent) return;
        if (!pSubChunks) LoadSubChunks();
        if (!pNewParent->pSubChunks) pNewParent->LoadSubChunks();
        ChunkList::iterator iter =
            find(pSubChunks->begin(), pSubChunks->end(), pSrc);
        if (iter == pSubChunks->end()) return;
        pSubChunks->erase(iter);
        pNewParent->pSubChunks->push_back(pSrc);
        // update chunk id map of this List
        if ((*pSubChunksMap)[pSrc->GetChunkID()] == pSrc) {
            pSubChunksMap->erase(pSrc->GetChunkID());
            // try to find another chunk of the same chunk ID
            ChunkList::iterator iter = pSubChunks->begin();
            ChunkList::iterator end  = pSubChunks->end();
            for (; iter != end; ++iter) {
                if ((*iter)->GetChunkID() == pSrc->GetChunkID()) {
                    (*pSubChunksMap)[pSrc->GetChunkID()] = *iter;
                    break; // we're done, stop search
                }
            }
        }
        // update chunk id map of other list
        if (!(*pNewParent->pSubChunksMap)[pSrc->GetChunkID()])
            (*pNewParent->pSubChunksMap)[pSrc->GetChunkID()] = pSrc;
    }

    /** @brief Creates a new list sub chunk.
     *
     * Creates and adds a new list sub chunk to this list chunk. Note that
     * you have to add sub chunks / sub list chunks to the new created chunk
     * <b>before</b> trying to make this change persisten to the actual
     * file with File::Save()!
     *
     * @param uiListType - list ID of the new list chunk
     */
    List* List::AddSubList(uint32_t uiListType) {
        if (!pSubChunks) LoadSubChunks();
        List* pNewListChunk = new List(pFile, this, uiListType);
        pSubChunks->push_back(pNewListChunk);
        (*pSubChunksMap)[CHUNK_ID_LIST] = pNewListChunk;
        ullNewChunkSize += LIST_HEADER_SIZE(pFile->FileOffsetSize);
        return pNewListChunk;
    }

    /** @brief Removes a sub chunk.
     *
     * Removes the sub chunk given by \a pSubChunk from this list and frees
     * it completely from RAM. The given chunk can either be a normal sub
     * chunk or a list sub chunk. In case the given chunk is a list chunk,
     * all its subchunks (if any) will be removed recursively as well. You
     * should call File::Save() to make this change persistent at any time.
     *
     * @param pSubChunk - sub chunk or sub list chunk to be removed
     */
    void List::DeleteSubChunk(Chunk* pSubChunk) {
        if (!pSubChunks) LoadSubChunks();
        ChunkList::iterator iter =
            find(pSubChunks->begin(), pSubChunks->end(), pSubChunk);
        if (iter == pSubChunks->end()) return;
        pSubChunks->erase(iter);
        if ((*pSubChunksMap)[pSubChunk->GetChunkID()] == pSubChunk) {
            pSubChunksMap->erase(pSubChunk->GetChunkID());
            // try to find another chunk of the same chunk ID
            ChunkList::iterator iter = pSubChunks->begin();
            ChunkList::iterator end  = pSubChunks->end();
            for (; iter != end; ++iter) {
                if ((*iter)->GetChunkID() == pSubChunk->GetChunkID()) {
                    (*pSubChunksMap)[pSubChunk->GetChunkID()] = *iter;
                    break; // we're done, stop search
                }
            }
        }
        delete pSubChunk;
    }

    /**
     *  Returns the actual total size in bytes (including List chunk header and
     *  all subchunks) of this List Chunk if being stored to a file.
     *
     *  @param fileOffsetSize - RIFF file offset size (in bytes) assumed when 
     *                          being saved to a file
     */
    file_offset_t List::RequiredPhysicalSize(int fileOffsetSize) {
        if (!pSubChunks) LoadSubChunks();
        file_offset_t size = LIST_HEADER_SIZE(fileOffsetSize);
        ChunkList::iterator iter = pSubChunks->begin();
        ChunkList::iterator end  = pSubChunks->end();
        for (; iter != end; ++iter)
            size += (*iter)->RequiredPhysicalSize(fileOffsetSize);
        return size;
    }

    void List::ReadHeader(file_offset_t filePos) {
        #if DEBUG_RIFF
        std::cout << "List::Readheader(file_offset_t) ";
        #endif // DEBUG_RIFF
        Chunk::ReadHeader(filePos);
        if (ullCurrentChunkSize < 4) return;
        ullNewChunkSize = ullCurrentChunkSize -= 4;

        const File::Handle hRead = pFile->FileHandle();

        #if POSIX
        lseek(hRead, filePos + CHUNK_HEADER_SIZE(pFile->FileOffsetSize), SEEK_SET);
        read(hRead, &ListType, 4);
        #elif defined(WIN32)
        LARGE_INTEGER liFilePos;
        liFilePos.QuadPart = filePos + CHUNK_HEADER_SIZE(pFile->FileOffsetSize);
        SetFilePointerEx(hRead, liFilePos, NULL/*new pos pointer*/, FILE_BEGIN);
        DWORD dwBytesRead;
        ReadFile(hRead, &ListType, 4, &dwBytesRead, NULL);
        #else
        fseeko(hRead, filePos + CHUNK_HEADER_SIZE(pFile->FileOffsetSize), SEEK_SET);
        fread(&ListType, 4, 1, hRead);
        #endif // POSIX
        #if DEBUG_RIFF
        std::cout << "listType=" << convertToString(ListType) << std::endl;
        #endif // DEBUG_RIFF
        if (!pFile->bEndianNative) {
            //swapBytes_32(&ListType);
        }
    }

    void List::WriteHeader(file_offset_t filePos) {
        // the four list type bytes officially belong the chunk's body in the RIFF format
        ullNewChunkSize += 4;
        Chunk::WriteHeader(filePos);
        ullNewChunkSize -= 4; // just revert the +4 incrementation

        const File::Handle hWrite = pFile->FileWriteHandle();

        #if POSIX
        lseek(hWrite, filePos + CHUNK_HEADER_SIZE(pFile->FileOffsetSize), SEEK_SET);
        write(hWrite, &ListType, 4);
        #elif defined(WIN32)
        LARGE_INTEGER liFilePos;
        liFilePos.QuadPart = filePos + CHUNK_HEADER_SIZE(pFile->FileOffsetSize);
        SetFilePointerEx(hWrite, liFilePos, NULL/*new pos pointer*/, FILE_BEGIN);
        DWORD dwBytesWritten;
        WriteFile(hWrite, &ListType, 4, &dwBytesWritten, NULL);
        #else
        fseeko(hWrite, filePos + CHUNK_HEADER_SIZE(pFile->FileOffsetSize), SEEK_SET);
        fwrite(&ListType, 4, 1, hWrite);
        #endif // POSIX
    }

    void List::LoadSubChunks(progress_t* pProgress) {
        #if DEBUG_RIFF
        std::cout << "List::LoadSubChunks()";
        #endif // DEBUG_RIFF
        if (!pSubChunks) {
            pSubChunks    = new ChunkList();
            pSubChunksMap = new ChunkMap();

            const File::Handle hRead = pFile->FileHandle();
            if (!_isValidHandle(hRead)) return;

            const file_offset_t ullOriginalPos = GetPos();
            SetPos(0); // jump to beginning of list chunk body
            while (RemainingBytes() >= CHUNK_HEADER_SIZE(pFile->FileOffsetSize)) {
                Chunk* ck;
                uint32_t ckid;
                // return value check is required here to prevent a potential
                // garbage data use of 'ckid' below in case Read() failed
                if (Read(&ckid, 4, 1) != 4)
                    throw Exception("LoadSubChunks(): Failed reading RIFF chunk ID");
                #if DEBUG_RIFF
                std::cout << " ckid=" << convertToString(ckid) << std::endl;
                #endif // DEBUG_RIFF
                const file_offset_t pos = GetPos();
                if (ckid == CHUNK_ID_LIST) {
                    ck = new RIFF::List(pFile, ullStartPos + pos - 4, this);
                    SetPos(ck->GetSize() + LIST_HEADER_SIZE(pFile->FileOffsetSize) - 4, RIFF::stream_curpos);
                }
                else { // simple chunk
                    ck = new RIFF::Chunk(pFile, ullStartPos + pos - 4, this);
                    SetPos(ck->GetSize() + CHUNK_HEADER_SIZE(pFile->FileOffsetSize) - 4, RIFF::stream_curpos);
                }
                pSubChunks->push_back(ck);
                (*pSubChunksMap)[ckid] = ck;
                if (GetPos() % 2 != 0) SetPos(1, RIFF::stream_curpos); // jump over pad byte
            }
            SetPos(ullOriginalPos); // restore position before this call
        }
        if (pProgress)
            __notify_progress(pProgress, 1.0); // notify done
    }

    void List::LoadSubChunksRecursively(progress_t* pProgress) {
        const int n = (int) CountSubLists();
        int i = 0;
        for (List* pList = GetSubListAt(i); pList; pList = GetSubListAt(++i)) {
            if (pProgress) {
                // divide local progress into subprogress
                progress_t subprogress;
                __divide_progress(pProgress, &subprogress, n, i);
                // do the actual work
                pList->LoadSubChunksRecursively(&subprogress);
            } else
                pList->LoadSubChunksRecursively(NULL);
        }
        if (pProgress)
            __notify_progress(pProgress, 1.0); // notify done
    }

    /** @brief Write list chunk persistently e.g. to disk.
     *
     * Stores the list chunk persistently to its actual "physical" file. All
     * subchunks (including sub list chunks) will be stored recursively as
     * well.
     *
     * @param ullWritePos - position within the "physical" file where this
     *                     list chunk should be written to
     * @param ullCurrentDataOffset - offset of current (old) data within
     *                              the file
     * @param pProgress - optional: callback function for progress notification
     * @returns new write position in the "physical" file, that is
     *          \a ullWritePos incremented by this list chunk's new size
     *          (including its header size of course)
     */
    file_offset_t List::WriteChunk(file_offset_t ullWritePos, file_offset_t ullCurrentDataOffset, progress_t* pProgress) {
        const file_offset_t ullOriginalPos = ullWritePos;
        ullWritePos += LIST_HEADER_SIZE(pFile->FileOffsetSize);

        if (pFile->GetMode() != stream_mode_read_write)
            throw Exception("Cannot write list chunk, file has to be opened in read+write mode");

        // write all subchunks (including sub list chunks) recursively
        if (pSubChunks) {
            size_t i = 0;
            const size_t n = pSubChunks->size();
            for (ChunkList::iterator iter = pSubChunks->begin(), end = pSubChunks->end(); iter != end; ++iter, ++i) {
                if (pProgress) {
                    // divide local progress into subprogress for loading current Instrument
                    progress_t subprogress;
                    __divide_progress(pProgress, &subprogress, n, i);
                    // do the actual work
                    ullWritePos = (*iter)->WriteChunk(ullWritePos, ullCurrentDataOffset, &subprogress);
                } else
                    ullWritePos = (*iter)->WriteChunk(ullWritePos, ullCurrentDataOffset, NULL);
            }
        }

        // update this list chunk's header
        ullCurrentChunkSize = ullNewChunkSize = ullWritePos - ullOriginalPos - LIST_HEADER_SIZE(pFile->FileOffsetSize);
        WriteHeader(ullOriginalPos);

        // offset of this list chunk in new written file may have changed
        ullStartPos = ullOriginalPos + LIST_HEADER_SIZE(pFile->FileOffsetSize);

        if (pProgress)
            __notify_progress(pProgress, 1.0); // notify done

        return ullWritePos;
    }

    void List::__resetPos() {
        Chunk::__resetPos();
        if (pSubChunks) {
            for (ChunkList::iterator iter = pSubChunks->begin(), end = pSubChunks->end(); iter != end; ++iter) {
                (*iter)->__resetPos();
            }
        }
    }

    /**
     *  Returns string representation of the lists's id
     */
    String List::GetListTypeString() const {
        return convertToString(ListType);
    }



// *************** File ***************
// *

    /** @brief Create new RIFF file.
     *
     * Use this constructor if you want to create a new RIFF file completely
     * "from scratch". Note: there must be no empty chunks or empty list
     * chunks when trying to make the new RIFF file persistent with Save()!
     *
     * Note: by default, the RIFF file will be saved in native endian
     * format; that is, as a RIFF file on little-endian machines and
     * as a RIFX file on big-endian. To change this behaviour, call
     * SetByteOrder() before calling Save().
     *
     * @param FileType - four-byte identifier of the RIFF file type
     * @see AddSubChunk(), AddSubList(), SetByteOrder()
     */
    File::File(uint32_t FileType)
        : List(this), bIsNewFile(true), Layout(layout_standard),
          FileOffsetPreference(offset_size_auto)
    {
        io.isPerThread = false;
        #if defined(WIN32)
        io.hRead = io.hWrite = INVALID_HANDLE_VALUE;
        #else
        io.hRead = io.hWrite = 0;
        #endif
        io.Mode = stream_mode_closed;
        bEndianNative = true;
        ListType = FileType;
        FileOffsetSize = 4;
        ullStartPos = RIFF_HEADER_SIZE(FileOffsetSize);
    }

    /** @brief Load existing RIFF file.
     *
     * Loads an existing RIFF file with all its chunks.
     *
     * @param path - path and file name of the RIFF file to open
     * @throws RIFF::Exception if error occurred while trying to load the
     *                         given RIFF file
     */
    File::File(const String& path)
        : List(this), Filename(path), bIsNewFile(false), Layout(layout_standard),
          FileOffsetPreference(offset_size_auto)
    {
        #if DEBUG_RIFF
        std::cout << "File::File("<<path<<")" << std::endl;
        #endif // DEBUG_RIFF
        bEndianNative = true;
        FileOffsetSize = 4;
        try {
            __openExistingFile(path);
            if (ChunkID != CHUNK_ID_RIFF && ChunkID != CHUNK_ID_RIFX) {
                throw RIFF::Exception("Not a RIFF file");
            }
        }
        catch (...) {
            Cleanup();
            throw;
        }
    }

    /** @brief Load existing RIFF-like file.
     *
     * Loads an existing file, which is not a "real" RIFF file, but similar to
     * an ordinary RIFF file.
     *
     * A "real" RIFF file contains at top level a List chunk either with chunk
     * ID "RIFF" or "RIFX". The simple constructor above expects this to be
     * case, and if it finds the toplevel List chunk to have another chunk ID
     * than one of those two expected ones, it would throw an Exception and
     * would refuse to load the file accordingly.
     *
     * Since there are however a lot of file formats which use the same simple
     * principles of the RIFF format, with another toplevel List chunk ID
     * though, you can use this alternative constructor here to be able to load
     * and handle those files in the same way as you would do with "real" RIFF
     * files.
     *
     * @param path - path and file name of the RIFF-alike file to be opened
     * @param FileType - expected toplevel List chunk ID (this is the very
     *                   first chunk found in the file)
     * @param Endian - whether the file uses little endian or big endian layout
     * @param layout - general file structure type
     * @param fileOffsetSize - (optional) preference how to deal with large files
     * @throws RIFF::Exception if error occurred while trying to load the
     *                         given RIFF-alike file
     */
    File::File(const String& path, uint32_t FileType, endian_t Endian, layout_t layout, offset_size_t fileOffsetSize)
        : List(this), Filename(path), bIsNewFile(false), Layout(layout),
          FileOffsetPreference(fileOffsetSize)
    {
        SetByteOrder(Endian);
        if (fileOffsetSize < offset_size_auto || fileOffsetSize > offset_size_64bit)
            throw Exception("Invalid RIFF::offset_size_t");
        FileOffsetSize = 4;
        try {
            __openExistingFile(path, &FileType);
        }
        catch (...) {
            Cleanup();
            throw;
        }
    }

    /**
     * Opens an already existing RIFF file or RIFF-alike file. This method
     * shall only be called once (in a File class constructor).
     *
     * @param path - path and file name of the RIFF file or RIFF-alike file to
     *               be opened
     * @param FileType - (optional) expected chunk ID of first chunk in file
     * @throws RIFF::Exception if error occurred while trying to load the
     *                         given RIFF file or RIFF-alike file
     */
    void File::__openExistingFile(const String& path, uint32_t* FileType) {
        io.isPerThread = false;
        #if POSIX
        io.hRead = io.hWrite = open(path.c_str(), O_RDONLY | O_NONBLOCK);
        if (io.hRead == -1) {
            io.hRead = io.hWrite = 0;
            String sError = strerror(errno);
            throw RIFF::Exception("Can't open \"" + path + "\": " + sError);
        }
        #elif defined(WIN32)
        io.hRead = io.hWrite = CreateFile(
                                     path.c_str(), GENERIC_READ,
                                     FILE_SHARE_READ | FILE_SHARE_WRITE,
                                     NULL, OPEN_EXISTING,
                                     FILE_ATTRIBUTE_NORMAL |
                                     FILE_FLAG_RANDOM_ACCESS, NULL
                                 );
        if (io.hRead == INVALID_HANDLE_VALUE) {
            io.hRead = io.hWrite = INVALID_HANDLE_VALUE;
            throw RIFF::Exception("Can't open \"" + path + "\"");
        }
        #else
        io.hRead = io.hWrite = fopen(path.c_str(), "rb");
        if (!io.hRead) throw RIFF::Exception("Can't open \"" + path + "\"");
        #endif // POSIX
        io.Mode = stream_mode_read;

        // determine RIFF file offset size to be used (in RIFF chunk headers)
        // according to the current file offset preference
        FileOffsetSize = FileOffsetSizeFor(GetCurrentFileSize());

        switch (Layout) {
            case layout_standard: // this is a normal RIFF file
                ullStartPos = RIFF_HEADER_SIZE(FileOffsetSize);
                ReadHeader(0);
                if (FileType && ChunkID != *FileType)
                    throw RIFF::Exception("Invalid file container ID");
                break;
            case layout_flat: // non-standard RIFF-alike file
                ullStartPos = 0;
                ullNewChunkSize = ullCurrentChunkSize = GetCurrentFileSize();
                if (FileType) {
                    uint32_t ckid;
                    if (Read(&ckid, 4, 1) != 4) {
                        throw RIFF::Exception("Invalid file header ID (premature end of header)");
                    } else if (ckid != *FileType) {
                        String s = " (expected '" + convertToString(*FileType) + "' but got '" + convertToString(ckid) + "')";
                        throw RIFF::Exception("Invalid file header ID" + s);
                    }
                    SetPos(0); // reset to first byte of file
                }
                LoadSubChunks();
                break;
        }
    }

    String File::GetFileName() const {
        return Filename;
    }
    
    void File::SetFileName(const String& path) {
        Filename = path;
    }

    /**
     * This is an internal-only method which must not be used by any application
     * and might change at any time.
     *
     * Resolves and returns a reference (memory location) of the RIFF file's
     * internal and OS dependent file I/O handles which are intended to be used
     * by the calling thread.
     */
    File::HandlePair& File::FileHandlePairUnsafeRef() {
        if (io.byThread.empty()) return io;
        const std::thread::id tid = std::this_thread::get_id();
        const auto it = io.byThread.find(tid);
        return (it != io.byThread.end()) ?
            it->second :
            io.byThread[tid] = {
                // designated initializers require C++20, so commented for now
                #if defined(WIN32)
                /* .hRead  = */ INVALID_HANDLE_VALUE,
                /* .hWrite = */ INVALID_HANDLE_VALUE,
                #else
                /* .hRead  = */ 0,
                /* .hWrite = */ 0,
                #endif
                /* .Mode = */ stream_mode_closed
            };
    }

    /**
     * Returns the OS dependent file I/O read and write handles intended to be
     * used by the calling thread.
     *
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    File::HandlePair File::FileHandlePair() const {
        std::lock_guard<std::mutex> lock(io.mutex);
        if (io.byThread.empty()) return io;
        const std::thread::id tid = std::this_thread::get_id();
        const auto it = io.byThread.find(tid);
        return (it != io.byThread.end()) ?
            it->second :
            io.byThread[tid] = {
                // designated initializers require C++20, so commented for now
                #if defined(WIN32)
                /* .hRead  = */ INVALID_HANDLE_VALUE,
                /* .hWrite = */ INVALID_HANDLE_VALUE,
                #else
                /* .hRead  = */ 0,
                /* .hWrite = */ 0,
                #endif
                /* .Mode = */ stream_mode_closed
            };
     }

    /**
     * Returns the OS dependent file I/O read handle intended to be used by the
     * calling thread.
     *
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    File::Handle File::FileHandle() const {
        return FileHandlePair().hRead;
    }

    /**
     * Returns the OS dependent file I/O write handle intended to be used by the
     * calling thread.
     *
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    File::Handle File::FileWriteHandle() const {
        return FileHandlePair().hWrite;
    }

    /**
     * Returns the file I/O mode currently being available for the calling
     * thread for this RIFF file (either ro, rw or closed).
     *
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    stream_mode_t File::GetMode() const {
        return FileHandlePair().Mode;
    }

    layout_t File::GetLayout() const {
        return Layout;
    }

    /** @brief Change file access mode.
     *
     * Changes files access mode either to read-only mode or to read/write
     * mode.
     *
     * @param NewMode - new file access mode
     * @returns true if mode was changed, false if current mode already
     *          equals new mode
     * @throws RIFF::Exception if file could not be opened in requested file
     *         access mode or if passed access mode is unknown
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    bool File::SetMode(stream_mode_t NewMode) {
        bool bResetPos = false;
        bool res = SetModeInternal(NewMode, &bResetPos);
        // resetting position must be handled outside above's call to avoid any
        // potential dead lock, as SetModeInternal() acquires a lock and
        // __resetPos() acquires a lock by itself (not a theoretical issue!)
        if (bResetPos)
            __resetPos(); // reset read/write position of ALL 'Chunk' objects
        return res;
    }

    bool File::SetModeInternal(stream_mode_t NewMode, bool* pResetPos) {
        std::lock_guard<std::mutex> lock(io.mutex);
        HandlePair& io = FileHandlePairUnsafeRef();
        if (NewMode != io.Mode) {
            switch (NewMode) {
                case stream_mode_read:
                    if (_isValidHandle(io.hRead)) _close(io.hRead);
                    #if POSIX
                    io.hRead = io.hWrite = open(Filename.c_str(), O_RDONLY | O_NONBLOCK);
                    if (io.hRead == -1) {
                        io.hRead = io.hWrite = 0;
                        String sError = strerror(errno);
                        throw Exception("Could not (re)open file \"" + Filename + "\" in read mode: " + sError);
                    }
                    #elif defined(WIN32)
                    io.hRead = io.hWrite = CreateFile(
                                                 Filename.c_str(), GENERIC_READ,
                                                 FILE_SHARE_READ | FILE_SHARE_WRITE,
                                                 NULL, OPEN_EXISTING,
                                                 FILE_ATTRIBUTE_NORMAL |
                                                 FILE_FLAG_RANDOM_ACCESS,
                                                 NULL
                                             );
                    if (io.hRead == INVALID_HANDLE_VALUE) {
                        io.hRead = io.hWrite = INVALID_HANDLE_VALUE;
                        throw Exception("Could not (re)open file \"" + Filename + "\" in read mode");
                    }
                    #else
                    io.hRead = io.hWrite = fopen(Filename.c_str(), "rb");
                    if (!io.hRead) throw Exception("Could not (re)open file \"" + Filename + "\" in read mode");
                    #endif
                    *pResetPos = true;
                    break;
                case stream_mode_read_write:
                    if (_isValidHandle(io.hRead)) _close(io.hRead);
                    #if POSIX
                    io.hRead = io.hWrite = open(Filename.c_str(), O_RDWR | O_NONBLOCK);
                    if (io.hRead == -1) {
                        io.hRead = io.hWrite = open(Filename.c_str(), O_RDONLY | O_NONBLOCK);
                        String sError = strerror(errno);
                        throw Exception("Could not open file \"" + Filename + "\" in read+write mode: " + sError);
                    }
                    #elif defined(WIN32)
                    io.hRead = io.hWrite = CreateFile(
                                                 Filename.c_str(),
                                                 GENERIC_READ | GENERIC_WRITE,
                                                 FILE_SHARE_READ,
                                                 NULL, OPEN_ALWAYS,
                                                 FILE_ATTRIBUTE_NORMAL |
                                                 FILE_FLAG_RANDOM_ACCESS,
                                                 NULL
                                             );
                    if (io.hRead == INVALID_HANDLE_VALUE) {
                        io.hRead = io.hWrite = CreateFile(
                                                     Filename.c_str(), GENERIC_READ,
                                                     FILE_SHARE_READ | FILE_SHARE_WRITE,
                                                     NULL, OPEN_EXISTING,
                                                     FILE_ATTRIBUTE_NORMAL |
                                                     FILE_FLAG_RANDOM_ACCESS,
                                                     NULL
                                                 );
                        throw Exception("Could not (re)open file \"" + Filename + "\" in read+write mode");
                    }
                    #else
                    io.hRead = io.hWrite = fopen(Filename.c_str(), "r+b");
                    if (!io.hRead) {
                        io.hRead = io.hWrite = fopen(Filename.c_str(), "rb");
                        throw Exception("Could not open file \"" + Filename + "\" in read+write mode");
                    }
                    #endif
                    *pResetPos = true;
                    break;
                case stream_mode_closed:
                    if (_isValidHandle(io.hRead)) _close(io.hRead);
                    if (_isValidHandle(io.hWrite)) _close(io.hWrite);
                    #if POSIX
                    io.hRead = io.hWrite = 0;
                    #elif defined(WIN32)
                    io.hRead = io.hWrite = INVALID_HANDLE_VALUE;
                    #else
                    io.hRead = io.hWrite = NULL;
                    #endif
                    break;
                default:
                    throw Exception("Unknown file access mode");
            }
            io.Mode = NewMode;
            return true;
        }
        return false;
    }

    /** @brief Set the byte order to be used when saving.
     *
     * Set the byte order to be used in the file. A value of
     * endian_little will create a RIFF file, endian_big a RIFX file
     * and endian_native will create a RIFF file on little-endian
     * machines and RIFX on big-endian machines.
     *
     * @param Endian - endianess to use when file is saved.
     */
    void File::SetByteOrder(endian_t Endian) {
        #if WORDS_BIGENDIAN
        bEndianNative = Endian != endian_little;
        #else
        bEndianNative = Endian != endian_big;
        #endif
    }

    /** @brief Save changes to same file.
     *
     * Make all changes of all chunks persistent by writing them to the
     * actual (same) file.
     *
     * @param pProgress - optional: callback function for progress notification
     * @throws RIFF::Exception if there is an empty chunk or empty list
     *                         chunk or any kind of IO error occurred
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    void File::Save(progress_t* pProgress) {
        //TODO: implementation for the case where first chunk is not a global container (List chunk) is not implemented yet (i.e. Korg files)
        if (Layout == layout_flat)
            throw Exception("Saving a RIFF file with layout_flat is not implemented yet");

        // make sure the RIFF tree is built (from the original file)
        if (pProgress) {
            // divide progress into subprogress
            progress_t subprogress;
            __divide_progress(pProgress, &subprogress, 3.f, 0.f); // arbitrarily subdivided into 1/3 of total progress
            // do the actual work
            LoadSubChunksRecursively(&subprogress);
            // notify subprogress done
            __notify_progress(&subprogress, 1.f);
        } else
            LoadSubChunksRecursively(NULL);

        // reopen file in write mode
        SetMode(stream_mode_read_write);

        // get the current file size as it is now still physically stored on disk
        const file_offset_t workingFileSize = GetCurrentFileSize();

        // get the overall file size required to save this file
        const file_offset_t newFileSize = GetRequiredFileSize(FileOffsetPreference);

        // determine whether this file will yield in a large file (>=4GB) and
        // the RIFF file offset size to be used accordingly for all chunks
        FileOffsetSize = FileOffsetSizeFor(newFileSize);

        const HandlePair io = FileHandlePair();
        const Handle hRead  = io.hRead;
        const Handle hWrite = io.hWrite;

        // to be able to save the whole file without loading everything into
        // RAM and without having to store the data in a temporary file, we
        // enlarge the file with the overall positive file size change,
        // then move current data towards the end of the file by the calculated
        // positive file size difference and finally update / rewrite the file
        // by copying the old data back to the right position at the beginning
        // of the file

        // if there are positive size changes...
        file_offset_t positiveSizeDiff = 0;
        if (newFileSize > workingFileSize) {
            positiveSizeDiff = newFileSize - workingFileSize;

            // divide progress into subprogress
            progress_t subprogress;
            if (pProgress)
                __divide_progress(pProgress, &subprogress, 3.f, 1.f); // arbitrarily subdivided into 1/3 of total progress

            // ... we enlarge this file first ...
            ResizeFile(newFileSize);

            // ... and move current data by the same amount towards end of file.
            int8_t* pCopyBuffer = new int8_t[4096];
            #if defined(WIN32)
            DWORD iBytesMoved = 1; // we have to pass it via pointer to the Windows API, thus the correct size must be ensured
            #else
            ssize_t iBytesMoved = 1;
            #endif
            for (file_offset_t ullPos = workingFileSize, iNotif = 0; iBytesMoved > 0; ++iNotif) {
                iBytesMoved = (ullPos < 4096) ? ullPos : 4096;
                ullPos -= iBytesMoved;
                #if POSIX
                lseek(hRead, ullPos, SEEK_SET);
                iBytesMoved = read(hRead, pCopyBuffer, iBytesMoved);
                lseek(hWrite, ullPos + positiveSizeDiff, SEEK_SET);
                iBytesMoved = write(hWrite, pCopyBuffer, iBytesMoved);
                #elif defined(WIN32)
                LARGE_INTEGER liFilePos;
                liFilePos.QuadPart = ullPos;
                SetFilePointerEx(hRead, liFilePos, NULL/*new pos pointer*/, FILE_BEGIN);
                ReadFile(hRead, pCopyBuffer, iBytesMoved, &iBytesMoved, NULL);
                liFilePos.QuadPart = ullPos + positiveSizeDiff;
                SetFilePointerEx(hWrite, liFilePos, NULL/*new pos pointer*/, FILE_BEGIN);
                WriteFile(hWrite, pCopyBuffer, iBytesMoved, &iBytesMoved, NULL);
                #else
                fseeko(hRead, ullPos, SEEK_SET);
                iBytesMoved = fread(pCopyBuffer, 1, iBytesMoved, hRead);
                fseeko(hWrite, ullPos + positiveSizeDiff, SEEK_SET);
                iBytesMoved = fwrite(pCopyBuffer, 1, iBytesMoved, hWrite);
                #endif
                if (pProgress && !(iNotif % 8) && iBytesMoved > 0)
                    __notify_progress(&subprogress, float(workingFileSize - ullPos) / float(workingFileSize));
            }
            delete[] pCopyBuffer;
            if (iBytesMoved < 0) throw Exception("Could not modify file while trying to enlarge it");

            if (pProgress)
                __notify_progress(&subprogress, 1.f); // notify subprogress done
        }

        // rebuild / rewrite complete RIFF tree ...

        // divide progress into subprogress
        progress_t subprogress;
        if (pProgress)
            __divide_progress(pProgress, &subprogress, 3.f, 2.f); // arbitrarily subdivided into 1/3 of total progress
        // do the actual work
        const file_offset_t finalSize = WriteChunk(0, positiveSizeDiff, pProgress ? &subprogress : NULL);
        const file_offset_t finalActualSize = __GetFileSize(hWrite);
        // notify subprogress done
        if (pProgress)
            __notify_progress(&subprogress, 1.f);

        // resize file to the final size
        if (finalSize < finalActualSize) ResizeFile(finalSize);

        if (pProgress)
            __notify_progress(pProgress, 1.0); // notify done
    }

    /** @brief Save changes to another file.
     *
     * Make all changes of all chunks persistent by writing them to another
     * file. <b>Caution:</b> this method is optimized for writing to
     * <b>another</b> file, do not use it to save the changes to the same
     * file! Use File::Save() in that case instead! Ignoring this might
     * result in a corrupted file, especially in case chunks were resized!
     *
     * After calling this method, this File object will be associated with
     * the new file (given by \a path) afterwards.
     *
     * @param path - path and file name where everything should be written to
     * @param pProgress - optional: callback function for progress notification
     * @see File::IsIOPerThread() for multi-threaded streaming
     */
    void File::Save(const String& path, progress_t* pProgress) {
        //TODO: we should make a check here if somebody tries to write to the same file and automatically call the other Save() method in that case

        //TODO: implementation for the case where first chunk is not a global container (List chunk) is not implemented yet (i.e. Korg files)
        if (Layout == layout_flat)
            throw Exception("Saving a RIFF file with layout_flat is not implemented yet");

        // make sure the RIFF tree is built (from the original file)
        if (pProgress) {
            // divide progress into subprogress
            progress_t subprogress;
            __divide_progress(pProgress, &subprogress, 2.f, 0.f); // arbitrarily subdivided into 1/2 of total progress
            // do the actual work
            LoadSubChunksRecursively(&subprogress);
            // notify subprogress done
            __notify_progress(&subprogress, 1.f);
        } else
            LoadSubChunksRecursively(NULL);

        if (!bIsNewFile) SetMode(stream_mode_read);

        {
            std::lock_guard<std::mutex> lock(io.mutex);
            HandlePair& io = FileHandlePairUnsafeRef();

            // open the other (new) file for writing and truncate it to zero size
            #if POSIX
            io.hWrite = open(path.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);
            if (io.hWrite == -1) {
                io.hWrite = io.hRead;
                String sError = strerror(errno);
                throw Exception("Could not open file \"" + path + "\" for writing: " + sError);
            }
            #elif defined(WIN32)
            io.hWrite = CreateFile(
                path.c_str(), GENERIC_WRITE, FILE_SHARE_READ,
                NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL |
                FILE_FLAG_RANDOM_ACCESS, NULL
            );
            if (io.hWrite == INVALID_HANDLE_VALUE) {
                io.hWrite = io.hRead;
                throw Exception("Could not open file \"" + path + "\" for writing");
            }
            #else
            io.hWrite = fopen(path.c_str(), "w+b");
            if (!io.hWrite) {
                io.hWrite = io.hRead;
                throw Exception("Could not open file \"" + path + "\" for writing");
            }
            #endif // POSIX
            io.Mode = stream_mode_read_write;
        }

        // get the overall file size required to save this file
        const file_offset_t newFileSize = GetRequiredFileSize(FileOffsetPreference);

        // determine whether this file will yield in a large file (>=4GB) and
        // the RIFF file offset size to be used accordingly for all chunks
        FileOffsetSize = FileOffsetSizeFor(newFileSize);

        // write complete RIFF tree to the other (new) file
        file_offset_t ullTotalSize;
        if (pProgress) {
            // divide progress into subprogress
            progress_t subprogress;
            __divide_progress(pProgress, &subprogress, 2.f, 1.f); // arbitrarily subdivided into 1/2 of total progress
            // do the actual work
            ullTotalSize = WriteChunk(0, 0, &subprogress);
            // notify subprogress done
            __notify_progress(&subprogress, 1.f);
        } else
            ullTotalSize = WriteChunk(0, 0, NULL);

        const file_offset_t ullActualSize = __GetFileSize(FileWriteHandle());

        // resize file to the final size (if the file was originally larger)
        if (ullActualSize > ullTotalSize) ResizeFile(ullTotalSize);

        {
            std::lock_guard<std::mutex> lock(io.mutex);
            HandlePair& io = FileHandlePairUnsafeRef();

            if (_isValidHandle(io.hWrite)) _close(io.hWrite);
            io.hWrite = io.hRead;

            // associate new file with this File object from now on
            Filename = path;
            bIsNewFile = false;
            io.Mode = (stream_mode_t) -1; // Just set it to an undefined mode ...
        }
        SetMode(stream_mode_read_write); // ... so SetMode() has to reopen the file handles.

        if (pProgress)
            __notify_progress(pProgress, 1.0); // notify done
    }

    void File::ResizeFile(file_offset_t ullNewSize) {
        const Handle hWrite = FileWriteHandle();
        #if POSIX
        if (ftruncate(hWrite, ullNewSize) < 0)
            throw Exception("Could not resize file \"" + Filename + "\"");
        #elif defined(WIN32)
        LARGE_INTEGER liFilePos;
        liFilePos.QuadPart = ullNewSize;
        if (
            !SetFilePointerEx(hWrite, liFilePos, NULL/*new pos pointer*/, FILE_BEGIN) ||
            !SetEndOfFile(hWrite)
        ) throw Exception("Could not resize file \"" + Filename + "\"");
        #else
        # error Sorry, this version of libgig only supports POSIX and Windows systems yet.
        # error Reason: portable implementation of RIFF::File::ResizeFile() is missing (yet)!
        #endif
    }

    File::~File() {
        #if DEBUG_RIFF
        std::cout << "File::~File()" << std::endl;
        #endif // DEBUG_RIFF
        Cleanup();
    }

    /**
     * Returns @c true if this file has been created new from scratch and
     * has not been stored to disk yet.
     */
    bool File::IsNew() const {
        return bIsNewFile;
    }

    void File::Cleanup() {
        if (IsIOPerThread()) {
            for (auto it = io.byThread.begin(); it != io.byThread.end(); ++it) {
                _close(it->second.hRead);
            }
        } else {
            _close(io.hRead);
        }
        DeleteChunkList();
        pFile = NULL;
    }

    /**
     * Returns the current size of this file (in bytes) as it is currently
     * yet stored on disk. If this file does not yet exist on disk (i.e. when
     * this RIFF File has just been created from scratch and Save() has not
     * been called yet) then this method returns 0.
     */
    file_offset_t File::GetCurrentFileSize() const {
        file_offset_t size = 0;
        const Handle hRead = FileHandle();
        try {
            size = __GetFileSize(hRead);
        } catch (...) {
            size = 0;
        }
        return size;
    }

    /**
     * Returns the required size (in bytes) for this RIFF File to be saved to
     * disk. The precise size of the final file on disk depends on the RIFF
     * file offset size actually used internally in all headers of the RIFF
     * chunks. By default libgig handles the required file offset size
     * automatically for you; that means it is using 32 bit offsets for files
     * smaller than 4 GB and 64 bit offsets for files equal or larger than
     * 4 GB. You may however also override this default behavior by passing the
     * respective option to the RIFF File constructor to force one particular
     * offset size. In the latter case this method will return the file size
     * for the requested forced file offset size that will be used when calling
     * Save() later on.
     *
     * You may also use the overridden method below to get the file size for
     * an arbitrary other file offset size instead.
     *
     * @see offset_size_t
     * @see GetFileOffsetSize()
     */
    file_offset_t File::GetRequiredFileSize() {
        return GetRequiredFileSize(FileOffsetPreference);
    }

    /**
     * Returns the rquired size (in bytes) for this RIFF file to be saved to
     * disk, assuming the passed @a fileOffsestSize would be used for the
     * Save() operation.
     *
     * This overridden method essentialy behaves like the above method, with
     * the difference that you must provide a specific RIFF @a fileOffsetSize
     * for calculating the theoretical final file size.
     *
     * @see GetFileOffsetSize()
     */
    file_offset_t File::GetRequiredFileSize(offset_size_t fileOffsetSize) {
        switch (fileOffsetSize) {
            case offset_size_auto: {
                file_offset_t fileSize = GetRequiredFileSize(offset_size_32bit);
                if (fileSize >> 32)
                    return GetRequiredFileSize(offset_size_64bit);
                else
                    return fileSize;
            }
            case offset_size_32bit: break;
            case offset_size_64bit: break;
            default: throw Exception("Internal error: Invalid RIFF::offset_size_t");
        }
        return RequiredPhysicalSize(FileOffsetSize);
    }

    int File::FileOffsetSizeFor(file_offset_t fileSize) const {
        switch (FileOffsetPreference) {
            case offset_size_auto:
                return (fileSize >> 32) ? 8 : 4; 
            case offset_size_32bit:
                return 4;
            case offset_size_64bit:
                return 8;
            default:
                throw Exception("Internal error: Invalid RIFF::offset_size_t");
        }
    }

    /**
     * Returns the current size (in bytes) of file offsets stored in the
     * headers of all chunks of this file.
     *
     * Most RIFF files are using 32 bit file offsets internally, which limits
     * them to a maximum file size of less than 4 GB though. In contrast to the
     * common standard, this RIFF File class implementation supports handling of
     * RIFF files equal or larger than 4 GB. In such cases 64 bit file offsets
     * have to be used in all headers of all RIFF Chunks when being stored to a
     * physical file. libgig by default automatically selects the correct file
     * offset size for you. You may however also force one particular file
     * offset size by supplying the respective option to the RIFF::File
     * constructor.
     *
     * This method can be used to check which RIFF file offset size is currently
     * being used for this RIFF File.
     *
     * @returns current RIFF file offset size used (in bytes)
     * @see offset_size_t
     */
    int File::GetFileOffsetSize() const {
        return FileOffsetSize;
    }

    /**
     * Returns the required size (in bytes) of file offsets stored in the
     * headers of all chunks of this file if the current RIFF tree would be
     * saved to disk by calling Save().
     *
     * See GetFileOffsetSize() for mor details about RIFF file offsets.
     *
     * @returns RIFF file offset size required (in bytes) if being saved
     * @see offset_size_t
     */
    int File::GetRequiredFileOffsetSize() {
        return FileOffsetSizeFor(GetCurrentFileSize());
    }

    /** @brief Whether file streams are independent for each thread.
     *
     * All file I/O operations like reading from a RIFF chunk body (e.g. by
     * calling Chunk::Read(), Chunk::ReadInt8()), writing to a RIFF chunk body
     * (e.g. by calling Chunk::Write(), Chunk::WriteInt8()) or saving the
     * current RIFF tree structure to some file (e.g. by calling Save())
     * operate on a file I/O stream state, i.e. there is a "current" file
     * read/write position and reading/writing by a certain amount of bytes
     * automatically advances that "current" file position.
     *
     * By default there is only one stream state for a RIFF::File object, which
     * is not an issue as long as only one thread is using the RIFF::File
     * object at a time (which might also be the case in a collaborative /
     * coroutine multi-threaded scenario).
     *
     * If however a RIFF::File object is read/written @b simultaniously by
     * multiple threads this can lead to undefined behaviour as the individual
     * threads would concurrently alter the file stream position. For such a
     * concurrent multithreaded file I/O scenario @c SetIOPerThread(true) might
     * be enabled which causes each thread to automatically use its own file
     * stream state.
     *
     * @returns true if each thread has its own file stream state
     *          (default: false)
     * @see SetIOPerThread()
     */
    bool File::IsIOPerThread() const {
        //NOTE: Not caring about atomicity here at all, for three reasons:
        // 1. SetIOPerThread() is assumed to be called only once for the entire
        //    life time of a RIFF::File, usually very early at its lifetime, and
        //    hence a change to isPerThread should already safely be propagated
        //    before any other thread would actually read this boolean flag.
        // 2. This method is called very frequently, and therefore any
        //    synchronization techique would hurt runtime efficiency.
        // 3. Using even a mutex lock here might easily cause a deadlock due to
        //    other locks been taken in this .cpp file, i.e. at a higher call
        //    stack level (and this is the main reason why I removed it here).
        return io.isPerThread;
    }

    /** @brief Enable/disable file streams being independent for each thread.
     *
     * By enabling this feature (default off) each thread will automatically use
     * its own file I/O stream state for allowing simultanious multi-threaded
     * file read/write operations.
     *
     * @b NOTE: After having enabled this feature, the individual threads must
     * at least once check GetState() and if their file I/O stream is yet closed
     * they must call SetMode() (i.e. once) respectively to open their own file
     * handles before being able to use any of the Read() or Write() methods.
     *
     * @param enable - @c true: one independent stream state per thread,
     *                 @c false: only one stream in total shared by @b all threads
     * @see IsIOPerThread() for more details about this feature
     */
    void File::SetIOPerThread(bool enable) {
        std::lock_guard<std::mutex> lock(io.mutex);
        if (!io.byThread.empty() == enable) return;
        io.isPerThread = enable;
        if (enable) {
            const std::thread::id tid = std::this_thread::get_id();
            io.byThread[tid] = io;
        } else {
            // retain an arbitrary handle pair, close all other handle pairs
            for (auto it = io.byThread.begin(); it != io.byThread.end(); ++it) {
                if (it == io.byThread.begin()) {
                    io.hRead  = it->second.hRead;
                    io.hWrite = it->second.hWrite;
                } else {
                    _close(it->second.hRead);
                    _close(it->second.hWrite);
                }
            }
            io.byThread.clear();
        }
    }

    #if POSIX
    file_offset_t File::__GetFileSize(int hFile) const {
        struct stat filestat;
        if (fstat(hFile, &filestat) == -1)
            throw Exception("POSIX FS error: could not determine file size");
        return filestat.st_size;
    }
    #elif defined(WIN32)
    file_offset_t File::__GetFileSize(HANDLE hFile) const {
        LARGE_INTEGER size;
        if (!GetFileSizeEx(hFile, &size))
            throw Exception("Windows FS error: could not determine file size");
        return size.QuadPart;
    }
    #else // standard C functions
    file_offset_t File::__GetFileSize(FILE* hFile) const {
        off_t curpos = ftello(hFile);
        if (fseeko(hFile, 0, SEEK_END) == -1)
            throw Exception("FS error: could not determine file size");
        off_t size = ftello(hFile);
        fseeko(hFile, curpos, SEEK_SET);
        return size;
    }
    #endif


// *************** Exception ***************
// *

    Exception::Exception() {
    }

    Exception::Exception(String format, ...) {
        va_list arg;
        va_start(arg, format);
        Message = assemble(format, arg);
        va_end(arg);
    }

    Exception::Exception(String format, va_list arg) {
        Message = assemble(format, arg);
    }

    void Exception::PrintMessage() {
        std::cout << "RIFF::Exception: " << Message << std::endl;
    }

    String Exception::assemble(String format, va_list arg) {
        char* buf = NULL;
        vasprintf(&buf, format.c_str(), arg);
        String s = buf;
        free(buf);
        return s;
    }


// *************** functions ***************
// *

    /**
     * Returns the name of this C++ library. This is usually "libgig" of
     * course. This call is equivalent to DLS::libraryName() and
     * gig::libraryName().
     */
    String libraryName() {
        return PACKAGE;
    }

    /**
     * Returns version of this C++ library. This call is equivalent to
     * DLS::libraryVersion() and gig::libraryVersion().
     */
    String libraryVersion() {
        return VERSION;
    }

} // namespace RIFF
