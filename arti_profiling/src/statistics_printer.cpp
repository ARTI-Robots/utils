/*
 * This file is part of the software provided by the Graz University of Technology AIS group.
 *
 * Copyright (c) 2017, Alexander Buchegger
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted  provided that the
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *    disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *    following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <arti_profiling/statistics_printer.h>
#include <functional>
#include <iostream>

namespace arti_profiling
{

StatisticsPrinter::StatisticsPrinter(Profiler& profiler, std::ostream& out, const ros::NodeHandle& node_handle)
  : profiler_(&profiler), out_(&out), node_handle_(node_handle), config_server_(node_handle_)
{
  config_server_.setCallback(std::bind(&StatisticsPrinter::reconfigure, this, std::placeholders::_1));
}

StatisticsPrinter::StatisticsPrinter(Profiler& profiler, const ros::NodeHandle& node_handle)
  : StatisticsPrinter(profiler, std::cout, node_handle)
{
}

StatisticsPrinter::~StatisticsPrinter() = default;

void StatisticsPrinter::reconfigure(const StatisticsPrinterConfig& config)
{
  config_ = config;

  if (config_.print_statistics)
  {
    timer_ = node_handle_.createWallTimer(ros::WallDuration(config_.print_statistics_interval),
                                          &StatisticsPrinter::printStatistics, this);
  }
  else if (timer_)
  {
    timer_.stop();
    timer_ = {};
  }
}

void StatisticsPrinter::printStatistics(const ros::WallTimerEvent&)
{
  profiler_->printStatistics(*out_);
  profiler_->clear();
}

}  // namespace arti_profiling
