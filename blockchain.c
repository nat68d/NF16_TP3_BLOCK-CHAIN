#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

int getday()
{
   int day=-1;
   while((day<1)||(day>31))
      {
          printf("Rentrez le jour de la transaction \n");
          if(scanf("%d",&day)!=1)
            day=-1;
          fflush(stdin);
      }
      return day;
}

int getmonth()
{
   int month=-1;
   while((month<1)||(month>12))
      {
         printf("Rentrez le mois de la transaction \n");
         if(scanf("%d", &month)!=1)
            month=-1;
        fflush(stdin);
      }
      return month;
}

int getyears()
{
   int year=-1;
   while(year<2017)
      {
        printf("Rentrez l'annee de la transaction (YYYY)\n");
        if (scanf("%d", &year)!=1)
            year=-1;
        fflush(stdin);
      }
      return year;
}

int date(T_Block* b, int day, int month, int year)
{
    b->date =malloc(3*sizeof(int));
    if (b->date==NULL)
    {
       return -1;
    }

    b->date[0]= day;
    b->date[1]= month;
    b->date[2]= year;

   return 0;
}


T_Transaction* ajouterTransaction(int idEtu, float montant, char *descr, T_Transaction *listeTransaction)
{
   T_Transaction* newTransac = malloc(sizeof(T_Transaction));
   if(newTransac==NULL)
      exit(0);
   newTransac->idetu= idEtu;
   newTransac->montant= montant;
   newTransac->description=malloc(strlen(descr)*sizeof(char));
   if(newTransac->description==NULL)
      exit(0);
   strcpy(newTransac->description,descr);
   newTransac->tsuiv=listeTransaction;
   return newTransac;
}

BlockChain ajouterBlock(BlockChain bc, int day, int month, int year)
{
      BlockChain newBlock=malloc(sizeof(T_Block));
      if(newBlock==NULL)
         exit(0);
      if (bc==NULL)
         newBlock->idblock=0;
      else
         newBlock->idblock=bc->idblock +1; //Equivalent à newBlock->idblock= (bc==NULL) ? 0 : bc->idblock +1;
      newBlock->bsuiv=bc;
      date(newBlock,day,month,year);
      newBlock->first_t=NULL;
      bc=newBlock;
   return bc;
}

float totalTransactionEtudiantBlock(int idEtu, T_Block b)
{
   float total=0;
   T_Transaction* trans=b.first_t;
   while (trans!=NULL)
   {
      if (trans->idetu==idEtu)
         total+=trans->montant;
      trans=trans->tsuiv;
   }
   return total;
}

float soldeEtudiant(int idEtu, BlockChain bc)
{
   float solde=0;
   while (bc!=NULL)
   {
      solde += totalTransactionEtudiantBlock(idEtu,*bc);
      bc=bc->bsuiv;
   }
   return solde;
}

void crediter(int idEtu, float montant, char *descr, BlockChain bc)
{

    if (montant>0) {
        (bc->first_t)=ajouterTransaction(idEtu,montant,descr,bc->first_t);
        printf("Compte credite\n\n");
    }
    else printf("Impossible de crediter un montant negatif.");

}

int payer(int idEtu, float montant, char *descr, BlockChain bc)
{

    if (montant>0)
      montant= -montant;
    if(soldeEtudiant(idEtu,bc)+ montant >= 0)
    {
       bc->first_t=ajouterTransaction(idEtu,montant,descr,bc->first_t);
       printf("Paiement effectue\n\n");
       return 1;
    }
    else
    {
         printf("Paiemant impossible : solde insuffisant.\n");
         return 0;
    }
}

void consulter(int idEtu, BlockChain bc)
{
   float solde=soldeEtudiant(idEtu,bc);
   printf("Solde de l'etudiant n°%d : %.2f euros.\n5 dernieres transactions effectuees:\n",idEtu,solde);
   int compteur=0;
   while (bc!=NULL && compteur<=5)
   {
      T_Transaction* liste=bc->first_t;
      while(liste!=NULL && compteur<5)
      {
         if (liste->idetu==idEtu)
         {
            printf("Block: %d, date: %d/%d/%d, description : %s , montant : %.2f\n",bc->idblock,bc->date[0],bc->date[1],bc->date[2],liste->description,liste->montant);
            ++compteur;

         }
         liste=liste->tsuiv;
      }
      bc=bc->bsuiv;
   }
}

int transfert(int idSource, int idDestination, float montant, char *descr, BlockChain bc)
{
   if (payer(idSource,montant, descr, bc))
   {
      crediter(idDestination,montant,descr,bc);
      printf("Transfert effectue\n\n");
      return 1;
   }
   else
   {
      printf("Transfert impossible: solde a debiter insuffisant.\n");
      return 0;
   }
}

void afficheTransaction(T_Transaction *listeTransaction)
{
   printf(" DEBUT AFFICHAGE LISTE TRANSACTION\n");
   while (listeTransaction!=NULL)
   {
      printf("  Etudiant: %d, Montant : %.2f , Description: %s \n",listeTransaction->idetu, listeTransaction->montant, listeTransaction->description);
      listeTransaction=listeTransaction->tsuiv;
   }
   printf(" FIN AFFICHAGE LISTE TRANSACTION\n\n");
}

