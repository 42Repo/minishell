#!/bin/bash

# Vérifier si le fichier est un .c ou .h
if [[ $1 =~ \.(c|h)$ ]]; then
  # Supprimer les 10 premières lignes du fichier
  tail -n +11 "$1"
else
  # Sinon, afficher le contenu du fichier tel quel
  cat "$1"
fi
