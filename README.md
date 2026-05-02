# Crazy-Circus
# Crazy Circus

## Description

Crazy Circus est un jeu de réflexion stratégique implémenté en langage C. Le principe consiste à manipuler des animaux placés sur deux podiums (bleu et rouge) afin de transformer une configuration initiale en une configuration cible, en utilisant une suite limitée d’actions.

Chaque joueur doit proposer une séquence d’ordres permettant de résoudre l’objectif le plus rapidement possible. Le jeu met ainsi en concurrence plusieurs joueurs qui s’affrontent tour à tour.

---

## Principe du jeu

Les animaux sont empilés sur deux podiums :

* Podium bleu
* Podium rouge

Une pile représente un empilement d’animaux sur un même podium.

Les joueurs utilisent des commandes prédéfinies pour modifier l’état du jeu et atteindre la configuration cible.

---

## Commandes disponibles

| Commande | Effet                                          |
| -------- | ---------------------------------------------- |
| KI       | Déplace le sommet du podium bleu vers le rouge |
| LO       | Déplace le sommet du podium rouge vers le bleu |
| SO       | Échange les sommets des deux podiums           |
| NI       | Remonte l’animal du bas vers le haut (bleu)    |
| MA       | Remonte l’animal du bas vers le haut (rouge)   |

---

## Objectifs du projet

Ce projet a pour but de :

* Simuler une partie complète du jeu Crazy Circus
* Gérer plusieurs joueurs et leurs scores
* Vérifier la validité des séquences proposées
* Appliquer des transformations sur des états de jeu
* Respecter des contraintes strictes d’affichage et de fonctionnement

---

## Concepts utilisés

Ce projet permet de mettre en pratique :

* Structures de données (piles)
* Gestion des entrées utilisateur
* Simulation d’états
* Algorithmes de transformation
* Organisation modulaire en C

---

## Fonctionnement général

1. Chargement de la configuration du jeu
2. Initialisation des joueurs
3. Génération et sélection des états de jeu
4. Déroulement des tours
5. Vérification des séquences proposées
6. Attribution des points
7. Affichage du classement final

