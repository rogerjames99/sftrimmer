#include "../SF.h"

// abort compilation here if neither libsndfile nor libaudiofile are available
#if !HAVE_SNDFILE
#error "libsndfile not available!"
#error "(HAVE_SNDFILE is false)"
#endif

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
        RIFF::File *riff = new RIFF::File(argv[1]);
        sf2::File *sf = new sf2::File(riff);
        for (int i = 0; i < sf->GetSampleCount(); i++)
        {
            sf2::Sample *s = sf->GetSample(i);
            cout << "\t" << s->Name << " (Depth: " << ((s->GetFrameSize() / s->GetChannelCount()) * 8);
            cout << ", SampleRate: " << s->SampleRate;
            cout << ", Pitch: " << ((int)s->OriginalPitch);
            cout << ", Pitch Correction: " << ((int)s->PitchCorrection) << endl;
            cout << "\t\tStart: " << s->Start << ", End: " << s->End;
            cout << ", Start Loop: " << s->StartLoop << ", End Loop: " << s->EndLoop << endl;
            cout << "\t\tSample Type: " << GetSampleType(s->SampleType) << ", Sample Link: " << s->SampleLink << ")" << endl;

            // Get the sample data
            sf2::Sample::buffer_t sample_data = s->LoadSampleData();

            s->ReleaseSampleData();
        }

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
