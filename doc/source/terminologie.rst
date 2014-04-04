Terminologie
============

* `step`: L’asservissement étant discret, les consignes sont recalculées à chaque
  pas appelé `step` dans cette documentation.
* `period`: Le temps entre deux `step`.
* `dist`: Abscisse curviligne parcourue par le centre du robot. Il s’agit de la
  moyenne du déplacement des roues droite et gauche.
* `delta`: Évolution de la distance curviligne entre deux `step`.
* `speed`: Vitesse curviligne. Peut-être calculé en fonction de `delta` et de la
  période.
* `theta`: Orientation angulaire du robot. Il s’agit de la différence entre le
* `alpha`: Évolution de l’orientation angulaire du robot.
  déplacement de la roue droite et celui de la roue gauche.
* `omega`: Vitesse angulaire de rotation. Peut être calculé en fonction
  d'`alpha` et de la période.
* `tic`: Les `tics` désignent l’unité de comptage des roues codeuse.

Remarques sur les unités :

* `dist` est en mètre et `theta` en radian
* `delta` est en mètre par `step` et `alpha` en radian par `step`
* `speed` est en mètre par seconde et `omega` en radian par seconde
* `period` est en seconde par `step`

On a donc les égalités suivante :

.. math::

    \mathrm{speed} = \frac{\mathrm{delta}}{\mathrm{period}}

    \mathrm{omega} = \frac{\mathrm{alpha}}{\mathrm{period}}

Attention ! Des fois `delta` et `alpha`, notamment lorsqu’ils constituent le nom
d’une autre variable, sont juste présent pour faire la différence entre
l’asservissement rectiligne et l’asservissement de l’orientation, mais ne
désignent pas nécessairement une distance par `step` ou rotation par `step`.
