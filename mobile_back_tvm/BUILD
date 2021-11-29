# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],  # Apache 2.0
)

config_setting(
    name = "android_arm64",
    values = {"cpu": "arm64-v8a"},
)

cc_library(
    name = "tvm_runtime",
    srcs = [
        "tvm/build-android/libtvm_runtime.so",
    ],
    hdrs = glob([
        "tvm/include/**/*.h",
        "tvm/3rdparty/dmlc-core/include/**/*.h",
        "tvm/3rdparty/dlpack/include/**/*.h",
    ]),
    copts = ["-Itvm/include", "-Itvm/3rdparty/dmlc-core/include", "-Itvm/3rdparty/dlpack/include"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "tvmbackend",
    srcs = select({
        ":android_arm64": [
            "//mobile_back_tvm/cpp/backend_tvm:libtvmbackend.so"
        ],
    }),
    deps = select({
        ":android_arm64": [
            "tvm_runtime",
        ],
    }),
)

