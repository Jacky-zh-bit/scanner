#include "pch.h" 
#include "scanner_Dll.h"
#include"scanner.h"

//遍历目录树，获得：
//	系统目录中含有的子目录数量；
//	系统目录中含有的文件总数量；
//	目录的层数；
//	最长的带全路径的文件名及长度；
//	计算树的深度；
//	统计文件信息。最早时间、最晚时间；
//	总的文件大小；





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


int tree_deep(node* root)				//目录树深度 (DNS深度搜索)
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
			//入栈
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
			//入栈
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

