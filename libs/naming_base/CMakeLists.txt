# Copyright (c) 2019 The STE||AR-Group
#
# SPDX-License-Identifier: BSL-1.0
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# Default location is $HPX_ROOT/libs/naming_base/include
set(naming_base_headers
  hpx/naming_base.hpp
  )

# Default location is $HPX_ROOT/libs/naming_base/include_compatibility
set(naming_base_compat_headers)

set(naming_base_sources)

include(HPX_AddModule)
add_hpx_module(naming_base
  COMPATIBILITY_HEADERS OFF
  DEPRECATION_WARNINGS
  FORCE_LINKING_GEN
  GLOBAL_HEADER_GEN OFF
  HEADERS ${naming_base_headers}
  DEPENDENCIES
    hpx_config
  CMAKE_SUBDIRS examples tests
)
