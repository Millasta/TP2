/**
 * \file tp2_a.c
 * \brief analyseur lexical pour le langage JSON
 * \author NM
 * \version 0.1
 * \date 25/11/2015
 *
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#define JSON_LEX_ERROR -1 /**< code d'erreur lexicale */
#define JSON_TRUE 1 /**< entite lexicale true */
#define JSON_FALSE 2 /**< entite lexicale false */
#define JSON_NULL 3 /**< entite lexicale null */
#define JSON_LCB 4 /**< entite lexicale { */
#define JSON_RCB 5 /**< entite lexicale } */
#define JSON_LB 6 /**< entite lexicale [ */
#define JSON_RB 7 /**< entite lexicale ] */
#define JSON_COMMA 8 /**< entite lexicale , */
#define JSON_COLON 9 /**< entite lexicale : */
#define JSON_STRING 10 /**<entite lexicale chaine de caracteres */
#define JSON_INT_NUMBER 11 /**< entite lexicale nombre entier */
#define JSON_REAL_NUMBER 12 /**< entite lexicale nombre reel */

/**
 * \union TSymbole
 * \brief union permettant de  manipuler un entier/reel/chaine pour la table
 * des symboles
 */
typedef struct {
	int type; /**< l'un des 3 types suivants : JSON_STRING/JSON_INT_NUMBER/JSON_REAL_NUMBER */
	union {
        int entier;
        float reel;
        char * chaine;
    } val; /**< valeur associer a un element de la table des symboles */
} TSymbole;

/**
 * \struct TLex
 * \brief structure contenant tous les parametres/donnees pour
 * l'analyse lexicale
 */
typedef struct{
	char * data; /**< chaine a parcourir */
	char * startPos; /**< position de depart pour la prochaine analyse */
	int nbLignes; /**< nb de lignes analysees */
	TSymbole * tableSymboles; /**< tableau des symboles : chaines/entier/reel */
	int nbSymboles; /**< taille du tableau tableSymboles */
} TLex;

/**
 * \fn int isSep(char _symb)
 * \brief fonction qui teste si un symbole fait partie des separateurs
 *
 * \param[in] _symb symbole a analyser
 * \return 1 (vrai) si _symb est un separateur, 0 (faux) sinon
 */
int isSep(const char _symb) {
/****** A ECRIRE *******/
	char separators[] = {':', ',', '{', '[', ']', '}', '"', '\0'};
	int idx = 0;

	while (separators[idx] != _symb && separators[idx] != '\0')
		idx++;

	return (separators[idx] == _symb);
}

/**
 * \fn TLex * initLexData(char * _data)
 * \brief fonction qui reserve la memoire et initialise les
 * donnees pour l'analyseur lexical
 *
 * \param[in] _data chaine a analyser
 * \return pointeur sur la structure de donnees creee
 */
TLex * initLexData(char * _data) {
    TLex * lex = malloc(sizeof(TLex));
    TSymbole * table = NULL;

	lex->data = strdup(_data);
	lex->startPos = lex->data;
	lex->nbLignes = 0;
	lex->tableSymboles = table;
	lex->nbSymboles = 0;

	return lex;
}

/**
 * \fn void deleteLexData(TLex * _lexData)
 * \brief fonction qui supprime de la memoire les donnees pour
 * l'analyseur lexical
 *
 * \param[in,out] _lexData donnees de l'analyseur lexical
 * \return neant
 */
void deleteLexData(TLex * _lexData) {
	int idx;

	free(_lexData->data);

	for (idx = 0 ; idx < _lexData->nbSymboles ; idx++) {
		if(_lexData->tableSymboles[idx].type == JSON_STRING) {
			free(_lexData->tableSymboles[idx].val.chaine);
		}
	}

	free(_lexData->tableSymboles);
	free(_lexData);
}

/**
 * \fn void printLexData(TLex * _lexData)
 * \brief fonction qui affiche les donnees pour
 * l'analyseur lexical
 *
 * \param[in] _lexData donn�es de l'analyseur lexical
 * \return neant
 */
