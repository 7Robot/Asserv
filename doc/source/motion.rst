Motion
======

Le fichier `motion.rst` contient les fonctions avancé de gestion du déplacement
du robot. C’est sans doute la plus grosse partie de la bibliothèque
d’asservissement.

Le module `motion` instancie deux `asserv` : l’un en `delta`, l’autre en
`alpha`. Tout le travail de ce module consiste à déterminer les consignes et
modes de ces deux `asserv`.

Les fonctions de ce module sont pour la plupart public et leurs prototypes sont
référencé dans `libasserv.h`.


Variables définites
-------------------

* `deltaState`, `alphaState`: état **mesuré** du robot (position et vitesse)
* `deltaOrder`, `alphaOrder`: état **désiré** du robot (position, vitesse et
  accélération) **pour le `step` courant** (consigne pour les PID)
* `deltaFinalOrder`, `alphaFinalOrder`: consignes finals, utilisé dans les
  rampes et non directement par les PID
* `deltaMode`, `alphaMode`: mode de génération des consignes pour les PID
  (fixe (manuel), rampe de position, ramp de vitesse)
* `epsDist/Speed/Theta/Omega`: seuils de détection d’une consigne atteinte (cf
  :doc:`Pid <pid>`)
* `(v/a/d)(Dist/Rot)Max(Default)`: vitesse, accélération et décélération maximum
  effective, pour l’asserv en delta et l’asserv en alpha
  (ainsi que les valeurs par défaut chargé depuis les headers)


Fonctions générales
-------------------

motion_init
```````````

L’initialisation du module `motion` prend un argument une fonction de callback
qui sera appelé lorsque l’objectif est atteint.


motion_set_epsilons
```````````````````

Ce setter permet de définir les seuils d’erreur sur les PID avant que la
consigne ne soit considéré comme atteinte.

* `Ed`: epsilon dist
* `Es`: epsilon speed (delta)
* `Et`: epsilon theta
* `Eo`: epsilon omega (alpha)


motion_step
```````````

Cette fonction, appelé par l’utilisateur à chaque `step`, gère tout
l’asservissement.

Elle commence par appeler `odo_step` afin de mettre à jour la position du robot.
`odo_step` lui renvoit alors les variables `delta` et `alpha` qui sont utilisées
pour mettre à jour l’état du robot (`deltaState`, `alphaState`).

Pour chaque asservissement (delta, alpha), `motion_step` observe le mode de
fonctionnement.
Si ce mode nécessite l’utilisation d’une rampe, les fonctions de génération de
rampe sont appelé afin de mettre à joursr les variables `*Order`.
Si le mode est manuel, les variables `*Order` contiennent déjà la consigne à
fournir au PID.

Le module `asserv` est alors appelé pour exécuter les bons PID.

Les commandes pour les roues droite et gauche en sont déterminé :

.. math::

    \mathrm{cmdLeft} = \mathrm{cmdDelta} - 2 \cdot \mathrm{cmdAlpha}

    \mathrm{cmdRight} = \mathrm{cmdDelta} + 2 \cdot \mathrm{cmdAlpha}


Fonctions fixant une consigne
-----------------------------

motion_stop
```````````

Les deux `asserv` sont désactivé (roue libre).


motion_dist
```````````

Parcourir une distance donnée avec rampe.

* `delta`: rampe de position, asserv en position pour suivre les positions
  générées par la rampe
* `alpha`: pas de rampe, asserv en position, la consigne de position étant nulle

motion_reach_x
``````````````

Calcul la distance nécessaire pour intersecter l’axe x=X et appelle
`motion_dist`.

motion_reach_y
``````````````

Calcul la distance nécessaire pour intersecter l’axe y=Y et appelle
`motion_dist`.


motion_rot
``````````

Rotation d’un angle (relatif) donnée avec rampe.

* `delta`: pas de rampe, asserv en position, la consigne de position étant nulle
* `alpha`: rampe de position, asserv en position pour suivre les positions
  générées par la rampe

motion_reach_theta
``````````````````

Calcul la rotation a effectuer pour être orienté dans la direction theta=T et
appelle `motion_rot`.

motion_dist_rot
```````````````

S’orienter dans la direction theta=T tout en effectuant un déplacement linéaire
d’une longueur donnée, avec rampe pour les deux asservissements.

* `delta`: rampe de position, asserv en position pour suivre les positions
  générées par la rampe
* `alpha`: rampe de position, asserv en position pour suivre les positions
  générées par la rampe

motion_dist_free
````````````````

Parcourir une distance donnée sans rampe.
Utile surtout pour observer la réponse impulsionnelle du PID.

* `delta`: pas de rampe, asserv en position, consigne de position fixé par
  l’utilisateur
* `alpha`: pas de rampe, asserv en position, consigne de position nulle

motion_rot_free
```````````````

Effectuer une rotation d’un angle (relatif) donnée sans rampe.
Utile surtout pour observer la réponse impulsionnelle du PID.

* `delta`: pas de rampe, asserv en position, consigne de position nulle
* `alpha`: pas de rampe, asserv en position, consigne de position fixé par
  l’utilisateur

motion_speed
````````````

Avancer à une vitesse donnée.

* `delta`: rampe de vitesse, asserv en vitesse afin de suivre les consignes de
  vitesses générées par la rampe
* `alpha`: pas de rampe, asserv en position, consigne de position nulle

motion_omega
````````````

Tourner sur place à une vitesse angulaire donnée.

* `delta`: pas de rampe, asserv en position, consigne de position nulle
* `alpha`: rampe de vitesse, asserv en vitesse afin de suivre les consignes de
  vitesses générées par la rampe

motion_speed_free
`````````````````

Avancer à une vitesse donnée sans rampe.
Utile surtout pour observer la réponse impulsionnelle du PID.

* `delta`: pas de rampe, asserv en vitesse, consigne de vitesse fixé par
  l’utilisateur
* `alpha`: pas de rampe, asserv en position, consigne de position nulle

motion_omega_free
`````````````````

Tourner sur place à une vitesse angulaire donnée sans rampe.
Utile surtout pour observer la réponse impulsionnelle du PID.

* `delta`: pas de rampe, asserv en position, consigne de position nulle
* `alpha`: pas de rampe, asserv en vitesse, consigne de vitesse fixé par
  l’utilisateur


Remarque : Pour `motion_speed` et `motion_omega`, on aurait pû imaginer générer
des positions assurant un déplacement à la vitesse voulu. Cependant, les
expérimentations ont révélé de nombreuse oscillation autour de la consigne de
vitesse, un asservissement en vitesse s’étant montrer bien plus performant.
