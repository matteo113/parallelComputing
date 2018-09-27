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

##### Limites fondamentales de performences

- Vitesse de la lumière
- Basé sur les lois fondamentales de la physique E =mc^2 E=h * freq  h:constante de plank.
	- ce qui donne la limite de 10^35 op/sec par kg de matière.
	- mémoire : 2 * 10^31 bits/kg basée sur l'entropie
	- cerveau humain 10^11 neurones, 10^4 synapses --> 10 Hz ==> 10^15 petaflops pour 100 W

Quelle est la meilleure limite pour optimiser la puissance de calcul par rapport à la puissance consommée ?
Beaucoup de proc à faible puissance ou peu de proc à grande puissance ?

Soit n le nb de proc avec la fréquence u, P la puissance consommée et R la puissance de calcul.

on a P=nu2, R=nu

On veut maximiser R/P
nu/nu^2 = 1/u donc la limite u --> 0 et n --> infini est l'optimum.

#### L'architecture

Comment structurer un calcul sur des circuits électroniques donnés ?

Exemple :
- la mémoire cache
- proc vectorielles,  le pipeline
- Architecture RISC
- branchement spéculatifs (spectre)
- parallélisme :
	- au niveau de chque coeur
	- multicoeur
	- multiprocesseur

Une des difficulté est le "power wall" qui dit qu'il est énergétiquement plus cher de bouger les données que de calculer.

#### Les Algorithmes

**Comparaison calcul matriciel 1970 - 2000**
gains de 10000 en 30 ans

**Graph amélioration des machines**
gains semblables à ceux des algorithmes

Autre exemple : cryptographie : casser un code Croissance

Il faut factoriser des très grands nombres.

En 1970 on estimait à 15*10^9 ans le temps pour casser RSA
En 1990 une instance de RSA a été cassée en 8 mois sur 600 machines de travail, grace à des améliorations algorithmiques subtiles.

### 1.3 Evollution des machines HPC

1970 : première machine parallèle en production. Prévision météo ILLIAC IV initiallement prévu avec 256 procs, finalement construite avec 64 pour 4 fois le prix.

1975-1990 machines vectorielles : superordinateurs,
détrône les machines parallèles pendant au moins 15 ans

1990-2000 : Machines parallèles font mieux que les machines vectorielles. (SIMD, MIMD, MPP (massivelly parallel processing) SMP (symetric multiproc)).
Machines dédiées et spécialisées.

1995 : Beowulf, clusters de PC, Technologie ouverte

2005 : Grid, Cloud

2010 : GPU, multicore, MPP

2018 : "Exascale" env 10^18 flop/s

Le parallélisme à tout les niveaux est présent dans les architectures:
- A l'échelle du coeur : ILP Instr Level Parallelism
- Entre Coeur : machines multicoeur
- Entre processeurs : "inter proc Parallelism"

### 1.4 Systèmes parallèles et répartis

Le parallélisme c'est la coopération de plusieurs proc à la solution d'un même problème.

Un système répartis (distribué) : Ce sont plusieurs ordinateurs qui résolvent des problèmes couplés.

Éléments différenciateurs:


|  Systèmes  | Couplage |                 Mise en commun des ressources                | Connaissance mutuelle entre processeurs |    Degré de parallélisme   |
|:----------:|:--------:|:------------------------------------------------------------:|:---------------------------------------:|:--------------------------:|
| Parallèles |   Fort   | Délibérée et souhaitée pour avoir de meilleures performances |                  Forte                  | Important (100'000 coeurs) |
|  Répartis  |  Faible  |                          Inévitable                          |                  Faible                 | Faible (Quelques machines) |

### 1.5 Exploiter le parallélisme

Comment faire efficacement coopérer les processeurs d'une machine parallèle ?

On va illustrer plusieurs stratégies de coopération à travers une situation de la vie de tous les jours.

On va considérer le cas de la famille Dupont composée de Mme Dupont, M. Dupont et 2 enfants Pierre et Marie.
Cette famille doit produire un grand nombre de tartines, en vue d'un goûter d'aniversaire.
On va supposer que les tâches a effectuer sont :
- Couper les tranches
- Beurrer les tartines
- Mettre la confiture
- Ranger les tartines

**1 ière stratégie (séquentielle)**

Mme Dupont fait tout le travail.

C'est la bonne solution car Mme Dupont est habile, rapide, efficace alors que le mari et les enfants sont des "bras-cassés" et ne feront que compliquer le travail.
Si le problème devient trop grand, peut-être que Mme Dupont aura quand même besoin d'aide.

**Deuxième stratégie**

Mme D coupe le pain ==> M D beure ==> Pierre met la confiture ==> Marie range sur le plat.

- Le pipeline requiert que toutes les tâches soient de la même durée.
- Chaque travailleur est un maillon essentiel
- le parallélisme est limité au nombre de taches. ici 4, le petit voisin ne peut pas aider à faire des tartines.
