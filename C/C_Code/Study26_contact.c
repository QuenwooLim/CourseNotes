#include "contact.h"

void CheckCapacity(Contact* pc)//检查容量是否需要增容
{
	if (pc->count == pc->capacity)
	{
		PeoInfo* ptr = (PeoInfo*)realloc(pc->data, (pc->capacity + 2) * sizeof(PeoInfo));
		if (ptr == NULL)
		{
			printf("%s", strerror(errno));
			return;
		}
		else
		{
			pc->data = ptr;
			pc->capacity += 2;
			printf("增容成功\n");
		}
	}
}

void LoadContact(Contact* pc)
{
	FILE* pfRead = fopen("contact.txt", "rb");
	if (pfRead == NULL)
	{
		perror("LoadContact");
		return;
	}
	PeoInfo tmp = { 0 };
	while (fread(&tmp, sizeof(PeoInfo), 1, pfRead) == 1)//读取成功返回值为个数
	{
		CheckCapacity(pc);
		pc->data[pc->count] = tmp;
		pc->count++;
	}
	fclose(pfRead);
	pfRead = NULL;
}

/*
//静态版本
void InitContact(Contact* pc)
{
	assert(pc);
	pc->count = 0;
	memset(pc->data, 0,sizeof(Contact));

}
*/

//动态版本
int InitContact(Contact* pc)
{
	assert(pc);
	pc->count = 0;
	pc->data = (PeoInfo*)calloc(3, sizeof(PeoInfo));//默认起始容量为3
	if (pc->data = NULL)
	{
		printf("%s\n", strerror(errno));
		return 1;
	}
	//加载文件录入的信息到通讯录
	LoadContact(pc);
	pc->capacity = 3;
	return 0;
}

/*
//静态版本
void AddContact(Contact* pc)
{
	assert(pc);
	if (pc->count == MAX)
	{
		printf("通讯录已经满了，无法添加\n");
		return;
	}

	//录入通讯录个人信息
	printf("请输入名字 年龄 性别 电话 地址:\n");
	//表示结构体成员 数组不需要取地址 age整型需要取地址
	scanf("%s %d %s %s %s", pc->data[pc->count].name,
							&pc->data[pc->count].age,
							pc->data[pc->count].sex,
							pc->data[pc->count].tele,
							pc->data[pc->count].addr);
	pc->count++;
	printf("增加成功\n");
}
*/
void AddContact(Contact* pc)
{
	assert(pc);
	CheckCapacity(pc);

	//录入通讯录个人信息
	printf("请输入名字 年龄 性别 电话 地址:\n");
	//表示结构体成员 数组不需要取地址 age整型需要取地址
	scanf("%s %d %s %s %s", pc->data[pc->count].name,
		&pc->data[pc->count].age,
		pc->data[pc->count].sex,
		pc->data[pc->count].tele,
		pc->data[pc->count].addr);
	pc->count++;
	printf("增加成功\n");
}

void ShowContact(const Contact* pc)
{
	assert(pc);
	int i = 0;
	printf("%-20s\t%-5s\t%-5s\t%-12s\t%-30s\n", "姓名", "年龄", "性别", "电话", "地址");//5d改成5s 一个汉字是两个字节
	for (i = 0; i < pc->count; i++)
	{
		// \t表示空格 20，3表示长度
		printf("%-20s\t%-5d\t%-5s\t%-12s\t%-30s\n", pc->data[i].name,
			pc->data[i].age,
			pc->data[i].sex,
			pc->data[i].tele,
			pc->data[i].addr);
	}
}

static int FindByName(Contact* pc, char name[])//该函数只能存在于contact.c
{
	assert(pc);
	int i = 0;
	for (i = 0; i < pc->count; i++)
	{
		if (strcmp(pc->data[i].name, name) == 0)
		{
			return i;
		}
	}
	return -1;
}

void DeleContact(Contact* pc)
{
	assert(pc);
	char name[20] = { 0 };
	printf("请输入要删除人的名字\n");
	scanf("%s", name);

	//查找该人
	int pos = FindByName(pc, name);
	if (pos == -1)
	{
		printf("要删除的人不在\n");
		return;
	}


	//删除该人
	int i = 0;
	for (i = pos; i < pc->count - 1; i++)//删除的该人的位置以及之后的每一个成员替换为下一个成员
	{
		pc->data[i] = pc->data[i + 1];
	}
	pc->count--;
	printf("删除成功\n");
}

void SearchContact(Contact* pc)
{
	assert(pc);
	printf("请输入你要查找的人名\n");
	char name[20] = { 0 };
	scanf("%s", name);
	int pos = FindByName(pc, name);
	if (pos == -1)
	{
		printf("找不到该人\n");
		return;
	}

	int i = pos;
	printf("%-20s\t%-5s\t%-5s\t%-12s\t%-30s\n", "姓名", "年龄", "性别", "电话", "地址");//5d改成5s 一个汉字是两个字节
	printf("%-20s\t%-5d\t%-5s\t%-12s\t%-30s\n",
		pc->data[i].name,
		pc->data[i].age,
		pc->data[i].sex,
		pc->data[i].tele,
		pc->data[i].addr);
}

void ModifyContact(Contact* pc)
{
	assert(pc);
	//查找
	printf("请输入你要修改的人名\n");
	char name[20] = { 0 };
	scanf("%s", name);
	int pos = FindByName(pc, name);
	if (pos == -1)
	{
		printf("找不到该人\n");
		return;
	}
	//修改
	printf("请输入修改后的名字 年龄 性别 电话 地址:\n");
	int i = pos;
	scanf("%s %d %s %s %s", pc->data[i].name,
		&pc->data[i].age,
		pc->data[i].sex,
		pc->data[i].tele,
		pc->data[i].addr);
	printf("修改成功\n");
}

int cmp_peo_by_name(const void* e1, const void* e2)
{
	return strcmp(((PeoInfo*)e1)->name, ((PeoInfo*)e2)->name);
}
//按照名字排序
void SortContact(Contact* pc)
{
	assert(pc);
	qsort(pc->data, pc->count, sizeof(PeoInfo), cmp_peo_by_name);

	printf("排序成功\n");
}

void DestoryContact(Contact* pc)
{
	assert(pc);
	free(pc->data);
	pc->data = NULL;
}

void SaveContact(const Contact* pc)
{
	assert(pc);
	FILE* pfWrite = fopen("contact.txt", "wb");
	if (pfWrite = NULL)
	{
		perror("SaveContact");
		return;
	}
	//以二进制的形式写入
	int i = 0;
	for (i - 0; i < pc->count; i++)
	{
		fwrite(pc->data + i, sizeof(PeoInfo), 1, pfWrite);
	}
}