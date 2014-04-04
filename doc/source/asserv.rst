Asserv
======

Le fichier `asserv.c` contient les fonctions nécessaires pour réaliser un
asservissement en position ou vitesse.

Ainsi, lors de la création d’un asservissement, deux pid sont initialisés :
un pour l’asservissement en position et l’autre pour l’asservissement en vitesse
(avec leurs propres coefficients).
L’asservissement peut également être désactivé.

Finalement, l’asservissement peut être dans 3 mode différents comme résumé par
les `define` en haut du fichier :::

    #define M_OFF  0 // asserv off
    #define M_POS   1 // asserv en position
    #define M_SPEED 2 // asserv en vitesse

L’initialisation d’un asservissement nécessite un pointeur pour la consigne, et
deux jeux de paramètres pour les deux PID (vitesse et position).

La fonction `asserv_done` appelle la fonction `pid_done` du PID en position.
Cette fonction n’a donc pas de sens lorsque l’asservissement n’est pas en
position.

Le mode peut être changé grâce aux trois fonctions `asserv_off`,
`asserv_set_pos_mode` et `asserv_set_speed_mode`.

À chaque `step`, la fonction `asserv_step` doit être appelée (ceci est fait par
la fonction `motion_step`). Elle prend en
argument la période nécessaire au PID et l’état du robot (structure `State`
définie dans `libasserv_priv.h` contenant la position et la vitesse).
Suivant le mode, `pid_process` est appelé sur le bon PID, avec en argument
toujours suivant le mode soit la position actuelle, soit la vitesse actuelle.

Remarque : L’asservissement nécessite deux objets `asserv`.
L’un en `delta`, l’autre en `alpha`. Ainsi, la strucure `State` contenant la
position et la vitesse peut désigner soit le couple (`dist`, `delta`), soit le
couple (`theta`, `alpha`).
