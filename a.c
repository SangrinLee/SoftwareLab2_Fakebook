#include "a.h"

data::data()
{
}

void data::add_friends()
{
	WINDOW *make;
	make = newwin(17, 50, 9, 2);
	box(make, ACS_VLINE, ACS_HLINE);
	init_pair(10, COLOR_GREEN, COLOR_BLACK);
	wbkgd(make, COLOR_PAIR(10));
	echo();
	info temp;
	int i;

	mvwprintw(make, 2, 2, "Enter ID to add : ");
	mvwgetstr(make, 2, 31, temp.id);
	for(i=0; i<total; i++)
	{
		if(strcmp(people[i].myinfo.id, temp.id) == 0)
		{
			friends[myinfo.number++] = people[i].myinfo;
			mvwprintw(make, 1, 2, "Friend added.");
			insert_friend(friends[myinfo.number-1], myinfo.number);
		}
	}
	show_friends2();
	refresh();
	wgetch(make);
	delwin(make);
	noecho();
}

void data::show_friends()
{
	WINDOW *make;
	make = newwin(17, 50, 9, 2);
	box(make, ACS_VLINE, ACS_HLINE);
	init_pair(10, COLOR_GREEN, COLOR_BLACK);
	wbkgd(make, COLOR_PAIR(10));

	for(int i=0; i<myinfo.number; i++)
	{
		mvwprintw(make, 2+i, 2, "id : %s, name : %s, major : %s", friends[i].id, friends[i].name, friends[i].uni_major);
	}
	refresh();
	wgetch(make);
	delwin(make);
}

void data::show_friends2()
{
	attron(COLOR_PAIR(7));
	mvprintw(1, 54, " User ID ");
	mvprintw(2, 54, " ");
	mvprintw(2, 55, "%-14s", myself.myinfo.id);
	mvprintw(4, 54, "   Friend List   ");
	for(int i=0; i<myinfo.number; i++)
	{
		mvprintw(5+i, 54, " ");
		mvprintw(5+i, 55, "%2d. %-10s", i+1, friends[i].id);
	}
	attroff(COLOR_PAIR(7));
	refresh();
}

void data::recommend(data_all people[])
{
	WINDOW *make;
	make = newwin(17, 50, 9, 2);
	box(make, ACS_VLINE, ACS_HLINE);
	init_pair(10, COLOR_GREEN, COLOR_BLACK);
	wbkgd(make, COLOR_PAIR(10));

	int i, j;
	for(i=0; i<total; i++)
	{
		if(strcmp(people[i].myinfo.uni_name, myself.myinfo.uni_name) == 0)
			people[i].myinfo.possible += 20;
		if(strcmp(people[i].myinfo.uni_major, myself.myinfo.uni_major) == 0)
			people[i].myinfo.possible += 10;
		if(strcmp(people[i].myinfo.uni_grade, myself.myinfo.uni_grade) == 0)
			people[i].myinfo.possible += 9;
		if(strcmp(people[i].myinfo.residence, myself.myinfo.uni_grade) == 0)
			people[i].myinfo.possible += 8;
		if(strcmp(people[i].myinfo.birth_place, myself.myinfo.birth_place) == 0)
			people[i].myinfo.possible += 7;
		if(strcmp(people[i].myinfo.club, myself.myinfo.club) == 0)
			people[i].myinfo.possible += 6;
		for(j=0; j<myself.myinfo.number; j++)
		{
			if(strcmp(people[i].myinfo.id, myself.list[j]) == 0)
			{
				people[i].myinfo.possible = 100;
			}
			if(strcmp(people[i].myinfo.id, myself.myinfo.id) == 0)
			{
				people[i].myinfo.possible = 100;
			}
		}
		people[i].myinfo.possible += 10;
	}


	j=0;
	for(i=0; i<total; i++)
	{
		if(people[i].myinfo.possible < 100)
		{
			mvwprintw(make, 2+j, 2, "%2d : %s ( Possibility of Friend : %d )", ++j, people[i].myinfo.id, people[i].myinfo.possible);
		}
	}

	refresh();
	wgetch(make);
	delwin(make);
}

