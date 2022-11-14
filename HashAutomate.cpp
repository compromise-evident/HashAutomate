/// HashAutomate - downloads from links in a file, prints sha256sum of individual and 1 big hash of all.
/// Nikolay Valentinovich Repnitskiy - License: WTFPLv2+ (wtfpl.net)


/* Version 2.0.0.  Create the text file  "Links"  with 1 download link per line.
Max# of links if 10^6-Byte links: 10^12. Max# of links if 100-Byte links: 10^16.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
How to run the program  -  Software package repositories for GNU+Linux operating
systems have all the tools you can imagine. Open a terminal and use this command
as root to install Geany and g++ on your computer: apt install geany g++   Geany
is a fast & lightweight text editor and Integrated Development Environment where
you can write and run code. g++ is the GNU compiler for C++ which allows written
code to run. The compiler operates in the background and displays errors in your
code as you will see in the lower Geany box. Make a new folder somewhere on your
machine. Paste this code into Geany. For clarity in auditing, enable indentation
guides: go to View >> Show Indentation Guides. Save the document as anything.cpp
within the newly-created folder. Use these shortcuts to run the program: F9, F5.
You may paste over this code with other  .cpp files, or open a new tab & repeat.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
How to make an executable with g++  -  Save this program as anything.cpp, open a
terminal, and type g++ then space. Drag & drop this saved file into the terminal
and its directory will be  appended to your pending entry. Click on the terminal
and press enter.   a.out now resides in the user directory, you may rename it to
anything.  To run that executable, simply drag and drop it into a terminal, then
click on the terminal and press enter.  Reminder:  executable's effect-directory
is the user directory on your machine, for example:  /home/nikolay    Enjoy. */

#include <fstream>
#include <iostream>
using namespace std;