void printLexData(TLex * _lexData) {
    int idx;

	printf("Nombre de symboles : %d\n\n", _lexData->nbSymboles);

    for (idx = 0; idx < _lexData->nbSymboles; idx++) {
		switch (_lexData->tableSymboles[idx].type) {
			case JSON_INT_NUMBER:
				printf("Symbole n°%d :\t JSON_INT_NUMBER \t%d\n", idx + 1, _lexData->tableSymboles[idx].val.entier);
				break;
			case JSON_REAL_NUMBER:
            	printf("Symbole n°%d :\t JSON_REAL_NUMBER \t%f\n", idx + 1, _lexData->tableSymboles[idx].val.reel);
				break;
			case JSON_STRING:
            	printf("Symbole n°%d :\t JSON_STRING \t\t%s\n", idx + 1, _lexData->tableSymboles[idx].val.chaine);
				break;
			case JSON_LEX_ERROR:
            	printf("Symbole n°%d :\t JSON_LEX_ERROR\n", idx + 1);
				break;
		}
    }
}


/**
 * \fn void addIntSymbolToLexData(TLex * _lexData, const int _val)
 * \brief fonction qui ajoute un symbole entier a la table des symboles
 *
 * \param[in,out] _lexData donnees de l'analyseur lexical
 * \param[in] _val valeur entiere e ajouter
 * \return neant
 */
void addIntSymbolToLexData(TLex * _lexData, const int _val) {
    if (_lexData->tableSymboles == NULL) {
        _lexData->tableSymboles = malloc(sizeof(TSymbole));
    }
    else {
        _lexData->tableSymboles = realloc(_lexData->tableSymboles, (_lexData->nbSymboles + 1) * sizeof(TSymbole));
    }

    (_lexData->tableSymboles[_lexData->nbSymboles]).type = JSON_INT_NUMBER;
    (_lexData->tableSymboles[_lexData->nbSymboles]).val.entier = _val;
    _lexData->nbSymboles++;
}

/**
 * \fn void addRealSymbolToLexData(TLex * _lexData, const float _val)
 * \brief fonction qui ajoute un symbole reel a la table des symboles
 *
 * \param[in,out] _lexData donnees de l'analyseur lexical
 * \param[in] _val valeur reelle a ajouter
 */
void addRealSymbolToLexData(TLex * _lexData, const float _val) {
    if (_lexData->tableSymboles == NULL) {
        _lexData->tableSymboles = malloc(sizeof(TSymbole));
    }
    else {
        _lexData->tableSymboles = realloc(_lexData->tableSymboles, (_lexData->nbSymboles + 1) * sizeof(TSymbole));
    }

    (_lexData->tableSymboles[_lexData->nbSymboles]).type = JSON_REAL_NUMBER;
    (_lexData->tableSymboles[_lexData->nbSymboles]).val.reel = _val;
    _lexData->nbSymboles++;
}

 /**
 * \fn void addStringSymbolToLexData(TLex * _lexData, char * _val)
 * \brief fonction qui ajoute une chaine de caracteres a la table des symboles
 *
 * \param[in,out] _lexData donnees de l'analyseur lexical
 * \param[in] _val chaine a ajouter
 */
void addStringSymbolToLexData(TLex * _lexData, char * _val) {
    if (_lexData->tableSymboles == NULL) {
        _lexData->tableSymboles = malloc(sizeof(TSymbole));
    }
    else {
        _lexData->tableSymboles = realloc(_lexData->tableSymboles, (_lexData->nbSymboles + 1) * sizeof(TSymbole));
    }

    (_lexData->tableSymboles[_lexData->nbSymboles]).type = JSON_STRING;
    (_lexData->tableSymboles[_lexData->nbSymboles]).val.chaine = strdup(_val);
    _lexData->nbSymboles++;
}

