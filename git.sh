#!/bin/bash

clear
menu="
\e[34mProjet HulaHoop - Interface GitHub\n\e[32m---------------------------------------\n
\e[31m1-\e[0m Pousser une modification sur le serveur\n
\e[31m2-\e[0m Récupérer le dernier code depuis le serveur\n
\e[31m3-\e[0m Quitter\n
\n
"
echo -e $menu
echo Votre choix : 
read choix

#utiliser le select bash
if test $choix = 1
then
	clear
	echo -e $menu
	echo Entrez le nom du commit:
	read commit
	echo
	echo -e $menu

	git add *
	git add */*

	git commit -am "$commit"
	clear
	echo -e $menu
	git push origin master
	echo
else
	if test $choix = 2
	then
		clear
		echo -e $menu 
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