int main()
{	cout << "\n(HashAutomate)\n\n";
	ifstream in_stream;
	ofstream out_stream;
	
	//Checks if file "Links" is missing or empty.
	in_stream.open("Links");
	if(in_stream.fail() == true) {cout << "File \"Links\" is missing."; in_stream.close(); return 0;}
	char garbage_byte;
	in_stream.get(garbage_byte);
	if(in_stream.eof() == true)  {cout << "File \"Links\" is empty.";   in_stream.close(); return 0;}
	in_stream.close();
	
	//Counts number of links.
	in_stream.open("Links");
	long number_of_links = 0;
	char temp_garbage_byte;
	for(; in_stream.eof() == false;)
	{	if(garbage_byte == '\n') {number_of_links++;}
		temp_garbage_byte = garbage_byte;
		in_stream.get(garbage_byte);
	}
	in_stream.close();
	
	if(temp_garbage_byte != '\n') {number_of_links++;}
	
	//Downloads everything and makes a singular hash of everything, as well as sha256sum of individual.
	int hash_of_all_files[1280] = {0};
	long links_read_bookmark = -1;
	bool at_least_1_download_problem = false;
	for(int a = 0; a < number_of_links; a++)
	{	//..........Loads a link.
		in_stream.open("Links");
		links_read_bookmark++;
		for(int b = 0; b < links_read_bookmark; b++) {in_stream.get(garbage_byte);} //..........Skips link Bytes that have been read.
		char wget[1000015] = {"wget -q -t 9 "};
		int wget_write_bookmark = 13;
		in_stream.get(garbage_byte);
		for(; garbage_byte != '\n';)
		{	wget[wget_write_bookmark] = garbage_byte;
			wget_write_bookmark++;
			links_read_bookmark++;
			in_stream.get(garbage_byte);
			
			if(in_stream.eof() == true) {break;}
		}
		in_stream.close();
		
		//..........Appends " -O temp" then null.
		wget[wget_write_bookmark    ] = ' ';
		wget[wget_write_bookmark + 1] = '-';
		wget[wget_write_bookmark + 2] = 'O';
		wget[wget_write_bookmark + 3] = ' ';
		wget[wget_write_bookmark + 4] = 't';
		wget[wget_write_bookmark + 5] = 'e';
		wget[wget_write_bookmark + 6] = 'm';
		wget[wget_write_bookmark + 7] = 'p';
		wget[wget_write_bookmark + 8] ='\0';
		
		//..........Downloads & prints the URL
		system(wget);
		for(int b = 13; b < wget_write_bookmark; b++) {cout << wget[b];}
		cout << "\n";
		
		//..........Checks if downloaded.
		in_stream.open("temp");
		bool download_problem = false;
		if(in_stream.fail() == true) {download_problem = true;}
		in_stream.get(garbage_byte);
		if(in_stream.eof() == true)  {download_problem = true;}
		in_stream.close();
		
		//..........Gets sha256sum.
		if(download_problem == true) {at_least_1_download_problem = true; cout << "FAILED!\n\n\n";}
		else
		{	system("sha256sum temp > temp_256");
			in_stream.open("temp_256");
			cout << "sha256sum = ";
			for(int a = 0; a < 64; a++)
			{	in_stream.get(garbage_byte);
				cout << char(garbage_byte);
			}
			in_stream.close();
			
			cout << "\n(" << (a + 1) << " of " << number_of_links << " OK)\n\n\n";
		}
		
		//..........Continues adding to the big hash.
		if(at_least_1_download_problem == false)
		{	system("sha512sum temp > temp_512");
			int hash_of_individual_file[1280] = {0};
			
			in_stream.open("temp_256"); //..........First, applies sha256sum to hash_of_individual_file[].
			if(in_stream.fail() == true) {cout << "\n\n\nFAILED! (1)\n\n\n"; in_stream.close(); return 0;}
			in_stream.get(garbage_byte);
			if(in_stream.eof() == true)  {cout << "\n\n\nFAILED! (2)\n\n\n"; in_stream.close(); return 0;}
			for(; in_stream.eof() == false;)
			{	srand(garbage_byte);
				
				if((garbage_byte % 2) == 0) //..........Left-to-right.
				{	for(int b = 0; b < 1280; b++)
					{	hash_of_individual_file[b] += (rand() % 95);
						hash_of_individual_file[b] %= 95;
					}
				}
				else
				{	for(int b = 1279; b >= 0; b--) //..........Right-to-left.
					{	hash_of_individual_file[b] += (rand() % 95);
						hash_of_individual_file[b] %= 95;
					}
				}
				
				in_stream.get(garbage_byte);
			}
			in_stream.close();
			
			in_stream.open("temp_512"); //..........Additionally, applies sha512sum to hash_of_individual_file[].
			if(in_stream.fail() == true) {cout << "\n\n\nFAILED! (3)\n\n\n"; in_stream.close(); return 0;}
			in_stream.get(garbage_byte);
			if(in_stream.eof() == true)  {cout << "\n\n\nFAILED! (4)\n\n\n"; in_stream.close(); return 0;}
			for(; in_stream.eof() == false;)
			{	srand(garbage_byte);
				
				if((garbage_byte % 2) == 0) //..........Left-to-right.
				{	for(int b = 0; b < 1280; b++)
					{	hash_of_individual_file[b] += (rand() % 95);
						hash_of_individual_file[b] %= 95;
					}
				}
				else
				{	for(int b = 1279; b >= 0; b--) //..........Right-to-left.
					{	hash_of_individual_file[b] += (rand() % 95);
						hash_of_individual_file[b] %= 95;
					}
				}
				
				in_stream.get(garbage_byte);
			}
			in_stream.close();
			
			//..........Constructively combines hash.
			for(int b = 0; b < 1280; b++)
			{	hash_of_all_files[b] += hash_of_individual_file[b];
				hash_of_all_files[b] %= 95;
			}
		}
		
		remove("temp");
	}
	
	remove("temp_256");
	remove("temp_512");
	system("sleep 1" );
	if(at_least_1_download_problem == true) {cout << "\n\nAt least 1 download failed; the big hash is useless.";}
	else
	{	//Prints big hash.
		cout << "\n-------------1,280-character hash of all files REGARDLESS OF ORDER:-------------\n";
		for(int a = 0; a < 1280; a++) {cout << char(hash_of_all_files[a] + 32);}
	}
}
