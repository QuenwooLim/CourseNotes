#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include<assert.h>
#define MAX 100
#include<stdlib.h>

//类型声明


//个人信息
typedef struct PeoInfo
{
	char name[20];
	int age;
	char sex[10];
	char tele[12];
	char addr[30];

}PeoInfo; //标识符的重命名

//通讯录


/*
//静态版本
typedef struct Contact
{
	PeoInfo data[MAX];//存储一百条信息
	int count;//计算通讯录记录数目
}Contact;
*/

//动态版本
typedef struct Contact
{
	PeoInfo* data;//存储人的信息
	int count;//计算当前通讯录记录数目
	int capacity;//当前通讯录的容量
}Contact;

int InitContact(Contact* pc);

void AddContact(Contact* pc);

void ShowContact(const Contact* pc);//防止被修改

void DeleContact(Contact* pc);

void SearchContact(Contact* pc);

void ModifyContact(Contact* pc);

void SortContact(Contact* pc);

void DestoryContact(Contact* pc);//动态内存回收

void SaveContact(Contact* pc);//通讯录保存

void LoadContact(Contact* pc);//通讯录信息加载

void LoadContact(Contact* pc);

