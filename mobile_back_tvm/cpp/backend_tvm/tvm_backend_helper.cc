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

#include <fstream>
#include <string>
#include <vector>

#include "tvm_backend_helper.h"
#include "tensorflow/core/platform/logging.h"

int TVMBackendHelper::tvm_config_model(const char *model_path) {
  void* lib_handle_{nullptr};
  LOG(INFO) << "TVM Backend Helper: Load Model";
  /* Load Model */
  tvm::runtime::Module mod_dylib =
        tvm::runtime::Module::LoadFromFile((std::string(model_path) + "/model.so").c_str()) ;


  LOG(INFO) << "TVM Backend Helper: Model Loaded";
  /* Read Graph */
  LOG(INFO) << "TVM Backend Helper: Read Graph";
  std::ifstream json_in((std::string(model_path) + "/model.json").c_str());
  if(json_in.fail())
  { 
    LOG(ERROR) << "Fail to open model graph (json):" + std::string(model_path) + "/model.json";
    return -1;
  }
  const std::string json_data((std::istreambuf_iterator<char>(json_in)),
                               std::istreambuf_iterator<char>());
  json_in.close();


  /* Read Parameters */ 
  LOG(INFO) << "TVM Backend Helper: Read Params";
  std::ifstream params_in((std::string(model_path) + "/model.params").c_str(), std::ios::binary);
  if(params_in.fail())
  { 
    LOG(ERROR) << "Fail to open model params:" + std::string(model_path) + "/model.params";
    return -1;
  }
  LOG(INFO) << "TVM Backend Helper: Params Read:" << params_in.tellg();

  const std::string params_data((std::istreambuf_iterator<char>(params_in)),
                                 std::istreambuf_iterator<char>());
  params_in.close();

  TVMByteArray params_arr;
  params_arr.data = params_data.c_str();
  params_arr.size = params_data.length();
  LOG(INFO) << "TVM Backend Helper: Byte Array Prepared:" << params_data.length();


  /* Initialize Graph Runtime */
  int dtype_code = kDLFloat;
  int dtype_bits = 32;
  int dtype_lanes = 1;
  int device_type = device_;
  int device_id = 0;

  /* Get global function module for graph runtime */
  LOG(INFO) << "TVM Backend Helper: Create Runtime";
  mod_ =
    //(*tvm::runtime::Registry::Get("tvm.graph_executor.create"))(json_data,
    (*tvm::runtime::Registry::Get("tvm.graph_runtime.create"))(json_data,
                                                                mod_dylib,
                                                                device_type,
                                                                device_id);
  /* Load Params */
  LOG(INFO) << "TVM Backend Helper: Load Params";
  tvm::runtime::PackedFunc load_params = mod_.GetFunction("load_params");
  load_params(params_arr);

  LOG(INFO) << "TVM Backend Helper: Runtime and Model Setup Completed";
  return 0;
}

size_t TVMBackendHelper::get_data_size(const DLTensor& arr, int& dtypeSize) {
  size_t size = 1;
  for (tvm_index_t i = 0; i < arr.ndim; ++i) {
    size *= static_cast<size_t>(arr.shape[i]);
  }
  dtypeSize = (arr.dtype.bits * arr.dtype.lanes + 7) / 8;
  return size;
}


void TVMBackendHelper::get_data_formats() {
  int num_inputs = mod_.GetFunction("get_num_inputs")();
  int num_outputs = mod_.GetFunction("get_num_outputs")();
  int dtypeSize = 1;
  LOG(INFO) << "Num Inputs:" << num_inputs;
  LOG(INFO) << "Num Outputs:" << num_outputs;

  DLTensor* x;
  tvm::runtime::PackedFunc get_input = mod_.GetFunction("get_input");
  x = get_input(0);
  int bufSize = get_data_size(*x, dtypeSize);
  input_format_.push_back(
      {mlperf_data_t::Type::Float32, bufSize});
  LOG(INFO) << "Input Size:" << bufSize;

  dl_inputs.push_back(x);

  DLTensor* y;
  for (int i=0;i<num_outputs;++i) {
    tvm::runtime::PackedFunc get_output = mod_.GetFunction("get_output");
    y = get_output(i);
    bufSize = get_data_size(*y, dtypeSize);
    LOG(INFO) << "Output Size: " << i << " :" << bufSize;
    output_format_.push_back(
        {mlperf_data_t::Type::Float32, bufSize});
    dl_outputs.push_back(y);

    dl_cpu_outputs.push_back(malloc(bufSize*dtypeSize));
  }
}
