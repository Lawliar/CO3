//
// Created by charl on 4/29/2022.
//

#ifndef SYMBACKEND_MESSAGES_H
#define SYMBACKEND_MESSAGES_H



class Message {
public:
    typedef enum _MessageType{
        BasicBlockMsg,
        CallMsg,
        RetMsg,
        RuntimeDataMsg,
        SymbolizeMsg,
    } MessageType;
    MessageType type;
    Message(MessageType t): type(t){}
};

class ControlMessgaes : public Message{
public:
    ControlMessgaes(MessageType t, unsigned int id);
    unsigned id;
};

class RuntimeDataMessage: public Message

#endif //SYMBACKEND_MESSAGES_H
