/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains struct and other type definitions for shema in 
 * namespace dunedaq::networkqueue::fsd.
 */
#ifndef DUNEDAQ_NETWORKQUEUE_FSD_STRUCTS_HPP
#define DUNEDAQ_NETWORKQUEUE_FSD_STRUCTS_HPP

#include <cstdint>


namespace dunedaq::networkqueue::fsd {

    // @brief A count of not too many things
    using Count = int32_t;


    // @brief A fake timestamp
    using Timestamp = int64_t;


    // @brief Another fake Serializable data
    struct AnotherFakeData {

        // @brief A fake count of something
        Count fake_count;

        // @brief A fake timestamp for the data
        Timestamp fake_timestamp;
    };

    // @brief Fake Serializable data
    struct FakeData {

        // @brief A fake count of something
        Count fake_count;
    };

} // namespace dunedaq::networkqueue::fsd

#endif // DUNEDAQ_NETWORKQUEUE_FSD_STRUCTS_HPP