/**
* \fn void addErrorSymbolToLexData(TLex * _lexData)
* \brief fonction qui ajoute la chaine et le type ERROR a la table des symboles
*
* \param[in,out] _lexData donnees de l'analyseur lexical
*/
void addErrorSymbolToLexData(TLex * _lexData) {
   if (_lexData->tableSymboles == NULL) {
	   _lexData->tableSymboles = malloc(sizeof(TSymbole));
   }
   else {
	   _lexData->tableSymboles = realloc(_lexData->tableSymboles, (_lexData->nbSymboles + 1) * sizeof(TSymbole));
   }

   (_lexData->tableSymboles[_lexData->nbSymboles]).type = JSON_LEX_ERROR;
   (_lexData->tableSymboles[_lexData->nbSymboles]).val.chaine = "Error";
   _lexData->nbSymboles++;
}


/**
 * \fn int lex(TLex * _lexData)
 * \brief fonction qui effectue l'analyse lexicale (contient le code l'automate fini)
 *
 * \param[in,out] _lexData donnees de suivi de l'analyse lexicale
 * \return code d'identification de l'entite lexicale trouvee
*/
int lex(TLex * _lexData) {
	char buffer[9999] = {0};
	char trueModel[] = {'t', 'r', 'u', 'e', '\0'};
	char falseModel[] = {'f', 'a', 'l', 's', 'e', '\0'};
	char nullModel[] = {'n', 'u', 'l', 'l', '\0'};
	int idx = 0;

	switch(*_lexData->startPos) {
		case '"':
			_lexData->startPos++;

			printf("\nEn lecture :\t");

			while (!(*_lexData->startPos == '"' && *(_lexData->startPos - 1) != '\\') && *_lexData->startPos != '\0') {
				printf("%c", *_lexData->startPos);
				buffer[idx] = *_lexData->startPos;
				_lexData->startPos++;
				idx++;
			}

			if (*_lexData->startPos == '\0'){
				addErrorSymbolToLexData(_lexData);
				return JSON_LEX_ERROR;
			}

			_lexData->startPos++;

			if (*_lexData->startPos == ',' || *_lexData->startPos == ']' || *_lexData->startPos == '}' || *_lexData->startPos == ':') {
				addStringSymbolToLexData(_lexData, buffer);
				return JSON_STRING;
			}

			addErrorSymbolToLexData(_lexData);
			return JSON_LEX_ERROR;

			break;
		case '{':
			_lexData->startPos++;
			return JSON_LCB;
			break;
		case '}':
			_lexData->startPos++;
			return JSON_RCB;
			break;
		case 't':
			printf("\nEn lecture :\t");
			while (trueModel[idx] != '\0' && *_lexData->startPos == trueModel[idx]) {
				printf("%c", *_lexData->startPos);
				_lexData->startPos++;
				idx++;
			}

			if (*_lexData->startPos == ',' || *_lexData->startPos == ']' || *_lexData->startPos == '}') {
				return JSON_TRUE;
			}

			addErrorSymbolToLexData(_lexData);
			return JSON_LEX_ERROR;

			break;
		case 'f':
			printf("\nEn lecture :\t");
			while (falseModel[idx] != '\0' && *_lexData->startPos == falseModel[idx]) {
				printf("%c", *_lexData->startPos);
				_lexData->startPos++;
				idx++;
			}

			if (*_lexData->startPos == ',' || *_lexData->startPos == ']' || *_lexData->startPos == '}') {
				return JSON_FALSE;
			}

			addErrorSymbolToLexData(_lexData);
			return JSON_LEX_ERROR;
			break;
		case 'n':
			printf("\nEn lecture :\t");
			while (nullModel[idx] != '\0' && *_lexData->startPos == nullModel[idx]) {
				printf("%c", *_lexData->startPos);
				_lexData->startPos++;
				idx++;
			}

			if (*_lexData->startPos == ',' || *_lexData->startPos == ']' || *_lexData->startPos == '}') {
				return JSON_NULL;
			}

			addErrorSymbolToLexData(_lexData);
			return JSON_LEX_ERROR;
			break;
		case ',':
			_lexData->startPos++;
			return JSON_COMMA;
			break;
		case '[':
			_lexData->startPos++;
			return JSON_LB;
			break;
		case ']':
			_lexData->startPos++;
			return JSON_RB;
			break;
		case ':':
			_lexData->startPos++;
			return JSON_COLON;
			break;
		default:
			printf("\nEn lecture :\t");

			if (*_lexData->startPos == '-') {	// If the integer is negative
				printf("%c", *_lexData->startPos);
				buffer[idx] = *_lexData->startPos;
				_lexData->startPos++;
				idx++;
			}

			while (*_lexData->startPos >= '0' && *_lexData->startPos <= '9') {
				printf("%c", *_lexData->startPos);
				buffer[idx] = *_lexData->startPos;
				_lexData->startPos++;
				idx++;
			}

			if (*_lexData->startPos == ',' || *_lexData->startPos == ']' || *_lexData->startPos == '}') {
				addIntSymbolToLexData(_lexData, atoll(buffer));
				return JSON_INT_NUMBER;	// Simple integer case done
			}
			else if (*_lexData->startPos == 'e' || *_lexData->startPos == 'E') {
				printf("%c", *_lexData->startPos);
				buffer[idx] = *_lexData->startPos;
				_lexData->startPos++;
				idx++;

				if (*_lexData->startPos == '-' || *_lexData->startPos == '+') {		// The sign of the exp is negative
					printf("%c", *_lexData->startPos);
					buffer[idx] = *_lexData->startPos;
					_lexData->startPos++;
					idx++;
				}

				while (*_lexData->startPos >= '0' && *_lexData->startPos <= '9') {
					printf("%c", *_lexData->startPos);
					buffer[idx] = *_lexData->startPos;
					_lexData->startPos++;
					idx++;
				}

				if (*_lexData->startPos == ',' || *_lexData->startPos == ']' || *_lexData->startPos == '}') {
					addIntSymbolToLexData(_lexData, atoll(buffer));
					return JSON_INT_NUMBER;	// Exp integer case done
				}
			}
			else if (*_lexData->startPos == '.') {
				printf("%c", *_lexData->startPos);
				buffer[idx] = *_lexData->startPos;
				_lexData->startPos++;
				idx++;

				if (*_lexData->startPos < '0' || *_lexData->startPos > '9') return JSON_LEX_ERROR;

				while (*_lexData->startPos >= '0' && *_lexData->startPos <= '9') {
					printf("%c", *_lexData->startPos);
					buffer[idx] = *_lexData->startPos;
					idx++;
					_lexData->startPos++;
				}

				if (*_lexData->startPos == ',' || *_lexData->startPos == ']' || *_lexData->startPos == '}') {
					addRealSymbolToLexData(_lexData, atof(buffer));
					return JSON_REAL_NUMBER;	// Simple reel case done
				}
				else if (*_lexData->startPos == 'e' || *_lexData->startPos == 'E') {
					printf("%c", *_lexData->startPos);
					buffer[idx] = *_lexData->startPos;
					_lexData->startPos++;
					idx++;

					if (*_lexData->startPos == '-' || *_lexData->startPos == '+') {		// The sign of the exp is negative
						printf("%c", *_lexData->startPos);
						buffer[idx] = *_lexData->startPos;
						_lexData->startPos++;
						idx++;
					}

					while (*_lexData->startPos >= '0' && *_lexData->startPos <= '9') {
						printf("%c", *_lexData->startPos);
						buffer[idx] = *_lexData->startPos;
						_lexData->startPos++;
						idx++;
					}

					if (*_lexData->startPos == ',' || *_lexData->startPos == ']' || *_lexData->startPos == '}') {
						addRealSymbolToLexData(_lexData, atof(buffer));
						return JSON_REAL_NUMBER;	// Exp reel case done
					}
				}
			}

			addErrorSymbolToLexData(_lexData);
			return JSON_LEX_ERROR;	// The sequence is not part of the above cases : the sequence is not valid

			break;
	}

	addErrorSymbolToLexData(_lexData);
	return JSON_LEX_ERROR;
}

