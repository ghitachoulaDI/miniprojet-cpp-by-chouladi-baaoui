# StudentManager - Systeme de Gestion des Etudiants

Application C++17/Qt6 complete pour le mini-projet IMS de gestion des etudiants. Le projet couvre l'OOP, la STL, les exceptions personnalisees, la persistance fichier, une GUI Qt moderne, les tests, Git/GitHub et les bonus.

## Fonctionnalites principales

- Trois types d'etudiants : Licence, Master, Doctorat.
- Classe abstraite `Student` et polymorphisme avec `computeScholarship()`, `getType()`, `display()`.
- CRUD complet : ajouter, modifier, supprimer, rechercher, rafraichir.
- Recherche dynamique par nom, ID, type, filiere, sujet ou superviseur.
- Filtres bonus par type et plage GPA.
- Tri par GPA decroissant et tri par nom alphabetique.
- Statistiques : nombre total, moyenne GPA, nombre par type.
- Export CSV compatible Excel.
- Histogramme GPA avec QtCharts.
- Mode sombre / clair.
- Persistance automatique dans `data/students.txt`.
- Tests metier dans `tests/test_main.cpp`.

## Compilation avec VS Code

Pre-requis : Qt 6 avec Widgets et Charts, CMake, MinGW, extensions C/C++ et CMake Tools.

```powershell
cd "C:\Users\Pc\Downloads\StudentManager"
$env:CMAKE_PREFIX_PATH="C:\Qt\6.11.1\mingw_64"
$env:Path="C:\Qt\6.11.1\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;$env:Path"
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
.\build\StudentManager.exe
```

## Compilation avec Qt Creator

Ouvrir `StudentManager.pro`, selectionner un kit Qt6 Desktop, puis Build et Run.

## Tests

```powershell
cmake --build build --target StudentManagerTests
ctest --test-dir build --output-on-failure
```

## Format de persistance

```text
Licence|1001|Sara Benali|3.72|Informatique
Master|1002|Youssef Amrani|3.40|Intelligence Artificielle
Doctorat|1003|Nadia El Fassi|3.90|Dr. Karim Idrissi|3
```

## GitHub recommande

- Branche `feature/oop` pour les classes metier.
- Branche `feature/services` pour StudentManager.
- Branche `feature/gui` pour l'interface Qt.
- Branche `feature/tests` pour les tests.
- Pull requests vers `main`.
- Minimum 10 commits au nom des deux membres.

## Lien Video


## Auteurs

- Marwa Baaoui
- Binome : a completer
