/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains functions struct and other type definitions for shema in 
 * namespace dunedaq::networkqueue::fsdc to be serialized via nlohmann::json.
 */
#ifndef DUNEDAQ_NETWORKQUEUE_FSDC_NLJS_HPP
#define DUNEDAQ_NETWORKQUEUE_FSDC_NLJS_HPP


#include "networkqueue/fsdc/Structs.hpp"


#include <nlohmann/json.hpp>

namespace dunedaq::networkqueue::fsdc {

    using data_t = nlohmann::json;


    
    inline void to_json(data_t& j, const Conf& obj) {
        j["nIntsPerVector"] = obj.nIntsPerVector;
        j["starting_int"] = obj.starting_int;
        j["ending_int"] = obj.ending_int;
        j["queue_timeout_ms"] = obj.queue_timeout_ms;
    }
    
    inline void from_json(const data_t& j, Conf& obj) {
        if (j.contains("nIntsPerVector"))
            j.at("nIntsPerVector").get_to(obj.nIntsPerVector);    
        if (j.contains("starting_int"))
            j.at("starting_int").get_to(obj.starting_int);    
        if (j.contains("ending_int"))
            j.at("ending_int").get_to(obj.ending_int);    
        if (j.contains("queue_timeout_ms"))
            j.at("queue_timeout_ms").get_to(obj.queue_timeout_ms);    
    }
    
    // fixme: add support for MessagePack serializers (at least)

} // namespace dunedaq::networkqueue::fsdc

#endif // DUNEDAQ_NETWORKQUEUE_FSDC_NLJS_HPP