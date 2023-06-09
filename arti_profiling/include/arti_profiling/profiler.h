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
#ifndef ARTI_PROFILING_PROFILER_H
#define ARTI_PROFILING_PROFILER_H

#include <iosfwd>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace arti_profiling
{

class Profile;

using ProfilePtr = std::shared_ptr<Profile>;

class Profiler
{
public:
  using Mutex = std::recursive_mutex;
  using Lock = std::unique_lock<Mutex>;

  explicit Profiler(std::string name);
  Profiler(Profiler& parent, std::string name);
  Profiler(const Profiler&) = delete;
  virtual ~Profiler();

  Profiler& operator=(const Profiler&) = delete;

  static Profiler& getRootInstance();

  void printStatistics(std::ostream& out, int indent = 0) const;

  struct ProfileUpdate
  {
    ProfileUpdate(ProfilePtr& _profile, Mutex &mutex) : profile(_profile), local_lock(mutex)
    {
    }

    ProfilePtr& profile;

    Lock local_lock;
  };

  ProfileUpdate getProfile(const std::string& name);
  void clear();
  bool hasChildren() const;

protected:
  Profiler();

  void addChild(Profiler* child);
  void removeChild(Profiler* child);

  mutable Mutex mutex_;
  Profiler* parent_{nullptr};
  std::string name_;
  std::vector<Profiler*> children_;
  std::map<std::string, ProfilePtr> profiles_;
};

}  // namespace arti_profiling

#endif  // ARTI_PROFILING_PROFILER_H
