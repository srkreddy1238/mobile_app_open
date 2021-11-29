/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef TVMBACKENDHELPER_H
#define TVMBACKENDHELPER_H

#include <vector>
#include "android/cpp/c/backend_c.h"
#include "android/cpp/c/type.h"

#include <dlpack/dlpack.h>
#include <tvm/runtime/module.h>
#include <tvm/runtime/registry.h>
#include "tvm/runtime/c_runtime_api.h"


class TVMBackendHelper {
 private:
 public:
  tvm::runtime::Module mod_;

  const char *name_ = "tvm";
  std::string runtime_;
  DLDeviceType device_;
  std::vector<mlperf_data_t> input_format_;
  std::vector<mlperf_data_t> output_format_;
  std::vector<DLTensor*> dl_inputs;
  std::vector<DLTensor*> dl_outputs;
  std::vector<void*> dl_cpu_outputs;

  int batchSize_;

  /* exposed functions */

  int tvm_config_model(const char *model_path);
  void get_data_formats();
  size_t get_data_size(const DLTensor& arr);
};


#endif
