#include "catch2/catch.hpp"
#include "../sources/AudioBuffer.h"
#include <algorithm>
using namespace Catch::literals;

TEST_CASE("[AudioBuffer/SplitBuffer] Empty buffers")
{
    AudioBuffer<float> floatBuffer;
    REQUIRE(floatBuffer.empty());
    REQUIRE(floatBuffer.getNumFrames() == 0);
    AudioBuffer<double> doubleBuffer;
    REQUIRE(doubleBuffer.empty());
    REQUIRE(doubleBuffer.getNumFrames() == 0);
    AudioBuffer<int> intBuffer;
    REQUIRE(intBuffer.empty());
    REQUIRE(intBuffer.getNumFrames() == 0);

    SplitAudioBuffer<float> floatSplitBuffer;
    REQUIRE(floatSplitBuffer.empty());
    REQUIRE(floatSplitBuffer.getNumFrames() == 0);
    SplitAudioBuffer<double> doubleSplitBuffer;
    REQUIRE(doubleSplitBuffer.empty());
    REQUIRE(doubleSplitBuffer.getNumFrames() == 0);
    SplitAudioBuffer<int> intSplitBuffer;
    REQUIRE(intSplitBuffer.empty());
    REQUIRE(intSplitBuffer.getNumFrames() == 0);
}

TEST_CASE("[AudioBuffer/SplitBuffer] Non-empty")
{
    AudioBuffer<float> floatBuffer(10);
    REQUIRE(!floatBuffer.empty());
    REQUIRE(floatBuffer.getNumFrames() == 10);
    AudioBuffer<double> doubleBuffer(10);
    REQUIRE(!doubleBuffer.empty());
    REQUIRE(doubleBuffer.getNumFrames() == 10);
    AudioBuffer<int> intBuffer(10);
    REQUIRE(!intBuffer.empty());
    REQUIRE(intBuffer.getNumFrames() == 10);

    SplitAudioBuffer<float> floatSplitBuffer(10);
    REQUIRE(!floatSplitBuffer.empty());
    REQUIRE(floatSplitBuffer.getNumFrames() == 10);
    SplitAudioBuffer<double> doubleSplitBuffer(10);
    REQUIRE(!doubleSplitBuffer.empty());
    REQUIRE(doubleSplitBuffer.getNumFrames() == 10);
    SplitAudioBuffer<int> intSplitBuffer(10);
    REQUIRE(!intSplitBuffer.empty());
    REQUIRE(intSplitBuffer.getNumFrames() == 10);
}

TEST_CASE("[AudioBuffer/SplitBuffer] Access")
{
    const int size { 5 };
    AudioBuffer<double> doubleBuffer(size);
    for (auto chanIdx = 0; chanIdx < doubleBuffer.getNumChannels(); ++chanIdx)
        for (auto frameIdx = 0; frameIdx < doubleBuffer.getNumFrames(); ++frameIdx)
            doubleBuffer.getSample(chanIdx, frameIdx) = static_cast<double>(doubleBuffer.getNumFrames()) * chanIdx + frameIdx;

    for (auto chanIdx = 0; chanIdx < doubleBuffer.getNumChannels(); ++chanIdx)
        for (auto frameIdx = 0; frameIdx < doubleBuffer.getNumFrames(); ++frameIdx)
            REQUIRE(doubleBuffer.getSample(chanIdx, frameIdx) == static_cast<double>(doubleBuffer.getNumFrames()) * chanIdx + frameIdx);

    SplitAudioBuffer<double> splitDoubleBuffer(size);
    for (auto chanIdx = 0; chanIdx < splitDoubleBuffer.getNumChannels(); ++chanIdx)
        for (auto frameIdx = 0; frameIdx < splitDoubleBuffer.getNumFrames(); ++frameIdx)
            splitDoubleBuffer.getSample(chanIdx, frameIdx) = static_cast<double>(splitDoubleBuffer.getNumFrames()) * chanIdx + frameIdx;

    for (auto chanIdx = 0; chanIdx < splitDoubleBuffer.getNumChannels(); ++chanIdx)
        for (auto frameIdx = 0; frameIdx < splitDoubleBuffer.getNumFrames(); ++frameIdx)
            REQUIRE(splitDoubleBuffer.getSample(chanIdx, frameIdx) == static_cast<double>(splitDoubleBuffer.getNumFrames()) * chanIdx + frameIdx);
}

