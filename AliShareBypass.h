#pragma once
#include <iostream>
#include <tchar.h>
#include <clocale>
#include <string>
#include <io.h>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <random>
#include <ctime>

bool encodeMode = true;

#define IS_FOLDER    2
#define IS_FILE      1
#define INVALID_PATH 0

#define HEADER_LENGTH 2
#define BUF_LENGTH    4
#define FILE_TYPE     L"EXE"

//unsigned char header[BUF_LENGTH] = { 0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a }; //PNG
unsigned char header[BUF_LENGTH] = { 0x4d,0x5a };//exe
//unsigned char header[BUF_LENGTH] = { 0x41,0x56, 0x49, 0x20 };//avi
unsigned char buf[BUF_LENGTH] = {};

#define SUCCESS      0
#define IO_ERROR     1
#define RENAME_ERROR 2

std::wstring version = L"v0.2.0",
helpMsg =
LR"(AliShareBypass <command> <path> [<path>...]

<Commands>
e : 对文件编码，混淆绕过阿里云盘格式检查
d : 解码文件，恢复处理过的文件)";

/*
* encodeFile(): 解码文件，恢复处理过的文件
* file: 文件路径
* 
* retc:
* 0 SUCCESS      - 成功
* 1 IO_ERROR     - 文件IO错误
* 2 RENAME_ERROR - 文件重命名失败
*/
int encodeFile(const std::wstring& file);

/*
* decodeFile(): 对文件编码，混淆绕过阿里云盘格式检查
* file: 文件路径
* size: 文件大小
* 
* retc:
* 0 SUCCESS      - 成功
* 1 IO_ERROR     - 文件IO错误
* 2 RENAME_ERROR - 文件重命名失败
*/
int decodeFile(const std::wstring& file, const _fsize_t size);

/*
* handlePath(): 处理路径，路径可以指向文件或文件夹，支持通配符
* path: 路径
*/
void handlePath(const std::wstring& path);

/*
* handleFile: 处理文件
* file: 文件路径
* size: 文件大小
* 
* retc: 与decodeFile()和encodeFile()相同
*/
inline int handleFile(const std::wstring& file, const _fsize_t size);

inline void print(const std::wstring& msg);

/*
* formatPath(): 去掉路径末尾的'\\'和'/'
* path: 路径
* 
* retc: 处理后的路径
*/
inline std::wstring formatPath(const std::wstring& path);