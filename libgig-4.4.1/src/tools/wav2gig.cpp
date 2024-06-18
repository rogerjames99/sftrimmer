/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2021 Christian Schoenebeck                              *
 *                      <cuse@users.sourceforge.net>                       *
 *                                                                         *
 *   This program is part of libgig.                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston,                 *
 *   MA  02111-1307  USA                                                   *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <set>
#include <map>
#include <regex>

#if !defined(WIN32)
# include <unistd.h>
#endif

#include "../gig.h"
#include "../helper.h" // for ToString()

// only libsndfile is available for Windows, so we use that for writing the sound files
#ifdef WIN32
# define HAVE_SNDFILE 1
#endif // WIN32

// abort compilation here if libsndfile is not available
#if !HAVE_SNDFILE
# error "It seems as if libsndfile is not available!"
# error "(HAVE_SNDFILE is false)"
#endif

#if HAVE_SNDFILE
# ifdef LIBSNDFILE_HEADER_FILE
#  include LIBSNDFILE_HEADER_FILE(sndfile.h)
# else
#  include <sndfile.h>
# endif
#endif // HAVE_SNDFILE

#ifdef WIN32
# define DIR_SEPARATOR '\\'
#else
# define DIR_SEPARATOR '/'
#endif

using namespace std;

static string Revision() {
    string s = "$Revision: 3994 $";
    return s.substr(11, s.size() - 13); // cut dollar signs, spaces and CVS macro keyword
}

static void printVersion() {
    cout << "wav2gig revision " << Revision() << endl;
    cout << "using " << gig::libraryName() << " " << gig::libraryVersion() << endl;
}

static void printUsage() {
    cout << "wav2gig - Create GigaStudio file from a set of WAV files." << endl;
    cout << endl;
    cout << "Usage: wav2gig [OPTIONS] GIGFILE WAVFILEORDIR1 [ WAVFILEORDIR2 ... ]" << endl;
    cout << endl;
    cout << "  -v  Print version and exit." << endl;
    cout << endl;
    cout << "  -f  Overwrite output gig file if it already exists." << endl;
    cout << endl;
    cout << "  -r  Recurse through all subdirs of provided input WAV dirs." << endl;
    cout << endl;
    cout << "  --dry-run" << endl;
    cout << endl;
    cout << "      Scan input sample (.wav) files, but exit before creating any .gig file." << endl;
    cout << endl;
    cout << "  --verbose" << endl;
    cout << endl;
    cout << "      Increase amount of info being shown." << endl;
    cout << endl;
    cout << "  --regex-name1 PATTERN" << endl;
    cout << endl;
    cout << "      Regular expression for overriding the NAME1 part of the input sample file name scheme." << endl;
    cout << endl;
    cout << "  --regex-name2 PATTERN" << endl;
    cout << endl;
    cout << "      Regular expression for overriding the NAME2 part of the input sample file name scheme." << endl;
    cout << endl;
    cout << "  --regex-velocity-nr PATTERN" << endl;
    cout << endl;
    cout << "      Regular expression for overriding the VELOCITY_NR part of the input sample file name scheme." << endl;
    cout << endl;
    cout << "  --regex-note-nr PATTERN" << endl;
    cout << endl;
    cout << "      Regular expression for overriding the NOTE_NR part of the input sample file name scheme." << endl;
    cout << endl;
    cout << "  --regex-note-name PATTERN" << endl;
    cout << endl;
    cout << "      Regular expression for overriding the NOTE_NAME part of the input sample file name scheme." << endl;
    cout << endl;
    cout << "Read 'man wav2gig' for detailed help." << endl;
    cout << endl;
}

static bool beginsWith(const string& haystack, const string& needle) {
    return haystack.substr(0, needle.size()) == needle;
}

static bool endsWith(const string& haystack, const string& needle) {
    return haystack.substr(haystack.size() - needle.size(), needle.size()) == needle;
}

static string tokenByRegExGroup(const string& haystack, const string& pattern,
                                size_t group = 1)
{
    regex rx(pattern);
    smatch m;
    regex_search(haystack, m, rx);
    return (m.size() <= group) ? (string) "" : (string) m[group];
}

