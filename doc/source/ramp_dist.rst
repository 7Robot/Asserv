Rampe de distance
=================

Le fichier `ramp_dist.c` contient un générateur de rampe pour atteindre une
consigne de distance.

Cette fonction prend en argument la distance à parcourir, l’état actuelle
(position, vitesse), la vitesse souhaité lorsque la distance aura été parcouru,
ainsi que l’accélération et la vitesse maximal tolérés.

La fonction de génération de rampe est totalement stateless : elle ignore si
elle est dans une phase d’accélération, de décélération ou dans une phase
constante. Elle se contente de prendre en argument la vitesse actuelle et de
déterminer, compte tenu de la distance à parcourir et la vitesse final
désiré, si il est possible d’accélérer ou au contraire s’il faut ralentir.

Cette approche permet de modifier la consigne final à tout moment, lors du
prochain `step` l’action la plus judicieuse est alors effectué (accélération ou
décélération).

Ceci a pour effet de bord de faire apparaître des oscillations sur
l’accélération lors des phases de vitesse constante.
L’asservissement se faisant sur les consignes de position, cela est bien lissé
et sans conséquence.


Traçé de profil avec gnuplot
----------------------------

Le script `plot-dist.sh` permet de tracer des rampes avec gnuplot en utilisant
le petit utilitaire `test_ramp_dist`.
Les graphe généré peuvent être récupéré dans `/tmp`.


Exemples
--------

Voici quelques exemples commenté de rampes pouvant être généré.

Exemple 1
`````````

* Distance : 1 m
* Vitesse initial : 0 m.s⁻¹
* Vitesse finale : 0 m.s⁻¹
* Vitesse max : 1 m.s⁻¹
* Accélération max : 1 m.s⁻²

.. image:: img/ramp_dist_classic.png
    :width: 800px
    :align: center

Courbe classique avec une phase d’accélération, une phase « constante »
(artefacte dû au fait qu’on ne roule jamais à la bonne vitesse mathématiquement
parlant) et une phase de décélération. La consigne est bien atteinte.

Exemple 2
`````````

* Distance : 1m
* Vitesse initial : 0 m.s⁻¹
* Vitesse finale : 1 m.s⁻¹
* Vitesse max : 1 m.s⁻¹
* Accélération max : 1 m.s⁻²

.. image:: img/ramp_dist_speed_end.png
    :width: 800px
    :align: center

Cette fois-ci, la vitesse final n’est pas nul. La consigne est bien atteinte.

Exemple 3
`````````

* Distance : 1 m
* Vitesse initial : 0 m.s⁻¹
* Vitesse finale : 0.5 m.s⁻¹
* Vitesse max : 1 m.s⁻¹
* Accélération max : 1 m.s⁻²

.. image:: img/ramp_dist_speed_end_inf.png
    :width: 800px
    :align: center

Même graphique que l’exemple 2, mais la vitesse final est inférieur à la vitesse
maximale faisant alors apparaître un petit palier constant.
La consigne est bien atteinte.

Exemple 4
`````````

* Distance : 0.5 m
* Vitesse initial : 0 m.s⁻¹
* Vitesse finale : 1 m.s⁻¹
* Vitesse max : 1 m.s⁻¹
* Accélération max : 1 m.s⁻²

.. image:: img/ramp_dist_too_slow.png
    :width: 800px
    :align: center

La position voulu est bien atteinte, mais la vitesse désiré est trop faible
malgré une accélération maximal constante. Tans pis !

Exemple 5
`````````

* Distance : 1 m
* Vitesse initial : 2 m.s⁻¹
* Vitesse finale : 0 m.s⁻¹
* Vitesse max : 0.8 m.s⁻¹
* Accélération max : 1 m.s⁻²

.. image:: img/ramp_dist_depassement.png
    :width: 800px
    :align: center

Ici, la consigne de distante est rapidement atteinte, mais la consigne de
vitesse ne l’est pas ! En effet, la vitesse initiale de 2 m.s⁻¹ est trop
importante pour pouvoir s’arrêter en 1 mètre .

Cependant, en prolongeant la rampe, on constate que les deux consignes (distance
et vitesse) sont bien atteinte (en marche arrière !).

Exemple 6
`````````

* Distance : 1 m
* Vitesse initial : -1 m.s⁻¹
* Vitesse finale : 1 m.s⁻¹
* Vitesse max : 1 m.s⁻¹
* Accélération max : 1 m.s⁻²

.. image:: img/ramp_dist_neg.png
    :width: 800px
    :align: center

La vitesse initiale est négative, mais cela n’empêche pas de respecter les
consignes de distance et de vitesse.