int makeid()
{
	WINDOW *make;
	make = newwin(17, 50, 9, 2);
	box(make, ACS_VLINE, ACS_HLINE);
	init_pair(10, COLOR_GREEN, COLOR_BLACK);
	wbkgd(make, COLOR_PAIR(10));
	echo();

	MYSQL *connection=NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	info user;
	char query1[255];
	char query2[255];
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DBHOST, DBUSER, DBPASS, DBNAME, 3306, (char *)NULL, 0);
	if(connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}
	while(1)
	{
		mvwprintw(make, 2, 2, "Enter ID : ");
		mvwgetstr(make, 2, 24, user.id);
		sprintf(query1, "select * from userinfo where id='%s'",user.id);
		query_stat = mysql_query(connection, query1);
		sql_result = mysql_store_result(connection);
		if((sql_row = mysql_fetch_row(sql_result))==NULL)
		{
			mvwprintw(make, 1, 2, "Available ID.");
			mysql_free_result(sql_result);
			break;
		}
		else
		{
			mysql_free_result(sql_result);
			mvwprintw(make, 1, 2, "ID already registered.");
		}
	}

	noecho();
	mvwprintw(make, 3, 2, "Enter Password : ");
	mvwgetstr(make, 3, 26, user.pwd);

	echo();
	mvwprintw(make, 4, 2, "Enter Name : ");
	mvwgetstr(make, 4, 22, user.name);

	mvwprintw(make, 5, 2, "Enter University : ");
	mvwgetstr(make, 5, 22, user.uni_name);

	mvwprintw(make, 6, 2, "Enter Major : ");
	mvwgetstr(make, 6, 22, user.uni_major);

	mvwprintw(make, 7, 2, "Enter Grade : ");
	mvwgetstr(make, 7, 22, user.uni_grade);

	mvwprintw(make, 8, 2, "Enter Student ID : ");
	mvwgetstr(make, 8, 22, user.uni_id);

	mvwprintw(make, 9, 2, "Enter Gender : ");
	mvwgetstr(make, 9, 22, user.gender);

	mvwprintw(make, 10, 2, "Enter Birthday : ");
	mvwgetstr(make, 10, 22, user.birth_date);

	mvwprintw(make, 11, 2, "Enter Residence : ");
	mvwgetstr(make, 11, 24, user.residence);

	mvwprintw(make, 12, 2, "Enter Birthplace : ");
	mvwgetstr(make, 12, 24, user.birth_place);

	mvwprintw(make, 13, 2, "Enter Phone No. : ");
	mvwgetstr(make, 13, 24, user.phone);

	mvwprintw(make, 14, 2, "Enter Club : ");
	mvwgetstr(make, 14, 24, user.club);

	mvwprintw(make, 15, 2, "Enter Intro : ");
	mvwgetstr(make, 15, 20, user.intro);

	sprintf(query2, "insert into userinfo values ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','0','0','0','not')", user.id,user.pwd,user.name,user.uni_name,user.uni_major,user.uni_grade,user.uni_id,user.gender,user.birth_date,user.residence,user.birth_place,user.phone,user.club,user.intro);
	query_stat = mysql_query(connection, query2);

	mysql_close(connection);

	refresh();
	wgetch(make);
	delwin(make);
	noecho();
	return 0;
}


