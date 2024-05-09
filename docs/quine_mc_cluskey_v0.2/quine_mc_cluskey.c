//-------------------------------------------------------------------------------------------------------------
// Quine–McCluskey Algorithm
// =========================
//-------------------------------------------------------------------------------------------------------------
// English:
//-------------------------------------------------------------------------------------------------------------
// Description: Application to simplify boolean functions with Quine-McCluskey algorithm
// Date: 05/16/2012
// Author: Stefan Moebius (mail@stefanmoebius.de)
// Licence: Can be used freely (Public Domain)
//-------------------------------------------------------------------------------------------------------------
// German:
//-------------------------------------------------------------------------------------------------------------
// Beschreibung: Programm zur Vereinfachung von Booleschen Funktionen mit hilfe des Quine–McCluskey Verfahrens.
// Datum: 16.05.2012
// Author: Stefan Moebius (mail@stefanmoebius.de)
// Lizenz: darf frei verwendet werden (Public Domain)
//-------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define TRUE 1
#define FALSE 0
#define MAXVARS 7
#define MAXOUTS 1
#define MAXNAMELEN "16"
#define MAX 2048


/* Define host machine characteristics of "unsigned int" */
#ifndef BPI
#define BPI             64              /* # bits per integer */
#endif

#if BPI == 64
#define LOGBPI          6               /* log(BPI)/log(2) */
#elif BPI == 32
#define LOGBPI          5               /* log(BPI)/log(2) */
#elif BPI == 16
#define LOGBPI          4               /* log(BPI)/log(2) */
#else
#define LOGBPI          4               /* log(BPI)/log(2) */
#endif

//Global fields: / Globale Felder:
int minterm[MAX][MAX];
int mask[MAX][MAX];		// mask of minterm  /  Maske des Minterm
int used[MAX][MAX];		// minterm used  /  Minterm wurde verwendet
int result[MAX];		// results  /  Ergebnisse
int primmask[MAX];		// mask for prime implicants  /  Maske für Primimplikant
int prim[MAX];			// prime implicant  /  Primimplikant
int wprim[MAX];			// essential prime implicant (TRUE/FALSE)  /  wesentlicher Primimplikant (TRUE/FALSE)
int nwprim[MAX];		// needed not essential prime implicant  /  benötigter unwesentlicher Primimplikant
typedef struct {
  char* names[MAX];	        // Input variable names
  int num;
} names_t;
char tempstringbuff[MAX];      // temporary buffer to store strings
char tempstringbuff2[MAX];      // temporary buffer to store strings
//Count all set bits of the integer number  /  Zählen der gesetzen Bits in einer Integerzahl
int popCount(unsigned x) { // Taken from book "Hackers Delight"  / Aus dem Buch "Hackers Delight" 
	x = x - ((x >> 1) & 0x55555555);
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	x = (x + (x >> 4)) & 0x0F0F0F0F;
	x = x + (x >> 8);
	x = x + (x >> 16);
	return x & 0x0000003F;
}

//Calculate hamming weight/distance of two integer numbers  /  Berechnung der Hammingdistanz von 2 Integerzahlen
int hammingWeight(int v1, int v2) {
	return popCount(v1 ^ v2);
} 

//Output upper part of term in console  /  Oberer Teil des Terms in der Konsole ausgeben
void upperTerm(int bitfield, int mask, int num) {
	if (mask) {
		int z;
		//printf("num: %d\n", num);
		for ( z = 0; z < num; z++) {
			if (mask & (1 << z)) {      
				if (bitfield & (1 << z))		
					printf("_");
				else
					printf(" ");
			}
		} 
	} else {
	    //printf("mask: %x\n", mask);
	}
}

//Output lower part of term in console  /  Unterer Teil des Terms in der Konsole ausgeben
void lowerTerm(int mask, int num) {
	if (mask) {
		int z;
		for (z = 0; z < num; z++) {
			if (mask & (1 << z)) {
				printf("%c", 'z' - (num - 1) + z);
			} 
		} 
	}
}

//Output a term to console  /  Ausgabe eines Terms in der Konsole
void outputTerm(int bitfield, int mask, int num) {
	upperTerm(bitfield, mask, num);
	if (mask) printf("\n");
	lowerTerm(mask, num);
}

//Determines whether "value" contains "part"  /  Bestimmt, ob "value" "part" beinhaltet
int contains(int value, int mask, int part, int partmask) {
	if ((value & partmask) == (part & partmask)) {
		if ((mask & partmask) ==  partmask)
			return TRUE;
	}   
	return FALSE;
}