static bool fileExists(const string& filename) {
    FILE* hFile = fopen(filename.c_str(), "r");
    if (!hFile) return false;
    fclose(hFile);
    return true;
}

static bool isDir(const string& dirname) {
    struct stat sb;
    return (stat(dirname.c_str(), &sb) == 0) && S_ISDIR(sb.st_mode);
}

static bool isRegularFile(const string& filename) {
    struct stat sb;
    return (stat(filename.c_str(), &sb) == 0) && S_ISREG(sb.st_mode);
}

// this could also be replaced by fopen(name, "w") to simply truncate the file to zero
static void deleteFile(const string& filename) {
    #if defined(WIN32)
    DeleteFile(filename.c_str());
    #else
    unlink(filename.c_str());
    #endif
}

static bool isGigFileName(const string& filename) {
    return endsWith(filename, ".gig") || endsWith(filename, ".GIG");
}

static bool isWavFileName(const string& filename) {
    return endsWith(filename, ".wav") || endsWith(filename, ".WAV");
}

static bool isValidWavFile(const string& filename) {
    SF_INFO info;
    info.format = 0;
    SNDFILE* hFile = sf_open(filename.c_str(), SFM_READ, &info);
    if (!hFile) {
        cerr << "Could not open input wav file \"" << filename << "\"" << endl;
        return false;
    }
    sf_close(hFile);
    switch (info.format & 0xff) {
        case SF_FORMAT_PCM_S8:
        case SF_FORMAT_PCM_16:
        case SF_FORMAT_PCM_U8:
        case SF_FORMAT_PCM_24:
        case SF_FORMAT_PCM_32:
        case SF_FORMAT_FLOAT:
        case SF_FORMAT_DOUBLE:
            return true;
        default:
            cerr << "Format of input wav file \"" << filename << "\" not supported!" << endl;
            return false;
    }
    return false;
}

struct FilenameRegExPatterns {
    string name1;
    string name2;
    string velocityNr;
    string noteNr;
    string noteName;
};

static void collectWavFilesOfDir(set<string>& result, string path, bool bRecurse, bool* pbError = NULL) {
    DIR* d = opendir(path.c_str());
    if (!d) {
        if (pbError) *pbError = true;
        cerr << strerror(errno) << " : '" << path << "'" << endl;
        return;
    }

    for (struct dirent* e = readdir(d); e; e = readdir(d)) {
        if (string(e->d_name) == "." || string(e->d_name) == "..")
            continue;

        const string fullName = path + DIR_SEPARATOR + e->d_name;

        struct stat s;
        if (stat(fullName.c_str(), &s)) {
            if (pbError) *pbError = true;
            cerr << strerror(errno) << " : '" << fullName << "'" << endl;
            continue;
        }

        if (S_ISREG(s.st_mode) && isWavFileName(fullName) && isValidWavFile(fullName)) {
            result.insert(fullName);
        } else if (S_ISDIR(s.st_mode) && bRecurse) {
            collectWavFilesOfDir(result, fullName, bRecurse, pbError);
        }
    }

    closedir(d);
}

static void collectWavFiles(set<string>& result, string path, bool bRecurse, bool* pbError = NULL) {
    struct stat s;
    if (stat(path.c_str(), &s)) {
        if (pbError) *pbError = true;
        cerr << strerror(errno) << " : '" << path << "'" << endl;
        return;
    }
    if (S_ISREG(s.st_mode) && isWavFileName(path) && isValidWavFile(path)) {
        result.insert(path);
    } else if (S_ISDIR(s.st_mode)) {
        collectWavFilesOfDir(result, path, bRecurse, pbError);
    } else {
        if (pbError) *pbError = true;
        cerr << "Neither a regular (.wav) file nor directory : '" << path << "'" << endl;
    }
}

struct WavInfo {
    string fileName;
    int note;
    int velocity;
    SF_INFO sfinfo;
    string noteName;
    string name1;
    string name2;
    SF_INSTRUMENT sfinst;
    bool hasSfInst;

    bool isStereo() const { return sfinfo.channels == 2; }

    string outputSampleName() const {
        return name1 + "_" + noteName + "_" + ToString(velocity);
    }

