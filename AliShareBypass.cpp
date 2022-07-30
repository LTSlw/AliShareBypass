#include "AliShareBypass.h"

int _tmain(const int argc, const wchar_t* argv[]) {
	using namespace std;
	_wsetlocale(LC_ALL, L"zh-CN");

	wcout << L"AliShareBypass by LTSlw" << std::endl;
	wcout << L"Version: " << version << std::endl << std::endl;

	srand((unsigned int)time(0));
	for (int i = HEADER_LENGTH; i < BUF_LENGTH; i++) {
		header[i] = rand() % UCHAR_MAX;
	}

	if (1 == argc) {
		wcerr << L"错误的命令 输入\"AliShareBypass h\"查看帮助" << std::endl;
		return 1;
	}

	if (0 == wcscmp(L"e", argv[1])) {
		encodeMode = true;
	}
	else if (0 == wcscmp(L"d", argv[1])) {
		encodeMode = false;
	}
	else if (0 == wcscmp(L"h", argv[1])) {
		wcout << helpMsg << std::endl;
	}
	else {
		wcerr << L"错误的命令 输入\"AliShareBypass h\"查看帮助" << std::endl;
		return 1;
	}

	for (int i = 2; i < argc; ++i) {
		handlePath(formatPath(argv[i]));
	}

	wcout << std::endl << L"完成" << std::endl;
	return 0;
}

void handlePath(const std::wstring& path) {
	_wfinddata_t fileInfo;
	intptr_t handle = _wfindfirst(path.c_str(), &fileInfo);
	size_t slashPos = path.find_last_of(L"/\\");
	std::wstring parentDir = path.substr(0, slashPos + 1);
	if (-1 == handle) {
		_findclose(handle);
		return;
	}
	do {
		if (0 == wcscmp(L".", fileInfo.name) || 0 == wcscmp(L"..", fileInfo.name)) {
			continue;
		}

		if (fileInfo.attrib & _A_SUBDIR) {
			handlePath(parentDir + fileInfo.name + L"/*");
		}
		else {
			print(parentDir + fileInfo.name + L"\r");
			switch (handleFile(parentDir + fileInfo.name, fileInfo.size)) {
			case SUCCESS:
				print(L"[SUCCESS]" + parentDir + fileInfo.name + L"\n");
				break;
			case IO_ERROR:
				print(L"[IO ERROR]" + parentDir + fileInfo.name + L"\n");
				break;
			case RENAME_ERROR:
				print(L"[RENAME ERROR]" + parentDir + fileInfo.name + L"\n");
				break;
			}
		}
	} while (-1 != _wfindnext(handle, &fileInfo));
	_findclose(handle);
	return;
}

inline void print(const std::wstring& msg) {
	using namespace std;
	wcout << msg;
	if (!std::wcout) {
		std::wcout.clear();
		std::wcout << L"! 此条输出可能不完整" << std::endl;
	}
}

inline std::wstring formatPath(const std::wstring& path) {
	if (L'\\' != path[path.length() - 1] && L'/' != path[path.length() - 1]) {
		return path;
	}
	size_t pos = path.find_last_not_of(L"/\\");
	return path.substr(0, pos + 1);
}

inline int handleFile(const std::wstring& file, const _fsize_t size) {
	return encodeMode ? encodeFile(file) : decodeFile(file, size);
}

int encodeFile(const std::wstring& file) {
	std::fstream f;
	f.open(file, std::ios::in | std::ios::out | std::ios::binary);
	if (!f) {
		return IO_ERROR;
	}

	int fSize = 0;
	for (int i = 0; i < BUF_LENGTH; i++) {
		f.read((char*)(buf + i), sizeof(buf[i]));
		if (f.eof()) {
			f.clear();
			break;
		}
		fSize++;
	}
	f.seekp(0, std::ios::beg);
	f.write((char*)header, sizeof(header));
	f.seekp(0, std::ios::end);
	f.write((char*)buf, fSize);
	f.close();

	if (0 != _wrename(file.c_str(), (file + L"." + FILE_TYPE).c_str())) { //改变后缀名
		return RENAME_ERROR;
	}
	return SUCCESS;
}

int decodeFile(const std::wstring& file, const _fsize_t size) {
	std::fstream f;
	f.open(file, std::ios::in | std::ios::out | std::ios::binary);
	if (!f) {
		return IO_ERROR;
	}

	f.seekg(BUF_LENGTH, std::ios::beg);
	int fSize = 0;
	for (int i = 0; i < BUF_LENGTH; i++) {
		f.read((char*)(buf + i), sizeof(buf[i]));
		if (f.eof()) {
			f.clear();
			break;
		}
		fSize++;
	}
	if (fSize == BUF_LENGTH) {
		f.seekg(-BUF_LENGTH, std::ios::end);
		f.read((char*)buf, sizeof(buf));
	}
	f.seekp(0, std::ios::beg);
	f.write((char*)buf, fSize);
	f.close();
	std::filesystem::resize_file(file, size - BUF_LENGTH);

	size_t len = file.find_last_of(L".");
	if (0 != _wrename(file.c_str(), file.substr(0, len).c_str())) {
		return RENAME_ERROR;
	}
	return SUCCESS;
}