// Converts a binary string into an integer value
int binary_to_int(char* inbits) {
    int RADIX = 2;
    int totval = 0;
    int bitval = 1;
    int x = 0;
    for (x = strlen(inbits)-1; x >= 0; --x) {
	// printf("inbits: %s\n", inbits);
	assert(inbits[x] == '0' || inbits[x] == '1');
	int bit = (int)(inbits[x] - '0');
	totval += bitval * bit;
	bitval *= RADIX;
    }
    return totval;
}

char* stralloc(size_t len) {
    char* outbits = (char*) malloc((len+1)*sizeof(char));
    outbits[len] = '\0';
    return outbits;
}
void malloc_int_to_binary_string(int totval,  char* outbits) {
    int nbits = (int) ceil(log2(totval));
    outbits = (char*) malloc((nbits+1)*sizeof(char));
}

// Converts an integer value into binary strig
void int_to_binary_string(int totval, char* outbits, size_t bitlength) { 
    int RADIX = 2;
    // malloc_int_to_binary_string(totval, outbits); 
    int nbits = (totval <= 0) ? 1 : (int) ceil(log2(totval));
    // printf("assert %ld >= log2(%d)\n", bitlength, totval);
    assert(bitlength >= nbits);
    int x = 0;
    int remval = totval;
    for (x = bitlength-1; x >= 0; --x) {
	if (remval >= RADIX) {
	  outbits[x] = (char)(remval % RADIX) + '0';
	  remval = totval / RADIX;
	} else {
	    outbits[x] = '0';
	}
    }
    // free(outbits);
}

