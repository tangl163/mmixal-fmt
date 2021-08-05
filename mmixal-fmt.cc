#include "mmixal-fmt.h"

int
main(void)
{
    std::istream &input = std::cin;
    std::ostream &output = std::cout;

    std::string line;
    Format format;
    auto instructions = std::make_shared<std::vector<Instruction>>();

    while (std::getline(input, line)) {
        auto ins = parse(line);
        for (auto &tmp : *ins) {
            format(tmp);
            instructions->push_back(tmp);
        }
    }

    for (auto &mmixal : *instructions) {
        output << mmixal.fmt(format) << std::endl;
    }

    output.flush();

    return 0;
}

static std::shared_ptr<std::vector<Instruction>>
parse(std::string &line)
{
    int flags = LABEL;
    auto ins = std::make_shared<std::vector<Instruction>>();

    Label label;
    Opcode opcode;
    Oprand oprand;
    Comment comment;

    int pos = line.size() - 1;

    // Right trim
    while (std::isspace(line[pos]) && pos >= 0) {
        --pos;
    }

    // The whole line is blank
    if (line.empty() || pos < 0) {
        ins->emplace_back("", true);
        goto Done;
    } else {
        line.erase(pos+1);
    }

    for (decltype(line.size()) i = 0; i < line.size(); ++i) {
        switch (flags & PRIMARY_MASK) {
        case LABEL:
            if (std::isspace(line[i])) {
                flags = (flags & ~LABEL) | OPCODE;
            } else {
                // The whole line is a comment if the first nonblank character is
                // not a letter or digit
                if (label.empty() && !isLetter(line[i]) && !std::isdigit(line[i])) {
                    ins->emplace_back(std::string(line, i, line.size()-i), true);
                    goto Done;
                }

                label.push_back(line[i]);
            }
            break;
        case OPCODE:
            if (std::isspace(line[i])) {
                if (!opcode.empty()) {
                    flags = (flags & ~OPCODE) | OPRAND;
                }
            } else {
                // As the label field might be empty, we need to test this condition
                // on opcode field as well. Since every instruction must have at least
                // one opcode.
                if (label.empty() && opcode.empty() && !isLetter(line[i]) && !std::isdigit(line[i])) {
                    ins->emplace_back(std::string(line, i, line.size()-i), true);
                    goto Done;
                }

                // Convert all opcode to uppercase
                opcode.push_back(std::toupper(line[i]));
            }
            break;
        case OPRAND:
            if (std::isspace(line[i])) {
                if (oprand.empty()) {
                    break;
                }

                if (flags & IN_QUOTES) {
                    oprand.push_back(line[i]);
                } else {
                    flags = (flags & ~OPRAND) | COMMENT;
                }
            } else {
                // Comment must begin with "//" or "%", if oprand is blank
                if (oprand.empty() && ((line[i] == '/' && line[i+1] == '/') || line[i] == '%')) {
                    --i;
                    flags = (flags & ~OPRAND) | COMMENT;
                    break;
                }

                if (line[i] == '"') {
                    if (flags & IN_QUOTES) {
                        if (flags & IN_DOUBLE_QUOTES) {
                            flags &= ~IN_DOUBLE_QUOTES;
                        }
                    } else {
                        flags |= IN_DOUBLE_QUOTES;
                    }
                } else if (line[i] == '\'') {
                    if (flags & IN_QUOTES) {
                        if (flags & IN_SINGLE_QUOTES) {
                            flags &= ~IN_SINGLE_QUOTES;
                        }
                    } else {
                        flags |= IN_SINGLE_QUOTES;
                    }
                } else if (line[i] == ';') {
                    // The end of oprand
                    if (!(flags & IN_QUOTES)) {
                        --i;
                        flags = (flags & ~OPRAND) | COMMENT;
                        break;
                    }
                }

                oprand.push_back(line[i]);
            }
            break;
        case COMMENT:
            if (comment.empty()) {
                if (std::isspace(line[i])) {
                    break;
                }

                // Two or more instructions on a single line
                if (line[i] == ';') {
                    ins->emplace_back(label, opcode, oprand, comment);
                    flags = LABEL;
                    label.clear();
                    opcode.clear();
                    oprand.clear();
                    comment.clear();
                } else {
                    comment.push_back(line[i]);
                }
            } else {
                comment.push_back(line[i]);
            }
            break;
        }
    }

    ins->emplace_back(label, opcode, oprand, comment);

Done:
    return ins;
}

static bool
isLetter(char c)
{
    // As the definition of *letter*, please refer to
    // _MMIXware A RISC Computer for the Third Millennium_ MMIX 7
    if (std::isalpha(c) || c == ':' || c == '_' || c > 126) {
        return true;
    }

    return false;
}

