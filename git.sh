#!/bin/bash

clear
echo Projet HulaHoop - Interface GitHub
echo ---------------------------------------
echo 1- Pousser une modification sur le serveur
echo 2- Récupérer le dernier code depuis le serveur
echo 3- Quitter
echo
echo Votre choix : 
read choix

#utiliser le select bash
if test $choix = 1
then
	clear
	echo Projet HulaHoop - Interface GitHub
	echo ---------------------------------------
	echo 1- Pousser une modification sur le serveur
	echo 2- Récupérer le dernier code depuis le serveur
	echo 3- Quitter
	echo
	echo Entrez le nom du commit:
	read commit
	echo
	echo Projet HulaHoop - Interface GitHub
	echo ---------------------------------------
	echo 1- Pousser une modification sur le serveur
	echo 2- Récupérer le dernier code depuis le serveur
	echo 3- Quitter
	echo

	git commit -am "$commit"
	echo
	git push origin master
	echo
else
	if test $choix = 2
	then
		clear
		echo Projet HulaHoop - Interface GitHub
		echo ---------------------------------------
		echo 1- Pousser une modification sur le serveur
		echo 2- Récupérer le dernier code depuis le serveur
		echo 3- Quitter
		echo
		git pull
		echo
	else
		if test $choix = 3
		then
			clear
			exit 0
		else
			./git.sh
		fi
	fi
fi