int parse_input(char* filename, names_t* p_invars, names_t* p_outvars, 
    int mask[MAX][MAX],
    int minterm[MAX][MAX], int* result)
{
      char* inbits = NULL;
      char* outbits = NULL;
      int retcode = 0;
      int _ = 0; // ignore variable
      int x = 0;
      int pos = 0;
      char* line = NULL;
      size_t len = 0;
      ssize_t redlinelen = 0;
      FILE* fp = fopen("sample_input.in", "r");
      // FILE* fp = stdout;
      redlinelen = getline(&line, &len, fp);
      if (redlinelen == -1) {
	  fprintf(stderr, "ERROR:%d: Unexpected end of file! '%s'\n", __LINE__, line);
	  retcode = EIO; goto FREE;
      }
      if ( sscanf(line, ".i %d", &(p_invars->num)) != 1) {
	  fprintf(stderr, "ERROR: Invalid format, expected '.i %%d'!\n");
	  retcode = EIO; goto FREE;
      }
      if (p_invars->num > MAXVARS) {
	      fprintf(stderr, "ERROR: Number of variables too big!\n");
	      retcode = EIO; goto FREE;
      }
      if (p_invars->num < 1) {
	      fprintf(stderr, "ERROR: Number of variables must be at least 1!\n");
	      retcode = EIO; goto FREE;
      }
      inbits = stralloc(p_invars->num);

      redlinelen = getline(&line, &len, fp);
      if (redlinelen == -1) {
	  fprintf(stderr, "ERROR:%d: Unexpected end of file! '%s'\n", __LINE__, line);
	  retcode = EIO; goto FREE;
      }
      if ( sscanf(line, ".o %d", &p_outvars->num) != 1) {
	  _ = scanf("%[^\n]\n", tempstringbuff);
	  fprintf(stderr, "ERROR: Invalid format, expected '.o %%d' got %s!\n", tempstringbuff);
	  retcode = EIO; goto FREE;
      }
      if (p_outvars->num > MAXOUTS) {
	      fprintf(stderr, "ERROR: Number of outputs too big! Got %d\n", p_outvars->num);
	      retcode = EIO; goto FREE;
      }
      if (p_outvars->num < 1) {
	      fprintf(stderr, "ERROR: Number of outputs must be at least 1! Got %d\n", p_outvars->num);
	      retcode = EIO; goto FREE;
      }
      outbits = stralloc(p_invars->num);

      redlinelen = getline(&line, &len, fp);
      if (redlinelen == -1) {
	  fprintf(stderr, "ERROR:%d: Unexpected end of file! '%s'\n", __LINE__, line);
	  retcode = EIO; goto FREE;
      }
      int offset = 0;
      for (x=0; x < p_invars->num; ++x) {
	  int scannedvars = 0;
	  int inc = 0;
	  if (x == 0)
	    scannedvars = sscanf(line, ".ilb %" MAXNAMELEN "s%n", tempstringbuff, &inc);
	  else
	    scannedvars = sscanf(line+ offset, "%" MAXNAMELEN "s%n", tempstringbuff, &inc);
	  offset += inc;
	  if (scannedvars != 1) {
	      _ = scanf("%[^\n]\n", tempstringbuff);
	      fprintf(stderr, "ERROR: invalid input variable name! Got %s\n", tempstringbuff);
	      retcode = EIO; goto FREE;
	  }
	  p_invars->names[x] = (char*) malloc((strlen(tempstringbuff)+1) * sizeof(char));
	  strcpy(p_invars->names[x], tempstringbuff);
      }

      redlinelen = getline(&line, &len, fp);
      if (redlinelen == -1) {
	  fprintf(stderr, "ERROR:%d: Unexpected end of file! '%s'\n", __LINE__, line);
	  retcode = EIO; goto FREE;
      }

      offset = 0;
      for (x=0; x < p_outvars->num; ++x) {
	  int scannedvars = 0;
	  int inc = 0;
	  if (x == 0)
	    scannedvars = sscanf(line, ".ob %" MAXNAMELEN "s%n", tempstringbuff, &inc);
	  else
	    scannedvars = sscanf(line+offset,"%" MAXNAMELEN "s%n", tempstringbuff, &inc);
	  offset += inc;
	  if (scannedvars != 1) {
	      _ = scanf("%[^\n]\n", tempstringbuff);
	      fprintf(stderr, "ERROR: invalid output  variable name! Got %s\n", tempstringbuff);
	      retcode = EIO; goto FREE;
	  }
	  p_outvars->names[x] = (char*) malloc((strlen(tempstringbuff)+1) * sizeof(char));
	  strcpy(p_outvars->names[x], tempstringbuff);
      }

      pos = (1 << p_invars->num);  // 2 ^ num
      // printf("Please enter desired results: ( 0 or 1)\n\n");

      int cur = 0;
      char format[256];
      snprintf(format, sizeof(format), " %%%ds %%%ds", p_invars->num, p_outvars->num);
      for ( x=0; x < pos; x++) {
	  redlinelen = getline(&line, &len, fp);
	  if (redlinelen == -1) {
	      fprintf(stderr, "ERROR:%d: Unexpected end of file! '%s'\n", __LINE__, line);
	      retcode = EIO; goto FREE;
	  }
	  if (sscanf(line, ".e %[^\n]\n", tempstringbuff) == 1) {
	      printf("found .e\n");
	      break;
	  }
	  if (sscanf(line, format, inbits, outbits) != 2) {
	      _ = scanf("%[^\n]\n", tempstringbuff);
	      fprintf(stderr, "ERROR: Expected inbits and outbits! Got %s\n", tempstringbuff);
	      retcode = EIO; goto FREE;
	  }
	  int minterm_num = binary_to_int(inbits);
	  printf("%s -> %d = %c\n", inbits, minterm_num, outbits[0]);

	  if (outbits[0] == '1') {
	      mask[cur][0] = ((1 << p_invars->num)- 1);   
	      minterm[cur][0] = minterm_num;  
	      cur++;
	      result[minterm_num] = 1; // (int)(outbits[0] - '0');
	  } else if (outbits[0] == '0') {
	      // all good
	  } else if (outbits[0] == '-') {
	      fprintf(stderr, "ERROR: dont care are unhandled!\n");
	      retcode = EIO; goto FREE;
	  } else {
	      fprintf(stderr, "ERROR: dont recognize outbits! Got %c\n", outbits[0]);
	      retcode = EIO; goto FREE;
	  }
      }
      _ += 0;
FREE:
      if (outbits) free(outbits);
      if (inbits) free(inbits);
      if (line) free(line);
      if (fp != stdout) fclose(fp);
      return retcode;
}

void print_mask_minterm_res(names_t* invars, int mask[MAX][MAX], 
    int minterm[MAX][MAX], int* result) {
    int x = 0;
    int pos = (1 << invars->num); // 2^num
    printf(".i %d\n", invars->num);
    for (x = 0; x < pos; ++x) {
	char* outbits = stralloc(invars->num);
	// printf("strlen(outbits) = %ld, invars->num= %d \n", strlen(outbits), invars->num);
	int_to_binary_string(x, outbits, invars->num);
	//if (result[x]) {
	printf("(%d)%s = %d\n", x, outbits, result[x]);
	//}
    }
}

