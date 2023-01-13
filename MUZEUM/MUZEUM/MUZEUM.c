#pragma warning(disable : 4996)
#define _USE_MATH_DEFINES

/*++ BUILD Version: 0.0

Copyright (c) Łukasz Wyszomierski. All rights reserved.

--*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>

#define LIM_CHAR 255                  // limit of characters user can use
#define OVERFLOW_CHAR 10              // overflow of characters to clearing cache
#define LIM_ROOM 32                   // limit of rooms
#define LIM_DAY 31                    // day max in month
#define LIM_MONTH 12                  // month max in year
#define LIM_YEAR_MIN 1990             // create of museum
#define LIM_YEAR_MAX 2023             // current year
#define EXISTENCE_LIM_MIN -2137692137 // minimal year to exist
#define EXISTENCE_LIM_MAX  2023       // current year
#define LIB_NAME "lib_museum.mlib"	  // database file name

typedef unsigned char uchar;	     // shortcut of data types limit range [0, 255]							                        %c
typedef unsigned short int usint;    // shortcut of data types limit range [0, 65,535]						                        %hu
typedef unsigned long int ulint;     // shortcut of data types limit range [0, 4,294,967,295]				                        %lu
typedef signed long int slint;	     // shortcut of data types limit range [−2,147,483,647, +2,147,483,647]	                        %li
typedef signed long long int sllint; // shortcut of data types limit range [−9,223,372,036,854,775,807, +9,223,372,036,854,775,807] %lli
typedef struct lib lib;			     // Struct type as database name

ulint length_list = 0;	   // Length of database [global-value]

struct lib
{
	ulint id;			   // id of record [current session] 
	uchar title[LIM_CHAR], // title of exhibit limit range [0,255]
		descr[LIM_CHAR];   // description of exhibit limit range [0,255]
	slint yearofexistence; // year from where it exist of exhibit 
	bool  century;		   // if year is writed as century of exhibit
	float weight;		   // weight of exhibit
	usint day,		       // day of adding to database 
		month,	           // month of adding to database
		year;	           // year of adding to database 
	uchar state; 		   // State of exhibit in [A to E] A - like brand new | E - destroyed 
	usint room;			   // room in museum 0 is a storage
	lib* nextval;          // Address to next value
};

// List operations
int show(lib* tab, bool colapse);                          // function that simply display information about exhibit if something exits
void show_by_id(lib* tab, ulint id, bool colapse);         // function that display information about exhibit by id assigned in startup
void show_all_list(lib* tab, bool colapse);                // function that simply display all exhibits from list if something exits
lib* add_lib(lib* tab, uchar title[], 			           // /*
	uchar desc[], slint yearofEXISTENCE, 		           // 
	bool century, float weight, usint day, 		           // adding new record to current list if empty creating new list
	usint month, usint year, uchar state, 		           // 
	usint room); 					   			           // */
lib* remove_val_lib(lib* tab, ulint id);                   // remove one value from db by id
void edit_val_lib(lib* tab, ulint id, int edit_one_value); // edit current record
lib* free_lib(lib* tab);                                   // remove all records

// File operations
lib* load_lib_from_file(char fname[]);             // load all current records from file 
void save_lib_to_file(char fname[], lib* tab);     // save all current records to file 

// Searching operations
void room_list(lib* tab);				    // shows list of rooms that have any exhibit if has 0 do not appear
lib* user_add_to_lib(lib* tab);			    // ability of user to add new value with text gui 
int  show_from_room(lib* tab, usint room);  // shows list of exhibits in room 'x'
int  show_by_name(lib* tab, uchar str[]);   // find a exhibit by name
int  show_by_century(lib* tab, slint year); // find a exhibit by century
int  show_by_date(lib* tab, usint day, 	    // find a exhibit by date of adding to database
	usint month, usint year);			    // ...<---/-/--->...<---/-/--->...<---/-/--->...

// Data operations
ulint get_id_from_user();			               // please user to give a specific data with special protection [id]
slint get_yearofexistence_from_user();             // please user to give a specific data with special protection [year of existence]
slint get_yearofexistence_not_century_from_user(); // please user to give a specific data with special protection [year of existence]
bool  get_century_from_user();		               // please user to give a specific data with special protection [century]
float get_weight_from_user();		               // please user to give a specific data with special protection [weight]
usint get_day_from_user();			               // please user to give a specific data with special protection [day]
usint get_month_from_user();		               // please user to give a specific data with special protection [month]
usint get_year_from_user();			               // please user to give a specific data with special protection [year]
uchar get_state_from_user();		               // please user to give a specific data with special protection [state]
usint get_room_from_user();			               // please user to give a specific data with special protection [room]
int str_cmp_ch(char strT[], char strP[]);

// Menu operations
void user_get_by_id(lib* tab);	                              // user activity to show record by id
void user_edit_by_id(lib* tab, ulint id, int edit_one_value); // user activity to edit record by id
lib* user_remove_by_id(lib* tab);                             // user activity to remove one record from db by id
void user_find_name(lib* tab);	                              // user activity to find exactly name of exhibit
void user_find_century(lib* tab);                             // user activity to find exhibit by century
void user_find_date(lib* tab);	                              // user activity to find date of adding exhibit to db
void user_find_room(lib* tab);	                              // user activity to find exhibit by room number

