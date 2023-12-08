/// HashAutomate - detect if file hosts changed something. Downloads from links in a file, saves
///                sha256sum of individual and 1 big hash of all, regardless of link order.
/// Nikolay Valentinovich Repnitskiy - License: WTFPLv2+ (wtfpl.net)


/* Version 2.0.2
#########*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*##########
#####'`                                                                  `'#####
###'                                                                        '###
##                                                                            ##
#,                         Have a text file ready with                        ,#
#'                          1 download link per line.                         '#
##                                                                            ##
###,                                                                        ,###
#####,.                                                                  .,#####
##########*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#######*/

#include <fstream>
#include <iostream>
using namespace std;

int main()
{	ifstream in_stream;
	ofstream out_stream;
	
	//Gets path to file from user.
	cout << "\nMake a file with 1 download link per line.\n"
	     << "Drag & drop file into terminal or enter path:\n";
	char  path_to_file[10000];
	for(int a = 0; a < 10000; a++) {path_to_file[a] = '\0';} //Fills path_to_file[] with null.
	//char catching_new_line[1]; //Uncomment these 2 if you make cin >> where you press enter before dropping path.
	//cin.getline(catching_new_line, 1);
	cin.getline(path_to_file, 10000);
	if(path_to_file[0] == '\0') {cout << "\nNo path given.\n"; return 0;}
	
	//Fixes path to file if drag & dropped (removes single quotes for ex:)   '/home/nikolay/my documents/links'
	if(path_to_file[0] == '\'')
	{	for(int a = 0; a < 10000; a++)
		{	path_to_file[a] = path_to_file[a + 1];
			if(path_to_file[a] == '\'')
			{	path_to_file[a] = '\0';
				path_to_file[a + 1] = '\0';
				path_to_file[a + 2] = '\0';
				break;
			}
		}
	}
	
	//Checks if file exists (failure can be bad path info as well.)
	in_stream.open(path_to_file);
	if(in_stream.fail() == true) {in_stream.close(); cout << "\n\nNo such file or directory.\n";             return 0;}
	char sniffed_one_file_character;
	in_stream.get(sniffed_one_file_character);
	if(in_stream.eof() == true) {in_stream.close();  cout << "\n\nNothing to process, the file is empty.\n"; return 0;}
	in_stream.close();
	
	//Gets location of the first encountered end-null coming from the left in path_to_file[].
	int path_to_file_null_bookmark;
	for(int a = 0; a < 10000; a++) {if(path_to_file[a] == '\0') {path_to_file_null_bookmark = a; break;}}
	
	//Prepares out file stream based on given directory.
	path_to_file[path_to_file_null_bookmark    ] = '-'; //..........Appending -saved to file name in path to file.
	path_to_file[path_to_file_null_bookmark + 1] = 's';
	path_to_file[path_to_file_null_bookmark + 2] = 'a';
	path_to_file[path_to_file_null_bookmark + 3] = 'v';
	path_to_file[path_to_file_null_bookmark + 4] = 'e';
	path_to_file[path_to_file_null_bookmark + 5] = 'd';
	out_stream.open(path_to_file);
	path_to_file[path_to_file_null_bookmark    ] ='\0'; //..........Reverts.
	
	//Counts number of links.
	in_stream.open(path_to_file);
	long number_of_links = 0;
	char garbage_byte;
	char temp_garbage_byte;
	in_stream.get(garbage_byte);
	for(; in_stream.eof() == false;)
	{	if(garbage_byte == '\n') {number_of_links++;}
		temp_garbage_byte = garbage_byte;
		in_stream.get(garbage_byte);
	}
	in_stream.close();
	
	if(temp_garbage_byte != '\n') {number_of_links++;}
	
	//Downloads everything and makes a singular hash of everything, as well as sha256sum of individual.
	cout       << "\n\n";
	out_stream << "(HashAutomate) - Links from " << path_to_file << "\n\n\n";
	int hash_of_all_files[1280] = {0};
	long links_read_bookmark = -1;
	bool at_least_1_download_problem = false;
	for(long a = 0; a < number_of_links; a++)
	{	//..........Loads a link.
		in_stream.open(path_to_file);
		links_read_bookmark++;
		for(long b = 0; b < links_read_bookmark; b++) {in_stream.get(garbage_byte);} //..........Skips link Bytes that have been read.
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
		for(int b = 13; b < wget_write_bookmark; b++)
		{	cout       << wget[b];
			out_stream << wget[b];
		}
		
		cout       << "\n";
		out_stream << "\n";
		
		//..........Checks if downloaded.
		in_stream.open("temp");
		bool download_problem = false;
		if(in_stream.fail() == true) {download_problem = true;}
		in_stream.get(garbage_byte);
		if(in_stream.eof() == true)  {download_problem = true;}
		in_stream.close();
		
		//..........Gets sha256sum.
		if(download_problem == true)
		{	at_least_1_download_problem = true;
			cout       << "FAILED!\n\n\n";
			out_stream << "FAILED!\n\n\n";
		}
		else
		{	system("sha256sum temp > temp_256");
			in_stream.open("temp_256");
			cout       << "sha256sum = ";
			out_stream << "sha256sum = ";
			for(int a = 0; a < 64; a++)
			{	in_stream.get(garbage_byte);
				cout       << char(garbage_byte);
				out_stream << char(garbage_byte);
			}
			in_stream.close();
			
			cout       << "\n(" << (a + 1) << " of " << number_of_links << " OK)\n\n\n";
			out_stream << "\n(" << (a + 1) << " of " << number_of_links << " OK)\n\n\n";
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
	if(at_least_1_download_problem == true)
	{	cout       << "\n\nAt least 1 download failed; the big hash is useless.";
		out_stream << "\n\nAt least 1 download failed; the big hash is useless.";
	}
	else
	{	//Prints big hash.
		cout       << "\n-------------1,280-character hash of all files REGARDLESS OF ORDER:-------------\n";
		out_stream << "\n-------------1,280-character hash of all files REGARDLESS OF ORDER:-------------\n";
		for(int a = 0; a < 1280; a++)
		{	cout       << char(hash_of_all_files[a] + 32);
			out_stream << char(hash_of_all_files[a] + 32);
		}
	}
	
	cout       << "\n\n(all saved to given directory)";
	out_stream << "\n";
	out_stream.close();
}





/*#######*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*##########
#####'`                                                                  `'#####
###'                              TLDR: run it                              '###
##                                                                            ##
#,                                                                            ,#
#'                              apt install g++                               '#
##                           g++ /path/this_file.cpp                          ##
###,                          /path/resulting_file                          ,###
#####,.                                                                  .,#####
##########*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#######*/

/*How to alter this code - Software package repositories for GNU+Linux operating
systems have all the tools you can imagine. Open a terminal and use this command
as root to install Geany and g++ on your computer: apt install geany g++   Geany
is a fast & lightweight text editor and Integrated Development Environment where
you can write and run code. g++ is the GNU compiler for C++  (industry standard)
which creates an executable file out of written code. It also displays errors in
code as you will see in the lower Geany box. Make a new folder somewhere on your
machine. Paste this code into Geany. For clarity in auditing, enable indentation
guides: go to View >> Show Indentation Guides. Save the document as anything.cpp
within the newly-created folder. Use these shortcuts to run the program: F9, F5.
Once  F9  is used, it needs not be used again unless you had modified this code.
You may paste over this code with other  .cpp files, or open a new tab & repeat.
Additionally, executables created by the  F9  command can be drag & dropped into
terminals (lazy directory pasting) so the executable can run.  exe is in folder.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
How to make an executable with g++  -  Save this program as anything.cpp, open a
terminal, and type g++ then space. Drag & drop this saved file into the terminal
and its directory will be  appended to your pending entry. Click on the terminal
and press enter.   a.out now resides in the user directory, you may rename it to
anything.  To run that executable, simply drag and drop it into a terminal, then
click on the terminal and press enter.  Reminder:  executable's effect-directory
is /home/user or where you put the executable. Opening  .cpp  files in Geany and
hitting F9 creates the same executable--movable, renameable, drag & droppable.*/
