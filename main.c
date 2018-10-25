#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include "a.h"
#include "a.c"
#define ESCAPE 27
#define ENTER 10

char *first_item[] = {"","Login/Logout", " Find ID/PW", "  Sign up", "Withdrawal"};
char *second_item[] = {"", "Find Friend", "Recommend", "Add Friend", "Chatting"};
char *third_item[] = {"", " Write Post", " View Post", "  Setting", "  Fakebook"};

WINDOW **first_menu()
{
	int i;
	WINDOW **items;

	items=(WINDOW **)malloc(5 * sizeof(WINDOW *)); 

	items[0]=newwin(6,14,2,3);
	wbkgd(items[0], COLOR_PAIR(3));
	box(items[0],ACS_VLINE, ACS_HLINE);

	items[1]=subwin(items[0], 1, 12, 3, 4);
	items[2]=subwin(items[0], 1, 12, 4, 4);
	items[3]=subwin(items[0], 1, 12, 5, 4);
	items[4]=subwin(items[0], 1, 12, 6, 4);
	for (i =1; i < 5; i++)
		wprintw(items[i], "%s",first_item[i]);
	wbkgd(items[1], COLOR_PAIR(1));
	wrefresh(items[0]);
	return items;
}

WINDOW **second_menu()
{
	int i;
	WINDOW **items;

	items=(WINDOW **)malloc(5 * sizeof(WINDOW *)); 
	items[0]=newwin(6,14,2,20);
	wbkgd(items[0], COLOR_PAIR(3));
	box(items[0],ACS_VLINE, ACS_HLINE);

	items[1]=subwin(items[0], 1, 12, 3, 21);
	items[2]=subwin(items[0], 1, 12, 4, 21);
	items[3]=subwin(items[0], 1, 12, 5, 21);
	items[4]=subwin(items[0], 1, 12, 6, 21);
	for (i =1; i < 5; i++)
		wprintw(items[i], "%s",second_item[i]);
	wbkgd(items[1], COLOR_PAIR(1));
	wrefresh(items[0]);
	return items;
}

WINDOW **third_menu()
{
	int i;
	WINDOW **items;

	items=(WINDOW **)malloc(5 * sizeof(WINDOW *));
	items[0]=newwin(6,14,2,37);
	wbkgd(items[0], COLOR_PAIR(3));
	box(items[0],ACS_VLINE, ACS_HLINE);

	items[1]=subwin(items[0], 1, 12, 3, 38);
	items[2]=subwin(items[0], 1, 12, 4, 38);
	items[3]=subwin(items[0], 1, 12, 5, 38);
	items[4]=subwin(items[0], 1, 12, 6, 38);
	for (i = 1; i < 5; i++)
		wprintw(items[i], "%s", third_item[i]);
	wbkgd(items[1], COLOR_PAIR(1));
	wrefresh(items[0]);
	return items;
}

void about_window()
{
	WINDOW *about;
	//	about = newwin(6, 40, 5, 10);
	about = newwin(4, 40, 20, 10);
	box(about, ACS_VLINE, ACS_HLINE);
	wmove(about, 1, 2);
	wprintw(about, "Ncurses Test Verson 0.1"); 
	wmove(about, 2, 2);
	wprintw(about, "Made : yundream"); 
	wmove(about, 3, 2);
	wprintw(about, "Date : 2001.5.2"); 
	wmove(about, 4, 2);
	wprintw(about, "Press any key.. "); 
	mvprintw(20, 20, "AAAAAAAAAAAAAAAAAAAAAA");	
	wbkgd(about, COLOR_PAIR(3));
	refresh();
	wgetch(about);
	delwin(about);
}

int check_quit()
{
	WINDOW *check;
	int key;
	check = newwin(3, 40, 10, 20);
	wmove(check, 1, 2);
	wprintw(check, "Exit program (y/n) ? "); 
	wbkgd(check, COLOR_PAIR(2));
	box(check, ACS_VLINE, ACS_HLINE);
	refresh();
	key = wgetch(check);
	delwin(check);
	if (key == 'y')
		return 1;
	else 
		return 0;
}

void delete_menu(WINDOW **items, int count)
{
	int i;
	for (i = 0; i < count; i++)
		delwin(items[i]);
	free(items);
}

