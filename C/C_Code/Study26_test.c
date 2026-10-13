#include "contact.h"

void menu()
{
	printf("*******************************\n");
	printf("****** 1.add   2.del    *******\n");
	printf("****** 3.search4.modify *******\n");
	printf("****** 5.show  6.sort   *******\n");
	printf("****** 0.exit           *******\n");
	printf("*******************************\n");

}
int main()
{
	int input = 0;
	//通讯录
	Contact con;
	//初始化通讯录
	InitContact(&con);
	do
	{
		menu();
		printf("请输入你的选择\n");
		scanf("%d", &input);
		switch (input)
		{
		case 0:
			SaveContact(&con);//通讯录保存
			DestoryContact(&con);//释放动态内存空间
			printf("退出\n");
			break;
		case 1:
			AddContact(&con);
			break;
		case 2:
			DeleContact(&con);
			break;
		case 3:
			SearchContact(&con);
			break;
		case 4:
			ModifyContact(&con);
			break;
		case 5:
			ShowContact(&con);
			break;
		case 6:
			SortContact(&con);
			break;
		default:
			printf("输入错误\n");
			break;
		}
	} while (input);


	return 0;
}