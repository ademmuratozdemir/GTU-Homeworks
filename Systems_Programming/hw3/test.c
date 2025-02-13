#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#define MAX_AUTOMOBILE 8
#define MAX_PICKUP 4
#define TOTAL_VEHICLES 50

// Semaphores
sem_t newAutomobile; // Semaphore for new automobile
sem_t inChargeforAutomobile;  // Semaphore for automobile attendant
sem_t newPickup; // Semaphore for new pickup
sem_t inChargeforPickup; // Semaphore for pickup attendant
sem_t automobileSpots; // Semaphore for automobile spots
sem_t pickupSpots; // Semaphore for pickup spots
sem_t vehicleQueueSemaphore; // Semaphore for vehicle queue to take one car at a time
sem_t processedSemaphore; // Semaphore for processed vehicle count
sem_t singleVehicleEntry; // Semaphore to allow only one vehicle to be processed at a time

// Shared variables
int mFree_automobile = MAX_AUTOMOBILE;
int mFree_pickup = MAX_PICKUP;
int vehicleQueue[TOTAL_VEHICLES]; // Queue for vehicles
int vehicleIndex = 0; // Index for vehicle queue
int vehiclesProcessed = 0; // Count of processed vehicles

// Function declarations
void* carOwner();
void* carAttendant(void* arg);

int main() {
    pthread_t threads[4]; // 4 threads for 2 owners and 2 attendants

    // Initialize semaphores
    sem_init(&newAutomobile, 0, 0);
    sem_init(&inChargeforAutomobile, 0, 1);
    sem_init(&newPickup, 0, 0);
    sem_init(&inChargeforPickup, 0, 1);
    sem_init(&automobileSpots, 0, MAX_AUTOMOBILE);
    sem_init(&pickupSpots, 0, MAX_PICKUP);
    sem_init(&vehicleQueueSemaphore, 0, 1);
    sem_init(&processedSemaphore, 0, 1);
    sem_init(&singleVehicleEntry, 0, 1); // Initialize single vehicle entry semaphore

    // Initialize vehicle queue
    for (int i = 0; i < TOTAL_VEHICLES; i++) {
        vehicleQueue[i] = (rand() % 2) + 1; // 1: Automobile, 2: Pickup
    }

    // Create initial threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, carOwner, NULL);
    }
    for (int i = 2; i < 4; i++) {
        pthread_create(&threads[i], NULL, carAttendant, (void*)(intptr_t)(i - 1));
    }

    // Join threads
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&newAutomobile);
    sem_destroy(&inChargeforAutomobile);
    sem_destroy(&newPickup);
    sem_destroy(&inChargeforPickup);
    sem_destroy(&automobileSpots);
    sem_destroy(&pickupSpots);
    sem_destroy(&vehicleQueueSemaphore);
    sem_destroy(&processedSemaphore);
    sem_destroy(&singleVehicleEntry);

    return 0;
}

void* carOwner() {
    while (1) {
        int vehicleType;

        // Allow only one vehicle to be processed at a time
        sem_wait(&singleVehicleEntry);

        // Critical section to fetch the next vehicle type from the queue
        sem_wait(&vehicleQueueSemaphore);
        if (vehicleIndex >= TOTAL_VEHICLES) {
            sem_post(&vehicleQueueSemaphore);
            sem_post(&singleVehicleEntry);
            exit(0);
        }
        vehicleType = vehicleQueue[vehicleIndex];
        vehicleIndex++;
        sem_post(&vehicleQueueSemaphore);

        if (vehicleType == 1) { // Automobile
            sem_wait(&inChargeforAutomobile);
            if (sem_trywait(&automobileSpots) == 0) { // Check if there is a free spot
                printf("\nAutomobile owner arrived. Free automobile spots before parking: %d\n", mFree_automobile);
                mFree_automobile--;
                sem_post(&newAutomobile);
                printf("Automobile parked to the temporary park. Free automobile spots now: %d\n\n", mFree_automobile);
            } else {
                printf("An automobile arrived but no free spots for automobiles. Automobile owner left.\n");
            }
            sem_post(&inChargeforAutomobile);
        } else if (vehicleType == 2) { // Pickup
            sem_wait(&inChargeforPickup);
            if (sem_trywait(&pickupSpots) == 0) { // Check if there is a free spot
                printf("\nPickup owner arrived. Free pickup spots before parking: %d\n", mFree_pickup);
                mFree_pickup--;
                sem_post(&newPickup);
                printf("Pickup parked to the temporary park. Free pickup spots now: %d\n\n", mFree_pickup);
            } else {
                printf("A pickup arrived but no free spots for pickups. Pickup owner left.\n");
            }
            sem_post(&inChargeforPickup);
        }

        // Allow the next vehicle to be processed
        sem_post(&singleVehicleEntry);

        usleep(500000); // Simulate time between arrivals

        // Check if all vehicles have been processed
        sem_wait(&processedSemaphore);
        if (vehiclesProcessed >= TOTAL_VEHICLES) {
            sem_post(&processedSemaphore);
            exit(0);
        }
        sem_post(&processedSemaphore);
    }
}

void* carAttendant(void* arg) {
    int attendantType = (intptr_t)arg;

    while (1) {
        if (attendantType == 1) { // Automobile attendant
            sem_wait(&newAutomobile);
            sleep(1); // Simulate parking time
            sem_wait(&inChargeforAutomobile);
            mFree_automobile++;
            printf("\nAn automobile is taken from temporary park to the main parking lot. Free automobile spots now: %d\n", mFree_automobile);
            sem_post(&automobileSpots);
            sem_post(&inChargeforAutomobile);
        } else if (attendantType == 2) { // Pickup attendant
            sem_wait(&newPickup);
            sleep(1); // Simulate parking time
            sem_wait(&inChargeforPickup);
            mFree_pickup++;
            printf("\nA pickup is taken from temporary park to the main parking lot. Free pickup spots now: %d\n", mFree_pickup);
            sem_post(&pickupSpots);
            sem_post(&inChargeforPickup);
        }

        // Increment the processed vehicle count
        sem_wait(&processedSemaphore);
        vehiclesProcessed++;
        if (vehiclesProcessed >= TOTAL_VEHICLES) {
            sem_post(&processedSemaphore);
            exit(0);
        }
        sem_post(&processedSemaphore);
    }
}
