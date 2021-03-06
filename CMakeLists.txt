# Copyright 2013 Edwin van Leeuwen.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 2.6)
project(bson_stream)

# Can be used by youcompleteme to lookup compile information
SET (CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET (CMAKE_CXX_FLAGS "-std=c++11 -O2 -march=native -Wall ${CMAKE_CXX_FLAGS}")

include_directories("include")
include_directories("/usr/include/mongo")

find_library( MONGO mongoclient )

set(BOOST_LIBS thread system filesystem )
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)

SET(LIBS "${MONGO};${Boost_LIBRARIES}")


SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
install (FILES include/bson/bson_stream.hh
	DESTINATION include/bson)

# Tests
if (CMAKE_BUILD_TYPE MATCHES Debug)
	find_package(CxxTest)
	if(CXXTEST_FOUND)
		if(NOT MONGO)
			message( WARNING "Could not find mongoclient library. 
			Will not compile the tests" )
		endif()


		if (MONGO)
			enable_testing()
			CXXTEST_ADD_TEST(unittest_stream_out test_stream_out.cc
				include/bson/bson_stream.hh
				${CMAKE_CURRENT_SOURCE_DIR}/tests/test_stream_out.hh)
			target_link_libraries( unittest_stream_out ${LIBS}; )

			CXXTEST_ADD_TEST(unittest_stream_in test_stream_in.cc
				include/bson/bson_stream.hh
				${CMAKE_CURRENT_SOURCE_DIR}/tests/test_stream_in.hh)
			target_link_libraries( unittest_stream_in ${LIBS}; )
		endif()
	endif()
endif()
