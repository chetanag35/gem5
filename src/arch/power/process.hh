/*
 * Copyright (c) 2007-2008 The Florida State University
 * Copyright (c) 2009 The University of Edinburgh
 * Copyright (c) 2021 IBM Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __POWER_PROCESS_HH__
#define __POWER_PROCESS_HH__

#include "sim/process.hh"

namespace Loader
{
class ObjectFile;
} // namespace Loader;

class PowerProcess : public Process
{
  protected:
    void initState() override;

  public:
    PowerProcess(const ProcessParams &params, ::Loader::ObjectFile *objFile);

    template <typename IntType>
    void argsInit(int pageSize);
};

enum PowerHWCAPFeature {
    PPC_FEATURE_32 = 1ULL << 31,            // Always set for powerpc64
    PPC_FEATURE_64 = 1ULL << 30,            // Always set for powerpc64
    PPC_FEATURE_HAS_ALTIVEC = 1ULL << 28,
    PPC_FEATURE_HAS_FPU = 1ULL << 27,
    PPC_FEATURE_HAS_MMU = 1ULL << 26,
    PPC_FEATURE_UNIFIED_CACHE = 1ULL << 24,
    PPC_FEATURE_NO_TB = 1ULL << 20,         // 601/403gx have no timebase
    PPC_FEATURE_POWER4 = 1ULL << 19,        // POWER4 ISA 2.00
    PPC_FEATURE_POWER5 = 1ULL << 18,        // POWER5 ISA 2.02
    PPC_FEATURE_POWER5_PLUS = 1ULL << 17,   // POWER5+ ISA 2.03
    PPC_FEATURE_CELL_BE = 1ULL << 16,       // CELL Broadband Engine
    PPC_FEATURE_BOOKE = 1ULL << 15,         // ISA Category Embedded
    PPC_FEATURE_SMT = 1ULL << 14,           // Simultaneous Multi-Threading
    PPC_FEATURE_ICACHE_SNOOP = 1ULL << 13,
    PPC_FEATURE_ARCH_2_05 = 1ULL << 12,     // ISA 2.05
    PPC_FEATURE_PA6T = 1ULL << 11,          // PA Semi 6T Core
    PPC_FEATURE_HAS_DFP = 1ULL << 10,       // Decimal FP Unit
    PPC_FEATURE_POWER6_EXT = 1ULL << 9,     // P6 + mffgpr/mftgpr
    PPC_FEATURE_ARCH_2_06 = 1ULL << 8,      // ISA 2.06
    PPC_FEATURE_HAS_VSX = 1ULL << 7,        // P7 Vector Extension
    PPC_FEATURE_PSERIES_PERFMON_COMPAT = 1ULL << 6,
    PPC_FEATURE_TRUE_LE = 1ULL << 1,
    PPC_FEATURE_PPC_LE = 1ULL << 0
};

enum PowerHWCAP2Feature {
    PPC_FEATURE2_ARCH_2_07 = 1ULL << 31,    // ISA 2.07
    PPC_FEATURE2_HAS_HTM = 1ULL << 30,      // Hardware Transactional Memory
    PPC_FEATURE2_HAS_DSCR = 1ULL << 29,     // Data Stream Control Register
    PPC_FEATURE2_HAS_EBB = 1ULL << 28,      // Event Base Branching
    PPC_FEATURE2_HAS_ISEL = 1ULL << 27,     // Integer Select
    PPC_FEATURE2_HAS_TAR = 1ULL << 26,      // Target Address Register
    PPC_FEATURE2_HAS_VCRYPTO = 1ULL << 25,  // Vector AES category
    PPC_FEATURE2_HTM_NOSC = 1ULL << 24,
    PPC_FEATURE2_ARCH_3_00 = 1ULL << 23,    // ISA 3.0
    PPC_FEATURE2_HAS_IEEE128 = 1ULL << 22,  // VSX IEEE Binary Float 128-bit
};

#endif // __POWER_PROCESS_HH__