    void assertValid() const {
        if (note < 0 || note > 127) {
            cerr << "ERROR: note number " << note << " of \"" << fileName << "\" is invalid!" << endl;
            exit(EXIT_FAILURE);
        }
        if (velocity < 0 || velocity > 127) {
            cerr << "ERROR: velocity number " << velocity << " of \"" << fileName << "\" is invalid!" << endl;
            exit(EXIT_FAILURE);
        }
    }
};

class WavRegion : public map<int,WavInfo> {
public:
    typedef map<int,WavInfo> base_t;

//     WavRegion () :
//         map<int,WavInfo>()
//     {
//     }
// 
//     WavRegion (const WavRegion& x) :
//         map<int,WavInfo>(x)
//     {
//     }
// 
//     WavRegion& operator= (const WavRegion& x) {
//         base_t::operator=(x);
//         return *this;
//     }
    
    bool isStereo() const {
        for (const auto& it : *this)
            if (it.second.isStereo())
                return true;
        return false;
    }
};

typedef map<int,WavRegion> WavInstrument;

static WavInfo getWavInfo(string filename,
                          const FilenameRegExPatterns& patterns)
{
    WavInfo wav;
    wav.fileName = filename;
    wav.sfinfo = {};
    {
        SNDFILE* hFile = sf_open(filename.c_str(), SFM_READ, &wav.sfinfo);
        if (!hFile) {
            cerr << "Could not open input wav file \"" << filename << "\"" << endl;
            exit(EXIT_FAILURE);
        }
        wav.hasSfInst = (sf_command(hFile, SFC_GET_INSTRUMENT,
                                    &wav.sfinst, sizeof(wav.sfinst)) != SF_FALSE);
        sf_close(hFile);
        switch (wav.sfinfo.channels) {
            case 1:
            case 2:
                break;
            default:
                cerr << int(wav.sfinfo.channels) << " audio channels in WAV file \"" << filename << "\"; this is not supported!" << endl;
                exit(EXIT_FAILURE);
        }
    }
    {
        wav.name1 = tokenByRegExGroup(filename, patterns.name1);
        if (wav.name1.empty()) {
            cerr << "Unexpected file name format \"" << filename
                 << "\" for 'name1' RegEx pattern \"" << patterns.name1
                 << "\" !" << endl;
            exit(EXIT_FAILURE);
        }
        wav.name2 = tokenByRegExGroup(filename, patterns.name2);
        if (wav.name2.empty()) {
            cerr << "Unexpected file name format \"" << filename
                 << "\" for 'name2' RegEx pattern \"" << patterns.name2
                 << "\" !" << endl;
            exit(EXIT_FAILURE);
        }
        string sVelocity = tokenByRegExGroup(filename, patterns.velocityNr);
        if (sVelocity.empty()) {
            cerr << "Unexpected file name format \"" << filename
                 << "\" for 'velocity-nr' RegEx pattern \"" << patterns.velocityNr
                 << "\" !" << endl;
            exit(EXIT_FAILURE);
        }
        wav.velocity = atoi(sVelocity.c_str());
        string sNoteNr = tokenByRegExGroup(filename, patterns.noteNr);
        if (sNoteNr.empty()) {
            cerr << "Unexpected file name format \"" << filename
                 << "\" for 'note-nr' RegEx pattern \"" << patterns.noteNr
                 << "\" !" << endl;
            exit(EXIT_FAILURE);
        }
        wav.note = atoi(sNoteNr.c_str());
        wav.noteName = tokenByRegExGroup(filename, patterns.noteName);
        if (wav.noteName.empty()) {
            cerr << "Unexpected file name format \"" << filename
                 << "\" for 'note-name' RegEx pattern \"" << patterns.noteName
                 << "\" !" << endl;
            exit(EXIT_FAILURE);
        }
    }
    return wav;
}

inline int getDimensionIndex(gig::Region* region, gig::dimension_t type) {
    for (int d = 0; d < region->Dimensions; ++d)
        if (region->pDimensionDefinitions[d].dimension == type)
            return d;
    return -1;
}

