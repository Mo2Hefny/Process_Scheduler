#include "Scheduler/Scheduler.h"
#include <conio.h>

void CreateMainMenu()
{
	printf(Text);
	cout << "+";
	for (int i = 0; i < 64; i++)	cout << "-";
	cout << "+\n|";
	printf(Title);
	for (int i = 0; i < 23; i++)    cout << " ";
	cout << "Scheduling Manager";
	for (int i = 0; i < 23; i++)    cout << " ";
	printf(Text);
	cout << "|\n+";
	for (int i = 0; i < 21; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 20; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 21; i++)	cout << "-";
	cout << "+\n|";
	printf(Late);
	cout << "     Nour Khaled     ";
	printf(Text); cout << "|";
	printf(Late);
	cout << "    Moamen Hefny    ";
	printf(Text); cout << "|";
	printf(Late);
	cout << "      Mina Hany      ";
	printf(Text);
	cout << "|\n+";
	for (int i = 0; i < 21; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 20; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 21; i++)	cout << "-";
	cout << "+\n|";
	for (int i = 0; i < 9; i++)    cout << " ";
	cout << "Welcome to our Scheduler Simulator project! :)";
	for (int i = 0; i < 9; i++)    cout << " ";
	cout << "|\n+";
	for (int i = 0; i < 64; i++)	cout << "-";
	cout << "+\n|";
	printf(SmallTitle);
	for (int i = 0; i < 13; i++)    cout << " ";
	cout << "The following are our processes states";
	for (int i = 0; i < 13; i++)    cout << " ";
	printf(Text);
	cout << "|\n+";
	for (int i = 0; i < 16; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 14; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 15; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 16; i++)	cout << "-";
	cout << "+\n|";
	printf(SmallTitle);
	cout << "     Normal     ";
	printf(Text); cout << "|";
	printf(SmallTitle);
	cout << "   Migrated   ";
	printf(Text); cout << "|";
	printf(SmallTitle);
	cout << "    Parents    ";
	printf(Text); cout << "|";
	printf(SmallTitle);
	cout << "     Forked     ";
	printf(Text);
	cout << "|\n+";
	for (int i = 0; i < 16; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 14; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 15; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 16; i++)	cout << "-";
	cout << "+\n|      ";
	printf(Early);
	cout << "Early";
	printf(Text); cout << "     |  ";
	printf(MigEarly);
	cout << "RR/SJF [E]";
	printf(Text); cout << "  |   ";
	printf(Parent);
	cout << "Parent [E]";
	printf(Text); cout << "  |    ";
	printf(Child);
	cout << "Child [E]";
	printf(Text);
	cout << "   |\n+";
	for (int i = 0; i < 16; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 14; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 15; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 16; i++)	cout << "-";
	cout << "+\n|      ";
	printf(Late);
	cout << "Early";
	printf(Text); cout << "     |  ";
	printf(MigLate);
	cout << "RR/SJF [L]";
	printf(Text); cout << "  |   ";
	printf(ParentLate);
	cout << "Parent [L]";
	printf(Text); cout << "  |    ";
	printf(ChildLate);
	cout << "Child [L]";
	printf(Text);
	cout << "   |\n+";
	for (int i = 0; i < 16; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 14; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 15; i++)	cout << "-";
	cout << "+";
	for (int i = 0; i < 16; i++)	cout << "-";
	cout << "+\n";
	_getch();
}

int main()
{
	srand(time(0));
	CreateMainMenu();
	Scheduler* schedule_manager = new Scheduler;

	if (schedule_manager)
		delete schedule_manager;

	return 0;
}