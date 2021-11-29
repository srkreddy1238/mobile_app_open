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

libtvmbackend: docker_image
	@echo "Building libtvmbackend"
	@mkdir -p output/home/mlperf/cache && chmod 777 output/home/mlperf/cache
	@mkdir -p output/mobile_back_tvm
	@docker run \
		${COMMON_DOCKER_FLAGS} \
		--config android_arm64 //mobile_back_tvm:tvmbackend
	@cp output/`readlink bazel-bin`/mobile_back_tvm/cpp/backend_tvm/libtvmbackend.so output/mobile_back_tvm/libttvmbackend.so