int changeinfo()
{
	WINDOW *make;
	make = newwin(17, 50, 9, 2);
	box(make, ACS_VLINE, ACS_HLINE);
	init_pair(10, COLOR_GREEN, COLOR_BLACK);
	wbkgd(make, COLOR_PAIR(10));
	echo();

	MYSQL *connection=NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	char pwd[25];
	char query[255];
	info user;

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DBHOST, DBUSER, DBPASS, DBNAME, 3306, (char *)NULL, 0);
	if(connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}
	strcpy(user.id, myself.myinfo.id);

	noecho();
	while(1)
	{
		mvwprintw(make, 2, 2, "Enter Password : ");
		mvwgetstr(make, 2, 26, pwd);

		if(strcmp(myself.myinfo.pwd, pwd) != 0)
		{	
			mvwprintw(make, 1, 2, "Password Failed. Try Again.");
		}
		else
		{
			mvwprintw(make, 1, 2, "                              ");
			break;
		}
	}
	echo();
	mvwprintw(make, 3, 2, "Enter University : ");
	mvwgetstr(make, 3, 22, user.uni_name);

	mvwprintw(make, 4, 2, "Enter Major : ");
	mvwgetstr(make, 4, 22, user.uni_major);

	mvwprintw(make, 5, 2, "Enter Grade : ");
	mvwgetstr(make, 5, 22, user.uni_grade);

	mvwprintw(make, 6, 2, "Enter Student ID : ");
	mvwgetstr(make, 6, 22, user.uni_id);

	mvwprintw(make, 7, 2, "Enter Residence : ");
	mvwgetstr(make, 7, 24, user.residence);

	mvwprintw(make, 8, 2, "Enter Phone No. : ");
	mvwgetstr(make, 8, 31, user.phone);

	mvwprintw(make, 9, 2, "Enter Club : ");
	mvwgetstr(make, 9, 24, user.club);

	mvwprintw(make, 10, 2, "Enter Intro. : ");
	mvwgetstr(make, 10, 31, user.intro);

	sprintf(query, "update userinfo set uninamed='%s',unimajor='%s',unigrade='%s',uniid='%s',residence='%s',phone='%s',club='%s',intro='%s' where id='%s'", user.uni_name,user.uni_major,user.uni_grade,user.uni_id,user.residence,user.phone,user.club,user.intro,user.id);

	query_stat = mysql_query(connection, query);
	mysql_close(connection);

	refresh();
	wgetch(make);
	delwin(make);
	noecho();
	return 0;
}


int secede()
{
	WINDOW *make;
	make = newwin(17, 50, 9, 2);
	box(make, ACS_VLINE, ACS_HLINE);
	init_pair(10, COLOR_GREEN, COLOR_BLACK);
	wbkgd(make, COLOR_PAIR(10));
	echo();

	MYSQL *connection=NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	info user;
	char query1[255];
	char query2[255];
	char query3[255];
	char query4[255];
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DBHOST, DBUSER, DBPASS, DBNAME, 3306, (char *)NULL, 0);
	if(connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}
	echo();
	mvwprintw(make, 2, 2, "아이디 : ");
	mvwgetstr(make, 2, 11, user.id);

	mvwprintw(make, 3, 2, "비밀번호 : ");
	mvwgetstr(make, 3, 13, user.pwd);

	sprintf(query1, "select * from userinfo where id='%s' AND pwd='%s'", user.id,user.pwd);

	query_stat = mysql_query(connection, query1);
	sql_result = mysql_store_result(connection);
	if((sql_row = mysql_fetch_row(sql_result)) == NULL)
	{
		mysql_free_result(sql_result);
		mvwprintw(make, 4, 2, "No Available Information.");
	}
	else
	{
		mvwprintw(make, 4, 2, "<%s> has been unregistered.",user.id);
		sprintf(query2, "delete from userinfo where id = '%s'", user.id);
		query_stat = mysql_query(connection, query2);

		sprintf(query3, "delete from friendlist where userid = '%s'", user.id);
		query_stat = mysql_query(connection, query3);

		sprintf(query4, "delete from forums where userid = '%s'", user.id);
		query_stat = mysql_query(connection, query4);
	}
	mysql_close(connection);

	refresh();
	wgetch(make);
	delwin(make);
	noecho();
	return 0;
}

