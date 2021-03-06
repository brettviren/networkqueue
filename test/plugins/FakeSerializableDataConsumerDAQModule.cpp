/**
 * @file FakeSerializableDataConsumerDAQModule.cxx FakeSerializableDataConsumerDAQModule class
 * implementation
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "FakeSerializableDataConsumerDAQModule.hpp"

#include "appfwk/cmd/Nljs.hpp"
#include "networkqueue/fsdc/Nljs.hpp"
#include "networkqueue/fsd/Nljs.hpp"

#include "TRACE/trace.h"
#include <ers/ers.h>

/**
 * @brief Name used by TRACE TLOG calls from this source file
 */
#define TRACE_NAME "FakeSerializableDataConsumer" // NOLINT

#include <chrono>
#include <string>
#include <vector>

namespace dunedaq::networkqueue {

FakeSerializableDataConsumerDAQModule::FakeSerializableDataConsumerDAQModule(const std::string& name)
  : DAQModule(name)
  , thread_(std::bind(&FakeSerializableDataConsumerDAQModule::do_work, this, std::placeholders::_1))
  , inputQueue_(nullptr)
{
  register_command("conf",  &FakeSerializableDataConsumerDAQModule::do_configure);
  register_command("start", &FakeSerializableDataConsumerDAQModule::do_start);
  register_command("stop",  &FakeSerializableDataConsumerDAQModule::do_stop);
}

void
FakeSerializableDataConsumerDAQModule::init(const nlohmann::json& init_data)
{
  auto ini = init_data.get<dunedaq::appfwk::cmd::ModInit>();
  for (const auto& qi : ini.qinfos) {
    if (qi.name == "input") {
      ERS_INFO("FDP: input queue is " << qi.inst);
      inputQueue_.reset(new dunedaq::appfwk::DAQSource<fsd::FakeData>(qi.inst));
    }
  }
}

void
FakeSerializableDataConsumerDAQModule::do_configure(const data_t& data)
{
  cfg_ = data.get<fsdc::Conf>();

  queueTimeout_ = std::chrono::milliseconds(cfg_.queue_timeout_ms);
}

void
FakeSerializableDataConsumerDAQModule::do_start(const data_t& /*data*/)
{
  thread_.start_working_thread();
}

void
FakeSerializableDataConsumerDAQModule::do_stop(const data_t& /*data*/)
{
  thread_.stop_working_thread();
}

void
FakeSerializableDataConsumerDAQModule::do_work(std::atomic<bool>& running_flag)
{
  ERS_INFO("FDC: do_work");
  int counter = 0;
  int fail_count = 0;
  int timeout_count = 0;
  fsd::FakeData fake_data;
  int prev_fake_count=-1;
  std::ostringstream oss;

  while (running_flag.load()) {
    try {
      inputQueue_->pop(fake_data, queueTimeout_);
    } catch (const dunedaq::appfwk::QueueTimeoutExpired& excpt) {
      //ERS_INFO("FDC \"" << get_name() << "\" queue timeout on " << counter);
      ++timeout_count;
      continue;
    }

    if(prev_fake_count != -1 && (fake_data.fake_count != prev_fake_count+1)){
      ERS_INFO("Got fake_count " << fake_data.fake_count << " when expecting " << (prev_fake_count+1));
      ++fail_count;
    }
    counter++;
  }
  
  oss << ": Processed " << counter << " objects with " << fail_count << " failures and " << timeout_count << " timeouts";
  ers::info(ConsumerProgressUpdate(ERS_HERE, get_name(), oss.str()));
}

} // namespace dunedaq::appfwk

DEFINE_DUNE_DAQ_MODULE(dunedaq::networkqueue::FakeSerializableDataConsumerDAQModule)

// Local Variables:
// c-basic-offset: 2
// End:
