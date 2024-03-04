#include "pch.h" 
#include "scanner_Dll.h"
#include"scanner.h"

dir_x dir_data_find(node* root)		//������Ŀ¼�����Ѽ������Ϣ
{
	dir_x data;
	data.dir_path[0] = '\0';
	data.first_time_file[0] = data.last_time_file[0] = '\0';
	strcpy(data.dir_path, root->data.dir.path);
	data.file_num = 0;
	data.size = 0;
	//��������
	stack* _stack = new stack;
	_stack->num = 0;
	int i = 1;
	node* p;
	info_dir _info;
	p = root->right_child;
	FILETIME last, first;

	int first_file = 0;


	while (p)
	{
		if (p->left_brother != NULL)
			//��ջ
			in_stack(p->left_brother, _stack, i + 1);
		if (p->right_child != NULL)
		{
			i++;
			p = p->right_child;
			continue;
		}
		else if (p->type == 'f') {
			//Ϊ�ļ�
			data.file_num++;
			data.size += p->data.file.size;
			if (first_file == 0)
			{
				last = first = p->data.file.lastWriteTime;
				strcpy(data.last_time_file, p->data.file.path);
				strcpy(data.first_time_file, p->data.file.path);
			}

			else {
				if (CompareFileTime(&last, &(p->data.file.lastWriteTime)) == -1)
				{
					last = p->data.file.lastWriteTime;
					strcpy(data.last_time_file, p->data.file.path);
				}
				if (CompareFileTime(&first, &(p->data.file.lastWriteTime)) == 1)
				{
					first = p->data.file.lastWriteTime;
					strcpy(data.first_time_file, p->data.file.path);
				}
			}
			first_file++;
		}

		if (_stack->num > 0)
		{
			_info = out_stack(_stack);
			p = _info.dir;
			i = _info.tree_depth;
			continue;
		}
		else if (p->right_child == NULL && _stack->num == 0)
			break;
	}
	delete _stack;
	if (data.file_num == 0)
	{
		//�ļ��������ļ�
		data.size = 0;
		data.last_time = data.ear_time = root->data.dir.lastWriteTime;
		return data;
	}
	data.last_time = last;
	data.ear_time = first;
	return data;
}

dir_x dir_find(long long id, node* root)	//����Ŀ¼�����ҵ���Ŀ¼���ڽڵ�
{
	dir_x data;

	int i = 1;
	node* p;
	info_dir _info;
	p = root;
	node* dir_point = root;
	{
		stack* _stack = new stack;
		_stack->num = 0;
		while (true)
		{
			if (p->id == id)
			{
				dir_point = p;
				break;
			}
			if (p->left_brother != NULL)
				//��ջ
				in_stack(p->left_brother, _stack, i + 1);
			if (p->right_child != NULL)
			{
				i++;
				p = p->right_child;
				continue;
			}

			if (_stack->num > 0)
			{
				_info = out_stack(_stack);
				p = _info.dir;
				i = _info.tree_depth;
				continue;
			}
			else if (p->right_child == NULL && _stack->num == 0)
				break;
		}
		delete _stack;
	}

	data = dir_data_find(dir_point);
	return data;
}

dir_xs dirs_data(node* root, map<string, long long> mp)
{
	SYSTEMTIME* time = new SYSTEMTIME;
	dir_xs* dirs_data = new dir_xs;
	dirs_data->num = 0;

	//��ȡmystat�ļ�

	FILE* f = new FILE;
	f = fopen("mystat.txt", "r");
	FILE* p;
	p = fopen("info.txt", "w");
	char* str = new char[260];
	str[0] = '\0';


	while (fgets(str, 260, f))
	{
		if (strcmp(str, "stat dirs\n") == 0 || strcmp(str, "end of dirs\n") == 0)
			continue;
		//��ȡ����ļ���Ϣ
		str[strlen(str) - 1] = '\0';
		dirs_data->dirs_data[dirs_data->num] = dir_find(mp[str], root);
		dirs_data->num++;
		fprintf(p, "·����%s\n", dirs_data->dirs_data[dirs_data->num - 1].dir_path);
		fprintf(p, "�ļ���С��%lld\t", dirs_data->dirs_data[dirs_data->num - 1].size);

		fprintf(p, "�ļ�������%d\n", dirs_data->dirs_data[dirs_data->num - 1].file_num);

		FileTimeToSystemTime(&dirs_data->dirs_data[dirs_data->num - 1].ear_time, time);
		fprintf(p, "����ʱ�䣺%u-%u-%u %u:%u:%u\n", time->wYear, time->wMonth, time->wDay, time->wHour, time->wMinute, time->wSecond);
		FileTimeToSystemTime(&dirs_data->dirs_data[dirs_data->num - 1].last_time, time);
		fprintf(p, "����ʱ�䣺%u-%u-%u %u:%u:%u\n", time->wYear, time->wMonth, time->wDay, time->wHour, time->wMinute, time->wSecond);

	}
	fclose(f);
	delete time, f;
	fclose(p);
	return *dirs_data;
}


