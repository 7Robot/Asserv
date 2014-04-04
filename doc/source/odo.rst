Odométrie
=========

Le fichier `odo.c` contient les fonctions liées à l’odométrie (calcul de la
position en intégrant les déplacements élémentaires des deux roues).

Deux structures sont définies dans `odo.c` :

* une structure pour les coefficients nécessaire aux calculs d’odométrie :
  l’entraxe du robot (variable `spacing`) et les coefficients de conversion des
  `tics` en mètre et vice-versa (variables `tic_by_meter` et `meter_by_tic`).
* une strucure `Position` contenant la position actuelle du robot.

La fonction `odo_step` permet de mettre à jour la position du robot tenant
compte des `tics` fournis.
Elle fournit également le déplacement curviligne (`delta`) et la rotation
(`alpha`) engendrée par les `tics` fournis.

Remarque : La position angulaire (`theta`) est en radian et comprise dans
l’intervalle [-PI;PI[.
Attention à bien fournir une valeur dans l’interval attendu: `odo_set_theta`
ne fait aucune vérification !

Pour plus d’informations sur l’odométrie, cet article de club elek (INSA Lyon)
est plutôt compréhensible :
http://clubelek.insa-lyon.fr/joomla/fr/base_de_connaissances/informatique/calcul_d_odometrie_approximation_tra.php
