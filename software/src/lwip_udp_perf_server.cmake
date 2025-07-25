# Copyright (C) 2023 Advanced Micro Devices, Inc.  All rights reserved.
# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.3)

list(APPEND TOTAL_MAC_INSTANCES ${EMACPS_NUM_DRIVER_INSTANCES})
list(APPEND TOTAL_MAC_INSTANCES ${AXIETHERNET_NUM_DRIVER_INSTANCES})
list(APPEND TOTAL_MAC_INSTANCES ${EMACLITE_NUM_DRIVER_INSTANCES})
SET(MAC_INSTANCES "${TOTAL_MAC_INSTANCES}" CACHE STRING "MAC Instances")
SET_PROPERTY(CACHE MAC_INSTANCES PROPERTY STRINGS "${TOTAL_MAC_INSTANCES}")
list(LENGTH TOTAL_MAC_INSTANCES _len)

if (${_len} GREATER 1)
    list(GET MAC_INSTANCES 0 MAC_INSTANCES)
elseif(${_len} EQUAL 0)
    message(FATAL_ERROR "This application requires an Ethernet MAC IP instance in the hardware.")
endif()

list(APPEND TOTAL_TIMER_INSTANCES ${TMRCTR_NUM_DRIVER_INSTANCES})
list(APPEND TOTAL_TIMER_INSTANCES ${TTCPS_NUM_DRIVER_INSTANCES})
list(LENGTH TOTAL_TIMER_INSTANCES _len)
if(${_len} EQUAL 0)
    message(FATAL_ERROR "There seems to be no timer peripheral in the hardware. lwIP requires an timer IP for TCP operations.")
endif()

set(index 0)
if (MAC_INSTANCES IN_LIST AXIETHERNET_NUM_DRIVER_INSTANCES)
    LIST_INDEX(${index} ${MAC_INSTANCES} "${AXIETHERNET_NUM_DRIVER_INSTANCES}")
    list(GET TOTAL_AXIETHERNET_PROP_LIST ${index} prop_list)
endif()

if (MAC_INSTANCES IN_LIST EMACLITE_NUM_DRIVER_INSTANCES)
    LIST_INDEX(${index} ${MAC_INSTANCES} "${EMACLITE_NUM_DRIVER_INSTANCES}")
    list(GET TOTAL_EMACLITE_PROP_LIST ${index} prop_list)
endif()

if (MAC_INSTANCES IN_LIST EMACPS_NUM_DRIVER_INSTANCES)
    LIST_INDEX(${index} ${MAC_INSTANCES} "${EMACPS_NUM_DRIVER_INSTANCES}")
    list(GET TOTAL_EMACPS_PROP_LIST ${index} prop_list)
endif()

set(y ${${prop_list}})
list(GET y 0 reg)
set(PLATFORM_EMAC_BASEADDR "${reg}")
configure_file(${CMAKE_SOURCE_DIR}/platform_/platform_config.h.in ${CMAKE_BINARY_DIR}/include/platform_config.h)