int scroll_menu(WINDOW **items,int count,int menu_start_col, int index_num)
{
	int key;
	int selected=0;
	while (1) 
	{
		key=getch();
		if (key==KEY_DOWN || key==KEY_UP) 
		{
			wbkgd(items[selected+1],COLOR_PAIR(3));
			wnoutrefresh(items[selected+1]);
			if (key==KEY_DOWN) 
			{
				selected=(selected+1) % count;
			} 
			else 
			{
				selected=(selected+count-1) % count;
			}
			wbkgd(items[selected+1],COLOR_PAIR(1));
			wnoutrefresh(items[selected+1]);
			doupdate();
		} 
		else if (key==ESCAPE) 
		{
			return -1;
		} 
		else if (key==ENTER) 
		{
			return selected;
		}
	}
}

void init_scr()
{
	//	initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_BLUE, COLOR_WHITE);
	init_pair(3, COLOR_GREEN, COLOR_WHITE);

	init_pair(4, COLOR_BLACK, COLOR_BLUE);
	init_pair(5, COLOR_RED, COLOR_WHITE);
	init_pair(6, COLOR_GREEN, COLOR_BLACK);
	
	init_pair(7, COLOR_WHITE, COLOR_GREEN); // fixed
	init_pair(8, COLOR_CYAN, COLOR_BLUE);
	init_pair(9, COLOR_GREEN, COLOR_MAGENTA);

	attron(COLOR_PAIR(4));
	draw(show1);
	refresh();
	sleep(5);
	draw(show2);
	attroff(COLOR_PAIR(4));

	curs_set(2);
	noecho();
	keypad(stdscr, TRUE);
}

void make_menubar(WINDOW *menubar)
{
	wbkgd(menubar, COLOR_PAIR(2));
	wattron(menubar, COLOR_PAIR(3));
	waddstr(menubar, "File");
	wattron(menubar, COLOR_PAIR(3));
	waddstr(menubar, "(a)");
	wmove(menubar, 0, 20);
	wattron(menubar, COLOR_PAIR(3));
	waddstr(menubar, "Help");
	wattron(menubar, COLOR_PAIR(3));
	waddstr(menubar, "(h)");
	wattron(menubar, COLOR_PAIR(3));
}

int main()
{
	main2();
	return 0;
}

int main2()
{

	setlocale(LC_ALL, "ko_KR.utf8");
	WINDOW *menubar, *statusbar, *about;
	int key, check = 1;
	initscr();	
	//	bkgd(COLOR_PAIR(1));
	init_scr();
	menubar = subwin(stdscr, 1, 80, 0, 0);
	int a, b; char temp[20];
	statusbar = subwin(stdscr, 2, 20, 10, 10);
	make_menubar(menubar);
	curs_set(0);
	refresh();    
	while(1)
	{
		WINDOW **menu_items;            
		int selected_item;

		key = getch();
		werase(statusbar);
		wrefresh(statusbar);

		if (key == 'q')
		{
			menu_items=first_menu();
			selected_item = scroll_menu(menu_items, 4, 20, 0); 
			delete_menu(menu_items, 5);
			//				if (selected_item < 0)
			//						wprintw(statusbar, "You haven't selected any item");
			if(selected_item == 0)
			{
				if(check == 1)
				{
					check = join();
					myself.myinfo.number = load_friendlist();
					insert_data_all();
					load_friend();
					getmyIP();
					if(check == 2)
						myself.show_friends2();
				}
				else
					logout();
			}
			else if(selected_item == 1)
			{
				findinfo(); 
			}
			else if(selected_item == 2)
			{
				makeid();
			}
			else if(selected_item == 3)
			{
				secede();
			}
			touchwin(stdscr);
			refresh();    
		}
		if (key == 'w')
		{
			menu_items=second_menu();
			selected_item = scroll_menu(menu_items, 4, 10, 1); 
			delete_menu(menu_items, 5);
			//			if (selected_item < 0)
			//				wprintw(statusbar, "You haven't selected any item");
			if(selected_item == 0)
				myself.show_friends();
			else if(selected_item == 1)
				myself.recommend(people);
			else if(selected_item == 2)
				myself.add_friends();
			else if(selected_item == 3)
			{
				endwin();
				chatting_client("127.0.0.1");
		//		endwin();
			}
			
			touchwin(stdscr);
			refresh();    
		}

		if (key == 'e')
		{
			menu_items=third_menu();
			selected_item = scroll_menu(menu_items, 4, 20, 1); 
			delete_menu(menu_items, 5);
			if(selected_item == 0)
			{ 
				write_post();
			}
			else if(selected_item == 1)
			{ 
				show_post();
			}
			else if(selected_item == 2)
			{ 
				changeinfo();
			}
			else if(selected_item == 3)
			{ 
				about_window();
			}
		}
		touchwin(stdscr);
		refresh();    
	}
	endwin();
	tcflush(0, TCIFLUSH);
	fflush(stdout);
}
