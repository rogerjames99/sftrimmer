#include <cstring>
#include <SF.h>
#include <sndfile.h>

using namespace std;

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
        return EXIT_FAILURE;
    }

    try
    {
        std::vector<uint16_t*> modified_samples;
        std::vector<size_t> size_of_individual_modified_samples;
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

            // Correct the sample offsets LoadSampleData loads the sample into the start of the buffer.
            int StartLoop = s->StartLoop - s->Start;
            int EndLoop = s->EndLoop - s->Start;
            int End = s->End - s->Start;

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

            // Switching to hex output
            cout << std::hex;

            /*
                Ideal loop layout
                =================

                1. 8 samples before loop start sample, the last 4 samples of this section
                should be identical to the 4 samples before the loop end sample.

                2. loop start sample

                3. loop samples

                5. loop end sample

                6. 8 samples after loop end sample, the first 4 samples of this section
                should be identical to the 4 samples after the loop start sample.
            */

            // Allocate data to hold the modified sample
            size_t modified_data_length = EndLoop - StartLoop + 16;
            size_of_individual_modified_samples.push_back(modified_data_length);
            total_size_of_modified_data += modified_data_length;
            uint16_t* modified_data = new uint16_t[modified_data_length];
            if (NULL == modified_data)
            {
                cout << "Unable to allocate memory for modified sample" << endl;
                exit(-1);
            }

            std::memcpy(modified_data, samples16bit + StartLoop -8 , modified_data_length * 2);

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

        // Modify the sample headers

        // Write out the modified file
        sf->GetRiffFile()->Save("test.sf2");

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