int main()
{
	SetConsoleTitle(L"Witaj w programie lokalnym Muzeum!"); // changing title of window to custom name
	lib* master = NULL;                                     // main db variable
	master = load_lib_from_file(LIB_NAME);                  // loading all records from file
	bool faultdetect = false;                               // detecting if user click other button that he should
	char num = '\0';										// char select to selecting buffor 

	if (master == NULL)
	{
		SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - brak bazy danych");
		system("cls");
		printf("\n\nPlik bazy danych jest pusty!\n\nKliknij [1] aby dodac nowy rekord lub kliknij dowolny przycisk aby zamknac program... ");
		num = getch();
		switch (num)
		{
		case '1':
			SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - wczytywanie do czasu osiagniecia jednego rekordu");
			do
			{
				system("cls");
				printf("\n\n*UWAGA! Dodawanie do bazy danych bedzie wymuszane do czasu dodania jednego rekordu!*\n\nKilinij dowolny przycisk aby kontynuowac...");
				getch();
				master = user_add_to_lib(master);
			} while (master == NULL);

			//save_lib_to_file(LIB_NAME, master); // saving db to file
			break;
		default:
			return -1;
			break;
		}

		printf("\nKilinij dowolny przycisk aby kontynuowac...");
		getch();
	}
	else
	{
		system("cls");
		SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Bibioteka Wczytana");
		printf("\n\nBibioteka wczytana poprawnie!\n\n\n");
		Sleep(1250);
	}

	do
	{
		SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Menu Glowne Muzeum");
		system("cls");
		printf("\n\n<*>======== -Menu-Glowne-Muzeum- =======<*>\n"); // Main Menu
		printf("1) Lista eksponatow\n");							 // Main Menu
		printf("2) Zarzadzaj eksponatami\n");						 // Main Menu
		printf("3) Wyszukaj eksponat\n");							 // Main Menu
		printf("4) Zakoncz program i zapisz baze danych\n");		 // Main Menu
		printf("5) Zakoncz program i nie zapisuj bazy danych\n\n");	 // Main Menu

		if (faultdetect) printf("Opcja bledna wybierz jeszcze raz aby kontynuowac:"); else printf("Wybierz opcje aby kontynuowac:");
		num = getch(); // get from user option from menu

		switch (num)
		{
		case '1':
			faultdetect = false;
		opt1:
			num = '\0';
			SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Operacje Na Listach");
			system("cls");
			printf("\n\n<*>========= -Lista-Eksponatow- ========<*>\n"); // Exhibits list operation	menu
			printf("1) Wyswietl wszystkie wersja skrocona\n");			 // Exhibits list operation	menu
			printf("2) Wyswietl wszystkie szczegolowo\n");				 // Exhibits list operation	menu
			printf("3) Wyswietl przez ID\n");							 // Exhibits list operation	menu
			printf("4) Wyswietl zawartosc sal\n");						 // Exhibits list operation	menu
			printf("5) Powrot\n\n");									 // Exhibits list operation	menu

			if (faultdetect) printf("Opcja bledna wybierz jeszcze raz aby kontynuowac:"); else printf("Wybierz opcje aby kontynuowac:");
			num = getch(); // get from user option from menu
			switch (num)
			{
			case '1':
				num = '\0';
				faultdetect = false;
				SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Lista Skrocona");
				system("cls");
				show_all_list(master, false);
				printf("\nKilinij dowolny przycisk aby kontynuowac...");
				getch();
				goto opt1;

			case '2':
				num = '\0';
				faultdetect = false;
				SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Lista Szczegolowa");
				system("cls");
				show_all_list(master, true);
				printf("\nKilinij dowolny przycisk aby kontynuowac...");
				getch();
				goto opt1;

			case '3':
				num = '\0';
				faultdetect = false;
				SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Wyszukiwanie Przez ID");
				system("cls");
				user_get_by_id(master);
				printf("\nKilinij dowolny przycisk aby kontynuowac...");
				getch();
				goto opt1;

			case '4':
				num = '\0';
				faultdetect = false;
				SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Lista Sal");
				system("cls");
				room_list(master);
				printf("\nKilinij dowolny przycisk aby kontynuowac...");
				getch();
				goto opt1;

			case '5':
				num = '\0';
				faultdetect = false;
				continue;

			default:
				faultdetect = true; // when user push other button that he should
				goto opt1;
			}

			break;
		case '2':
			faultdetect = false;
		opt2:
			num = '\0';
			SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Zarzadzanie Eksponatami");
			system("cls");
			printf("\n\n<*>===== -Zarzadzanie-Eksponatami- =====<*>\n"); // Exhibit management menu
			printf("1) Dodaj eksponat\n");								 // Exhibit management menu
			printf("2) Edytuj eksponat przez ID\n");					 // Exhibit management menu
			printf("3) Usun eskponat przez ID\n");						 // Exhibit management menu
			printf("4) Powrot\n\n");									 // Exhibit management menu

			if (faultdetect) printf("Opcja bledna wybierz jeszcze raz aby kontynuowac:"); else printf("Wybierz opcje aby kontynuowac:");
			num = getch(); // get from user option from menu
			switch (num)
			{
			case '1':
				num = '\0';
				faultdetect = false;
				SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Dodawanie Nowego Rekordu");
				system("cls");
				master = user_add_to_lib(master);
				printf("\nKilinij dowolny przycisk aby kontynuowac...");
				getch();
				goto opt2;

			case '2':
				faultdetect = false;
			opt5:
				if (length_list == 0)
				{
					printf("\n\nNie istnieje zaden rekord!\n\n");
					getch();
					goto opt2;
				}
				ulint id = get_id_from_user();
			opt4:
				num = '\0';
				SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Edycja Eksponatu");
				system("cls");
				printf("\n\n<*>==== -Edycja-Eksponatu-o-ID-%d- ====<*>\n", id); // Exhibit editor menu
				printf("1) Edytuj tytul\n");					                // Exhibit editor menu
				printf("2) Edytuj opis\n");					                    // Exhibit editor menu
				printf("3) Edytuj rok/stulecie odkrycia\n");	                // Exhibit editor menu
				printf("4) Edytuj wage\n");					                    // Exhibit editor menu
				printf("5) Edytuj date dodania do bazy danych\n");              // Exhibit editor menu
				printf("6) Edytuj stan\n");  					                // Exhibit editor menu
				printf("7) Edytuj sale\n");					                    // Exhibit editor menu
				printf("8) Edytuj wszystkie dane\n");			                // Exhibit editor menu
				printf("9) Zmien aktualne id\n");                               // Exhibit editor menu
				printf("0) Powrot\n\n");							            // Exhibit editor menu

				if (faultdetect) printf("Opcja bledna wybierz jeszcze raz aby kontynuowac:"); else printf("Wybierz opcje aby kontynuowac:");
				num = getch(); // get from user option from menu
				switch (num)
				{
				case '1':
					num = '\0';
					faultdetect = false;
					SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Edycja Tytulu Przez ID");
					system("cls");
					user_edit_by_id(master, id, 1);
					printf("\nKilinij dowolny przycisk aby kontynuowac...");
					getch();
					goto opt4;
				case '2':
					num = '\0';
					faultdetect = false;
					SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Edycja Opisu Przez ID");
					system("cls");
					user_edit_by_id(master, id, 2);
					printf("\nKilinij dowolny przycisk aby kontynuowac...");
					getch();
					goto opt4;
				case '3':
					num = '\0';
					faultdetect = false;
					SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Edycja Roku/Stulecia Odkrycia Przez ID");
					system("cls");
					user_edit_by_id(master, id, 3);
					printf("\nKilinij dowolny przycisk aby kontynuowac...");
					getch();
					goto opt4;
				case '4':
					num = '\0';
					faultdetect = false;
					SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Edycja Wagi Przez ID");
					system("cls");
					user_edit_by_id(master, id, 4);
					printf("\nKilinij dowolny przycisk aby kontynuowac...");
					getch();
					goto opt4;
				case '5':
					num = '\0';
					faultdetect = false;
					SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Edycja Daty Dodania Do Zbioru Przez ID");
					system("cls");
					user_edit_by_id(master, id, 5);
					printf("\nKilinij dowolny przycisk aby kontynuowac...");
					getch();
					goto opt4;
				case '6':
					num = '\0';
					faultdetect = false;
					SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Edycja Statusu Przez ID");
					system("cls");
					user_edit_by_id(master, id, 6);
					printf("\nKilinij dowolny przycisk aby kontynuowac...");
					getch();
					goto opt4;
				case '7':
					num = '\0';
					faultdetect = false;
					SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Edycja Sali Przez ID");
					system("cls");
					user_edit_by_id(master, id, 7);
					printf("\nKilinij dowolny przycisk aby kontynuowac...");
					getch();
					goto opt4;
				case '8':
					num = '\0';
					faultdetect = false;
					SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Edycja Calego Rekordu Przez ID");
					system("cls");
					user_edit_by_id(master, id, 0);
					printf("\nKilinij dowolny przycisk aby kontynuowac...");
					getch();
					goto opt4;
				case '9':
					goto opt5;
				case '0':
					goto opt2;

				default:
					faultdetect = true; // when user push other button he should
					goto opt4;
					break;
				}
			case '3':
				num = '\0';
				faultdetect = false;
				SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Usuwanie Rekordu Przez ID");
				system("cls");
				master = user_remove_by_id(master);
				printf("\nKilinij dowolny przycisk aby kontynuowac...");
				getch();
				goto opt2;

			case '4':
				num = '\0';
				faultdetect = false;
				continue;

			default:
				faultdetect = true; // when user push other button he should
				goto opt2;
			}


			break;
		case '3':
			faultdetect = false;
		opt3:
			num = '\0';
			SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Wyszukiwanie Eksponatow");
			system("cls");
			printf("\n\n<*>===== -Wyszukiwanie-Eksponatow- =====<*>\n"); // Exhibit search menu
			printf("1) Wyszukaj po nazwie\n");							 // Exhibit search menu
			printf("2) Wyszukaj po stuleciu\n");						 // Exhibit search menu
			printf("3) Wyszukaj po dacie dodania do zbioru\n");			 // Exhibit search menu
			printf("4) Wyszukaj po numerze sali\n");					 // Exhibit search menu
			printf("5) Powrot\n\n");									 // Exhibit search menu

			if (faultdetect) printf("Opcja bledna wybierz jeszcze raz aby kontynuowac:"); else printf("Wybierz opcje aby kontynuowac:");
			num = getch(); // get from user option from menu
			switch (num)
			{
			case '1':
				num = '\0';
				faultdetect = false;
				SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Wyszukiwanie Przez Nazwe");
				system("cls");
				user_find_name(master);
				printf("\nKilinij dowolny przycisk aby kontynuowac...");
				getch();
				goto opt3;

			case '2':
				num = '\0';
				faultdetect = false;
				SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Wyszukiwanie Przez Stulecie");
				system("cls");
				user_find_century(master);
				printf("\nKilinij dowolny przycisk aby kontynuowac...");
				getch();
				goto opt3;

			case '3':
				num = '\0';
				faultdetect = false;
				SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Wyszukiwanie Przez Date Dodania");
				system("cls");
				user_find_date(master);
				printf("\nKilinij dowolny przycisk aby kontynuowac...");
				getch();
				goto opt3;

			case '4':
				num = '\0';
				faultdetect = false;
				SetConsoleTitle(L"Witaj w programie lokalnym Muzeum! - Wyszukiwanie Przez Numer Sali");
				system("cls");
				user_find_room(master);
				printf("\nKilinij dowolny przycisk aby kontynuowac...");
				getch();
				goto opt3;

			case '5':
				num = '\0';
				faultdetect = false;
				continue;

			default:
				faultdetect = true; // when user push other button that he should
				goto opt3;
			}

			break;

		case '4':
			save_lib_to_file(LIB_NAME, master); // saving db to file
			free_lib(master);					// releasing memory
			return 1;
			break;

		case '5':
			free_lib(master); // releasing memory
			return 2;
			break;

		default:
			faultdetect = true; // when user push other button he should
			continue;
			break;
		}
	} while (true);

	free_lib(master); // releasing memory
	return 0;
}

