#include "pge/ini.hpp"
#include "pge/debug.hpp"
#include "pge/file.hpp"
#include <iostream>
#include <locale>
#include <string>
#include <vector>

namespace pge::ini {

struct StructParameter {
    std::wstring name;
    std::wstring value;
};

struct StructSection {
    std::wstring name;
    std::vector<StructParameter> param;
};

struct StructINI {
    std::string path;
    std::vector<StructSection> section;
};

std::vector<StructINI> vINI;

uint8_t open(std::string path)
{
    std::cout << path << std::endl;
    std::wstring buffer;
    uint8_t sect;

    for (int i = 0; i < vINI.size(); i++) {
        if (vINI[i].path == path) {
            return i;
        }
    }

    StructINI flagINI;
    flagINI.path = path;

    while ((buffer = file::read_nextline(path)) != L"\n") {
        if (buffer[0] == L'[') {
            StructSection flag;
            for (int i = 1; i < buffer.length(); i++) {
                if (buffer[i] == L']')
                    break;
                flag.name += buffer[i];
            }
            flagINI.section.push_back(flag);
            sect = flagINI.section.size();

        } else if (flagINI.section.size() > 0 && buffer[0] != L' ' && buffer[0] != L';') {
            StructParameter flag;
            bool f = true;
            for (int i = 0; i < buffer.length(); i++) {
                if (buffer[i] == L'=') {
                    f = false;
                    i++;
                }
                if (f)
                    flag.name += buffer[i];
                else
                    flag.value += buffer[i];
            }
            flagINI.section[flagINI.section.size() - 1].param.push_back(flag);
        }
    }

    vINI.push_back(flagINI);
    return vINI.size() - 1;
}

uint8_t save(uint8_t ID, std::string path)
{
    std::wstring buffer;
    file::close();
    file::write(path, L"", 2);
    for (int i = 0; i < vINI[ID].section.size(); i++) {
        file::write(path, L"[" + vINI[ID].section[i].name + L"]\n", 1);
        for (int j = 0; j < vINI[ID].section[i].param.size(); j++) {
            file::write(path, vINI[ID].section[i].param[j].name + L"=" + vINI[ID].section[i].param[j].value + L"\n", 1);
        }
    }

    return 0;
}

uint8_t close(std::string path)
{
    return 0;
}

uint8_t close(uint8_t ID)
{
    return 0;
}

std::wstring read(uint8_t ID, std::wstring section, std::wstring parameter)
{
    uint8_t sect = (uint8_t)-1;
    for (int i = 0; i < vINI[ID].section.size(); i++) {
        if (vINI[ID].section[i].name == section) {
            sect = i;
            break;
        }
    }
    if (sect != (uint8_t)-1)
        for (int i = 0; i < vINI[ID].section[sect].param.size(); i++) {
            if (vINI[ID].section[sect].param[i].name == parameter) {
                return vINI[ID].section[sect].param[i].value;
            }
        }
    return L"";
}

int read_int(uint8_t ID, std::wstring section, std::wstring parameter)
{
    std::wstring buffer = ini::read(ID, section, parameter);
    bool flag = true;

    for (int i = 0; i < buffer.length(); i++) {
        if (!isdigit(buffer[i])) {
            flag = false;
            break;
        }
    }

    if (flag && buffer != L"")
        return stoi(buffer);
    else
        return flag;
}

bool write(uint8_t ID, std::wstring section, std::wstring parameter, std::wstring value)
{
    uint8_t sect = (uint8_t)-1;
    for (int i = 0; i < vINI[ID].section.size(); i++) {
        if (vINI[ID].section[i].name == section) {
            sect = i;
            break;
        }
    }
    if (sect == (uint8_t)-1) {
        StructSection newSect;
        newSect.name = section;
        StructParameter newParam;
        newParam.name = parameter;
        newParam.value = value;
        newSect.param.push_back(newParam);
        vINI[ID].section.push_back(newSect);
        return true;
    } else {
        for (int j = 0; j < vINI[ID].section[sect].param.size(); j++) {
            if (vINI[ID].section[sect].param[j].name == parameter) {
                vINI[ID].section[sect].param[j].value = value;
                return true;
            }
        }
        StructParameter newParam;
        newParam.name = parameter;
        newParam.value = value;
        vINI[ID].section[sect].param.push_back(newParam);
        return true;
    }
    return false;
}
}