//Ѱ�Ҹ��ļ�
node* file_find(char* file_name, node* root, map<string, long long>* mp)
{
	//�ҵ����ļ���id
	int id;
	id = (*mp)[file_name];

	//��Ŀ¼����Ѱ�Ҹ��ļ�
	node* p;
	info_dir _info;
	p = root;
	{
		stack* _stack = new stack;
		_stack->num = 0;
		while (true)
		{



			if (p->id == id)
			{
				break;
			}
			if (p->left_brother != NULL)
				//��ջ
				in_stack(p->left_brother, _stack, 0);
			if (p->right_child != NULL)
			{
				p = p->right_child;
				continue;
			}

			if (_stack->num > 0)
			{
				_info = out_stack(_stack);
				p = _info.dir;
				continue;
			}
		}
		delete _stack;
	}
	return p;
}


uint64_t string_to_number(char* str)
{
	// �����ַ���ת����
	int i = strlen(str);
	uint64_t result = 0;
	for (int j = 0; j < i; j++)
		result = result * 10 + (str[j] - '0');

	return result;
}

void unixtime_to_filetime(uint64_t unixtime, FILETIME* filetime)
{
	//unixtimeתfiletime
	const uint64_t EPOCH_DIFF = 11644473600ULL;
	uint64_t filetime_low = (unixtime + EPOCH_DIFF) * 10000000ULL;
	filetime->dwLowDateTime = (DWORD)filetime_low;
	filetime->dwHighDateTime = (DWORD)(filetime_low >> 32);
}


// ��ȡ�ļ���Ŀ¼path
string get_parent_path(char* child_path_str)
{
	string parent_path;
	string child_path = child_path_str;
	int end = child_path.rfind('\\');
	parent_path = child_path.substr(0, end + 1);

	return parent_path;
}


//ɾ���ļ�����
void delete_file(char* file_name, node* root, map<string, long long>* mp)
{
	//�ҵ�Ҫɾ�����ļ����ڵ��ļ���
	string parent_path = get_parent_path(file_name);
	node* parent_dir;
	char parent_path_str[260] = { '\0' };
	strcpy(parent_path_str, parent_path.c_str());
	parent_dir = file_find(parent_path_str, root, mp);
	//���ҵ�Ҫɾ�����ļ�
	node* target;
	target = file_find(file_name, parent_dir, mp);


	//ִ��ɾ������
	if (parent_dir->right_child == target)
	{
		//Ŀ���ļ��պ�ΪĿ¼�µ�һ��
		parent_dir->right_child = target->left_brother;
	}
	else
	{
		node* p = parent_dir->right_child;
		while (p->left_brother != target)
			p = p->left_brother;
		//�ҵ�Ŀ���ļ���ǰһ��dir��file
		p->left_brother = target->left_brother;
	}
}

//ɾ���ļ���
void delete_dir(char* file_name, node* root, map<string, long long>* mp)
{
	//�ҵ�Ҫɾ�����ļ����ڵ��ļ���
	char string_1[260] = { '\0' };
	strcpy(string_1, file_name);
	string_1[strlen(string_1) - 1] = '\0';
	string parent_path = get_parent_path(string_1);
	node* parent_dir;
	char parent_path_str[260] = { '\0' };
	strcpy(parent_path_str, parent_path.c_str());
	parent_dir = file_find(parent_path_str, root, mp);
	//���ҵ�Ҫɾ�����ļ���
	node* target;
	target = file_find(file_name, parent_dir, mp);


	//ִ��ɾ������
	if (parent_dir->right_child == target)
	{
		//Ŀ���ļ��պ�ΪĿ¼�µ�һ��
		parent_dir->right_child = target->left_brother;
	}
	else
	{
		node* p = parent_dir->right_child;
		while (p->left_brother != target)
			p = p->left_brother;
		//�ҵ�Ŀ���ļ���ǰһ��dir��file
		p->left_brother = target->left_brother;
	}
}


//�޸��ļ�����
void update_file(char* file_name, node* root, map<string, long long>* mp, char* time_str, char* size_str)
{
	//��Ϣת��
	FILETIME time;
	unsigned long size;
	uint64_t time_unix;
	size = (unsigned long)string_to_number(size_str);
	time_unix = string_to_number(time_str);
	unixtime_to_filetime(time_unix, &time);

	//���Ҹ��ļ�
	node* target;
	target = file_find(file_name, root, mp);

	target->data.file.lastWriteTime = time;
	target->data.file.size = size;

}

