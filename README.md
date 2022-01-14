# minishell
old
parseur --> voir les erreurs missings cotes --> "bash: missing quote" -- > ok pour pipe et chevron out

-->verifier errno, code trouvee 130 si erreur ou 2 si multiligne
les options de echo sont forcement apres l'appel systeme et avangt le text

--> clean pipe, differente fonction de gestion de fd a enlever

--> gerer les multi fd --> demander a yuan comment elle a gerer ca ?

--> trouver solution espace dans guillemets (voir avec parsing_cotes) --> meilleur option !

---------------------------------------------------------------------------------------------------------------------------------------

--> check echo $? (errno) //ce n'est pas erno mais le code re retour du programme donc avec exit !

--> ctr + m est considerer comme une commande --> erreur !

--> pas de parsing a faire pour $.
    * juste cher le '$' dans la ligne, si rien derriere rien faire sinon la chercher dans l'env !