void afficheBlockChain(BlockChain bc)
{
    printf("DEBUT AFFICHAGE DES BLOCKS\n");
    while (bc!=NULL)
    {
        printf(" Block numero %d du %d/%d/%d\n",bc->idblock,bc->date[0],bc->date[1],bc->date[2]);
        bc=bc->bsuiv;
    }
    printf("FIN AFFICHAGE DES BLOCKS\n\n");
}

void afficheBlockChainEtu(BlockChain bc, int idEtu){
   consulter(idEtu,bc);
   printf("\nVoulez vous voir l'historique entier? (pour oui entrez 1 sinon entrez 0) : ");
   int choix=0;
   if(scanf("%d",&choix)!=1)
      choix=0;
   if (choix)
   {
      printf("DEBUT AFFICHAGE BLOCKCHAIN\n\n");
      while (bc!=NULL)
      {
         printf(" Jour: %d/%d/%d :\n",bc->date[0],bc->date[1],bc->date[2]);
         afficheTransactionEtu(bc->first_t,idEtu);
         bc=bc->bsuiv;
      }
      printf("FIN AFFICHAGE BLOCKCHAIN\n\n");
   }
}

void afficheTransactionEtu(T_Transaction *listeTransaction, int idEtu)
{
    printf("DEBUT AFFICHAGE LISTE TRANSACTION DE L'ETUDIANT %d\n",idEtu);
   while (listeTransaction!=NULL)
   {
      if (listeTransaction->idetu==idEtu)
        printf(" Montant : %.2f , Description: %s \n", listeTransaction->montant, listeTransaction->description);
      listeTransaction=listeTransaction->tsuiv;
   }
   printf("FIN AFFICHAGE LISTE TRANSACTION DE L'ETUDIANT %d\n\n",idEtu);
}

void EcrireFichier(FILE* fichier, BlockChain bc)
{
   BlockChain pt_bc=bc;
   if (pt_bc!=NULL)
   {
      EcrireFichier(fichier, pt_bc->bsuiv);
      T_Transaction* tmpfirst_t=pt_bc->first_t;
      while (tmpfirst_t !=NULL)
      {
         fprintf(fichier,"Date %d / %d / %d,",pt_bc->date[0],pt_bc->date[1],pt_bc->date[2]);
         fprintf(fichier," Etudiant: %d , Montant : %.2f , Description: %s\n",tmpfirst_t->idetu, tmpfirst_t->montant, tmpfirst_t->description);
         tmpfirst_t=tmpfirst_t->tsuiv;
      }
   }
}
int ExportVersFichier(BlockChain bc) //return 1 s'il n'y a pas d'erreur de processing
{
    FILE* fichier = NULL;
    FILE* test=NULL;
    int boolean=0;
    char NomFichier[M];
    printf("Comment voulez-vous nommez votre export ? \n");
    while((test==NULL)&&(boolean==0))
    {
        scanf("%s", NomFichier);
        test=fopen(NomFichier,"r");
        if(test==NULL)
        {
            boolean=1;
        }
        if(test!=NULL)
        {
            printf("Nom déjà utilise, veuillez entrer un autre nom. \n");
            fclose(test);
            test=NULL;
        }
    }

    fichier = fopen( NomFichier, "w");

    if(fichier != NULL )
    {
        EcrireFichier(fichier,bc);
        fclose(fichier);
        printf("L'exportation du fichier est un succes.\n");
        return 1;
    }
    else
    {
        return 0;
    }
}

BlockChain ImportDunFichier()
{
    BlockChain bc=NULL;
    FILE * fichier;
    char NomFichier[M];
    int idetu,i;
    int* date_precedente=malloc(3*sizeof(int));
    int* date=malloc(3*sizeof(int));
    float montant;
    char description[M];
    printf("Veuillez entrer le nom exact du fichier a importer : \n");
    scanf("%s", NomFichier);
    fflush(stdin) ;
    fichier=fopen(NomFichier,"r");
    if (fichier != NULL)
    {
      char chaine[MAX] = "";
      while(fgets(chaine, MAX, fichier) != NULL)
      {
         int retour=sscanf(chaine,"Date %d / %d / %d, Etudiant: %d , Montant : %f , Description: %s\n",&date[0],&date[1],&date[2],&idetu, &montant, description);
         if(retour==6)
         {
            if((bc==NULL) || (date[0]!=date_precedente[0])||(date[1]!=date_precedente[1])||(date[2]!=date_precedente[2]))
            {
               bc=ajouterBlock(bc,date[0],date[1],date[2]);
            }
            bc->first_t=ajouterTransaction(idetu,montant,description, bc->first_t);
            for(i=0;i<=2;i++)
            {
               date_precedente[i]=date[i];
            }
         }
      }
      fclose(fichier);
      printf("Fichier %s importe avec succes. \n\n", NomFichier);
    }
    else
    {
      printf("Ce nom n'appartient a aucun fichier connu \n\n");
    }
    return bc;
}
