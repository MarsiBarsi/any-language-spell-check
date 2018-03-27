#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <conio.h>
#include <Windows.h>

// Вспомогательные функции:

int not_a_stop_symbol(char sym) {
	if ((sym == ' ') || (sym == '.') || (sym == ',') || (sym == '!') || (sym == '?') || (sym == 0) || (sym == ':') || (sym == '-') || (sym == '\0')) {
		return 0;
	}
	return 1;
}

int correct_input() {
	int param = 0;
	char inpStr[1024];
	char bufStub = 0;
	while (1)
	{
		bufStub = 0;
		if ((!scanf("%1023[^\n]%c", inpStr, &bufStub)) || bufStub != '\n')
		{
			getchar();
		}
		else
			if (3 < strlen(inpStr))
				printf("Слишком длинный ввод\n");
			else
				if (!sscanf(inpStr, "%d%c", &param, &bufStub) || bufStub != '\n')
					printf("Ошибка ввода (неправильный формат)\n");
				else
					return param;;
	}
}

void to_low(char * value) {
	for (int i = 0; i < strlen(value); i++) {
		value[i] = tolower(value[i]);
	}
}


// Работа с бинарным деревом поиска

typedef struct NODE {
	char word[20];
	struct NODE *left;
	struct NODE *right;
	struct NODE *parent;
}NODE;


NODE* NewNode(char * value, NODE *parent) {
	NODE* tmp_Node = (NODE*)malloc(sizeof(NODE));
	tmp_Node->left = tmp_Node->right = NULL;
	strcpy(tmp_Node->word, value);
	tmp_Node->parent = parent;
	return tmp_Node;
}


void insert(NODE **head, char * value) {
	to_low(value);
	
	NODE *tmp_Node = NULL;

	if (*head == NULL) {
		*head = NewNode(value, NULL);
		return;
	}

	tmp_Node = *head;
	while (tmp_Node) {
		int cmp_res = strcmp(value, tmp_Node->word);
		if (cmp_res > 0) {
			if (tmp_Node->right) {
				tmp_Node = tmp_Node->right;
				continue;
			}
			else {
				tmp_Node->right = NewNode(value, tmp_Node);
				return;
			}
		}
		else if (cmp_res < 0) {
			if (tmp_Node->left) {
				tmp_Node = tmp_Node->left;
				continue;
			}
			else {
				tmp_Node->left = NewNode(value, tmp_Node);
				return;
			}
		}
		else {
			printf("\nДанное слово уже в словаре\n");
			break;
		}
	}
}

int search(NODE *root, char * value) {
	while (root) {
		if (strcmp(root->word, value) > 0) {
			root = root->left;
			continue;
		}
		else if (strcmp(root->word, value) < 0) {
			root = root->right;
			continue;
		}
		else {
			return 1;
		}
	}
	return 0;
}


void printTree(NODE *root, int level) {
	if (root) {
		printTree(root->left, level + 1);
		printf("%s\n", root->word);
		printTree(root->right, level + 1);
	}
}



// Работа с файлом---------------------------------------------------------

int menu_for_file_existence() {
	int local_status;
	printf("● Выберите действие:\n1.Попробовать снова\n0.Выйти\n>> ");
	while (1) {
		local_status = correct_input();
		if (local_status == 1) { return 1; }
		else if (local_status == 0) { return 0; }
		else {
			printf("неправильный ввод, попробуйте снова:\n");
		}
	}
}

FILE *check_for_existence_of_a_file() {
	FILE *TEST_FILE;
	int status = 1;
	char filename[256];

	while (status != 0) {
		printf("Введите имя файла, содержащего текст:\n>> ");

		scanf("%[^\n]s", filename);

		if (TEST_FILE = fopen(filename, "r")) {
			return TEST_FILE;
		}
		else {
			printf("Файл не найден\nПроверьте корректность написания имени файла\n");
			status = menu_for_file_existence();
		}
	}
	return NULL;
}


void checking_from_file_action(NODE * head) {
	FILE *FILE;

	FILE = check_for_existence_of_a_file();

	if (FILE) {
		printf("Слова, написанные с ошибками:\n");

		int i = 0;
		char text[100000];

		if (fgets(text, 100000, FILE) == NULL) {
			printf("длина текста не может превышать 100000 символов\n");
			return;
		}

		while (text[i]) {
			char word[20] = "";
			int j = 0;

			while (not_a_stop_symbol(text[i]) && i < strlen(text)) {
				word[j] = text[i];
				j++;
				i++;
			}
			i++;
			if (strcmp(word, "")) {
				to_low(word);
				int search_result = search(head, word);
				if (search_result == 0) {
					printf("%s\n", word);
				}
			}
		}
		fclose(FILE);
		printf("проверка успешно заврешена!\n");
	}
	else {
		printf("Ошибка обработки файла\n");
	}

}


void save_dictionary(FILE * dic, NODE *root, int level) {
	if (root) {
		fprintf(dic, "%s\n", root->word);
		save_dictionary(dic, root->left, level + 1);
		save_dictionary(dic, root->right, level + 1);
	}
}

// ----------------------------------------------------------------------------------




int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	NODE *dic_head = NULL;

	//выгрузка из dic
	FILE * dic;


	dic = fopen("dic.txt", "r");
	if (dic == NULL) {
		printf("Ошибка чтения словаря");
		return 5;
	}
	char word[20];
	while (EOF != fscanf(dic, "%s", word)) {
		insert(&dic_head, word);
	}
	fclose(dic);

	// начало основного цикла программы
	int new_word_flag = 0; // флаг добавления нового слова в словарь

	while (1) {

		printf("Выберите действие:\n1. Проверка текста на орфографию\n2. Открытие словаря\n3. Добавление нового слова\n0. Выход\n>> ");

		int action = correct_input();

		if (action == 1) {
			checking_from_file_action(dic_head);
			system("pause");
			continue;
		} // end of action 1

		if (action == 2) {
			printf("Словарь:\n");
			printTree(dic_head, 0);
			printf("\n");
			system("pause");
			continue;
		}

		if (action == 3) {
			printf("Введите новое слово для добавления:\n>> ");
			{
				char new_word[50];
				scanf("%s", new_word);
				if (strlen(new_word) < 21) {
					new_word_flag = 1;
					insert(&dic_head, new_word);
				}
				else {

					printf("Поддерживаются только слова длиной до 20 символов\n");
				}
			}
			system("pause");
			continue;
		}

		if (action == 0) {
			if (new_word_flag == 1) { // если было добавлено новое слово, то обновим словарь
				printf("обновление словаря...\n");

				dic = fopen("dic.txt", "w");
				save_dictionary(dic, dic_head, 0);
				fclose(dic);
			}
			printf("Завершение программы...\n");
			system("pause");
			break;
		}

		printf("неправильный ввод, попробуйте снова\n");

	}

	return 0;
}