int join()
{
	WINDOW *make;
	make = newwin(5, 30, 10, 5);
	box(make, ACS_VLINE, ACS_HLINE);
	init_pair(10, COLOR_GREEN, COLOR_BLACK);
	wbkgd(make, COLOR_PAIR(10));
	echo();

	MYSQL *connection=NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	info user;
	int num;
	char *t1,*t2,*t3;
	char query1[255];
	char query2[255];
	char query3[255];
	t1 = (char*)malloc(100);
	t2 = (char*)malloc(100);
	t3 = (char*)malloc(100);

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DBHOST, DBUSER, DBPASS, DBNAME, 3306, (char *)NULL, 0);

	mvwprintw(make, 2, 2, " ID  : ");
	mvwgetstr(make, 2, 13, user.id);

	noecho();
	mvwprintw(make, 3, 2, "Password : ");
	mvwgetstr(make, 3, 13, user.pwd);
	echo();

	sprintf(query1, "select * from userinfo where id='%s' AND pwd='%s'", user.id,user.pwd);

	query_stat = mysql_query(connection, query1);
	sql_result = mysql_store_result(connection);
	if((sql_row = mysql_fetch_row(sql_result)) == NULL)
	{
		mvwprintw(make, 1, 2, "Wrong ID or Password.");
		num = 1;
	}
	else
	{
		sprintf(query2, "update userinfo set login='1' where id='%s'",user.id);
		query_stat = mysql_query(connection, query2);
		mvwprintw(make, 1, 2, "LOGIN SUCCESSFUL!!!!\n");
		num = 2;
		sprintf(query3, "select * from userinfo where id='%s'", user.id);
		query_stat = mysql_query(connection, query3);
		sql_result = mysql_store_result(connection);
		sql_row = mysql_fetch_row(sql_result);
		strcpy(myself.myinfo.id, sql_row[0]);
		strcpy(myself.myinfo.pwd, sql_row[1]);
		strcpy(myself.myinfo.name, sql_row[2]);
		strcpy(myself.myinfo.uni_name, sql_row[3]);
		strcpy(myself.myinfo.uni_major, sql_row[4]);
		strcpy(myself.myinfo.uni_grade, sql_row[5]);
		strcpy(myself.myinfo.uni_id, sql_row[6]);
		strcpy(myself.myinfo.gender, sql_row[7]);
		strcpy(myself.myinfo.birth_date, sql_row[8]);
		strcpy(myself.myinfo.residence, sql_row[9]);
		strcpy(myself.myinfo.birth_place, sql_row[10]);
		strcpy(myself.myinfo.phone, sql_row[11]);
		strcpy(myself.myinfo.club, sql_row[12]);
		strcpy(myself.myinfo.intro, sql_row[13]);
		strcpy(t1, sql_row[14]);
		myself.myinfo.possible = atoi(t1);
		strcpy(t2, sql_row[15]);
		myself.myinfo.number = atoi(t2);
		strcpy(t3, sql_row[16]);
		myself.myinfo.login = atoi(t3);
	}
	mysql_close(connection);
	//sleep(1);
	refresh();
	wgetch(make);
	delwin(make);
	noecho();
	return num;
}


