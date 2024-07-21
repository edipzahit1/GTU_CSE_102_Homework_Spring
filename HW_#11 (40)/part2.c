#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct Hospital {
    char* name;
    char* citiesServed[3];
};

bool isCityCovered(char* city, char* coveredCities[], int coveredCount) {
    for (int i = 0; i < coveredCount; i++) {
        if (strcmp(city, coveredCities[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool canCoverAllCitiesHelper(char* cities[], int cityCount, struct Hospital hospitals[], int hospitalCount, int maxHospitals, char* coveredCities[], int coveredCount, int usedHospitals) {
    if (usedHospitals > maxHospitals) return false;

    // Check if all cities are covered
    bool allCovered = true;
    for (int i = 0; i < cityCount; i++) {
        if (!isCityCovered(cities[i], coveredCities, coveredCount)) {
            allCovered = false;
            break;
        }
    }
    if (allCovered) return true;

    // Try adding each hospital and recurse
    for (int i = 0; i < hospitalCount; i++) {
        char* newCoveredCities[100];
        int newCoveredCount = coveredCount;
        memcpy(newCoveredCities, coveredCities, coveredCount * sizeof(char*));

        for (int j = 0; j < 3; j++) {
            if (!isCityCovered(hospitals[i].citiesServed[j], newCoveredCities, newCoveredCount)) {
                newCoveredCities[newCoveredCount++] = hospitals[i].citiesServed[j];
            }
        }

        if (canCoverAllCitiesHelper(cities, cityCount, hospitals, hospitalCount, maxHospitals, newCoveredCities, newCoveredCount, usedHospitals + 1)) {
            return true;
        }
    }

    return false;
}

bool canCoverAllCities(char* cities[], int cityCount, struct Hospital hospitals[], int hospitalCount, int maxHospitals) {
    char* coveredCities[100];
    return canCoverAllCitiesHelper(cities, cityCount, hospitals, hospitalCount, maxHospitals, coveredCities, 0, 0);
}

int main() {
    char* cities[] = { "A", "B", "C", "D", "E", "F" };
    struct Hospital hospitals[] = {
        { "H1", { "A", "B", "C" } },
        { "H2", { "A", "C", "D" } },
        { "H3", { "B", "F" } },
        { "H4", { "C", "E", "F" } }
    };
    int cityCount = 6;
    int hospitalCount = 4;
    int maxHospitals = 3;

    if (canCoverAllCities(cities, cityCount, hospitals, hospitalCount, maxHospitals)) {
        printf("All cities can be covered with the given number of hospitals.\n");
    } else {
        printf("Not all cities can be covered with the given number of hospitals.\n");
    }

    return 0;
}