int main(int argc, char** argv) {
	char* filename;
	if (argc < 2) {
	    fprintf(stderr, "ERROR: need filename\n"
		"Usage: ./quine_mc_cluskey sample_input.in\n");
	    return 0;
	} else {
	    filename = argv[1];
	}
	names_t invars; // Number of Variables  /  Anzahl Eingänge
	names_t outvars; // Number of outputs
	int pos = 0;
	int cur = 0;
	int reduction = 0; //reduction step  / Reduktionsschritt
	int maderedction = FALSE;
	int prim_count = 0;
	int term = 0;
	int termmask = 0;
	int found = 0;
	int x = 0;
	int y = 0;
	int z = 0;
	int count = 0;
	int lastprim = 0; 
	int res = 0; // actual result  /  Ist-Ausgabe
	int retcode = 0;

	// Fill all arrays with default values / Alle Arrays mit Standardwert auffüllen
	for (x = 0; x < MAX; x++) {
		primmask[x] = 0;
		prim[x] = 0;
		wprim[x] = FALSE;
		nwprim[x] = FALSE;
		result[x] = FALSE;
		nwprim[x] = TRUE; //unwesentliche Primimplikaten als benötigt markieren
		for (y = 0; y < MAX; y++) {
			mask[x][y] = 0;
			minterm[x][y] = 0;
			used[x][y] = FALSE;
		}
	}

	parse_input(filename, &invars, &outvars, mask, minterm, result);
	print_mask_minterm_res(&invars, mask, minterm, result);

	// Start of algorithm
	for (reduction = 0; reduction < MAX; reduction++) {
	    cur = 0; 
	    maderedction = FALSE;
	    for (y=0; y < MAX; y++) {
		for (x=0; x < MAX; x++) {   
		    if ((mask[x][reduction]) && (mask[y][reduction])) {      
			if (popCount(mask[x][reduction]) > 1) { // Do not allow complete removal (problem if all terms are 1)  /  Komplette aufhebung nicht zulassen (sonst problem, wenn alle Terme = 1)
			    if ((hammingWeight(minterm[x][reduction] & mask[x][reduction], minterm[y][reduction] & mask[y][reduction]) == 1) && (mask[x][reduction] == mask[y][reduction])) { // Simplification only possible if 1 bit differs  /  Vereinfachung nur möglich, wenn 1 anderst ist 
				term = minterm[x][reduction]; // could be mintern x or y /  egal ob mintern x oder minterm y 
							      //e.g.:
							      //1110
							      //1111
							      //Should result in mask of 1110  /  Soll Maske von 1110 ergeben
				termmask = mask[x][reduction]  ^ (minterm[x][reduction] ^ minterm[y][reduction]); 
				term  &= termmask;

				found = FALSE;		
				for ( z=0; z<cur; z++) {
				    if ((minterm[z][reduction+1] == term) && (mask[z][reduction+1] == termmask) ) {							
					found = TRUE;
				    }
				}

				if (found == FALSE) {
				    minterm[cur][reduction+1] = term;
				    mask[cur][reduction+1] = termmask;
				    cur++; 
				}
				used[x][reduction] = TRUE;
				used[y][reduction] = TRUE;  
				maderedction = TRUE;
			    }
			}
		    } 
		}    
	    }
	    if (maderedction == FALSE)
	      break; //exit loop early (speed optimisation)  /  Vorzeitig abbrechen (Geschwindigkeitsoptimierung)
	}

	prim_count = 0;
	printf("\nprime implicants:\n");
	for ( reduction = 0 ; reduction < MAX; reduction++) {
	    for ( x=0 ;x < MAX; x++) {		
		//Determine all not used minterms  /  Alle nicht verwendeten Minterme bestimmen
		if ((used[x][reduction] == FALSE) && (mask[x][reduction]) ) {
		    //Check if the same prime implicant is already in the list  /  Überprüfen, ob gleicher Primimplikant bereits in der Liste
		    found = FALSE;
		    for ( z=0; z < prim_count; z++) {
			if (((prim[z] & primmask[z]) == (minterm[x][reduction] & mask[x][reduction])) &&  (primmask[z] == mask[x][reduction]) )					
			  found = TRUE;
		    } 
		    if (found == FALSE) {
			outputTerm(minterm[x][reduction], mask[x][reduction], invars.num);
			printf("\n");
			primmask[prim_count] = mask[x][reduction];
			prim[prim_count] = minterm[x][reduction];
			prim_count++;
		    }     
		} 
	    } 
	}  

	//find essential and not essential prime implicants  /  wesentliche und unwesentliche Primimplikanten finden
	//all alle prime implicants are set to "not essential" so far  /  Primimplikanten sind bisher auf "nicht wesentlich" gesetzt
	for (y=0; y < pos; y++) { //for all minterms  /  alle Minterme durchgehen 	
	    count = 0;
	    lastprim = 0;   
	    if (mask[y][0]) {
		for (x=0; x < prim_count; x++ ) { //for all prime implicants  /  alle Primimplikanten durchgehen  
		    if (primmask[x]) {
			// Check if the minterm contains prime implicant  /  the Überprüfen, ob der Minterm den Primimplikanten beinhaltet
			if (contains(minterm[y][0], mask[y][0], prim[x], primmask[x])) {					
			    count++;
			    lastprim = x;          
			}  
		    } 		
		}
		// If count = 1 then it is a essential prime implicant /  Wenn Anzahl = 1, dann wesentlicher Primimplikant
		if (count == 1) {
		    wprim[lastprim] = TRUE;
		} else {
		    printf("count = %d\n", count);
		}
	    }
	}

	// successively testing if it is possible to remove prime implicants from the rest matrix  /  Nacheinander testen, ob es mögich ist, Primimplikaten der Restmatrix zu entfernen
	for ( z=0; z < prim_count; z++) {
	    if (primmask[z] ) {
		if ((wprim[z] == FALSE)) { // && (rwprim[z] == TRUE))
		    nwprim[z] = FALSE; // mark as "not essential" /  als "nicht benötigt" markiert
		    for ( y=0; y < pos; y++) { // for all possibilities  /  alle Möglichkeiten durchgehen 
			res = 0;
			for ( x=0; x < prim_count; x++) {
			    if ( (wprim[x] == TRUE) || (nwprim[x] == TRUE)) {  //essential prime implicant or marked as required  /  wesentlicher Primimplikant oder als benötigt markiert
				if ((y & primmask[x]) == (prim[x] & primmask[x])) { //All bits must be 1  /  Es müssen alle Bits auf einmal auf 1 sein (da And-Verknüpfung)
				    res = 1; 
				    break;
				}
			    }
			}
			//printf(" %d\t%d\n", result, result[y]);
			if (res == result[y]) {  // compare calculated result with input value /  Berechnetes ergebnis mit sollwert vergleichen				
						 //printf("not needed\n"); //prime implicant not required  /  Primimplikant wird nicht benötigt
			}
			else {
			    //printf("needed\n");
			    nwprim[z] = TRUE; //prime implicant required  /  Primimplikant wird doch benötigt
			}
		    }
		}
	    }
	}
	printf("\nResult:\n\n");
	// Output of essential and required prime implicants / Ausgabe der wesentlichen und restlichen benötigten Primimplikanten:
	printf("prim_count: %d\n", prim_count);
	count = 0;
	for ( x = 0 ; x < prim_count; x++) {
	    if (wprim[x] == TRUE) {
		if (count > 0) printf("   ");
		upperTerm(prim[x], primmask[x], invars.num);
		count++;
	    }
	    else if ((wprim[x] == FALSE) && (nwprim[x] == TRUE)) {
		if (count > 0) printf("   ");
		upperTerm(prim[x], primmask[x], invars.num);
		count++;
	    }
	}
	printf("\n");
	count = 0;
	for ( x = 0 ; x < prim_count; x++) {
	    if (wprim[x] == TRUE) {
		if (count > 0) printf(" + ");
		lowerTerm(primmask[x], invars.num);
		count++;
	    }
	    else if ((wprim[x] == FALSE) && (nwprim[x] == TRUE)) {
		if (count > 0) printf(" + ");
		lowerTerm(primmask[x], invars.num);
		count++;
	    }
	}
	printf("\n");
	for (x = 0; x < outvars.num; ++x) {
	    if (outvars.names[x]) free(outvars.names[x]);
	}
	for (x = 0; x < invars.num; ++x) {
	    if (invars.names[x]) free(invars.names[x]);
	}
	return retcode;
}