int show(lib* tab, bool colapse) // show current record
{
	int exist = 0;
	if (tab != NULL)
	{
		exist++;
		bool epoch = false; // for testing when 'year of existence' is lower than 0

		printf("<*>=====EKSPONAT=====<*> \n");
		printf("-> ID: %d \n-> Tytul:\"%s\" \n-> Opis:\"%s\" \n", tab->id, tab->title, tab->descr);
		if (colapse)
		{
			printf("-> Odnaleziony ");
			if (tab->century) printf("w stuleciu: "); else printf("w roku: ");

			if (tab->yearofexistence < 0)
			{
				epoch = true;
				tab->yearofexistence *= (-1);
			}
			printf("%d ", tab->yearofexistence);

			if (epoch)
			{
				tab->yearofexistence *= (-1);
				printf("p.n.e.");
			}
			else printf("n.e.");
			printf("\n-> Waga: %.2f kg\n-> Data doadania do zbioru [DD-MM-YYYY]: %d-%d-%d \n-> Stan uszkodzenia [1 - nienaruszony / 5 - zniszczony]: %d \n-> Miejsce przebywania: ", tab->weight, tab->day, tab->month, tab->year, (int)(tab->state) - 64);

			if (tab->room == 0) printf("Magazyn"); else printf("sala nr %d", tab->room);
			printf("\n\n");
		}
		else printf("\n");
	}
	else printf("\nEksponat nie jest zarejestrowany w pamieci.\n");
	return exist;
}

void show_by_id(lib* tab, ulint id, bool colapse) // show one exhibit if exist by id
{
	while (tab != NULL)
	{
		if (tab->id == id)
		{
			show(tab, colapse);
			return;
		}
		tab = tab->nextval;
	}
	return;
}

void show_all_list(lib* tab, bool colapse) // show entire database
{
	if (tab == NULL)
	{
		printf("\n\nNie istnieje zaden rekord!\n\n");
		return;
	}

	if (!colapse) printf("\n\n<*>= -Lista-Eksponatow-Wersja-Skrocona- =<*>\n\n");
	if (colapse) printf("\n\n<*>=== -Lista-Eksponatow-Szczegolowa- ==<*>\n\n");

	while (tab != NULL)
	{
		show(tab, colapse);
		tab = tab->nextval;
	}

	if (colapse)  printf("<*>=== -Lista-Eksponatow-Szczegolowa- ==<*>\n");
	if (!colapse) printf("<*>= -Lista-Eksponatow-Wersja-Skrocona- =<*>\n");


	return;
}

lib* add_lib(lib* tab, uchar title[], uchar desc[], slint yearofEXISTENCE, bool century, float weight, usint day, usint month, usint year, uchar state, usint room) // add new value to lib
{
	if (tab == NULL) // if db has no values
	{
		length_list++;							// adding to global value one to expand db size
		tab = calloc(1, sizeof(*tab));			// allocation new memory to write
		tab->id = 1;							// writing memory with special arguments
		strcpy(tab->title, title);				// writing memory with special arguments
		strcpy(tab->descr, desc);				// writing memory with special arguments
		tab->yearofexistence = yearofEXISTENCE;	// writing memory with special arguments
		tab->century = century;					// writing memory with special arguments
		tab->weight = weight;					// writing memory with special arguments
		tab->day = day;							// writing memory with special arguments
		tab->month = month;						// writing memory with special arguments
		tab->year = year;						// writing memory with special arguments
		tab->state = state;						// writing memory with special arguments
		tab->room = room;						// writing memory with special arguments
		tab->nextval = NULL;					// writing memory with special arguments

		return tab; // returning address
	}
	else // if db have at least one record
	{
		lib* buf = tab;   // saving address to future operations
		int id = buf->id; // saving current id of exhibit

		while (buf->nextval != NULL) // founding list element with end value 
		{
			id++;
			buf = buf->nextval;
		}

		buf->nextval = calloc(1, sizeof(*tab)); // allocation new memory to write
		buf = buf->nextval;						// change address to new fresh value
		id++;									// adding one to create new id for new values

		length_list++;							// incrising length
		buf->id = id;							// writing memory with special arguments
		strcpy(buf->title, title);				// writing memory with special arguments
		strcpy(buf->descr, desc);				// writing memory with special arguments
		buf->yearofexistence = yearofEXISTENCE;	// writing memory with special arguments
		buf->century = century;					// writing memory with special arguments
		buf->weight = weight;					// writing memory with special arguments
		buf->day = day;							// writing memory with special arguments
		buf->month = month;						// writing memory with special arguments
		buf->year = year;						// writing memory with special arguments
		buf->state = state;						// writing memory with special arguments
		buf->room = room;						// writing memory with special arguments
		buf->nextval = NULL;					// writing memory with special arguments
	}
	return tab;
}

lib* remove_val_lib(lib* tab, ulint id) // remove one value from db
{
	if (tab == NULL) return NULL; // if list is empty

	lib* buf = NULL; // buffor variable 

	if (tab->id == id) // in case removing first value
	{
		length_list--;
		buf = tab->nextval;
		free(tab);
		tab = buf;

		while (tab != NULL) // ADD subtracting to id to make it works fine <3
		{
			tab->id = tab->id - 1;
			tab = tab->nextval;
		}

		printf("\n\nRekord ID:%d zostal usuniety.\n\n", id);

		return buf;
	}

	lib* start = NULL; 	// buffor variable start address
	start = tab;		// saving first address to release after
	buf = tab->nextval;	// jump to next record
	while (tab != NULL && buf != NULL) // in case value is grater than first element
	{
		if (buf->id == id) // finding wanted id and releasing 
		{
			length_list--;
			tab->nextval = buf->nextval;
			free(buf);
			break;
		}
		tab = buf;
		buf = tab->nextval;
	}

	buf = tab->nextval;

	if (buf != NULL) // decreasing id from rest records
	{
		while (buf != NULL)
		{
			buf->id = buf->id - 1;
			buf = buf->nextval;
		}
	}

	printf("\n\nRekord ID:%d zostal usuniety.\n\n", id);

	return start;
}


