# Parallélisme

# Chap 1

### 1.1 Introduction

Le parallélisme consiste à utiliser **plusieurs** cœurs qui **coopèrent** à la solution d'**un même problème**.

But: résoudre plus rapidement ce problèmes ou résoudre un problème plus gros.

Approche basée sur l'idée de performance.

Le parallélisme est naturel : Dans la vie de tout les jours, la coopération entre plusieurs travailleurs est habituelle.

La collaboration implique une coordination.

==> travail supplémentaire (overhead)

D'un coté on gagne en performance car plusieurs à travailler mais on perd à cause de la coordination.

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
