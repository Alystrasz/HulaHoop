#!/bin/bash

clear
menu="
\e[34mProjet HulaHoop - Interface GitHub\n\e[32m---------------------------------------\n
\e[31m1-\e[0m Pousser une modification sur le serveur\n
\e[31m2-\e[0m Récupérer le dernier code depuis le serveur\n
\e[31m3-\e[0m Consulter l'historique des commits\n
\e[31m4-\e[0m Quitter\n
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
	
	echo "Voulez-vous créer un tag ? [y/N]"
	read t
	if test $t = y
	then
		clear
		echo -e $menu
		echo "Entrez le nom du tag:"
		read tag
		git tag $tag
		clear
		echo -e $menu
		git push --tag
		git push origin master
	else
		clear
		echo -e $menu
		git push origin master
	fi
	
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
			echo -e $menu
			git log
			./git.sh
		else	
			if test $choix = 4
			then
				clear
				exit 0
			else
				./git.sh
			fi
		fi
	fi
fi