static gig::Sample* createSample(gig::File* gig, WavInfo* wav, bool bVerbose) {
    gig::Sample* s = gig->AddSample();

    s->pInfo->Name      = wav->outputSampleName();
    s->Channels         = wav->sfinfo.channels;
    s->SamplesPerSecond = wav->sfinfo.samplerate;

    if (bVerbose) {
        cout << "Add Sample [" << gig->CountSamples() << "] '"
             << s->pInfo->Name << "' to gig file:" << endl;
    }

    switch (wav->sfinfo.format & 0xff) {
        case SF_FORMAT_PCM_S8:
        case SF_FORMAT_PCM_16:
        case SF_FORMAT_PCM_U8:
            s->BitDepth = 16;
            break;
        case SF_FORMAT_PCM_24:
        case SF_FORMAT_PCM_32:
        case SF_FORMAT_FLOAT:
        case SF_FORMAT_DOUBLE:
            s->BitDepth = 24;
            break;
        default:
            throw gig::Exception("format not supported");
    }

    s->FrameSize = s->Channels * s->BitDepth / 8;

    if (bVerbose) {
        cout << "\t" << s->BitDepth << " Bits, "
             << s->SamplesPerSecond << " Hz, "
             << s->Channels << " Channels"
             << " [Source: .WAV File Content]" << endl;
    }

    if (wav->hasSfInst) {
        s->MIDIUnityNote = wav->sfinst.basenote;
        s->FineTune      = wav->sfinst.detune;
        if (bVerbose) {
            cout << "\tRoot Note " << s->MIDIUnityNote
                 << " [Source: .WAV File Content]" << endl;
            cout << "\tFine Tune " << s->FineTune
                 << " [Source: .WAV File Content]" << endl;
        }
        if (wav->sfinst.loop_count && wav->sfinst.loops[0].mode != SF_LOOP_NONE) {
            s->Loops = 1;
            if (bVerbose) cout << "\t";
            switch (wav->sfinst.loops[0].mode) {
                case SF_LOOP_FORWARD:
                    s->LoopType = gig::loop_type_normal;
                    if (bVerbose) cout << "Normal ";
                    break;
                case SF_LOOP_BACKWARD:
                    s->LoopType = gig::loop_type_backward;
                    if (bVerbose) cout << "Backward ";
                    break;
                case SF_LOOP_ALTERNATING:
                    s->LoopType = gig::loop_type_bidirectional;
                    if (bVerbose) cout << "Pingpong ";
                    break;
            }
            s->LoopStart = wav->sfinst.loops[0].start;
            s->LoopEnd = wav->sfinst.loops[0].end;
            s->LoopPlayCount = wav->sfinst.loops[0].count;
            s->LoopSize = s->LoopEnd - s->LoopStart + 1;
            if (bVerbose) {
                cout << "Loop " << s->LoopPlayCount << " times from "
                     << s->LoopStart << " to " << s->LoopEnd
                     << " (Size " << s->LoopSize << ")" << endl;
            }
        }
    } else {
        s->MIDIUnityNote = wav->note;
        cout << "\tRoot Note " << s->MIDIUnityNote << " [Source: .WAV Filename Schema]" << endl;
    }
    cout << "\tVelocity " << wav->velocity
         << " [Source: .WAV Filename Schema]" << endl;
    cout << "\tRegion " << wav->note
         << " [Source: .WAV Filename Schema]" << endl;

    // schedule for resize (will be performed when gig->Save() is called)
    s->Resize(wav->sfinfo.frames);

    return s;
}

