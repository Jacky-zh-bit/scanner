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
	// ���� 32 λ�͸� 32 λ��ϳ�һ�� 64 λ������
	return (static_cast<uint64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

time_t filetimeToUnixtime(const FILETIME& ft) {
	// �Ƚ� FILETIME ת��Ϊ uint64_t
	uint64_t n = filetimeToUint64(ft);
	// �ټ�ȥ EPOCH_DIFF�������� 10000000���õ�����
	return static_cast<time_t>((n - EPOCH_DIFF) / 10000000);
}
void dirsql_close(FILE* f)
{
	fclose(f);
}

//��б��ת˫б��
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