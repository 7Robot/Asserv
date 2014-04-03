Asserv
======

Le fichier `asserv.c` contient les fonctions nécessaire pour réaliser un
asservissement en position ou vitesse.
Ainsi, lors de la création d’un asservissement, deux pid sont initialisé :
un pour l’asservissement en position et l’autre pour l’asservissement en vitesse
(avec leurs propres coefficiants).
L’asservissement peut également être désactivé.
Finalement, l’asservissement peut être dans 3 mode différents comme résumé par
les `define` en haut du fichier :::

    #define M_OFF  0 // asserv off
    #define M_POS   1 // asserv en position
    #define M_SPEED 2 // asserv en vitesse