//�����ļ�����
void insert_file(char* file_name, node* root, map<string, long long>* mp, char* time_str, char* size_str)
{
	//��Ϣת��
	FILETIME time;
	unsigned long size;
	uint64_t time_unix;
	size = (unsigned long)string_to_number(size_str);
	time_unix = string_to_number(time_str);
	unixtime_to_filetime(time_unix, &time);

	//�ҵ�Ҫ������ļ����ڵ��ļ���
	string parent_path = get_parent_path(file_name);
	node* parent_dir;
	char parent_path_str[260] = { '\0' };
	strcpy(parent_path_str, parent_path.c_str());
	parent_dir = file_find(parent_path_str, root, mp);

	//ִ�в������
	if (parent_dir->right_child == NULL)
	{
		//���ļ���Ϊ���ļ���
		node* new_file = new node;
		parent_dir->right_child = new_file;
		new_file->type = 'f';
		new_file->id = -1;
		new_file->depth = 0;
		new_file->left_brother = NULL;
		new_file->parent = parent_dir;
		new_file->right_child = NULL;
		new_file->data.file.size = size;
		new_file->data.file.lastWriteTime = time;
		new_file->data.file.path[0] = '\0';
		strcpy(new_file->data.file.path, file_name);
	}
	else {
		//���ļ������滹���ļ�
		node* p = parent_dir->right_child;
		while (p->left_brother != NULL)
			p = p->left_brother;
		node* new_file = new node;
		p->left_brother = new_file;
		new_file->type = 'f';
		new_file->id = -1;
		new_file->depth = 0;
		new_file->left_brother = NULL;
		new_file->parent = parent_dir;
		new_file->right_child = NULL;
		new_file->data.file.size = size;
		new_file->data.file.lastWriteTime = time;
		new_file->data.file.path[0] = '\0';
		strcpy(new_file->data.file.path, file_name);
	}
}

//��ȡ�ļ�������Ϣ����
function_data data_get(char* data)
{
	function_data data_1;
	string str = data;
	int begin = str.find(",");
	char function;
	string path = str.substr(0, begin);
	path[begin] = '\0';
	function = str[begin + 1];
	int last = str.rfind(",");
	string time = str.substr(begin + 3, last - begin - 3);
	string size = str.substr(last + 1);
	data_1.function = function;
	data_1.path[0] = data_1.size[0] = data_1.time[0] = '\0';
	strcpy(data_1.path, path.c_str());
	strcpy(data_1.size, size.c_str());
	data_1.size[strlen(data_1.size) - 1] = '\0';
	strcpy(data_1.time, time.c_str());

	return data_1;
}



//�ļ���������
void functon_file(node* root, map<string, long long>* mp, dir_xs* change)
{
	FILE* f;
	f = fopen("myfile.txt", "r");

	char str_1[260] = { '\0' };
	function_data data_file;


	while (fgets(str_1, 260, f))
	{
		if (strcmp(str_1, "selected files\n") == 0 || strcmp(str_1, "end of files\n") == 0)
			continue;
		data_file = data_get(str_1);
		if (data_file.function == 'M')
		{
			//�޸Ĳ���
			update_file(data_file.path, root, mp, data_file.time, data_file.size);
		}
		else if (data_file.function == 'A')
		{
			//���Ӳ���
			insert_file(data_file.path, root, mp, data_file.time, data_file.size);
		}
		else if (data_file.function == 'D')
		{
			delete_file(data_file.path, root, mp);
		}
	}

	*change = dirs_data(root, *mp);
}

//�ļ��в�������
void function_dir(node* root, map<string, long long>* mp, dir_xs* change)
{
	FILE* f;
	f = fopen("mydir.txt", "r");

	char str_1[260] = { '\0' };
	function_data data_dir;


	while (fgets(str_1, 260, f))
	{
		if (strcmp(str_1, "selected dirs\n") == 0 || strcmp(str_1, "end of dirs\n") == 0)
			continue;
		data_dir = data_get(str_1);
		//ɾ������
		delete_dir(data_dir.path, root, mp);
	}

	*change = dirs_data(root, *mp);
}


//�Ƚϲ���ǰ����죬������log��־