/**
 * \fn char * removeBlanks(char * string)
 * \brief fonction qui renvoie une copie allouée sur le tas de la chaîne passée en paramètre, dépourvue d'espaces (' ', '\n', '\t')
 * \param[in] string la chaîne à traiter
 * \return une copie de la chaîne dépourvue d'espace
*/
char * removeBlanks(char * string) {
	char * cleanString = malloc(sizeof(char) * strlen(string));
	int strIdx = 0;
	int resuIdx = 0;
	int insideString = 0;

	while (string[strIdx] != '\0') {
		if (string[strIdx] == '"') {
			if (strIdx > 0 && string[strIdx - 1] != '\\') {
				if (insideString) {
					insideString = 0;
				}
				else {
					insideString = 1;
				}
			}
			else {
				insideString = 1;
			}
		}

		if (!insideString) {
			if (string[strIdx] != '\n' && string[strIdx] != '\t' && string[strIdx] != ' ') {
				cleanString[resuIdx] = string[strIdx];
				resuIdx++;
			}
		}
		else {
			cleanString[resuIdx] = string[strIdx];
			resuIdx++;
		}
		strIdx++;
	}

	cleanString[resuIdx] = '\0';
	cleanString = realloc(cleanString, sizeof(char) * (resuIdx + 1));

	return cleanString;
}

