Python
======

La bibliothèque possède un wrapper en python. Celui-ci permet de l’utiliser par
exemple dans un simulateur.

Ce wrapper définie un objet python `Asserv` possèdant des méthodes pour régler
le type d’asservissement désiré, et la bien connu méthode `step` pour calculer
les consignes d’une itération.

La bibliothèque C est appelé en utilisant `ctypes`.
