PID
===

Le fichier `pid.c` contient une implémentation d’un correcteur `PID
<https://fr.wikipedia.org/wiki/R%C3%A9gulateur_PID>`.

Les PID ne sont pas utilisés directement par l’utilisateur, mais par le module
`asserv`. Les prototypes des fonctions sont donc référencées dans
`libasserv_priv.h`.

On peut y trouver également deux structures :

* `PidCoefs`: les coefficients du PID (kp, ki, kd)
* `Pid`: la structure contenant toutes les données du pid (consigne, erreur,
  terme intégral, terme dérivé, et bien évidement une structure `PidCoefs`)

On remarquera que la consigne (`order`) est un pointeur, la fonction
`pid_set_order` permettant de le définir.
Ainsi, il suffit de modifier sa variable contenant la consigne sans devoir à
chaque fois ré-informer le PID de la modification (celui-ci accédant directement
à notre variable).

Outre les setter pour les coefficients et pour la consigne, le module `pid`
défini 3 fonctions :

* `pid_reset`: Vide le terme intégral.
* `pid_process`: Calcule la prochaine consigne. Pour cela, prend en argument la
  position actuelle ainsi que la période (variable `period`, temps entre deux
  `step`). Il s’agit ici d’un calcul standard de PID, précédé par le calcul de
  l’erreur, du terme dérivé et la mise à jour du terme intégrale.
* `pid_done`: Renvoit un booléen indiquant si la consigne a été atteinte. Pour
  cela, prend en argument un seuil sur l’erreur et un seuil sur le terme dérivé
  (on doit avoir atteint l’objectif avec une variation nulle, sans quoi on est
  dans une phase de dépassement).

