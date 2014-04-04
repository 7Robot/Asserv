Introduction
============

Cette partie contient quelques généralités sur le fonctionnement de
l’asservissement.


Fonctionnement général
----------------------

L’asservissement fonctionne de manière discrète, à une fréquence de l’ordre de
100Hz à 1000Hz (10ms à 1ms entre chaque `step`). La valeur optimale est toujours
en débat, une durée plus faible est théoriquement plus précise mais implique
également plus de calculs, avec les erreurs d’arrondi associées (notamment lors
des calculs trigonométrique).

À chaque `step`, l’asservissement prend en entrée les `tics` des roues gauche et
droite depuis la dernière itération et ressort les nouvelles consignes pour
chaque roue. Derrière cela se cachent pas mal de calcul !


Odométrie
---------

Les `tics` du `step` courant sont d’abord envoyés à l’odométrie, qui met à jour
les coordonnées du robot. On pourra par exemple consulter cette page de
`clubelek` (INSA Lyon) pour plus d’informations :
http://clubelek.insa-lyon.fr/joomla/fr/base_de_connaissances/informatique/calcul_d_odometrie_approximation_tra.php.

Asservissement polaire
----------------------

Le robot se commande par une consigne sur la roue droite et une consigne sur la
roue gauche, et non directement par une consigne de position absolue.
Ainsi, si l’on souhaite avancer d’un mètre dans une direction fixe, on souhaite
*à priori* que chaques roues parcoure un mètre.
Cependant, ceci a un petit inconvénient : si une roue se voit retardée par
rapport à l’autre, le robot va dévier de sa trajectoire.

C’est pourquoi l’on préfère effectuer un asservissement polaire : on ne
considère plus des consignes roue droite / roue gauche, mais des consignes
delta (somme roue droite / roue gauche) & alpha (différence roue droite / roue
gauche).
À cela s’ajoutent quelques coefficients afin de tenir compte des encodeurs, de la
taille des roues et de leur entraxe.
Le calcul d’alpha nécessite théoriquement un sinus, mais on considère être dans
une situation de petite angle.

.. math::

    \delta = \frac{\mathrm{ticsDroit} + \mathrm{ticsGauche}}{2}
                \cdot \mathrm{meterByTic}

    \alpha = \frac{\mathrm{ticsDroit} - \mathrm{ticsGauche}}{\mathrm{entraxe}}
                \cdot \mathrm{meterByTic}

L’asservissement est alors fait sur ces deux grandeurs, et la transformation
inverse est effectuée pour transmettre les consignes aux moteurs.

Ces deux variables contiennent des valeurs dérivées : il s’agit seulement de la
variation par rapport au dernier `step`. Nous appellerons respectivement `dist`
et `theta` leur valeur absolue, c’est à dire leur intégration au cours du temps
(ainsi, à chaque `step, `dist` se voit incrémenté de la dernière valeur de `delta`
et `theta` se voit incrémentée de la dernière valeur d'`alpha`).


Génération des consignes
------------------------

Nous avons à présent une variable `dist` et une variable `theta`.
Pour effectuer notre déplacement d’un mètre linéaire, il suffit de fixer un
objectif d’un mètre pour la variable `dist` et de maintenir nulle la valeur de
`theta`.

La variable theta peut être directement asservie par un PID : celui-ci
s’occupera de corriger les erreurs dès que le robot s’écartera de la direction
voulu.
En revanche, il n’est pas possible d’utiliser directement un PID sur la distance
finale que doit parcourir le robot. En effet, la réponse d’un PID est peu
souhaitée : accélération lente, dépassement, oscillations, pas de contrôle de
l’accélération ou de la vitesse …

Comme nous sommes exigeant, nous voulons un déplacement « parfait » du robot :
une première phase d’accélération, avec une accélération constante déterminée par
l’adhérence des roues sur le sol, une phase constante à la vitesse maximale
possible pour le robot (sans qu’il n’en perdre ses boulons), et une phase de décélération telle
que le robot ait une vitesse nulle en atteignant son objectif.

Pour cela, à chaque `step`, nous calculons la position que devrait avoir le
robot pour suivre ce profil idéal. Cette position sert alors d’objectif à un
régulateur PID. Bien évidemment, la consigne de ce régulateur PID se voit
modifiée à chaque `step`. Cependant, la consigne évolue faiblement à chaque
`step`, et le PID n’a donc qu’une faible correction à effectuer.

Ce principe de génération de consigne de position est également utilisé pour les
consignes de vitesse. En effet, là encore un simple PID aurait un comportement
peu enviable : accélération non contrôlée, oscillations autour de la vitesse finale …
De la même manière, nous calculons à chaque `step` la position souhaitée pour que
le robot suive un profil d’accélération contrôlé et respecte la consigne de
vitesse. L’asservissement se fait encore en position par un PID.


.. image:: img/ramp_dist.png
    :width: 800px
    :align: center

Voici un exemple de rampe générée.
Les paramètres utilisés sont une distance d’un mètre, des vitesses initiale et
finale nulles, une accélération de 0.2 mètres par seconde et une vitesse maximale
de 0.3 mètres par seconde. On remarque de nombreuse fluctuations de l’accélération
dûes aux méthodes interne de calcul de celle-ci (cf :doc:`Rampe de distance <ramp_dist>`) mais cela n’a que très
peu d’influence sur les consignes de positions et ne gène en rien la régulation.


Architecture
------------

La bibliothèque d’asservissement est indépendante de son implémentation sur pic
(elle est prévue pour être également utilisée dans un simulateur).

Elle est divisée en plusieurs partie, du plus bas au plus au niveau :

* `pid`: implémentation d’un régulateur pid
* `asserv`: sélection d’un pid en position, en vitesse ou arrêt (roue libre)
* `motion`: gestion des fonctions de déplacement, qui repose sur l’utilisation de
  deux asserv : un en delta et l’autre en alpha.

L’utilisateur n’accède qu’aux fonctions fournies par le module `motion`. C’est
le module `motion` qui instancie derrière deux `asserv`, elles même instanciant
deux `pid` chacune.

À cela s’ajoutent des fonctions de génération de rampe, utilisées par `motion`, et
un module d’odométrie calculant la position absolue du robot.

Les fonctions disponibles pour l’utilisateur sont référencées dans `libasserv.h`.
Les autres fonctions, lorsque partagé entre plusieurs parties de la bibliothèque,
sont référencées dans `libasserv_priv.h`.
Les coefficients par défaut pour les différents PID sont référencés dans
`libasserv_default.h`.
Le fichier `libasserv_robot.h` contient les coefficients propres au robot. Ce
header doit être inclu dans `libasserv.h` pour être utilisé au lieu des
coefficients par défaut.