void insert_data_all()
{
	MYSQL *connection=NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	int i=0;
	char* t1, *t2;
	t1 = (char*)malloc(100);
	t2 = (char*)malloc(100);

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DBHOST, DBUSER, DBPASS, DBNAME, 3306, (char *)NULL, 0);
	query_stat = mysql_query(connection, "select * from userinfo");
	sql_result = mysql_store_result(connection);
	while((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		strcpy(people[i].myinfo.id, sql_row[0]);
		strcpy(people[i].myinfo.pwd, sql_row[1]);
		strcpy(people[i].myinfo.name, sql_row[2]);
		strcpy(people[i].myinfo.uni_name, sql_row[3]);
		strcpy(people[i].myinfo.uni_major, sql_row[4]);
		strcpy(people[i].myinfo.uni_grade, sql_row[5]);
		strcpy(people[i].myinfo.uni_id, sql_row[6]);
		strcpy(people[i].myinfo.gender, sql_row[7]);
		strcpy(people[i].myinfo.birth_date, sql_row[8]);
		strcpy(people[i].myinfo.residence, sql_row[9]);
		strcpy(people[i].myinfo.birth_place, sql_row[10]);
		strcpy(people[i].myinfo.phone, sql_row[11]);
		strcpy(people[i].myinfo.club, sql_row[12]);
		strcpy(people[i].myinfo.intro, sql_row[13]);
		strcpy(t1, sql_row[14]);
		people[i].myinfo.possible = atoi(t1);
		strcpy(t2, sql_row[15]);
		people[i].myinfo.number = atoi(t2);
		i++;
	}
	total = i;
	mysql_free_result(sql_result);
	mysql_close(connection);
}

void insert_friend(info friendinfo, int number)
{
	MYSQL *connection=NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	char query1[255];
	char query2[255];
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DBHOST, DBUSER, DBPASS, DBNAME, 3306, (char *)NULL, 0);

	sprintf(query1, "insert into friendlist values ('%s','%s')", myself.myinfo.id, friendinfo.id);
query_stat = mysql_query(connection, query1);

	sprintf(query2, "update userinfo set number='%d' where id='%s'", number,  myself.myinfo.id);
	query_stat = mysql_query(connection, query2);

	mysql_close(connection);
}

int load_friendlist()
{
	MYSQL *connection=NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	int i=0;
	char query[255];
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DBHOST, DBUSER, DBPASS, DBNAME, 3306, (char *)NULL, 0);
	sprintf(query, "select * from friendlist where userid = '%s'", myself.myinfo.id);
	query_stat = mysql_query(connection, query);
	sql_result = mysql_store_result(connection);

	while((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		strcpy(myself.list[i], sql_row[1]);
		i++;
	}

	mysql_close(connection);
	return i;
}

void load_friend()
{
	MYSQL *connection=NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	int i=0, j, k;
	char* t1, *t2;
	t1 = (char*)malloc(100);
	t2 = (char*)malloc(100);

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DBHOST, DBUSER, DBPASS, DBNAME, 3306, (char *)NULL, 0);
	query_stat = mysql_query(connection, "select * from userinfo");
	sql_result = mysql_store_result(connection);
	while((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		k=0;
		for(j=0;j<myself.myinfo.number;j++)
		{
			if(strcmp(myself.list[j], sql_row[0]) == 0)
				k++;
		}
		if(k!=0)
		{
			strcpy(myself.friends[i].id, sql_row[0]);
			strcpy(myself.friends[i].pwd, sql_row[1]);
			strcpy(myself.friends[i].name, sql_row[2]);
			strcpy(myself.friends[i].uni_name, sql_row[3]);
			strcpy(myself.friends[i].uni_major, sql_row[4]);
			strcpy(myself.friends[i].uni_grade, sql_row[5]);
			strcpy(myself.friends[i].uni_id, sql_row[6]);
			strcpy(myself.friends[i].gender, sql_row[7]);
			strcpy(myself.friends[i].birth_date, sql_row[8]);
			strcpy(myself.friends[i].residence, sql_row[9]);
			strcpy(myself.friends[i].birth_place, sql_row[10]);
			strcpy(myself.friends[i].phone, sql_row[11]);
			strcpy(myself.friends[i].club, sql_row[12]);
			strcpy(myself.friends[i].intro, sql_row[13]);
			strcpy(t1, sql_row[14]);
			myself.friends[i].possible = atoi(t1);
			strcpy(t2, sql_row[15]);
			myself.friends[i].number = atoi(t2);
			i++;
		}
	}
	mysql_free_result(sql_result);
	mysql_close(connection);
}

