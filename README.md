Asserv
======

Asserv
------
ramp_dist: Générateur de rampe pour des consignes de distance
ramp_speed: Générateur de rampe pour des consignes de vitesse
odo: Odométrie
asserv: Asservissement en position ou en vitesse, usuellement deux instances
sont utilisé, l’une pour l’asserv en delta, et l’autre pour l’asserv en alpha.
pid: Pid, 2 instances sont utilisé par chaque objet asserv (un pour l’asserv en
position, et l’autre pour l’asserv en vitesse)
motion: Communique les consignes à asserv, les génère préalablement si
nécessaire grâce à ramp_dist et ramp_speed. Propose pleins de fonctions de
contrôle, mais seulement 4 devrait être utilisé :
- motion_dist_rot, consigne de distance et/ou rotation (relative)
- motion_speed, consigne de vitesse rectiligne
- motion_omega, consigne de vitesse en rotation
- motion_speed_omega, consigne de vitesse rectiligne et rotation
Les autres fonctions risques de créer quelques secousses lorsque enchaînées de
manière défavorable. C’est également le cas des consigne de vitesse et omega
pour le moment, faire attention à être à l’arrêt sur l’autre axe.
La fin des consignes est mal (ou pas) géré pour le moment : seules les consignes de distance et rotation provoque un done lors de la fin de la génération de rampe, il est prévu de rajouter une condition sur les pid.


Simu
----
Simulateur en python + box2d


Pic
---
Code pour dsPIC33F
