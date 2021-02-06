/*
 * Copyright (c) 2009 The University of Edinburgh
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

#include "arch/power/insts/integer.hh"

using namespace PowerISA;

std::string
IntOp::generateDisassembly(Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;
    bool printDest = true;
    bool printSrcs = true;
    bool printSecondSrc = true;

    // Generate the correct mnemonic
    std::string myMnemonic(mnemonic);

    // Special cases
    if (!myMnemonic.compare("or") && srcRegIdx(0) == srcRegIdx(1)) {
        myMnemonic = "mr";
        printSecondSrc = false;
    } else if (myMnemonic == "mtcrf" ||
               myMnemonic == "mtxer" ||
               myMnemonic == "mtlr"  ||
               myMnemonic == "mtctr" ||
               myMnemonic == "mttar" ||
               myMnemonic == "cmpi") {
        printDest = false;
    } else if (myMnemonic == "mfcr"  ||
               myMnemonic == "mfxer" ||
               myMnemonic == "mflr"  ||
               myMnemonic == "mfctr" ||
               myMnemonic == "mftar") {
        printSrcs = false;
    }

    // Additional characters depending on isa bits being set
    if (oe)
        myMnemonic = myMnemonic + "o";
    if (rc)
        myMnemonic = myMnemonic + ".";
    ccprintf(ss, "%-10s ", myMnemonic);

    // Print the first destination only
    if (_numDestRegs > 0 && printDest)
        printReg(ss, destRegIdx(0));

    // Print the (possibly) two source registers
    if (_numSrcRegs > 0 && printSrcs) {
        if (_numDestRegs > 0 && printDest)
            ss << ", ";
        printReg(ss, srcRegIdx(0));
        if (_numSrcRegs > 1 && printSecondSrc) {
          ss << ", ";
          printReg(ss, srcRegIdx(1));
        }
    }

    return ss.str();
}


std::string
IntImmOp::generateDisassembly(Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    // Print the first destination only
    if (_numDestRegs > 0)
        printReg(ss, destRegIdx(0));

    // Print the source register
    if (_numSrcRegs > 0) {
        if (_numDestRegs > 0)
            ss << ", ";
        printReg(ss, srcRegIdx(0));
    }

    // Print the immediate value last
    ss << ", " << (int32_t)si;

    return ss.str();
}


std::string
IntArithOp::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;
    bool printSecondSrc = true;
    bool printThirdSrc = false;

    // Generate the correct mnemonic
    std::string myMnemonic(mnemonic);

    // Special cases
    if (myMnemonic == "addme" ||
        myMnemonic == "addze" ||
        myMnemonic == "subfme" ||
        myMnemonic == "subfze" ||
        myMnemonic == "neg") {
        printSecondSrc = false;
    } else if (myMnemonic == "maddhd" ||
               myMnemonic == "maddhdu" ||
               myMnemonic == "maddld") {
        printThirdSrc = true;
    }

    // Additional characters depending on isa bits being set
    if (oe)
        myMnemonic = myMnemonic + "o";
    if (rc)
        myMnemonic = myMnemonic + ".";
    ccprintf(ss, "%-10s ", myMnemonic);

    // Print the first destination only
    if (_numDestRegs > 0)
        printReg(ss, destRegIdx(0));

    // Print the first source register
    if (_numSrcRegs > 0) {
        if (_numDestRegs > 0)
            ss << ", ";
        printReg(ss, srcRegIdx(0));

        // Print the second source register
        if (_numSrcRegs > 1 && printSecondSrc) {
            ss << ", ";
            printReg(ss, srcRegIdx(1));

            // Print the third source register
            if (_numSrcRegs > 2 && printThirdSrc) {
                ss << ", ";
                printReg(ss, srcRegIdx(2));
            }
        }
    }

    return ss.str();
}


std::string
IntImmArithOp::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;
    bool negateImm = false;

    // Generate the correct mnemonic
    std::string myMnemonic(mnemonic);

    // Special cases
    if (myMnemonic == "addi") {
        if (_numSrcRegs == 0) {
            myMnemonic = "li";
        } else if (si < 0) {
            myMnemonic = "subi";
            negateImm = true;
        }
    } else if (myMnemonic == "addis") {
        if (_numSrcRegs == 0) {
            myMnemonic = "lis";
        } else if (si < 0) {
            myMnemonic = "subis";
            negateImm = true;
        }
    } else if (myMnemonic == "addic" && si < 0) {
        myMnemonic = "subic";
        negateImm = true;
    } else if (myMnemonic == "addic_") {
        if (si < 0) {
            myMnemonic = "subic.";
            negateImm = true;
        } else {
            myMnemonic = "addic.";
        }
    }

    ccprintf(ss, "%-10s ", myMnemonic);

    // Print the first destination only
    if (_numDestRegs > 0)
        printReg(ss, destRegIdx(0));

    // Print the source register
    if (_numSrcRegs > 0) {
        if (_numDestRegs > 0)
            ss << ", ";
        printReg(ss, srcRegIdx(0));
    }

    // Print the immediate value
    if (negateImm)
        ss << ", " << -si;
    else
        ss << ", " << si;

    return ss.str();
}


std::string
IntDispArithOp::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;
    bool printSrcs = true;
    bool printDisp = true;
    bool negateDisp = false;

    // Generate the correct mnemonic
    std::string myMnemonic(mnemonic);

    // Special cases
    if (myMnemonic == "addpcis") {
        printSrcs = false;
        if (d == 0) {
            myMnemonic = "lnia";
            printDisp = false;
        } else if (d < 0) {
            myMnemonic = "subpcis";
            negateDisp = true;
        }
    }

    ccprintf(ss, "%-10s ", myMnemonic);

    // Print the first destination only
    if (_numDestRegs > 0)
        printReg(ss, destRegIdx(0));

    // Print the source register
    if (_numSrcRegs > 0 && printSrcs) {
        if (_numDestRegs > 0)
            ss << ", ";
        printReg(ss, srcRegIdx(0));
    }

    // Print the displacement
    if (printDisp)
        ss << ", " << (negateDisp ? -d : d);

    return ss.str();
}


std::string
IntShiftOp::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    // Print the first destination only
    if (_numDestRegs > 0)
        printReg(ss, destRegIdx(0));

    // Print the first source register
    if (_numSrcRegs > 0) {
        if (_numDestRegs > 0)
            ss << ", ";
        printReg(ss, srcRegIdx(0));
    }

    // Print the shift
    ss << ", " << sh;

    return ss.str();
}


std::string
IntRotateOp::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    // Print the first destination only
    if (_numDestRegs > 0)
        printReg(ss, destRegIdx(0));

    // Print the first source register
    if (_numSrcRegs > 0) {
        if (_numDestRegs > 0)
            ss << ", ";
        printReg(ss, srcRegIdx(0));
    }

    // Print the shift, mask begin and mask end
    ss << ", " << sh << ", " << mb << ", " << me;

    return ss.str();
}