void write_post()
{
	WINDOW *make;
	make = newwin(17, 50, 9, 2);
	box(make, ACS_VLINE, ACS_HLINE);
	init_pair(10, COLOR_GREEN, COLOR_BLACK);
	wbkgd(make, COLOR_PAIR(10));
	echo();

	MYSQL *connection=NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	char text[100];
	char query[255];

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DBHOST, DBUSER, DBPASS, DBNAME, 3306, (char *)NULL, 0);
	echo();
	mvwprintw(make, 2, 2, "Newsfeed Post : ");
	mvwgetstr(make, 2, 19, text);

	sprintf(query, "insert into forum values ('NULL','%s','%s','0')", myself.myinfo.id, text);
	query_stat = mysql_query(connection, query);

	mysql_close(connection);
	refresh();
	wgetch(make);
	delwin(make);
}
void logout()
{
	MYSQL *connection=NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	char query[255];

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DBHOST, DBUSER, DBPASS, DBNAME, 3306, (char *)NULL, 0);

	sprintf(query, "update userinfo set login='0', number='%d', ip='not' where id='%s'", myself.myinfo.number, myself.myinfo.id);
	query_stat = mysql_query(connection, query);
	mvprintw(15, 30, "< Log out >");
	refresh();
	mysql_close(connection);
	sleep(2);
	endwin();
	exit(1);
}


void show_post()
{
	WINDOW *make;
	make = newwin(17, 50, 9, 2);
	box(make, ACS_VLINE, ACS_HLINE);
	init_pair(10, COLOR_GREEN, COLOR_BLACK);
	wbkgd(make, COLOR_PAIR(10));


	MYSQL *connection=NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	char query[255];
	int num=0, i=0;

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DBHOST, DBUSER, DBPASS, DBNAME, 3306, (char *)NULL, 0);

	query_stat = mysql_query(connection, "select * from forum ORDER BY no DESC");
	sql_result = mysql_store_result(connection);

	while((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		mvwprintw(make, i+1, 2, "[ %s ] : %s", sql_row[1], sql_row[2]);
		if(++i == 14)
			break;
	}
	mysql_free_result(sql_result);
	mysql_close(connection);

	refresh();
	wgetch(make);
	delwin(make);
	noecho();
}

////////////////////////////////////////////////
void draw(int (*show)[70])
{
	int i, j;
	clear();
	for(i=0; i<35; i++)
	{
		for(j=0; j<70; j++)
		{
			if(show[i][j] == 1)
				mvprintw(i, j, " ");
		}
	}
}
void findinfo()
{
	WINDOW *make;
	make = newwin(17, 50, 9, 2);
	box(make, ACS_VLINE, ACS_HLINE);
	init_pair(10, COLOR_GREEN, COLOR_BLACK);
	wbkgd(make, COLOR_PAIR(10));
	echo();

	MYSQL *connection=NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	info user;
	char query[255];

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DBHOST, DBUSER, DBPASS, DBNAME, 3306, (char *)NULL, 0);

	mvwprintw(make, 2, 2, "Enter ID : ");
	mvwgetstr(make, 2, 22, user.name);

	mvwprintw(make, 3, 2, "Enter Phone No. : ");
	mvwgetstr(make, 3, 29, user.phone);

	sprintf(query, "select * from userinfo where name='%s' AND phone='%s'", user.name, user.phone);

	query_stat = mysql_query(connection, query);
	sql_result = mysql_store_result(connection);
	if((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		mvwprintw(make, 5, 2, "ID : %s", sql_row[0]);
		mvwprintw(make, 6, 2, "Password : %s", sql_row[1]);
	}
	else
	{
		mvwprintw(make, 5, 2, "Wrong Name or Password");
	}
	mysql_free_result(sql_result);
	mysql_close(connection);

	refresh();
	wgetch(make);
	delwin(make);
	noecho();
}
void getmyIP()
{
	int myfd;
	struct ifreq ifr;

	myfd = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
	ioctl(myfd, SIOCGIFADDR, &ifr);

	close(myfd); 

	MYSQL *connection=NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	char query[255];

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DBHOST, DBUSER, DBPASS, DBNAME, 3306, (char *)NULL, 0);

	sprintf(query, "update userinfo set ip='%s' where id='%s'", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), myself.myinfo.id);
	query_stat = mysql_query(connection, query);
	mysql_close(connection);
}

