#include "pch.h" 
#include "scanner_Dll.h"
#include"scanner.h"

char input_data(WIN32_FIND_DATA find_file_data, node* p, node* q)			//记录文件信息，文件夹返回'd',文件返回'f'
{

	//录入节点信息
	// 上级目录
	q->parent = p;
	//深度
	q->depth = p->depth + 1;
	q->left_brother = NULL;
	q->right_child = NULL;
	//判断文件类型
	if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		//为文件夹
	{
		q->type = 'd';
		strcpy(q->data.dir.path, p->data.dir.path);
		strcat(q->data.dir.path, find_file_data.cFileName);
		strcat(q->data.dir.path, "\\");
		q->data.dir.lastWriteTime = find_file_data.ftLastWriteTime;
		return q->type;
	}
	else {
		//为文件
		q->type = 'f';
		strcpy(q->data.dir.path, p->data.dir.path);
		strcat(q->data.dir.path, find_file_data.cFileName);
		q->data.file.lastWriteTime = find_file_data.ftLastWriteTime;
		//文件大小
		q->data.file.size = ((unsigned long)find_file_data.nFileSizeHigh * (MAXDWORD + 1)) + find_file_data.nFileSizeLow;
		return q->type;
	}

}

void tree_build(node* root, long long* e, long long* f, FILE* dir_sql, FILE* file_sql, long long* size, map <string, long long>* mp)				//构建目录树
{
	WIN32_FIND_DATA find_file_data;
	HANDLE			hfind;

	//目录所处路径
	char path[260 * 4];
	path[0] = '\0';
	strcpy(path, root->data.dir.path);
	strcat(path, "\\*.*");
	LPCTSTR way = path;
	node* j;
	j = root;

	//cout<<root->data.dir.path<<endl;

	hfind = FindFirstFile(way, &find_file_data);
	if (hfind == INVALID_HANDLE_VALUE)
	{
		//cout << GetLastError() << endl;

	}
	else {
		for (int i = 0; FindNextFile(hfind, &find_file_data);) {
			if (find_file_data.cFileName[0] == '.')
				continue;
			if (i == 0)					//放入左孩子结点
			{
				i++;
				node* p = new node;
				j->right_child = p;

				if (input_data(find_file_data, root, p) == 'd')
				{
					//为文件夹
					(*e)++;
					p->id = *e + *f;
					dir_sql_write(dir_sql, p);
					(*mp)[p->data.dir.path] = p->id;
					tree_build(p, e, f, dir_sql, file_sql, size, mp);
				}
				else
				{
					//为文件
					(*f)++;
					p->id = *e + *f;
					file_sql_write(file_sql, p);
					(*mp)[p->data.file.path] = p->id;
					*size += p->data.file.size;
				}
				j = j->right_child;

			}
			else
			{
				//放入右兄弟节点
				node* p = new node;
				j->left_brother = p;
				if (input_data(find_file_data, root, p) == 'd')
				{
					//为文件夹
					(*e)++;
					p->id = *e + *f;
					dir_sql_write(dir_sql, p);
					(*mp)[p->data.dir.path] = p->id;
					tree_build(p, e, f, dir_sql, file_sql, size, mp);
				}
				else
				{
					//为文件
					(*f)++;
					p->id = *e + *f;
					file_sql_write(file_sql, p);
					(*mp)[p->data.file.path] = p->id;
					*size += p->data.file.size;
				}
				j = j->left_brother;
			}
		}
	}

	FindClose(hfind);

}