void compare(dir_xs* first, dir_xs* second,int a)
{
	FILE* f;
	f = fopen("myLOG.txt", "a+");
	FILE* p;
	if (a == 1)
		p = fopen("info.txt", "w");
	else
		p = fopen("info.txt", "a+");
	SYSTEMTIME time;
	int tag = 0;
	int different = 0;
	for (int i = 0; i < first->num; i++)
	{
		tag = 0;


		if (strcmp(first->dirs_data[i].dir_path, second->dirs_data[i].dir_path) != 0)
		{
			different++;
			//��Ŀ¼��ɾ����
			fprintf(f, "%d ):  Ŀ¼���ƣ� %s\n", i + 1, first->dirs_data[i].dir_path);
			fprintf(p, "%d ):  Ŀ¼���ƣ� %s\n", i + 1, first->dirs_data[i].dir_path);
			fprintf(f, "\t���ļ��������%d\n", 0 - first->dirs_data[i].file_num);
			fprintf(p, "\t���ļ��������%d\n", 0 - first->dirs_data[i].file_num);
			fprintf(f, "\t���ļ���С���%lld\n", 0 - first->dirs_data[i].size);
			fprintf(p, "\t���ļ���С���%lld\n", 0 - first->dirs_data[i].size);
			FileTimeToSystemTime(&first->dirs_data[i].ear_time, &time);
			fprintf(f, "\t����ʱ�����Ϊ��");
			fprintf(p, "\t����ʱ�����Ϊ��");
			fprintf(f, "%u-%u-%u %u:%u:%u\t", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
			fprintf(p, "%u-%u-%u %u:%u:%u\t", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
			FileTimeToSystemTime(&first->dirs_data[i].last_time, &time);
			fprintf(f, "\t����ʱ�����Ϊ��");
			fprintf(p, "\t����ʱ�����Ϊ��");
			fprintf(f, "%u-%u-%u %u:%u:%u\n\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
			fprintf(p, "%u-%u-%u %u:%u:%u\n\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		}
		else
		{
			//Ŀ���ļ��д���
			//�Ƚϲ���
			if (first->dirs_data[i].file_num != second->dirs_data[i].file_num)
			{
				tag = 1;
				fprintf(f, "%d ):  Ŀ¼���ƣ� %s\n", i + 1, first->dirs_data[i].dir_path);
				fprintf(p, "%d ):  Ŀ¼���ƣ� %s\n", i + 1, first->dirs_data[i].dir_path);
				fprintf(f, "\t���ļ��������%d\n", second->dirs_data[i].file_num - first->dirs_data[i].file_num);
				fprintf(p, "\t���ļ��������%d\n", second->dirs_data[i].file_num - first->dirs_data[i].file_num);

			}
			if (first->dirs_data[i].size != second->dirs_data[i].size)
			{
				if (tag == 0)
				{
					fprintf(f, "%d ):  Ŀ¼���ƣ� %s\n", i + 1, first->dirs_data[i].dir_path);
					fprintf(p, "%d ):  Ŀ¼���ƣ� %s\n", i + 1, first->dirs_data[i].dir_path);

				}

				tag = 1;
				fprintf(f, "\t���ļ���С���%lld\n", second->dirs_data[i].size - first->dirs_data[i].size);
				fprintf(p, "\t���ļ���С���%lld\n", second->dirs_data[i].size - first->dirs_data[i].size);

			}
			if (CompareFileTime(&first->dirs_data[i].ear_time, &second->dirs_data[i].ear_time) != 0)
			{
				if (tag == 0)
				{
					fprintf(f, "%d ):  Ŀ¼���ƣ� %s\n", i + 1, first->dirs_data[i].dir_path);
					fprintf(p, "%d ):  Ŀ¼���ƣ� %s\n", i + 1, first->dirs_data[i].dir_path);

				}

				tag = 1;
				FileTimeToSystemTime(&second->dirs_data[i].ear_time, &time);
				cout << "\t����ʱ�����Ϊ��";
				fprintf(f, "\t����ʱ�����Ϊ��");
				fprintf(p, "\t����ʱ�����Ϊ��");

				fprintf(f, "%u-%u-%u %u:%u:%u\t", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
				fprintf(p, "%u-%u-%u %u:%u:%u\t", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

				tag = 1;
			}
			if (CompareFileTime(&first->dirs_data[i].ear_time, &second->dirs_data[i].ear_time) != 0)
			{
				if (tag == 0)
				{
					fprintf(f, "%d ):  Ŀ¼���ƣ� %s\n", i + 1, first->dirs_data[i].dir_path);
					fprintf(p, "%d ):  Ŀ¼���ƣ� %s\n", i + 1, first->dirs_data[i].dir_path);

				}
				tag = 1;
				FileTimeToSystemTime(&second->dirs_data[i].ear_time, &time);
				fprintf(f, "\t����ʱ�����Ϊ��");
				fprintf(p, "\t����ʱ�����Ϊ��");

				fprintf(f, "%u-%u-%u %u:%u:%u\n\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
				fprintf(p, "%u-%u-%u %u:%u:%u\n\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

				tag = 1;
			}
			if (tag == 1)
			{

				different++;

			}
		}
		cout << endl;


	}
	fprintf(f, "--���Ƚ� %d ���ļ���, �� %d ���ļ����в���\n\n", first->num, different);
	fprintf(p, "--���Ƚ� %d ���ļ���, �� %d ���ļ����в���\n\n", first->num, different);


	cout << endl;
	fclose(f);
	fclose(p);
}