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
#ifndef MLPERFHELPER_H
#define MLPERFHELPER_H

#include "android/cpp/c/backend_c.h"
#include "android/cpp/c/type.h"
#include "tvm_backend_helper.h"

static void process_config(const mlperf_backend_configuration_t *configs,
                           TVMBackendHelper *backend_data) {

  backend_data->batchSize_ = 1;

  std::string &runtime = backend_data->runtime_;
  runtime = configs->accelerator;
  LOG(INFO) << "Config: Accelerator: " << configs->accelerator;

  if (runtime == "CPU")
  {
    backend_data->device_ = kDLCPU;
  } else if (runtime == "GPU") {
    backend_data->device_ = kDLGPU;
/*  } else if (runtime == "CUDA") {
    backend_data->device_ = kDLCUDA;
  } else if (runtime == "CUDAHost") {
    backend_data->device_ = kDLCUDAHost;*/
  } else if (runtime == "OpenCL") {
    backend_data->device_ = kDLOpenCL;
  } else if (runtime == "Vulkan") {
    backend_data->device_ = kDLVulkan;
  } else if (runtime == "Metal") {
    backend_data->device_ = kDLMetal;
  } else if (runtime == "VPI") {
    backend_data->device_ = kDLVPI;
  } else if (runtime == "ROCM") {
    backend_data->device_ = kDLROCM;
  } else if (runtime == "ExtDev") {
    backend_data->device_ = kDLExtDev;
  } else {
    LOG(FATAL) << "Error: Unsupported runtime: " << runtime;
  }

  // Batch size is zero if not specified
  backend_data->batchSize_ =
      (configs->batch_size == 0) ? 1 : configs->batch_size;

  LOG(INFO) << "Config: runtime: " << runtime
            << " batchSize: " << backend_data->batchSize_;
}

#endif
