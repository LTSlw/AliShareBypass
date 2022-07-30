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
e : ���ļ����룬�����ƹ��������̸�ʽ���
d : �����ļ����ָ���������ļ�)";

/*
* encodeFile(): �����ļ����ָ���������ļ�
* file: �ļ�·��
* 
* retc:
* 0 SUCCESS      - �ɹ�
* 1 IO_ERROR     - �ļ�IO����
* 2 RENAME_ERROR - �ļ�������ʧ��
*/
int encodeFile(const std::wstring& file);

/*
* decodeFile(): ���ļ����룬�����ƹ��������̸�ʽ���
* file: �ļ�·��
* size: �ļ���С
* 
* retc:
* 0 SUCCESS      - �ɹ�
* 1 IO_ERROR     - �ļ�IO����
* 2 RENAME_ERROR - �ļ�������ʧ��
*/
int decodeFile(const std::wstring& file, const _fsize_t size);

/*
* handlePath(): ����·����·������ָ���ļ����ļ��У�֧��ͨ���
* path: ·��
*/
void handlePath(const std::wstring& path);

/*
* handleFile: �����ļ�
* file: �ļ�·��
* size: �ļ���С
* 
* retc: ��decodeFile()��encodeFile()��ͬ
*/
inline int handleFile(const std::wstring& file, const _fsize_t size);

inline void print(const std::wstring& msg);

/*
* formatPath(): ȥ��·��ĩβ��'\\'��'/'
* path: ·��
* 
* retc: ������·��
*/
inline std::wstring formatPath(const std::wstring& path);