/**
 * \fn int main(int argc, char *argv[])
 * \brief fonction principale
 */
int main(int argc, char *argv[]) {
	char rawData[100000] = {0};
	char * blanklessData;
	int len = 0;
	int idx = 0;
	int code[200];
    char currChar;
	TLex * lex_data;
	FILE * file;

	if (argc > 1) {
		/* Lecture des données depuis le fichier indiqué en argument */

		file = fopen(argv[1], "r");
		currChar = fgetc(file);

		for(idx = 0 ; currChar != EOF ; idx++) {
			rawData[idx] = currChar;
			currChar = fgetc(file);
		}

		fclose(file);
	}
	else {
		strcpy(rawData, "\"test\" : \t-36.6E-5,[null, {\"obj1\": [ {\"obj2\": 12, \"obj3\":\"text1 \\\"and\\\" text2\"},\n {\"obj4\":0.32} ], \"obj5\": true }]");
	}

	blanklessData = removeBlanks(rawData);

	lex_data = initLexData(blanklessData);
	free(blanklessData);

	printf("\n\n");
	printf("==========       ENTREE       ============");
	printf("\n\n");

	printf("\n%s\n", lex_data->data);

	printf("\n\n");
	printf("========== SUIVI DE L'ANALYSE ============");
	printf("\n\n");

	/* Analyse lexicale des données */
	while (code[len - 1] != JSON_LEX_ERROR && *lex_data->startPos != '\0') {
		code[len] = lex(lex_data);
		len++;
	}

	printf("\n\n");
	printf("==========      RESULTAT      ============");
	printf("\n\n");

	if (*lex_data->startPos == '\0') {
		printf("\n- Analyse lexicale réussie !");
	}
	else {
		printf("\n- Analyse lexicale interrompue. Dernier caractère lu : '%c'\t(itération n°%d)", *lex_data->startPos, len);
	}

	printf("\n- Codes : ");

	for (idx = 0; idx < len - 1; idx++) {
		printf("%d|", code[idx]);
	}
	printf("%d", code[len - 1]);

    printf("\n\n");
    printf("==========  TABLE DES SYMBOLES  ==========");
    printf("\n\n");

    printLexData(lex_data);

	deleteLexData(lex_data);

	return 0;
}
