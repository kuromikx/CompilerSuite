#include "pch.h"

IR::BasicBlock::Iterator IR::BasicBlock::begin() const {
    return instructions.begin();
}

IR::BasicBlock::Iterator IR::BasicBlock::end() const {
    return instructions.end();
}
