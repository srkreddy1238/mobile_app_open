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

#
# This file contains an application that loads and executes a neural network
# using the TVM Native API and saves the MLPerf output to a file.
#
load("@bazel_skylib//rules:common_settings.bzl", "string_flag")

package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],  # Apache 2.0
)


config_setting(
    name = "android_arm64",
    values = {"cpu": "arm64-v8a"},
)

load(
    "@org_tensorflow//tensorflow/lite:build_def.bzl",
    "tflite_copts",
    "tflite_jni_binary",
)

tvm_copts = [
    "-Imobile_back_tvm/tvm/include",
    "-Imobile_back_tvm/tvm/3rdparty/dmlc-core/include",
    "-Imobile_back_tvm/tvm/3rdparty/dlpack/include",
    "-lc++_shared",
]

load(
    "@org_tensorflow//tensorflow/lite:build_def.bzl",
    "tflite_copts",
    "tflite_jni_binary",
)

cc_library(
    name = "tvm_c",
    srcs = select({
        "android_arm64": [
            "tvm_c.cc",
            "tvm_backend_helper.cc",
        ],
    }),
    hdrs = [] + select({
        "android_arm64": [
            "tvm_settings.h",
            "tvm_backend_helper.h",
            "mlperf_helper.h",
        ],
    }),
    copts = tvm_copts,
    deps = [
        "//android/cpp/c:headers",
        "@org_tensorflow//tensorflow/core:tflite_portable_logging",
    ] + select({
        "android_arm64": [
            "@org_tensorflow//tensorflow/lite/c:c_api",
            "//mobile_back_tvm:tvm_runtime",
        ],
    }),
    alwayslink = 1,
)

tflite_jni_binary(
    name = "libtvmbackend.so",
    linkscript = "//android/cpp/c:version_script.lds",
    deps = [
        ":tvm_c",
    ],
)

