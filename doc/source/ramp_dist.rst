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

Le script `plot-dist.sh` permet de tracer les rampes générés.


Exemples
--------

Voici quelques exemples commenté de rampes pouvant être généré.


