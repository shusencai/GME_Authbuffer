/*
 * platform.h
 *
 *  Created on: 2017年3月28日
 *      Author: XELEMENTLIU
 */

#ifndef INCLUDE_BASE_PLATFORM_H_
#define INCLUDE_BASE_PLATFORM_H_

#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef override
#define override
#endif

#ifdef _WIN32
#define  DECLSPEC __declspec(dllexport)
#else
#define  DECLSPEC
#endif


#endif /* INCLUDE_BASE_PLATFORM_H_ */
