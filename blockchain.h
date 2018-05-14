#ifndef TP3_H_INCLUDED
#define TP3_H_INCLUDED
#define M 50
#define MAX 256

#define TOTO 26

typedef struct Transaction T_Transaction;
struct Transaction
{
   int idetu;
   float montant;
   char* description;
   T_Transaction* tsuiv;
};

typedef struct Block T_Block;
struct Block
{
   int idblock;
   int *date; //0:jour , 1:mois, 2:annee
   T_Transaction* first_t;
   T_Block* bsuiv;
};

typedef T_Block* BlockChain;

int getday();
int getmonth();
int getyears();
int date(T_Block* b, int day, int month, int year);
T_Transaction* ajouterTransaction(int idEtu, float montant, char *descr, T_Transaction *listeTransaction);
BlockChain ajouterBlock(BlockChain bc, int day, int month, int year);
float totalTransactionEtudiantBlock(int idEtu, T_Block b);
float soldeEtudiant(int idEtu, BlockChain bc);
void crediter(int idEtu, float montant, char *descr, BlockChain bc);
int payer(int idEtu, float montant, char *descr, BlockChain bc);
void consulter(int idEtu, BlockChain bc);
int transfert(int idSource, int idDestination, float montant, char *descr, BlockChain bc);
void EcrireFichier(FILE* fichier, BlockChain bc);
int ExportVersFichier(BlockChain bc);
BlockChain ImportDunFichier();

void afficheTransaction(T_Transaction *listeTransaction);
void afficheBlockChain(BlockChain bc);
void afficheBlockChainEtu(BlockChain bc, int idEtu);
void afficheTransactionEtu(T_Transaction *listeTransaction, int idEtu);


#endif // TP3_H_INCLUDED
