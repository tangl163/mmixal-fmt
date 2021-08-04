#ifndef _MMIXAL_FMT_INCLUDED_
#define _MMIXAL_FMT_INCLUDED_


#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>
#include <cctype>
#include <memory>

#define LABEL 1
#define OPCODE 2
#define OPRAND 4
#define COMMENT 8

#define PRIMARY_MASK 0xFF

#define IN_SINGLE_QUOTES (1<<8)
#define IN_DOUBLE_QUOTES (1<<9)

#define IN_QUOTES (IN_SINGLE_QUOTES | IN_DOUBLE_QUOTES)

class Format;
class Instruction;

static bool isLetter(char c);
static std::shared_ptr<std::vector<Instruction>> parse(std::string &line);

class Format {
    friend class Instruction;
public:
    Format(std::string sep = "  "): label(0), opcode(0), oprand(0), comment(0), sep(sep) {}
    void operator()(Instruction &mmixal);
private:
    std::string::size_type label, opcode, oprand, comment;
    std::string sep;
};

using Label = std::string;
using Opcode = std::string;
using Oprand = std::string;
using Comment = std::string;

class Instruction {
    friend class Format;
public:
    Instruction() = default;
    Instruction(const std::string &line, bool raw = false): raw(raw), line(line) {}
    Instruction(Label &label, Opcode &opcode, Oprand &oprand, Comment &comment):
        label(label), opcode(opcode), oprand(oprand), comment(comment) {}

    virtual std::string fmt(Format &format);
    virtual ~Instruction() = default;
private:
    bool raw = false;
    std::string line;
    Label label;
    Opcode opcode;
    Oprand oprand;
    Comment comment;
};

void
Format::operator()(Instruction &mmixal)
{
    if (mmixal.label.size() > label) {
        label = mmixal.label.size();
    }

    if (mmixal.opcode.size() > opcode) {
        opcode = mmixal.opcode.size();
    }

    if (mmixal.oprand.size() > oprand) {
        oprand = mmixal.oprand.size();
    }

    if (mmixal.comment.size() > comment) {
        comment = mmixal.comment.size();
    }
}

std::string
Instruction::fmt(Format &format)
{
    if (raw) {
        return line;
    }

    std::string str;

    str = label + std::string(format.label - label.size(), ' ') + format.sep + opcode;

    /* The OPRAND field can also be blank */
    if (oprand.size()) {
        str += std::string(format.opcode - opcode.size(), ' ') + format.sep + oprand;
    }

    if (comment.size()) {
        if (oprand.empty()) {
            str += format.sep + std::string(format.oprand, ' ') + format.sep + comment;
        } else {
            str += std::string(format.oprand - oprand.size(), ' ') + format.sep + comment;
        }
    }

    return str;
}


#endif /* _MMIXAL_FMT_INCLUDED_ */

