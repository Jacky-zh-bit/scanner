#include "pch.h" 
#include "scanner_Dll.h"
#include"scanner.h"

FILE* dirsql_open()
{
	FILE* file;
	file = fopen("dir_sql.sql", "wb");
	return file;
}

uint64_t filetimeToUint64(const FILETIME& ft) {
	// 将低 32 位和高 32 位组合成一个 64 位的整数
	return (static_cast<uint64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

time_t filetimeToUnixtime(const FILETIME& ft) {
	// 先将 FILETIME 转换为 uint64_t
	uint64_t n = filetimeToUint64(ft);
	// 再减去 EPOCH_DIFF，并除以 10000000，得到秒数
	return static_cast<time_t>((n - EPOCH_DIFF) / 10000000);
}
void dirsql_close(FILE* f)
{
	fclose(f);
}

//单斜杠转双斜杠
void pathConvert_Single2Double(string& s)
{
	int pos = 0;
	while ((pos = s.find('\\', pos)) != string::npos) {
		s.insert(pos, "\\");
		pos = pos + 2;
	}
}

void dir_sql_write(FILE* f, node* data_dir)
{
	string str = data_dir->data.dir.path;
	pathConvert_Single2Double(str);
	fprintf(f, "insert into mydir(pathname,pathID,upPathID) values(\"%s\",%lld,%lld);\n", str.c_str(), data_dir->id, data_dir->parent->id);
}


FILE* filesql_open()
{
	FILE* file;
	file = fopen("file_sql.sql", "wb");
	return file;
}

void filesql_close(FILE* f)
{
	fclose(f);
}


void file_sql_write(FILE* f, node* data_file)
{
	string str = data_file->data.dir.path;
	pathConvert_Single2Double(str);
	fprintf(f, "insert into myfile(pathname,file_size,file_time) values(\"%s\",%lu,%lld);\n", str.c_str(), data_file->data.file.size, filetimeToUnixtime((data_file->data.file.lastWriteTime)));
}