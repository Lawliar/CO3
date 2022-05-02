//
// Created by charl on 4/29/2022.
//

#include "Messages.h"
#include <assert.h>

ControlMessgaes::ControlMessgaes(MessageType t, unsigned int id): Messages(t),id(id){
    assert(t == BasicBlockMsg || t == CallMsg || t == RetMsg);
}