#include "pch.h" 
#include "scanner_Dll.h"
#include"scanner.h"

char input_data(WIN32_FIND_DATA find_file_data, node* p, node* q)			//��¼�ļ���Ϣ���ļ��з���'d',�ļ�����'f'
{

	//¼��ڵ���Ϣ
	// �ϼ�Ŀ¼
	q->parent = p;
	//���
	q->depth = p->depth + 1;
	q->left_brother = NULL;
	q->right_child = NULL;
	//�ж��ļ�����
	if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		//Ϊ�ļ���
	{
		q->type = 'd';
		strcpy(q->data.dir.path, p->data.dir.path);
		strcat(q->data.dir.path, find_file_data.cFileName);
		strcat(q->data.dir.path, "\\");
		q->data.dir.lastWriteTime = find_file_data.ftLastWriteTime;
		return q->type;
	}
	else {
		//Ϊ�ļ�
		q->type = 'f';
		strcpy(q->data.dir.path, p->data.dir.path);
		strcat(q->data.dir.path, find_file_data.cFileName);
		q->data.file.lastWriteTime = find_file_data.ftLastWriteTime;
		//�ļ���С
		q->data.file.size = ((unsigned long)find_file_data.nFileSizeHigh * (MAXDWORD + 1)) + find_file_data.nFileSizeLow;
		return q->type;
	}

}

void tree_build(node* root, long long* e, long long* f, FILE* dir_sql, FILE* file_sql, long long* size, map <string, long long>* mp)				//����Ŀ¼��
{
	WIN32_FIND_DATA find_file_data;
	HANDLE			hfind;

	//Ŀ¼����·��
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
			if (i == 0)					//�������ӽ��
			{
				i++;
				node* p = new node;
				j->right_child = p;

				if (input_data(find_file_data, root, p) == 'd')
				{
					//Ϊ�ļ���
					(*e)++;
					p->id = *e + *f;
					dir_sql_write(dir_sql, p);
					(*mp)[p->data.dir.path] = p->id;
					tree_build(p, e, f, dir_sql, file_sql, size, mp);
				}
				else
				{
					//Ϊ�ļ�
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
				//�������ֵܽڵ�
				node* p = new node;
				j->left_brother = p;
				if (input_data(find_file_data, root, p) == 'd')
				{
					//Ϊ�ļ���
					(*e)++;
					p->id = *e + *f;
					dir_sql_write(dir_sql, p);
					(*mp)[p->data.dir.path] = p->id;
					tree_build(p, e, f, dir_sql, file_sql, size, mp);
				}
				else
				{
					//Ϊ�ļ�
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
