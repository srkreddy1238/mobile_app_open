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

#include <unordered_map>
#include <string>

#include "mlperf_helper.h"
#include "tvm_settings.h"
#include "tensorflow/core/platform/logging.h"
#include "tvm_backend_helper.h"

static TVMBackendHelper *backend_data_g = nullptr;

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

// Should return true if current hardware is supported.
bool mlperf_backend_matches_hardware(const char **not_allowed_message,
                                     const char **settings,
                                     const mlperf_device_info_t *device_info) {
  LOG(WARNING) << "Called: mlperf_backend_matches_hardware";
  if (device_info && device_info->model && device_info->manufacturer) {
    LOG(WARNING) << "TVM HW supported check: model: "
              << device_info->model
              << ", manufacturer: " << device_info->manufacturer;
  }

  *settings = tvm_settings.c_str();
  return true;
}

// Create a new backend and return the pointer to it.
mlperf_backend_ptr_t mlperf_backend_create(
    const char *model_path, mlperf_backend_configuration_t *configs,
    const char *native_lib_path) {
  LOG(WARNING) << "Called: mlperf_backend_create:" << model_path << " NativeLibPath:" << native_lib_path;
  if (backend_data_g) {
    LOG(WARNING) << "Only one backend instance can be active at a time";
    return NULL;
  }
  LOG(WARNING) << "CONFIGS count = " << configs->count;
  for (int i = 0; i < configs->count; ++i) {
    LOG(WARNING) << "configs->[" << configs->keys[i]
              << "] = " << configs->values[i];
  }

  LOG(WARNING) << "TVM Version:" << TVM_VERSION;

  backend_data_g = new TVMBackendHelper();
  TVMBackendHelper *backend_data = backend_data_g;

  process_config(configs, backend_data);
  LOG(WARNING) << "TVM Configuration Process Completed.";

  if(backend_data->tvm_config_model(model_path)) {
    LOG(WARNING) << "TVM Runtime init failed";
    delete backend_data;
    return NULL;
  }

  backend_data->get_data_formats();

  LOG(WARNING) << "TVM Setup completed successfully";

  return backend_data;
}

// Return the name of this backend.
const char *mlperf_backend_name(mlperf_backend_ptr_t backend_ptr) {
  TVMBackendHelper *backend_data = (TVMBackendHelper *)backend_ptr;
  LOG(WARNING) << "Called: mlperf_backend_name";
  return backend_data->name_;
}

// Destroy the backend pointer and its data.
void mlperf_backend_delete(mlperf_backend_ptr_t backend_ptr) {
  TVMBackendHelper *backend_data = (TVMBackendHelper *)backend_ptr;
  LOG(WARNING) << "Called: mlperf_backend_delete";
  for (void *ptr : backend_data->dl_cpu_outputs) {
    free(ptr);
  }
  delete backend_data;
  backend_data_g = nullptr;
}

// Run the inference for a sample.
mlperf_status_t mlperf_backend_issue_query(mlperf_backend_ptr_t backend_ptr) {
  TVMBackendHelper *backend_data = (TVMBackendHelper *)backend_ptr;
  LOG(WARNING) << "Called: mlperf_backend_issue_query: Call run";
  tvm::runtime::PackedFunc run = backend_data->mod_.GetFunction("run");
  run();
  LOG(WARNING) << "Called: mlperf_backend_issue_query: Run Completed";
  return MLPERF_SUCCESS;
}

// Flush the staged queries immediately.
mlperf_status_t mlperf_backend_flush_queries(mlperf_backend_ptr_t backend_ptr) {
  TVMBackendHelper *backend_data = (TVMBackendHelper *)backend_ptr;
  LOG(WARNING) << "Called: mlperf_backend_flush_queries";
  return MLPERF_SUCCESS;
}

// Return the number of inputs of the model.
int32_t mlperf_backend_get_input_count(mlperf_backend_ptr_t backend_ptr) {
  TVMBackendHelper *backend_data = (TVMBackendHelper *)backend_ptr;
  LOG(WARNING) << "Called: mlperf_backend_get_input_count";
  return backend_data->input_format_.size();
}

// Return the type of the ith input.
mlperf_data_t mlperf_backend_get_input_type(mlperf_backend_ptr_t backend_ptr,
                                            int32_t i) {
  TVMBackendHelper *backend_data = (TVMBackendHelper *)backend_ptr;
  LOG(WARNING) << "Called: mlperf_backend_get_input_type";
  return backend_data->input_format_[i];
}

// Set the data for ith input.
mlperf_status_t mlperf_backend_set_input(mlperf_backend_ptr_t backend_ptr,
                                         int32_t batchIndex, int32_t i,
                                         void *data) {
  TVMBackendHelper *backend_data = (TVMBackendHelper *)backend_ptr;
  LOG(WARNING) << "Called: mlperf_backend_set_input:" << batchIndex << " i:" << i;
  int dtypeSize = 1;
  int bufsize = backend_data->get_data_size(*backend_data->dl_inputs[i], dtypeSize);
  TVMArrayCopyFromBytes(backend_data->dl_inputs[i], data, dtypeSize*bufsize);
  LOG(WARNING) << "Called: mlperf_backend_set_input: Completed";
  return MLPERF_SUCCESS;
}

// Return the number of outputs for the model.
int32_t mlperf_backend_get_output_count(mlperf_backend_ptr_t backend_ptr) {
  TVMBackendHelper *backend_data = (TVMBackendHelper *)backend_ptr;
  LOG(WARNING) << "Called: mlperf_backend_get_output_count";
  return backend_data->output_format_.size();
}
// Return the type of ith output.
mlperf_data_t mlperf_backend_get_output_type(mlperf_backend_ptr_t backend_ptr,
                                             int32_t i) {
  TVMBackendHelper *backend_data = (TVMBackendHelper *)backend_ptr;
  LOG(WARNING) << "Called: mlperf_backend_get_output_type";
  return backend_data->output_format_[i];
}

// Get the data from ith output.
mlperf_status_t mlperf_backend_get_output(mlperf_backend_ptr_t backend_ptr,
                                          uint32_t batchIndex, int32_t i,
                                          void **data) {
  LOG(WARNING) << "Called: mlperf_backend_get_output:" << batchIndex << " i:" << i;
  TVMBackendHelper *backend_data = (TVMBackendHelper *)backend_ptr;

  LOG(WARNING) << "Called: mlperf_backend_get_output: TVM Copy call";
  int dtypeSize = 1;
  int bufsize = backend_data->get_data_size(*backend_data->dl_outputs[i], dtypeSize);
  TVMArrayCopyToBytes(backend_data->dl_outputs[i], backend_data->dl_cpu_outputs[i],
      dtypeSize*bufsize);
  LOG(WARNING) << "Called: mlperf_backend_get_output: TVM Copy Completed";
  *data = backend_data->dl_cpu_outputs[i];
  LOG(WARNING) << "Called: mlperf_backend_get_output: Returning";
  return MLPERF_SUCCESS;
}

void *mlperf_backend_get_buffer(size_t n) {
  LOG(WARNING) << "Called: mlperf_backend_get_buffer:" << n;
  void * ptr =  malloc(n);
  LOG(WARNING) << "Malloc Address:" << ptr;
  memset(ptr, 0x00, n);
  LOG(WARNING) << "Return get_buffer";
  return ptr;
}

void mlperf_backend_release_buffer(void *p) {
  LOG(WARNING) << "Called: mlperf_backend_release_buffer";
  free(p);
}

#ifdef __cplusplus
}
#endif  // __cplusplus
