#include "pge/file.hpp"
//#include <boost/filesystem.hpp>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>
#include <vector>

namespace pge::file {

std::wfstream fileio;
std::string lastpath;

bool close()
{
    fileio.close();
    fileio.clear();
    return true;
}

std::wstring read_nextline(std::string path)
{
    std::wstring output = L"";

    if (path != lastpath) {
        close();
    }

    if (!fileio.is_open()) {
        fileio.open(path, std::ios::binary | std::ios::in | std::ios::out);
        lastpath = path;
        fileio.imbue(std::locale(fileio.getloc(),
            new std::codecvt_utf8<wchar_t, 0x10ffff, std::little_endian>));
    }

    if (fileio.is_open()) {
        if (!std::getline(fileio, output)) {
            return L"\n";
            fileio.seekg(0);
        }
    }
    return output;
}

std::wstring read(std::string path, int mode)
{
    std::wstring output = L"";

    if (path != lastpath)
        close();

    if (!fileio.is_open()) {
        lastpath = path;
        fileio.open(path, std::ios::binary | std::ios::in | std::ios::out);
        fileio.imbue(std::locale(fileio.getloc(),
            new std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>));
    }

    if (fileio.is_open()) {
        for (wchar_t c; fileio.get(c);) {
            output += c;
        }
        std::wcout << output << std::endl;
    }
    return output;
}

bool write(std::string path, std::wstring value, int mode)
{
    if (path != lastpath)
        close();

    if (!fileio.is_open())
        switch (mode) {
        default:
        case 0:
            fileio.open(path, std::ios::binary | std::ios::in | std::ios::out);
            break;
        case 1:
            fileio.open(path, std::ios::binary | std::ios::in | std::ios::out | std::ios::app);
            break;
        case 2:
            fileio.open(path, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
            break;
        };

    if (fileio.is_open()) {
        fileio.imbue(std::locale(fileio.getloc(),
            new std::codecvt_utf8<wchar_t, 0x10ffff, std::little_endian>));

        for (uint16_t i = 0; i < value.length(); i++) {
            fileio.put(value[i]);
        }
    }
    return true;
}

std::vector<std::string> get_folderlist(std::string path)
{
    std::vector<std::string> buffer;
    /*
    boost::filesystem::path p(path);
    for (auto i = boost::filesystem::directory_iterator(p); i != boost::filesystem::directory_iterator(); i++) {
        if (!boost::filesystem::is_directory(i->path())) {
            std::cout << i->path().filename().string() << std::endl;
            buffer.push_back(i->path().filename().string());
        } else
            continue;
    }
    */
    return buffer;
}
}
