#include <cstring>
#include <SF.h>
#include <sndfile.h>

using namespace std;

const int LOOP_PADDING_POINTS = 16;
const int SAMPLE_ZERO_PADDING_POINTS = 46;
const int LENGTH_OF_SAMPLE_NAME = 20;
const int SAMPLE_HEADER_SIZE = 46;


string GetSampleType(uint16_t type)
{
    switch (type)
    {
    case sf2::Sample::MONO_SAMPLE:
        return "Mono Sample";
    case sf2::Sample::RIGHT_SAMPLE:
        return "Right Sample";
    case sf2::Sample::LEFT_SAMPLE:
        return "Left Sample";
    case sf2::Sample::LINKED_SAMPLE:
        return "Linked Sample";
    case sf2::Sample::ROM_MONO_SAMPLE:
        return "ROM Mono Sample";
    case sf2::Sample::ROM_RIGHT_SAMPLE:
        return "ROM Right Sample";
    case sf2::Sample::ROM_LEFT_SAMPLE:
        return "ROM Left Sample";
    case sf2::Sample::ROM_LINKED_SAMPLE:
        return "ROM Linked Sample";
    default:
        return "Unknown";
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Input and output arguments required" << endl;
        return EXIT_FAILURE;
    }

    try
    {
        typedef struct __attribute__ ((__packed__)) new_offsets
        {
            uint32_t Start;
            uint32_t End;
            uint32_t StartLoop;
            uint32_t EndLoop;
        } new_offsets_t;

        std::vector<uint16_t*> modified_samples;
        std::vector<size_t> size_of_individual_modified_samples;
        std::vector<new_offsets_t*> newoffsets_vector;
        size_t total_size_of_modified_data = 0;
    

        RIFF::File *riff = new RIFF::File(argv[1]);
        sf2::File *sf = new sf2::File(riff);
        for (int i = 0; i < sf->GetSampleCount(); i++)
        {
            sf2::Sample *s = sf->GetSample(i);
            uint depth = (s->GetFrameSize() / s->GetChannelCount()) * 8;
            cout << std::dec << "Processing sample " << i << endl;
            cout << "\t" << s->Name << " (Depth: " << depth;
            cout << ", SampleRate: " << s->SampleRate;
            cout << ", Pitch: " << ((int)s->OriginalPitch);
            cout << ", Pitch Correction: " << ((int)s->PitchCorrection) << endl;
            cout << "\t\tStart: " << s->Start << ", End: " << s->End;
            cout << ", Start Loop: " << s->StartLoop << ", End Loop: " << s->EndLoop << endl;
            cout << "\t\tSample Type: " << GetSampleType(s->SampleType) << ", Sample Link: " << s->SampleLink << ")" << endl;

            // Check the validity of the loop offsets
            // Check that there are least 8 sample points between
            // the sample start and the loop start
            if ((s->StartLoop - s->Start) < 8)
            {
                cout << "Insufficient sample points before loop start" << endl;
                continue;
            }

            // Check that there are least 8 sample points between
            // the sample end and the loop end.
            if ((s->End - s->EndLoop) < 8)
            {
                cout << "Insufficient sample points after loop end" << endl;
                continue;
            }

            // Correct the sample offsets LoadSampleData loads the sample into the start of the buffer.
            size_t SampleStartLoop = s->StartLoop - s->Start;
            size_t SampleEndLoop = s->EndLoop - s->Start;
            size_t SampleEnd = s->End - s->Start;
            size_t LoopSize = SampleEndLoop - SampleStartLoop;


            // Get the sample data
            sf2::Sample::buffer_t sample_data = s->LoadSampleData();
            long frame_count = s->GetTotalFrameCount();


            // Only handle 16 bit samples for now
            if (16 != depth)
            {
                cout << "Sample is not 16 bit" << endl;
                continue;
            }

            uint16_t* samples16bit = (uint16_t*)sample_data.pStart;

            /*
                Ideal loop layout
                =================

                1. 8 sample points before loop start sample point, the last 4 sample points of this section
                should be identical to the 4 sample points before the loop end sample.

                2. loop start sample point, this should be identical to the loop end sample point.

                3. loop sample points.

                5. loop end sample point.

                6. 8 sample points after loop end sample point, the first 4 sample points of this section
                should be identical to the 4 sample points after the loop start sample point.

                7. 46 zero sample points.

                This implementation overwrites the 9 bytes at loop end sample point - 4 and the 9 bytes from
                loop start sample point - 4 with the mean of the two values.
            */

            // Prepare to update the sample header
            new_offsets_t* newoffsets = new new_offsets_t;
            newoffsets_vector.push_back(newoffsets);
            newoffsets->Start = total_size_of_modified_data;

            // Allocate data to hold the modified sample
            size_t modified_data_length = SampleEndLoop - SampleStartLoop + LOOP_PADDING_POINTS + SAMPLE_ZERO_PADDING_POINTS;
            newoffsets->End = newoffsets->Start + modified_data_length - SAMPLE_ZERO_PADDING_POINTS;
            newoffsets->StartLoop = newoffsets->Start + 8;
            newoffsets->EndLoop = newoffsets->End - 8;

            cout << "NewOffsets" << endl
                << "Start " << newoffsets->Start
                << " End " << newoffsets->End
                << " StartLoop " << newoffsets->StartLoop
                << " EndLoop " << newoffsets->EndLoop << endl;

            size_of_individual_modified_samples.push_back(modified_data_length);
            total_size_of_modified_data += modified_data_length;
            uint16_t* modified_data = new uint16_t[modified_data_length];
            if (NULL == modified_data)
            {
                cout << "Unable to allocate memory for modified sample" << endl;
                exit(-1);
            }
            // Copy the sample data
            std::memset(modified_data, 0, modified_data_length * 2);
            std::memcpy(modified_data, samples16bit + SampleStartLoop - 8 , (modified_data_length - SAMPLE_ZERO_PADDING_POINTS) * 2);

            // Recalculate the sample point 
            size_t newSampleStart = 0;
            size_t newSampleStartLoop = 8;
            size_t newSampleEndLoop = newSampleStartLoop + LoopSize;
            size_t newEnd = modified_data_length;

            // Fix the loop points
            for (int i = newSampleStartLoop - 4; i < 9; i++)
                modified_data[i] = modified_data[i + LoopSize] = (modified_data[i] + modified_data[i + LoopSize]) / 2;
	
            modified_samples.push_back(modified_data);

            s->ReleaseSampleData();
        }

        cout << "Total size of modified sample data " << total_size_of_modified_data << endl;

        // Coalesce the modified samples
        size_t coalesced_samples_index = 0;
        uint16_t* coalesced_samples = new uint16_t[total_size_of_modified_data];
        if (NULL == coalesced_samples)
        {
            cout << "Unable to allocate memory for coalesced samples" << endl;
            exit(-1);
        }

        for (uint i = 0; i < modified_samples.size(); i++)
        {
            size_t size = size_of_individual_modified_samples[i];
            memcpy(coalesced_samples + coalesced_samples_index, modified_samples[i], size * 2);
            coalesced_samples_index += size;
        }

        // Replace the SMPL-CK chunk with one pointing at the new data
        RIFF::Chunk* pChunk = sf->GetSample(0)->pCkSmpl;
        RIFF::List* pList = pChunk->GetParent();
        cout << "Chunk ID " << pChunk->GetChunkIDString() 
            << " Parent list type " << pList->GetListTypeString() 
            << " Parent list subchunks " << pList->CountSubChunks() << endl;

        pChunk->Resize(total_size_of_modified_data * 2);
        pChunk->GetFile()->Save(argv[2]);
        // Save the sample data into soundfont object
        pChunk->WriteInt16((int16_t*)coalesced_samples, total_size_of_modified_data);

            // Modify the sample headers
        RIFF::File* pRiff = sf->GetRiffFile();
        RIFF::List* pPDTA = pRiff->GetSubList(LIST_TYPE_PDTA);
        RIFF::Chunk* pSHDR = pPDTA->GetSubChunk(CHUNK_ID_SHDR);
        uint8_t* pData = (uint8_t*)pSHDR->LoadChunkData();
        pData += LENGTH_OF_SAMPLE_NAME;

        for (uint i = 0; i < modified_samples.size(); i++)
        {
            // Get the new offset data
            new_offsets_t temp_offsets;
            std::memcpy(&temp_offsets, pData, sizeof(new_offsets_t));
            new_offsets_t* n = newoffsets_vector[i];
            cout << endl << "New offsets from vector" << endl
                << "Start " <<  n->Start << "(" <<  std::hex << n->Start << ")" << std::dec << endl
                << "End " <<  n->End << "(" <<  std::hex << n->End << ")" << std::dec << endl
                <<  "StartLoop " << n->StartLoop << "(" <<  std::hex << n->StartLoop << ")" << std::dec << endl
                <<  "EndLoop " << n->EndLoop << "(" <<  std::hex << n->EndLoop << ")" << std::dec << endl;

            cout << endl << "Old offsets" << endl
                << "Start " <<  temp_offsets.Start << "(" <<  std::hex << temp_offsets.Start << ")" << std::dec << endl
                << "End " <<  temp_offsets.End << "(" <<  std::hex << temp_offsets.End << ")" << std::dec << endl
                <<  "StartLoop " << temp_offsets.StartLoop << "(" <<  std::hex << temp_offsets.StartLoop << ")" << std::dec << endl
                <<  "EndLoop " << temp_offsets.EndLoop << "(" <<  std::hex << temp_offsets.EndLoop << ")" << std::dec << endl;
                
            // Patch the chunk data
            std::memcpy(pData, n, sizeof(new_offsets_t));

            std::memcpy(&temp_offsets, pData, sizeof(new_offsets_t));
            cout << endl << "New offsets" << endl
                << "Start " <<  temp_offsets.Start << "(" <<  std::hex << temp_offsets.Start << ")" << std::dec << endl
                << "End " <<  temp_offsets.End << "(" <<  std::hex << temp_offsets.End << ")" << std::dec << endl
                <<  "StartLoop " << temp_offsets.StartLoop << "(" <<  std::hex << temp_offsets.StartLoop << ")" << std::dec << endl
                <<  "EndLoop " << temp_offsets.EndLoop << "(" <<  std::hex << temp_offsets.EndLoop << ")" << std::dec << endl;

            pData += SAMPLE_HEADER_SIZE;
        }
        pSHDR->GetFile()->Save(argv[2]);
        pSHDR->ReleaseChunkData();

        delete sf;
        delete riff;
    }
    catch (RIFF::Exception e)
    {
        e.PrintMessage();
        return EXIT_FAILURE;
    }
    catch (...)
    {
        cout << "Unknown exception while trying to parse file." << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
