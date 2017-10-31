/*
	Copyright (C) 2017 Minho Jo <whitestone8214@openmailbox.org>
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/* gcc length.c -o length */


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


void usage () {
	printf ("\n\n");
	printf ("length - Measures and prints out the length of given string or file.\n");
	printf ("\n");
	printf ("Usage: length [TARGET] [value1, value2, ...]\n");
	printf ("\n");
	printf ("TARGET:\n");
	printf ("input: Do the work against the given values itself (Prints out the total length of given values)\n");
	printf ("file: Do the work against the given files (Prints out the total length of given files)\n");
	printf ("\n\n");
}

int main (int argc, char **argv) {
	if (argc < 3) {usage (); return 1;}
	if ((strcmp (argv[1], "input") != 0) && (strcmp (argv[1], "file") != 0)) {usage (); return 1;}
	
	int _given = argc - 2;
	int _total = 0;
	if (strcmp (argv[1], "input") == 0) {
		int _nth; for (_nth = 2; _nth < argc; _nth++) _total += strlen (argv[_nth]);
	}
	else {
		int _nth; for (_nth = 2; _nth < argc; _nth++) {
			struct stat _profile;
			if (stat (argv[_nth], &_profile) != 0) {printf ("ERROR: %s (%s)\n", strerror (errno), argv[_nth]); usage (); return 1;}
			_total += _profile.st_size;
		}
	}
	
	printf ("%d", _total);
	return 0;
}