void edit_val_lib(lib* tab, ulint id, int edit_one_val) // user editing one recored from db
{
	if (tab == NULL) return; // if list is empty


	while (tab != NULL)	   // /*
	{					   // Finding record by id to edit 
		if (tab->id == id) // */
		{
			uchar title[LIM_CHAR + OVERFLOW_CHAR],
				descr[LIM_CHAR + OVERFLOW_CHAR];	// TEMP VARIABLES FROM STRUCTURE
			slint yearofexistence;				    // TEMP VARIABLES FROM STRUCTURE
			bool  century;						    // TEMP VARIABLES FROM STRUCTURE
			float weight;						    // TEMP VARIABLES FROM STRUCTURE
			usint day, month, year;	          	    // TEMP VARIABLES FROM STRUCTURE
			uchar state; 		  				    // TEMP VARIABLES FROM STRUCTURE
			usint room;							    // TEMP VARIABLES FROM STRUCTURE
			char letter = '\0';						// buf letter for chooses
			int buf = 0;						    // buf integer to count overflow data and choosing state of exhibit
			bool faildetected = false;				// buf bool for detecting fail of writing
			bool epoch = false;						// buf bool for year that is p.n.e.

			if (edit_one_val < 0 && edit_one_val > 7)
			{
				return;
			}
			else
			{
				if (edit_one_val != 1) strcpy(title, tab->title);
				if (edit_one_val != 2) strcpy(descr, tab->descr);
				if (edit_one_val != 3)
				{
					yearofexistence = tab->yearofexistence;
					century = tab->century;
				}
				if (edit_one_val != 4) weight = tab->weight;
				if (edit_one_val != 5)
				{
					day = tab->day;
					month = tab->month;
					year = tab->year;
				}
				if (edit_one_val != 6) state = tab->state;
				if (edit_one_val != 7) room = tab->room;
			}

			if (edit_one_val == 0 || edit_one_val == 1)
			{
				do // security gate testing for overflow
				{
					system("cls");
					if (buf <= 1)
					{
						buf++;
						faildetected = false;
					}

					if (faildetected) printf("* Podany tytul jest nie poprawny wpisz go jeszcze raz *");
					printf("\n\n");

					printf("Aktualny tytul: \"%s\"\n-> Podaj nowy (maksymalnie %d znakow): ", tab->title, LIM_CHAR);
					fgets(title, LIM_CHAR + OVERFLOW_CHAR, stdin);

					if (strlen(title) > LIM_CHAR)
					{
						scanf("%*[^\n]"); scanf("%*c"); // clearing overflow of characters
					}

					if (title[strlen(title) - 1] == '\n') title[strlen(title) - 1] = '\0';

					if (strlen(title) < 1 || strlen(title) > LIM_CHAR) faildetected = true; else faildetected = false;
				} while (faildetected);
			}
			buf = INT_MIN;

			if (edit_one_val == 0 || edit_one_val == 2)
			{
				do // security gate testing for overflow
				{
					system("cls");

					if (faildetected) printf("* Podany opis jest nie poprawny wpisz go jeszcze raz *");
					printf("\n\n");

					printf("Aktualny opis: \"%s\"\n-> Podaj nowy (maksymalnie %d znakow): ", tab->descr, LIM_CHAR);
					fgets(descr, LIM_CHAR + OVERFLOW_CHAR, stdin);

					if (strlen(descr) > LIM_CHAR)
					{
						scanf("%*[^\n]"); scanf("%*c"); // clearing overflow of characters
					}

					if (descr[strlen(descr) - 1] == '\n') descr[strlen(descr) - 1] = '\0';

					if (strlen(descr) < 1 || strlen(descr) > LIM_CHAR) faildetected = true; else faildetected = false;
				} while (faildetected);
			}

			if (edit_one_val == 0 || edit_one_val == 3)
			{
				do // security gate testing for overflow
				{
					system("cls");

					if (faildetected) printf("* Podany rok odkrycia / stulecie jest nie poprawne wpisz go jeszcze raz *");
					printf("\n\n");

					printf("Aktualny rok odkrycia / stulecie (liczba ujemna oznacza p.n.e.): \"%d\"\n-> Podaj nowy: ", tab->yearofexistence);
					scanf("%li", &yearofexistence);

					if (yearofexistence > EXISTENCE_LIM_MAX || yearofexistence < EXISTENCE_LIM_MIN) faildetected = true; else faildetected = false;
				} while (faildetected);

				do // security gate testing for overflow
				{
					system("cls");

					if (faildetected) printf("* Podana odpowiedz jest nie poprawna wpisz ja jeszcze raz *");
					printf("\n\n");

					scanf("%c", &letter);

					printf("Wartosc \"%d\" ", tab->yearofexistence);
					if (!tab->century) printf("nie");

					printf(" jest zapisana jako stulecie.\nNowa wartosc wynosi \"%d\".\n", yearofexistence);


					printf("\n-> Czy nowa wartosc zapisywac jako stulecie (y/n): ");
					scanf("%c", &letter);

					if (tolower(letter) != 'y' && tolower(letter) != 'n') faildetected = true; else faildetected = false;

				} while (faildetected);

				if (tolower(letter) == 'y') century = true; else century = false;
			}



			if (edit_one_val == 0 || edit_one_val == 4)
			{
				do // security gate testing for overflow
				{
					system("cls");

					if (faildetected) printf("* Podana waga jest nie poprawna wpisz ja jeszcze raz *");
					printf("\n\n");

					printf("Aktualna waga: \"%.2f\"\n-> Podaj nowa: ", tab->weight);
					scanf("%f", &weight);

					if (weight <= 0) faildetected = true; else faildetected = false;
				} while (faildetected);
			}


			if (edit_one_val == 0 || edit_one_val == 5)
			{
				do // security gate testing for overflow
				{
					system("cls");

					if (faildetected) printf("* Podany dzien jest nie poprawny wpisz go jeszcze raz *");
					printf("\n\n");

					printf("Aktualny dzien dodania do bazy: \"%d\"\n-> Podaj nowa: ", tab->day);
					scanf("%hu", &day);

					if (day <= 0 || day > LIM_DAY) faildetected = true; else faildetected = false;
				} while (faildetected);

				do // security gate testing for overflow
				{
					system("cls");

					if (faildetected) printf("* Podany miesiac jest nie poprawny wpisz go jeszcze raz *");
					printf("\n\n");

					printf("Aktualny miesiac dodania do bazy: \"%d\"\n-> Podaj nowy: ", tab->month);
					scanf("%hu", &month);

					if (month <= 0 || month > LIM_MONTH) faildetected = true; else faildetected = false;
				} while (faildetected);

				do // security gate testing for overflow
				{
					system("cls");

					if (faildetected) printf("* Podany dzien jest nie poprawny wpisz go jeszcze raz *");
					printf("\n\n");

					printf("Aktualny rok dodania do bazy (%d-%d): \"%d\"\n-> Podaj nowy: ", LIM_YEAR_MIN, LIM_YEAR_MAX, tab->year);
					scanf("%hu", &year);

					if (year < LIM_YEAR_MIN || year > LIM_YEAR_MAX) faildetected = true; else faildetected = false;
				} while (faildetected);
			}


			if (edit_one_val == 0 || edit_one_val == 6)
			{
				do // security gate testing for overflow
				{
					system("cls");

					if (faildetected) printf("* Podany stan jest nie poprawny wpisz go jeszcze raz *");
					printf("\n\n");

					printf("Aktualny stan: %d\n-> Podaj nowy (1-5) [1 - nienaruszone / 5 - zniszczone]: ", (int)(tab->state) - 64);
					scanf("%d", &buf);

					if (buf < 1 || buf > 5) faildetected = true; else faildetected = false;
				} while (faildetected);
				state = buf + 64;
			}


			if (edit_one_val == 0 || edit_one_val == 7)
			{
				do // security gate testing for overflow
				{
					system("cls");

					if (faildetected) printf("* Podane polozenie jest nie poprawne wpisz je jeszcze raz *");
					printf("\n\n");

					printf("Aktualne polozenie [0 oznacza magazyn]: %d\n-> Podaj nowe polozenie (0-%d): ", tab->room, LIM_ROOM);
					scanf("%hu", &room);

					if (room < 0 || room > LIM_ROOM) faildetected = true; else faildetected = false;
				} while (faildetected);
			}

			do // displaying final result
			{
				system("cls");
				printf("\n\n<*>=========== -Podsumowanie- ==========<*>\n");
				if (edit_one_val == 0 || edit_one_val == 1)
				{
					printf("-> Stary tytul:\"%s\" \n-> Nowy tytul:\"%s\" \n", tab->title, title);
				}

				if (edit_one_val == 0 || edit_one_val == 2)
				{
					printf("-> Stary opis:\"%s\" \n-> Nowy opis:\"%s\" \n", tab->descr, descr);
				}

				if (edit_one_val == 0 || edit_one_val == 3)
				{
					printf("-> Odnaleziony ");
					if (tab->century) printf("w stuleciu: "); else printf("w roku: ");
					if (tab->yearofexistence < 0)
					{
						epoch = true;
						tab->yearofexistence *= (-1);
					}
					printf("%d ", tab->yearofexistence);

					if (epoch)
					{
						tab->yearofexistence *= (-1);
						printf("p.n.e.");
					}
					else printf("n.e.");
					printf(" => Odnaleziony ");

					if (century) printf("w stuleciu: "); else printf("w roku: ");

					epoch = false;

					if (yearofexistence < 0)
					{
						epoch = true;
						yearofexistence *= (-1);
					}

					printf("%d ", yearofexistence);

					if (epoch)
					{
						yearofexistence *= (-1);
						printf("p.n.e.");
					}
					else printf("n.e.");
				}


				if (edit_one_val == 0 || edit_one_val == 4)
				{
					printf("\n-> Waga: %.2f =>  %.2f \n", tab->weight, weight);
				}

				if (edit_one_val == 0 || edit_one_val == 5)
				{
					printf("-> Data doadania do zbioru [DD-MM-YYYY]: %d-%d-%d => %d-%d-%d\n", tab->day, tab->month, tab->year, day, month, year);
				}

				if (edit_one_val == 0 || edit_one_val == 6)
				{
					printf("-> Stan uszkodzenia [1 - nienaruszony / 5 - zniszczony]: %d => %d\n", (int)(tab->state) - 64, (int)state - 64);
				}

				if (edit_one_val == 0 || edit_one_val == 7)
				{
					printf("-> Miejsce przebywania: ");
					if (tab->room == 0) printf("Magazyn"); else printf("sala nr %d", tab->room);

					printf(" => ");

					if (room == 0) printf("Magazyn"); else printf("sala nr %d", room);
				}


				letter = NULL;
				if (edit_one_val != 1 && edit_one_val != 2) scanf("%c", &letter); // skipping null value 

				printf("\n\n Czy zapisane dane sa poprawne (y/n): ");

				scanf("%c", &letter);
			} while (tolower(letter) != 'y' && tolower(letter) != 'n');

			if (tolower(letter) == 'y')	// when user want to save 
			{
				strcpy(tab->title, title);				// writing memory with arguments
				strcpy(tab->descr, descr);				// writing memory with arguments
				tab->yearofexistence = yearofexistence;	// writing memory with arguments
				tab->century = century;					// writing memory with arguments
				tab->weight = weight;					// writing memory with arguments
				tab->day = day;							// writing memory with arguments
				tab->month = month;						// writing memory with arguments
				tab->year = year;						// writing memory with arguments
				tab->state = state;						// writing memory with arguments
				tab->room = room;						// writing memory with arguments

				system("cls");
				printf("\n\nEdycja zakonczona dane zapisane.\n");
			}
			else
			{
				system("cls");
				printf("\n\nEdycja zakonczona dane pominiete.\n");
			}

			return;
		}

		tab = tab->nextval; // jumping to next record
	}
	return;
}