char* setfriendIP(info friendy)
{
	MYSQL *connection=NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
	char query[255];
	char *friendIP;

	friendIP = (char*)malloc(100);

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DBHOST, DBUSER, DBPASS, DBNAME, 3306, (char *)NULL, 0);

	sprintf(query, "select * from userinfo where id='%s'", friendy.id);
	query_stat = mysql_query(connection, query);
	sql_result = mysql_store_result(connection);
	sql_row = mysql_fetch_row(sql_result);

	if(strcmp(sql_row[17],"not")!=0)
	{
		strcpy(friendIP, sql_row[17]);
		printf("Connect Chatting.\n");
		printf("%s\n",friendIP);
	}		
	else
	{
		strcpy(friendIP, "not");
		printf("Friend Logged Out.\n");
	}

	mysql_free_result(sql_result);
	mysql_close(connection);

	return friendIP;
}

///////////////////////////////////////
void *chatting_server(void *arg)
{
	struct sockaddr_in cliaddr;
	char buf[MAXLINE+1];
	int i, j, nbyte, accp_sock;
	int addrlen = sizeof(struct sockaddr_in);
	fd_set read_fds;
	listen_sock = tcp_listen(INADDR_ANY, PORT, 5);
	while(1)
	{
		FD_ZERO(&read_fds);
		FD_SET(listen_sock, &read_fds);

		for(i=0;i<num_chat;i++)
			FD_SET(clisock_list[i],&read_fds);

		maxfdp1 = getmax() + 1;

		if(select(maxfdp1, &read_fds, NULL,NULL,NULL)<0)
			errquit("select fail");

		if(FD_ISSET(listen_sock, &read_fds))
		{
			accp_sock = accept(listen_sock, (struct sockaddr*)&cliaddr, (socklen_t *)&addrlen);
			if(accp_sock == -1)
				errquit("accept fail");
			addClient(accp_sock, &cliaddr);
			send(accp_sock, START_STRING, strlen(START_STRING),0);
		}

		for(i=0;i<num_chat;i++)
		{
			if(FD_ISSET(clisock_list[i],&read_fds))
			{
				nbyte = recv(clisock_list[i], buf, MAXLINE, 0);
				if(nbyte<=0)
				{
					removeClient(i);
					continue;
				}
				buf[nbyte] = 0;

				if(strstr(buf, EXIT_STRING)!=NULL)
				{
					removeClient(i);
					continue;
				}

				for( j=0;j<num_chat;j++)
					send (clisock_list[j],buf,nbyte,0);
			}
		}
	}
}

void addClient(int s, struct sockaddr_in *newcliaddr)
{
	char buf[20];

	inet_ntop(AF_INET, &newcliaddr->sin_addr, buf, sizeof(buf));

	clisock_list[num_chat] = s;
	num_chat++;
}

void removeClient(int s)
{
	close(clisock_list[s]);
	if(s!=num_chat-1)
		clisock_list[s] = clisock_list[num_chat -1];
	num_chat--;
}

int getmax()
{
	int max = listen_sock;
	int i;

	for(i=0;i<num_chat;i++)
		if(clisock_list[i]>max)
			max = clisock_list[i];
	return max;
}

