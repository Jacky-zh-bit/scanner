#include "pch.h" 
#include "scanner_Dll.h"
#include "scanner.h"



//日志记录

void log_write(long long dirN, long long fileN, int Maxdepth, char* path, int tree_depth, long long size)
{
	FILE* log;
	log = fopen("myLOG.txt", "w");
	fprintf(log, "目录：%s\n", INIT_PATH);
	fprintf(log, "dirN=%lld\tfileN=%lld\tAll=%lld\tMaxdepth=%d\n", dirN, fileN, dirN + fileN, Maxdepth);
	fprintf(log, "最长路径：%s\t", path);
	fprintf(log, "长度为：%d\n", strlen(path));
	fprintf(log, "目录树深度：%d\t\t", tree_depth);
	fprintf(log, "文件总大小：%lld bytes\n", size);
	fprintf(log, "——————————————————————————————\n");
	fclose(log);
}


void file_fun()
{
	//文件操作函数

	//根节点初始化
	map<string, long long> mp;
	char Maxpath[260];
	Maxpath[0] = '\0';
	long long size = 0;
	node root;
	root.parent = NULL;
	root.right_child = NULL;
	root.left_brother = NULL;
	root.depth = 1;
	root.type = 'd';
	root.id = 0;
	root.data.dir.path[0] = '\0';
	strcpy(root.data.dir.path, INIT_PATH);
	long long dirN = 0;
	long long fileN = 0;
	FILE* dir_sql;
	FILE* file_sql;
	FILE* f;
	dir_xs* dir_init = new dir_xs;
	dir_xs* change_file = new dir_xs;
	int depth = 0;
	int dir_depth = 0;

	//建立目录树

	dir_sql = dirsql_open();
	file_sql = filesql_open();
	tree_build(&root, &dirN, &fileN, dir_sql, file_sql, &size, &mp);
	dirsql_close(dir_sql);
	filesql_close(file_sql);
	depth = tree_deep(&root);
	dir_depth = dir_deep(&root, Maxpath);
	//统计文件信息
	log_write(dirN, fileN, dir_depth, Maxpath, depth, size);
	*dir_init = dirs_data(&root, mp);
	//文件操作
	functon_file(&root, &mp, change_file);
	compare(dir_init, change_file,1);
}
void dir_fun()
{
	//文件夹操作
	//建立节点树
	//根节点初始化
	map<string, long long> mp;
	char Maxpath[260];
	Maxpath[0] = '\0';
	long long size = 0;
	node root;
	root.parent = NULL;
	root.right_child = NULL;
	root.left_brother = NULL;
	root.depth = 1;
	root.type = 'd';
	root.id = 0;
	root.data.dir.path[0] = '\0';
	strcpy(root.data.dir.path, INIT_PATH);
	long long dirN = 0;
	long long fileN = 0;
	FILE* dir_sql;
	FILE* file_sql;
	FILE* f;
	dir_xs* dir_init = new dir_xs;
	dir_xs* change_file = new dir_xs;
	dir_xs* change_dir = new dir_xs;
	int depth = 0;
	int dir_depth = 0;

	//文件夹操作
		//建立目录树

	dir_sql = dirsql_open();
	file_sql = filesql_open();
	tree_build(&root, &dirN, &fileN, dir_sql, file_sql, &size, &mp);
	dirsql_close(dir_sql);
	filesql_close(file_sql);
	depth = tree_deep(&root);
	dir_depth = dir_deep(&root, Maxpath);
	log_write(dirN, fileN, dir_depth, Maxpath, depth, size);
	*dir_init = dirs_data(&root, mp);
	//文件操作
	functon_file(&root, &mp, change_file);
	compare(dir_init, change_file,1);
	function_dir(&root, &mp, change_dir);
	compare(change_file, change_dir,1);
	compare(dir_init, change_dir,0);
}




int function_two(int function)
{
	//建立节点树
	//根节点初始化
	map<string, long long> mp;
	char Maxpath[260];
	Maxpath[0] = '\0';
	long long size = 0;
	node root;
	root.parent = NULL;
	root.right_child = NULL;
	root.left_brother = NULL;
	root.depth = 1;
	root.type = 'd';
	root.id = 0;
	root.data.dir.path[0] = '\0';
	strcpy(root.data.dir.path, INIT_PATH);
	long long dirN = 0;
	long long fileN = 0;
	FILE* dir_sql;
	FILE* file_sql;
	FILE* f;
	dir_xs* dir_init = new dir_xs;
	dir_xs* change_file = new dir_xs;
	dir_xs* change_dir = new dir_xs;
	int depth = 0;
	int dir_depth = 0;

	switch (function) {

	case 1:
		//建立目录树

		f = fopen("info.txt", "w");
		dir_sql = dirsql_open();
		file_sql = filesql_open();
		tree_build(&root, &dirN, &fileN, dir_sql, file_sql, &size, &mp);
		dirsql_close(dir_sql);
		filesql_close(file_sql);
		fprintf(f, "目录总数：%lld\t", dirN);
		fprintf(f, "文件总数：%lld\n", fileN);
		depth = tree_deep(&root);
		fprintf(f, "目录树深度：%d\t\t", depth);
		dir_depth = dir_deep(&root, Maxpath);
		fprintf(f, "文件最大层数：%d\n", dir_depth);
		fclose(f);
		log_write(dirN, fileN, dir_depth, Maxpath, depth, size);
		break;
	case 2:


		dir_sql = dirsql_open();
		file_sql = filesql_open();
		tree_build(&root, &dirN, &fileN, dir_sql, file_sql, &size, &mp);
		dirsql_close(dir_sql);
		filesql_close(file_sql);
		depth = tree_deep(&root);
		dir_depth = dir_deep(&root, Maxpath);
		log_write(dirN, fileN, dir_depth, Maxpath, depth, size);

		*dir_init = dirs_data(&root, mp);

		break;
	default:
		break;

	}

	delete dir_init, change_file, change_dir;
	return 0;
}


int function(int function)
{
	if (function < 3)
		function_two(function);
	else if (function == 3)
		file_fun();
	else if (function == 4)
		dir_fun();
	return 0;
}