lib* free_lib(lib* tab) // releasing the memory
{
	lib* buf = NULL;

	buf = tab;

	while (tab != NULL && buf != NULL) // changing records until releasing all records
	{
		buf = tab->nextval;
		free(tab);
		tab = buf;
	}

	return tab; // || NULL
}

lib* load_lib_from_file(char fname[]) // loading db file
{
	lib* tab = NULL;           // buffor start db
	FILE* fl = NULL;           // file with all records to open 
	fl = fopen(LIB_NAME, "r"); // opening file with records

	if (fl == NULL) // in case if file does not exist
	{
		return NULL;
	}

	int   breakpoint = 0,                // if search found a ';' char
		wordscount = 0,                  // counting how many variables have
		validation = 0;                  // validaton testing if saving is complete 
	int templen = 0;					 // variable storaging length of temp var
	uchar buf[LIM_CHAR + OVERFLOW_CHAR], // current text get by file 
		temp[LIM_CHAR + OVERFLOW_CHAR];  // temporary saving of combination buf var
	bool wrongdata = false;              // testing if data in db is correct 
	ulint id = 1;		                 // TEMP VARIABLES FROM STRUCTURE 
	uchar title[LIM_CHAR],               // TEMP VARIABLES FROM STRUCTURE 
		descr[LIM_CHAR];                 // TEMP VARIABLES FROM STRUCTURE 
	slint yearofexistence;               // TEMP VARIABLES FROM STRUCTURE 
	bool  century;		                 // TEMP VARIABLES FROM STRUCTURE 
	float weight;		                 // TEMP VARIABLES FROM STRUCTURE 
	usint day, month, year;              // TEMP VARIABLES FROM STRUCTURE 
	uchar state; 		                 // TEMP VARIABLES FROM STRUCTURE 
	usint room;			                 // TEMP VARIABLES FROM STRUCTURE 

	temp[0] = '\0'; // clearing inside of variable to add with buf

	while (fscanf(fl, "%s ", buf) != EOF)
	{
		wordscount++;               // counting words who at the end will be added to temp 
		templen = strlen(temp);	    // temp length
		if (templen > LIM_CHAR + 1) // if temp is overloaded
		{
			temp[0] = '\0';
			wrongdata = true;
		}


		if (buf[0] == ';') // find a separator 
		{
			buf[0] = '\0';
			breakpoint++;
			wordscount = 0;

		}

		if (wrongdata && breakpoint <= 9) // skipping rest values if any data is not correct to skip rest from saving
		{
			validation = 0;
			wordscount = 0;

			continue;
		}

		if (wrongdata && breakpoint == 10) // creating special moment to skip wrong values
		{
			buf[0] = ';';
		}


		if (breakpoint == 1 && validation == 0) // set title
		{
			if (templen == 0 || templen > LIM_CHAR) // testing if data from file is not longer that limit
			{
				wrongdata = true;
				continue;
			}

			temp[--templen] = '\0';
			strcpy(title, temp);
			temp[0] = '\0';
			validation = 1;
		}

		if (breakpoint == 2 && validation == 1) // set description
		{
			if (templen == 0 || templen > LIM_CHAR) // testing if data from file is not longer that limit
			{
				wrongdata = true;
				continue;
			}

			temp[--templen] = '\0';
			strcpy(descr, temp);
			temp[0] = '\0';
			validation = 2;
		}

		if (breakpoint == 3 && validation == 2) // set year of existence 
		{
			if (!(atoi(temp) <= EXISTENCE_LIM_MAX && atoi(temp) >= EXISTENCE_LIM_MIN)) // testing if value is not reaching out of space
			{
				wrongdata = true;
				continue;
			}

			yearofexistence = atoi(temp);
			temp[0] = '\0';
			validation = 3;
		}


		if (breakpoint == 4 && validation == 3) // set if year of existence is a century
		{
			if (atoi(temp) != 1 && atoi(temp) != 0) // testing if it is a bool value
			{
				wrongdata = true;
				continue;
			}

			century = atoi(temp);
			temp[0] = '\0';
			validation = 4;
		}

		if (breakpoint == 5 && validation == 4) // set weight 
		{
			if (atof(temp) <= 0) // testing if weight is not negative
			{
				wrongdata = true;
				continue;
			}

			weight = atof(temp);
			temp[0] = '\0';
			validation = 5;
		}

		if (breakpoint == 6 && validation == 5) // set day of adding
		{
			if (!(atoi(temp) <= LIM_DAY && atoi(temp) > 0)) // testing if value is not reaching out of space
			{
				wrongdata = true;
				continue;
			}

			day = atoi(temp);
			temp[0] = '\0';
			validation = 6;
		}

		if (breakpoint == 7 && validation == 6) // set month of adding
		{
			if (!(atoi(temp) <= LIM_MONTH && atoi(temp) > 0)) // testing if value is not reaching out of space
			{
				wrongdata = true;
				continue;
			}

			month = atoi(temp);
			temp[0] = '\0';
			validation = 7;
		}

		if (breakpoint == 8 && validation == 7) // set year of adding
		{
			if (!(atoi(temp) <= LIM_YEAR_MAX && atoi(temp) >= LIM_YEAR_MIN)) // testing if value is not reaching out of space
			{
				wrongdata = true;
				continue;
			}

			year = atoi(temp);
			temp[0] = '\0';
			validation = 8;
		}

		if (breakpoint == 9 && validation == 8) // set state from A to E
		{
			switch (toupper(temp[0])) // testing if state has good format
			{
			case 'A':
				break;
			case 'B':
				break;
			case 'C':
				break;
			case 'D':
				break;
			case 'E':
				break;
			default:
				wrongdata = true;
				continue;
				break;
			}

			state = toupper(temp[0]);
			temp[0] = '\0';
			validation = 9;
		}

		if (breakpoint == 10 && validation == 9) // set room 
		{
			if (!(atoi(temp) <= LIM_ROOM && atoi(temp) >= 0)) // testing if value is not reaching out of space
			{
				wrongdata = true;
				continue;
			}

			room = atoi(temp);
			temp[0] = '\0';
			validation = 10;
		}

		if ((buf[0] == ';' || buf[0] == '\0') && breakpoint == 10) // ending the line and reseting important variables
		{
			if (!wrongdata)
			{
				tab = add_lib(tab, title, descr, yearofexistence, century, weight, day, month, year, state, room); // saving correct data
			}
			else wrongdata = false;	// setting values to default to read next line
			breakpoint = 0;			// setting values to default to read next line
			validation = 0;			// setting values to default to read next line

			continue;

		}

		if (wordscount >= 1 && strlen(buf) <= LIM_CHAR) // if any words exist make space after to easier combine
		{
			strcat(buf, " \0");
		}

		strcat(temp, buf); // adding buf to temp
	}

	fclose(fl); // closing file 
	return tab; // release address of start
}

