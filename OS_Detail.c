#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 100

// Fungsi untuk mengurutkan array (penting untuk SCAN/C-SCAN)
void sortArray(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// 1. FCFS - First Come First Served
void calculateFCFS(int req[], int n, int head) {
    int seek_count = 0;
    int current_head = head;
    int angka[n]; 

    printf("\n=== FCFS Policy ===\n");

    for (int i = 0; i < n; i++) {
        int dist = abs(req[i] - current_head);
        
        printf("%d. %d -> %d\n", i + 1, current_head, req[i]);
        printf("   Jarak: |%d - %d| = %d\n", req[i], current_head, dist);

        seek_count += dist;
        angka[i] = dist; 
        current_head = req[i];
    }
    
    printf("\nTotal Head Movement: ");
    for(int i = 0; i < n; i++) {
        printf("%d", angka[i]);
        if (i < n - 1) {
            printf(" + ");
        }
    }

    printf(" = %d\n", seek_count);
}

void calculateSSTF(int req[], int n, int head) {
    int seek_count = 0;
    int current_head = head;
    int visited[MAX] = {0}; 
    int count = 0;
    
    // Array untuk menyimpan history jarak per langkah
    int history_dist[MAX]; 

    printf("\n=== SSTF Policy (Step-by-Step Calculation) ===\n");

    while (count < n) {
        int min_dist = 100000;
        int index = -1;

        // Cari tetangga terdekat
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                int dist = abs(req[i] - current_head);
                if (dist < min_dist) {
                    min_dist = dist;
                    index = i;
                }
            }
        }

        visited[index] = 1;
        
        // Simpan jarak langkah ini ke history
        history_dist[count] = min_dist;

        // Tampilkan Step
        printf("%d. %d -> %d\n", count + 1, current_head, req[index]);
        printf("   Jarak: |%d - %d| = %d\n", req[index], current_head, min_dist);

        seek_count += min_dist;
        current_head = req[index];
        count++;
    }

    // Tampilkan Penjumlahan Total
    printf("\nTotal Head Movement: ");
    for(int i = 0; i < n; i++) {
        printf("%d", history_dist[i]);
        if (i < n - 1) printf(" + ");
    }
    printf(" = %d\n", seek_count);
}

void calculateSCAN(int req[], int n, int head, int disk_size) {
    int seek_count = 0;
    int current_head = head;
    int temp[MAX];
    int history_dist[MAX]; // Array penyimpan jarak
    int step = 0;          // Counter langkah global
    
    for(int i=0; i<n; i++) temp[i] = req[i];
    temp[n] = head;
    temp[n+1] = disk_size - 1; 
    int temp_n = n + 2;

    sortArray(temp, temp_n);

    printf("\n=== SCAN Policy (Step-by-Step Calculation) ===\n");

    int head_index;
    for(int i=0; i<temp_n; i++) {
        if(temp[i] == head) {
            head_index = i;
            break;
        }
    }

    // 1. Gerak ke Kanan (Nilai membesar)
    for (int i = head_index + 1; i < temp_n; i++) {
        int dist = abs(temp[i] - current_head);
        
        printf("%d. %d -> %d\n", step + 1, current_head, temp[i]);
        printf("   Jarak: |%d - %d| = %d\n", temp[i], current_head, dist);
        
        history_dist[step] = dist;
        seek_count += dist;
        current_head = temp[i];
        step++;
    }
    
    // 2. Balik Arah ke Kiri
    for (int i = head_index - 1; i >= 0; i--) {
        int dist = abs(temp[i] - current_head);
        
        printf("%d. %d -> %d\n", step + 1, current_head, temp[i]);
        printf("   Jarak: |%d - %d| = %d\n", temp[i], current_head, dist);
        
        history_dist[step] = dist;
        seek_count += dist;
        current_head = temp[i];
        step++;
    }

    // Tampilkan Penjumlahan Total
    printf("\nTotal Head Movement: ");
    for(int i = 0; i < step; i++) {
        printf("%d", history_dist[i]);
        if (i < step - 1) printf(" + ");
    }
    printf(" = %d\n", seek_count);
}

void calculateCSCAN(int req[], int n, int head, int disk_size) {
    int seek_count = 0;
    int current_head = head;
    int temp[MAX];
    int history_dist[MAX];
    int step = 0;
    
    for(int i=0; i<n; i++) temp[i] = req[i];
    temp[n] = head;
    temp[n+1] = 0;
    temp[n+2] = disk_size - 1;
    int temp_n = n + 3;

    sortArray(temp, temp_n);

    printf("\n=== C-SCAN Policy (Step-by-Step Calculation) ===\n");

    int head_index;
    for(int i=0; i<temp_n; i++) {
        if(temp[i] == head) {
            head_index = i;
            break;
        }
    }

    // 1. Ke Kanan sampai Ujung
    for (int i = head_index + 1; i < temp_n; i++) {
        int dist = abs(temp[i] - current_head);
        printf("%d. %d -> %d\n", step + 1, current_head, temp[i]);
        printf("   Jarak: |%d - %d| = %d\n", temp[i], current_head, dist);
        
        history_dist[step] = dist;
        seek_count += dist;
        current_head = temp[i];
        step++;
    }
    
    // 2. Lompat ke 0 (Circular Jump)
    int jump_dist = abs(temp[0] - current_head);
    printf("%d. %d -> %d (CIRCULAR JUMP)\n", step + 1, current_head, temp[0]);
    printf("   Jarak: |%d - %d| = %d\n", temp[0], current_head, jump_dist);
    
    history_dist[step] = jump_dist;
    seek_count += jump_dist;
    current_head = temp[0];
    step++;

    // 3. Lanjut dari 0 ke Kanan lagi
    for (int i = 1; i < head_index; i++) {
        int dist = abs(temp[i] - current_head);
        printf("%d. %d -> %d\n", step + 1, current_head, temp[i]);
        printf("   Jarak: |%d - %d| = %d\n", temp[i], current_head, dist);
        
        history_dist[step] = dist;
        seek_count += dist;
        current_head = temp[i];
        step++;
    }

    // Tampilkan Penjumlahan Total
    printf("\nTotal Head Movement: ");
    for(int i = 0; i < step; i++) {
        printf("%d", history_dist[i]);
        if (i < step - 1) printf(" + ");
    }
    printf(" = %d\n", seek_count);
}

int main() {
    int n, head, disk_size;
    int req[MAX];

    printf("--- Disk Scheduling Simulation ---\n");
    printf("Enter number of requests: ");
    scanf("%d", &n);

    printf("Enter request sequence (space separated): ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &req[i]);
    }

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter disk size (max track number, e.g. 200): ");
    scanf("%d", &disk_size);

    // Jalankan semua simulasi
    calculateFCFS(req, n, head);
    calculateSSTF(req, n, head);
    
    // Untuk SCAN asumsi arah = 1 (ke kanan/naik)
    calculateSCAN(req, n, head, disk_size);
    
    calculateCSCAN(req, n, head, disk_size);

    return 0;
}
