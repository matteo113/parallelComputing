# Parallélisme

# Chap 1

### 1.1 Introduction

#### Définition

Le parallélisme consiste à utiliser **plusieurs** cœurs qui **coopèrent** à la solution d'**un même problème**.

But: résoudre plus rapidement ce problèmes ou résoudre un problème plus gros.

Approche basée sur l'idée de performance.

Le parallélisme est naturel : Dans la vie de tout les jours, la coopération entre plusieurs travailleurs est habituelle.

La collaboration implique une coordination.

==> travail supplémentaire (overhead)

D'un coté on gagne en performance car plusieurs à travailler mais on perd à cause de la coordination.

#### Introduction

ENIAC : un des premiers ordinateurs dans les années 1940, équipé de plusieurs unités de calcul activables en même temps

Les premières tentatives d'implémenter du parallélisme dans les ordinateurs n'ont pas été un succès:
- manque de fiabilité des composants matériels
- pas de modèles de programmations qui collaient à la structure matérielle.

C'est à ce moment que l'architecture de Von Neuman s'est développée avec succès.

CPU sequ <==> data + instr

Modèle simple, très bien défini, qui permet le développement de logiciels adaptés sur la base d'un modèle de programmation unique.

Notre monde à toujours besoin de plus de performances et on ne peut pas se contenter de solution figée.

Les besoins augmentent à mesure que les machines plus puissantes sont disponibles.

==> Il n'y aura jamais une machine assez puissante pour satisfaire les besoins de tous.

En 2018 l'ordinateur le plus puissant du monde (IBM avec GPU nvidia) estcapable de 187 petaflops (peta = 10^15) avec 2.4 millions de cœurs avec 8 megawatts de consommation.

En suisse, au CSCS (Lugano) 361 000 cœurs, 25 petaflops, top 6 mondial et 1er en Europe.

les performances sont calculées en flop/s = nombre d'opérations en virgules flottantes par secondes.

On parle aussi de HPC (high Performances Computation) pour désigner les calculs sur ces machines.

HPC : 2 ordres de grandeurs (x100) qu'un ordinateur standard.

**Grands défis de calculs :**
- Applications militaires
- Météo
- Prédictions climatiques
- Biomedical

La principale limite du séquentiel c'est la chaleur dissipée. Pour augmenter la vitesse d'un processeur on doit augmenter la fréquence mais on augmente ainsi la chaleur dissipée.

Un deuxième aspect est ce qu'on appel "le goulet de Von Neuman" => le temps d'accès à la mémoire est très lent par rapport aux processeurs.

C'est l'énergie nécessaire au transport fiable des données sur la distance mémoire CPU qui limite l'accès mémoire (bande passante Byte/s)

Actuellement l'énergie dépensée pour bouger les données dans l'ordinateur est largement supérieur à celle nécessaire pour une opération arithmétique.

#### Roofline model

C'est un model qui montre les limites d'un processeur.

Texec = Taccès + Tcalcul = Phit Tcache + Pmiss Tmemory + I/Rmax

I = nb de calcul qu'on fait avec la même donnée
R = execution rate (vitesse du proc) en instr/s ou flop/s


**AJOUTER FORMULES (et les limites et je sais pas quoi)**

### 1.2 Où trouver des Performances

Sur quels aspects peut-on jouer pour augmenter les performances ?

1. La technologie
2. L'architecture
3. Algorithmes

#### La Technologie

faire des circuits plus rapides.

Utiliser des processus physiques qui permettent de calculer plus vite.

Actuellement la technologie utilisée se repose sur les semi-conducteurs au silicium.

Dans les semi-conducteurs au silicium, le plus simple est d'augmenter la fréquence.

La loi de Denard : puissance consomée +-= V^2*freq

Mais on peut diminuer le voltage des transistors.

Il faut diminuer la taille physique des transistors sur la puce (finesse du trait en photolithogravure)

Pendant Longtemps on avait la relation Freq +-= 1/finesse de trait

Actuellement la finesse du trait est environ de 10 nm.

Si on miniaturise trop le semi-conducteur cesse de fonctionner:
- Effet tunel : effet quantique ou les isolants ne marche plus.
- Le nb d'électrons qui distingue l'état 1 de l'état 0 dans le transistor ne peut pas être inférieur à 1.

Ce chemin pour augmenter les performances a atteint ses limites. Maintenant les fréquences restent stables.

Croissance exponentielle des performances sur plusieurs années.