void save_lib_to_file(char fname[], lib* tab) // saving entire db to a file with specified format
{
	if (tab == NULL) return; // if db is empty 

	FILE* file = fopen(fname, "w");

	if (file == NULL) // if file is not opened
	{
		printf("\n\nZapis nie zostal wykonany plik bazy danych nie jest dostępny.\n\n");
		getchar();
		return;
	}

	while (tab != NULL) // saving db until records come to end
	{
		fprintf(file, "%s ; %s ; %d ; %d ; %.2f ; %d ; %d ; %d ; %c ; %d ;\n", tab->title, tab->descr, tab->yearofexistence, tab->century, tab->weight, tab->day, tab->month, tab->year, tab->state, tab->room);
		tab = tab->nextval;
	}

	fclose(file); // closing file
	return;
}

void room_list(lib* tab)
{
	if (length_list == 0)
	{
		printf("\n\nNie istnieje zaden rekord!\n\n");
		return;
	}


	usint rooms[LIM_ROOM + 1]; // how many exhibits been in room 'x'
	usint sum = 0;			   // sum of exhibits in any room

	for (int i = 0; i <= LIM_ROOM; i++) rooms[i] = 0; // clearing space

	while (tab != NULL) // counting how many exhibits is in current room
	{
		rooms[tab->room]++;
		tab = tab->nextval;
	}

	printf("\n\n<*>============ -Lista-Sal- ============<*>\n\n");

	for (int i = 1; i <= LIM_ROOM; i++) // displaying how many exhibits is in 'x' room
	{
		if (rooms[i] > 0)
		{
			if (i == 0)
			{
				printf("Magazyn ");
			}
			else
			{
				printf("Sala nr %2d ", i);
			}

			printf("posiada %d ", rooms[i]);
			if (rooms[i] > 1) printf("eksponatow.\n"); else printf("eksponat.\n");

			sum += rooms[i];
		}
	}
	if (sum == 0) printf("Wszystkie eksponaty sa w magazynie. Stan magazynu wynosi %d\n", length_list); else printf("\nSuma wystawionych eksponatow wynosi %d, %d jest w magazynie.\n", sum, length_list - sum);

	printf("\n<*>============ -Lista-Sal- ============<*>\n");

	return;
}