int tcp_listen(int host, int port, int backlog)
{
	int sd;
	struct sockaddr_in servaddr;

	if((sd = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		perror("socket fail");
		exit(1);
	}

	bzero((char*)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(host);
	servaddr.sin_port = htons(port);

	if(bind(sd, (struct sockaddr*)&servaddr, sizeof(servaddr)) <0)
	{
		perror("bind fail");
		exit(1);
	}

	listen(sd, backlog);
	return sd;
}


/////////////////////////////////////////////////////
void chatting_client(char *uip)
{
	char bufall[ MAXLINE + NAME_LEN ];
	char *bufmsg;
	char* buff;
	int maxfdp1;
	int s;
	int namelen;
	fd_set read_fds;

	sprintf( bufall, "[ %s ] : ", myself.myinfo.id );
	namelen = strlen( bufall );
	bufmsg  =  bufall + namelen;

	s  =  tcp_connect( AF_INET, "127.0.0.1", PORT );

	if( s == -1 )
		errquit( "tcp_connect fail" );

	puts( "Connected Server" );
tcflush(0, TCIFLUSH);
fflush(stdout);
	maxfdp1 = s + 1;
	FD_ZERO( &read_fds );

	while( 1 )
	{
		FD_SET( 0, &read_fds );
		FD_SET( s, &read_fds );

		if( select( maxfdp1, &read_fds, NULL, NULL, NULL )<0 )
			errquit( "select fail" );
		if( FD_ISSET( s, &read_fds ) )
		{
			int nbyte;
			if( ( nbyte  =  recv( s, bufmsg, MAXLINE, 0 ) )>0 )
			{
				bufmsg[ nbyte ] = '\0';

				buff = bufmsg+strlen(bufmsg)-5; 
				if(strcmp("exit\n", bufmsg+strlen(bufmsg)-5 ) == 0 )
				{
					printf("Other User has been Logged out.\n");
					tcflush(0, TCIFLUSH);
					fflush(stdout);
				}
				else
				{
					printf( "%s", bufmsg, strlen(bufmsg) ); /////////////////
	tcflush(0, TCIFLUSH);
	fflush(stdout);
					record_history(bufmsg, strlen(bufmsg));
				}
			}
		}
		if( FD_ISSET( 0, &read_fds ) )
		{
			if( fgets( bufmsg, MAXLINE, stdin ) ) ///////////////
			{
				if( send( s, bufall, namelen + strlen( bufmsg ), 0 )<0 )
					puts( "error : write error on socket." );
				if( strstr( bufmsg, EXIT_STRING_2 ) != NULL )
				{
				//	save_history();
					puts( "Good bye." );
					close( s );
					break;
			//		main2();
				//	return;
				//	exit( 0 );
				}
			//	record_history(bufmsg, strlen(bufmsg));
			}
		}
	}
	main2();
}

int tcp_connect( int af, char *servip, unsigned short port )
{
	struct sockaddr_in servaddr;
	int s;

	if( ( s = socket( af, SOCK_STREAM, 0 ) )<0 )
		return -1;

	bzero( ( char* )&servaddr, sizeof( servaddr ) );
	servaddr.sin_family = af;
	inet_pton( AF_INET, servip, &servaddr.sin_addr );
	servaddr.sin_port  =  htons( port );

	if( connect( s, ( struct sockaddr * )&servaddr, sizeof( servaddr ) )<0 )
		return -1;
	return s;
}

void record_history(char* bufmsg, int bufmsg_len)
{
	history_fd = open("history", O_RDWR | O_APPEND | O_CREAT, 0666);
	write(history_fd, bufmsg, bufmsg_len);
	close(history_fd);
}

void save_history(void)
{
	char yn;
	char file_name[21];

	while(1)
	{
		printf("Do you want to save this chatting history? [y/n]");
		scanf("%c", &yn);

		if(yn == 'n')
		{
			unlink("history");
			break;
		}
		else if(yn == 'y')
		{
			printf("Type a name of the file(using under 21 charaters) ==> ");
			scanf("%s", file_name);

			rename("history", file_name);
			break;
		}
		else
		{
			printf("Type again!\n");
			continue;
		}
	}
}

