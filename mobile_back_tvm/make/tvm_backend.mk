# Copyright 2021 The MLPerf Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################

.PHONY:

ifeq (${USE_PROXY_WORKAROUND},1)
  export PROXY_WORKAROUND1=\
	-v /etc/ssl/certs:/etc/ssl/certs \
	-v /usr/share/ca-certificates:/usr/share/ca-certificates \
	-v /usr/share/ca-certificates-java:/usr/share/ca-certificates-java

  export PROXY_WORKAROUND2=--host_jvm_args -Djavax.net.ssl.trustStore=/etc/ssl/certs/java/cacerts
else
  export PROXY_WORKAROUND1=
  export PROXY_WORKAROUND2=
endif

# Set the included backends
WITH_TVM=0

ifeq (${WITH_TVM},1)
  ifneq (${TVM_SRC},)
    echo "INFO: TVM_SRC Configured"
    TVM_BACKEND=--//android/java/org/mlperf/inference:with_tvm="1"
    TVM_TARGET=//mobile_back_tvm:tvmbackend
    TVM_LIB_COPY=cp output/`readlink bazel-bin`/mobile_back_tvm/cpp/backend_tvm/libtvmbackend.so output/binary/libtvmbackend.so
    TVM_VERSION=1.0
    TVM_VOLUMES=-v ${TVM_SRC}:/home/mlperf/mobile_app/mobile_back_tvm/tvm/
  else
    echo "ERROR: TVM_SRC not set"
    TVM_BACKEND=--ERROR_TVM_SRC_NOT_SET
    TVM_VOLUMES=
    TVM_DEPS=
    TVM_TARGET=
    TVM_LIB_COPY=true
  endif
else
  TVM_BACKEND=
  TVM_VOLUMES=
  TVM_DEPS=
  TVM_TARGET=
  TVM_LIB_COPY=true
endif

