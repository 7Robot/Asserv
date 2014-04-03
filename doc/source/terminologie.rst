Terminologie
============

* `step`: L’asservissement étant discret, les consignes sont recalculé à chaque
  pas appelé `step` dans cette documentation.
* `dist`: Abscisse curviligne parcouru par le centre du robot. Il s’agit de la
  moyenne du déplacement des roues droite et gauche.
* `delta`: Évolution de la distance curviligne entre deux `step` (autrement
  dit, la dérivé première de la variable `dist`).
* `theta`: Orientation angulaire du robot. Il s’agit de la différence entre le
* `alpha`: Évolution de l’orientation angulaire du robot (ie la dérivé première
  de théta).
  déplacement de la roue droite et celui de la roue gauche.
* `tic`: Les `tics` désignent l’unité de comptage des roues codeuse.
