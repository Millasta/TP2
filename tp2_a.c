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
#include <errno.h>

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
/****** A ECRIRE *******/
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
 * \fn void deleteLexData(TLex ** _lexData)
 * \brief fonction qui supprime de la memoire les donnees pour
 * l'analyseur lexical
 *
 * \param[in/out] _lexData donnees de l'analyseur lexical
 * \return neant
 */
void deleteLexData(TLex * _lexData) {
/****** A ECRIRE *******/
	free(_lexData->data);
	int i;
	for(i = 0 ; i < _lexData->nbSymboles ; i++) {
		if(_lexData->tableSymboles[i].type == JSON_STRING)
			free(_lexData->tableSymboles[i].val.chaine);
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
/****** A ECRIRE *******/
	int i;
	for(i = 0 ; i < _lexData->nbSymboles ; i++) {
		switch (_lexData->tableSymboles[i].type) {
			case JSON_INT_NUMBER:
				printf("%d ", _lexData->tableSymboles[i].val.entier);
				break;
			case JSON_REAL_NUMBER:
				printf("%f ", _lexData->tableSymboles[i].val.reel);
				break;
			case JSON_STRING:
				printf("%s ", _lexData->tableSymboles[i].val.chaine);
				break;
			default:
				printf("Error : %d", _lexData->tableSymboles[i].type);
				break;
		}
	}
	printf("\n");
}


/**
 * \fn void addIntSymbolToLexData(TLex * _lexData, const int _val)
 * \brief fonction qui ajoute un symbole entier a la table des symboles
 *
 * \param[in/out] _lexData donnees de l'analyseur lexical
 * \param[in] _val valeur entiere e ajouter
 * \return neant
 */
void addIntSymbolToLexData(TLex * _lexData, const int _val) {
/****** A ECRIRE *******/
	_lexData->nbSymboles++;
	if(_lexData->nbSymboles > 1) {
		_lexData->tableSymboles = (TSymbole*)realloc(_lexData->tableSymboles, _lexData->nbSymboles * sizeof(TSymbole));
		if(_lexData->tableSymboles == NULL)
			perror("realloc");
	}
	else {
		_lexData->tableSymboles = (TSymbole*)malloc(sizeof(TSymbole));
		if(_lexData->tableSymboles == NULL)
			perror("malloc");
	}
	_lexData->tableSymboles[_lexData->nbSymboles - 1].type = JSON_INT_NUMBER;
	_lexData->tableSymboles[_lexData->nbSymboles - 1].val.entier = _val;
}

/**
 * \fn void addRealSymbolToLexData(TLex * _lexData, const float _val)
 * \brief fonction qui ajoute un symbole reel a la table des symboles
 *
 * \param[in/out] _lexData donnees de l'analyseur lexical
 * \param[in] _val valeur reelle a ajouter
 */
void addRealSymbolToLexData(TLex * _lexData, const float _val) {
/****** A ECRIRE *******/
	_lexData->nbSymboles++;
	if(_lexData->nbSymboles > 1) {
		_lexData->tableSymboles = (TSymbole*)realloc(_lexData->tableSymboles, _lexData->nbSymboles * sizeof(TSymbole));
		if(_lexData->tableSymboles == NULL)
			perror("realloc");
	}
	else {
		_lexData->tableSymboles = (TSymbole*)malloc(sizeof(TSymbole));
		if(_lexData->tableSymboles == NULL)
			perror("malloc");
	}
	_lexData->tableSymboles[_lexData->nbSymboles - 1].type = JSON_REAL_NUMBER;
	_lexData->tableSymboles[_lexData->nbSymboles - 1].val.reel = _val;
}

 /**
 * \fn void addStringSymbolToLexData(TLex * _lexData, char * _val)
 * \brief fonction qui ajoute une chaine de caracteres a la table des symboles
 *
 * \param[in/out] _lexData donnees de l'analyseur lexical
 * \param[in] _val chaine a ajouter
 */
void addStringSymbolToLexData(TLex * _lexData, char * _val) {
/****** A ECRIRE *******/
	_lexData->nbSymboles++;
	if(_lexData->nbSymboles > 1) {
		_lexData->tableSymboles = (TSymbole*)realloc(_lexData->tableSymboles, _lexData->nbSymboles * sizeof(TSymbole));
		if(_lexData->tableSymboles == NULL)
			perror("realloc");
	}
	else {
		_lexData->tableSymboles = (TSymbole*)malloc(sizeof(TSymbole));
		if(_lexData->tableSymboles == NULL)
			perror("malloc");
	}
	_lexData->tableSymboles[_lexData->nbSymboles - 1].type = JSON_STRING;
	_lexData->tableSymboles[_lexData->nbSymboles - 1].val.chaine = strdup(_val);
}

/**
 * \fn int lex(const char * _entree, TLex * _lexData)
 * \brief fonction qui effectue l'analyse lexicale (contient le code l'automate fini)
 *
 * \param[in/out] _lexData donn�es de suivi de l'analyse lexicale
 * \return code d'identification de l'entite lexicale trouvee
*/
int lex(TLex * _lexData) {
/****** A ECRIRE *******/
}


/**
 * \fn int main()
 * \brief fonction principale
 */
int main() {
	char * test;
	int response;
	int safety = 0;
	TLex * lex_data;

	test = strdup("{\"obj1\": [ {\"obj2\": 12, \"obj3\":\"text1 \\\"and\\\" text2\"},\n {\"obj4\":314.32} ], \"obj5\": true }");

	printf("%s",test);
	printf("\n");

	lex_data = initLexData(test);
	response = lex(lex_data);

	addIntSymbolToLexData(lex_data, 123);
	addStringSymbolToLexData(lex_data, "hello");
	addRealSymbolToLexData(lex_data, 3.14);

	while (response != JSON_LEX_ERROR && safety < 500) {
		//printf("lex() = %d\n",response);
		response = lex(lex_data);
		safety++;
	}

    printLexData(lex_data);

	deleteLexData(lex_data);

	free(test);

	return 0;
}
