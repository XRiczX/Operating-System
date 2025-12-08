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

    printf("\n=== FCFS Policy ===\n");
    printf("Path: %d", head);

    for (int i = 0; i < n; i++) {
        seek_count += abs(req[i] - current_head);
        current_head = req[i];
        printf(" -> %d", current_head);
    }
    
    printf("\nTotal Head Movement: %d\n", seek_count);
    // Asumsi: Seek time berbanding lurus dengan movement. 
    // Jika ada parameter waktu per track (misal 5ms), kalikan di sini.
}

// 2. SSTF - Shortest Seek Time First
void calculateSSTF(int req[], int n, int head) {
    int seek_count = 0;
    int current_head = head;
    int visited[MAX] = {0}; // Penanda agar track tidak diproses 2x
    int count = 0;

    printf("\n=== SSTF Policy ===\n");
    printf("Path: %d", head);

    while (count < n) {
        int min_dist = 100000;
        int index = -1;

        // Cari tetangga terdekat yang belum dikunjungi
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
        seek_count += min_dist;
        current_head = req[index];
        printf(" -> %d", current_head);
        count++;
    }
    printf("\nTotal Head Movement: %d\n", seek_count);
}

// 3. SCAN (Elevator Algorithm)
void calculateSCAN(int req[], int n, int head, int disk_size, int direction) {
    int seek_count = 0;
    int current_head = head;
    int temp[MAX + 2]; // Array sementara (+2 untuk head dan batas disk)
    
    // Copy request ke temp
    for(int i=0; i<n; i++) temp[i] = req[i];
    
    // Tambahkan batas disk jika perlu (misal ujung 0 atau ujung max)
    temp[n] = head;
    int temp_n = n + 1;
    
    // Menambahkan batas ujung disk (0 atau disk_size-1) tergantung implementasi
    // Di sini kita asumsikan SCAN akan menabrak ujung akhir lalu balik
    temp[temp_n] = (direction == 1) ? disk_size - 1 : 0;
    temp_n++;

    sortArray(temp, temp_n);

    printf("\n=== SCAN Policy ===\n");
    printf("Path: %d", head);

    int head_index;
    for(int i=0; i<temp_n; i++) {
        if(temp[i] == head) {
            head_index = i;
            break;
        }
    }

    // Jika arah ke kanan (High)
    if (direction == 1) {
        for (int i = head_index + 1; i < temp_n; i++) {
            seek_count += abs(temp[i] - current_head);
            current_head = temp[i];
            printf(" -> %d", current_head);
        }
        // Balik arah ke kiri
        for (int i = head_index - 1; i >= 0; i--) {
            seek_count += abs(temp[i] - current_head);
            current_head = temp[i];
            printf(" -> %d", current_head);
        }
    } 
    // Jika arah ke kiri (Low) -- Opsional, kode ini default kanan dulu
    else {
         // Logika similar tapi mundur dulu baru maju
    }

    printf("\nTotal Head Movement: %d\n", seek_count);
}

// 4. C-SCAN (Circular SCAN)
void calculateCSCAN(int req[], int n, int head, int disk_size) {
    int seek_count = 0;
    int current_head = head;
    int temp[MAX + 3];
    
    // Copy dan tambah boundary (0 dan max-1)
    for(int i=0; i<n; i++) temp[i] = req[i];
    temp[n] = head;
    temp[n+1] = 0;
    temp[n+2] = disk_size - 1;
    int temp_n = n + 3;

    sortArray(temp, temp_n);

    printf("\n=== C-SCAN Policy ===\n");
    printf("Path: %d", head);

    int head_index;
    for(int i=0; i<temp_n; i++) {
        if(temp[i] == head) {
            head_index = i;
            break;
        }
    }

    // C-SCAN selalu satu arah (biasanya ke kanan/naik)
    // 1. Jalan ke kanan sampai ujung
    for (int i = head_index + 1; i < temp_n; i++) {
        seek_count += abs(temp[i] - current_head);
        current_head = temp[i];
        printf(" -> %d", current_head);
    }
    
    // 2. Lompat dari ujung kanan ke ujung kiri (0)
    // Note: Lompatan ini biasanya dihitung jaraknya, atau dianggap 0 tergantung dosen.
    // Di sini kita hitung jarak lompatannya (Total movement).
    seek_count += abs(temp[0] - current_head);
    current_head = temp[0];
    printf(" -> %d (JUMP)", current_head);

    // 3. Lanjut jalan ke kanan sampai sebelum head awal
    for (int i = 1; i < head_index; i++) {
        seek_count += abs(temp[i] - current_head);
        current_head = temp[i];
        printf(" -> %d", current_head);
    }

    printf("\nTotal Head Movement: %d\n", seek_count);
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
    calculateSCAN(req, n, head, disk_size, 1);
    
    calculateCSCAN(req, n, head, disk_size);

    return 0;
}
