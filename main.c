#include <stdio.h>
#include <stdlib.h>
#include "blockchain.h"

int main()
{
    int choix=0, idEtu=0,idEtu2=0, jour=0, mois=0, annee=0, testscanf=0;
    float montant=0;
    char description[M];
    BlockChain chaine=NULL, tmpchaine=NULL;
    while(choix!=8)
    {
        printf("   1. Afficher la liste des blocs de la BlockChain \n");
        printf("   2. Afficher toutes les transactions d un bloc\n");
        printf("   3. Afficher toutes les transactions du jour pour un etudiant\n");
        printf("   4. Afficher l historique pour un etudiant\n");
        printf("   5. Crediter un compte\n");
        printf("   6. Payer un repas\n");
        printf("   7. Transferer des EATCoins entre deux etudiants\n");
        printf("   8. Quitter\n");
        printf("   9. Exporter toutes les transactions vers un fichier\n");
        printf("  10. Importer des transactions depuis un fichier\n");

        printf("\n\nQue voulez vous faire ? ");
        if (scanf("%d",&choix)!=1)
         choix=0;
        fflush(stdin);
        switch (choix)
        {
        case 1 : afficheBlockChain(chaine);
            break;
        case 2 : if (chaine)
                 {
                     printf("De quel jour voulez vous afficher les transactions ?\n");
                     jour=getday();
                     mois=getmonth();
                     annee=getyears();
                     tmpchaine=chaine; //on pert pas la tete du blockchain qui reste dans chaine
                     while(tmpchaine&&(tmpchaine->date[0]!=jour||tmpchaine->date[1]!=mois||tmpchaine->date[2]!=annee))
                           tmpchaine=tmpchaine->bsuiv;
                     if (tmpchaine==NULL)
                        printf("Il n'y a pas de transactions ce jour la.\n\n");
                     else
                        afficheTransaction(tmpchaine->first_t);
                 }
                 else printf("Blockchain vide\n\n");
            break;
        case 3 : if (chaine)
                 {
                     while (testscanf!=1)
                     {
                        printf("De quel etudiant voulez vous afficher les transactions du jour ?");
                        testscanf=scanf("%d",&idEtu);
                        fflush(stdin);
                     }
                     testscanf=0;
                     afficheTransactionEtu(chaine->first_t,idEtu);
                 }
                 else printf("Blockchain vide\n\n");
            break;
        case 4 : if (chaine)
                    {
                        while (testscanf!=1)
                        {
                           printf("De quel etudiant voulez-vous afficher l historique ? ");
                           testscanf=scanf("%d",&idEtu);
                           fflush(stdin);
                        }
                        testscanf=0;
                        afficheBlockChainEtu(chaine,idEtu);
                    }
                    else printf("BlockChain vide\n\n");
            break;
        case 5 : jour=getday();
                 mois=getmonth();
                 annee=getyears();
                 if((chaine==NULL)|| chaine->date[0]!=jour|| chaine->date[1]!=mois|| chaine->date[2]!=annee) //si c'est la première transaction de la journée, on recrée un block
                 {
                    chaine=ajouterBlock(chaine,jour,mois,annee);
                 }
                 while (testscanf!=1)
                 {
                     printf("Quel compte voulez-vous crediter ? ");
                     testscanf=scanf("%d",&idEtu);
                     fflush(stdin);
                 }
                  testscanf=0;
                 while (testscanf!=1)
                 {
                    printf("De quel montant ? ");
                    testscanf=scanf("%f",&montant);
                    fflush(stdin);
                 }
                 testscanf=0;
                 crediter(idEtu,montant,"Rechargement",chaine);
            break;
        case 6 : if (chaine==NULL)
                  {
                     printf("Paiement impossible, veuillez recharger votre compte avant.\n\n");
                     break;
                  }
                 jour=getday();
                 mois=getmonth();
                 annee=getyears();

                  if( chaine->date[0]!=jour|| chaine->date[1]!=mois|| chaine->date[2]!=annee) //si c'est la première transaction de la journée, on recrée un block
                      {//enlever test chaine=NULL car si null aucun compte n'as de l'argent donc paiement ne va pas se faire donc pas la peine d'ajouter un block
                          chaine=ajouterBlock(chaine,jour, mois,annee);
                      }

                 while (testscanf!=1)
                 {
                     printf("Quel compte doit etre debite ? ");
                     testscanf=scanf("%d",&idEtu);
                     fflush(stdin);
                 }
                  testscanf=0;
                 while (testscanf!=1)
                 {
                    printf("De quel montant?");
                    testscanf=scanf("%f",&montant);
                    fflush(stdin);
                 }
                 testscanf=0;
                 printf("Pour quel achat ? ");
                 scanf("%s", description);
                 fflush(stdin);
                 payer(idEtu,montant,description,chaine);
            break;
        case 7 : if (chaine==NULL)
                  {
                     printf("Transfert impossible, veuillez recharger le compte a debiter avant.\n\n");
                     break;
                  }
                 jour=getday();
                 mois=getmonth();
                 annee=getyears();

                 if( chaine->date[0]!=jour|| chaine->date[1]!=mois|| chaine->date[2]!=annee) //si c'est la première transaction de la journée, on recrée un block
                      {
                          chaine=ajouterBlock(chaine,jour, mois,annee);
                      }
                 while (testscanf!=1)
                 {
                     printf("Quel compte doit etre debite ? ");
                     testscanf=scanf("%d",&idEtu);
                     fflush(stdin);
                 }
                  testscanf=0;
                 while (testscanf!=1)
                 {
                     printf("Quel compte voulez-vous crediter ? ");
                     testscanf=scanf("%d",&idEtu2);
                     fflush(stdin);
                 }
                  testscanf=0;
                 while (testscanf!=1)
                 {
                    printf("De quel montant ? ");
                    testscanf=scanf("%f",&montant);
                    fflush(stdin);
                 }
                 testscanf=0;
                 transfert(idEtu,idEtu2,montant,"Transfert",chaine);
            break;
        case 8 : break;
        case 9 : ExportVersFichier(chaine);
            break;
        case 10 : chaine=ImportDunFichier();
            break;
        default : printf("Choix incorrect.\n\n");
        }
    }

    while(chaine!=NULL)
    {
       T_Transaction* liste=chaine->first_t;
       while(liste!=NULL)
       {
          T_Transaction* tmp=liste->tsuiv;
          free(liste->description);
          free(liste);

          liste=tmp;

       }
       BlockChain temp=chaine->bsuiv;
       free(chaine->date);
       free(chaine);
       chaine=temp;
    }
    chaine=NULL;

    return 0;
}
