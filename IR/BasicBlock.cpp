#include "pch.h"

IR::Instruction IR::BasicBlock::GetTerminator() const {
    return instructions.back();
}

IR::BasicBlock::Iterator IR::BasicBlock::begin() const {
    return instructions.begin();
}

IR::BasicBlock::Iterator IR::BasicBlock::end() const {
    return instructions.end();
}
