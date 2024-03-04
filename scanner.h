
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<windows.h>
#include<map>

//目标文件夹
const char INIT_PATH[] = "c:\\windows\\";
const uint64_t EPOCH_DIFF = 116444736000000000ULL;
using namespace std;

//文件夹信息
struct dir_data {
	char			path[260];
	FILETIME		lastWriteTime;
};

//文件信息
struct file_data {
	char			path[260];
	unsigned long	size;
	FILETIME		lastWriteTime;
};

union info {
	struct dir_data  dir;
	struct file_data file;
};

typedef struct node_info {
	struct node_info* parent;
	struct node_info* left_brother;
	struct node_info* right_child;
	int		depth;
	char  type;				//d,dir;f,file
	union info data;
	long long id;					//id号
}node;

//栈

typedef struct stack_depth {
	node* dir;
	int tree_depth;
}info_dir;

typedef struct stack_view {
	info_dir stack_dir[30000];
	int num;
}stack;

//存储文件信息

typedef struct dir_xinxi {
	char dir_path[260];
	int	file_num;
	long long size;
	char last_time_file[260];
	char first_time_file[260];
	FILETIME last_time;
	FILETIME	ear_time;
}dir_x;


typedef struct dir_xinxis {
	int num;
	dir_x dirs_data[200];
}dir_xs;

//文件（夹）操作信息
typedef struct function_data {
	char path[260];
	char function;
	char time[10];
	char size[10];
}function_data;



int tree_deep(node* root);
void tree_build(node* root, long long* e, long long* f, FILE* dir_sql, FILE* file_sql, long long* size, map <string, long long>* mp);
int dir_deep(node* root, char*);
FILE* dirsql_open();
void dirsql_close(FILE* f);
void dir_sql_write(FILE* f, node* data);
FILE* filesql_open();
void filesql_close(FILE* f);
void file_sql_write(FILE* f, node* data);
void in_stack(node* dir, stack* _stack, int i);
info_dir out_stack(stack* _stack);
dir_xs dirs_data(node* root, map<string, long long> mp);
void functon_file(node* root, map<string, long long>* mp, dir_xs* change);
void function_dir(node* root, map<string, long long>* mp, dir_xs* change);
void compare(dir_xs* first, dir_xs* later,int a);