lib* user_add_to_lib(lib* tab)
{
	//if (tab == NULL) return tab; // if list is empty

	uchar title[LIM_CHAR + OVERFLOW_CHAR],
		descr[LIM_CHAR + OVERFLOW_CHAR];	// TEMP VARIABLES FROM STRUCTURE
	slint yearofexistence;				    // TEMP VARIABLES FROM STRUCTURE
	bool  century;						    // TEMP VARIABLES FROM STRUCTURE
	float weight;						    // TEMP VARIABLES FROM STRUCTURE
	usint day, month, year;	          	    // TEMP VARIABLES FROM STRUCTURE
	uchar state; 		  				    // TEMP VARIABLES FROM STRUCTURE
	usint room;							    // TEMP VARIABLES FROM STRUCTURE
	char letter = '\0';						// buf letter
	int buf = INT_MIN;						// buf integer
	bool faildetected = false;				// buf bool for detecting fail of writing
	bool epoch = false;						// buf bool for year that is p.n.e.

	do
	{
		system("cls");

		if (faildetected) printf("* Podany tytul jest nie poprawny wpisz go jeszcze raz *");
		printf("\n\n");

		printf("-> Podaj nazwe (maksymalnie %d znakow): ", LIM_CHAR);
		fgets(title, LIM_CHAR + OVERFLOW_CHAR, stdin);

		if (strlen(title) > LIM_CHAR)
		{
			scanf("%*[^\n]"); scanf("%*c"); // clearing overflow of characters
		}

		if (title[strlen(title) - 1] == '\n') title[strlen(title) - 1] = '\0';

		if (strlen(title) < 1 || strlen(title) > LIM_CHAR) faildetected = true; else faildetected = false;
	} while (faildetected);


	do // security gate testing for overflow
	{
		system("cls");

		if (faildetected) printf("* Podany opis jest nie poprawny wpisz go jeszcze raz *");
		printf("\n\n");

		printf("-> Podaj opis (maksymalnie %d znakow): ", LIM_CHAR);
		fgets(descr, LIM_CHAR + OVERFLOW_CHAR, stdin);

		if (strlen(descr) > LIM_CHAR)
		{
			scanf("%*[^\n]"); scanf("%*c"); // clearing overflow of characters
		}

		if (descr[strlen(descr) - 1] == '\n') descr[strlen(descr) - 1] = '\0';

		if (strlen(descr) < 1 || strlen(descr) > LIM_CHAR) faildetected = true; else faildetected = false;
	} while (faildetected);

	yearofexistence = get_yearofexistence_not_century_from_user(); // automatic get info from user with values protection
	century = get_century_from_user();				               // automatic get info from user with values protection
	weight = get_weight_from_user();				               // automatic get info from user with values protection
	day = get_day_from_user();						               // automatic get info from user with values protection
	month = get_month_from_user();					               // automatic get info from user with values protection
	year = get_year_from_user();					               // automatic get info from user with values protection
	state = get_state_from_user();					               // automatic get info from user with values protection
	room = get_room_from_user();					               // automatic get info from user with values protection

	// displaying final result
	do
	{
		letter = '\0';
		scanf("%c", &letter); // skipping null value 

		system("cls");
		printf("\n\n<*>=========== -Podsumowanie- ==========<*>\n");
		printf("-> Tytul:\"%s\"\n-> Opis:\"%s\" \n-> Odnaleziony ", title, descr);

		if (century) printf("w stuleciu: "); else printf("w roku: ");
		if (yearofexistence < 0)
		{
			epoch = true;
			yearofexistence *= (-1);
		}
		printf("%d ", yearofexistence);

		if (epoch)
		{
			yearofexistence *= (-1);
			epoch = false;
			printf("p.n.e.");
		}
		else printf("n.e.");

		printf("\n-> Waga: %.2f \n-> Data doadania do zbioru [DD-MM-YYYY]: %d-%d-%d\n-> Stan uszkodzenia [1 - nienaruszony / 5 - zniszczony]: %d\n-> Miejsce przebywania: ", weight, day, month, year, (int)state - 64);

		if (room == 0) printf("Magazyn"); else printf("sala nr %d", room);
		if (faildetected) printf("\n\n* Podana odpowiedz jest nie poprawna wpisz ja jeszcze raz *\n");

		printf("\n\n Czy zapisane dane sa poprawne (y/n): ");
		scanf("%c", &letter);

		if (tolower(letter) != 'y' && tolower(letter) != 'n') faildetected = true; else faildetected = false;
	} while (faildetected);

	if (tolower(letter) == 'y') // when user want to add new record 
	{
		system("cls");
		tab = add_lib(tab, title, descr, yearofexistence, century, weight, day, month, year, state, room);

		printf("\n\nDodawanie zakonczone pomyslnie.\n");
	}
	else
	{
		system("cls");
		printf("\n\nDodawanie zakonczone. Dane nie zapisane.\n");
	}

	return tab;
}

int show_from_room(lib* tab, usint room) // showing all exhibits from current room
{
	if ((room < 0 && room > LIM_ROOM)) return 0;
	int count = 0;
	bool first = false;

	while (tab != NULL)
	{
		if (tab->room == room)
		{
			if (!first)
			{
				printf("\n\n<*>============= -Eksponat- ============<*>\n\n");
				first = true;
			}
			show(tab, true);
			count++;
		}
		tab = tab->nextval;
	}

	if (first) printf("<*>============= -Eksponat- ============<*>\n");

	return count;
}

int show_by_name(lib* tab, uchar str[]) // showing all exhibits who have special name
{
	if (strlen(str) <= 0) return;
	int count = 0;
	bool first = false;

	while (tab != NULL)
	{
		if (str_cmp_ch(tab->title, str))
		{
			if (!first)
			{
				printf("\n\n<*>============= -Eksponat- ============<*>\n\n");
				first = true;
			}
			show(tab, true);
			count++;
		}
		tab = tab->nextval;
	}

	if (first) printf("<*>============= -Eksponat- ============<*>\n");

	return count;
}

int show_by_century(lib* tab, slint year) // showing all exhibits with current century
{
	if (year > EXISTENCE_LIM_MAX || year < EXISTENCE_LIM_MIN) return 0;
	int count = 0;
	bool first = false;

	while (tab != NULL)
	{
		if (tab->century == true && year == tab->yearofexistence)
		{
			if (!first)
			{
				printf("\n\n<*>============= -Eksponat- ============<*>\n\n");
				first = true;
			}
			show(tab, true);
			count++;
		}
		tab = tab->nextval;
	}

	if (first) printf("<*>============= -Eksponat- ============<*>\n");

	return count;
}

int show_by_date(lib* tab, usint day, usint month, usint year) // showing all exhibits with current date
{
	if (day <= 0 || day > LIM_DAY) return 0;
	if (month <= 0 || month > LIM_MONTH) return 0;
	if (year < LIM_YEAR_MIN || year > LIM_YEAR_MAX) return 0;
	int count = 0;
	bool first = false;

	while (tab != NULL)
	{
		if (tab->day == day && tab->month == month && tab->year == year)
		{
			if (!first)
			{
				printf("\n\n<*>============= -Eksponat- ============<*>\n\n");
				first = true;
			}
			show(tab, true);
			count++;
		}
		tab = tab->nextval;
	}

	if (first) printf("<*>============= -Eksponat- ============<*>\n");

	return count;
}

ulint get_id_from_user() // getting secured id from user
{
	ulint id = -1;
	bool faildetected = false;

	do
	{
		system("cls");

		if (faildetected) printf("* Podane ID jest nie poprawne wpisz je jeszcze raz *");
		printf("\n\n");

		printf("-> Podaj ID eksponatu (1-%d): ", length_list);
		scanf("%d", &id);

		if (id < 1 || id > length_list) faildetected = true; else faildetected = false;
	} while (faildetected);
	return id;
}

slint get_yearofexistence_from_user() // getting secured year of existence from user
{
	slint yearofexistence;
	bool faildetected = false;
	do
	{
		system("cls");

		if (faildetected) printf("* Podane stulecie jest nie poprawne wpisz je jeszcze raz *");
		printf("\n\n");

		printf("-> Podaj stulecie (liczba ujemna oznacza p.n.e.): ");
		scanf("%li", &yearofexistence);
		if (yearofexistence > EXISTENCE_LIM_MAX || yearofexistence < EXISTENCE_LIM_MIN) faildetected = true; else faildetected = false;
	} while (faildetected);
	return yearofexistence;
}

slint get_yearofexistence_not_century_from_user() // getting secured year of existence from user
{
	slint yearofexistence;
	bool faildetected = false;
	do
	{
		system("cls");

		if (faildetected) printf("* Podany rok / stulecie jest nie poprawne wpisz je jeszcze raz *");
		printf("\n\n");

		printf("-> Podaj rok [stulecie] (liczba ujemna oznacza p.n.e.): ");
		scanf("%li", &yearofexistence);
		if (yearofexistence > EXISTENCE_LIM_MAX || yearofexistence < EXISTENCE_LIM_MIN) faildetected = true; else faildetected = false;
	} while (faildetected);
	return yearofexistence;
}

bool get_century_from_user() // getting secured century from user
{
	char letter = NULL;
	bool faildetected = false;
	scanf("%c", &letter);

	do
	{
		system("cls");

		if (faildetected) printf("* Podana odpowiedz jest nie poprawna wpisz ja jeszcze raz *");

		printf("\n\n-> Czy podana wczesniej wartosc zapisywac jako stulecie (y/n): ");
		scanf("%c", &letter);

		if (tolower(letter) != 'y' && tolower(letter) != 'n') faildetected = true; else faildetected = false;

	} while (faildetected);

	if (tolower(letter) == 'y') return true; else return false;
}

