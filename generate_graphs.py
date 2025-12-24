import csv
import matplotlib.pyplot as plt
import os

# Créer un dossier pour les graphes individuels s'il n'existe pas
if not os.path.exists('graphes'):
    os.makedirs('graphes')

data = {}

# Lecture des données
with open('resultats.csv', newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        algo = row['Algorithm']
        if algo not in data:
            data[algo] = {'N': [], 'Time': []}
        data[algo]['N'].append(int(row['N']))
        data[algo]['Time'].append(float(row['Time']))

# Génération d'un graphe simple (Temps uniquement) par algorithme
for algo, values in data.items():
    plt.figure(figsize=(8, 5))
    plt.plot(values['N'], values['Time'], marker='o', color='blue', linestyle='-')
    plt.title(f'Temps d\'exécution : {algo}')
    plt.xlabel('Taille du tableau (N)')
    plt.ylabel('Temps moyen (secondes)')
    plt.grid(True)
    
    # Sauvegarde du fichier
    filename = f'graphes/Graphe_{algo}.png'
    plt.savefig(filename)
    plt.close()
    print(f"Généré : {filename}")

# Graphe comparatif global du Temps
plt.figure(figsize=(10, 6))
for algo, values in data.items():
    plt.plot(values['N'], values['Time'], marker='o', label=algo)

plt.title('Comparaison Globale : Temps d\'exécution de tous les tris')
plt.xlabel('Taille du tableau (N)')
plt.ylabel('Temps moyen (secondes)')
plt.legend()
plt.grid(True)
plt.savefig('graphes/comparaison_globale_temps.png')
plt.close()

print("\nSeuls les graphes de temps ont été générés dans le dossier 'graphes/'.")