int main(int argc, char *argv[]) {
    bool bForce = false;
    bool bRecursive = false;
    bool bDryRun = false;
    bool bVerbose = false;
    // using C++11 raw string literals for the RegEx patterns here to avoid
    // having to double escape special characters inside the RegEx patterns
    FilenameRegExPatterns patterns = {
        // name 1 (e.g. "BSTEIN18")
        .name1 = R"RegEx(([^-\/\\]+) - [^-]+ - [^-]+ - [^-]+ - [^.]+)RegEx",
        // name 2 (e.g. "noname")
        .name2 = R"RegEx([^-\/\\]+ - ([^-]+) - [^-]+ - [^-]+ - [^.]+)RegEx",
        // velocity value (e.g. "18")
        .velocityNr = R"RegEx([^-\/\\]+ - [^-]+ - ([^-]+) - [^-]+ - [^.]+)RegEx",
        // note number (e.g. "021")
        .noteNr = R"RegEx([^-\/\\]+ - [^-]+ - [^-]+ - ([^-]+) - [^.]+)RegEx",
        // note name (e.g. "a-1")
        .noteName = R"RegEx([^-\/\\]+ - [^-]+ - [^-]+ - [^-]+ - ([^.]+))RegEx",
    };

    // validate & parse arguments provided to this program
    int iArg;
    for (iArg = 1; iArg < argc; ++iArg) {
        const string opt = argv[iArg];
        const string nextOpt = (iArg + 1 < argc) ? argv[iArg + 1] : "";
        if (opt == "--") { // common for all command line tools: separator between initial option arguments and subsequent file arguments
            iArg++;
            break;
        }
        if (opt.substr(0, 1) != "-") break;

        if (opt == "-v") {
            printVersion();
            return EXIT_SUCCESS;
        } else if (opt == "-f") {
            bForce = true;
        } else if (opt == "-r") {
            bRecursive = true;
        } else if (opt == "--dry-run") {
            bDryRun = true;
        } else if (opt == "--verbose") {
            bVerbose = true;
        } else if (opt == "--regex-name1") {
            if (nextOpt.empty() || beginsWith(nextOpt, "-")) {
                cerr << "Missing argument for option '" << opt << "'" << endl;
                return EXIT_FAILURE;
            }
            patterns.name1 = nextOpt;
            ++iArg;
        } else if (opt == "--regex-name2") {
            if (nextOpt.empty() || beginsWith(nextOpt, "-")) {
                cerr << "Missing argument for option '" << opt << "'" << endl;
                return EXIT_FAILURE;
            }
            patterns.name2 = nextOpt;
            ++iArg;
        } else if (opt == "--regex-velocity-nr") {
            if (nextOpt.empty() || beginsWith(nextOpt, "-")) {
                cerr << "Missing argument for option '" << opt << "'" << endl;
                return EXIT_FAILURE;
            }
            patterns.velocityNr = nextOpt;
            ++iArg;
        } else if (opt == "--regex-note-nr") {
            if (nextOpt.empty() || beginsWith(nextOpt, "-")) {
                cerr << "Missing argument for option '" << opt << "'" << endl;
                return EXIT_FAILURE;
            }
            patterns.noteNr = nextOpt;
            ++iArg;
        } else if (opt == "--regex-note-name") {
            if (nextOpt.empty() || beginsWith(nextOpt, "-")) {
                cerr << "Missing argument for option '" << opt << "'" << endl;
                return EXIT_FAILURE;
            }
            patterns.noteName = nextOpt;
            ++iArg;
        } else {
            cerr << "Unknown option '" << opt << "'" << endl;
            cerr << endl;
            printUsage();
            return EXIT_FAILURE;
        }
    }
    if (argc < 3) {
        printUsage();
        return EXIT_FAILURE;
    }

    set<string> inNames; // may be file names and/or dir names
    string outFileName;

    // all options have been processed, all subsequent args should be file/dir arguments
    for (int i = 0; iArg < argc; ++iArg, ++i) {
        if (i == 0) {
            outFileName = argv[iArg];
        } else {
            inNames.insert(argv[iArg]);
        }
    }
    if (outFileName.empty()) {
        cerr << "You must provide one output file (.gig format)!" << endl;
        return EXIT_FAILURE;
    }
    if (inNames.empty()) {
        cerr << "You must provide at least one input WAV file or directory!" << endl;
        return EXIT_FAILURE;
    }
    if (!isGigFileName(outFileName)) {
        cerr << "Provided output file name should end with \".gig\"!" << endl;
        return EXIT_FAILURE;
    }

    // now collect the actual list of input WAV files
    set<string> wavFileNames;
    cout << "Scanning for input WAV files ... " << flush;
    for (set<string>::const_iterator it = inNames.begin();
         it != inNames.end(); ++it)
    {
        bool error = false;
        collectWavFiles(wavFileNames, *it, bRecursive, &error);
        if (error) return EXIT_FAILURE;
    }
    if (wavFileNames.empty()) {
        cerr << "No input WAV file provided (or found)!" << endl;
        return EXIT_FAILURE;
    }
    cout << "(" << int(wavFileNames.size()) << " found).\n";

    // check if output file already exists
    if (fileExists(outFileName) && !bDryRun) {
        if (bForce) deleteFile(outFileName);
        else {
            cerr << "Output file '" << outFileName << "' already exists. Use -f to overwrite it." << endl;
            return EXIT_FAILURE;
        }
    }

    // order all input wav files into regions and velocity splits
    WavInstrument wavInstrument;
    cout << "Preprocessing input WAV files by their names ... " << flush;
    for (set<string>::const_iterator it = wavFileNames.begin();
         it != wavFileNames.end(); ++it)
    {
        WavInfo wavInfo = getWavInfo(*it, patterns);
        wavInfo.assertValid(); // make sure collected informations are OK
        if (wavInstrument[wavInfo.note].count(wavInfo.velocity)) {
            cerr << "Velocity conflict between file '" << wavInfo.fileName
                 << "' and file '" << wavInstrument[wavInfo.note][wavInfo.velocity].fileName << "'!" << endl;
            return EXIT_FAILURE;
        }
        wavInstrument[wavInfo.note][wavInfo.velocity] = wavInfo;
    }
    if (wavInstrument.empty()) {
        cerr << "After sorting the WAV files around, there is no single WAV left to create a GIG file with!" << endl;
        return EXIT_FAILURE;
    }
    cout << "OK\n";

    // create and assemble a new .gig file as output
    try {
        cout << "Creating new gig file and one new gig instrument ... " << flush;

        // start with an empty .gig file
        gig::File gig;

        gig::Instrument* instr = gig.AddInstrument();
        instr->pInfo->Name = "Unnamed by wav2gig";

        cout << "OK\n";
        
        map<gig::Sample*,WavInfo> queuedSamples;

        cout << "Assembling new gig instrument with interpreted multi sample structure ... " << flush;
        for (auto& itWavRgn : wavInstrument) {
            const int note = itWavRgn.first;
            WavRegion& wavRgn = itWavRgn.second;

            gig::Region* gigRegion = instr->AddRegion();
            gigRegion->SetKeyRange(note/*low*/, note/*high*/);

            if (wavRgn.isStereo()) {
                gig::dimension_def_t dim;
                dim.dimension = gig::dimension_samplechannel;
                dim.bits  = 1; // 2^(1) = 2
                dim.zones = 2; // stereo = 2 audio channels = 2 split zones
                gigRegion->AddDimension(&dim);
            }
            
            if (wavRgn.size() > 1) {
                gig::dimension_def_t dim;
                dim.dimension = gig::dimension_velocity;
                dim.zones = wavRgn.size();
                // Find the number of bits required to hold the
                // specified amount of zones.
                int zoneBits = dim.zones - 1;
                for (dim.bits = 0; zoneBits > 1; dim.bits += 2, zoneBits >>= 2);
                dim.bits += zoneBits;
                gigRegion->AddDimension(&dim);
            }

            const int iStereoDimensionIndex = getDimensionIndex(gigRegion, gig::dimension_samplechannel);
            const int iVelocityDimensionIndex = getDimensionIndex(gigRegion, gig::dimension_velocity);

            int iVelocitySplitZone = 0;
            for (auto& itWav : wavRgn) {
                const int velocity = itWav.first;
                WavInfo& wav = itWav.second;
                gig::Sample* gigSample = createSample(&gig, &wav, bVerbose);
                queuedSamples[gigSample] = wav;

                uint8_t iDimBits[8] = {};

                for (int iAudioChannel = 0; iAudioChannel < (wavRgn.isStereo() ? 2 : 1); ++iAudioChannel) {

                    // if region has velocity splits, select the respective velocity split zone
                    if (wavRgn.size() > 1) {
                        if (iVelocityDimensionIndex < 0)
                            throw gig::Exception("Could not resolve velocity dimension index");
                        iDimBits[iVelocityDimensionIndex] = iVelocitySplitZone;
                    }

                    // select dimension bit for this stereo dimension split
                    if (iAudioChannel > 0) {
                        if (iStereoDimensionIndex < 0)
                            throw gig::Exception("Could not resolve stereo dimension index");
                        iDimBits[iStereoDimensionIndex] = 1;
                    }

                    gig::DimensionRegion* dimRgn = gigRegion->GetDimensionRegionByBit(iDimBits);
                    if (!dimRgn)
                        throw gig::Exception("Internal error: Could not resolve Dimension Region");

                    // if this is a velocity split, apply the precise velocity split range values
                    if (wavRgn.size() > 1) {
                        dimRgn->VelocityUpperLimit = velocity; // gig v2
                        dimRgn->DimensionUpperLimits[iVelocityDimensionIndex] = velocity; // gig v3 and above
                    }

                    dimRgn->pSample = gigSample;
                    if (gigSample) {
                        dimRgn->UnityNote = gigSample->MIDIUnityNote;
                        if (gigSample->Loops) {
                            DLS::sample_loop_t loop;
                            loop.Size = sizeof(loop);
                            loop.LoopType   = gigSample->LoopType;
                            loop.LoopStart  = gigSample->LoopStart;
                            loop.LoopLength = gigSample->LoopEnd - gigSample->LoopStart;
                            dimRgn->AddSampleLoop(&loop);
                        }
                    }

                    dimRgn->FineTune = gigSample->FineTune;
                }

                iVelocitySplitZone++;
            }
        }
        cout << "OK\n";

        if (bDryRun)
            return EXIT_SUCCESS;

        cout << "Saving initial gig file layout ... " << flush;
        // save result to disk (as .gig file)
        gig.Save(outFileName); 
        cout << "OK\n";

        cout << "Copying audio sample data ... " << flush;
        // finally write the actual wav sample data directly to the created gig file
        for (auto& itSmpl : queuedSamples) {
            gig::Sample* gigSample = itSmpl.first;
            WavInfo& wav = itSmpl.second;

            SF_INFO info = {};
            SNDFILE* hFile = sf_open(wav.fileName.c_str(), SFM_READ, &info);
            sf_command(hFile, SFC_SET_SCALE_FLOAT_INT_READ, 0, SF_TRUE);
            if (!hFile) throw gig::Exception("could not open file");
            // determine sample's bit depth
            int bitdepth;
            switch (info.format & 0xff) {
                case SF_FORMAT_PCM_S8:
                case SF_FORMAT_PCM_16:
                case SF_FORMAT_PCM_U8:
                    bitdepth = 16;
                    break;
                case SF_FORMAT_PCM_24:
                case SF_FORMAT_PCM_32:
                case SF_FORMAT_FLOAT:
                case SF_FORMAT_DOUBLE:
                    bitdepth = 24;
                    break;
                default:
                    sf_close(hFile); // close sound file
                    throw gig::Exception("format not supported");
            }

            const int bufsize = 10000;
            switch (bitdepth) {
                case 16: {
                    short* buffer = new short[bufsize * info.channels];
                    sf_count_t cnt = info.frames;
                    while (cnt) {
                        // libsndfile does the conversion for us (if needed)
                        int n = sf_readf_short(hFile, buffer, bufsize);
                        // write from buffer directly (physically) into .gig file
                        gigSample->Write(buffer, n);
                        cnt -= n;
                    }
                    delete[] buffer;
                    break;
                }
                case 24: {
                    int* srcbuf = new int[bufsize * info.channels];
                    uint8_t* dstbuf = new uint8_t[bufsize * 3 * info.channels];
                    sf_count_t cnt = info.frames;
                    while (cnt) {
                        // libsndfile returns 32 bits, convert to 24
                        int n = sf_readf_int(hFile, srcbuf, bufsize);
                        int j = 0;
                        for (int i = 0 ; i < n * info.channels ; i++) {
                            dstbuf[j++] = srcbuf[i] >> 8;
                            dstbuf[j++] = srcbuf[i] >> 16;
                            dstbuf[j++] = srcbuf[i] >> 24;
                        }
                        // write from buffer directly (physically) into .gig file
                        gigSample->Write(dstbuf, n);
                        cnt -= n;
                    }
                    delete[] srcbuf;
                    delete[] dstbuf;
                    break;
                }
            }
            sf_close(hFile);
        }
        cout << "OK\n";

    } catch (RIFF::Exception e) {
        cerr << "Failed generating output file:" << endl;
        e.PrintMessage();
        return EXIT_FAILURE;
    } catch (...) {
        cerr << "Unknown exception while trying to assemble output file." << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
