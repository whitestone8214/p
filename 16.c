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


/* gcc 16.c -o 16 */


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


void usage () {
	printf ("\n\n");
	printf ("16 - Prints out given raw data as hexadecimal string, or vice versa.\n");
	printf ("\n");
	printf ("Usage: 16 [TYPE] [DIRECTION] [value1, value2, ...]\n");
	printf ("\n");
	printf ("TYPE:\n");
	printf ("input: Do the work against the given values itself\n");
	printf ("file: Do the work against the contents of given files\n");
	printf ("\n");
	printf ("DIRECTION:\n");
	printf ("encode: Treat given values as raw data, encode as hexadecimal string\n");
	printf ("decode: Treat given values as hexadecimal strings, decode as raw data\n");
	printf ("\n");
	printf ("WARNING:\n");
	printf ("1. When decode, if given value is not valid(i.e. length is an odd number (only for input; for file it will print out error message), has a non-hexadecimal string), it will print out 'gg' and stop working.\n");
	printf ("\n\n");
}

int main (int argc, char **argv) {
	if (argc < 4) {usage (); return 1;}
	if ((strcmp (argv[1], "input") != 0) && (strcmp (argv[1], "file") != 0)) {usage (); return 1;}
	if ((strcmp (argv[2], "encode") != 0) && (strcmp (argv[2], "decode") != 0)) {usage (); return 1;}
	
	if (strcmp (argv[1], "input") == 0) {
		int _nth; for (_nth = 3; _nth < argc; _nth++) {
			if (strcmp (argv[2], "encode") == 0) {
				int _nth1; for (_nth1 = 0; _nth1 < strlen (argv[_nth]); _nth1++) {
					unsigned char _byte = argv[_nth][_nth1];
					if (_byte < 0x10) printf ("0%x", _byte); else printf ("%x", _byte);
				}
			}
			else {
				if (strlen (argv[_nth]) % 2 != 0) {printf ("gg"); return 1;}
				
				int _nth1; for (_nth1 = 0; _nth1 < strlen (argv[_nth]); _nth1 += 2) {
					unsigned char _result = 0;
					
					unsigned char _byte = argv[_nth][_nth1];
					if ((_byte >= 0x30) && (_byte <= 0x39)) _result += (_byte - 0x30) * 16;
					else if ((_byte >= 0x41) && (_byte <= 0x46)) _result += (_byte - 0x37) * 16;
					else if ((_byte >= 0x61) && (_byte <= 0x66)) _result += (_byte - 0x57) * 16;
					else {printf ("gg"); return 1;}
					
					_byte = argv[_nth][_nth1 + 1];
					if ((_byte >= 0x30) && (_byte <= 0x39)) _result += _byte - 0x30;
					else if ((_byte >= 0x41) && (_byte <= 0x46)) _result += _byte - 0x37;
					else if ((_byte >= 0x61) && (_byte <= 0x66)) _result += _byte - 0x57;
					else {printf ("gg"); return 1;}
					
					printf ("%c", _result);
				}
			}
		}
	}
	else {
		int _nth; for (_nth = 3; _nth < argc; _nth++) {
			struct stat _profile; if (stat (argv[_nth], &_profile) != 0) {printf ("ERROR: %s (%s)\n", strerror (errno), argv[_nth]); return 1;}
			if ((strcmp (argv[2], "decode") == 0) && (_profile.st_size % 2 != 0)) {printf ("ERROR: File size must be even number (%s)\n", argv[_nth]); return 1;}
			FILE *_file = fopen (argv[_nth], "r"); if (!_file) {printf ("ERROR: %s (%s)\n", strerror (errno), argv[_nth]); return 1;}
			
			if (strcmp (argv[2], "encode") == 0) {
				while (argv[0]) {
					if (feof (_file) != 0) break;
					unsigned char _byte[1]; fread (_byte, 1, sizeof (char), _file);
					if (_byte[0] < 0x10) printf ("0%x", _byte[0]); else printf ("%x", _byte[0]);
				}
			}
			else {
				while (argv[0]) {
					if (feof (_file) != 0) break;
					
					unsigned char _result = 0;
					unsigned char _byte[1];
					fread (_byte, 1, sizeof (char), _file);
					if ((_byte[0] >= 0x30) && (_byte[0] <= 0x39)) _result += (_byte[0] - 0x30) * 16;
					else if ((_byte[0] >= 0x41) && (_byte[0] <= 0x46)) _result += (_byte[0] - 0x37) * 16;
					else if ((_byte[0] >= 0x61) && (_byte[0] <= 0x66)) _result += (_byte[0] - 0x57) * 16;
					else {printf ("gg"); fclose (_file); return 1;}
					
					fread (_byte, 1, sizeof (char), _file);
					if ((_byte[0] >= 0x30) && (_byte[0] <= 0x39)) _result += _byte[0] - 0x30;
					else if ((_byte[0] >= 0x41) && (_byte[0] <= 0x46)) _result += _byte[0] - 0x37;
					else if ((_byte[0] >= 0x61) && (_byte[0] <= 0x66)) _result += _byte[0] - 0x57;
					else {printf ("gg"); fclose (_file); return 1;}
					
					printf ("%c", _result);
				}
			}
			
			fclose (_file);
		}
	}
	
	return 0;
}
