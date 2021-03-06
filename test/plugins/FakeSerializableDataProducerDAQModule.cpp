/**
 * @file FakeSerializableDataProducerDAQModule.cc FakeSerializableDataProducerDAQModule class
 * implementation
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "FakeSerializableDataProducerDAQModule.hpp"

#include "appfwk/cmd/Nljs.hpp"
#include "networkqueue/fsdp/Nljs.hpp"
#include "networkqueue/fsd/Nljs.hpp"

#include <chrono>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <iostream>

#include <TRACE/trace.h>
/**
 * @brief Name used by TRACE TLOG calls from this source file
 */
#define TRACE_NAME "FakeSerializableDataProducer" // NOLINT

namespace dunedaq {
namespace networkqueue {

FakeSerializableDataProducerDAQModule::FakeSerializableDataProducerDAQModule(const std::string& name)
  : DAQModule(name)
  , thread_(std::bind(&FakeSerializableDataProducerDAQModule::do_work, this, std::placeholders::_1))
  , outputQueue_(nullptr)
  , queueTimeout_(100)
{
  register_command("conf", &FakeSerializableDataProducerDAQModule::do_configure);
  register_command("start", &FakeSerializableDataProducerDAQModule::do_start);
  register_command("stop", &FakeSerializableDataProducerDAQModule::do_stop);
}

void
FakeSerializableDataProducerDAQModule::init(const nlohmann::json& init_data)
{
  auto ini = init_data.get<dunedaq::appfwk::cmd::ModInit>();
  for (const auto& qi : ini.qinfos) {
    if (qi.name == "output") {
      ERS_INFO("FDP: output queue is " << qi.inst);
      outputQueue_.reset(new dunedaq::appfwk::DAQSink<fsd::FakeData>(qi.inst));
    }
  }
}

void
FakeSerializableDataProducerDAQModule::do_configure(const data_t& data)
{
  cfg_ = data.get<fsdp::Conf>();

  queueTimeout_ = std::chrono::milliseconds(cfg_.queue_timeout_ms);
}

void
FakeSerializableDataProducerDAQModule::do_start(const data_t& /*data*/)
{
  thread_.start_working_thread();
}

void
FakeSerializableDataProducerDAQModule::do_stop(const data_t& /*data*/)
{
  thread_.stop_working_thread();
}

void
FakeSerializableDataProducerDAQModule::do_work(std::atomic<bool>& running_flag)
{
  ERS_INFO("FDP: do_work");
  int current_int = cfg_.starting_int;
  size_t counter = 0;
  std::ostringstream oss;

  while (running_flag.load()) {
    TLOG(TLVL_TRACE) << get_name() << ": Creating output vector";
    fsd::FakeData output{current_int++};
    oss << "Produced vector " << counter << " with contents " << current_int;
    ers::debug(ProducerProgressUpdate(ERS_HERE, get_name(), oss.str()));
    oss.str("");

    TLOG(TLVL_TRACE) << get_name() << ": Pushing vector into outputQueue";
    ERS_INFO("FDP \"" << get_name() << "\" push " << counter);
    try {
      outputQueue_->push(std::move(output), queueTimeout_);
    } catch(const dunedaq::appfwk::QueueTimeoutExpired& ex) {
      ERS_INFO("FDP \"" << get_name() << "\" queue timeout on " << counter);
      ers::warning(ex);
    }
    

    TLOG(TLVL_TRACE) << get_name() << ": Start of sleep between sends";
    std::this_thread::sleep_for(std::chrono::milliseconds(cfg_.wait_between_sends_ms));
    TLOG(TLVL_TRACE) << get_name() << ": End of do_work loop";
    counter++;
  }
}

} // namespace networkqueue
} // namespace dunedaq

DEFINE_DUNE_DAQ_MODULE(dunedaq::networkqueue::FakeSerializableDataProducerDAQModule)


// Local Variables:
// c-basic-offset: 2
// End:
