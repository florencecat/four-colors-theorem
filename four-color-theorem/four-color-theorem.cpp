#include <windows.h>
#include <stdio.h>
#include <cstdio>
#include <ctime>
#include <locale.h>

#define _CRT_SECURE_NO_WARNINGS

int colors[4] = { RGB(205, 92, 92), RGB(60, 179, 113), RGB(70, 130, 180), RGB(255, 165, 0) };

struct countries
{
	char name[64];
	char filename[64];

	int color = 0;
	int num = 0;

	countries* neighbours[10];

	bool ispainted = 0;
};

int map_cr(POINT* pois, const char* filename)
{
	POINT p; int i = 0, counter = 0;
	FILE* temp = fopen(filename, "wt");

	printf("\a");
	do
	{
		GetCursorPos(&p);

		pois[i].x = p.x;
		pois[i].y = p.y;
		i++;
		Sleep(1000);
	} while (p.x != 0 || p.y != 0);

	for (i = 0; i < 750 && (pois[i + 1].x != 0 && pois[i + 1].y != 0); i++)
	{
		fprintf(temp, "%d %d\n", pois[i].x, pois[i].y);
		counter++;
	}
	fclose(temp);

	temp = fopen("countries/countries.txt", "at");
	fprintf(temp, "\n%s", filename);
	fclose(temp);

	printf("\a");

	return counter;
}

int reading(FILE* stream, const char* filename, POINT* ps)
{
	int c = 0;
	stream = fopen(filename, "rt");

	while (fscanf(stream, "%d%d", &ps[c].x, &ps[c].y) != EOF)
		c++;

	fclose(stream);
	return c;
}

void drawsmth(HDC dc, HBRUSH brush, HPEN pen, POINT* pois, int count, int color)
{
	SelectObject(dc, pen);

	Polyline(dc, pois, count);

	brush = CreateSolidBrush(color);
	SelectObject(dc, brush);

	Polygon(dc, pois, count);

	DeleteObject(brush);
}

void checking(countries* country_info, int* freeclrs)
{
	int x = 0, i = 0;

	while (country_info->neighbours[x] != 0)
	{
		i = 0;
		if (country_info->neighbours[x]->ispainted == 1)
		{
			while (freeclrs[i] != country_info->neighbours[x]->color && i < 4)
				i++;

			freeclrs[i] = 0;
		}
		x++;
	}
}

void menu_intro(FILE* stream, HDC dc, HPEN pen, HBRUSH brush, POINT* pois)
{
	stream = fopen("menu/countries.txt", "rt");
	FILE* temp = NULL;

	int count = 0;
	char menu_buffer[64];

	while (!feof(stream))
	{
		fscanf(stream, "%s", &menu_buffer);
		count = reading(temp, menu_buffer, pois);

		SelectObject(dc, pen);
		Polyline(dc, pois, count);

		SelectObject(dc, brush);
		Polygon(dc, pois, count);
	}

}

int main()
{
	setlocale(LC_ALL, "Rus");

	system("color 70");
	HWND hwnd = GetConsoleWindow();
	HDC dc = GetDC(hwnd);

	FILE* stream = NULL;

	HBRUSH brush = CreateSolidBrush(RGB(105, 105, 105));
	HPEN pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));

	POINT pois[750];
	countries country_info[50], * circle[20], * bufffer[20];

	bool graph[50][50];

	int answer, count_of_countries = 0, k = 1, temp, painted = 1, freeclrs[5], n, m;
	char bff[64], * buffer;

	srand(time(NULL));

	//Инициализация массива стран
	stream = fopen("countries/countries.txt", "rt");
	while (!feof(stream))
	{
		fscanf(stream, "%s", &bff);
		strcpy(country_info[k].filename, bff);

		buffer = strtok(bff, "/.");	buffer = strtok(NULL, "/.");

		strcpy(country_info[k].name, buffer);

		country_info[k].num = k; country_info[k].ispainted = 0; country_info[k].color = 0;

		for (int i = 0; i < 10; i++)
			country_info[k].neighbours[i] = 0;
		k++;
	}
	fclose(stream);
	count_of_countries = k - 1;

	//Чтение графа из файла и присвоение странам их соседей
	stream = fopen("countries/graph_array.txt", "rt");
	for (int i = 0; i < count_of_countries; i++)
	{
		k = 0;
		for (int j = 0; j < count_of_countries; j++)
		{
			fscanf(stream, "%d ", &graph[i][j]);
			if (graph[i][j] == 1)
			{
				country_info[i + 1].neighbours[k] = &country_info[j + 1];
				k++;
			}
		}
	}
	fclose(stream);

	//Отрисовка заставки
	Sleep(2000);
	n = 10;
	for (int i = 0; i < 12; i++)
	{
		menu_intro(stream, dc, pen, brush, pois);
		pen = CreatePen(PS_SOLID, 3, RGB(0 + n * i * 2, 0 + n * i * 2, 0 + n * i * 2));
		brush = CreateSolidBrush(RGB(105 + n * i, 105 + n * i, 105 + n * i));

		Sleep(75);
	}

	do
	{
		system("cls");
		printf("\n\n\t\t\t\t\t\t\t\t\t\t\t COLORING THE MAP WITH A MINIMUM NUMBER OF COLORS\n\n\n\n\n\n");
		printf("\t\t\t\t\t\t\t\t\t\t\t\t  1. SHOW THE COLORED MAP (USA)\n\n");
		printf("\t\t\t\t\t\t\t\t\t\t\t\t   2. SHOW CURRENT GRAPH (USA)\n\n");
		printf("\t\t\t\t\t\t\t\t\t\t\t      3. CREATE MAP (COUNTRY, STATE, ETC.)\n\n");
		printf("\t\t\t\t\t\t\t\t\t\t\t\t       4. CUSTOMIZE GRAPH\n\n\n");
		printf("\t\t\t\t\t\t\t\t\t\t\t\t         5. USER MANUAL\n\n");
		printf("\t\t\t\t\t\t\t\t\t\t\t\t      6. ABOUT THE PROGRAM\n\n\n");
		printf("\t\t\t\t\t\t\t\t\t\t\t\t\t    7. QUIT\n\n");
		printf("\t\t\t\t\t\t\t\t\t\t\t\t\t [Your answer]: ");

		scanf("%d", &answer);
		switch (answer)
		{
		case 1:
			//Рисование карты
		again:
			system("cls");

			for (int i = 0; i <= count_of_countries; i++)
			{
				country_info[i].color = 0;
				country_info[i].ispainted = 0;
				if (i < 20)
				{
					circle[i] = 0;
					bufffer[i] = 0;
				}
			}

			pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));

			temp = rand() % count_of_countries + 1;
			country_info[temp].color = colors[0];

			drawsmth(dc, brush, pen, pois, reading(stream, country_info[temp].filename, pois), country_info[temp].color);
			country_info[temp].ispainted = 1;
			circle[0] = &country_info[temp];

			painted = 1;
			while (painted < count_of_countries)
			{
				for (int i = 0; i < 20; i++)
					bufffer[i] = 0;
				n = 0; m = 0;
				//n - number of element from current circle
				//m - poi at last place in buffer circle
				while (circle[n] != 0)
				{
					k = 0;
					// ctr - counter of painted neighbors of the state
					// k - current neighbor of the state
					while (circle[n]->neighbours[k] != 0)
					{
						if (circle[n]->neighbours[k]->ispainted == 0)
						{
							for (int i = 0; i < 5; i++)
								freeclrs[i] = colors[i];

							checking(circle[n]->neighbours[k], freeclrs);

							temp = 0;
							while (freeclrs[temp] == 0)
								temp++;

							if (temp > 3)
								goto again;

							circle[n]->neighbours[k]->color = freeclrs[temp];

							pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
							drawsmth(dc, brush, pen, pois, reading(stream, circle[n]->neighbours[k]->filename, pois), circle[n]->neighbours[k]->color);

							circle[n]->neighbours[k]->ispainted = 1;
							bufffer[m] = circle[n]->neighbours[k];
							m++; painted++;
						}
						k++;
					}
					n++;
				}
				for (int i = 0; i < 20; i++)
					circle[i] = bufffer[i];
			}

			Sleep(10000);
			system("pause");
			break;
		case 2:
			//Вывод графа
			system("cls");
			printf("   ");
			for (int i = 0; i < count_of_countries; i++)
				printf("%c%c ", country_info[i + 1].name[0], country_info[i + 1].name[1]);

			printf("\n");
			for (int i = 0; i < count_of_countries; i++)
			{
				printf("%c%c ", country_info[i + 1].name[0], country_info[i + 1].name[1]);
				for (int j = 0; j < count_of_countries; j++)
					printf("%d  ", graph[i][j]);

				printf("\n");
			}

			system("pause");
			break;
		case 3:
			//Создание карты
			system("cls");

			printf("Enter the country name: ");
			scanf("%d", &bff);

			Sleep(2000);
			k = map_cr(pois, bff);
			Sleep(2000);

			Polyline(dc, pois, k);
			system("pause");
			break;
		case 4:
			// Создание графа
			system("cls");
			temp = -1;
			for (int i = 0; i < count_of_countries; i++)
			{
				do
				{
					system("cls");
					printf("Select neighbors for %s:\n\n", country_info[i + 1].name);

					for (int j = 0; j < count_of_countries; j++)
					{
						if (j != i && graph[i][j] == 0)
							printf("%d. %s\n", country_info[j + 1].num, country_info[j + 1].name);
						if (graph[i][j] == 1)
							temp++;
					}

					printf("\nCurrent - %d\n", temp);
					scanf("%d", &temp);
					if (temp != 0)
					{
						graph[i][temp - 1] = 1;
						graph[temp - 1][i] = 1;
					}
				} while (temp != 0);
			}

			for (int i = 0; i < count_of_countries; i++)
			{
				for (int j = 0; j < count_of_countries; j++)
					fprintf(stream, "%d ", graph[i][j]);
				fprintf(stream, "\n");
			}
			break;
		case 5:
			system("cls");
			printf("ПРОГРАММА ДЛЯ РАСКРАШИВАНИЯ КАРТЫ МИНИМАЛЬНЫМ КОЛИЧЕСТВОМ ЦВЕТОВ\n\nДанная программа закрашивает карту (находящуюся в папке countries) минимальным количеством цветов.\nДля того, чтобы закрасить карту, нужно выбрать первый пункт меню.\nВторой пункт меню нужен для того, чтобы показать заданный на текущий момент граф.\nСледующие два пункта нужны для создания карты и задания нового графа соответственно.\n\n");
			system("pause");

			break;
		case 6:
			system("cls");
			printf("Разработчик программы - студент группы 1-42хх Ивановского государственного энергетического университета Тонков Кирилл\nТекущая версия программы - 1.0\n\n");
			system("pause");

			break;
		}
	} while (answer != 7);
}