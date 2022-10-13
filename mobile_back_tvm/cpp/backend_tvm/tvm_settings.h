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
#ifndef TVM_SETTINGS_H
#define TVM_SETTINGS_H

#include <string>

const std::string empty_settings = "";

const std::string tvm_settings = R"SETTINGS(
common_setting {
  id: "num_threads"
  name: "Number of threads"
  value {
    value: "4"
    name: "4 threads"
  }
  acceptable_value {
    value: "1"
    name: "Single thread"
  }
  acceptable_value {
    value: "2"
    name: "2 threads"
  }
  acceptable_value {
    value: "4"
    name: "4 threads"
  }
  acceptable_value {
    value: "8"
    name: "8 threads"
  }
  acceptable_value {
    value: "16"
    name: "16 threads"
  }
}

common_setting {
  id: "configuration"
  name: "Configuration"
  value {
    value: "TVM backend"
    name: "TVM"
  }
}

common_setting {
  id: "share_results"
  name: "Share results"
  value {
    value: "0"
    name: "false"
  }
  acceptable_value {
    value: "1"
    name: "true"
  }
  acceptable_value {
    value: "0"
    name: "false"
  }
}

benchmark_setting {
  benchmark_id: "IC_tpu_uint8"
  accelerator: "OpenCL"
  accelerator_desc: "TVM OpenCL-Texture Backend"
  configuration: "TVM"
  src: "https://github.com/srkreddy1238/mobile_models/raw/tvm/v1.2/fp16-texture/model_ic.zip"

}

benchmark_setting {
  benchmark_id: "IS_uint8"
  accelerator: "OpenCL"
  accelerator_desc: "TVM OpenCL-Texture Backend"
  configuration: "TVM"
  src: "https://github.com/srkreddy1238/mobile_models/raw/tvm/v1.2/fp16-texture/model_is.zip"
}

benchmark_setting {
  benchmark_id: "LU_float32"
  accelerator: "OpenCL"
  accelerator_desc: "TVM OpenCL-Buffer(Vanila TVM) Backend"
  configuration: "TVM"
  src: "https://github.com/srkreddy1238/mobile_models/raw/tvm/v1.2/fp16-texture/model_lu.zip"
}

benchmark_setting {
  benchmark_id: "OD_uint8"
  accelerator: "OpenCL"
  accelerator_desc: "TVM OpenCL-Texture Backend"
  configuration: "TVM"
  src: "https://github.com/srkreddy1238/mobile_models/raw/tvm/v1.2/fp16-texture/model_lu.zip"
}


)SETTINGS";

#endif
