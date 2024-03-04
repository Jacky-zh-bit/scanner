#include "pch.h" 
#include "scanner_Dll.h"
#include"scanner.h"

//����Ŀ¼������ã�
//	ϵͳĿ¼�к��е���Ŀ¼������
//	ϵͳĿ¼�к��е��ļ���������
//	Ŀ¼�Ĳ�����
//	��Ĵ�ȫ·�����ļ��������ȣ�
//	����������ȣ�
//	ͳ���ļ���Ϣ������ʱ�䡢����ʱ�䣻
//	�ܵ��ļ���С��





void in_stack(node* dir, stack* _stack, int i)
{
	_stack->stack_dir[_stack->num].dir = dir;
	_stack->stack_dir[_stack->num].tree_depth = i;
	_stack->num++;
}

info_dir out_stack(stack* _stack)
{
	info_dir dir_info;
	dir_info.dir = _stack->stack_dir[_stack->num - 1].dir;
	dir_info.tree_depth = _stack->stack_dir[_stack->num - 1].tree_depth;
	_stack->num--;
	return dir_info;
}


int tree_deep(node* root)				//Ŀ¼����� (DNS�������)
{
	stack* _stack = new stack;
	_stack->num = 0;
	int depth = 1, i = 1;
	node* p;
	info_dir _info;
	p = root;

	while (true)
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
		else {
			if (depth < i)
				depth = i;
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
	return depth;
}


int dir_deep(node* root, char* path)
{
	stack* _stack = new stack;
	_stack->num = 0;
	int depth = 0, i;
	node* p, * q;
	info_dir _info;
	p = q = root;
	int path_len = strlen(q->data.dir.path);


	while (true)
	{
		i = p->depth;
		if (p->left_brother != NULL)
			//��ջ
			in_stack(p->left_brother, _stack, i);
		if (p->right_child != NULL)
		{
			p = p->right_child;
			continue;
		}
		else {
			if (depth < i)
			{
				depth = i;
			}
			if (p->type == 'f')
			{
				if (strlen(p->data.file.path) > path_len)
				{
					q = p;
					path_len = strlen(p->data.file.path);
				}
			}
			else {
				if (strlen(p->data.dir.path) > path_len)
				{
					q = p;
					path_len = strlen(p->data.dir.path);
				}
			}

		}
		if (_stack->num > 0)
		{
			_info = out_stack(_stack);
			p = _info.dir;
			continue;
		}
		else if (p->right_child == NULL && _stack->num == 0)
			break;
	}
	if (q->type == 'f')
		strcpy(path, q->data.file.path);
	else
		strcpy(path, q->data.dir.path);


	delete _stack;
	return depth;
}