TEST_CASE("[AudioBuffer/SplitBuffer] Access 2")
{
    const int size { 5 };
    AudioBuffer<int> doubleBuffer(size);
    for (auto chanIdx = 0; chanIdx < doubleBuffer.getNumChannels(); ++chanIdx)
        for (auto frameIdx = 0; frameIdx < doubleBuffer.getNumFrames(); ++frameIdx)
            doubleBuffer(chanIdx, frameIdx) = static_cast<int>(doubleBuffer.getNumFrames()) * chanIdx + frameIdx;

    for (auto chanIdx = 0; chanIdx < doubleBuffer.getNumChannels(); ++chanIdx)
        for (auto frameIdx = 0; frameIdx < doubleBuffer.getNumFrames(); ++frameIdx)
            REQUIRE(doubleBuffer(chanIdx, frameIdx) == static_cast<int>(doubleBuffer.getNumFrames()) * chanIdx + frameIdx);

    SplitAudioBuffer<int> splitDoubleBuffer(size);
    for (auto chanIdx = 0; chanIdx < splitDoubleBuffer.getNumChannels(); ++chanIdx)
        for (auto frameIdx = 0; frameIdx < splitDoubleBuffer.getNumFrames(); ++frameIdx)
            splitDoubleBuffer(chanIdx, frameIdx) = static_cast<int>(splitDoubleBuffer.getNumFrames()) * chanIdx + frameIdx;

    for (auto chanIdx = 0; chanIdx < splitDoubleBuffer.getNumChannels(); ++chanIdx)
        for (auto frameIdx = 0; frameIdx < splitDoubleBuffer.getNumFrames(); ++frameIdx)
            REQUIRE(splitDoubleBuffer(chanIdx, frameIdx) == static_cast<int>(splitDoubleBuffer.getNumFrames()) * chanIdx + frameIdx);
}

TEST_CASE("[AudioBuffer/SplitBuffer] Iterators")
{
    const int size { 256 };
    const float fillValue { 2.0f };
    AudioBuffer<float> buffer(size);
    for (auto chanIdx = 0; chanIdx < buffer.getNumChannels(); ++chanIdx)
        std::fill(buffer.begin(chanIdx), buffer.end(chanIdx), fillValue);

    for (auto chanIdx = 0; chanIdx < buffer.getNumChannels(); ++chanIdx)
        REQUIRE( std::all_of(buffer.begin(chanIdx), buffer.end(chanIdx), [fillValue](auto value) { return value == fillValue; }) );

    SplitAudioBuffer<float> splitBuffer(size);
    for (auto chanIdx = 0; chanIdx < splitBuffer.getNumChannels(); ++chanIdx)
        std::fill(splitBuffer.begin(chanIdx), splitBuffer.end(chanIdx), fillValue);

    for (auto chanIdx = 0; chanIdx < splitBuffer.getNumChannels(); ++chanIdx)
        REQUIRE( std::all_of(splitBuffer.begin(chanIdx), splitBuffer.end(chanIdx), [fillValue](auto value) { return value == fillValue; }) );
}

template<class Type, unsigned int NumChannels, unsigned int Alignment = 16>
void channelAlignmentTest(int size)
{
    static constexpr auto AlignmentMask { Alignment - 1 };
    const Type fillValue { 2 };
    AudioBuffer<Type, NumChannels, Alignment> buffer(size);
    for (auto chanIdx = 0; chanIdx < NumChannels; ++chanIdx)
        REQUIRE( ((size_t)buffer.getChannel(chanIdx) & AlignmentMask) == 0 );

    SplitAudioBuffer<Type, NumChannels, Alignment> splitBuffer(size);
    for (auto chanIdx = 0; chanIdx < NumChannels; ++chanIdx)
        REQUIRE( ((size_t)splitBuffer.getChannel(chanIdx) & AlignmentMask) == 0 );
}