float get_weight_from_user() // getting secured weight from user
{
	float weight;
	bool faildetected = false;

	do
	{
		system("cls");

		if (faildetected) printf("* Podana waga jest nie poprawna wpisz ja jeszcze raz *");
		printf("\n\n");

		printf("-> Podaj wage: ");
		scanf("%f", &weight);

		if (weight <= 0) faildetected = true; else faildetected = false;
	} while (faildetected);
	return weight;
}

usint get_day_from_user() // getting secured day from user
{
	usint day;
	bool faildetected = false;

	do
	{
		system("cls");

		if (faildetected) printf("* Podany dzien jest nie poprawny wpisz go jeszcze raz *");
		printf("\n\n");

		printf("-> Podaj dzien: ");
		if (scanf("%hu", &day) == EOF)
		{
			scanf("%*[^\n]"); scanf("%*c");
		}

		if (day <= 0 || day > LIM_DAY) faildetected = true; else faildetected = false;
	} while (faildetected);

	return day;
}

usint get_month_from_user() // getting secured month from user
{
	usint month;
	bool faildetected = false;

	do
	{
		system("cls");

		if (faildetected) printf("* Podany miesiac jest nie poprawny wpisz go jeszcze raz *");
		printf("\n\n");

		printf("-> Podaj miesiac: ");
		scanf("%hu", &month);

		if (month <= 0 || month > LIM_MONTH) faildetected = true; else faildetected = false;
	} while (faildetected);

	return month;
}

usint get_year_from_user() // getting secured year from user
{
	usint year;
	bool faildetected = false;

	do
	{
		system("cls");

		if (faildetected) printf("* Podany rok jest nie poprawny wpisz go jeszcze raz *");
		printf("\n\n");

		printf("-> Podaj rok (%d-%d): ", LIM_YEAR_MIN, LIM_YEAR_MAX);
		scanf("%hu", &year);

		if (year < LIM_YEAR_MIN || year > LIM_YEAR_MAX) faildetected = true; else faildetected = false;
	} while (faildetected);

	return year;
}

uchar get_state_from_user() // getting secured state from user
{
	uchar state = NULL;
	int buf = 0;
	bool faildetected = false;

	do // security gate testing for overflow
	{
		system("cls");

		if (faildetected) printf("* Podany stan jest nie poprawny wpisz go jeszcze raz *");
		printf("\n\n");

		printf("-> Podaj stan w skali (1-5) [1 - nienaruszone / 5 - zniszczone]: ");
		scanf("%d", &buf);

		if (buf < 1 || buf > 5) faildetected = true; else faildetected = false;
	} while (faildetected);
	state = buf + 64;
	return state;
}

usint get_room_from_user() // getting secured room from user
{
	usint room;
	bool faildetected = false;

	do
	{
		system("cls");

		if (faildetected) printf("* Podane polozenie jest nie poprawne wpisz je jeszcze raz *");
		printf("\n\n");

		printf("-> Podaj polozenie [0 oznacza magazyn] (0-%d): ", LIM_ROOM);
		scanf("%hu", &room);

		if (room < 0 || room > LIM_ROOM) faildetected = true; else faildetected = false;
	} while (faildetected);

	return room;
}

int str_cmp_ch(char strT[], char strP[])
{
	int lenT = strlen(strT),
		lenP = strlen(strP);
	bool is_part = false,
		skip = false;

	if (lenT < lenP)
	{
		return 0;
	}
	else if (lenT == lenP)
	{
		return !strcpy(lenT, lenP);
	}

	for (int i = 0, j = 0; i < lenT; i++)
	{
		if (skip)
		{
			if (strT[i] == ' ')
			{
				skip = false;
			}
			j = 0;
			continue;
		}

		if (tolower(strT[i]) == tolower(strP[j]) && !skip)
		{
			j++;
			if (j == lenP) return 1;
			is_part = true;
			continue;
		}
		else
		{
			is_part = false;
			skip = true;
		}

	}
	return 0;
}

void user_get_by_id(lib* tab)  // getting list of exhibit with special id
{
	if (length_list == 0)
	{
		printf("\n\nNie istnieje zaden rekord!\n\n");
		return;
	}

	ulint id = get_id_from_user();

	system("cls");
	printf("\n\n<*>========== -Eksponat-ID-%d- =========<*>\n\n", id);
	show_by_id(tab, id, true);
	printf("<*>========== -Eksponat-ID-%d- =========<*>\n", id);

	return;
}

void user_edit_by_id(lib* tab, ulint id, int edit_one_value) // edit exhibit with special id
{
	if (length_list == 0)
	{
		printf("\n\nNie istnieje zaden rekord!\n\n");
		return;
	}

	system("cls");
	edit_val_lib(tab, id, edit_one_value);

	return;
}

lib* user_remove_by_id(lib* tab) // remove exhibit with special id
{
	if (length_list == 0)
	{
		printf("\n\nNie istnieje zaden rekord!\n\n");
		return tab;
	}

	ulint id = get_id_from_user();

	system("cls");
	tab = remove_val_lib(tab, id);

	return tab;
}

void user_find_name(lib* tab) // find exhibit with special name
{
	if (length_list == 0)
	{
		printf("\n\nNie istnieje zaden rekord!\n\n");
		return;
	}

	uchar title[LIM_CHAR + OVERFLOW_CHAR];
	bool faildetected = false;
	do
	{
		system("cls");

		if (faildetected) printf("* Podany tytul jest nie poprawny wpisz go jeszcze raz *");
		printf("\n\n");

		printf("-> Podaj nazwe (maksymalnie %d znakow): ", LIM_CHAR);
		fgets(&title, LIM_CHAR + OVERFLOW_CHAR, stdin);

		if (strlen(title) > LIM_CHAR)
		{
			scanf("%*[^\n]"); scanf("%*c"); // clearing overflow of characters
		}

		if (title[strlen(title) - 1] == '\n') title[strlen(title) - 1] = '\0';

		if (strlen(title) < 1 || strlen(title) > LIM_CHAR) faildetected = true; else faildetected = false;
	} while (faildetected);

	system("cls");
	if (!show_by_name(tab, title))
	{
		printf("\n\nNie istnieje rekord o nazwie \"%s\"\n", title);
	}

	return;
}

void user_find_century(lib* tab) // find exhibit with special century
{
	if (length_list == 0)
	{
		printf("\n\nNie istnieje zaden rekord!\n\n");
		return;
	}

	slint yearofexistence = get_yearofexistence_from_user();
	bool isold = false;

	system("cls");

	if (!show_by_century(tab, yearofexistence))
	{
		printf("\n\nNie istnieje rekord o stuleciu ");

		if (yearofexistence < 0)
		{
			isold = true;
			yearofexistence *= (-1);
		}

		printf("%d ", yearofexistence);

		if (isold)
		{
			yearofexistence *= (-1);
			isold = false;
			printf("p.n.e.");
		}
		else printf("n.e.");
		printf("\n");
	}

	return;
}

void user_find_date(lib* tab) // find exhibit with special date of adding to db
{
	if (length_list == 0)
	{
		printf("\n\nNie istnieje zaden rekord!\n\n");
		return;
	}

	usint day = get_day_from_user(),
		month = get_month_from_user(),
		year = get_year_from_user();

	system("cls");

	if (!show_by_date(tab, day, month, year))
	{
		printf("\n\nNie istnieje rekord o dacie dodania %d-%d-%d\n", day, month, year);
	}
	return;
}

void user_find_room(lib* tab)  // find exhibit with special room number
{
	if (length_list == 0)
	{
		printf("\n\nNie istnieje zaden rekord!\n\n");
		return;
	}

	usint room = get_room_from_user();

	system("cls");

	if (!show_from_room(tab, room))
	{
		if (room == 0) printf("\n\nW magazynie nie ma zadnych eksponatow\n", room); else printf("\n\nW %d sali nie ma zadnych eksponatow\n", room);
	}
	return;
}