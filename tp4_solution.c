#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

// --- Global variables for counting comparisons ---
long long counts = 0;

// --- Helper Functions ---
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void fill_random(int *tab, int n) {
    for (int i = 0; i < n; i++) {
        tab[i] = rand() % 10000;
    }
}

void copy_array(int *src, int *dest, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

// --- 1. Bubble Sort (Tri à bulles) ---

void TriBulle(int *tab, int n) {
    bool change;
    for (int i = 0; i < n - 1; i++) {
        change = false;
        for (int j = 0; j < n - i - 1; j++) {
            counts++;
            if (tab[j] > tab[j + 1]) {
                swap(&tab[j], &tab[j + 1]);
                change = true;
            }
        }
        if (!change) break;
    }
}

void TriBulleOpt(int *tab, int n) {
    int i = n - 1;
    while (i > 0) {
        int derniereSwap = 0;
        for (int j = 0; j < i; j++) {
            counts++;
            if (tab[j] > tab[j + 1]) {
                swap(&tab[j], &tab[j + 1]);
                derniereSwap = j;
            }
        }
        i = derniereSwap;
    }
}

// --- 2. Shaker Sort (Tri Gnaque) ---

void TriGnaque(int *tab, int n) {
    bool swapped = true;
    int start = 0;
    int end = n - 1;
    while (swapped) {
        swapped = false;
        for (int i = start; i < end; ++i) {
            counts++;
            if (tab[i] > tab[i + 1]) {
                swap(&tab[i], &tab[i + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
        swapped = false;
        --end;
        for (int i = end - 1; i >= start; --i) {
            counts++;
            if (tab[i] > tab[i + 1]) {
                swap(&tab[i], &tab[i + 1]);
                swapped = true;
            }
        }
        ++start;
    }
}

// --- 3. Radix Sort (Tri par distribution) ---

int Extraire(int val, int j) {
    int p = (int)pow(10, j);
    return (val / p) % 10;
}

void TriBulleAlt(int *tab, int n, int j) {
    for (int i = 0; i < n - 1; i++) {
        for (int k = 0; k < n - i - 1; k++) {
            counts++;
            if (Extraire(tab[k], j) > Extraire(tab[k + 1], j)) {
                swap(&tab[k], &tab[k + 1]);
            }
        }
    }
}

void TriDistribution(int *tab, int n, int k) {
    for (int j = 0; j < k; j++) {
        TriBulleAlt(tab, n, j);
    }
}

// --- 4. Quick Sort (Tri Rapide) ---

int partitionner(int *tab, int debut, int fin) {
    int pivot = tab[debut];
    int i = debut + 1;
    int j = fin;
    while (i <= j) {
        while (i <= fin && tab[i] <= pivot) {
            counts++;
            i++;
        }
        while (j > debut && tab[j] > pivot) {
            counts++;
            j--;
        }
        if (i < j) {
            swap(&tab[i], &tab[j]);
        }
    }
    swap(&tab[debut], &tab[j]);
    return j;
}

void triRapide(int *tab, int debut, int fin) {
    if (debut < fin) {
        int p = partitionner(tab, debut, fin);
        triRapide(tab, debut, p - 1);
        triRapide(tab, p + 1, fin);
    }
}

// --- 5. Heap Sort (Tri par tas) ---

void Tamiser(int *tab, int n, int i) {
    int max = i;
    int gauche = 2 * i + 1;
    int droite = 2 * i + 2;
    if (gauche < n) {
        counts++;
        if (tab[gauche] > tab[max]) max = gauche;
    }
    if (droite < n) {
        counts++;
        if (tab[droite] > tab[max]) max = droite;
    }
    if (max != i) {
        swap(&tab[i], &tab[max]);
        Tamiser(tab, n, max);
    }
}

void TriParTas(int *tab, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        Tamiser(tab, n, i);
    }
    for (int i = n - 1; i > 0; i--) {
        swap(&tab[0], &tab[i]);
        Tamiser(tab, i, 0);
    }
}

// --- Benchmarking Logic ---

int main() {
    srand(time(NULL));
    FILE *f = fopen("resultats.csv", "w");
    if (f == NULL) return 1;

    fprintf(f, "Algorithm,N,Time,Comparisons\n");

    int sizes[] = {100, 500, 1000, 2000, 5000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        int *origin = malloc(n * sizeof(int));
        int *test_tab = malloc(n * sizeof(int));
        fill_random(origin, n);

        // Define function pointers for simple API
        struct { char *name; void (*func)(int*, int); } algos[] = {
            {"TriBulle", TriBulle},
            {"TriBulleOpt", TriBulleOpt},
            {"TriGnaque", TriGnaque},
            {"TriParTas", TriParTas}
        };

        for (int a = 0; a < 4; a++) {
            double total_time = 0;
            long long total_comps = 0;
            for (int i = 0; i < 10; i++) {
                copy_array(origin, test_tab, n);
                counts = 0;
                clock_t start = clock();
                algos[a].func(test_tab, n);
                clock_t end = clock();
                total_time += (double)(end - start) / CLOCKS_PER_SEC;
                total_comps += counts;
            }
            fprintf(f, "%s,%d,%.6f,%lld\n", algos[a].name, n, total_time / 10, total_comps / 10);
        }

        // Quick Sort (special case for range arguments)
        double qt = 0; long long qc = 0;
        for (int i = 0; i < 10; i++) {
            copy_array(origin, test_tab, n);
            counts = 0;
            clock_t start = clock();
            triRapide(test_tab, 0, n - 1);
            clock_t end = clock();
            qt += (double)(end - start) / CLOCKS_PER_SEC;
            qc += counts;
        }
        fprintf(f, "TriRapide,%d,%.6f,%lld\n", n, qt / 10, qc / 10);

        // Radix (TriDistribution)
        double dt = 0; long long dc = 0;
        for (int i = 0; i < 10; i++) {
            copy_array(origin, test_tab, n);
            counts = 0;
            clock_t start = clock();
            TriDistribution(test_tab, n, 4); // max 4 digits (0-9999)
            clock_t end = clock();
            dt += (double)(end - start) / CLOCKS_PER_SEC;
            dc += counts;
        }
        fprintf(f, "TriDistribution,%d,%.6f,%lld\n", n, dt / 10, dc / 10);

        free(origin);
        free(test_tab);
        printf("Finished N = %d\n", n);
    }

    fclose(f);
    printf("Results saved to resultats.csv\n");
    return 0;
}