TEST_CASE("[AudioBuffer/SplitBuffer] Channel alignments (floats)")
{
    channelAlignmentTest<float, 1>(4);
    channelAlignmentTest<float, 1>(5);
    channelAlignmentTest<float, 1>(8);
    channelAlignmentTest<float, 1>(256);
    channelAlignmentTest<float, 1>(257);
    channelAlignmentTest<float, 1>(1023);
    channelAlignmentTest<float, 1>(1024);
    channelAlignmentTest<float, 1>(65537);
    channelAlignmentTest<float, 1>(65536);
    channelAlignmentTest<float, 1>(65535);

    channelAlignmentTest<float, 2>(4);
    channelAlignmentTest<float, 2>(5);
    channelAlignmentTest<float, 2>(8);
    channelAlignmentTest<float, 2>(256);
    channelAlignmentTest<float, 2>(257);
    channelAlignmentTest<float, 2>(1023);
    channelAlignmentTest<float, 2>(1024);
    channelAlignmentTest<float, 2>(65537);
    channelAlignmentTest<float, 2>(65536);
    channelAlignmentTest<float, 2>(65535);

    channelAlignmentTest<float, 3>(4);
    channelAlignmentTest<float, 3>(5);
    channelAlignmentTest<float, 3>(8);
    channelAlignmentTest<float, 3>(256);
    channelAlignmentTest<float, 3>(257);
    channelAlignmentTest<float, 3>(1023);
    channelAlignmentTest<float, 3>(1024);
    channelAlignmentTest<float, 3>(65537);
    channelAlignmentTest<float, 3>(65536);
    channelAlignmentTest<float, 3>(65535);

    channelAlignmentTest<float, 4>(4);
    channelAlignmentTest<float, 4>(5);
    channelAlignmentTest<float, 4>(8);
    channelAlignmentTest<float, 4>(256);
    channelAlignmentTest<float, 4>(257);
    channelAlignmentTest<float, 4>(1023);
    channelAlignmentTest<float, 4>(1024);
    channelAlignmentTest<float, 4>(65537);
    channelAlignmentTest<float, 4>(65536);
    channelAlignmentTest<float, 4>(65535);

    channelAlignmentTest<float, 1, 4>(4);
    channelAlignmentTest<float, 1, 4>(5);
    channelAlignmentTest<float, 1, 4>(8);
    channelAlignmentTest<float, 1, 4>(256);
    channelAlignmentTest<float, 1, 4>(257);
    channelAlignmentTest<float, 1, 4>(1023);
    channelAlignmentTest<float, 1, 4>(1024);
    channelAlignmentTest<float, 1, 4>(65537);
    channelAlignmentTest<float, 1, 4>(65536);
    channelAlignmentTest<float, 1, 4>(65535);

    channelAlignmentTest<float, 2, 4>(4);
    channelAlignmentTest<float, 2, 4>(5);
    channelAlignmentTest<float, 2, 4>(8);
    channelAlignmentTest<float, 2, 4>(256);
    channelAlignmentTest<float, 2, 4>(257);
    channelAlignmentTest<float, 2, 4>(1023);
    channelAlignmentTest<float, 2, 4>(1024);
    channelAlignmentTest<float, 2, 4>(65537);
    channelAlignmentTest<float, 2, 4>(65536);
    channelAlignmentTest<float, 2, 4>(65535);

    channelAlignmentTest<float, 1, 8>(4);
    channelAlignmentTest<float, 1, 8>(5);
    channelAlignmentTest<float, 1, 8>(8);
    channelAlignmentTest<float, 1, 8>(256);
    channelAlignmentTest<float, 1, 8>(257);
    channelAlignmentTest<float, 1, 8>(1023);
    channelAlignmentTest<float, 1, 8>(1024);
    channelAlignmentTest<float, 1, 8>(65537);
    channelAlignmentTest<float, 1, 8>(65536);
    channelAlignmentTest<float, 1, 8>(65535);

    channelAlignmentTest<float, 2, 8>(4);
    channelAlignmentTest<float, 2, 8>(5);
    channelAlignmentTest<float, 2, 8>(8);
    channelAlignmentTest<float, 2, 8>(256);
    channelAlignmentTest<float, 2, 8>(257);
    channelAlignmentTest<float, 2, 8>(1023);
    channelAlignmentTest<float, 2, 8>(1024);
    channelAlignmentTest<float, 2, 8>(65537);
    channelAlignmentTest<float, 2, 8>(65536);
    channelAlignmentTest<float, 2, 8>(65535);
}